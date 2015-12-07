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
#include "DexelaReconstructionCtrlObj.h"
#include "processlib/LinkTask.h"
#include <dexela/dexela_api.h>

using namespace lima;
using namespace lima::Dexela;

const static int NB_HARD_BUFFER = 16;

class ReconstructionCtrlObj::_Task : public LinkTask
{
public:
  _Task(ReconstructionCtrlObj& rec) : m_rec(rec) 
  {
  }
  Data process(Data &aData)
  {
    return m_rec.reconstruct(aData);
  }
private:
  ReconstructionCtrlObj& m_rec;
};

ReconstructionCtrlObj::ReconstructionCtrlObj(Interface& i) :
  m_interface(i)
{
  m_rec_task = new _Task(*this);
  gchar* model = dexela_get_model();
  m_model = atoi(model);
  g_free(model);
}

ReconstructionCtrlObj::~ReconstructionCtrlObj()
{
  m_rec_task->unref();
}

LinkTask* ReconstructionCtrlObj::getReconstructionTask()
{
  return m_rec_task;
}

Data ReconstructionCtrlObj::reconstruct(Data& src)
{
  unsigned nb_buffer = sizeof(m_interface.m_tmp_buffers) / sizeof(void*);
  dexela_descramble((ushort*)m_interface.m_tmp_buffers[src.frameNumber % nb_buffer],
		    (ushort*)src.data(),m_model);
  return src;
}
