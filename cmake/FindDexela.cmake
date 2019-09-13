# Try to find libdexela.so
#
# Defines
#
# DEXELA_DEFINTIONS - definitions
# DEXELA_FOUND - system has libdexela
# DEXELA_INCLUDE_DIRS - libdexela include directory
# DEXELA_LIBRARIES - dexela library

set(DEXELA_DEFINITIONS OS_LINUX)

find_package(PackageHandleStandardArgs)

find_path(DEXELA_INCLUDE_DIRS 
          NAMES dexela_api.h
          PATH_SUFFIXES dexela)
find_library(DEXELA_LIBRARIES dexela)

find_package_handle_standard_args(DEXELA DEFAULT_MSG DEXELA_LIBRARIES DEXELA_INCLUDE_DIRS)

mark_as_advanced(
    DEXELA_INCLUDE_DIRS
    DEXELA_LIBRARIES
    DEXELA_DEFINITIONS
)
