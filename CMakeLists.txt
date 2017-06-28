###########################################################################
# This file is part of LImA, a Library for Image Acquisition
#
#  Copyright (C) : 2009-2017
#  European Synchrotron Radiation Facility
#  CS40220 38043 Grenoble Cedex 9 
#  FRANCE
# 
#  Contact: lima@esrf.fr
# 
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
# 
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################
set(NAME "dexela")

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--add-needed")

#SOURCEs
set(${NAME}_srcs src/DexelaInterface.cpp src/DexelaDetInfoCtrlObj.cpp 
	src/DexelaSyncCtrlObj.cpp src/DexelaBinCtrlObj.cpp
	src/DexelaReconstructionCtrlObj.cpp)
	 
add_definitions(-DOS_LINUX)

add_library(lima${NAME} SHARED ${${NAME}_srcs})

#INCLUDEs
target_include_directories(lima${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(lima${NAME} PUBLIC "/usr/local/include/dexela")
target_include_directories(lima${NAME} PUBLIC "/usr/include/glib-2.0")
target_include_directories(lima${NAME} PUBLIC "/usr/lib/x86_64-linux-gnu/glib-2.0/include")
target_include_directories(lima${NAME} PUBLIC "${CMAKE_SOURCE_DIR}/../../dexela_lib/libdexela/include")

#LIBRARY
find_library(DEXELA_LIB dexela)
if (("${DEXELA_LIB}" STREQUAL "DEXELA_LIB-NOTFOUND"))
	message(FATAL_ERROR "${NAME} : libdexela not found, you can search it in cmake-gui or try to install it if it isn't here.")
endif()
target_link_libraries(lima${NAME} limacore)
target_link_libraries(lima${NAME} ${DEXELA_LIB})

include(CameraVERSION)
install(TARGETS lima${NAME} LIBRARY DESTINATION lib)

if(LIMA_ENABLE_PYTHON)
    include(CameraRunSIP)
    install(FILES python/__init__.py DESTINATION "${PYTHON_SITE_PACKAGES_DIR}/Lima/Dexela")
endif()
