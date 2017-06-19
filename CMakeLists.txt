############################################################################
# This file is part of LImA, a Library for Image Acquisition
#
# Copyright (C) : 2009-2017
# European Synchrotron Radiation Facility
# BP 220, Grenoble 38043
# FRANCE
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################
# CMakeLists.txt for Dexela camera

SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--add-needed")

#SOURCEs
SET(dexela_srcs src/DexelaInterface.cpp src/DexelaDetInfoCtrlObj.cpp 
	src/DexelaSyncCtrlObj.cpp src/DexelaBinCtrlObj.cpp
	src/DexelaReconstructionCtrlObj.cpp)
	 
FILE(STRINGS "VERSION" dexela_vers)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX")

ADD_LIBRARY(limadexela SHARED ${dexela_srcs})

#INCLUDEs
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${COMMON_PATH}/include/dexela")
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${COMMON_PATH}/../include/glib-2.0")
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${COMMON_PATH}/../lib/x86_64-linux-gnu/glib-2.0/include")
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${CMAKE_SOURCE_DIR}/../../dexela_lib/libdexela/include")

#LIBRARY
FIND_LIBRARY(DEXELA_LIB dexela)
IF (("${DEXELA_LIB}" STREQUAL "DEXELA_LIB-NOTFOUND"))
	MESSAGE(FATAL_ERROR "dexela : libdexela not found, you can search it in cmake-gui or try to install it if it isn't here.")
ENDIF()
TARGET_LINK_LIBRARIES(limadexela limacore)
TARGET_LINK_LIBRARIES(limadexela ${DEXELA_LIB})

SET_TARGET_PROPERTIES(limadexela PROPERTIES VERSION ${dexela_vers} SOVERSION "1")

INSTALL(TARGETS limadexela LIBRARY DESTINATION lib)

IF(COMPILE_SIP)
    SET(NAME "dexela")
    include(CameraRunSIP)
    INSTALL(FILES python/__init__.py DESTINATION "${PYTHON_SITE_PACKAGES_DIR}/Lima/Dexela")
ENDIF()
