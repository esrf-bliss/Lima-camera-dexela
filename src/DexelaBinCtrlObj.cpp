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
#include <NativeApi.h>

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

  bins hwBin;
  switch(x)
    {
    case 1:
      switch(y)
	{
	case 1: hwBin = x11;break;
	case 2: hwBin = x12;break;
	case 4: hwBin = x14;break;
	default:
	  THROW_HW_ERROR(InvalidValue) << "Hardware bin can be 1,2 or 4";
	}
      break;
    case 2:
      switch(y)
	{
	case 1: hwBin = x21;break;
	case 2: hwBin = x22;break;
	case 4: hwBin = x24;break;
	default:
	  THROW_HW_ERROR(InvalidValue) << "Hardware bin can be 1,2 or 4";
	}
      break;
    case 4:
      switch(y)
	{
	case 1: hwBin = x41;break;
	case 2: hwBin = x42;break;
	case 4: hwBin = x44;break;
	default:
	  THROW_HW_ERROR(InvalidValue) << "Hardware bin can be 1,2 or 4";
	}
      break;
    default:
      THROW_HW_ERROR(InvalidValue) << "Hardware bin can be 1,2 or 4";
    }

  ::SetBinningMode(hwBin);
}

void BinCtrlObj::getBin(Bin& bin)
{
  DEB_MEMBER_FUNCT();

  switch(GetBinningMode())
    {
    case x11: bin = Bin(1,1);break;
    case x12: bin = Bin(1,2);break;
    case x14: bin = Bin(1,4);break;
    case x21: bin = Bin(2,1);break;
    case x22: bin = Bin(2,2);break;
    case x24: bin = Bin(2,4);break;
    case x41: bin = Bin(4,1);break;
    case x42: bin = Bin(4,2);break;
    case x44: bin = Bin(4,4);break;
    default:
      THROW_HW_ERROR(Error) << "Bin error";
      break;
    }

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
  
  bin = Bin(x,y);
  
  DEB_RETURN() << DEB_VAR1(bin);
}
