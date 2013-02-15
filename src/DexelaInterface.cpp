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
#pragma warning(disable : 4290)
#include "DexelaInterface.h"
#include "DexelaDetInfoCtrlObj.h"
#include "DexelaSyncCtrlObj.h"
#include "DexelaBinCtrlObj.h"
#include "DexelaReconstructionCtrlObj.h"
#include <libconfig.h++>
#include <NativeApi.h>



using namespace lima;
using namespace lima::Dexela;

class Interface::_AcqThread : public Thread
{
  DEB_CLASS_NAMESPC(DebModCamera,"Interface","_AcqThread");
public:
  _AcqThread(Interface &i);
  virtual ~_AcqThread();

protected:
  virtual void threadFunction();

private:
  Interface& m_interface;
};

Interface::Interface(const std::string& dataBasePath,
		     const std::string& sensorName) :
  m_acq_desc(NULL),
  m_acq_event(NULL),
  m_acq_thread(NULL),
  m_wait_flag(true),
  m_quit(false),
  m_thread_running(false),
  m_sensor_desc(new SensorDesc())
{
  DEB_CONSTRUCTOR();
  std::string sensorFormat;
  //configure m_sensor_desc from configuration file
  _load_config_file(dataBasePath,sensorName,
		    *m_sensor_desc,sensorFormat);
  
  Derr error = openBoard((char*)sensorFormat.c_str(),&m_acq_desc);
  if(error != SUCCESS)
    THROW_HW_ERROR(Error) << "Could not open the acquisition board";

  DEB_ALWAYS() << GetTransportMethod();
  char version[32];
  GetFirmwareVersion(version,sizeof(version));
  if(error != SUCCESS)
    THROW_HW_ERROR(Error) << "Can't get firmware version";
  DEB_ALWAYS() << "Firmware :" << version;

  error = ::SetExposureTime(1 * 1000.);
  if(error != SUCCESS)
    THROW_HW_ERROR(Error) << "Can't set default exposure";

  error = ::SetFullWellMode(::High);
  if(error != SUCCESS)
    THROW_HW_ERROR(Error) << "Can't set the full well mode";

  error = ::SetBinningMode(x11);
  if(error != SUCCESS)
    THROW_HW_ERROR(Error) << "Can't set default binning";

  error = ::SetTriggerSource(Internal_Software);
  if(error != SUCCESS)
    THROW_HW_ERROR(Error) << "Can't set default trigger";

  error = ::SetExposureMode(Frame_Rate_exposure);
  if(error != SUCCESS)
    THROW_HW_ERROR(Error) << "Could not set exposure mode to Frame Rate";

  m_acq_event = CreateEvent(NULL,FALSE,FALSE,NULL);

  // TMP 16 Buffers
  m_tmp_buffer = _aligned_malloc(m_sensor_desc->imageBufferX * m_sensor_desc->imageBufferY * 16 * 2,16);
  if(!m_tmp_buffer)
    THROW_HW_ERROR(Error) << "Can't malloc tmp memory buffers";

  int detectorWidth = m_sensor_desc->sensorX * m_sensor_desc->sensorsH;
  int detectorHeight = m_sensor_desc->sensorY * m_sensor_desc->sensorsV;

  m_det_info = new DetInfoCtrlObj(sensorName,detectorHeight,detectorWidth);
  m_sync = new SyncCtrlObj();
  m_bin = new BinCtrlObj();
  m_reconstruction = new ReconstructionCtrlObj(*this);


  m_cap_list.push_back(HwCap(m_det_info));
  m_cap_list.push_back(HwCap(&m_buffer_ctrl_obj));
  m_cap_list.push_back(HwCap(m_sync));
  m_cap_list.push_back(HwCap(m_bin));
  m_cap_list.push_back(HwCap(m_reconstruction));

  m_acq_thread = new _AcqThread(*this);
  m_acq_thread->start();
}

Interface::~Interface()
{
  CloseBoard(m_acq_desc);
  delete m_det_info;
  delete m_sync;
  _aligned_free(m_tmp_buffer);
}

void Interface::getCapList(CapList &cap_list) const
{
  cap_list = m_cap_list;
}

void Interface::reset(ResetLevel reset_level)
{
}

void Interface::prepareAcq()
{
  DEB_MEMBER_FUNCT();
  m_acq_frame_nb = -1;
}

