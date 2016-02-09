//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <sys/signalfd.h>
#include "DexelaInterface.h"
#include "DexelaDetInfoCtrlObj.h"
#include "DexelaSyncCtrlObj.h"
#include "DexelaBinCtrlObj.h"
#include "DexelaReconstructionCtrlObj.h"
#include <dexela/dexela_api.h>
#include <xclib/xcliball.h>


static const int RESERVED = 0;
static const int SINGLE_UNIT = 0x1;

using namespace lima;
using namespace lima::Dexela;

class Interface::_AcqThread : public Thread
{
  DEB_CLASS_NAMESPC(DebModCamera,"Interface","_AcqThread");
public:
  _AcqThread(Interface &i);
  virtual ~_AcqThread();

  void signal();

protected:
  virtual void threadFunction();

private:
  Interface&	m_interface;
  bool		m_stop;
};

Interface::Interface(const std::string& formatFile) :
  m_acq_thread(NULL),
  m_skip_first_frame(true)
{
  DEB_CONSTRUCTOR();
  std::string sensorFormat;
  
  gboolean error_flag = dexela_open_detector(formatFile.c_str());
  if(!error_flag)
    THROW_HW_ERROR(Error) << "Could not open the acquisition board";

  guint error = dexela_init_serial_connection();
  if(error)
    THROW_HW_ERROR(Error) << "Can't init serial connection";
  //Info
  gchar* model = dexela_get_model();
  gchar* serial_number = dexela_get_serial_number();
  gchar* firmware = dexela_get_firmware_version();
  DEB_ALWAYS() << DEB_VAR3(model,serial_number,firmware);
  g_free(model),g_free(serial_number),g_free(firmware);

  DEB_ALWAYS() << DEB_VAR1(pxd_imageZdim());

  error = dexela_set_exposure_time_micros(1 * 1e6);
  if(error)
    THROW_HW_ERROR(Error) << "Can't set default exposure";

  error = dexela_set_binning_mode(1,1);
  if(error)
    THROW_HW_ERROR(Error) << "Can't set default binning";

  error = dexela_set_trigger_mode(::SOFTWARE);
  if(error)
    THROW_HW_ERROR(Error) << "Can't set default trigger";

  // TMP 16 Buffers
  int imageByteCount = pxd_imageXdim() * pxd_imageYdim();
  for(unsigned i = 0; i < sizeof(m_tmp_buffers)/sizeof(void*);++i)
    {
      if(posix_memalign(&m_tmp_buffers[i],16,imageByteCount))
	THROW_HW_ERROR(Error) << "Can't malloc tmp memory buffers";
      memset(m_tmp_buffers[i],0,imageByteCount);
    }

  error = dexela_power_on_sensor();
  if(error)
    THROW_HW_ERROR(Error) << "Can't power on detector";

  m_det_info = new DetInfoCtrlObj();
  int model_type = m_det_info->getModel();
  m_sync = new SyncCtrlObj();
  m_sync->setModel(model_type);
  m_bin = new BinCtrlObj();
  m_reconstruction = new ReconstructionCtrlObj(*this);


  m_cap_list.push_back(HwCap(m_det_info));
  m_cap_list.push_back(HwCap(&m_buffer_ctrl_obj));
  m_cap_list.push_back(HwCap(m_sync));
  m_cap_list.push_back(HwCap(m_bin));
  m_cap_list.push_back(HwCap(m_reconstruction));

  // Synchronization pipe
  if(pipe(m_synchro_pipe))
    THROW_HW_ERROR(Error) << "Can't open synchronization pipe";

  //Signal file descriptor
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask,SIGUSR1);
  if(sigprocmask(SIG_BLOCK,&mask,NULL) < 0)
    {
      char str_errno[1024];
      strerror_r(errno,str_errno,sizeof(str_errno));
      THROW_HW_ERROR(Error) << "Can't block signal: " << str_errno;
    }
  m_signal_fd = signalfd(-1,&mask,0);
  if(m_signal_fd < 0)
    {
      char str_errno[1024];
      strerror_r(errno,str_errno,sizeof(str_errno));
      THROW_HW_ERROR(Error) << "Can't create signal fd: " << str_errno;
    }
      
  m_acq_thread = new _AcqThread(*this);
  m_acq_thread->start();
}

Interface::~Interface()
{
  dexela_close_detector();
  delete m_acq_thread;
  delete m_det_info;
  delete m_sync;
  delete m_bin;
  delete m_reconstruction;
  for(unsigned i = 0; i < sizeof(m_tmp_buffers)/sizeof(void*);++i)
    free(m_tmp_buffers[i]);
  close(m_signal_fd);
}

void Interface::getCapList(CapList &cap_list) const
{
  cap_list = m_cap_list;
}

void Interface::reset(ResetLevel reset_level)
{
  DEB_MEMBER_FUNCT();
  dexela_power_off_sensor();
  dexela_power_on_sensor();
}

void Interface::prepareAcq()
{
  DEB_MEMBER_FUNCT();
  AutoMutex aLock(m_cond.mutex());
  m_acq_frame_nb = -1;
   
  m_acq_thread->signal();
}

