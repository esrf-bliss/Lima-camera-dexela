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
#PATH TO CHANGE IN OTHER COMPUTERS
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${COMMON_PATH}/../include/glib-2.0")
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${COMMON_PATH}/../lib/x86_64-linux-gnu/glib-2.0/include")
TARGET_INCLUDE_DIRECTORIES(limadexela PUBLIC "${CMAKE_SOURCE_DIR}/../../dexela_lib/libdexela/include")

#LIBRARY
FIND_LIBRARY(DEXELA_LIB dexela)

TARGET_LINK_LIBRARIES(limadexela limacore)
TARGET_LINK_LIBRARIES(limadexela ${DEXELA_LIB})

SET_TARGET_PROPERTIES(limadexela PROPERTIES VERSION ${dexela_vers} SOVERSION "1")

INSTALL(TARGETS limadexela LIBRARY DESTINATION lib)

IF(COMPILE_SIP)
    SET(NAME "dexela")
    SET(INCLUDES)
    FILE(GLOB sipfiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/sip" "${CMAKE_CURRENT_SOURCE_DIR}/sip/*.sip")
    FOREACH(sipfile ${sipfiles})
        SET(INCLUDES 
        "${INCLUDES} 
        %Include ${sipfile}")
    ENDFOREACH()

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/sip/limamodules.sip.in sip/limadexela.sip)
    SET(SIP_CONCAT_PARTS 1)
    SET(SIP_INCLUDES ${SIP_INCLUDES}
        "${CMAKE_SOURCE_DIR}/third-party/Processlib/sip"
        "${CMAKE_BINARY_DIR}/sip/core"
        "${CMAKE_SOURCE_DIR}/third-party/Processlib/tasks/sip"
        "${CMAKE_SOURCE_DIR}/common/sip"
        "${CMAKE_SOURCE_DIR}/hardware/sip"
        "${CMAKE_SOURCE_DIR}/control/sip"
        "${CMAKE_SOURCE_DIR}/control/software_operation/sip"
        "${CMAKE_CURRENT_SOURCE_DIR}/sip")
    set(SIP_DISABLE_FEATURES WITH_CONFIG)
    set(SIP_CHECK_EXC ON)
    ADD_SIP_PYTHON_MODULE(limadexela ${CMAKE_CURRENT_BINARY_DIR}/sip/limadexela.sip)
    TARGET_INCLUDE_DIRECTORIES(python_module_limadexela PRIVATE
                               ${PYTHON_INCLUDE_DIRS}
                               "${CMAKE_SOURCE_DIR}/sip"
                               "${CMAKE_SOURCE_DIR}/sip/core")
    TARGET_LINK_LIBRARIES(python_module_limadexela limadexela)

    INSTALL(FILES python/__init__.py DESTINATION "${PYTHON_SITE_PACKAGES_DIR}/Lima/Dexela")
ENDIF()
