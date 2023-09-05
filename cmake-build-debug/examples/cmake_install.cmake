# Install script for directory: /Users/ben/Documents/GitHub/parflow/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/examples" TYPE FILE FILES
    "/Users/ben/Documents/GitHub/parflow/examples/default_single.tcl"
    "/Users/ben/Documents/GitHub/parflow/examples/pftest.tcl"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/examples/correct_output" TYPE FILE FILES
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.concen.0.00.00000.pfsb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.concen.0.00.00001.pfsb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.concen.0.00.00002.pfsb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.concen.0.00.00003.pfsb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.concen.0.00.00004.pfsb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.concen.0.00.00005.pfsb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.perm_x.pfb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.perm_y.pfb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.perm_z.pfb"
    "/Users/ben/Documents/GitHub/parflow/examples/correct_output/default_single.out.press.00000.pfb"
    )
endif()

