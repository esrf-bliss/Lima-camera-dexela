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
#include "DexelaSyncCtrlObj.h"
#include <NativeApi.h>

using namespace lima;
using namespace lima::Dexela;

static const double MIN_EXPO_TIME = 1/86.;  // 86Hz maximum speed with 4x4 bin

SyncCtrlObj::SyncCtrlObj()
{
}

SyncCtrlObj::~SyncCtrlObj()
{
}

bool SyncCtrlObj::checkTrigMode(TrigMode trig_mode)
{
  bool valid_mode = false;
  switch (trig_mode)
    {
    case IntTrig:
    case IntTrigMult:
    case ExtTrigMult:
    case ExtGate:
      valid_mode = true;
      break;

    default:
      valid_mode = false;
      break;
    }
  return valid_mode;
}

void SyncCtrlObj::setTrigMode(TrigMode trig_mode)
{
  DEB_MEMBER_FUNCT();

  ExposureTriggerSource trigMode;
  switch(trig_mode)
    {
    case ExtTrigMult: trigMode = Ext_neg_edge_trig;break;
    case ExtGate: trigMode = Ext_Duration_Trig;break;
    default:
      trigMode = Internal_Software;break;
    }
  if(SetTriggerSource(trigMode) != SUCCESS)
    THROW_HW_ERROR(Error) << "Problem setting trigger mode";
}

void SyncCtrlObj::getTrigMode(TrigMode& trig_mode)
{
  DEB_MEMBER_FUNCT();

  switch(GetTriggerSource())
    {
    case Ext_neg_edge_trig: trig_mode = ExtTrigMult;break;
    case Ext_Duration_Trig: trig_mode = ExtGate;break;
    default:
      trig_mode = IntTrig;break;
    }

  DEB_RETURN() << DEB_VAR1(trig_mode);
}

void SyncCtrlObj::setExpTime(double exp_time)
{
  DEB_MEMBER_FUNCT();

  if(SetExposureTime(float(exp_time * 1e3)) != SUCCESS)
    THROW_HW_ERROR(Error) << "Problem setting exposure time";
}
void SyncCtrlObj::getExpTime(double& exp_time)
{
  exp_time = GetExposureTime() / 1e3;
}

void SyncCtrlObj::setLatTime(double lat_time)
{
  //Not managed
}

void SyncCtrlObj::getLatTime(double& lat_time)
{
  lat_time = 0;		// Readout
}

void SyncCtrlObj::setNbHwFrames(int nb_frames)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(nb_frames);

  m_acq_nb_frames = nb_frames;
}

void SyncCtrlObj::getNbHwFrames(int& nb_frames)
{
  nb_frames = m_acq_nb_frames;
}

void SyncCtrlObj::getValidRanges(ValidRangesType& valid_ranges)
{
  valid_ranges.min_exp_time = MIN_EXPO_TIME;
  valid_ranges.max_exp_time = 120.;
  valid_ranges.min_lat_time = 0.;
  valid_ranges.max_lat_time = 0.;
}



