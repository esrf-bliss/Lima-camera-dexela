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
#ifndef DEXELAINTERFACE_H
#define DEXELAINTERFACE_H
#include "Debug.h"
#include "DexelaCompatibility.h"
#include "HwInterface.h"

typedef struct SensorDescription SensorDesc;

namespace lima
{
  namespace Dexela
  {
    class DetInfoCtrlObj;
    class SyncCtrlObj;
    class BinCtrlObj;
    class ReconstructionCtrlObj;

    class LIBDEXELA_API Interface : public HwInterface
    {
      DEB_CLASS_NAMESPC(DebModCamera, "Interface", "Dexela");
      friend class ReconstructionCtrlObj;
      class _AcqThread;
    public:
      enum wellMode {Low,High};
      
      Interface(const std::string& databasePath,
		const std::string& sensorFromat);
      virtual ~Interface();

      virtual void getCapList(CapList &) const;
      
      virtual void reset(ResetLevel reset_level);
      virtual void prepareAcq();
      virtual void startAcq();
      virtual void stopAcq();
      virtual void getStatus(StatusType& status);
      
      virtual int getNbHwAcquiredFrames();

      //Specific
      void setFullWellMode(wellMode);
      wellMode getFullWellMode() const;

      
    private:
      void _load_config_file(const std::string&,
			     const std::string&,
			     SensorDesc&,std::string&);

      HANDLE 			m_acq_desc;
      HANDLE			m_acq_event;
      Thread*			m_acq_thread;
      long			dummy0;
      Cond			m_cond;
      long			dummy1;
      volatile bool		m_wait_flag;
      volatile bool		m_quit;
      volatile bool		m_thread_running;
      DetInfoCtrlObj* 		m_det_info;
      SyncCtrlObj*		m_sync;
      BinCtrlObj*		m_bin;
      ReconstructionCtrlObj* 	m_reconstruction;
      SoftBufferCtrlObj 	m_buffer_ctrl_obj;
      int			m_acq_frame_nb;
      CapList			m_cap_list;
      void*			m_tmp_buffer;
      SensorDesc*		m_sensor_desc;
    };
  }
}
#endif