void Interface::startAcq()
{
  DEB_MEMBER_FUNCT();

  TrigMode trig_mode;
  m_sync->getTrigMode(trig_mode);
  bool waitStart = (trig_mode != IntTrig && 
		    trig_mode != IntTrigMult);

  StdBufferCbMgr& buffer_mgr = m_buffer_ctrl_obj.getBuffer();
  buffer_mgr.setStartTimestamp(Timestamp::now());
  AutoMutex aLock(m_cond.mutex());
  m_wait_flag = false;
  m_cond.broadcast();
  while(waitStart && !m_thread_running)
    m_cond.wait();
}

void Interface::stopAcq()
{
  DEB_MEMBER_FUNCT();

  AutoMutex aLock(m_cond.mutex());
  m_wait_flag = true;
  SetEvent(m_acq_event);
  
  while(m_thread_running)
    m_cond.wait();
}

void Interface::getStatus(StatusType &status)
{
  DEB_MEMBER_FUNCT();

  status.set(m_thread_running ? 
	     HwInterface::StatusType::Exposure : HwInterface::StatusType::Ready);

  DEB_RETURN() << DEB_VAR1(status);
}

int Interface::getNbHwAcquiredFrames()
{
  return m_acq_frame_nb + 1;
}

void Interface::setFullWellMode(Interface::wellMode mode)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(mode);

  ::FullWellModes raw_mode;
  switch(mode)
    {
    case Interface::Low: 	raw_mode = ::Low;break;
    case Interface::High: 	raw_mode = ::High;break;
    default:
      THROW_HW_ERROR(InvalidValue) << "WellMode can only be High or Low";break;
    }

  if(::SetFullWellMode(raw_mode) != SUCCESS)
    THROW_HW_ERROR(Error) << "Problem setting full well mode";
}

Interface::wellMode Interface::getFullWellMode() const
{
  DEB_MEMBER_FUNCT();

  ::FullWellModes raw_mode = ::GetFullWellMode();
  Interface::wellMode mode;
  switch(raw_mode)
    {
    case ::Low: 	mode = Interface::Low;break;
    case ::High: 	mode = Interface::High;break;
    default:
      THROW_HW_ERROR(Error) << "Problem getting full well mode";
    }
  return mode;
}
/*============================================================================
			   Static Methodes
============================================================================*/
void Interface::_load_config_file(const std::string& dataBasePath,
				  const std::string& sensorName,
				  SensorDesc& sensor_desc,
				  std::string& sensorFormat)
{
  DEB_STATIC_FUNCT();

  libconfig::Config config;
  try
    {
      config.readFile(dataBasePath.c_str());
    }
  catch(libconfig::ParseException &exp)
    {
      THROW_HW_ERROR(Error) << exp.getError() 
			    << " (" << exp.getFile()
			    << ":" << exp.getLine() << ")";
    }
  catch(libconfig::FileIOException &exp)
    {
      THROW_CTL_ERROR(Error) << exp.what();
    }

  libconfig::Setting& root = config.getRoot();
  int nbSensor = root.getLength();
  bool found = false;
  for(int i = 0;!found && i < nbSensor;++i)
    {
      const libconfig::Setting &sensorConfig = root[i];
      if(sensorConfig.isGroup() && sensorName == sensorConfig.getName())
	{
	  found = true;

	  if(!sensorConfig.lookupValue("numSensors",sensor_desc.numSensors))
	    THROW_HW_ERROR(Error) << "Field numSensors must be in detector configuration";

	  if(!sensorConfig.lookupValue("sensorStrips",sensor_desc.sensorStrips))
	    THROW_HW_ERROR(Error) << "Field sensorStrips must be in detector configuration";
    
	  if(!sensorConfig.lookupValue("stripLength",sensor_desc.stripLength))
	    THROW_HW_ERROR(Error) << "Field stripLength must be in detector configuration";

	  if(!sensorConfig.lookupValue("sensorX",sensor_desc.sensorX))
	    THROW_HW_ERROR(Error) << "Field sensorX must be in detector configuration";
	  
	  if(!sensorConfig.lookupValue("sensorY",sensor_desc.sensorY))
	    THROW_HW_ERROR(Error) << "Field sensorY must be in detector configuration";
	  
	  if(!sensorConfig.lookupValue("sensorsH",sensor_desc.sensorsH))
	    THROW_HW_ERROR(Error) << "Field sensorsH must be in detector configuration";

	  if(!sensorConfig.lookupValue("sensorsV",sensor_desc.sensorsV))
	    THROW_HW_ERROR(Error) << "Field sensorsV must be in detector configuration";

	  if(!sensorConfig.lookupValue("sensorFormat",sensorFormat))
	    THROW_HW_ERROR(Error) << "Field sensorFormat must be in detector configuration";
	  
	  sensor_desc.imageBufferX = sensor_desc.sensorX * sensor_desc.sensorsH;
	  sensor_desc.imageBufferY = sensor_desc.sensorY * sensor_desc.sensorsV;
	}
    }
  if(!found)
    THROW_HW_ERROR(InvalidValue) << DEB_VAR1(sensorName) << 
      " not found in config " << DEB_VAR1(dataBasePath);
}
/*----------------------------------------------------------------------------
			     Thread class
----------------------------------------------------------------------------*/
Interface::_AcqThread::_AcqThread(Interface &i) :
  m_interface(i)
{
}

