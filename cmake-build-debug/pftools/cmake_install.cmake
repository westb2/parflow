# Install script for directory: /Users/ben/Documents/GitHub/parflow/pftools

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/libpftools.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libpftools.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libpftools.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libpftools.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libpftools.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/libpftools.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpftools.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpftools.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpftools.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpftools.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES
    "/Users/ben/Documents/GitHub/parflow/pftools/dirt.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/evap.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/fsb.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/fsbox.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/metaEListBox.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/metaEntry.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/parflow.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/pflocked.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/pftformat.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfvtk.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfwrite.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/pkgIndex.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/tkdiff.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/winsync.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xparflow.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpfci.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpfmpeg.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftdatadsp.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftdiffdsp.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftfunctions.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftfuncwin.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftgeneral.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftgriddsp.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpfthelp.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpfthreeslices.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftinfowin.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftnewgrid.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftools.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftpf.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftsds.tcl"
    "/Users/ben/Documents/GitHub/parflow/pftools/xpftstatdsp.tcl"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "/Users/ben/Documents/GitHub/parflow/pftools/pfhelp"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfmvio"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfbtosa"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfbtovis"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfbtosilo"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfsbtosa"
    "/Users/ben/Documents/GitHub/parflow/pftools/pfstrip"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/pfwell_cat")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_cat" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_cat")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_cat")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_cat")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/pfwell_data")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_data" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_data")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_data")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfwell_data")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/gmssol2pfsol")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmssol2pfsol" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmssol2pfsol")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmssol2pfsol")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmssol2pfsol")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/gmstinvertices")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstinvertices" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstinvertices")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstinvertices")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstinvertices")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/gmstriangulate")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstriangulate" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstriangulate")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstriangulate")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/gmstriangulate")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/projecttin")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/projecttin" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/projecttin")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/projecttin")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/projecttin")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/pfmask-to-pfsol")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmask-to-pfsol" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmask-to-pfsol")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools"
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmask-to-pfsol")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmask-to-pfsol")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/pfsol-to-vtk")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfsol-to-vtk" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfsol-to-vtk")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfsol-to-vtk")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfsol-to-vtk")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/pfmaskdownsize")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmaskdownsize" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmaskdownsize")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ben/Documents/GitHub/parflow/cmake-build-debug/${PARFLOW_DIR}/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmaskdownsize")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pfmaskdownsize")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/ben/Documents/GitHub/parflow/cmake-build-debug/pftools/test/cmake_install.cmake")
endif()

