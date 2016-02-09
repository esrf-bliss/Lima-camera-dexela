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
#include <dexela/dexela_api.h>

using namespace lima;
using namespace lima::Dexela;

static const double MIN_EXPO_TIME = 1/86.;  // 86Hz maximum speed with 4x4 bin

SyncCtrlObj::SyncCtrlObj() : m_model(-1)
{
  DEB_CONSTRUCTOR();

  m_paused_shutter_mode_available = dexela_is_paused_shutter_mode_available();
  if(!m_paused_shutter_mode_available)
    DEB_WARNING() << "Shutter paused mode is not available,"
		  << "this will cause trigger jitters. "
		  << "Update the camera firmware";
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
      valid_mode = true;
      break;
    case ExtTrigMult:
    case ExtGate:
      valid_mode = !m_paused_shutter_mode_available;
      break;
    case ExtTrigReadout:
      valid_mode = m_paused_shutter_mode_available;
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
  
  TrigMode previous_trig_mode;
  getTrigMode(previous_trig_mode);

  ::TriggerMode trigMode;
  switch(trig_mode)
    {
    case ExtTrigReadout:
    case ExtTrigMult:	trigMode = ::EDGE;	break;
    case ExtGate:	trigMode = ::DURATION;	break;
    default:
			trigMode = ::SOFTWARE;	break;
    }
  if(trig_mode == ExtTrigReadout)
    {
      if(dexela_set_paused_shutter_mode(::PAUSED))
	THROW_HW_ERROR(Error) << "Can't activate paused shutter mode";
      dexela_set_exposure_time_micros(0);
    }
  else if(m_paused_shutter_mode_available)
    dexela_set_paused_shutter_mode(::CONTINUOUS);

  if(dexela_set_trigger_mode(trigMode))
    THROW_HW_ERROR(Error) << "Problem setting trigger mode";

  if(previous_trig_mode == ExtTrigReadout)
    setExpTime(m_exp_time);
}

void SyncCtrlObj::getTrigMode(TrigMode& trig_mode)
{
  DEB_MEMBER_FUNCT();

  switch(dexela_get_trigger_mode())
    {
    case ::EDGE:
      trig_mode = m_paused_shutter_mode_available ?
	ExtTrigReadout : ExtTrigMult;
      break;
    case ::DURATION:	trig_mode = ExtGate;		break;
    default:
			trig_mode = IntTrig;		break;
    }

  DEB_RETURN() << DEB_VAR1(trig_mode);
}

void SyncCtrlObj::setExpTime(double exp_time)
{
  DEB_MEMBER_FUNCT();

  m_exp_time = exp_time;
  
  TrigMode trig_mode;
  getTrigMode(trig_mode);
  /* Don't set the exposure time in ExtTrigReadout
     because this mode is link with the fastest detector
     exposure time.
  */
  if(trig_mode == ExtTrigReadout)
    return;

  int bin = dexela_get_binning_mode_vertical();
  double readout;
  switch(m_model)
    {
    case 1207:
    case 2307:
      switch(bin)
	{
	case 1: readout = 1/60.;break;
	case 2: readout = 1/156.;break;
	case 4: readout = 1/191.;break;
	}
      break;
    case 1512:
    case 2315:
    case 2923:
      switch(bin)
	{
	case 1: readout = 1/26.;break;
	case 2: readout = 1/70.;break;
	case 4: readout = 1/86.;break;
	}
      break;
    default:
      THROW_HW_ERROR(Error) << "model wasn't set or not managed yet: "
			    << DEB_VAR1(m_model);
    }
  exp_time -= readout;
  if(exp_time < 0) exp_time = 0.;
    
  if(dexela_set_exposure_time_micros(gint(exp_time * 1e6)))
    THROW_HW_ERROR(Error) << "Problem setting exposure time";
}
void SyncCtrlObj::getExpTime(double& exp_time)
{
  exp_time = dexela_get_exposure_time_micros()  / 1e6;
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

void SyncCtrlObj::setModel(int model)
{
  m_model = model;
}