Interface::_AcqThread::~_AcqThread()
{
  AutoMutex aLock(m_interface.m_cond.mutex());
  m_interface.m_quit = true;
  SetEvent(m_interface.m_acq_event);
  m_interface.m_cond.broadcast();
}

void Interface::_AcqThread::threadFunction()
{
  DEB_MEMBER_FUNCT();

  HANDLE sync[] = {m_interface.m_acq_event,m_interface.m_acq_desc};
  AutoMutex aLock(m_interface.m_cond.mutex());
  StdBufferCbMgr& buffer_mgr = m_interface.m_buffer_ctrl_obj.getBuffer();

  while(!m_interface.m_quit)
    {
      while(m_interface.m_wait_flag && !m_interface.m_quit)
	{
	  DEB_TRACE() << "Wait";
	  m_interface.m_thread_running = false;
	  m_interface.m_cond.broadcast();
	  m_interface.m_cond.wait();
	}
      DEB_TRACE() << "Run";

      m_interface.m_thread_running = true;
      if(m_interface.m_quit) return;

      int raw_buffer_size = imageXdim() * imageYdim();
      int nb_frames_to_acq;
      m_interface.m_sync->getNbHwFrames(nb_frames_to_acq);

      int bufferId = 1;
      bool continueFlag = true;
      int intRetCode = Snap(1,bufferId,1);
      m_interface.m_cond.broadcast();
      aLock.unlock();

      do
	{
	  DWORD ret = WaitForMultipleObjects(sizeof(sync)/sizeof(HANDLE),
					     sync,FALSE,INFINITE);
	  switch(ret)
	    {
	    case WAIT_OBJECT_0 + 0:
	      DEB_TRACE() << "Event signal";
	      aLock.lock();
	      continueFlag = !m_interface.m_wait_flag && !m_interface.m_quit;
	      aLock.unlock();
	      break;
	    case WAIT_OBJECT_0 + 1:
	      {
		int capBuf = GetCapturedBuffer();
		if(++bufferId > 16) bufferId = 1;
		aLock.lock();
		++m_interface.m_acq_frame_nb;
		int frame_number = m_interface.m_acq_frame_nb;
		aLock.unlock();
		if((nb_frames_to_acq - 1) > frame_number)
		  Snap(1,bufferId,1);
		else
		  continueFlag = false;

		byte* pMem = (byte*)m_interface.m_tmp_buffer;
		pMem += (capBuf - 1) * raw_buffer_size;
		int readSize = ReadBufferUchar(1,capBuf,0,0,-1,-1,
					       pMem,raw_buffer_size,"Grey");
		if(readSize == raw_buffer_size)
		  {
		    HwFrameInfoType frame_info;
		    frame_info.acq_frame_nb = frame_number;
		    continueFlag = buffer_mgr.newFrameReady(frame_info) && continueFlag;
		  }
		else
		  {
		    DEB_ERROR() << "Acquisition Error:" << DEB_VAR2(readSize,raw_buffer_size);
		    continueFlag = false;
		  }
	      }
	      break;
	    default:
	      DEB_ERROR() << "Snap failed";
	      continueFlag = false;
	      break;
	    }
	}
      while(continueFlag);
      m_interface.m_wait_flag = true;
    }
}