void Interface::startAcq()
{
  DEB_MEMBER_FUNCT();

  TrigMode trig_mode;
  m_sync->getTrigMode(trig_mode);

  StdBufferCbMgr& buffer_mgr = m_buffer_ctrl_obj.getBuffer();
  buffer_mgr.setStartTimestamp(Timestamp::now());

  int nb_frames_to_acq;
  m_sync->getNbHwFrames(nb_frames_to_acq);
  double lat_time;
  m_sync->getLatTime(lat_time);

  gint error = dexela_set_exposure_mode(::NORMAL,
					nb_frames_to_acq,
					int(lat_time * 1e6));
  if(error)
    THROW_HW_ERROR(Error) << "Could not set exposure mode to Frame Rate";

  error = pxd_goLive(SINGLE_UNIT,1);
  if(error < 0)
    THROW_HW_ERROR(Error) << "Can't go Snap" << DEB_VAR1(pxd_mesgErrorCode(error));
  
  if(trig_mode == IntTrig && 
     trig_mode == IntTrigMult)
    {
      pxd_setCameraLinkCCOut(0x1, 0x1);
      pxd_setCameraLinkCCOut(0x1, 0x0);
    }
}

void Interface::stopAcq()
{
  DEB_MEMBER_FUNCT();
  
  int error = pxd_goAbortLive(SINGLE_UNIT);
  if(error)
    THROW_HW_ERROR(Error) << "Unable to stop acquisition"
			  << DEB_VAR1(pxd_mesgErrorCode(error));
}

void Interface::getStatus(StatusType &status)
{
  DEB_MEMBER_FUNCT();

  char error_msg[1024];
  if(pxd_mesgFaultText(SINGLE_UNIT, error_msg, sizeof(error_msg)))
    DEB_ERROR() << DEB_VAR1(error_msg);

  status.set(HwInterface::StatusType::Ready);

  DEB_RETURN() << DEB_VAR1(status);
}

int Interface::getNbHwAcquiredFrames()
{
  AutoMutex lock(m_cond.mutex());
  return m_acq_frame_nb + 1;
}

void Interface::setFullWellMode(Interface::wellMode mode)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(mode);

  THROW_HW_ERROR(NotSupported) << "Not implemented yet ;)";
}

Interface::wellMode Interface::getFullWellMode() const
{
  DEB_MEMBER_FUNCT();
  THROW_HW_ERROR(NotSupported) << "Not implemented yet ;)";

  Interface::wellMode mode;
  return mode;
}
/*----------------------------------------------------------------------------
			     Thread class
----------------------------------------------------------------------------*/
Interface::_AcqThread::_AcqThread(Interface &i) :
  m_interface(i),m_stop(false)
{
}

Interface::_AcqThread::~_AcqThread()
{
  AutoMutex aLock(m_interface.m_cond.mutex());
  m_stop = true;
  signal();
}

void Interface::_AcqThread::signal()
{
  write(m_interface.m_synchro_pipe[1],"|",1);
}

void Interface::_AcqThread::threadFunction()
{
  DEB_MEMBER_FUNCT();

  if(pxd_eventCapturedFieldCreate(SINGLE_UNIT,SIGUSR1,RESERVED))
    THROW_HW_ERROR(Error) << "Can't set signal on capture";

  StdBufferCbMgr& buffer_mgr = m_interface.m_buffer_ctrl_obj.getBuffer();

  int imageByteCount = pxd_imageXdim() * pxd_imageYdim();
  bool skip_first_frame = false;
  bool continue_flag = true;
  unsigned nb_buffer = sizeof(m_interface.m_tmp_buffers) / sizeof(void*);
  int nb_frames_to_acq = 0;
  struct pollfd fds[2];
  fds[0].fd = m_interface.m_synchro_pipe[0];
  fds[0].events = POLLIN;
  fds[1].fd = m_interface.m_signal_fd;
  fds[1].events = POLLIN;

  while(continue_flag)
    {
      poll(fds,2,-1);

      if(fds[1].revents)
	{
	  struct signalfd_siginfo si;
	  ssize_t res = read(fds[1].fd,&si,sizeof(si));
	  if(res == -1)
	    {
	      char str_errno[1024];
	      strerror_r(errno,str_errno,sizeof(str_errno));
 	      DEB_WARNING() << "Something strange happen!" << str_errno;
	      continue;
	    }

	  // skip the first frame
	  if(skip_first_frame)
	    {
	      skip_first_frame = false;
	      continue;
	    }

	  AutoMutex lock(m_interface.m_cond.mutex());
	  int frame_id = ++m_interface.m_acq_frame_nb;
	  lock.unlock();
	  if(frame_id >= nb_frames_to_acq) continue;

	  pxbuffer_t buffer_id = pxd_capturedBuffer(SINGLE_UNIT);
	  pxd_readuchar(SINGLE_UNIT, buffer_id,
			0,0,-1,-1,
			(uchar*)m_interface.m_tmp_buffers[frame_id % nb_buffer],
			imageByteCount,
			(char*)"Grey");
	  if(frame_id == nb_frames_to_acq -1)
	    pxd_goAbortLive(SINGLE_UNIT);

	  HwFrameInfoType frame_info;
	  frame_info.acq_frame_nb = frame_id;
	  bool continueFlag = buffer_mgr.newFrameReady(frame_info);
	  if(!continueFlag) nb_frames_to_acq = 0;
	}
      if(fds[0].revents)
	{
	  char event_buffer[1024];
	  read(m_interface.m_synchro_pipe[0],event_buffer,sizeof(event_buffer));

	  m_interface.m_sync->getNbHwFrames(nb_frames_to_acq);
	  TrigMode trig_mode;
	  m_interface.m_sync->getTrigMode(trig_mode);
	  skip_first_frame = trig_mode == ExtTrigReadout && m_interface.m_skip_first_frame;
	  AutoMutex lock(m_interface.m_cond.mutex());
	  continue_flag = !m_stop;
	}
    }
}
