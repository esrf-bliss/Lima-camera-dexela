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
#include "DexelaDetInfoCtrlObj.h"
#include "DexelaInterface.h"
#include <NativeApi.h>

using namespace lima;
using namespace lima::Dexela;
DetInfoCtrlObj::DetInfoCtrlObj(const std::string& detectorModel,
			       int max_rows,int max_columns) :
  m_detector_model(detectorModel),
  m_max_rows(max_rows),
  m_max_columns(max_columns)
{
}

DetInfoCtrlObj::~DetInfoCtrlObj()
{
}

void DetInfoCtrlObj::getMaxImageSize(Size& max_image_size)
{
  max_image_size = Size(m_max_columns,m_max_rows);
}

void DetInfoCtrlObj::getDetectorImageSize(Size& det_image_size)
{
  DEB_MEMBER_FUNCT();

  getMaxImageSize(det_image_size);

  DEB_RETURN() << DEB_VAR1(det_image_size);
}

void DetInfoCtrlObj::getDefImageType(ImageType& def_image_type)
{
  def_image_type = Bpp16;
}

void DetInfoCtrlObj::getCurrImageType(ImageType& curr_image_type)
{
  curr_image_type = Bpp16;
}

void DetInfoCtrlObj::setCurrImageType(ImageType  curr_image_type)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(curr_image_type);

  if(curr_image_type != Bpp16)
    THROW_HW_ERROR(Error) << "Only support Bpp16";
}

void DetInfoCtrlObj::getPixelSize(double& x_size,double &y_size)
{
  x_size = y_size = 74.8e-6;
}

void DetInfoCtrlObj::getDetectorType(std::string& det_type)
{
  det_type = "Dexela";
}

void DetInfoCtrlObj::getDetectorModel(std::string& det_model)
{
  det_model = m_detector_model;
}

void DetInfoCtrlObj::registerMaxImageSizeCallback(HwMaxImageSizeCallback& cb)
{
  m_mis_cb_gen.registerMaxImageSizeCallback(cb);
}

void DetInfoCtrlObj::unregisterMaxImageSizeCallback(HwMaxImageSizeCallback& cb)
{
  m_mis_cb_gen.unregisterMaxImageSizeCallback(cb);
}
