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
#include "lima/Debug.h"
#include "DexelaCompatibility.h"
#include "lima/HwInterface.h"

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
      
      Interface(const std::string& formatFile);
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

      void setSkipFirstFrame(bool flag) {m_skip_first_frame = flag;};
      bool getSkipFirstFrame() const {return m_skip_first_frame;};

    private:
      _AcqThread*		m_acq_thread;
      long			dummy0;
      Cond			m_cond;
      long			dummy1;
      DetInfoCtrlObj* 		m_det_info;
      SyncCtrlObj*		m_sync;
      BinCtrlObj*		m_bin;
      ReconstructionCtrlObj* 	m_reconstruction;
      SoftBufferCtrlObj 	m_buffer_ctrl_obj;
      int			m_acq_frame_nb;
      CapList			m_cap_list;
      void*			m_tmp_buffers[16];
      int			m_synchro_pipe[2];
      int			m_signal_fd;
      bool			m_skip_first_frame;
    };
  }
}
#endif
