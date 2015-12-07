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
#include "DexelaBinCtrlObj.h"
#include <dexela/dexela_api.h>

using namespace lima;
using namespace lima::Dexela;

BinCtrlObj::BinCtrlObj()
{
}

BinCtrlObj::~BinCtrlObj()
{
}

void BinCtrlObj::setBin(const Bin& bin)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(bin);

  int x = bin.getX();
  int y = bin.getY();
  if(dexela_set_binning_mode(y,x))
    THROW_HW_ERROR(Error) << "Set binning failed";
}

void BinCtrlObj::getBin(Bin& bin)
{
  DEB_MEMBER_FUNCT();
  
  bin = Bin(dexela_get_binning_mode_vertical(),
	    dexela_get_binning_mode_horizontal());

  DEB_RETURN() << DEB_VAR1(bin);
}

void BinCtrlObj::checkBin(Bin& bin)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(bin);

  int x = bin.getX();
  int y = bin.getY();
  if(x == 3) x = 2;
  else if(x > 4) x = 4;

  if(y == 3) y = 2;
  else if(y > 4) y = 4;
  
  int minVal = std::min(x,y);
  x = y = minVal;		// binning must be square
  bin = Bin(x,y);
  
  DEB_RETURN() << DEB_VAR1(bin);
}
