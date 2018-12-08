|image|

*Suggested citation:* Maxwell, R.M., S.J. Kollet, S.G. Smith, C.S.
Woodward, R.D. Falgout, I.M. Ferguson, N. Engdahl, L.E. Condon, B.
Hector, S.R. Lopez, J. Gilbert, L. Bearup, J. Jefferson, C. Collins, I.
de Graaf, C. Prubilick, C. Baldwin, W.J. Bosl, R. Hornung, S. Ashby,
ParFlow User’s Manual. Integrated GroundWater Modeling Center Report
GWMI 2016-01, p.

ParFlow is released under the GNU LPGL License Version 1.3, 3 November
2008 Copyright © 2000, 2001, 2002, 2007, 2008 Free Software Foundation,
Inc.http://fsf.org/This manual is licensed under the GNU Free
Documentation License.© 2014 Reed M. Maxwell, Stefan J. Kollet, Ian M.
Ferguson, Steven G. Smith, Carol S. Woodward, Nicholas Engdahl, Laura E.
Condon. Permission is granted to copy, distribute and/or modify this
document under the terms of the GNU Free Documentation License, Version
1.3 or any later version published by the Free Software Foundation; with
no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts. A
copy of the license is included in the section entitled “GNU Free
Documentation License”. Permission is granted to make and distribute
verbatim copies of this manual provided the copyright notice and this
permission notice are preserved on all copies.

This computer software and documentation was prepared as an account of
work sponsored by an agency of the United States Government. Neither the
United States Government nor the University of California nor any of
their employees, makes any warranty, express or implied, or assumes any
legal liability or responsibility for the accuracy, completeness, or
usefulness of any information, apparatus, product, or process disclosed,
or represents that its use would not infringe privately owned rights.
Reference herein to any specific commercial products, process, or
service by trade name, trademark, manufacturer, or otherwise, does not
necessarily constitute or imply its endorsement, recommendation, or
favoring by the United States Government or the University of
California. The views and opinions of authors expressed herein do not
necessarily state or reflect those of the United States Government or
the University of California, and shall not be used for advertising or
product endorsement purposes.

Introduction
============

ParFlow (*PARallel FLOW*) is an integrated hydrology model that
simulates surface and subsurface flow. ParFlow is a parallel simulation
platform that operates in three modes:

#. steady-state saturated;

#. variably saturated;

#. and integrated-watershed flow.

ParFlow is especially suitable for large scale problems on a range of
single and multi-processor computing platforms. ParFlow simulates
saturated and variably saturated subsurface flow in heterogeneous porous
media in three spatial dimensions using a mulitgrid-preconditioned
conjugate gradient solver and a Newton-Krylov nonlinear solver . ParFlow
has recently been extended to coupled surface-subsurface flow to enable
the simulation of hillslope runoff and channel routing in a truly
integrated fashion . ParFlow is also fully-coupled with the land surface
model CLM as described in . The development and application of ParFlow
has been on-going for more than 20 years and resulted in some of the
most advanced numerical solvers and multigrid preconditioners for
massively parallel computer environments that are available today. Many
of the numerical tools developed within the ParFlow platform have been
turned into or are from libraries that are now distributed and
maintained at LLNL (*Hypre* and *SUNDIALS*, for example). An additional
advantage of ParFlow is the use of a sophisticated octree-space
partitioning algorithm to depict complex structures in three-space, such
as topography, different hydrologic facies, and watershed boundaries.
All these components implemented into ParFlow enable large scale, high
resolution watershed simulations.

ParFlow is primarily written in *C*, uses a modular architecture and
contains a flexible communications layer to encapsulate parallel process
interaction on a range of platforms. CLM is fully-integrated into
ParFlow as a module and has been parallelized (including I/O) and is
written in *FORTRAN 90/95*. ParFlow is organized into a main executable
*pfdir*/pfsimulator/parflow:sub:`e`\ xe and a library
*pfdir*/pfsimulator/parflow\_lib (where *pfdir* is the main directory
location) and is comprised of more than 190 separate source files.
ParFlow is structured to allow it to be called from within another
application (*e.g.* WRF, the Weather Research and Forecasting
atmospheric model) or as a stand-alone application. There is also a
directory structure for the message-passing layer
*pfdir*/pfsimulator/amps for the associated tools *pfdir*/pftools for
CLM *pfdir*/pfsimulator/clm and a directory of test cases *pfdir*/test.

How to use this manual
----------------------

This manual describes how to use ParFlow, and is intended for
hydrologists, geoscientists, environmental scientists and engineers.
This manual is written assuming the reader has a basic understanding of
Linux / UNIX environments, how to compose and execute scripts in various
programming languages (e.g. TCL), and is familiar with groundwater and
surface water hydrology, parallel computing, and numerical modeling in
general. In Chapter [Getting Started], we describe how to install
ParFlow, including building the code and associated libraries. Then, we
lead the user through a simple ParFlow run and discuss the automated
test suite. In Chapter [The ParFlow System], we describe the ParFlow
system in more detail. This chapter contains a lot of useful information
regarding how a run is constructed and most importantly contains two
detailed, annotated scripts that run two classical ParFlow problems, a
fully saturated, heterogeneous aquifer and a variably saturated,
transient, coupled watershed. Both test cases are published in the
literature and are a terrific initial starting point for a new ParFlow
user.

Chapter [Manipulating Data] describes data analysis and processing.
Chapter [Model\ :sub:`E`\ quations] provides the basic equations solved
by ParFlow. Chapter [ParFlow Files] describes the formats of the various
files used by ParFlow. These chapters are really intended to be used as
reference material. This manual provides some overview of ParFlow some
information on building the code, examples of scripts that solve certain
classes of problems and a compendium of keys that are set for code
options.

Published Studies That Have Used ParFlow
----------------------------------------

ParFlow has been used in a number of research studies published in the
literature. What follows are tables of ParFlow references with
information on topics, types of problem and application. [pfref1] to
[pfref6] describe any coupled physics, categorize the scale and domain
and discuss what processes within ParFlow are used. For this last set of
columns, **TB** = Turning Bands, **TFG** = Terrain Following Grid,
**VS** = Variably Saturated and **Vdz** = Variable DZ.

 l p1.5cm p2cm p1.5cm p1.5cm \| c \| c \| c \| c **Reference & **Coupled
Model & **Application & **Scale & **Domain & **TB & **TFG & **VS & **Vdz
 Bearup et al. (2016) & & Hillslope Hydrologic Response; MPB & Hillslope
& Idealized & & & X &
 Maxwell et al. (2016) & & Residence Time Distributions & Continental &
CONUS & & & X & X
 Reyes at al. (2015) & CLM & Surface Heterogeneity, Surface Energy
Budget (SEB) & Urban Watershed Ballona Creek Watershed, CA & & X&X &X
 Condon and Maxwell (2015) & & Subsurface Heterogeneity (groundwater
fluxes and topography) & Continental & CONUS & & & X & X
 Jefferson et al. (2015) & CLM & Active subspaces; Dimension reduction;
Energy fluxes & Hillslope & Idealized & & & &
 Jefferson and Maxwell (2015)& CLM & Sensitivity Analysis (evaporation
parameterization) & Column & Idealized & & & &
 Rihani et al. (2015) & ARPS, CLM & Land-atmosphere feedbacks &
Hillslope & Idealized & & X & X &
 Condon et al. (2015) & & Subsurface Heterogeneity (groundwater fluxes
and topography) & Continental & CONUS & &X &X & X
 Maxwell et al. (2015)& & Model Development (continental-scale) &
Continental & CONUS & &X &X & X
 Beisman et al. (2015)& CrunchFlow & Parallel Reactive Transport;
Subsurface Nutrient Cycling & Floodplain& Idealized & & & &
 Seck et al. (2015) & CLM & Spin-up behavior; Initial Conditions
Watershed & Regional& Monocacy, MD and PA & & & X &
 Bhaskar et al. (2015)& CLM & Urban Processes & Regional & Baltimore, MD
& & & X &
******************

[pfref1]

 l p1.5cm p2cm p1.5cm p1.5cm \| c \| c \| c \| c **Reference & **Coupled
Model & **Application & **Scale & **Domain & **TB & **TFG & **VS & **Vdz
******************

|  Engdahl and Maxwell (2015) & & Residence Time Distributions &
Watershed& East Inlet watershed, CO & X& &X &
|  Srivastava et al. (2014)& CLM & Global Sensitivity Analysis &
Watershed & Sante Fe River Basin, FL & & & X &
|  Kollet (2015) & CLM & Entropy Production & Hillslope & Idealized & &
& X &
|  Rahman et al. (2015)& TerrSysMP & Aquifer-to-atmosphere & Regional &
Rur Catchment, Germany & & & X &
|  Fang et al. (2015) & CLM & Soil Moisture Dynamics & Catchment &
Wüstebach catchment & & & X &
|  Shrestha et al. (2015) & TerrSysMP & Grid Resolution; Surface Energy
Fluxes & Catchment & Rür River sub-catchment & & & X &
|  Rahman et al. (2015) & CLM & Dual-boundary Forcing Concept &
Catchment & Rur Catchment, Germany & & & X &
|  Koch et al. (2016) &CLM Model Comparison (HydroGeoSphere, MIKE SHE) &
Catchment& Wüstebach catchment & & & X &
|  Ajami et al. (2015)& CLM& Initial Conditions& Catchment& Skjern River
basin in Denmark & & & X &

[pfref2]

 l p1.5cm p2cm p1.5cm p1.5cm \| c \| c \| c \| c **Reference & **Coupled
Model & **Application & **Scale & **Domain & **TB & **TFG & **VS & **Vdz
******************

|  Barnes et al. (2015)& Slope Processing Watershed & DR5, Gwynn Falls,
Baltimore, MD & Urban subwatershed & DR5, Baltimore, MD & &X & X &
|  Ajami et al. (2014) & CLM & Spin Up (initial conditions) & Watershed
& Ringkobing Fjord & & & X &
|  Condon and Maxwell (2014) & CLM & Agriculture & Watershed & Little
Washita, OK & & X & X &
|  Condon and Maxwell (2014) & CLM & Agriculture & Watershed & Little
Washita, OK & & X& X &
|  Cui et al. (2014) & SLIM-FAST & Model Development (nitrogen
biogeochemistry) & Column-Hillslope & Idealized & X & & X &
|  Maxwell et al. (2014) & Model Comparison & Many & Idealized & & & X &
X &
|  Meyerhoff et al. (2014) & - & Stochastic runoff generation,
conditioning & Hillslope & Idealized & X & & X &
|  Meyerhoff et al. (2014) & SLIM-FAST & Karst Environments & Aquifer &
Transects in Santa Fe River Watershed & & X & &
|  Shrestha et al. (2014) & COSMO-CLM & Model Development (TerrSysMP) &
Watershed & Idealized; Rur catchment & & X & X &
|  Atchley et al. (2013) & SLIM-FAST; CrunchFlow & Risk Assessment &
Aquifer & Idealized & X & & &
|  Atchley et al. (2013) & SLIM-FAST; CrunchFlow & Risk Assessment &
Aquifer & Idealized & & X & X &
|  Condon and Maxwell (2013) & CLM & Agriculture & Sub-Watershed &
Little Washita, OK & & X & X &
|  Condon et al. (2013) & CLM & Subsurface Heterogeneity (land surface
fluxes) & Watershed & Upper Klamath, OR & X & X & X &
|  de Rooij et al. (2013) & SLIM-FAST & Model Development (surface
particles) & Hillslope & Idealized & & & X &

[pfref3]

 l p1.5cm p2cm p1.5cm p1.5cm \| c \| c \| c \| c **Reference & **Coupled
Model & **Application & **Scale & **Domain & **TB & **TFG & **VS & **Vdz
******************

|  Maxwell (2013) & Model Development & Idealized & & & & X & X & X
|  Mikkelson et al. (2013) & CLM & Mountain Pine Beetle & Hillslope &
Idealized & & X & X &
|  Williams et al. (2013) & WRF & Atmosphere, DART, Data Assimilation &
Watershed & Idealized & X & & X &
|  Bürger et al. (2012) & ParFlow Web & Model Development (ParFlow Web)&
- & - & & & X &
|  Ferguson and Maxwell (2012) & CLM & Agriculture & Watershed & Little
Washita, OK & & & X &
|  Siirila et al. (2012) & SLIM-FAST & Risk Assessment & Aquifer &
Idealized & X & & &
|  Siirila and Maxwell (2012) & SLIM-FAST & Risk Assessment & Aquifer &
Idealized & X & & &
|  Siirila and Maxwell (2012) & SLIM-FAST & Risk Assessment & Aquifer &
Idealized & X & & &
|  Atchley and Maxwell (2011) & CLM & Subsurface Heterogeneity (land
surface processes) & Hillslope & Golden, CO & X & & X &
|  Daniels et al. (2011) & - & Regional & Streamflow & Owens Valley, CA
floodplain & & & X &
|  Ferguson and Maxwell (2011) & CLM & Agriculture & Watershed & Little
Washita, OK & & & X &
|  Maxwell et al. (2011) & WRF & Atmosphere & Watershed & Little
Washita, OK & & & X &

[pfref4]

 l p1.5cm p2cm p1.5cm p1.5cm \| c \| c \| c \| c **Reference & **Coupled
Model & **Application & **Scale & **Domain & **TB & **TFG & **VS & **Vdz
******************

|  Meyerhoff and Maxwell (2011) & - & Subsurface Heterogeneity (runoff
generation) & Hillslope & Idealized & X & & X &
|  Williams and Maxwell (2011) & WRF & Atmosphere & Watershed &
Idealized & X & & X &
|  Ferguson and Maxwell (2010) & CLM & Agriculture & Watershed & Little
Washita, OK & & & X &
|  Kollet et al. (2010) & CLM & Computational Scaling & Hillslope &
Idealized & X & & X &
|  Maxwell (2010) & CLM & Subsurface Heterogeneity (infiltration) &
Hillslope & Rainer Mesa (Nevada Test Site) & X & & X &
|  Rihani et al. (2010) & CLM & Subsurface Heterogeneity (land energy
fluxes) & Hillslope & Idealized & & & X &
|  Sulis et al. (2010) & - & Model Comparison (CATHY) & Hillslope &
Idealized & & & X &
|  Frei et al. (2009) & - & Groundwater-Surface water exchange &
Catchment & Consumnes River & X & & X &
|  Kollet et al. (2009) & CLM & Heat Transport (ParFlowE) & Column &
Wagineng, NL & & & X &
|  Kollet (2009) & CLM & Subsurface Heterogeneity (evapotranspiration) &
Column, Hillslope & Idealized & X & & X &

[pfref5]

 l p1.5cm p2cm p1.5cm p1.5cm \| c \| c \| c \| c **Reference & **Coupled
Model & **Application & **Scale & **Domain & **TB & **TFG & **VS & **Vdz
******************

|  Maxwell et al. (2009) & SLIM & Subsurface Transport & Hillslope &
Nevada Test Site & & & X &
|  Kollet and Maxwell (2008) & SLIM-FAST & Residence Time Distributions
& Watershed & Little Washita, OK & & & X &
|  Kollet and Maxwell (2008) & CLM & Subsurface Heterogeneity (land
energy fluxes) & Watershed & Little Washita, OK & & & X &
|  Maxwell and Kollet (2008) & - & Subsurface Heterogeneity (runoff) &
Hillslope & Idealized & X & & X &
|  Maxwell and Kollet (2008) & CLM & Climate Change (land-energy
feedbacks to groudnwater) & Watershed & Little Washita, OK & & & X &
|  Maxwell et al. (2007) & particles & Subsurface Transport & Aquifer &
Cape Cod, MA & X & & &
|  Maxwell et al. (2007) & ARPS, CLM & Model Development (ARPS) &
Watershed & Little Washita, OK & & & X &
|  Kollet and Maxwell (2006) & - & Model Development (Overland Flow) &
Subsurface Heterogeneity (shallow overland flow) & Catchment & Idealized
& X & & X &
|  Maxwell and Miller (2005) & CLM & Model Development (CLM) & Column &
Valdai, Russia & & & X &
|  Tompson et al. (2005) & - & Subsurface Transport & Aquifer & Nevada
Test Site & & & &
|  Maxwell et al. (2003) & - & Subsurface Transport & Aquifer & Forebay
Aquifer & & & &
|  Tompson et al. (1999) & - & Subsurface Transport & Aquifer & Nevada
Test Site & & & &
|  Tompson et al. (1998) & - & Subsurface Transport & Aquifer & LLNL
Site & & & &

[pfref6]

Getting Started
===============

This chapter is an introduction to setting up and running ParFlow. In
§ [Installing ParFlow], we describe how to install ParFlow. In
§ [Running the Sample Problem], we lead the user through a simple
groundwater problem, supplied with the ParFlow distribution. In
§ [ParFlow Solvers] we describe the solver options available for use
with ParFlow applications.

Installing ParFlow
------------------

ParFlow is distributed as source code only and must be configured and
built (compiled) on each machine on which you would like to run
simulations. ParFlow uses a configure/make system based on the standard
GNU autoconf configure system. This replaces the home grown set of
scripts used in previous versions and is much more portable to a range
of machines. Though we will cover some basic guidelines for the
installation of ParFlow here, many tips and tricks for building ParFlow
on a range of systems may be found at the ParFlow blog:
http://parflow.blogspot.com

For greater portability the ParFlow build process separates
configuration and compilation of the simulator and associated tools.
This separation allows easier porting to platforms where the
architecture is different on the nodes and the front-end.

These instructions are for building ParFlow on a range of serial and
parallel linux, unix and OSX machines, including stand-alone single and
multi-core to large parallel clusters. These instructions do *NOT*
include compilation on Windows machines.

ParFlow requires a Standard *ANSI C* and *FORTRAN 90/95* compiler to
build code. Many versions of C and Fortran are compatible with ParFlow
(e.g. Intel or IBM). However, GCC and gFortran, available for free on
almost every platform, are good options. They may be found at:

http://gcc.gnu.org/

and

http://gcc.gnu.org/wiki/GFortran

ParFlow also requires TCL/TK version 8.0 (or higher). TCL/TK can be
obtained from:

http://www.tcl.tk/

These three packages are often pre-installed on most computers and
generally do not need to be installed by the user. The following steps
are designed to take you through the process of installing ParFlow from
a source distribution. ParFlow uses the gnu package autoconf to create a
configuration file for building and installing the ParFlow program.

#. | **Setup**

   We will use the  /pfdir directory as the root directory for the
   source, build and install directory in this user manual; you can use
   a different directory if you wish.

   Decide where you wish to install Parflow and associated libraries.
   The following environment variable should be set up in your .profile,
   .cshrc, or other file. Set the environment variable
   PARFLOW\ :sub:`D`\ IR to your chosen location (if you are using bash
   or a bourne syntax shell):

   ::

       export PARFLOW_DIR=~/pfdir/install

   If you are using a csh like shell you will need the following in your
   .cshrc file:

   ::

       setenv PARFLOW_DIR ~/pfdir/install

#. | **Extract the source**
   | Extract the source files from the distribution compressed tar file
   or by cloning the repository from the ParFlow github repository. This
   example assumes the parflow.tar.Z file is in your home directory and
   you are building it in a directory  /parflow.

   ::

       mkdir ~/pfdir
       cd ~/pfdir
       tar -xvf ../parflow.tar.Z

#. | **Build and install ParFlow** with CMake
   | This step builds the ParFlow and associated PFTools excutables. A
   ParFlow library is also built which can be used when ParFlow is used
   as a component of another simulation (*e.g.* WRF).

   CMake can be invoked using a command line only version cmake or
   terminal based GUI ccmake. Most of the example below will use the
   command line version since it is easier to directly show the command.
   The ccmake version will show all the variables you may set in an
   interactive GUI.

   The following commands configure and build a sequenital version of
   ParFlow. You can control build options for ParFlow in the CMake
   configure step by adding other options to that command-line. Note we
   build in a seperate directory from the source. This keeps the source
   directory free of files created during the build. Going back to a
   clean state can be done by removing the build directory and starting
   over. This is fairly common CMake practice.

   Here we only set the directory to install in the CMake command.

   ::

       cd ~/pfdir
       mkdir build
       cd build
       cmake ../parflow -DCMAKE_INSTALL_PREFIX=$(PARFLOW_DIR) 
       make 
       make install

   For a list of *all* the CMake configure options, the easist method is
   to use the ccmake command:

   ::

       ccmake ../parflow  -DCMAKE_INSTALL_PREFIX=$(PARFLOW_DIR)

   ParFlow defaults to building a sequential version; setting the
   PARFLOW\ :sub:`A`\ MPS\ :sub:`L`\ AYER variable is required to build
   a parallel version. The easist way to use the MPI version to use is
   to ensure that the mpicc command is in your path and set the
   PARFLOW\ :sub:`A`\ MPS\ :sub:`L`\ AYER to “mpi1”. The
   PARFLOW\ :sub:`A`\ MPS\ :sub:`L`\ AYER variable controls *AMPS* which
   stands for *A*\ nother *M*\ essage *P*\ assing *S*\ ytem. *AMPS* is a
   flexible message-passing adapter layer within ParFlow that allows a
   common code core to be quickly and easily adapted to different
   parallel environments.

   Here is an example of the simplist MPI configuration:

   ::

       cmake ../parflow -DCMAKE_INSTALL_PREFIX=$(PARFLOW_DIR) -DPARFLOW_AMPS_LAYER=mpi1

   TCL is required for building ParFlow. If TCL is not installed in the
   system locations (/usr or /usr/local) you need to specify the path
   with the cmake option.

#. | **Running a sample problem**
   | There is a test directory that contains not only example scripts of
   ParFlow problems but the correct output for these scripts as well.
   This may be used to test the compilation process and verify that
   ParFlow is installed correctly. If all went well a sample ParFlow
   problem can be run using:

   ::

       cd $PARFLOW_DIR
       cd test
       tclsh default_single.tcl 1 1 1

   Note that PAFLOW\ :sub:`D`\ IR must be set for this to work and it
   assumes tclsh is in your path. Make sure to use the same TCL as was
   used in the CMake configure. The entire suite of test cases may be
   run using CTest to test a range of functionality in ParFlow. This may
   be done by:

   ::

       cd build
       make test

#. | **Notes and other options:**
   | ParFlow may be compiled with a number of options using the
   configure script. Some common options are compiling CLM as in to
   compile with timing and optimization or to use a compiler other than
   gcc. To compile with CLM add -PARFLOW:sub:`H`\ AVE\ :sub:`C`\ LM=ON
   to the configure line such as:

   ::

       cmake ../parflow -DPARFLOW_AMPS_LAYER=mpi1 -PARFLOW_HAVE_CLM=ON -DCMAKE_INSTALL_PREFIX=$(PARFLOW_DIR)) 

   Other common options are:

   -  to include the CLM module: –PARFLOW:sub:`H`\ AVE\ :sub:`C`\ LM=ON

   -  to include the *SILO* library which provides greater file output
      formats that are compatible with the *VisIt* rendering package and
      must first be compiled separately (see below):

   -  to include the *HDF5* library which provides greater file output
      formats”

   -  to include the *Hypre* library which provides greater solver
      flexibility and options and also needs to be downloaded and built
      separately:

   -  to write a single, undistributed ParFlow binary file:
      -DPARFLOW:sub:`A`\ MPS\ :sub:`S`\ EQUENTIAL\ :sub:`I`\ O=true

   -  to write timing information in the log file:
      -DPARFLOW:sub:`E`\ NABLE\ :sub:`T`\ IMING=true

   All these options combined in the configure line would look like:

   ::

       cd build
       cmake ../parflow \
         -DPARFLOW_AMPS_LAYER=mpi1 \
         -PARFLOW_HAVE_CLM=ON \
         -DSILO_ROOT=$(PARFLOW_SILO_DIR) \
         -DHDF5_ROOT=$(PARFLOW_HDF5_DIR) \
         -DHYPRE_ROOT=$(PARFLOW_HYPRE_DIR) \
         -DCMAKE_INSTALL_PREFIX=$(PARFLOW_DIR)) 
       make 
       make install

   To enable detailed timing of the performance of several different
   components within ParFlow use the –enable-timing option. The standard
   CMAKE\ :sub:`B`\ UILD\ :sub:`T`\ YPE variable controls if a debug or
   release (with optimization) is built.
   -DCMAKE:sub:`B`\ UILD\ :sub:`T`\ YPE=RELEASE will build a release
   version.

   It is often desirable to use different C and F90/95 compilers (such
   as *Intel* or *Portland Group*) to generate optimized code for
   specific architectures or simply personal preference. To change
   compilers, set the CC, FC and F77 variables (these may include a path
   too). For example, to change to the *Intel* compilers in the bash
   shell:

   ::

       export CC=icc
       export FC=ifort
       export F77=ifort

#. | **Build and install ParFlow** with GNU autoconf

   In addition configuration with cmake, ParFlow has deprecated support
   for GNU autoconf. GNU autoconf support will be maintained while the
   cmake support is fully tested. Support will be removed in a future
   relase. Please use cmake and report bugs.

   This step builds the ParFlow library and executable that runs on a
   serial or parallel machine. The library is used when ParFlow is used
   as a component of another simulation (*e.g.* WRF).

   ::

       cd $PARFLOW_DIR
       cd pfsimulator
       ./configure --prefix=$PARFLOW_DIR --with-amps=mpi1
       make 
       make install

   This will build a parallel version of /parflow using the MPI1
   libraries but no other options (a very basic installation with few
   features commonly used). You can control build options for /parflow
   in the configure step by adding other options to that command-line.
   For a list of *all* the configure options, use

   ::

       ./configure --help 

   to list them. Note that ParFlow defaults to building a sequential
   version so –with-amps is needed when building for a parallel
   computer. You can explicitly specify the path to the MPI to use with
   the –with-mpi option to configure. This controls *AMPS* which stands
   for *A*\ nother *M*\ essage *P*\ assing *S*\ ytem. *AMPS* is a
   flexible message-passing layer within ParFlow that allows a common
   code core to be quickly and easily adapted to different parallel
   environments.

#. | **Build and install pftools**
   | pftools is a package of utilities and a TCL library that is used to
   setup and postprocess Parflow files. The input files to ParFlow are
   TCL scripts so TCL must be installed on the system.

   The first command will build ParFlow and the bundled tools and
   install them in the $PARFLOW\ :sub:`D`\ IR directory. The second
   command will build and install the documentation. A bare-bones
   configure and build looks like:

   ::

       cd $PARFLOW_DIR
       cd pftools
       ./configure --prefix=$PARFLOW_DIR --with-amps=mpi1
       make 
       make install
       make doc_install

   Note that pftools is *NOT* parallel but some options for how files
   are written are based on the communication layer so pftools needs to
   have the same options that were used to build the ParFlow library.

   If TCL is not installed in the system locations (/usr or /usr/local)
   you need to specify the path with the –with-tcl=<PATH> configure
   option.

   See ./configure –help for additional configure options for pftools.

#. | **Running a sample problem**
   | There is a test directory that contains not only example scripts of
   ParFlow problems but the correct output for these scripts as well.
   This may be used to test the compilation process and verify that
   ParFlow is installed correctly. If all went well a sample ParFlow
   problem can be run using:

   ::

       cd $PARFLOW_DIR
       cd test
       tclsh default_single.tcl 1 1 1

   Note that PAFLOW\ :sub:`D`\ IR must be set for this to work and it
   assumes tclsh is in your path. Make sure to use the same TCL as was
   used in the pftools configure. The entire suite of test cases may be
   run at once to test a range of functionality in ParFlow. This may be
   done by:

   ::

       cd $PARFLOW_DIR
       cd test
       make check

#. | **Notes and other options:**
   | ParFlow may be compiled with a number of options using the
   configure script. Some common options are compiling CLM as in to
   compile with timing and optimization or to use a compiler other than
   gcc. To compile with CLM add –with-clm to the configure line such as:

   ::

       ./configure --prefix=$PARFLOW_DIR --with-amps=mpi1 --with-clm

   Common options are:

   -  to include the CLM module: –with-clm

   -  to include the *SILO* library which provides greater file output
      formats that are compatible with the *VisIt* rendering package and
      must first be compiled separately (see below):

   -  to include the *Hypre* library which provides greater solver
      flexibility and options and also needs to be downloaded and built
      separately:

   -  to write a single, undistributed ParFlow binary file:
      –with-amps-sequential-io

   -  to write timing information in the log file: –enable-timing

   All these options combined in the configure line would look like:

   ::

       cd $PARFLOW_DIR
       cd pfsimulator
       ./configure --prefix=$PARFLOW_DIR --with-amps=mpi1 --with-clm 
       --enable-timing --with-silo=$SILO_DIR --with-hypre=$HYPRE_DIR
       --with-amps-sequential-io
       make 
       make install

   pftools needs to be compiled and built with compatible options that
   correspond to ParFlow. For the options above, pftools would be built
   as follows:

   ::

       cd $PARFLOW_DIR
       cd pftools
       ./configure --prefix=$PARFLOW_DIR --with-amps=mpi1 --with-silo=$SILO_DIR
       --with-amps-sequential-io

       make 
       make install
       make doc_install

   Note that CLM and *Hypre* are not used by pftools and those options
   do not need to be included, however the file formats (*SILO* and
   single file PFB) are very important and need to match exactly what is
   specified in pfsimulator.

   To enable detailed timing of the performance of several different
   components within ParFlow use the –enable-timing option. To use
   compiler optimizations use the –enable-opt=STRING where the =STRING
   is an optional flag to specify the level and type of optimization.

   IMPORTANT NOTE: Optimization and debugging are controlled independent
   of one another. So to compile with optimization and no debugging you
   need to specify both –enable-opt=STRING AND –disable-debug.

   It is often desirable to use different C and F90/95 compilers (such
   as *Intel* or *Portland Group*) to match hardware specifics, for
   performance reasons or simply personal preference. To change
   compilers, set the CC, FC and F77 variables (these may include a path
   too). For example, to change to the *Intel* compilers in c-shell:

   ::

       setenv CC icc
       setenv FC ifort
       setenv F77 ifort

External Libraries
~~~~~~~~~~~~~~~~~~

Many of the features of ParFlow use a file structure called Silo. Silo
is a free, open-source, format detailed at:

https://wci.llnl.gov/codes/silo/

Support for Silo is integrated into ParFlow but the Silo libraries must
be built separately and then linked into ParFlow during the build and
configure process. This may be done using the –with-silo=PATH where the
PATH is the location of the Silo libraries.

Some features of ParFlow need to call the solver package *Hypre*
externally. These include the command options **PFMG**, **SMG** and
**PFMGOctree**. *Hypre* is a free, open-source, library detailed at:

https://computation.llnl.gov/casc/hypre/software.html

Support for *Hypre* 2.4.0b or later is integrated into ParFlow but the
libraries must be built separately and then linked into ParFlow during
the build and configure process. This may be done using the
–with-hypre=PATH where the PATH is the location of the *Hypre*
libraries.

Running the Sample Problem
--------------------------

Here, we assume that ParFlow is already built. The following steps will
allow you to run a simple test problem supplied with the distribution.

#. We first create a directory in which to run the problem, then copy
   into it some supplied default input files. So, do the following
   anywhere in your $HOME directory:

   ::

       mkdir foo
       cd foo
       cp $PARFLOW_DIR/examples/default_single.tcl .
       chmod 640 *

   We used the directory name foo above; you may use any name you
   wish [1]_. The last line changes the permissions of the files so that
   you may write to them.

#. Run ParFlow using the pftcl file as a TCL script

   ::

       tclsh default_single.tcl

You have now successfully run a simple ParFlow problem. For more
information on running ParFlow, see § [Running ParFlow].

ParFlow Solvers
---------------

ParFlow can operate using a number of different solvers. Two of these
solvers, IMPES (running in single-phase, fully-saturated mode, not
multiphase) and RICHARDS (running in variably-saturated mode, not
multiphase, with the options of land surface processes and coupled
overland flow) are detailed below. This is a brief summary of solver
settings used to simulate under three sets of conditions,
fully-saturated, variably-saturated and variably-saturated with overland
flow. A complete, detailed explanation of the solver parameters for
ParFlow may be found later in this manual. To simulate fully saturated,
steady-state conditions set the solver to IMPES, an example is given
below. This is also the default solver in ParFlow, so if no solver is
specified the code solves using IMPES.

::

    pfset Solver               Impes

To simulate variably-saturated, transient conditions, using Richards’
equation, variably/fully saturated, transient with compressible storage
set the solver to RICHARDS. An example is below. This is also the solver
used to simulate surface flow or coupled surface-subsurface flow.

::

    pfset Solver             Richards

To simulate overland flow, using the kinematic wave approximation to the
shallow-wave equations, set the solver to RICHARDS and set the upper
patch boundary condition for the domain geometry to OverlandFlow, an
example is below. This simulates overland flow, independently or coupled
to Richards’ Equation as detailed in . The overland flow boundary
condition can simulate both uniform and spatially-distributed sources,
reading a distribution of fluxes from a binary file in the latter case.

::

    pfset Patch.z-upper.BCPressure.Type	OverlandFlow

For this case, the solver needs to be set to RICHARDS:

::

    pfset Solver		Richards

ParFlow may also be coupled with the land surface model CLM . This
version of CLM has been extensively modified to be called from within
ParFlow as a subroutine, to support parallel infrastructure including
I/O and most importantly with modified physics to support coupled
operation to best utilize the integrated hydrology in ParFlow . To
couple CLM into ParFlow first the –with-clm option is needed in the
./configure command as indicated in § [Installing ParFlow]. Second, the
CLM module needs to be called from within ParFlow, this is done using
the following solver key:

::

    pfset Solver.LSM CLM

Note that this key is used to call CLM from within the nonlinear solver
time loop and requires that the solver bet set to RICHARDS to work. Note
also that this key defaults to *not* call CLM so if this line is omitted
CLM will not be called from within ParFlow even if compiled and linked.
Currently, CLM gets some of it’s information from ParFlow such as grid,
topology and discretization, but also has some of it’s own input files
for land cover, land cover types and atmospheric forcing.

The ParFlow System
==================

The ParFlow system is still evolving, but here we discuss how to define
the problem in § [Defining the Problem], how to run ParFlow in
§ [Running ParFlow], and restart a simulation in § [Restarting a Run].
We also cover options for visualizing the results in § [Visualizing
Output] and summarize the contents of a directory of test problems
provided with ParFlow in § [Test Directory]. Finally in § [Tutorial] we
walk through two ParFlow input scripts in detail.

The reader is also referred to § [Manipulating Data] for a detailed
listing of the of functions for manipulating ParFlow data.

Defining the Problem
--------------------

There are many ways to define a problem in ParFlow, here we summarize
the general approach for defining a domain (§ [Defining a domain]) and
simulating a real watershed (§ [Defining a Real domain]).

In all cases the \`\`main" ParFlow input file is the .tcl file. This
input file is a TCL script with some special routines to create a
database which is used as the input for ParFlow. See § [Main Input File
(.tcl)] for details on the format of this file. The input values into
ParFlow are defined by a key/value pair. For each key you provide the
associated value using the pfset command inside the input script.

Since the input file is a TCL script you can use any feature of TCL to
define the problem. This manual will make no effort to teach TCL so
refer to one of the available TCL manuals for more information
(“Practical Programming in TCL and TK” by Brent Welch is a good starting
point). This is NOT required, you can get along fine without
understanding TCL/TK.

Looking at the example programs in the test directory (§ [Test
Directory]) and going through the annotated input scripts included in
this manual ( § [Tutorial]) is one of the best ways to understand what a
ParFlow input file looks like.

Basic Domain Definition
~~~~~~~~~~~~~~~~~~~~~~~

ParFlow can handle complex geometries and defining the problem may
involve several steps. Users can specify simple box domains directly in
the tcl script. If a more complicated domain is required, the user may
convert geometries into the .pfsol file format (§ [ParFlow Solid Files
(.pfsol)]) using the appropriate PFTools conversion utility
(§ [Manipulating Data]). Alternatively, the topography can be specified
using .pfb files of the slopes in the x and y directions.

Regardless of the approach the user must set the computational grid
within the .pfb script as follows:

::

    #-----------------------------------------------------------------------------
    # Computational Grid
    #-----------------------------------------------------------------------------
    pfset ComputationalGrid.Lower.X                -10.0
    pfset ComputationalGrid.Lower.Y                 10.0
    pfset ComputationalGrid.Lower.Z                  1.0
     
    pfset ComputationalGrid.DX                       8.89
    pfset ComputationalGrid.DY                      10.67
    pfset ComputationalGrid.DZ                       1.0
     
    pfset ComputationalGrid.NX                      18
    pfset ComputationalGrid.NY                      15
    pfset ComputationalGrid.NZ                       8

The value is normally a single string, double, or integer. In some
cases, in particular for a list of names, you need to supply a space
seperated sequence. This can be done using either a double quote or
braces.

::

    pfset Geom.domain.Patches "left right front back bottom top"

    pfset Geom.domain.Patches {left right front back bottom top}

For commands longer than a single line, the TCL continuation character
can be used,

::

    pfset Geom.domain.Patches "very_long_name_1 very_long_name_2 very_long_name_3 \
                               very_long_name_4 very_long_name_5 very_long_name_6"

Setting Up a Real Domain
~~~~~~~~~~~~~~~~~~~~~~~~

This section provides a brief outline of a sample workflow for setup
ParFlow CLM simulation of a real domain. Of course there are many ways
to accomplish this and users are encouraged to develop a workflow that
works for them.

This example assumes that you are running with ParFlow CLM and it uses
slope files and an indicator file to define the topography and geologic
units of the domain. An alternate approach would be to define geometries
by building a .pfsol file (§ [ParFlow Solid Files (.pfsol)]) using the
appropriate PFTools conversion utility(§ [Manipulating Data]).

The general approach is as follows:

#. Gather input datasets to define the domain. First decide the
   resolution that you would like to simulate at. Then gather the
   following datasets at the appropriate resolution for your domain:

   #. Elevation (DEM)

   #. Soil data for the near surface layers

   #. Geologic maps for the deeper subsurface

   #. Land Cover

#. Create consistent gridded layers that are all clipped to your domain
   and have the same number of grid cells

#. Convert gridded files to .pfb (§ [ParFlow Binary Files (.pfb)]). One
   way to accomplish this is by reformatting the gridded outputs to the
   correct ParFlow .sa order (§ [ParFlow Simple ASCII Files (.sa and
   .sb)]) and to convert the .sa file to .pfb using the conversion tools
   (see § [common\ :sub:`p`\ ftcl] Example 1)

#. Calculate slopes in the x and y directions from the elevation
   dataset. This can be done with the built in tools as shown in
   § [common\ :sub:`p`\ ftcl] Example 5. In most cases some additional
   processing of the DEM will be required to ensure that the drainage
   patterns are correct. To check this you can run a \`\`parking lot
   test" by setting the permeability of surface to almost zero and
   adding a flux to the top surface. If the results from this test don’t
   look right (i.e. your runoff patterns don’t match what you expect)
   you will need to go back and modify your DEM. The built in ParFlow
   tools pitfill and flatfill can be used to address some issues. (These
   tools are also shown in § [common\ :sub:`p`\ ftcl] Example 5).

#. Create an indicator file for the subsurface. The indicator file is a
   3D .pfb file with the same dimensions as your domain that has an
   integer for every cell designating which unit it belongs to. The
   units you define will correspond to the soil types and geologic units
   from your input datasets.

#. Determine the hydrologic properties for each of the subsurface units
   defined in the indicator file. You will need: Permeability, specific
   storage, porosity and vanGenuchten parameters.

#. At this point you are ready to run a ParFlow model without CLM and if
   you don’t need to include the land surface model in your simulations
   you can ignore the following steps. Either way, at this point it is
   advisable to run a \`\`spinup" simulation to initialize the water
   table. There are several ways to approach this. One way is to start
   with the water table at a constant depth and run for a long time with
   a constant recharge forcing until the water table reaches a steady
   state. There are some additional key for spinup runs that are
   provided in § [Spinup Options].

#. Convert land cover classifications to the IGBP [2]_ land cover
   classes that are used in CLM.

   -  1. Evergreen Needleleaf Forest

   -  2. Evergreen Broadleaf Forest

   -  3. Deciduous Needleleaf Forest

   -  4. Deciduous Broadleaf Forest

   -  5. Mixed Forests

   -  6. Closed Shrublands

   -  7. Open Shrublands

   -  8. Woody Savannas

   -  9. Savannas

   -  10. Grasslands

   -  11. Permanent Wetlands

   -  12. Croplands

   -  13. Urban and Built-Up

   -  14. Cropland/Natural Vegetation Mosaic

   -  15. Snow and Ice

   -  16. Barren or Sparsely Vegetated

   -  17. Water

   -  18. Wooded Tundra

#. Create a CLM vegm file that designates the land cover fractions for
   every cell (Refer to the clm input directory in the Washita Example
   for an sample of what a vegm file should look like).

#. Create a CLM driver file to set the parameters for the CLM model
   (Refer to the clm input directory in the Washita Example for a sample
   of a CLM driver file).

#. Assemble meteorological forcing data for your domain. CLM uses
   Greenwich Mean Time (GMT), not local time. The year, date and hour
   (in GMT) that the forcing begins should match the values in
   drv\ :sub:`c`\ lmin.dat. CLM requires the following variables (also
   described on p. ):

   -  DSWR: Visible or short-wave radiation :math:`[W/m^2]`.

   -  DLWR: Long wave radiation :math:`[W/m^2]`

   -  APCP: Precipitation :math:`[mm/s]`

   -  Temp: Air Temperature :math:`[K]`

   -  UGRD: East-west wind speed :math:`[m/s]`

   -  VGRD: South-to-North wind speed :math:`[m/s]`

   -  Press: Atmospheric pressure :math:`[pa]`

   -  SPFH: Specific humidity :math:`[kg/kg]`

   If you choose to do spatially heterogenous forcings you will need to
   generate separate files for each variable. The files should be
   formatted in the standard ParFlow format with the third (i.e. z
   dimension) as time. If you are doing hourly simulations it is
   standard practice to put 24 hours in one file, but you can decide how
   many time steps per file. For an example of heterogenous forcing
   files refer to the NLDAS directory in the Washita Example)

   Alternatively, if you would like to force the model with spatially
   homogenous forcings, then a single file can be provided where each
   variable is a column and rows designate time steps.

#. Run your simulation!

Running ParFlow
---------------

Once the problem input is defined, you need to add a few things to the
script to make it execute ParFlow. First you need to add the TCL
commands to load the ParFlow command package.

::

    #
    # Import the ParFlow TCL package
    #
    lappend auto_path $env(PARFLOW_DIR)/bin 
    package require parflow
    namespace import Parflow::*

This loads the pfset and other ParFlow commands into the TCL shell.

Since this is a script you need to actually run ParFlow. These are
normally the last lines of the input script.

::

    #-----------------------------------------------------------------------------
    # Run and Unload the ParFlow output files
    #-----------------------------------------------------------------------------
    pfrun default_single
    pfundist default_single

The pfrun command runs ParFlow with the database as it exists at that
point in the file. The argument is the name to give to the output files
(which will normally be the same as the name of the script). Advanced
users can set up multiple problems within the input script by using
different output names.

The pfundist command takes the output files from the ParFlow run and
undistributes them. ParFlow uses a virtual file system which allows
files to be distributed across the processors. The pfundist takes these
files and collapses them into a single file. On some machines if you
don’t do the pfundist you will see many files after the run. Each of
these contains the output from a single node; before attempting using
them you should undistribute them.

Since the input file is a TCL script run it using TCL:

::

    tclsh runname.tcl

NOTE: Make sure you are using TCL 8.0 or later. The script will not work
with earlier releases.

One output file of particular interest is the <run name>.out.log file.
This file contains information about the run such as number of processes
used, convergence history of algorithms, timings and MFLOP rates. For
Richards’ equation problems (including overland flow) the <run
name>.out.kinsol.log file contains the nonlinear convergence information
for each timestep. Additionally, the <run name>.out.tx contains all
information routed to standard out of the machine you are running on and
often contains error messages and other control information.

Restarting a Run
----------------

A ParFlow run may need to be restarted because either a system time
limit has been reached, ParFlow has been prematurely terminated or the
user specifically sets up a problem to run in segments. In order to
restart a run the user needs to know the conditions under which ParFlow
stopped. If ParFlow was prematurely terminated then the user must
examine the output files from the last \`\`timed dump“ to see if they
are complete. If not then those data files should be discarded and the
output files from the next to last \`\`timed dump” will be used in the
restarting procedure. As an important note, if any set of \`\`timed
dump" files are deleted remember to also delete corresponding lines in
the well output file or recombining the well output files from the
individual segments afterwards will be difficult. It is not necessary to
delete lines from the log file as you will only be noting information
from it. To summarize, make sure all the important output data files are
complete, accurate and consistent with each other.

Given a set of complete, consistent output files - to restart a run
follow this procedure :

#. Note the important information for restarting :

   -  Write down the dump sequence number for the last collection of
      “timed dump” data.

   -  Examine the log file to find out what real time that \`\`timed
      dump" data was written out at and write it down.

#. Prepare input data files from output data files :

   -  Take the last pressure output file before the restart with the
      sequence number from above and format them for regular input using
      the keys detailed in § [Initial Conditions: Pressure] and possibly
      the pfdist utility in the input script.

#. Change the Main Input File § [Main Input File (.tcl)] :

   -  Edit the .tcl file (you may want to save the old one) and utilize
      the pressure initial condition input file option (as referenced
      above) to specify the input files you created above as initial
      conditions for concentrations.

#. Restart the run :

   -  Utilizing an editor recreate all the input parameters used in the
      run except for the following two items :

      -  Use the dump sequence number from step 1 as the start\_count.

      -  Use the real time that the dump occured at from step 1 as the
         start\_time.

      -  To restart with CLM, use the Solver.CLM.IstepStart key
         described in § [CLM Solver Parameters] with a value equal to
         the dump sequence plus one. Make sure this corresponds to
         changes to drv\ :sub:`c`\ lmin.dat.

Visualizing Output
------------------

| While ParFlow does not have any visualization capabilities built-in,
there are a number flexible, free options. Probably the best option is
to use *VisIt*. *VisIt* is a powerful, free, open-source, rendering
environment. It is multiplatform and may be downloaded directly from:
https://visit.llnl.gov/
| The most flexible option for using *VisIt* to view ParFlow output is
to write files using the *SILO* format, which is available either as a
direct output option (described in § [Code Parameters]) or a conversion
option using pftools. Many other output conversion options exist as
described in § [Manipulating Data] and this allows ParFlow output to be
converted into formats used by almost all visualization software.

Directory of Test Cases
-----------------------

ParFlow comes with a directory containing a few simple input files for
use as templates in making new files and for use in testing the code.
These files sit in the /test directory described earlier. This section
gives a brief description of the problems in this directory.

crater2D.tcl An example of a two-dimensional, variably-saturated crater
infiltration problem with time-varying boundary conditions. It uses the
solid file crater2D.pfsol.

default\ :sub:`r`\ ichards.tcl The default variably-saturated Richards’
Equation simulation test script.

default\ :sub:`s`\ ingle.tcl The default parflow, single-processor,
fully-saturated test script.

forsyth2.tcl An example two-dimensional, variably-saturated infiltration
problem with layers of different hydraulic properties. It runs problem 2
in and uses the solid file fors2\ :sub:`h`\ f.pfsol.

harvey.flow.tcl An example from for the Cape Cod bacterial injection
site. This example is a three-dimensional, fully-saturated flow problem
with spatially heterogeneous media (using a correlated, random field
approach). It also provides examples of how tcl/tk scripts may be used
in conjunction with ParFlow to loop iteratively or to run other scripts
or programs. It uses the input text file stats4.txt. This input script
is fully detailed in § [Tutorial]

default\ :sub:`o`\ verland.tcl An overland flow boundary condition test
and example script based loosely on the V-catchment problem in . There
are options provided to expand this problem into other overland
flow-type, transient boundary-type problems included in the file as
well.

LW\ :sub:`v`\ ar\ :sub:`d`\ z\ :sub:`s`\ pinup.tcl An example that uses
the Little Washita domain to demonstrate a steady-state spinup
initialization using P-E forcing. It also demonstrates the variable dz
keys.

LW\ :sub:`v`\ ar\ :sub:`d`\ z.tcl An example that uses the Little
Washita domain to demonstrate surface flow network development. It also
uses the variable dz keys.

Evap\ :sub:`T`\ rans\ :sub:`t`\ est.tcl An example that modifies the
default\ :sub:`o`\ verland.tcl to demonstrate steady-state external flux
.pfb files.

overland\ :sub:`f`\ lux.tcl An example that modifies the
default\ :sub:`o`\ verland.tcl to demonstrate transient external flux
.pfb files.

/clm/clm.tcl An example of how to use ParFlow coupled to clm. This
directory also includes clm-specific input. Note: this problem will only
run if –with-clm flag is used during the configure and build process.

water\ :sub:`b`\ alance\ :sub:`x`.tcl and
water\ :sub:`b`\ alance\ :sub:`y`.tcl. An overland flow example script
that uses the water-balance routines integrated into pftools. These two
problems are based on simple overland flow conditions with slopes
primarily in the x or y-directions. Note: this problem only will run if
the Silo file capability is used, that is a –with-silo=PATH flag is used
during the configure and build process.

pfmg.tcl and pfmg\ :sub:`o`\ ctree.tcl. Tests of the external *Hypre*
preconditioner options. Note: this problem only will run if the *Hypre*
capability is used, that is a –with-hypre=PATH flag is used during the
configure and build process.

test\ :sub:`x`.tcl A test problem for the Richards’ solver that compares
output to an analytical solution.

/washita/tcl:sub:`s`\ cripts/LW\ :sub:`T`\ est.tcl A three day
simulation of the Little Washita domain using ParFlow CLM with 3D
forcings.

Annotated Input Scripts
-----------------------

This section contains two annotated input scripts:

-  § [Harvey Flow Example] contains the harvey flow example
   (harvey.flow.tcl) which is an idealized domain with a heterogenous
   subsurface. The example also demonstrates how to generate multiple
   realizations of the subsurface and add pumping wells.

-  § [Little Washita Example] contains the Little Washita example
   (LW\ :sub:`T`\ est.tcl) which simulates a moderately sized (41km by
   41km) real domain using ParFlow CLM with 3D meteorological forcings.

To run ParFlow, you use a script written in Tcl/TK. This script has a
lot of flexibility, as it is somewhere in between a program and a user
interface. The tcl script gives ParFlow the data it requires (or tells
ParFlow where to find or read in that data) and also tells ParFlow to
run.

To run the simulation:

#. make any modifications to the tcl input script (and give a new name,
   if you want to)

#. save the tcl script

#. For Linux/Unix/OSX: invoke the script from the command line using the
   tcl-shell, this looks like: >tclsh filename.tcl

#. Wait patiently for the command prompt to return (Linux/Unix/OSX)
   indicating that ParFlow has finished. Intermediate files are written
   as the simulation runs, however there is no other indication that
   ParFlow is running.

To modify a tcl script, you right-click and select edit from the menu.
If you select open, you will run the script.

**Note:** The units for **K** (im/d, usually) are critical to the entire
construction. These length and time units for **K** set the units for
all other variables (input or generated, throughout the entire
simulation) in the simulation. ParFlow can set to solve using hydraulic
conductivity by literally setting density, viscosity and gravity to one
(as is done in the script below). This means the pressure units are in
length (meters), so pressure is now so-called pressure-head.

Harvey Flow Example
~~~~~~~~~~~~~~~~~~~

This tutorial matches the harvey\ :sub:`f`\ low.tcl file found in the
/test directory. This example is directly from . This example
demonstrates how to set up and run a fully saturated flow problem with
heterogeneous hydraulic conductivity using the turning bands approach .
Given statistical parameters describing the geology of your site, this
script can be easily modified to make as many realizations of the
subsurface as you like, each different and yet having the same
statistical parameters, useful for a Monte Carlo simulation. This
example is the basis for several fully-saturated ParFlow applications .

When the script runs, it creates a new directory named /flow right in
the directory where the tcl script is stored. ParFlow then puts all its
output in /flow. Of course, you can change the name and location of this
output directory by modifying the tcl script that runs ParFlow.

Now for the tcl script:

::

    #
    # Import the ParFlow TCL package
    #

These first three lines are what link ParFlow and the tcl script, thus
allowing you to use a set of commands seen later, such as pfset, etc.

::

    lappend auto_path $env(PARFLOW_DIR)/bin 
    package require parflow
    namespace import Parflow::*

    #-----------------------------------------------------------------------------
    # File input version number
    #-----------------------------------------------------------------------------
    pfset FileVersion 4

These next lines set the parallel process topology. The domain is
divided in *x*,\ *y* and *z* by P, Q and R. The total number of
processors is P\*Q\*R (*see* § [Computing Topology]).

::

    #----------------------------------------------------------------------------
    # Process Topology
    #----------------------------------------------------------------------------

    pfset Process.Topology.P     1
    pfset Process.Topology.Q     1
    pfset Process.Topology.R     1

Next we set up the computational grid (*see* § [Defining the Problem]
and § [Computational Grid]).

::

    #----------------------------------------------------------------------------
    # Computational Grid
    #----------------------------------------------------------------------------

Locate the origin in the domain.

::

    pfset ComputationalGrid.Lower.X    0.0
    pfset ComputationalGrid.Lower.Y    0.0
    pfset ComputationalGrid.Lower.Z    0.0

Define the size of the domain grid block. Length units, same as those on
hydraulic conductivity.

::

    pfset ComputationalGrid.DX     0.34
    pfset ComputationalGrid.DY     0.34
    pfset ComputationalGrid.DZ     0.038

Define the number of grid blocks in the domain.

::

    pfset ComputationalGrid.NX      50
    pfset ComputationalGrid.NY      30
    pfset ComputationalGrid.NZ      100

This next piece is comparable to a pre-declaration of variables. These
will be areas in our domain geometry. The regions themselves will be
defined later. You must always have one that is the name of your entire
domain. If you want subsections within your domain, you may declare
these as well. For Cape Cod, we have the entire domain, and also the 2
(upper and lower) permeability zones in the aquifer.

::

    #----------------------------------------------------------------------------
    # The Names of the GeomInputs
    #----------------------------------------------------------------------------
    pfset GeomInput.Names "domain_input upper_aquifer_input lower_aquifer_input"

Now you characterize your domain that you just pre-declared to be a box
(*see* § [Geometries]), and you also give it a name, domain.

::

    #----------------------------------------------------------------------------
    # Domain Geometry Input
    #----------------------------------------------------------------------------
    pfset GeomInput.domain_input.InputType       Box
    pfset GeomInput.domain_input.GeomName   domain

Here, you set the limits in space for your entire domain. The span from
Lower.X to Upper.X will be equal to the product of ComputationalGrid.DX
times ComputationalGrid.NX. Same for *Y* and *Z* (i.e. the number of
grid elements times size of the grid element has to equal the size of
the grid in each dimension). The Patches key assigns names to the
outside edges, because the domain is the limit of the problem in space.

::

    #----------------------------------------------------------------------------
    # Domain Geometry
    #----------------------------------------------------------------------------
    pfset Geom.domain.Lower.X       0.0 
    pfset Geom.domain.Lower.Y       0.0
    pfset Geom.domain.Lower.Z       0.0

    pfset Geom.domain.Upper.X       17.0
    pfset Geom.domain.Upper.Y       10.2
    pfset Geom.domain.Upper.Z       3.8

    pfset Geom.domain.Patches "left right front back bottom top"

Just like domain geometry, you also set the limits in space for the
individual components (upper and lower, as defined in the Names of
GeomInputs pre-declaration). There are no patches for these geometries
as they are internal to the domain.

::

    #----------------------------------------------------------------------------
    # Upper Aquifer Geometry Input
    #----------------------------------------------------------------------------
    pfset GeomInput.upper_aquifer_input.InputType        Box
    pfset GeomInput.upper_aquifer_input.GeomName   upper_aquifer

    #----------------------------------------------------------------------------
    # Upper Aquifer Geometry
    #----------------------------------------------------------------------------
    pfset Geom.upper_aquifer.Lower.X                        0.0 
    pfset Geom.upper_aquifer.Lower.Y                        0.0
    pfset Geom.upper_aquifer.Lower.Z                        1.5

    pfset Geom.upper_aquifer.Upper.X                        17.0
    pfset Geom.upper_aquifer.Upper.Y                        10.2
    pfset Geom.upper_aquifer.Upper.Z                        1.5

    #----------------------------------------------------------------------------
    # Lower Aquifer Geometry Input
    #----------------------------------------------------------------------------
    pfset GeomInput.lower_aquifer_input.InputType       Box
    pfset GeomInput.lower_aquifer_input.GeomName   lower_aquifer

    #----------------------------------------------------------------------------
    # Lower Aquifer Geometry
    #----------------------------------------------------------------------------
    pfset Geom.lower_aquifer.Lower.X      0.0 
    pfset Geom.lower_aquifer.Lower.Y      0.0
    pfset Geom.lower_aquifer.Lower.Z      0.0

    pfset Geom.lower_aquifer.Upper.X     17.0
    pfset Geom.lower_aquifer.Upper.Y     10.2
    pfset Geom.lower_aquifer.Upper.Z      1.5

Now you add permeability data to the domain sections defined above
(§ [Permeability]). You can reassign values simply by re-stating them –
there is no need to comment out or delete the previous version – the
final statement is the only one that counts.

::

    #----------------------------------------------------------------------------
    # Perm
    #----------------------------------------------------------------------------

Name the permeability regions you will describe.

::

    pfset Geom.Perm.Names "upper_aquifer lower_aquifer"

You can set, for example homogeneous, constant permeability, or you can
generate a random field that meets your statistical requirements. To
define a constant permeability for the entire domain:

::

    #pfset Geom.domain.Perm.Type     Constant
    #pfset Geom.domain.Perm.Value    4.0

However, for Cape Cod, we did not want a constant permeability field, so
we instead generated a random permeability field meeting our statistical
parameters for each the upper and lower zones. Third from the bottom is
the Seed. This is a random starting point to generate the K field. Pick
any large *ODD* number. First we do something tricky with Tcl/TK. We use
the native commands within tcl to open a text file and read in locally
set variables. Note we use set here and not pfset. One is a native tcl
command, the other a ParFlow-specific command. For this problem, we are
linking the parameter estimation code, PEST to ParFlow. PEST writes out
the ascii file stats4.txt (also located in the /test directory) as the
result of a calibration run. Since we are not coupled to PEST in this
example, we just read in the file and use the values to assign
statistical properties.

::

    # we open a file, in this case from PEST to set upper and lower # kg and sigma
    #
    set fileId [open stats4.txt r 0600]
    set kgu [gets $fileId]
    set varu [gets $fileId]
    set kgl [gets $fileId]
    set varl [gets $fileId]
    close $fileId

Now we set the heterogeneous parameters for the Upper and Lower aquifers
(*see* § [Permeability]). Note the special section at the very end of
this block where we reset the geometric mean and standard deviation to
our values we read in from a file. **Note:** ParFlow uses *Standard
Deviation* not *Variance*.

::

    pfset Geom.upper_aquifer.Perm.Type "TurnBands"
    pfset Geom.upper_aquifer.Perm.LambdaX  3.60
    pfset Geom.upper_aquifer.Perm.LambdaY  3.60
    pfset Geom.upper_aquifer.Perm.LambdaZ  0.19
    pfset Geom.upper_aquifer.Perm.GeomMean  112.00

    pfset Geom.upper_aquifer.Perm.Sigma   1.0
    pfset Geom.upper_aquifer.Perm.Sigma   0.48989794
    pfset Geom.upper_aquifer.Perm.NumLines 150
    pfset Geom.upper_aquifer.Perm.RZeta  5.0
    pfset Geom.upper_aquifer.Perm.KMax  100.0
    pfset Geom.upper_aquifer.Perm.DelK  0.2
    pfset Geom.upper_aquifer.Perm.Seed  33333
    pfset Geom.upper_aquifer.Perm.LogNormal Log
    pfset Geom.upper_aquifer.Perm.StratType Bottom
    pfset Geom.lower_aquifer.Perm.Type "TurnBands"
    pfset Geom.lower_aquifer.Perm.LambdaX  3.60
    pfset Geom.lower_aquifer.Perm.LambdaY  3.60
    pfset Geom.lower_aquifer.Perm.LambdaZ  0.19

    pfset Geom.lower_aquifer.Perm.GeomMean  77.0
    pfset Geom.lower_aquifer.Perm.Sigma   1.0
    pfset Geom.lower_aquifer.Perm.Sigma   0.48989794
    pfset Geom.lower_aquifer.Perm.NumLines 150
    pfset Geom.lower_aquifer.Perm.RZeta  5.0
    pfset Geom.lower_aquifer.Perm.KMax  100.0
    pfset Geom.lower_aquifer.Perm.DelK  0.2
    pfset Geom.lower_aquifer.Perm.Seed  33333
    pfset Geom.lower_aquifer.Perm.LogNormal Log
    pfset Geom.lower_aquifer.Perm.StratType Bottom

    #pfset lower aqu and upper aq stats to pest/read in values

    pfset Geom.upper_aquifer.Perm.GeomMean  $kgu
    pfset Geom.upper_aquifer.Perm.Sigma  $varu

    pfset Geom.lower_aquifer.Perm.GeomMean  $kgl
    pfset Geom.lower_aquifer.Perm.Sigma  $varl

The following section allows you to specify the permeability tensor. In
the case below, permeability is symmetric in all directions (x, y, and
z) and therefore each is set to 1.0.

::

    pfset Perm.TensorType               TensorByGeom

    pfset Geom.Perm.TensorByGeom.Names  "domain"

    pfset Geom.domain.Perm.TensorValX  1.0
    pfset Geom.domain.Perm.TensorValY  1.0
    pfset Geom.domain.Perm.TensorValZ  1.0

Next we set the specific storage, though this is not used in the
IMPES/steady-state calculation.

::

    #----------------------------------------------------------------------------
    # Specific Storage
    #----------------------------------------------------------------------------
    # specific storage does not figure into the impes (fully sat) 
    # case but we still need a key for it

    pfset SpecificStorage.Type            Constant
    pfset SpecificStorage.GeomNames       ""
    pfset Geom.domain.SpecificStorage.Value 1.0e-4

ParFlow has the capability to deal with a multiphase system, but we only
have one (water) at Cape Cod. As we stated earlier, we set density and
viscosity artificially (and later gravity) both to 1.0. Again, this is
merely a trick to solve for hydraulic conductivity and pressure head. If
you were to set density and viscosity to their true values, the code
would calculate **k** (permeability). By using the *normalized* values
instead, you effectively embed the conversion of **k** to **K**
(hydraulic conductivity). So this way, we get hydraulic conductivity,
which is what we want for this problem.

::

    #----------------------------------------------------------------------------
    # Phases
    #----------------------------------------------------------------------------

    pfset Phase.Names "water"

    pfset Phase.water.Density.Type	Constant
    pfset Phase.water.Density.Value	1.0

    pfset Phase.water.Viscosity.Type	Constant
    pfset Phase.water.Viscosity.Value	1.0

We will not use the ParFlow grid based transport scheme. We will then
leave contaminants blank because we will use a different code to model
(virus, tracer) contamination.

::

    #----------------------------------------------------------------------------
    # Contaminants
    #----------------------------------------------------------------------------
    pfset Contaminants.Names			""

As with density and viscosity, gravity is normalized here. If we used
the true value (in the *[L]* and *[T]* units of hydraulic conductivity)
the code would be calculating permeability. Instead, we normalize so
that the code calculates hydraulic conductivity.

::

    #----------------------------------------------------------------------------
    # Gravity
    #----------------------------------------------------------------------------

    pfset Gravity				1.0

    #----------------------------------------------------------------------------
    # Setup timing info
    #----------------------------------------------------------------------------

This basic time unit of 1.0 is used for transient boundary and well
conditions. We are not using those features in this example.

::

    pfset TimingInfo.BaseUnit		1.0

Cape Cod is a steady state problem, so these timing features are again
unused, but need to be included.

::

    pfset TimingInfo.StartCount   -1
    pfset TimingInfo.StartTime     0.0
    pfset TimingInfo.StopTime     0.0

Set the dump interval to -1 to report info at the end of every
calculation, which in this case is only when steady state has been
reached.

::

    pfset TimingInfo.DumpInterval	       -1

Next, we assign the porosity (*see* § [Porosity]). For the Cape Cod, the
porosity is 0.39.

::

    #----------------------------------------------------------------------------
    # Porosity
    #----------------------------------------------------------------------------

    pfset Geom.Porosity.GeomNames          domain

    pfset Geom.domain.Porosity.Type    Constant
    pfset Geom.domain.Porosity.Value   0.390

Having defined the geometry of our problem before and named it domain,
we are now ready to report/upload that problem, which we do here.

::

    #----------------------------------------------------------------------------
    # Domain
    #----------------------------------------------------------------------------
    pfset Domain.GeomName domain

Mobility between phases is set to 1.0 because we only have one phase
(water).

::

    #----------------------------------------------------------------------------
    # Mobility
    #----------------------------------------------------------------------------
    pfset Phase.water.Mobility.Type        Constant
    pfset Phase.water.Mobility.Value       1.0

Again, ParFlow has more capabilities than we are using here in the Cape
Cod example. For this example, we handle monitoring wells in a separate
code as we assume they do not remove a significant amount of water from
the domain. Note that since there are no well names listed here, ParFlow
assumes we have no wells. If we had pumping wells, we would have to
include them here, because they would affect the head distribution
throughout our domain. See below for an example of how to include
pumping wells in this script.

::

    #----------------------------------------------------------------------------
    # Wells
    #----------------------------------------------------------------------------
    pfset Wells.Names ""

You can give certain periods of time names if you want to (ie.
Pre-injection, post-injection, etc). Here, however we do not have
multiple time intervals and are simulating in steady state, so time
cycle keys are simple. We have only one time cycle and it’s constant for
the duration of the simulation. We accomplish this by giving it a repeat
value of -1, which repeats indefinitely. The length of the cycle is the
length specified below (an integer) multiplied by the base unit value we
specified earlier.

::

    #----------------------------------------------------------------------------
    # Time Cycles
    #----------------------------------------------------------------------------
    pfset Cycle.Names constant
    pfset Cycle.constant.Names		"alltime"
    pfset Cycle.constant.alltime.Length	 1
    pfset Cycle.constant.Repeat		-1

Now, we assign Boundary Conditions for each face (each of the Patches in
the domain defined before). Recall the previously stated Patches and
associate them with the boundary conditions that follow.

::

    pfset BCPressure.PatchNames "left right front back bottom top"

These are Dirichlet BCs (i.e. constant head over cell so the pressure
head is set to hydrostatic– *see* § [Boundary Conditions: Pressure]).
There is no time dependence, so use the constant time cycle we defined
previously. RefGeom links this to the established domain geometry and
tells ParFlow what to use for a datum when calculating hydrostatic head
conditions.

::

    pfset Patch.left.BCPressure.Type          DirEquilRefPatch
    pfset Patch.left.BCPressure.Cycle        "constant"
    pfset Patch.left.BCPressure.RefGeom domain

Reference the current (left) patch to the bottom to define the line of
intersection between the two.

::

    pfset Patch.left.BCPressure.RefPatch  bottom

Set the head permanently to 10.0m. Pressure-head will of course vary top
to bottom because of hydrostatics, but head potential will be constant.

::

    pfset Patch.left.BCPressure.alltime.Value  10.0

Repeat the declarations for the rest of the faces of the domain. The
left to right (*X*) dimension is aligned with the hydraulic gradient.
The difference between the values assigned to right and left divided by
the length of the domain corresponds to the correct hydraulic gradient.

::

    pfset Patch.right.BCPressure.Type               DirEquilRefPatch
    pfset Patch.right.BCPressure.Cycle              "constant"
    pfset Patch.right.BCPressure.RefGeom       domain
    pfset Patch.right.BCPressure.RefPatch       bottom
    pfset Patch.right.BCPressure.alltime.Value 9.97501

    pfset Patch.front.BCPressure.Type                FluxConst
    pfset Patch.front.BCPressure.Cycle              "constant"
    pfset Patch.front.BCPressure.alltime.Value 0.0

    pfset Patch.back.BCPressure.Type               FluxConst
    pfset Patch.back.BCPressure.Cycle              "constant"
    pfset Patch.back.BCPressure.alltime.Value 0.0

    pfset Patch.bottom.BCPressure.Type               FluxConst
    pfset Patch.bottom.BCPressure.Cycle              "constant"
    pfset Patch.bottom.BCPressure.alltime.Value 0.0

    pfset Patch.top.BCPressure.Type			FluxConst
    pfset Patch.top.BCPressure.Cycle			"constant"
    pfset Patch.top.BCPressure.alltime.Value		0.0

Next we define topographic slopes and Mannings *n* values. These are not
used, since we do not solve for overland flow. However, the keys still
need to appear in the input script.

::

    #---------------------------------------------------------
    # Topo slopes in x-direction
    #---------------------------------------------------------
    # topo slopes do not figure into the impes (fully sat) case but we still
    # need keys for them

    pfset TopoSlopesX.Type "Constant"
    pfset TopoSlopesX.GeomNames ""

    pfset TopoSlopesX.Geom.domain.Value 0.0

    #---------------------------------------------------------
    # Topo slopes in y-direction
    #---------------------------------------------------------

    pfset TopoSlopesY.Type "Constant"
    pfset TopoSlopesY.GeomNames ""

    pfset TopoSlopesY.Geom.domain.Value 0.0

    #---------------------------------------------------------
    # Mannings coefficient 
    #---------------------------------------------------------
    # mannings roughnesses do not figure into the impes (fully sat) case but we still
    # need a key for them

    pfset Mannings.Type "Constant"
    pfset Mannings.GeomNames ""
    pfset Mannings.Geom.domain.Value 0.

Phase sources allows you to add sources other than wells and boundaries,
but we do not have any so this key is constant, 0.0 over entire domain.

::

    #----------------------------------------------------------------------------
    # Phase sources:
    #----------------------------------------------------------------------------

    pfset PhaseSources.water.Type                         Constant
    pfset PhaseSources.water.GeomNames                    domain
    pfset PhaseSources.water.Geom.domain.Value        0.0

Next we define solver parameters for **IMPES**. Since this is the
default solver, we do not need a solver key.

::

    #---------------------------------------------------------
    #  Solver Impes  
    #---------------------------------------------------------

We allow up to 50 iterations of the linear solver before it quits or
converges.

::

    pfset Solver.MaxIter 50

The solution must be accurate to this level

::

    pfset Solver.AbsTol  1E-10

We drop significant digits beyond E-15

::

    pfset Solver.Drop   1E-15

    #--------------------------------------------------------
    # Run and Unload the ParFlow output files
    #---------------------------------------------------------

Here you set the number of realizations again using a local tcl
variable. We have set only one run but by setting the n\ :sub:`r`\ uns
variable to something else we can run more than one realization of
hydraulic conductivity.

::

    # this script is setup to run 100 realizations, for testing we just run one
    ###set n_runs 100
    set n_runs 1

Here is where you tell ParFlow where to put the output. In this case, it
is a directory called flow. Then you cd (change directory) into that new
directory. If you wanted to put an entire path rather than just a name,
you would have more control over where your output file goes. For
example, you would put file mkdir
\`\`/cape\ :sub:`c`\ od/revised\ :sub:`s`\ tatistics/flow" and then
change into that directory.

::

    file mkdir "flow"
    cd "flow"

Now we loop through the realizations, again using tcl. k is the integer
counter that is incremented for each realization. When you use a
variable (rather than define it), you precede it with\ ``$``. The
hanging character ``{`` opens the do loop for k.

::

    #
    #  Loop through runs
    #
    for {set k 1} {$k <= $n_runs} {incr k 1} {

The following expressions sets the variable seed equal to the expression
in brackets, which increments with each turn of the do loop and each
seed will produce a different random field of **K**. You set upper and
lower aquifer, because in the Cape Cod site, these are the two subsets
of the domain. Note the seed starts at a different point to allow for
different random field generation for the upper and lower zones.

::

    #
    # set the random seed to be different for every run
    #
    pfset Geom.upper_aquifer.Perm.Seed  [ expr 33333+2*$k ] 
    pfset Geom.lower_aquifer.Perm.Seed  [ expr 31313+2*$k ]

The following command runs ParFlow and gives you a suite of output files
for each realization. The file names will begin
harvey\ :sub:`f`\ low.1.xxxxx, harvey\ :sub:`f`\ low.2.xxxx, etc up to
as many realizations as you run. The .xxxxx part will designate x, y,
and z permeability, etc. Recall that in this case, since we normalized
gravity, viscosity, and density, remember that we are really getting
hydraulic conductivity.

::

    pfrun harvey_flow.$k

This command removes a large number of superfluous dummy files or
un-distributes parallel files back into a single file. If you compile
with the – with-amps-sequential-io option then a single ParFlow file is
written with corresponding XXXX.dist files and the pfundist command just
removes these .dist files (though you don’t really need to remove them
if you don’t want to).

::

    pfundist harvey_flow.$k

The following commands take advantage of PFTools (*see* § [PFTCL
Commands]) and load pressure head output of the /parflow model into a
pressure matrix.

::

    # we use pf tools to convert from pressure to head
    # we could do a number of other things here like copy files to different
    # format
    set press [pfload harvey_flow.$k.out.press.pfb]

The next command takes the pressures that were just loaded and converts
it to head and loads them into a head matrix tcl variable.

::

    set head [pfhhead $press]

Finally, the head matrix is saved as a ParFlow binary file (.pfb) and
the k do loop is closed by the ``}`` character. Then we move up to the
root directory when we are finished

::

     pfsave $head -pfb harvey_flow.$k.head.pfb
    }

    cd ".."

Once you have modified the tcl input script (if necessary) and run
ParFlow, you will have as many realizations of your subsurface as you
specified. Each of these realizations will be used as input for a
particle or streamline calculation in the future. We can see below, that
since we have a tcl script as input, we can do a lot of different
operations, for example, we might run a particle tracking transport code
simulation using the results of the ParFlow runs. This actually
corresponds to the example presented in the SLIM user’s manual.

::

    # this could run other tcl scripts now an example is below
    #puts stdout "running SLIM"
    #source bromide_trans.sm.tcl

We can add options to this script. For example if we wanted to add a
pumping well these additions are described below.

Adding a Pumping Well
~~~~~~~~~~~~~~~~~~~~~

Let us change the input problem by adding a pumping well:

#. Add the following lines to the input file near where the existing
   well information is in the input file. You need to replace the
   “Wells.Names” line with the one included here to get both wells
   activated (this value lists the names of the wells):

   ::

       pfset Wells.Names {new_well}

       pfset Wells.new_well.InputType                Recirc

       pfset Wells.new_well.Cycle		    constant

       pfset Wells.new_well.ExtractionType	    Flux
       pfset Wells.new_well.InjectionType            Flux

       pfset Wells.new_well.X			    10.0 
       pfset Wells.new_well.Y			    10.0
       pfset Wells.new_well.ExtractionZLower	     0.5
       pfset Wells.new_well.ExtractionZUpper	     0.5
       pfset Wells.new_well.InjectionZLower	     0.2
       pfset Wells.new_well.InjectionZUpper	     0.2

       pfset Wells.new_well.ExtractionMethod	    Standard
       pfset Wells.new_well.InjectionMethod          Standard

       pfset Wells.new_well.alltime.Extraction.Flux.water.Value        	     0.50
       pfset Wells.new_well.alltime.Injection.Flux.water.Value		     0.75

For more information on defining the problem, see § [Defining the
Problem].

We could also visualize the results of the ParFlow simulations, using
*VisIt*. For example, we can turn on *SILO* file output which allows
these files to be directly read and visualized. We would do this by
adding the following pfset commands, I usually add them to the solver
section:

::

    pfset Solver.WriteSiloSubsurfData True
    pfset Solver.WriteSiloPressure True
    pfset Solver.WriteSiloSaturation True

You can then directly open the file
harvey\ :sub:`f`\ low.#.out.perm\ :sub:`x`.silo (where # is the
realization number). The resulting image will be the hydraulic
conductivity field of your domain, showing the variation in
x-permeability in 3-D space. You can also generate representations of
head or pressure (or y or z permeability) throughout your domain using
ParFlow output files. See the section on visualization for more details.

Little Washita Example
~~~~~~~~~~~~~~~~~~~~~~

This tutorial matches the LW\ :sub:`T`\ est.tcl file found in the
/test/washita/tcl:sub:`s`\ cripts directory and corresponds to . This
script runs the Little Washita domain for three days using ParFlow CLM
with 3D forcings. The domain is setup using terrain following grid
(§ [TFG]) and subsurface geologes are specified using a .pfb indicator
file. Input files were generated using the workflow detailed in
§ [Defining a Real domain].

Now for the tcl script:

::

    #
    # Import the ParFlow TCL package
    #

These first three lines are what link ParFlow and the tcl script, thus
allowing you to use a set of commands seen later, such as pfset, etc.

::

    lappend auto_path $env(PARFLOW_DIR)/bin 
    package require parflow
    namespace import Parflow::*

    #-----------------------------------------------------------------------------
    # File input version number
    #-----------------------------------------------------------------------------
    pfset FileVersion 4

These next lines set the parallel process topology. The domain is
divided in *x*,\ *y* and *z* by P, Q and R. The total number of
processors is P\*Q\*R (*see* § [Computing Topology]).

::

    #----------------------------------------------------------------------------
    # Process Topology
    #----------------------------------------------------------------------------

    pfset Process.Topology.P     1
    pfset Process.Topology.Q     1
    pfset Process.Topology.R     1

Before we really get started make a directory for our outputs and copy
all of the required input files into the run directory. These files will
be described in detail later as they get used.

::

    #-----------------------------------------------------------------------------
    # Make a directory for the simulation and copy inputs into it
    #-----------------------------------------------------------------------------
    exec mkdir "Outputs"
    cd "./Outputs"

    # ParFlow Inputs
    file copy -force "../../parflow_input/LW.slopex.pfb" .
    file copy -force "../../parflow_input/LW.slopey.pfb" .
    file copy -force "../../parflow_input/IndicatorFile_Gleeson.50z.pfb"   .
    file copy -force "../../parflow_input/press.init.pfb"  .

    #CLM Inputs
    file copy -force "../../clm_input/drv_clmin.dat" .
    file copy -force "../../clm_input/drv_vegp.dat"  .
    file copy -force "../../clm_input/drv_vegm.alluv.dat"  . 

    puts "Files Copied"

Next we set up the computational grid (*see* § [Defining the Problem]
and § [Computational Grid]).

::

    #----------------------------------------------------------------------------
    # Computational Grid
    #----------------------------------------------------------------------------

Locate the origin in the domain.

::

    pfset ComputationalGrid.Lower.X    0.0
    pfset ComputationalGrid.Lower.Y    0.0
    pfset ComputationalGrid.Lower.Z    0.0

Define the size of the domain grid block. Length units, same as those on
hydraulic conductivity.

::

    pfset ComputationalGrid.DX     1000.0
    pfset ComputationalGrid.DY     1000.0
    pfset ComputationalGrid.DZ     2.0

Define the number of grid blocks in the domain.

::

    pfset ComputationalGrid.NX      41
    pfset ComputationalGrid.NY      41
    pfset ComputationalGrid.NZ      50

This next piece is comparable to a pre-declaration of variables. These
will be areas in our domain geometry. The regions themselves will be
defined later. You must always have one that is the name of your entire
domain. If you want subsections within your domain, you may declare
these as well. Here we define two geometries one is the domain and one
is for the indicator file (which will also span the entire domain).

::

    #-----------------------------------------------------------------------------
    # The Names of the GeomInputs
    #-----------------------------------------------------------------------------
    pfset GeomInput.Names                     "box_input indi_input"

Now you characterize the domain that you just pre-declared to be a box
(*see* § [Geometries]), and you also give it a name, domain.

::

    #-----------------------------------------------------------------------------
    # Domain Geometry Input
    #-----------------------------------------------------------------------------
    pfset GeomInput.box_input.InputType      Box
    pfset GeomInput.box_input.GeomName      domain

Here, you set the limits in space for your entire domain. The span from
Lower.X to Upper.X will be equal to the product of ComputationalGrid.DX
times ComputationalGrid.NX. Same for *Y* and *Z* (i.e. the number of
grid elements times size of the grid element has to equal the size of
the grid in each dimension). The Patches key assigns names to the
outside edges, because the domain is the limit of the problem in space.

::

    #-----------------------------------------------------------------------------
    # Domain Geometry 
    #-----------------------------------------------------------------------------
    pfset Geom.domain.Lower.X                        0.0
    pfset Geom.domain.Lower.Y                        0.0
    pfset Geom.domain.Lower.Z                        0.0
     
    pfset Geom.domain.Upper.X                        41000.0
    pfset Geom.domain.Upper.Y                        41000.0
    pfset Geom.domain.Upper.Z                          100.0

    pfset Geom.domain.Patches             "x-lower x-upper y-lower y-upper z-lower z-upper"

Now we setup the indicator file. As noted above, the indicator file has
integer values for every grid cell in the domain designating what
geologic unit it belongs to. The GeomNames list should include a name
for every unit in your indicator file. In this example we have thirteen
soil units and eight geologic units. The FileName points to the
indicator file that ParFlow will read. Recall that this file into the
run directory at the start of the script.

::

    #-----------------------------------------------------------------------------
    # Indicator Geometry Input
    #-----------------------------------------------------------------------------
    pfset GeomInput.indi_input.InputType      IndicatorField
    pfset GeomInput.indi_input.GeomNames      "s1 s2 s3 s4 s5 s6 s7 s8 s9 s10 s11 s12 s13 g1 g2 g3 g4 g5 g6 g7 g8"
    pfset Geom.indi_input.FileName            "IndicatorFile_Gleeson.50z.pfb"

For every name in the GeomNames list we define the corresponding value
in the indicator file. For example, here we are saying that our first
soil unit (s1) is represented by the number \`\`1“ in the indicator
file, while the first geologic unit (g1) is represented by the number
\`\`21”. Note that the integers used in the indicator file do not need
to be consecutive.

::

    pfset GeomInput.s1.Value                1
    pfset GeomInput.s2.Value                2
    pfset GeomInput.s3.Value                3
    pfset GeomInput.s4.Value                4
    pfset GeomInput.s5.Value                5
    pfset GeomInput.s6.Value                6
    pfset GeomInput.s7.Value                7
    pfset GeomInput.s8.Value                8
    pfset GeomInput.s9.Value                9
    pfset GeomInput.s10.Value               10
    pfset GeomInput.s11.Value               11
    pfset GeomInput.s12.Value               12
    pfset GeomInput.s13.Value               13
    pfset GeomInput.g1.Value                21
    pfset GeomInput.g2.Value                22
    pfset GeomInput.g3.Value                23
    pfset GeomInput.g4.Value                24
    pfset GeomInput.g5.Value                25
    pfset GeomInput.g6.Value                26
    pfset GeomInput.g7.Value                27
    pfset GeomInput.g8.Value                28

Now you add permeability data to the domain sections defined above
(§ [Permeability]). You can reassign values simply by re-stating them –
there is no need to comment out or delete the previous version – the
final statement is the only one that counts. Also, note that you do not
need to assign permeability values to all of the geometries names. Any
geometry that is not assigned its own permeability value will take the
domain value. However, every geometry listed in Porosity.GeomNames must
have values assigned.

::

    #-----------------------------------------------------------------------------
    # Permeability (values in m/hr)
    #-----------------------------------------------------------------------------
    pfset Geom.Perm.Names                     "domain s1 s2 s3 s4 s5 s6 s7 s8 s9 g2 g3 g6 g8"

    pfset Geom.domain.Perm.Type           Constant
    pfset Geom.domain.Perm.Value          0.2

    pfset Geom.s1.Perm.Type               Constant
    pfset Geom.s1.Perm.Value              0.269022595

    pfset Geom.s2.Perm.Type               Constant
    pfset Geom.s2.Perm.Value              0.043630356

    pfset Geom.s3.Perm.Type               Constant
    pfset Geom.s3.Perm.Value              0.015841225

    pfset Geom.s4.Perm.Type               Constant
    pfset Geom.s4.Perm.Value              0.007582087

    pfset Geom.s5.Perm.Type               Constant
    pfset Geom.s5.Perm.Value              0.01818816

    pfset Geom.s6.Perm.Type               Constant
    pfset Geom.s6.Perm.Value              0.005009435

    pfset Geom.s7.Perm.Type               Constant
    pfset Geom.s7.Perm.Value              0.005492736

    pfset Geom.s8.Perm.Type               Constant
    pfset Geom.s8.Perm.Value              0.004675077

    pfset Geom.s9.Perm.Type               Constant
    pfset Geom.s9.Perm.Value              0.003386794

    pfset Geom.g2.Perm.Type               Constant
    pfset Geom.g2.Perm.Value              0.025

    pfset Geom.g3.Perm.Type               Constant
    pfset Geom.g3.Perm.Value              0.059

    pfset Geom.g6.Perm.Type               Constant
    pfset Geom.g6.Perm.Value              0.2

    pfset Geom.g8.Perm.Type              Constant
    pfset Geom.g8.Perm.Value             0.68

The following section allows you to specify the permeability tensor. In
the case below, permeability is symmetric in all directions (x, y, and
z) and therefore each is set to 1.0. Also note that we just specify this
once for the whole domain because we want isotropic permeability
everywhere. You can specify different tensors for different units by
repeating these lines with different Geom.Names.

::

    pfset Perm.TensorType                     TensorByGeom
    pfset Geom.Perm.TensorByGeom.Names        "domain"
    pfset Geom.domain.Perm.TensorValX         1.0d0
    pfset Geom.domain.Perm.TensorValY         1.0d0
    pfset Geom.domain.Perm.TensorValZ         1.0d0

Next we set the specific storage. Here again we specify one value for
the whole domain but these lines can be easily repeated to set different
values for different units.

::

    #-----------------------------------------------------------------------------
    # Specific Storage
    #-----------------------------------------------------------------------------
    pfset SpecificStorage.Type                Constant
    pfset SpecificStorage.GeomNames           "domain"
    pfset Geom.domain.SpecificStorage.Value   1.0e-5

ParFlow has the capability to deal with a multiphase system, but we only
have one (water) in this example. As we stated earlier, we set density
and viscosity artificially (and later gravity) both to 1.0. Again, this
is merely a trick to solve for hydraulic conductivity and pressure head.
If you were to set density and viscosity to their true values, the code
would calculate **k** (permeability). By using the *normalized* values
instead, you effectively embed the conversion of **k** to **K**
(hydraulic conductivity). So this way, we get hydraulic conductivity,
which is what we want for this problem.

::

    #-----------------------------------------------------------------------------
    # Phases
    #-----------------------------------------------------------------------------
    pfset Phase.Names                         "water"

    pfset Phase.water.Density.Type            Constant
    pfset Phase.water.Density.Value           1.0

    pfset Phase.water.Viscosity.Type          Constant
    pfset Phase.water.Viscosity.Value         1.0

This example does not include the ParFlow grid based transport scheme.
Therefore we leave contaminants blank.

::

    #-----------------------------------------------------------------------------
    # Contaminants
    #-----------------------------------------------------------------------------
    pfset Contaminants.Names                  ""

As with density and viscosity, gravity is normalized here. If we used
the true value (in the *[L]* and *[T]* units of hydraulic conductivity)
the code would be calculating permeability. Instead, we normalize so
that the code calculates hydraulic conductivity.

::

    #-----------------------------------------------------------------------------
    # Gravity
    #-----------------------------------------------------------------------------
    pfset Gravity                             1.0

Next we set up the timing for our simulation.

::

    #-----------------------------------------------------------------------------
    # Timing (time units is set by units of permeability)
    #-----------------------------------------------------------------------------

This specifies the base unit of time for all time values entered. All
time should be expressed as multiples of this value. To keep things
simple here we set it to 1. Because we expressed our permeability in
units of m/hr in this example this means that our basin unit of time is
1hr.

::

    pfset TimingInfo.BaseUnit                 1.0

This key specifies the time step number that will be associated with the
first advection cycle of the transient problem. Because we are starting
from scratch we set this to 0. If we were restarting a run we would set
this to the last time step of your previous simulation. Refer to
§ [Restarting a Run] for additional instructions on restarting a run.

::

    pfset TimingInfo.StartCount               0.0

StartTime and StopTime specify the start and stop times for the
simulation. These values should correspond with the forcing files you
are using.

::

    pfset TimingInfo.StartTime                0.0
    pfset TimingInfo.StopTime                 72.0

This key specifies the timing interval at which ParFlow time dependent
outputs will be written. Here we have a base unit of 1hr so a dump
interval of 24 means that we are writing daily outputs. Note that this
key only controls the ParFlow output interval and not the interval that
CLM outputs will be written out at.

::

    pfset TimingInfo.DumpInterval             24.0

Here we set the time step value. For this example we use a constant time
step of 1hr.

::

    pfset TimeStep.Type                       Constant
    pfset TimeStep.Value                      1.0

Next, we assign the porosity (*see* § [Porosity]). As with the
permeability we assign different values for different indicator
geometries. Here we assign values for all of our soil units but not for
the geologic units, they will default to the domain value of 0.4. Note
that every geometry listed in Porosity.GeomNames must have values
assigned.

::

    #-----------------------------------------------------------------------------
    # Porosity
    #-----------------------------------------------------------------------------
    pfset Geom.Porosity.GeomNames             "domain s1 s2 s3 s4 s5 s6 s7 s8 s9"

    pfset Geom.domain.Porosity.Type          Constant
    pfset Geom.domain.Porosity.Value         0.4

    pfset Geom.s1.Porosity.Type    Constant
    pfset Geom.s1.Porosity.Value   0.375

    pfset Geom.s2.Porosity.Type    Constant
    pfset Geom.s2.Porosity.Value   0.39

    pfset Geom.s3.Porosity.Type    Constant
    pfset Geom.s3.Porosity.Value   0.387

    pfset Geom.s4.Porosity.Type    Constant
    pfset Geom.s4.Porosity.Value   0.439

    pfset Geom.s5.Porosity.Type    Constant
    pfset Geom.s5.Porosity.Value   0.489

    pfset Geom.s6.Porosity.Type    Constant
    pfset Geom.s6.Porosity.Value   0.399

    pfset Geom.s7.Porosity.Type    Constant
    pfset Geom.s7.Porosity.Value   0.384

    pfset Geom.s8.Porosity.Type            Constant
    pfset Geom.s8.Porosity.Value           0.482

    pfset Geom.s9.Porosity.Type            Constant
    pfset Geom.s9.Porosity.Value           0.442

Having defined the geometry of our problem before and named it domain,
we are now ready to report/upload that problem, which we do here.

::

    #-----------------------------------------------------------------------------
    # Domain
    #-----------------------------------------------------------------------------
    pfset Domain.GeomName                     "domain"

Mobility between phases is set to 1.0 because we only have one phase
(water).

::

    #----------------------------------------------------------------------------
    # Mobility
    #----------------------------------------------------------------------------
    pfset Phase.water.Mobility.Type        Constant
    pfset Phase.water.Mobility.Value       1.0

Again, ParFlow has more capabilities than we are using here in this
example. Note that since there are no well names listed here, ParFlow
assumes we have no wells. If we had pumping wells, we would have to
include them here, because they would affect the head distribution
throughout our domain. See § [Harvey Flow Example] for an example of how
to include pumping wells in this script.

::

    #-----------------------------------------------------------------------------
    # Wells
    #-----------------------------------------------------------------------------
    pfset Wells.Names                         ""

You can give certain periods of time names if you want. For example if
you aren’t running with CLM and you would like to have periods with rain
and periods without. Here, however we have only one time cycle because
CLM will handle the variable forcings. Therefore, we specify one time
cycle and it’s constant for the duration of the simulation. We
accomplish this by giving it a repeat value of -1, which repeats
indefinitely. The length of the cycle is the length specified below (an
integer) multiplied by the base unit value we specified earlier.

::

    #-----------------------------------------------------------------------------
    # Time Cycles
    #-----------------------------------------------------------------------------
    pfset Cycle.Names                         "constant"
    pfset Cycle.constant.Names                "alltime"
    pfset Cycle.constant.alltime.Length        1
    pfset Cycle.constant.Repeat               -1

Now, we assign Boundary Conditions for each face (each of the Patches in
the domain defined before). Recall the previously stated Patches and
associate them with the boundary conditions that follow.

::

    #-----------------------------------------------------------------------------
    # Boundary Conditions
    #-----------------------------------------------------------------------------
    pfset BCPressure.PatchNames                   [pfget Geom.domain.Patches]

The bottom and sides of our domain are all set to no-flow (i.e. constant
flux of 0) boundaries.

::

    pfset Patch.x-lower.BCPressure.Type		      FluxConst
    pfset Patch.x-lower.BCPressure.Cycle		      "constant"
    pfset Patch.x-lower.BCPressure.alltime.Value	      0.0

    pfset Patch.y-lower.BCPressure.Type		      FluxConst
    pfset Patch.y-lower.BCPressure.Cycle		      "constant"
    pfset Patch.y-lower.BCPressure.alltime.Value	      0.0

    pfset Patch.z-lower.BCPressure.Type		      FluxConst
    pfset Patch.z-lower.BCPressure.Cycle		      "constant"
    pfset Patch.z-lower.BCPressure.alltime.Value	      0.0

    pfset Patch.x-upper.BCPressure.Type		      FluxConst
    pfset Patch.x-upper.BCPressure.Cycle		      "constant"
    pfset Patch.x-upper.BCPressure.alltime.Value	      0.0

    pfset Patch.y-upper.BCPressure.Type		      FluxConst
    pfset Patch.y-upper.BCPressure.Cycle		      "constant"
    pfset Patch.y-upper.BCPressure.alltime.Value	      0.0

The top is set to an OverlandFLow boundary to turn on the fully-coupled
overland flow routing.

::

    pfset Patch.z-upper.BCPressure.Type		      OverlandFlow
    pfset Patch.z-upper.BCPressure.Cycle		      "constant"
    pfset Patch.z-upper.BCPressure.alltime.Value	      0.0

Next we define topographic slopes and values. These slope values were
derived from a digital elevation model of the domain following the
workflow outlined in § [Defining a Real domain]. In this example we read
the slope files in from .pfb files that were copied into the run
directory at the start of this script.

::

    #-----------------------------------------------------------------------------
    # Topo slopes in x-direction
    #-----------------------------------------------------------------------------
    pfset TopoSlopesX.Type                                "PFBFile"
    pfset TopoSlopesX.GeomNames                           "domain"
    pfset TopoSlopesX.FileName                            "LW.slopex.pfb"

    #-----------------------------------------------------------------------------
    # Topo slopes in y-direction
    #-----------------------------------------------------------------------------
    pfset TopoSlopesY.Type                                "PFBFile"
    pfset TopoSlopesY.GeomNames                           "domain"
    pfset TopoSlopesY.FileName                            "LW.slopey.pfb"

And now we define the Mannings *n*, again just one value for the whole
domain in this example.

::

    #-----------------------------------------------------------------------------
    # Mannings coefficient
    #-----------------------------------------------------------------------------
    pfset Mannings.Type                                   "Constant"
    pfset Mannings.GeomNames                              "domain"
    pfset Mannings.Geom.domain.Value                      5.52e-6

Following the same approach as we did for Porosity we define the
relative permeability inputs that will be used for Richards’ equation
implementation (§ [Richards RelPerm]). Here we use VanGenuchten
parameters. Note that every geometry listed in Porosity.GeomNames must
have values assigned.

::

    #-----------------------------------------------------------------------------
    # Relative Permeability
    #-----------------------------------------------------------------------------
    pfset Phase.RelPerm.Type                  VanGenuchten
    pfset Phase.RelPerm.GeomNames             "domain s1 s2 s3 s4 s5 s6 s7 s8 s9 "

    pfset Geom.domain.RelPerm.Alpha           3.5
    pfset Geom.domain.RelPerm.N               2.0

    pfset Geom.s1.RelPerm.Alpha        3.548
    pfset Geom.s1.RelPerm.N            4.162

    pfset Geom.s2.RelPerm.Alpha        3.467
    pfset Geom.s2.RelPerm.N            2.738

    pfset Geom.s3.RelPerm.Alpha        2.692
    pfset Geom.s3.RelPerm.N            2.445

    pfset Geom.s4.RelPerm.Alpha        0.501
    pfset Geom.s4.RelPerm.N            2.659

    pfset Geom.s5.RelPerm.Alpha        0.661
    pfset Geom.s5.RelPerm.N            2.659

    pfset Geom.s6.RelPerm.Alpha        1.122
    pfset Geom.s6.RelPerm.N            2.479

    pfset Geom.s7.RelPerm.Alpha        2.089
    pfset Geom.s7.RelPerm.N            2.318

    pfset Geom.s8.RelPerm.Alpha        0.832
    pfset Geom.s8.RelPerm.N            2.514

    pfset Geom.s9.RelPerm.Alpha        1.585
    pfset Geom.s9.RelPerm.N            2.413

Next we do the same thing for saturation (§ [Saturation]) again using
the VanGenuchten parameters Note that every geometry listed in
Porosity.GeomNames must have values assigned.

::

    #-----------------------------------------------------------------------------
    # Saturation
    #-----------------------------------------------------------------------------
    pfset Phase.Saturation.Type               VanGenuchten
    pfset Phase.Saturation.GeomNames          "domain s1 s2 s3 s4 s5 s6 s7 s8 s9 "

    pfset Geom.domain.Saturation.Alpha        3.5
    pfset Geom.domain.Saturation.N            2.
    pfset Geom.domain.Saturation.SRes         0.2
    pfset Geom.domain.Saturation.SSat         1.0

    pfset Geom.s1.Saturation.Alpha        3.548
    pfset Geom.s1.Saturation.N            4.162
    pfset Geom.s1.Saturation.SRes         0.000001
    pfset Geom.s1.Saturation.SSat         1.0

    pfset Geom.s2.Saturation.Alpha        3.467
    pfset Geom.s2.Saturation.N            2.738
    pfset Geom.s2.Saturation.SRes         0.000001
    pfset Geom.s2.Saturation.SSat         1.0

    pfset Geom.s3.Saturation.Alpha        2.692
    pfset Geom.s3.Saturation.N            2.445
    pfset Geom.s3.Saturation.SRes         0.000001
    pfset Geom.s3.Saturation.SSat         1.0

    pfset Geom.s4.Saturation.Alpha        0.501
    pfset Geom.s4.Saturation.N            2.659
    pfset Geom.s4.Saturation.SRes         0.000001
    pfset Geom.s4.Saturation.SSat         1.0

    pfset Geom.s5.Saturation.Alpha        0.661
    pfset Geom.s5.Saturation.N            2.659
    pfset Geom.s5.Saturation.SRes         0.000001
    pfset Geom.s5.Saturation.SSat         1.0

    pfset Geom.s6.Saturation.Alpha        1.122
    pfset Geom.s6.Saturation.N            2.479
    pfset Geom.s6.Saturation.SRes         0.000001
    pfset Geom.s6.Saturation.SSat         1.0

    pfset Geom.s7.Saturation.Alpha        2.089
    pfset Geom.s7.Saturation.N            2.318
    pfset Geom.s7.Saturation.SRes         0.000001
    pfset Geom.s7.Saturation.SSat         1.0

    pfset Geom.s8.Saturation.Alpha        0.832
    pfset Geom.s8.Saturation.N            2.514
    pfset Geom.s8.Saturation.SRes         0.000001
    pfset Geom.s8.Saturation.SSat         1.0

    pfset Geom.s9.Saturation.Alpha        1.585
    pfset Geom.s9.Saturation.N            2.413
    pfset Geom.s9.Saturation.SRes         0.000001
    pfset Geom.s9.Saturation.SSat         1.0

Phase sources allows you to add sources other than wells and boundaries,
but we do not have any so this key is constant, 0.0 over entire domain.

::

    #-----------------------------------------------------------------------------
    # Phase sources:
    #-----------------------------------------------------------------------------
    pfset PhaseSources.water.Type                         "Constant"
    pfset PhaseSources.water.GeomNames                    "domain"
    pfset PhaseSources.water.Geom.domain.Value            0.0

In this example we are using ParFlow CLM so we must provide some
parameters for CLM (§ [CLM Solver Parameters]). Note that CLM will also
require some additional inputs outside of the tcl script. Refer to
/washita/clm:sub:`i`\ nput/ for examples of the CLM vegm and driver
files. These inputs are also discussed briefly in § [Defining a Real
domain].

::

    #----------------------------------------------------------------
    # CLM Settings:
    # ------------------------------------------------------------

First we specify that we will be using CLM as the land surface model and
provide the name of a directory that outputs will be written to. For
this example we do not need outputs for each processor or a binary
output directory. Finally we set the dump interval to 1, indicating that
we will be writing outputs for every time step. Note that this does not
have to match the dump interval for ParFlow outputs. Recall that earlier
we set the ParFlow dump interval to 24.

::

    pfset Solver.LSM                                      CLM
    pfset Solver.CLM.CLMFileDir                           "clm_output/"
    pfset Solver.CLM.Print1dOut                           False
    pfset Solver.BinaryOutDir                             False
    pfset Solver.CLM.CLMDumpInterval                      1

Next we specify the details of the meteorological forcing files that clm
will read. First we provide the name of the files and the directory they
can be found in. Next we specify that we are using 3D forcing files
meaning that we have spatially distributed forcing with multiple time
steps in every file. Therefore we must also specify the number of times
steps (MetFileNT) in every file, in this case 24. Finally, we specify
the initial value for the CLM counter.

::

    pfset Solver.CLM.MetFileName                          "NLDAS"
    pfset Solver.CLM.MetFilePath                          "../../NLDAS/"
    pfset Solver.CLM.MetForcing                           3D
    pfset Solver.CLM.MetFileNT                            24
    pfset Solver.CLM.IstepStart                           1

This last set of CLM parameters refers to the physical properties of the
system. Refer to § [CLM Solver Parameters] for details.

::

    pfset Solver.CLM.EvapBeta                             Linear
    pfset Solver.CLM.VegWaterStress                       Saturation
    pfset Solver.CLM.ResSat                               0.1
    pfset Solver.CLM.WiltingPoint                         0.12
    pfset Solver.CLM.FieldCapacity                        0.98
    pfset Solver.CLM.IrrigationType                       none

Next we set the initial conditions for the domain. In this example we
are using a pressure .pfb file that was obtained by spinning up the
model in the workflow outlined in § [Defining a Real domain].
Alternatively, the water table can be set to a constant value by
changing the ICPressure.Type. Again, the input file that is referenced
here was was copied into the run directory at the top of this script.

::

    #---------------------------------------------------------
    # Initial conditions: water pressure
    #---------------------------------------------------------
    pfset ICPressure.Type                                 PFBFile
    pfset ICPressure.GeomNames                            domain
    pfset Geom.domain.ICPressure.RefPatch                   z-upper
    pfset Geom.domain.ICPressure.FileName                   press.init.pfb

Now we specify what outputs we would like written. In this example we
specify that we would like to write out CLM variables as well as
Pressure and Saturation. However, there are many options for this and
you should change these options according to what type of analysis you
will be performing on your results. A complete list of print options is
provided in § [Code Parameters].

::

    #----------------------------------------------------------------
    # Outputs
    # ------------------------------------------------------------
    #Writing output (all pfb):
    pfset Solver.PrintSubsurfData                         False
    pfset Solver.PrintPressure                            True
    pfset Solver.PrintSaturation                          True
    pfset Solver.PrintMask                                True

    pfset Solver.WriteCLMBinary                           False
    pfset Solver.PrintCLM                                 True
    pfset Solver.WriteSiloSpecificStorage                 False
    pfset Solver.WriteSiloMannings                        False
    pfset Solver.WriteSiloMask                            False
    pfset Solver.WriteSiloSlopes                          False
    pfset Solver.WriteSiloSubsurfData                     False
    pfset Solver.WriteSiloPressure                        False
    pfset Solver.WriteSiloSaturation                      False
    pfset Solver.WriteSiloEvapTrans                       False
    pfset Solver.WriteSiloEvapTransSum                    False
    pfset Solver.WriteSiloOverlandSum                     False
    pfset Solver.WriteSiloCLM                             False

Next we specify the solver settings for the ParFlow (§ [RE Solver
Parameters]). First we turn on solver Richards and the terrain following
grid. We turn off variable dz.

::

    #-----------------------------------------------------------------------------
    # Set solver parameters
    #-----------------------------------------------------------------------------
    # ParFlow Solution
    pfset Solver                                          Richards
    pfset Solver.TerrainFollowingGrid                     True
    pfset Solver.Nonlinear.VariableDz                     False

We then set the max solver settings and linear and nonlinear convergence
tolerance settings. The linear system will be solved to a norm of
:math:`10^{-8}` and the nonlinear system will be solved to less than
:math:`10^{-6}`. Of note in latter key block is the EtaChoice and that
we use the analytical Jacobian (*UseJacobian=\ **True***). We are using
the *FullJacobian* preconditioner, which is a more robust approach but
is more expensive.

::

    pfset Solver.MaxIter                                  25000
    pfset Solver.Drop                                     1E-20
    pfset Solver.AbsTol                                   1E-8
    pfset Solver.MaxConvergenceFailures                   8
    pfset Solver.Nonlinear.MaxIter                        80
    pfset Solver.Nonlinear.ResidualTol                    1e-6

    pfset Solver.Nonlinear.EtaChoice                         EtaConstant
    pfset Solver.Nonlinear.EtaValue                          0.001
    pfset Solver.Nonlinear.UseJacobian                       True 
    pfset Solver.Nonlinear.DerivativeEpsilon                 1e-16
    pfset Solver.Nonlinear.StepTol				 			1e-30
    pfset Solver.Nonlinear.Globalization                     LineSearch
    pfset Solver.Linear.KrylovDimension                      70
    pfset Solver.Linear.MaxRestarts                           2

    pfset Solver.Linear.Preconditioner                       PFMG
    pfset Solver.Linear.Preconditioner.PCMatrixType     FullJacobian

This key is just for testing the Richards’ formulation, so we are not
using it.

::

    #-----------------------------------------------------------------------------
    # Exact solution specification for error calculations
    #-----------------------------------------------------------------------------
    pfset KnownSolution                                   NoKnownSolution

Next we distribute all the inputs as described by the keys in § [PFTCL
Commands]. Note the slopes are 2D files, while the rest of the ParFlow
inputs are 3D so we need to alter the NZ accordingly following example
[dist example] in § [common\ :sub:`p`\ ftcl].

::

    #-----------------------------------------------------------------------------
    # Distribute inputs
    #-----------------------------------------------------------------------------
    pfset ComputationalGrid.NX                41 
    pfset ComputationalGrid.NY                41 
    pfset ComputationalGrid.NZ                1
    pfdist LW.slopex.pfb
    pfdist LW.slopey.pfb

    pfset ComputationalGrid.NX                41 
    pfset ComputationalGrid.NY                41 
    pfset ComputationalGrid.NZ                50 
    pfdist IndicatorFile_Gleeson.50z.pfb
    pfdist press.init.pfb

Now we run the simulation. Note that we use a tcl variable to set the
run name.

::

    #-----------------------------------------------------------------------------
    # Run Simulation
    #-----------------------------------------------------------------------------
    set runname "LW"
    puts $runname
    pfrun    $runname

All that is left is to undistribute files.

::

    #-----------------------------------------------------------------------------
    # Undistribute Files
    #-----------------------------------------------------------------------------
    pfundist $runname
    pfundist press.init.pfb
    pfundist LW.slopex.pfb
    pfundist LW.slopey.pfb
    pfundist IndicatorFile_Gleeson.50z.pfb

    puts "ParFlow run Complete"

Manipulating Data: PFTools
==========================

Introduction to the ParFlow TCL commands (PFTCL) 
-------------------------------------------------

Several tools for manipulating data are provided in PFTCL command set.
Tools can be accessed directly from the TCL shell or within a ParFlow
input script. In both cases you must first load the ParFlow package into
the TCL shell as follows:

::

    #
    # To Import the ParFlow TCL package
    #
    lappend auto_path $env(PARFLOW_DIR)/bin 
    package require parflow
    namespace import Parflow::*

In addition to these methods xpftools provides GUI access to most of
these features. However the simplest approach is generally to include
the tools commands within a tcl script. The following section lists all
of the available ParFlow TCL commands along with detailed instructions
for their use. § [PFTCL Commands] provides several examples of pre and
post processing using the tools. In addition, a list of tools can be
obtained by typing pfhelp into a TCL shell after importing ParFlow.
Typing ¿pfhelp¿ followed by a command name will display a detailed
description of the command in question.

PFTCL Commands
--------------

The tables that follow [pftools1], [pftools2] and [pftools3] provide a
list of ParFlow commands with short descriptions grouped according to
their function. The last two columns in this table indicate what
examples from § [common\ :sub:`p`\ ftcl], if any, the command is used in
and whether the command is compatible with a terrain following grid
domain formulation.

Detailed descriptions of every command are included below in
alphabetical order. Note that the required inputs are listed following
each command. Commands that perform operations on data sets will require
an identifier for each data set it takes as input. Inputs listed in
square brackets are optional and do not need to be provided.

::

    pfaxpy alpha x y

This command computes y = alpha\*x+y where alpha is a scalar and x and y
are identifiers representing data sets. No data set identifier is
returned upon successful completion since data set y is overwritten.

::

    pfbfcvel conductivity phead

This command computes the block face centered flow velocity at every
grid cell. Conductivity and pressure head data sets are given as
arguments. The output includes x, y, and z velocity components that are
appended to the Tcl result.

::

    pfbuilddomain database

This command builds a subgrid array given a ParFlow database that
contains the domain parameters and the processor topology.

::

    pfcelldiff datasetx datasety mask

This command computes cell-wise differences of two datasets
(diff=datasetx-datasety). This is the difference at each individual
cell, not over the domain. Datasets must have the same dimensions.

::

    pfcelldiffconst dataset constant mask

This command subtracts a constant value from each (active) cell of
dataset (dif=dataset - constant).

::

    pfcelldiv datasetx datasety mask

This command computes the cell-wise quotient of datasetx and datasety
(div = datasetx/datasety). This is the quotient at each individual cell.
Datasets must have the same dimensions.

::

    pfcelldivconst dataset constant mask

This command divides each (active) cell of dataset by a constant
(div=dataset/constant).

::

    pfcellmult datasetx datasety mask

This command computes the cell-wise product of datasetx and datasety
(mult = datasetx \* datasety). This is the product at each individual
cell. Datasets must have the same dimensions.

::

    pfcellmultconst dataset constant mask

This command multiplies each (active) cell of dataset by a constant
(mult=dataset \* constant).

::

    pfcellsum datasetp datasetq mask

This command computes the cellwise sum of two datasets (i.e., the sum at
each individual cell, not the sum over the domain). Datasets must have
the same dimensions.

::

    pfcellsumconst dataset constant mask

This command adds the value of constant to each (active) cell of
dataset.

::

    pfchildD8 dem

This command computes the unique D8 child for all cells. Child[i,j] is
the elevation of the cell to which [i,j] drains (i.e. the elevation of
[i,j]’s child). If [i,j] is a local minima the child elevation set the
elevation of [i,j].

::

    pfcomputebottom mask

This command computes the bottom of the domain based on the mask of
active and inactive zones. The identifier of the data set created by
this operation is returned upon successful completion.

::

    pfcomputedomain top bottom

This command computes a domain based on the top and bottom data sets.
The domain built will have a single subgrid per processor that covers
the active data as defined by the top and botttom. This domain will more
closely follow the topology of the terrain than the default single
computation domain.

A typical usage pattern for this is to start with a mask file (zeros in
inactive cells and non-zero in active cells), create the top and bottom
from the mask, compute the domain and then write out the domain. Refer
to example number 3 in the following section.

::

    pfcomputetop mask

This command computes the top of the domain based on the mask of active
and inactive zones. This is the land-surface in clm or overland flow
simulations. The identifier of the data set created by this operation is
returned upon successful completion.

::

    pfcvel conductivity phead

This command computes the Darcy velocity in cells for the conductivity
data set represented by the identifier ‘conductivity’ and the pressure
head data set represented by the identifier ‘phead’. (note: This “cell”
is not the same as the grid cells; its corners are defined by the grid
vertices.) The identifier of the data set created by this operation is
returned upon successful completion.

::

    pfdelete dataset

This command deletes the data set represented by the identifier
‘dataset’. This command can be useful when working with multiple
datasets / time series, such as those created when many timesteps of a
file are loaded and processed. Deleting these datasets in between reads
can help with tcl memory management.

::

    pfdiffelt datasetp datasetq i j k digits [zero]

This command returns the difference of two corresponding coordinates
from ‘datasetp’ and ‘datasetq’ if the number of digits in agreement
(significant digits) differs by more than ‘digits’ significant digits
and the difference is greater than the absolute zero given by ‘zero’.

::

    pfdist filename 

Distribute the file onto the virtual file system. This utility must be
used to create files which ParFlow can use as input. ParFlow uses a
virtual file system which allows each node of the parallel machine to
read from the input file independently. The utility does the inverse of
the pfundist command. If you are using a ParFlow binary file for input
you should do a pfdist just before you do the pfrun. This command
requires that the processor topology and computational grid be set in
the input file so that it knows how to distribute the data. NOTE: When
distributing slope files the NZ must be set to 1 to indicate a two
dimensional file.

::

    pfdistondomain filename domain

Distribute the file onto the virtual file system based on the domain
provided rather than the processor topology as used by pfdist. This is
used by the SAMRAI version of which allows for a more complicated
computation domain specification with different sized subgrids on each
processor and allows for more than one subgrid per processor. Frequently
this will be used with a domain created by the pfcomputedomain command.

::

    pfeffectiverecharge precip et slopex slopey dem

This command computes the effective recharge at every grid cell based on
total precipitation minus evapotranspiration (P-ET)in the upstream area.
Effective recharge is consistent with TOPMODEL definition, NOT local
P-ET. Inputs are total annual (or average annual) precipitation (precip)
at each point, total annual (or average annual) evapotranspiration (ET)
at each point, slope in the x direction, slope in the y direction and
elevation.

::

    pfenlargebox dataset sx sy sz

This command returns a new dataset which is enlarged to be of the new
size indicated by sx, sy and sz. Expansion is done first in the z plane,
then y plane and x plane.

::

    pfextract2Ddomain domain

This command builds a 2D domain based off a 3D domain. This can be used
for a pfdistondomain command for Parflow 2D data (such as slopes and
soil indices).

::

    pfextracttop top data

This command computes the top of the domain based on the top of the
domain and another dataset. The identifier of the data set created by
this operation is returned upon successful completion.

::

    pffillflats dem

This command finds the flat regions in the DEM and eliminates them by
bilinearly interpolating elevations across flat region.

::

    pfflintslaw dem c p

This command smooths the digital elevation model dem according to Flints
Law, with Flints Law parameters specified by c and p, respectively.
Flints Law relates the slope magnitude at a given cell to its upstream
contributing area: S = c\*A\*\*p. In this routine, elevations at local
minima retain the same value as in the original dem. Elevations at all
other cells are computed by applying Flints Law recursively up each
drainage path, starting at its terminus (a local minimum) until a
drainage divide is reached. Elevations are computed as:

dem[i,j] = dem[child] + c\*(A[i,j]\*\*p)\*ds[i,j]

where child is the D8 child of [i,j] (i.e., the cell to which [i,j]
drains according to the D8 method); ds[i,j] is the segment length
between the [i,j] and its child; A[i,j] is the upstream contributing
area of [i,j]; and c and p are constants.

::

    pfflintslawbybasin dem c0 p0 maxiter

This command smooths the digital elevation model (dem) using the same
approach as “pfflints law”. However here the c and p parameters are fit
for each basin separately. The Flint¿s Law parameters are calculated for
the provided digital elevation model dem using the iterative
Levenberg-Marquardt method of non-linear least squares minimization, as
in “pfflintslawfit”. The user must provide initial estimates of c0 and
p0; results are not sensitive to these initial values. The user must
also specify the maximum number of iterations as maxiter.

::

    pfflintslawfit dem c0 p0 maxiter

This command fits Flint’s Law parameters c and p for the provided
digital elevation model dem using the iterative Levenberg-Marquardt
method of non-linear least squares minimization. The user must provide
initial estimates of c0 and p0; results are not sensitive to these
initial values. The user must also specify the maximum number of
iterations as maxiter. Final values of c and p are printed to the
screen, and a dataset containing smoothed elevation values is returned.
Smoothed elevations are identical to running pfflintslaw for the final
values of c and p. Note that dem must be a ParFlow dataset and must have
the correct grid information – dx, dy, nx, and ny are used in parameter
estimation and Flint’s Law calculations. If gridded elevation values are
read in from a text file (e.g., using pfload’s simple ascii format),
grid information must be specified using the pfsetgrid command.

::

    pfflux conductivity hhead

This command computes the net Darcy flux at vertices for the
conductivity data set ‘conductivity’ and the hydraulic head data set
given by ‘hhead’. An identifier representing the flux computed will be
returned upon successful completion.

::

    pfgetelt dataset i j k

This command returns the value at element (i,j,k) in data set ‘dataset’.
The i, j, and k above must range from 0 to (nx - 1), 0 to (ny - 1), and
0 to (nz - 1) respectively. The values nx, ny, and nz are the number of
grid points along the x, y, and z axes respectively. The string
‘dataset’ is an identifier representing the data set whose element is to
be retrieved.

::

    pfgetgrid dataset

This command returns a description of the grid which serves as the
domain of data set ‘dataset’. The format of the description is given
below.

-  ::

       (nx, ny, nz)

   The number of coordinates in each direction.

-  ::

       (x, y, z)

   The origin of the grid.

-  ::

       (dx, dy, dz)

   The distance between each coordinate in each direction.

The above information is returned in the following Tcl list format: nx
ny nz x y z dx dy dz

::

    pfgetlist dataset

This command returns the name and description of a dataset if an
argument is provided. If no argument is given, then all of the data set
names followed by their descriptions is returned to the TCL interpreter.
If an argument (dataset) is given, it should be the it should be the
name of a loaded dataset.

::

    pfgetstats dataset

This command calculates the following statistics for the data set
represented by the identifier ¿dataset¿:minimum, maximum, mean, sum,
variance, and standard deviation.

::

    pfgetsubbox dataset il jl kl iu ju ku

This command computes a new dataset with the subbox starting at il, jl,
kl and going to iu, ju, ku.

::

    pfgridtype gridtype

This command sets the grid type to either cell centered if ‘gridtype’ is
set to ‘cell’ or vetex centered if ‘gridtype’ is set to ‘vertex’. If no
new value for ‘gridtype’ is given, then the current value of ‘gridtype’
is returned. The value of ‘gridtype’ will be returned upon successful
completion of this command.

::

    pfgwstorage mask porosity pressure saturation specific_storage

This command computes the sub-surface water storage (compressible and
incompressible components) based on mask, porosity, saturation,
storativity and pressure fields, similar to pfsubsurfacestorage, but
only for the saturated cells.

::

    pfhelp [command]

This command returns a list of pftools commands. If a command is
provided it gives a detailed description of the command and the
necessary inputs.

::

    pfhhead phead

This command computes the hydraulic head from the pressure head
represented by the identifier ‘phead’. An identifier for the hydraulic
head computed is returned upon successful completion.

::

    pfhydrostatic wtdepth top mask

Compute hydrostatic pressure field from water table depth

::

    pflistdata dataset

This command returns a list of pairs if no argument is given. The first
item in each pair will be an identifier representing the data set and
the second item will be that data set’s label. If a data set’s
identifier is given as an argument, then just that data set’s name and
label will be returned.

::

    pfload [file format] filename

Loads a dataset into memory so it can be manipulated using the other
utilities. A file format may preceed the filename in order to indicate
the file’s format. If no file type option is given, then the extension
of the filename is used to determine the default file type. An
identifier used to represent the data set will be returned upon
successful completion.

File type options include:

-  ::

       pfb

   ParFlow binary format. Default file type for files with a ‘.pfb’
   extension.

-  ::

       pfsb

   ParFlow scattered binary format. Default file type for files with a
   ‘.pfsb’ extension.

-  ::

       sa

   ParFlow simple ASCII format. Default file type for files with a ‘.sa’
   extension.

-  ::

       sb

   ParFlow simple binary format. Default file type for files with a
   ‘.sb’ extension.

-  ::

       silo

   Silo binary format. Default file type for files with a ‘.silo’
   extension.

-  ::

       rsa

   ParFlow real scattered ASCII format. Default file type for files with
   a ‘.rsa’ extension

::

    pfloadsds filename dsnum

This command is used to load Scientific Data Sets from HDF files. The
SDS number ‘dsnum’ will be used to find the SDS you wish to load from
the HDF file ‘filename’. The data set loaded into memory will be
assigned an identifier which will be used to refer to the data set until
it is deleted. This identifier will be returned upon successful
completion of the command.

::

    pfmdiff datasetp datasetq digits [zero]

If ‘digits’ is greater than or equal to zero, then this command computes
the grid point at which the number of digits in agreement (significant
digits) is fewest and differs by more than ‘digits’ significant digits.
If ‘digits’ is less than zero, then the point at which the number of
digits in agreement (significant digits) is minimum is computed.
Finally, the maximum absolute difference is computed. The above
information is returned in a Tcl list of the following form: mi mj mk sd
adiff

Given the search criteria, (mi, mj, mk) is the coordinate where the
minimum number of significant digits ‘sd’ was found and ‘adiff’ is the
maximum absolute difference.

::

    pfmovingaveragedem dem wsize maxiter 

This command fills sinks in the digital elevation model dem by a
standard iterative moving-average routine. Sinks are identified as cells
with zero slope in both x- and y-directions, or as local minima in
elevation (i.e., all adjacent cells have higher elevations). At each
iteration, a moving average is taken over a window of width wsize around
each remaining sink; sinks are thus filled by averaging over neighboring
cells. The procedure continues iteratively until all sinks are filled or
the number of iterations reaches maxiter. For most applications, sinks
should be filled prior to computing slopes (i.e., prior to executing
pfslopex and pfslopey).

::

    pfnewdata {nx ny nz} {x y z} {dx dy dz} label

This command creates a new data set whose dimension is described by the
lists nx ny nz, x y z, and dx dy dz. The first list, describes the
dimensions, the second indicates the origin, and the third gives the
length intervals between each coordinate along each axis. The ‘label’
argument will be the label of the data set that gets created. This new
data set that is created will have all of its data points set to zero
automatically. An identifier for the new data set will be returned upon
successful completion.

::

    pfnewgrid {nx ny nz} {x y z} {dx dy dz} label

Create a new data set whose grid is described by passing three lists and
a label as arguments. The first list will be the number of coordinates
in the x, y, and z directions. The second list will describe the origin.
The third contains the intervals between coordinates along each axis.
The identifier of the data set created by this operation is returned
upon successful completion.

::

    pfnewlabel dataset newlabel

This command changes the label of the data set ‘dataset’ to ‘newlabel’.

::

    pfphead hhead

This command computes the pressure head from the hydraulic head
represented by the identifier ‘hhead’. An identifier for the pressure
head is returned upon successful completion.

::

    pfpitfilldem dem dpit maxiter 

This command fills sinks in the digital elevation model dem by a
standard iterative pit-filling routine. Sinks are identified as cells
with zero slope in both x- and y-directions, or as local minima in
elevation (i.e., all adjacent neighbors have higher elevations). At each
iteration, the value dpit is added to all remaining sinks. The procedure
continues iteratively until all sinks are filled or the number of
iterations reaches maxiter. For most applications, sinks should be
filled prior to computing slopes (i.e., prior to executing pfslopex and
pfslopey).

::

    pfprintdata dataset

This command executes ‘pfgetgrid’ and ‘pfgetelt’ in order to display all
the elements in the data set represented by the identifier ‘dataset’.

::

    pfprintdiff datasetp datasetq digits [zero]

This command executes ‘pfdiffelt’ and ‘pfmdiff’ to print differences to
standard output. The differences are printed one per line along with the
coordinates where they occur. The last two lines displayed will show the
point at which there is a minimum number of significant digits in the
difference as well as the maximum absolute difference.

::

    pfprintdomain domain

This command creates a set of TCL commands that setup a domain as
specified by the provided domain input which can be then be written to a
file for inclusion in a Parflow input script. Note that this kind of
domain is only supported by the SAMRAI version of Parflow.

::

    pfprintelt i j k dataset

This command prints a single element from the provided dataset given an
i, j, k location.

::

    pfprintgrid dataset

This command executes pfgetgrid and formats its output before printing
it on the screen. The triples (nx, ny, nz), (x, y, z), and (dx, dy, dz)
are all printed on seperate lines along with labels describing each.

::

    pfprintlist [dataset]

This command executes pflistdata and formats the output of that command.
The formatted output is then printed on the screen. The output consists
of a list of data sets and their labels one per line if no argument was
given or just one data set if an identifier was given.

::

    pfprintmdiff datasetp datasetq digits [zero]

This command executes ‘pfmdiff’ and formats that command’s output before
displaying it on the screen. Given the search criteria, a line
displaying the point at which the difference has the least number of
significant digits will be displayed. Another line displaying the
maximum absolute difference will also be displayed.

::

    printstats dataset

This command executes ‘pfstats’ and formats that command’s output before
printing it on the screen. Each of the values mentioned in the
description of ‘pfstats’ will be displayed along with a label.

::

    pfreload dataset

This argument reloads a dataset. Only one arguments is required, the
name of the dataset to reload.

::

    pfreloadall

This command reloads all of the current datasets.

::

    pfsattrans mask perm

Compute saturated transmissivity for all [i,j] as the sum of the
permeability[i,j,k]\*dz within a column [i,j]. Currently this routine
uses dz from the input permeability so the dz in permeability must be
correct. Also, it is assumed that dz is constant, so this command is not
compatible with variable dz.

::

    pfsave dataset -filetype filename

This command is used to save the data set given by the identifier
‘dataset’ to a file ‘filename’ of type ‘filetype’ in one of the ParFlow
formats below.

File type options include:

-  pfb ParFlow binary format.

-  sa ParFlow simple ASCII format.

-  sb ParFlow simple binary format.

-  silo Silo binary format.

-  vis Vizamrai binary format.

::

    pfsavediff datasetp datasetq digits [zero] -file filename

This command saves to a file the differences between the values of the
data sets represented by ‘datasetp’ and ‘datasetq’ to file ‘filename’.
The data points whose values differ in more than ‘digits’ significant
digits and whose differences are greater than ‘zero’ will be saved.
Also, given the above criteria, the minimum number of digits in
agreement (significant digits) will be saved.

If ‘digits’ is less than zero, then only the minimum number of
significant digits and the coordinate where the minimum was computed
will be saved.

In each of the above cases, the maximum absolute difference given the
criteria will also be saved.

::

    pfsavesds dataset -filetype filename

This command is used to save the data set represented by the identifier
‘dataset’ to the file ‘filename’ in the format given by ‘filetype’.

The possible HDF formats are:

-  -float32

-  -float64

-  -int8

-  -uint8

-  -int16

-  -uint16

-  -int32

-  -uint32

::

    pfsegmentD8 dem

This command computes the distance between the cell centers of every
parent cell [i,j] and its child cell. Child cells are determined using
the eight-point pour method (commonly referred to as the D8 method)
based on the digital elevation model dem. If [i,j] is a local minima the
segment length is set to zero.

::

    pfsetgrid {nx ny nz} {x0 y0 z0} {dx dy dz} dataset

This command replaces the grid information of dataset with the values
provided.

::

    pfslopeD8 dem

This command computes slopes according to the eight-point pour method
(commonly referred to as the D8 method) based on the digital elevation
model dem. Slopes are computed as the maximum downward gradient between
a given cell and it’s lowest neighbor (adjacent or diagonal). Local
minima are set to zero; where local minima occur on the edge of the
domain, the 1st order upwind slope is used (i.e., the cell is assumed to
drain out of the domain). Note that dem must be a ParFlow dataset and
must have the correct grid information – dx and dy both used in slope
calculations. If gridded elevation values are read in from a text file
(e.g., using pfload’s simple ascii format), grid information must be
specified using the pfsetgrid command. It should be noted that ParFlow
uses slopex and slopey (NOT D8 slopes!) in runoff calculations.

::

    pfslopex dem

This command computes slopes in the x-direction using 1st order upwind
finite differences based on the digital elevation model dem. Slopes at
local maxima (in x-direction) are calculated as the maximum downward
gradient to an adjacent neighbor. Slopes at local minima (in
x-direction) do not drain in the x-direction and are therefore set to
zero. Note that dem must be a ParFlow dataset and must have the correct
grid information – dx in particular is used in slope calculations. If
gridded elevation values are read from a text file (e.g., using pfload’s
simple ascii format), grid inforamtion must be specified using the
pfsetgrid command.

::

    pfslopexD4 dem

This command computes the slope in the x-direction for all [i,j] using a
four point (D4) method. The slope is set to the maximum downward slope
to the lowest adjacent neighbor. If [i,j] is a local minima the slope is
set to zero (i.e. no drainage).

::

    pfslopey dem

This command computes slopes in the y-direction using 1st order upwind
finite differences based on the digital elevation model dem. Slopes at
local maxima (in y-direction) are calculated as the maximum downward
gradient to an adjacent neighbor. Slopes at local minima (in
y-direction) do not drain in the y-direction and are therefore set to
zero. Note that dem must be a ParFlow dataset and must have the correct
grid information - dy in particular is used in slope calculations. If
gridded elevation values are read in from a text file (e.g., using
pfload’s simple ascii format), grid information must be specified using
the pfsetgrid command.

::

    pfslopeyD4 dem

This command computes the slope in the y-direction for all [i,j] using a
four point (D4) method. The slope is set to the maximum downward slope
to the lowest adjacent neighbor. If [i,j] is a local minima the slope is
set to zero (i.e. no drainage).

::

    pfstats dataset

This command prints various statistics for the data set represented by
the identifier ‘dataset’. The minimum, maximum, mean, sum, variance, and
standard deviation are all computed. The above values are returned in a
list of the following form: min max mean sum variance (standard
deviation)

::

    pfsubsurfacestorage mask porosity pressure saturation specific_storage

This command computes the sub-surface water storage (compressible and
incompressible components) based on mask, porosity, saturation,
storativity and pressure fields. The equations used to calculate this
quantity are given in § [Water Balance]. The identifier of the data set
created by this operation is returned upon successful completion.

::

    pfsum dataset

This command computes the sum over the domain of the dataset.

::

    pfsurfacerunoff top slope_x slope_y  mannings pressure

This command computes the surface water runoff (out of the domain) based
on a computed top, pressure field, slopes and mannings roughness values.
This is integrated along all domain boundaries and is calculated at any
location that slopes at the edge of the domain point outward. This data
is in units of :math:`[L^3 T^{-1}]` and the equations used to calculate
this quantity are given in § [Water Balance]. The identifier of the data
set created by this operation is returned upon successful completion.

::

    pfsurfacestorage top pressure

This command computes the surface water storage (ponded water on top of
the domain) based on a computed top and pressure field. The equations
used to calculate this quantity are given in § [Water Balance]. The
identifier of the data set created by this operation is returned upon
successful completion.

::

    pftopodeficit profile m trans dem slopex slopey recharge ssat sres porosity mask

Compute water deficit for all [i,j] based on TOPMODEL/topographic index.
For more details on methods and assumptions refer to toposlopes.c in
pftools.

::

    pftopoindex dem sx sy

Compute topographic index for all [i,j]. Here topographic index is
defined as the total upstream area divided by the contour length,
divided by the local slope. For more details on methods and assumptions
refer to toposlopes.c in pftools.

::

    pftoporecharge riverfile nriver  trans dem sx sy

Compute effective recharge at all [i,j] over upstream area based on
topmodel assumptions and given list of river points. Notes: See detailed
notes in toposlopes.c regarding assumptions, methods, etc. Input Notes:
nriver is an integer (number of river points) river is an array of
integers [nriver][2] (list of river indices, ordered from outlet to
headwaters) is a Databox of saturated transmissivity dem is a Databox of
elevations at each cell sx is a Databox of slopes (x-dir) – lets you use
processed slopes! sy is a Databox of slopes (y-dir) – lets you use
processed slopes!

::

    pftopowt deficit porosity ssat sres mask top wtdepth

Compute water depth from column water deficit for all [i,j] based on
TOPMODEL/topographic index.

::

    pfundist filename, pfundist runname

The command undistributes a ParFlow output file. ParFlow uses a
distributed file system where each node can write to its own file. The
pfundist command takes all of these individual files and collapses them
into a single file.

The arguments can be a runname or a filename. If a runname is given then
all of the output files associated with that run are undistributed.

Normally this is done after every pfrun command.

::

    pfupstreamarea slope_x slope_y

This command computes the upstream area contributing to surface runoff
at each cell based on the x and y slope values provided in datasets
slope\ :sub:`x` and slope\ :sub:`y`, respectively. Contributing area is
computed recursively for each cell; areas are not weighted by slope
direction. Areas are returned as the number of upstream (contributing)
cells; to compute actual area, simply multiply by the cell area
(dx\*dy).

::

    pfvmag datasetx datasety datasetz

This command computes the velocity magnitude when given three velocity
components. The three parameters are identifiers which represent the x,
y, and z components respectively. The identifier of the data set created
by this operation is returned upon successful completion.

::

    pfvtksave dataset filetype filename [options]

This command loads PFB or SILO output, reads a DEM from a file and
generates a 3D VTK output field from that ParFlow output.

The options: Any combination of these can be used and they can be
specified in any order as long as the required elements immediately
follow each option.

-var specifies what the variable written to the dataset will be called.
This is followed by a text string, like “Pressure” or “Saturation” to
define the name of the data that will be written to the VTK. If this
isn’t specified, you’ll get a property written to the file creatively
called “Variable”. This option is ignored if you are using -clmvtk since
all its variables are predefined.

-dem specifies that a DEM is to be used. The argument following -dem
MUST be the handle of the dataset containing the elevations. If it
cannot be found, the tool ignores it and reverts to non-dem mode. If the
nx and ny dimensions of the grids don’t match, the tool will error out.
This option shifts the layers so that the top of the domain coincides
with the land surface defined by the DEM. Regardless of the actual
number of layers in the DEM file, the tool only uses the elevations in
the top layer of this dataset, meaning a 1-layer PFB can be used.

-flt tells the tool to write the data as type float instead of double.
Since the VTKs are really only used for visualization, this reduces the
file size and speeds up plotting.

-tfg causes the tool to override the specified dz in the dataset PFB and
uses a user specified list of layer thicknesses instead. This is
designed for terrain following grids and can only be used in conjunction
with a DEM. The argument following the flag is a text string containing
the number of layers and the dz list of actual layer thicknesses (not dz
multipliers) for each layer from the bottom up such as: -tfg “5 200.0
1.0 0.7 0.2 0.1” Note that the quotation marks around the list are
necessary.

Example:

::

    file copy -force CLM_dem.cpfb CLM_dem.pfb

    set CLMdat [pfload -pfb clm.out.clm_output.00005.C.pfb]
    set Pdat [pfload -pfb clm.out.press.00005.pfb]
    set Perm [pfload -pfb clm.out.perm_x.pfb]
    set DEMdat [pfload -pfb CLM_dem.pfb]

    set dzlist "10 6.0 5.0 0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5"

    pfvtksave $Pdat -vtk "CLM.out.Press.00005a.vtk" -var "Press" 
    pfvtksave $Pdat -vtk "CLM.out.Press.00005b.vtk" -var "Press" -flt 
    pfvtksave $Pdat -vtk "CLM.out.Press.00005c.vtk" -var "Press" -dem $DEMdat 
    pfvtksave $Pdat -vtk "CLM.out.Press.00005d.vtk" -var "Press" -dem $DEMdat -flt
    pfvtksave $Pdat -vtk "CLM.out.Press.00005e.vtk" -var "Press" -dem $DEMdat -flt -tfg $dzlist
    pfvtksave $Perm -vtk "CLM.out.Perm.00005.vtk" -var "Perm" -flt -dem $DEMdat -tfg $dzlist

    pfvtksave $CLMdat -clmvtk "CLM.out.CLM.00005.vtk" -flt 
    pfvtksave $CLMdat -clmvtk "CLM.out.CLM.00005.vtk" -flt -dem $DEMdat

    pfvtksave $DEMdat -vtk "CLM.out.Elev.00000.vtk" -flt -var "Elevation" -dem $DEMdat

::

    pfvvel conductivity phead

This command computes the Darcy velocity in cells for the conductivity
data set represented by the identifier ‘conductivity’ and the pressure
head data set represented by the identifier ‘phead’. The identifier of
the data set created by this operation is returned upon successful
completion.

::

    pfwatertabledepth top saturation 

This command computes the water table depth (distance from top to first
cell with saturation = 1). The identifier of the data set created by
this operation is returned upon successful completion.

::

    pfwritedb runname

This command writes the settings of parflow run to a pfidb database that
can be used to run the model at a later time. In general this command is
used in lieu of the pfrun command.

Common examples using ParFlow TCL commands (PFTCL) 
---------------------------------------------------

This section contains some brief examples of how to use the pftools
commands (along with standard *TCL* commands) to postprocess data.

#. Load a file as one format and write as another format.

   ::

       set press [pfload harvey_flow.out.press.pfb]
       pfsave $press -sa harvey_flow.out.sa

       #####################################################################
       # Also note that PFTCL automatically assigns 
       #identifiers to each data set it stores. In this 
       # example we load the pressure file and assign 
       #it the identifier press. However if you  
       #read in a file called foo.pfb into a TCL shell 
       #with assigning your own identifier, you get 
       #the following:

       #parflow> pfload foo.pfb
       #dataset0

       # In this example, the first line is typed in by the
       #user and the second line is printed out 
       #by PFTCL. It indicates that the data read 
       #from file foo.pfb is associated with the 
       #identifier dataset0.

#. Load pressure-head output from a file, convert to head-potential and
   write out as a new file.

   ::

       set press [pfload harvey_flow.out.press.pfb]
       set head [pfhhead $press]
       pfsave $head -pfb harvey_flow.head.pfb

#. Build a SAMARI compatible domain decomposition based off of a mask
   file

   ::

       #---------------------------------------------------------
       # This example script takes 3 command line arguments
       # for P,Q,R and then builds a SAMRAI compatible
       # domain decomposition based off of a mask file.
       #---------------------------------------------------------

       # Processor Topology
       set P [lindex $argv 0]
       set Q [lindex $argv 1]
       set R [lindex $argv 2]
       pfset Process.Topology.P $P
       pfset Process.Topology.Q $Q
       pfset Process.Topology.R $R

       # Computational Grid
       pfset ComputationalGrid.Lower.X -10.0
       pfset ComputationalGrid.Lower.Y 10.0
       pfset ComputationalGrid.Lower.Z 1.0

       pfset ComputationalGrid.DX 8.8888888888888893
       pfset ComputationalGrid.DY 10.666666666666666
       pfset ComputationalGrid.DZ 1.0

       pfset ComputationalGrid.NX 10
       pfset ComputationalGrid.NY 10
       pfset ComputationalGrid.NZ 8

       # Calculate top and bottom and build domain
       set mask [pfload samrai.out.mask.pfb]
       set top [pfcomputetop $mask]
       set bottom [pfcomputebottom $mask]

       set domain [pfcomputedomain $top $bottom]
       set out [pfprintdomain $domain]
       set grid\_file [open samrai_grid.tcl w]

       puts $grid_file $out
       close $grid_file

       #---------------------------------------------------------
       # The resulting TCL file samrai_grid.tcl may be read into
       # a Parflow input file using ¿¿source samrai_grid.tcl¿¿.
       #---------------------------------------------------------

#. Distributing input files before running [dist example]

   ::

       #--------------------------------------------------------
       # A common problem for new ParFlow users is to 
       # distribute slope files using 
       # the 3-D computational grid that is
       # set at the begging of a run script. 
       # This results in errors because slope 
       # files are 2-D. 
       # To avoid this problem the computational 
       # grid should be reset before and after
       # distributing slope files. As follows:
       #---------------------------------------------------------

       #First set NZ to 1 and distribute the 2D slope files
       pfset ComputationalGrid.NX                40 
       pfset ComputationalGrid.NY                40 
       pfset ComputationalGrid.NZ                1
       pfdist slopex.pfb
       pfdist slopey.pfb

       #Reset NZ to the correct value and distribute any 3D inputs
       pfset ComputationalGrid.NX                40 
       pfset ComputationalGrid.NY                40 
       pfset ComputationalGrid.NZ                50 
       pfdist IndicatorFile.pfb

#. Calculate slopes from an elevation file

   ::

       #Read in DEM
       set dem [pfload -sa dem.txt]
       pfsetgrid {209 268 1} {0.0 0.0 0.0} {100 100 1.0} $dem

       # Fill flat areas (if any)
       set flatfill [pffillflats $dem]

       # Fill pits (if any)
       set  pitfill [pfpitfilldem $flatfill 0.01 10000]

       # Calculate Slopes
       set  slope_x [pfslopex $pitfill]
       set  slope_y [pfslopey $pitfill]

       # Write to output...
       pfsave $flatfill -silo klam.flatfill.silo
       pfsave $pitfill  -silo klam.pitfill.silo
       pfsave $slope_x  -pfb  klam.slope_x.pfb
       pfsave $slope_y  -pfb  klam.slope_y.pfb

#. Calculate and output the *subsurface storage* in the domain at a
   point in time.

   ::

       set saturation [pfload runname.out.satur.00001.silo]
       set pressure [pfload runname.out.press.00001.silo]
       set specific_storage [pfload runname.out.specific_storage.silo]
       set porosity [pfload runname.out.porosity.silo]
       set mask [pfload runname.out.mask.silo]

       set subsurface_storage [pfsubsurfacestorage $mask $porosity \
       $pressure $saturation $specific_storage]
       set total_subsurface_storage [pfsum $subsurface_storage]
       puts [format "Subsurface storage\t\t\t\t : %.16e" $total_subsurface_storage]

#. Calculate and output the *surface storage* in the domain at a point
   in time.

   ::

       set pressure [pfload runname.out.press.00001.silo]
       set mask [pfload runname.out.mask.silo]
       set top [pfcomputetop $mask]
       set surface_storage [pfsurfacestorage $top $pressure]
       set total_surface_storage [pfsum $surface_storage]
       puts [format "Surface storage\t\t\t\t : %.16e" $total_surface_storage]

#. Calculate and output the runoff out of the *entire domain* over a
   timestep.

   ::

       set pressure [pfload runname.out.press.00001.silo]
       set slope_x [pfload runname.out.slope_x.silo]
       set slope_y [pfload runname.out.slope_y.silo]
       set mannings [pfload runname.out.mannings.silo]
       set mask [pfload runname.out.mask.silo]
       set top [pfcomputetop $mask]

       set surface_runoff [pfsurfacerunoff $top $slope_x $slope_y $mannings $pressure]
       set total_surface_runoff [expr [pfsum $surface_runoff] * [pfget TimeStep.Value]]
       puts [format "Surface runoff from pftools\t\t\t : %.16e" $total_surface_runoff]

#. Calculate overland flow at a point using *Manning’s* equation

   ::

       #Set the location
       set Xloc 2
       set Yloc 2
       set Zloc 50  #This should be a z location on the surface of your domain

       #Set the grid dimension and Mannings roughness coefficient
       set dx  1000.0
       set n   0.000005

       #Get the slope at the point
       set slopex   [pfload runname.out.slope_x.pfb]
       set slopey   [pfload runname.out.slope_y.pfb]
       set sx1 [pfgetelt $slopex $Xloc $Yloc 0]
       set sy1 [pfgetelt $slopey $Xloc $Yloc 0]
       set S [expr ($sx**2+$sy**2)**0.5]

       #Get the pressure at the point
       set press [pfload runname.out.press.00001.pfb]
       set P [pfgetelt $press $Xloc $Yloc $Zloc]

       #If the pressure is less than zero set to zero
       if {$P < 0} { set P 0 }
       set QT [expr ($dx/$n)*($S**0.5)*($P**(5./3.))]
       puts $QT

Model Equations
===============

In this chapter, we discuss the model equations used by ParFlow for its
fully and variably saturated flow, overland flow, and multiphase flow
and transport models. First, section [Steady-State, Saturated
Groundwater Flow] describes steady-state, groundwater flow (specified by
solver **IMPES**). Next, section [Richards’ Equation] describes the
Richards’ equation model (specified by solver **RICHARDS**) for variably
saturated flow as implemented in ParFlow. Section [TFG] describes the
terrain following grid formulation. Next, the overland flow equations
are presented in section [Overland Flow]. In section [Multi-Phase Flow
Equations] we describe the multi-phase flow equations (specified by
solver **IMPES**), and in section [Transport Equations] we describe the
transport equations. Finally, section [Notation and Units] presents some
notation and units and section [Water Balance] presents some basic water
balance equations.

Steady-State, Saturated Groundwater Flow
----------------------------------------

Many groundwater problems are solved assuming steady-state,
fully-saturated groundwater flow. This follows the form often written
as:

.. math::

   \begin{aligned}
   {\nabla \cdot}\textbf{q} = Q(x)
   \label{eq:ssgw}\end{aligned}

where :math:`Q` is the spatially-variable source-sink term (to represent
wells, etc) and :math:`\textbf{q}` is the Darcy flux
:math:`[L^{2}T^{-1}]` which is commonly written as:

.. math::

   \begin{aligned}
   \textbf{q}=- \textbf{K} {\nabla}H
   \label{eq:Darcy}\end{aligned}

where :math:`\textbf{K}` is the saturated, hydraulic conductivity tensor
:math:`[LT^{-1}]` and :math:`H` :math:`[L]` is the head-potential.
Inspection of [eqn-mass-balance] and [eqn-darcy] show that these
equations agree with the above formulation for a single-phase
(:math:`i=1`), fully-saturated (:math:`S_i=S=1`), problem where the
mobility, :math:`{{{\lambda}}_i}`, is set to the saturated hydraulic
conductivity, :math:`\textbf{K}`, below. This is accomplished by setting
the relative permeability and viscosity terms to unity in
[eqn-phase-mobility] as well as the gravity and density terms in
[eqn-darcy]. This is shown in the example in § [Tutorial], but please
note that the resulting solution is in pressure-head, :math:`h`, not
head potential, :math:`H`, and will still contain a hydrostatic pressure
gradient in the :math:`z` direction.

Richards’ Equation
------------------

The form of Richards’ equation implemented in ParFlow is given as,

.. math::

   \begin{aligned}
   S(p)S_s\frac{\partial p}{\partial t} -
   \frac{\partial (S(p)\rho(p)\phi)}{\partial t} 
   - {\nabla \cdot}(\textbf{K}(p)\rho(p)(\nabla p - \rho(p) {{\vec g}})) = Q, \;  {\rm in} \; \Omega, 
   \label{eq:richard}\end{aligned}

where :math:`\Omega` is the flow domain, :math:`p` is the pressure-head
of water :math:`[L]`, :math:`S` is the water saturation, :math:`S_s` is
the specific storage coefficient :math:`[L^{-1}]`, :math:`\phi` is the
porosity of the medium, :math:`\textbf{K}(p)` is the hydraulic
conductivity tensor :math:`[LT^{-1}]`, and :math:`Q` is the water
source/sink term :math:`[L^{3}T^{-1}]` (includes wells and surface
fluxes). The hydraulic conductivity can be written as,

.. math::

   \begin{aligned}
   K(p) =  \frac{{{\bar k}}k_r(p)}{\mu}\end{aligned}

Boundary conditions can be stated as,

.. math::

   \begin{aligned}
   p & = & p_D, \; {\rm on} \; \Gamma^D, \label{eq:bcd} \\
   -K(p)\nabla p \cdot {\bf n} & = & 
   g_N, \; {\rm on} \; \Gamma^N, \label{eq:bcn}\end{aligned}

where :math:`\Gamma^D \cup \Gamma^N = \partial \Omega`,
:math:`\Gamma^D \neq \emptyset`, and :math:`{\bf n}` is an outward
pointing, unit, normal vector to :math:`\Omega`. This is the mixed form
of Richards’ equation. Note here that due to the constant (or passive)
air phase pressure assumption, Richards’ equation ignores the air phase
except through its effects on the hydraulic conductivity, :math:`K`. An
initial condition,

.. math::

   \begin{aligned}
   p = p^0(x), \; t = 0,\end{aligned}

completes the specification of the problem.

Terrain Following Grid
----------------------

The terrain following grid formulation transforms the ParFlow grid to
conform to topography . This alters the form of Darcy’s law to include a
topographic slope component:

.. math::

   \begin{aligned}
   q_x=\textbf{K}(p)\rho(p)(\frac{\partial p}{\partial x}\cos \theta_x + \sin \theta_x) 
   \label{eq:darcyTFG}\end{aligned}

where :math:`\theta_x = \arctan(S_0,x)` and
:math:`\theta_y = \arctan(S_0,y)` which are assumed to be the same as
the **TopoSlope** keys assigned for overland flow, described below. The
terrain following grid formulation can be very useful for coupled
surface-subsurface flow problems where groundwater flow follows the
topography. As cells are distributed near the ground surface and can be
combined with the variable :math:`\delta Z` capability, the number of
cells in the problem can be reduced dramatically over the orthogonal
formulation. For complete details on this formulation, the stencil used
and the function evaluation developed, please see .

Overland Flow
-------------

As detailed in , ParFlow may simulate fully-coupled surface and
subsurface flow via an overland flow boundary condition. While complete
details of this approach are given in that paper, a brief summary of the
equations solved are presented here. Shallow overland flow is now
represented in ParFlow by the kinematic wave equation. In two spatial
dimensions, the continuity equation can be written as:

.. math::

   \begin{aligned}
   \frac{\partial \psi_s}{\partial t} =
   {\nabla \cdot}({{\vec v}}\psi_s) + q_r(x)
   \label{eq:kinematic}\end{aligned}

where :math:`{{\vec v}}` is the depth averaged velocity vector
:math:`[LT^{-1}]`; :math:`\psi_s` is the surface ponding depth
:math:`[L]` and :math:`q_r(x)` is the a general source/sink (e.g.
rainfall) rate :math:`[LT^{-1}]`. If diffusion terms are neglected the
momentum equation can be written as:

.. math::

   \begin{aligned}
   S_{f,i} = S_{o,i}
   \label{eq:ovmom}\end{aligned}

which is commonly referred to as the kinematic wave approximation. In
Equation [eq:ovmom] :math:`S_{o,i}` is the bed slope (gravity forcing
term) :math:`[-]`, which is equal to the friction slope :math:`S_{f,i}`
:math:`[L]`; :math:`i` stands for the :math:`x`- and
:math:`y`-direction. Manning’s equation is used to establish a flow
depth-discharge relationship:

.. math::

   \begin{aligned}
   v_x=- \frac{\sqrt{S_{f,x}}}{n}\psi_{s}^{2/3}
   \label{eq:manningsx}\end{aligned}

and

.. math::

   \begin{aligned}
   v_y=- \frac{\sqrt{S_{f,y}}}{n}\psi_{s}^{2/3}
   \label{eq:manningsy}\end{aligned}

where :math:`n` :math:`[TL^{-1/3}]` is the Manning’s coefficient.Though
complete details of the coupled approach are given in , brief details of
the approach are presented here. The coupled approach takes Equation
[eq:kinematic] and adds a flux for subsurface exchanges, :math:`q_e(x)`.

.. math::

   \begin{aligned}
   \frac{\partial \psi_s}{\partial t} =
   {\nabla \cdot}({{\vec v}}\psi_s) + q_r(x) + q_e(x)
   \label{eq:kinematic_ex}\end{aligned}

We then assign a continuity of pressure at the top cell of the boundary
between the surface and subsurface systems by setting pressure-head,
:math:`p` in [eq:richard] equal to the vertically-averaged surface
pressure, :math:`\psi_s` as follows:

.. math::

   \begin{aligned}
   p = \psi_s = \psi
   \label{eq:press_cont}\end{aligned}

If we substitute this relationship back into Equation
[eq:kinematic:sub:`e`\ x] as follows:

.. math::

   \begin{aligned}
   \frac{\partial \parallel\psi,0\parallel}{\partial t} =
   {\nabla \cdot}({{\vec v}}\parallel\psi,0\parallel) + q_r(x) + q_e(x)
   \label{eq:OF_BC_ex}\end{aligned}

Where the :math:`\parallel\psi,0\parallel` operator chooses the greater
of the two quantities, :math:`\psi` and :math:`0`. We may now solve this
term for the flux :math:`q_e(x)` which we may set equal to flux boundary
condition shown in Equation [eq:bcn]. This yields the following
equation, which is referred to as the overland flow boundary condition :

.. math::

   \begin{aligned}
   -K(\psi)\nabla \psi \cdot {\bf n}  = \frac{\partial \parallel\psi,0\parallel}{\partial t} -
   {\nabla \cdot}({{\vec v}}\parallel\psi,0\parallel) - q_r(x)
   \label{eq:overland_bc}\end{aligned}

This results a version of the kinematic wave equation that is only
active when the pressure at the top cell of the subsurface domain has a
ponded depth and is thus greater than zero. This method solves both
systems, where active in the domain, over common grids in a
fully-integrated, fully-mass conservative manner.

Multi-Phase Flow Equations
--------------------------

The flow equations are a set of *mass balance* and *momentum balance*
(Darcy’s Law) equations, given respectively by,

.. math::

   \label{eqn-mass-balance}
   \frac{\partial}{\partial t} ( {\phi}{{S}_i})
     ~+~ {\nabla}\cdot {{{\vec V}}_i}~-~ {{Q}_i}~=~ 0 ,

.. math::

   \label{eqn-darcy}
   {{{\vec V}}_i}~+~ {{{\lambda}}_i}\cdot ( {\nabla}{{p}_i}~-~ {{\rho}_i}{{\vec g}}) ~=~ 0 ,

for :math:`i = 0, \ldots , {n_p}- 1` :math:`({n_p}\in \{1,2,3\})`, where

.. math::

   \begin{aligned}
    \label{eqn-phase-mobility}
   {{{\lambda}}_i}& = & \frac{{{\bar k}}{k_{ri}}}{{{\mu}_i}} , \\
   {{\vec g}}& = & [ 0, 0, -g ]^T ,\end{aligned}

Table [table-flow-units] defines the symbols in the above equations, and
outlines the symbol dependencies and units.

| \|\|c\|\|c\|c\|\| symbol & quantity & units
| :math:`{\phi}({{\vec x}},t)` & porosity & []
| :math:`{{S}_i}({{\vec x}},t)` & saturation & []
| :math:`{{{\vec V}}_i}({{\vec x}},t)` & Darcy velocity vector &
[:math:`L T^{-1}`\ ]
| :math:`{{Q}_i}({{\vec x}},t)` & source/sink & [:math:`T^{-1}`\ ]
| :math:`{{{\lambda}}_i}` & mobility & [:math:`L^{3} T M^{-1}`\ ]
| :math:`{{p}_i}({{\vec x}},t)` & pressure & [:math:`M L^{-1} T^{-2}`\ ]
| :math:`{{\rho}_i}` & mass density & [:math:`M L^{-3}`\ ]
| :math:`{{\vec g}}` & gravity vector & [:math:`L T^{-2}`\ ]
| :math:`{{\bar k}}({{\vec x}},t)` & intrinsic permeability tensor &
[:math:`L^{2}`\ ]
| :math:`{k_{ri}}({{\vec x}},t)` & relative permeability & []
| :math:`{{\mu}_i}` & viscosity & [:math:`M L^{-1} T^{-1}`\ ]
| :math:`g` & gravitational acceleration & [:math:`L T^{-2}`\ ]

[table-flow-units]

Here, :math:`{\phi}` describes the fluid capacity of the porous medium,
and :math:`{{S}_i}` describes the content of phase :math:`i` in the
porous medium, where we have that :math:`0 \le {\phi}\le 1` and
:math:`0 \le {{S}_i}\le 1`. The coefficient :math:`{{\bar k}}` is
considered a scalar here. We also assume that :math:`{{\rho}_i}` and
:math:`{{\mu}_i}` are constant. Also note that in ParFlow, we assume
that the relative permeability is given as :math:`{k_{ri}}({{S}_i})`.
The Darcy velocity vector is related to the *velocity vector*,
:math:`{{{\vec v}}_i}`, by the following:

.. math::

   \label{eqn-Dvec-vs-vvec}
   {{{\vec V}}_i}= {\phi}{{S}_i}{{{\vec v}}_i}.

To complete the formulation, we have the following :math:`{n_p}`
*consititutive relations*

.. math::

   \label{eqn-constitutive-sum}
   \sum_i {{S}_i}= 1 ,

.. math::

   \label{eqn-constitutive-capillary}
   {p}_{i0} ~=~ {p}_{i0} ( {S}_0 ) ,
   ~~~~~~ i = 1 , \ldots , {n_p}- 1 .

where, :math:`{p}_{ij} = {p}_i - {p}_j` is the *capillary pressure*
between phase :math:`i` and phase :math:`j`. We now have the
:math:`3 {n_p}` equations, ([eqn-mass-balance]), ([eqn-darcy]),
([eqn-constitutive-sum]), and ([eqn-constitutive-capillary]), in the
:math:`3 {n_p}` unknowns, :math:`{{S}_i}, {{{\vec V}}_i}`, and
:math:`{{p}_i}`.

For technical reasons, we want to rewrite the above equations. First, we
define the *total mobility*, :math:`{{{\lambda}}_T}`, and the *total
velocity*, :math:`{{{\vec V}}_T}`, by the relations

.. math::

   \begin{aligned}
   {{{\lambda}}_T}~=~ \sum_{i} {{{\lambda}}_i}, \label{eqn-total-mob} \\
   {{{\vec V}}_T}~=~ \sum_{i} {{{\vec V}}_i}.     \label{eqn-total-vel}\end{aligned}

After doing a bunch of algebra, we get the following equation for
:math:`{p}_0`:

.. math::

   \label{eqn-pressure}
   -~ \sum_{i}
     \left \{
       {\nabla}\cdot {{{\lambda}}_i}\left ( {\nabla}( {p}_0 ~+~ {p}_{i0} ) ~-~ {{\rho}_i}{{\vec g}}\right )
       ~+~
       {{Q}_i}\right \}
   ~=~ 0 .

After doing some more algebra, we get the following :math:`{n_p}- 1`
equations for :math:`{{S}_i}`:

.. math::

   \label{eqn-saturation}
   \frac{\partial}{\partial t} ( {\phi}{{S}_i})
   ~+~
   {\nabla}\cdot
     \left (
        \frac{{{{\lambda}}_i}}{{{{\lambda}}_T}} {{{\vec V}}_T}~+~
        \sum_{j \neq i} \frac{{{{\lambda}}_i}{{\lambda}}_j}{{{{\lambda}}_T}} ( {\rho}_i - {\rho}_j ) {{\vec g}}\right )
   ~+~
   \sum_{j \neq i} {\nabla}\cdot
       \frac{{{{\lambda}}_i}{{\lambda}}_j}{{{{\lambda}}_T}} {\nabla}{p}_{ji}
   ~-~ {{Q}_i}~=~ 0 .

The capillary pressures :math:`{p}_{ji}` in ([eqn-saturation]) are
rewritten in terms of the constitutive relations in
([eqn-constitutive-capillary]) so that we have

.. math::

   \label{eqn-derived-capillary}
   {p}_{ji} ~=~ {p}_{j0} ~-~ {p}_{i0} ,

where by definition, :math:`{p}_{ii} = 0`. Note that equations
([eqn-saturation]) are analytically the same equations as in
([eqn-mass-balance]). The reason we rewrite them in this latter form is
because of the numerical scheme we are using. We now have the
:math:`3 {n_p}` equations, ([eqn-pressure]), ([eqn-saturation]),
([eqn-total-vel]), ([eqn-darcy]), and ([eqn-constitutive-capillary]), in
the :math:`3 {n_p}` unknowns, :math:`{{S}_i}, {{{\vec V}}_i}`, and
:math:`{{p}_i}`.

Transport Equations
-------------------

The transport equations in ParFlow are currently defined as follows:

.. math::

   \begin{aligned}
    \label{eqn-transport}
   \left ( \frac{\partial}{\partial t} ({\phi}{{c}_{i,j}}) ~+~ {\lambda_j}~ {\phi}{{c}_{i,j}}\right ) & + & {\nabla}\cdot \left ( {{c}_{i,j}}{{{\vec V}}_i}\right ) \nonumber \\
   & = & \\
   -\left ( \frac{\partial}{\partial t} ((1 - {\phi}) {\rho_{s}}{{F}_{i,j}}) ~+~  {\lambda_j}~ (1 - {\phi}) {\rho_{s}}{{F}_{i,j}}\right ) & + & \sum_{k}^{{n_{I}}} {\gamma^{I;i}_{k}}\chi_{{\Omega^{I}_{k}}} \left ( {{c}_{i,j}}- {{\bar c}^{k}_{ij}}\right ) ~-~ \sum_{k}^{{n_{E}}} {\gamma^{E;i}_{k}}\chi_{{\Omega^{E}_{k}}} {{c}_{i,j}}\nonumber\end{aligned}

where :math:`i = 0, \ldots , {n_p}- 1` :math:`({n_p}\in \{1,2,3\})` is
the number of phases, :math:`j = 0, \ldots , {n_c}- 1` is the number of
contaminants, and where :math:`{{c}_{i,j}}` is the concentration of
contaminant :math:`j` in phase :math:`i`. Recall also, that
:math:`\chi_A` is the characteristic function of set :math:`A`, i.e.
:math:`\chi_A(x) = 1` if :math:`x \in A` and :math:`\chi_A(x) = 0` if
:math:`x \not\in A`. Table [table-transport-units] defines the symbols
in the above equation, and outlines the symbol dependencies and units.
The equation is basically a statement of mass conservation in a
convective flow (no diffusion) with adsorption and degradation effects
incorporated along with the addition of injection and extraction wells.

| \|\|c\|\|c\|c\|\| symbol & quantity & units
| :math:`{\phi}({{\vec x}})` & porosity & []
| :math:`{{c}_{i,j}}({{\vec x}},t)` & concentration fraction & []
| :math:`{{{\vec V}}_i}({{\vec x}},t)` & Darcy velocity vector &
[:math:`L T^{-1}`\ ]
| :math:`{\lambda_j}` & degradation rate & [:math:`T^{-1}`\ ]
| :math:`{\rho_{s}}({{\vec x}})` & density of the solid mass &
[:math:`M L^{-3}`\ ]]
| :math:`{{F}_{i,j}}({{\vec x}}, t)` & mass concentration &
[:math:`L^{3} M^{-1}`\ ]
| :math:`{n_{I}}` & number of injection wells & []
| :math:`{\gamma^{I;i}_{k}}(t)` & injection rate & [:math:`T^{-1}`\ ]
| :math:`{\Omega^{I}_{k}}({{\vec x}})` & injection well region & []
| :math:`{{\bar c}^{k}_{ij}}()` & injected concentration fraction & []
| :math:`{n_{E}}` & number of extraction wells & []
| :math:`{\gamma^{E;i}_{k}}(t)` & extraction rate & [:math:`T^{-1}`\ ]
| :math:`{\Omega^{E}_{k}}({{\vec x}})` & extraction well region & []

[table-transport-units]

These equations will soon have to be generalized to include a diffusion
term. At the present time, as an adsorption model, we take the mass
concentration term (:math:`{{F}_{i,j}}`) to be instantaneous in time and
a linear function of contaminant concentration :

.. math::

   \label{eqn-linear-retardation}
   {{F}_{i,j}}= {K_{d;j}}{{c}_{i,j}},

where :math:`{K_{d;j}}` is the distribution coefficient of the component
([:math:`L^{3} M^{-1}`\ ]). If [eqn-linear-retardation] is substituted
into [eqn-transport] the following equation results (which is the
current model used in ParFlow) :

.. math::

   \begin{aligned}
    \label{eqn-transport2}
   ({\phi}+ (1 - {\phi}) {\rho_{s}}{K_{d;j}}) \frac{\partial}{\partial t} {{c}_{i,j}}& + & {\nabla}\cdot \left ( {{c}_{i,j}}{{{\vec V}}_i}\right ) \nonumber \\
   & = & \nonumber \\
   -~({\phi}+ (1 - {\phi}) {\rho_{s}}{K_{d;j}}) {\lambda_j}{{c}_{i,j}}& + & \sum_{k}^{{n_{I}}} {\gamma^{I;i}_{k}}\chi_{{\Omega^{I}_{k}}} \left ( {{c}_{i,j}}- {{\bar c}^{k}_{ij}}\right ) ~-~ \sum_{k}^{{n_{E}}} {\gamma^{E;i}_{k}}\chi_{{\Omega^{E}_{k}}} {{c}_{i,j}}\end{aligned}

Notation and Units
------------------

In this section, we discuss other common formulations of the flow and
transport equations, and how they relate to the equations solved by
ParFlow.

We can rewrite equation ([eqn-darcy]) as

.. math::

   \label{eqn-darcy-b}
   {{{\vec V}}_i}~+~ {{{\bar K}}_i}\cdot ( {\nabla}{{h}_i}~-~ \frac{{{\rho}_i}}{{\gamma}} {{\vec g}}) ~=~ 0 ,

where

.. math::

   \begin{aligned}
    \label{eqn-cond-phead}
   {{{\bar K}}_i}& = & {\gamma}{{{\lambda}}_i}, \\
   {{h}_i}& = & ( {{p}_i}~-~ {\bar{p}}) / {\gamma}.\end{aligned}

Table [table-flow-units-b] defines the symbols and their units.

| \|\|c\|\|c\|c\|\| symbol & quantity & units
| :math:`{{{\vec V}}_i}` & Darcy velocity vector & [:math:`L T^{-1}`\ ]
| :math:`{{{\bar K}}_i}` & hydraulic conductivity tensor &
[:math:`L T^{-1}`\ ]
| :math:`{{h}_i}` & pressure head & [:math:`L`\ ]
| :math:`{\gamma}` & constant scale factor & [:math:`M L^{-2} T^{-2}`\ ]
| :math:`{{\vec g}}` & gravity vector & [:math:`L T^{-2}`\ ]

[table-flow-units-b]

We can then rewrite equations ([eqn-pressure]) and ([eqn-saturation]) as

.. math::

   \label{eqn-pressure-b}
   -~ \sum_{i}
     \left \{
       {\nabla}\cdot {{{\bar K}}_i}\left ( {\nabla}( {h}_0 ~+~ {h}_{i0} ) ~-~
           \frac{{{\rho}_i}}{{\gamma}} {{\vec g}}\right )
       ~+~
       {{Q}_i}\right \}
   ~=~ 0 ,

.. math::

   \label{eqn-saturation-b}
   \frac{\partial}{\partial t} ( {\phi}{{S}_i})
   ~+~
   {\nabla}\cdot
     \left (
        \frac{{{{\bar K}}_i}}{{{{\bar K}}_T}} {{{\vec V}}_T}~+~
        \sum_{j \neq i} \frac{{{{\bar K}}_i}{{\bar K}}_j}{{{{\bar K}}_T}}
          \left ( \frac{{\rho}_i}{{\gamma}} - \frac{{\rho}_j}{{\gamma}} \right ) {{\vec g}}\right )
   ~+~
   \sum_{j \neq i} {\nabla}\cdot
       \frac{{{{\bar K}}_i}{{\bar K}}_j}{{{{\bar K}}_T}} {\nabla}{h}_{ji}
   ~-~ {{Q}_i}~=~ 0 .

Note that :math:`{{{\bar K}}_i}` is supposed to be a tensor, but we
treat it as a scalar here. Also, note that by carefully defining the
input to ParFlow, we can use the units of equations ([eqn-pressure-b])
and ([eqn-saturation-b]). To be more precise, let us denote ParFlow
input symbols by appending the symbols in table [table-flow-units] with
:math:`(I)`, and let :math:`{\gamma}= {\rho}_0 g` (this is a typical
definition). Then, we want:

.. math::

   \begin{aligned}
    \label{eqn-parflow-input}
   {{\bar k}}(I)    & = & {\gamma}{{\bar k}}/ {\mu}_0 ; \\
   {{\mu}_i}(I) & = & {{\mu}_i}/ {\mu}_0 ; \\
   {{p}_i}(I)   & = & {{h}_i}; \\
   {{\rho}_i}(I) & = & {{\rho}_i}/ {\rho}_0 ; \\
   g (I)      & = & 1 .\end{aligned}

By doing this, :math:`{{\bar k}}(I)` represents hydraulic conductivity
of the base phase :math:`{{\bar K}}_0` (e.g. water) under saturated
conditions (i.e. :math:`k_{r0} = 1`).

Water Balance
-------------

ParFlow can calculate a water balance for the Richards’ equation,
overland flow and clm capabilities. For a schematic of the water balance
in ParFlow please see . This water balance is computes using pftools
commands as described in § [Manipulating Data]. There are two water
balance storage components, subsurface and surface, and two flux
calculations, overland flow and evapotranspiration. The storage
components have units :math:`[L^3]` while the fluxes may be
instantaneous and have units :math:`[L^3T^{-1}]` or cumulative over an
output interval with units :math:`[L^3]`. Examples of water balance
calculations and errors are given in the scripts
water\ :sub:`b`\ alance\ :sub:`x`.tcl and
water\ :sub:`b`\ alance\ :sub:`y`.tcl. The size of water balance errors
depend on solver settings and tolerances but are typically very small,
:math:`<10^{-10} [-]`.The water balance takes the form:

.. math::

   \begin{aligned}
   \frac{\Delta [Vol_{subsurface} + Vol_{surface}]}{\Delta t} = Q_{overland} + Q_{evapotranspiration} + Q_{source sink}
   \label{eq:balance}\end{aligned}

where :math:`Vol_{subsurface}` is the subsurface storage :math:`[L^3]`;
:math:`Vol_{surface}` is the surface storage :math:`[L^3]`;
:math:`Q_{overland}` is the overland flux :math:`[L^3 T^{-1}]`;
:math:`Q_{evapotranspiration}` is the evapotranspiration flux passed
from clm or other LSM, etc, :math:`[L^3 T^{-1}]`; and
:math:`Q_{source sink}` are any other source/sink fluxes specified in
the simulation :math:`[L^3 T^{-1}]`. The surface and subsurface storage
routines are calculated using the ParFlow toolset commands
pfsurfacestorage and pfsubsurfacestorage respectively. Overland flow out
of the domain is calculated by pfsurfacerunoff. Details for the use of
these commands are given in § [PFTCL Commands] and
§ [common\ :sub:`p`\ ftcl]. :math:`Q_{evapotranspiration}` must be
written out by ParFlow as a variable (as shown in § refCode Parameters)
and only contains the external fluxes passed from a module such as clm
or *WRF*. Note that these volume and flux quantities are calculated
spatially over the domain and are returned as array values, just like
any other quantity in ParFlow. The tools command pfsum will sum these
arrays into a single value for the enrite domain. All other fluxes must
be determined by the user.The subsurface storage is calculated over all
active cells in the domain, :math:`\Omega`, and contains both
compressible and incompressible parts based on Equation [eq:richard].
This is computed on a cell-by-cell basis (with the result being an array
of balances over the domain) as follows:

.. math::

   \begin{aligned}
   Vol_{subsurface} = \sum_\Omega [ S(\psi)S_s \psi \Delta x \Delta y \Delta z +
   S(\psi)(\psi)\phi \Delta x \Delta y \Delta z]
   \label{eq:sub_store}\end{aligned}

The surface storage is calculated over the upper surface boundary cells
in the domain, :math:`\Gamma`, as computed by the mask and contains
based on Equation [eq:kinematic]. This is again computed on a
cell-by-cell basis (with the result being an array of balances over the
domain) as follows:

.. math::

   \begin{aligned}
   Vol_{surface} =  \sum_\Gamma \psi \Delta x \Delta y  
   \label{eq:surf_store}\end{aligned}

For the overland flow outflow from the domain, any cell at the top
boundary that has a slope that points out of the domain and is ponded
will remove water from the domain. This is calculated, for example in
the y-direction, as the multiple of Equation [eq:manningsy] and the
area:

.. math::

   \begin{aligned}
   Q_{overland}=vA= -\frac{\sqrt{S_{f,y}}}{n}\psi_{s}^{2/3}\psi \Delta x=- \frac{\sqrt{S_{f,y}}}{n}\psi_{s}^{5/3}\Delta x
   \label{eq:outflow}\end{aligned}

ParFlow Files
=============

In this chapter, we discuss the various file formats used in ParFlow. To
help simplify the description of these formats, we use a pseudocode
notation composed of *fields* and *control constructs*.

A field is a piece of data having one of the *field types* listed in
Table [table-field-types] (note that field types may have one meaning in
ASCII files and another meaning in binary files).

| \|\|c\|\|c\|c\|\| field type & ASCII & binary
| integer & integer & XDR integer
| real & real & -
| string & string & -
| double & - & IEEE 8 byte double
| float & - & IEEE 4 byte float

[table-field-types]

Fields are denoted by enclosing the field name with a < on the left and
a > on the right. The field name is composed of alphanumeric characters
and underscores (). In the defining entry of a field, the field name is
also prepended by its field type and a :. The control constructs used in
our pseudocode have the keyword names FOR, IF, and LINE, and the
beginning and end of each of these constructs is delimited by the
keywords BEGIN and END.

The FOR construct is used to describe repeated input format patterns.
For example, consider the following file format:

::

    <integer : num_coordinates>
    FOR coordinate = 0 TO <num_coordinates> - 1
    BEGIN
       <real : x>  <real : y>  <real : z>
    END

The field <num:sub:`c`\ oordinates> is an integer specifying the number
of coordinates to follow. The FOR construct indicates that
<num:sub:`c`\ oordinates> entries follow, and each entry is composed of
the three real fields, <x>, <y>, and <z>. Here is an example of a file
with this format:

::

    3
    2.0 1.0 -3.5
    1.0 1.1 -3.1
    2.5 3.0 -3.7

The IF construct is actually an IF/ELSE construct, and is used to
describe input format patterns that appear only under certain
circumstances. For example, consider the following file format:

::

    <integer : type>
    IF (<type> = 0)
    BEGIN
       <real : x>  <real : y>  <real : z>
    END
    ELSE IF (<type> = 1)
    BEGIN
       <integer : i>  <integer : j>  <integer : k>
    END

The field <type> is an integer specifying the “type” of input to follow.
The IF construct indicates that if <type> has value 0, then the three
real fields, <x>, <y>, and <z>, follow. If <type> has value 1, then the
three integer fields, <i>, <j>, and <k>, follow. Here is an example of a
file with this format:

::

    0
    2.0 1.0 -3.5

The LINE construct indicates fields that are on the same line of a file.
Since input files in ParFlow are all in “free format”, it is used only
to describe some output file formats. For example, consider the
following file format:

::

    LINE
    BEGIN
       <real : x>
       <real : y>
       <real : z>
    END

The LINE construct indicates that the three real fields, <x>, <y>, and
<z>, are all on the same line. Here is an example of a file with this
format:

::

    2.0 1.0 -3.5

Comment lines may also appear in our file format pseudocode. All text
following a # character is a comment, and is not part of the file
format.

Main Input File (.tcl)
----------------------

The main ParFlow input file is a TCL script. This might seem overly
combersome at first but the basic input file structure is not very
complicated (although it is somewhat verbose). For more advanced users,
the TCL scripting means you can very easily create programs to run
ParFlow. A simple example is creating a loop to run several hundred
different simulations using different seeds to the random field
generators. This can be automated from within the ParFlow input file.

The basic idea behind ParFlow input is a simple database. The database
contains entries which have a key and a value associated with that key.
This is very similiar in nature to the Windows XP/Vista registry and
several other systems. When ParFlow runs, it queries the database you
have created by key names to get the values you have specified.

The command pfset is used to create the database entries. A simple
ParFlow input script contains a long list of pfset commands.

It should be noted that the keys are “dynamic” in that many are built up
from values of other keys. For example if you have two wells named
*northwell* and *southwell* then you will have to set some keys which
specify the parameters for each well. The keys are built up in a simple
sort of heirarchy.

The following sections contain a description of all of the keys used by
ParFlow. For an example of input files you can look at the test
subdirectory of the ParFlow distribution. Looking over some examples
should give you a good feel for how the file scripts are put together.

Each key’s entry has the form:

type **KeyName** *default value* Description

The “type” is one of integer, double, string, list. Integer and double
are IEEE numbers. String is a text string (for example, a filename).
Strings can contain spaces if you use the proper TCL syntax (i.e. using
double quotes). These types are standard TCL types. Lists are strings
but they indicate the names of a series of items. For example you might
need to specify the names of the geometries. You would do this using
space seperated names (what we are calling a list) “layer1 layer2
layer3”.

The descriptions that follow are organized into functional areas. An
example for each database entry is given.

Note that units used for each physical quantity specified in the input
file must be consistent with units used for all other quantities. The
exact units used can be any consistent set as ParFlow does not assume
any specific set of units. However, it is up to the user to make sure
all specifications are indeed consistent.

Input File Format Number
~~~~~~~~~~~~~~~~~~~~~~~~

integer **FileVersion** *no default* This gives the value of the input
file version number that this file fits.

::

    pfset FileVersion 4

As development of the ParFlow code continues, the input file format will
vary. We have thus included an input file format number as a way of
verifying that the correct format type is being used. The user can check
in the parflow/config/file\ :sub:`v`\ ersions.h file to verify that the
format number specified in the input file matches the defined value of
PFIN\ :sub:`V`\ ERSION.

Computing Topology
~~~~~~~~~~~~~~~~~~

This section describes how processors are assigned in order to solve the
domain in parallel. “P” allocates the number of processes to the
grid-cells in x. “Q” allocates the number of processes to the grid-cells
in y. “R” allocates the number of processes to the grid-cells in z.
Please note “R” should always be 1 if you are running with Solver
Richards unless you’re running a totally saturated domain (solver
IMPES).

integer **Process.Topology.P** *no default* This assigns the process
splits in the *x* direction.

::

    pfset Process.Topology.P        2

integer **Process.Topology.Q** *no default* This assigns the process
splits in the *y* direction.

::

    pfset Process.Topology.Q        1

integer **Process.Topology.P** *no default* This assigns the process
splits in the *z* direction.

::

    pfset Process.Topology.R        1

In addition, you can assign the computing topology when you initiate
your parflow script using tcl. You must include the topology allocation
when using tclsh and the parflow script.

Example Usage:

::

    [from Terminal] tclsh default_single.tcl 2 1 1

    [At the top of default_single.tcl you must include the following]
    set NP  [lindex $argv 0]
    set NQ  [lindex $argv 1]

    pfset Process.Topology.P        $NP
    pfset Process.Topology.Q        $NQ
    pfset Process.Topology.R        1 

Computational Grid
~~~~~~~~~~~~~~~~~~

The computational grid is briefly described in § [Defining the Problem].
The computational grid keys set the bottom left corner of the domain to
a specific point in space. If using a .pfsol file, the bottom left
corner location of the .pfsol file must be the points designated in the
computational grid. The user can also assign the *x*, *y* and *z*
location to correspond to a specific coordinate system (i.e. UTM).

double **ComputationalGrid.Lower.X** *no default* This assigns the lower
*x* coordinate location for the computational grid.

::

    pfset   ComputationalGrid.Lower.X  0.0

double **ComputationalGrid.Lower.Y** *no default* This assigns the lower
*y* coordinate location for the computational grid.

::

    pfset   ComputationalGrid.Lower.Y  0.0

double **ComputationalGrid.Lower.Z** *no default* This assigns the lower
*z* coordinate location for the computational grid.

::

    pfset   ComputationalGrid.Lower.Z  0.0

integer **ComputationalGrid.NX** *no default* This assigns the number of
grid cells in the *x* direction for the computational grid.

::

    pfset  ComputationalGrid.NX  10 

integer **ComputationalGrid.NY** *no default* This assigns the number of
grid cells in the *y* direction for the computational grid.

::

    pfset  ComputationalGrid.NY  10 

integer **ComputationalGrid.NZ** *no default* This assigns the number of
grid cells in the *z* direction for the computational grid.

::

    pfset  ComputationalGrid.NZ  10 

real **ComputationalGrid.DX** *no default* This defines the size of grid
cells in the *x* direction. Units are *L* and are defined by the units
of the hydraulic conductivity used in the problem.

::

    pfset  ComputationalGrid.DX  10.0 

real **ComputationalGrid.DY** *no default* This defines the size of grid
cells in the *y* direction. Units are *L* and are defined by the units
of the hydraulic conductivity used in the problem.

::

    pfset  ComputationalGrid.DY  10.0 

real **ComputationalGrid.DZ** *no default* This defines the size of grid
cells in the *z* direction. Units are *L* and are defined by the units
of the hydraulic conductivity used in the problem.

::

    pfset  ComputationalGrid.DZ  1.0 

Example Usage:

::

    #---------------------------------------------------------
    # Computational Grid
    #---------------------------------------------------------
    pfset ComputationalGrid.Lower.X	-10.0
    pfset ComputationalGrid.Lower.Y     10.0
    pfset ComputationalGrid.Lower.Z	1.0

    pfset ComputationalGrid.NX		18
    pfset ComputationalGrid.NY		18
    pfset ComputationalGrid.NZ		8

    pfset ComputationalGrid.DX		8.0
    pfset ComputationalGrid.DY		10.0
    pfset ComputationalGrid.DZ		1.0

Geometries
~~~~~~~~~~

Here we define all “geometrical” information needed by ParFlow. For
example, the domain (and patches on the domain where boundary conditions
are to be imposed), lithology or hydrostratigraphic units, faults,
initial plume shapes, and so on, are considered geometries.

This input section is a little confusing. Two items are being specified,
geometry inputs and geometries. A geometry input is a type of geometry
input (for example a box or an input file). A geometry input can contain
more than one geometry. A geometry input of type Box has a single
geometry (the square box defined by the extants of the two points). A
SolidFile input type can contain several geometries.

list **GeomInput.Names** *no default* This is a list of the geometry
input names which define the containers for all of the geometries
defined for this problem.

::

    pfset GeomInput.Names    "solidinput indinput boxinput"

string **GeomInput.*geom\_input\_name*.InputType** *no default* This
defines the input type for the geometry input with *geom\_input\_name*.
This key must be one of: **SolidFile, IndicatorField**, **Box**.

::

    pfset GeomInput.solidinput.InputType  SolidFile

list **GeomInput.*geom\_input\_name*.GeomNames** *no default* This is a
list of the names of the geometries defined by the geometry input. For a
geometry input type of Box, the list should contain a single geometry
name. For the SolidFile geometry type this should contain a list with
the same number of gemetries as were defined using GMS. The order of
geometries in the SolidFile should match the names. For IndicatorField
types you need to specify the value in the input field which matches the
name using GeomInput.*geom\_input\_name*.Value.

::

    pfset GeomInput.solidinput.GeomNames "domain bottomlayer \
                                          middlelayer toplayer"

string **GeomInput.*geom\_input\_name*.Filename** *no default* For
IndicatorField and SolidFile geometry inputs this key specifies the
input filename which contains the field or solid information.

::

    pfset GeomInput.solidinput.FileName   ocwd.pfsol

integer **GeomInput.*geometry\_input\_name*.Value** *no default* For
IndicatorField geometry inputs you need to specify the mapping between
values in the input file and the geometry names. The named geometry will
be defined whereever the input file is equal to the specifed value.

::

    pfset GeomInput.sourceregion.Value   11

For box geometries you need to specify the location of the box. This is
done by defining two corners of the the box.

double **Geom.*box\_geom\_name*.Lower.X** *no default* This gives the
lower X real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Lower.X   -1.0

double **Geom.*box\_geom\_name*.Lower.Y** *no default* This gives the
lower Y real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Lower.Y   -1.0

double **Geom.*box\_geom\_name*.Lower.Z** *no default* This gives the
lower Z real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Lower.Z   -1.0

double **Geom.*box\_geom\_name*.Upper.X** *no default* This gives the
upper X real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Upper.X   151.0

double **Geom.*box\_geom\_name*.Upper.Y** *no default* This gives the
upper Y real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Upper.Y   171.0

double **Geom.*box\_geom\_name*.Upper.Z** *no default* This gives the
upper Z real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Upper.Z   11.0

::

    pfset Geom.background.Patches   "left right front back bottom top"

Here is an example geometry input section which has three geometry
inputs.

::

    #---------------------------------------------------------
    # The Names of the GeomInputs
    #---------------------------------------------------------
    pfset GeomInput.Names 			"solidinput indinput boxinput"
    #
    # For a solid file geometry input type you need to specify the names
    # of the gemetries and the filename
    #

    pfset GeomInput.solidinput.InputType	SolidFile

    # The names of the geometries contained in the solid file. Order is
    # important and defines the mapping. First geometry gets the first name. 
    pfset GeomInput.solidinput.GeomNames	"domain"
    #
    # Filename that contains the geometry
    #

    pfset GeomInput.solidinput.FileName 	ocwd.pfsol

    #
    # An indicator field is a 3D field of values. 
    # The values within the field can be mapped 
    # to ParFlow geometries. Indicator fields must match the
    # computation grid exactly!
    #

    pfset GeomInput.indinput.InputType 		IndicatorField
    pfset GeomInput.indinput.GeomNames    	“sourceregion concenregion”
    pfset GeomInput.indinput.FileName		ocwd.pfb

    #
    # Within the indicator.pfb file, assign the values to each GeomNames
    # 
    pfset GeomInput.sourceregion.Value 	11
    pfset GeomInput.concenregion.Value 	12

    #
    # A box is just a box defined by two points.
    #

    pfset GeomInput.boxinput.InputType	Box
    pfset GeomInput.boxinput.GeomName	background
    pfset Geom.background.Lower.X 		-1.0
    pfset Geom.background.Lower.Y 		-1.0
    pfset Geom.background.Lower.Z 		-1.0
    pfset Geom.background.Upper.X 		151.0
    pfset Geom.background.Upper.Y 		171.0
    pfset Geom.background.Upper.Z 		11.0

    #
    # The patch order is fixed in the .pfsol file, but you 
    # can call the patch name anything you 
    # want (i.e. left right front back bottom top)
    #

    pfset Geom.domain.Patches             		" z-upper x-lower y-lower \
                                          			x-upper y-upper z-lower"

Timing Information
~~~~~~~~~~~~~~~~~~

The data given in the timing section describe all the “temporal”
information needed by ParFlow. The data items are used to describe time
units for later sections, sequence iterations in time, indicate actual
starting and stopping values and give instructions on when data is
printed out.

This key is used to indicate the base unit of time for entering time
values. All time should be expressed as a multiple of this value. This
should be set to the smallest interval of time to be used in the
problem. For example, a base unit of “1” means that all times will be
integer valued. A base unit of “0.5” would allow integers and fractions
of 0.5 to be used for time input values.

The rationale behind this restriction is to allow time to be discretized
on some interval to enable integer arithmetic to be used when
computing/comparing times. This avoids the problems associated with real
value comparisons which can lead to events occurring at different
timesteps on different architectures or compilers.

This value is also used when describing “time cycling data” in,
currently, the well and boundary condition sections. The lengths of the
cycles in those sections will be integer multiples of this value,
therefore it needs to be the smallest divisor which produces an integral
result for every “real time” cycle interval length needed.

::

    pfset TimingInfo.BaseUnit      1.0

integer **TimingInfo.StartCount** *no default* This key is used to
indicate the time step number that will be associated with the first
advection cycle in a transient problem. The value **-1** indicates that
advection is not to be done. The value **0** indicates that advection
should begin with the given initial conditions. Values greater than
**0** are intended to mean “restart” from some previous “checkpoint”
time-step, but this has not yet been implemented.

::

    pfset TimingInfo.StartCount    0

double **TimingInfo.StartTime** *no default* This key is used to
indicate the starting time for the simulation.

::

    pfset TimingInfo.StartTime     0.0

double **TimingInfo.StopTime** *no default* This key is used to indicate
the stopping time for the simulation.

::

    pfset TimingInfo.StopTime      100.0

double **TimingInfo.DumpInterval** *no default* This key is the real
time interval at which time-dependent output should be written. A value
of **0** will produce undefined behavior. If the value is negative,
output will be dumped out every :math:`n` time steps, where :math:`n` is
the absolute value of the integer part of the value.

::

    pfset TimingInfo.DumpInterval  10.0

::

    pfset TimingInfo.DumpIntervalExecutionTimeLimit 360

For *Richards’ equation cases only* input is collected for time step
selection. Input for this section is given as follows:

list **TimeStep.Type** *no default* This key must be one of:
**Constant** or **Growth**. The value **Constant** defines a constant
time step. The value **Growth** defines a time step that starts as
:math:`dt_0` and is defined for other steps as
:math:`dt^{new} = \gamma dt^{old}` such that :math:`dt^{new} \leq 
dt_{max}` and :math:`dt^{new} \geq dt_{min}`.

::

    pfset TimeStep.Type      Constant

double **TimeStep.Value** *no default* This key is used only if a
constant time step is selected and indicates the value of the time step
for all steps taken.

::

    pfset TimeStep.Value      0.001

double **TimeStep.InitialStep** *no default* This key specifies the
initial time step :math:`dt_0` if the **Growth** type time step is
selected.

::

    pfset TimeStep.InitialStep    0.001

double **TimeStep.GrowthFactor** *no default* This key specifies the
growth factor :math:`\gamma` by which a time step will be multiplied to
get the new time step when the **Growth** type time step is selected.

::

    pfset TimeStep.GrowthFactor      1.5

double **TimeStep.MaxStep** *no default* This key specifies the maximum
time step allowed, :math:`dt_{max}`, when the **Growth** type time step
is selected.

::

    pfset TimeStep.MaxStep      86400

double **TimeStep.MinStep** *no default* This key specifies the minimum
time step allowed, :math:`dt_{min}`, when the **Growth** type time step
is selected.

::

    pfset TimeStep.MinStep      1.0e-3

Here is a detailed example of how timing keys might be used in a
simualtion.

::

    #-----------------------------------------------------------------------------
    # Setup timing info [hr]
    # 8760 hours in a year. Dumping files every 24 hours. Hourly timestep
    #-----------------------------------------------------------------------------
    pfset TimingInfo.BaseUnit		1.0
    pfset TimingInfo.StartCount		0
    pfset TimingInfo.StartTime		0.0
    pfset TimingInfo.StopTime		8760.0
    pfset TimingInfo.DumpInterval	-24

    ## Timing constant example
    pfset TimeStep.Type			Constant
    pfset TimeStep.Value			1.0

    ## Timing growth example
    pfset TimeStep.Type			Growth
    pfset TimeStep.InitialStep		0.0001
    TimeStep.GrowthFactor		1.4
    TimeStep.MaxStep			1.0
    TimeStep.MinStep			0.0001

Time Cycles
~~~~~~~~~~~

The data given in the time cycle section describe how time intervals are
created and named to be used for time-dependent boundary and well
information needed by ParFlow. All the time cycles are synched to the
**TimingInfo.BaseUnit** key described above and are *integer
multipliers* of that value.

list **CycleNames** *no default* This key is used to specify the named
time cycles to be used in a simulation. It is a list of names and each
name defines a time cycle and the number of items determines the total
number of time cycles specified. Each named cycle is described using a
number of keys defined below.

::

    pfset Cycle.Names constant onoff

list **Cycle.*cycle\_name*.Names** *no default* This key is used to
specify the named time intervals for each cycle. It is a list of names
and each name defines a time interval when a specific boundary condition
is applied and the number of items determines the total number of
intervals in that time cycle.

::

    pfset Cycle.onoff.Names "on off"

integer **Cycle.*cycle\_name.interval\_name*.Length** *no default* This
key is used to specify the length of a named time intervals. It is an
*integer multiplier* of the value set for the **TimingInfo.BaseUnit**
key described above. The total length of a given time cycle is the sum
of all the intervals multiplied by the base unit.

::

    pfset Cycle.onoff.on.Length             10

integer **Cycle.*cycle\_name*.Repeat** *no default* This key is used to
specify the how many times a named time interval repeats. A positive
value specifies a number of repeat cycles a value of -1 specifies that
the cycle repeat for the entire simulation.

::

    pfset Cycle.onoff.Repeat               -1

Here is a detailed example of how time cycles might be used in a
simualtion.

::


    #-----------------------------------------------------------------------------
    # Time Cycles
    #-----------------------------------------------------------------------------
    pfset Cycle.Names 			"constant rainrec"
    pfset Cycle.constant.Names		"alltime"
    pfset Cycle.constant.alltime.Length	8760
    pfset Cycle.constant.Repeat		-1

    # Creating a rain and recession period for the rest of year
    pfset Cycle.rainrec.Names		"rain rec"
    pfset Cycle.rainrec.rain.Length	10
    pfset Cycle.rainrec.rec.Length	8750
    pfset Cycle.rainrec.Repeat              	-1

Domain
~~~~~~

The domain may be represented by any of the solid types in
§ [Geometries] above that allow the definition of surface patches. These
surface patches are used to define boundary conditions in § [Boundary
Conditions: Pressure] and § [Boundary Conditions: Saturation] below.
Subsequently, it is required that the union (or combination) of the
defined surface patches equal the entire domain surface. NOTE: This
requirement is NOT checked in the code.

string **Domain.GeomName** *no default* This key specifies which of the
named geometries is the problem domain.

::

    pfset Domain.GeomName    domain

Phases and Contaminants
~~~~~~~~~~~~~~~~~~~~~~~

list **Phase.Names** *no default* This specifies the names of phases to
be modeled. Currently only 1 or 2 phases may be modeled.

::

    pfset Phase.Names    "water"

list **Contaminant.Names** *no default* This specifies the names of
contaminants to be advected.

::

    pfset Contaminants.Names   "tce"

Gravity, Phase Density and Phase Viscosity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

double **Gravity** *no default* Specifies the gravity constant to be
used.

::

    pfset Gravity	1.0

string **Phase.*phase\_name*.Density.Type** *no default* This key
specifies whether density will be a constant value or if it will be
given by an equation of state of the form :math:`(rd)exp(cP)`, where
:math:`P` is pressure, :math:`rd` is the density at atmospheric
pressure, and :math:`c` is the phase compressibility constant. This key
must be either **Constant** or **EquationOfState**.

::

    pfset Phase.water.Density.Type	 Constant

double **Phase.*phase\_name*.Density.Value** *no default* This specifies
the value of density if this phase was specified to have a constant
density value for the phase *phase\_name*.

::

    pfset Phase.water.Density.Value   1.0

double **Phase.*phase\_name*.Density.ReferenceDensity** *no default*
This key specifies the reference density if an equation of state density
function is specified for the phase *phase\_name*.

::

    pfset Phase.water.Density.ReferenceDensity   1.0

This key specifies the phase compressibility constant if an equation of
state density function is specified for the phase *phase\|-name*.

::

    pfset Phase.water.Density.CompressibilityConstant   1.0

string **Phase.*phase\_name*.Viscosity.Type** *Constant* This key
specifies whether viscosity will be a constant value. Currently, the
only choice for this key is **Constant**.

::

    pfset Phase.water.Viscosity.Type   Constant

double **Phase.*phase\_name*.Viscosity.Value** *no default* This
specifies the value of viscosity if this phase was specified to have a
constant viscosity value.

::

    pfset Phase.water.Viscosity.Value   1.0

Chemical Reactions
~~~~~~~~~~~~~~~~~~

double **Contaminants.*contaminant\_name*.Degradation.Value** *no
default* This key specifies the half-life decay rate of the named
contaminant, *contaminant\_name*. At present only first order decay
reactions are implemented and it is assumed that one contaminant cannot
decay into another.

::

    pfset Contaminants.tce.Degradation.Value         0.0

Permeability
~~~~~~~~~~~~

In this section, permeability property values are assigned to grid
points within geometries (specified in § [Geometries] above) using one
of the methods described below. Permeabilities are assumed to be a
diagonal tensor with entries given as,

.. math::

   \left( 
   \begin{array}{ccc}
   k_x({\bf x}) & 0 & 0 \\
   0 & k_y({\bf x}) & 0 \\
   0 & 0 & k_z({\bf x}) 
   \end{array} \right) 
   K({\bf x}),

where :math:`K({\bf x})` is the permeability field given below.
Specification of the tensor entries (:math:`k_x, k_y` and :math:`k_z`)
will be given at the end of this section.

The random field routines (*turning bands* and *pgs*) can use
conditioning data if the user so desires. It is not necessary to use
conditioning as ParFlow automatically defaults to not use conditioning
data, but if conditioning is desired, the following key should be set:

string **Perm.Conditioning.FileName** *“NA”* This key specifies the name
of the file that contains the conditioning data. The default string
**NA** indicates that conditioning data is not applicable.

::

    pfset Perm.Conditioning.FileName   "well_cond.txt"

The file that contains the conditioning data is a simple ascii file
containing points and values. The format is:

::

    nlines
    x1 y1 z1 value1
    x2 y2 z2 value2
    .  .  .    .
    .  .  .    .
    .  .  .    .
    xn yn zn valuen

The value of *nlines* is just the number of lines to follow in the file,
which is equal to the number of data points.

The variables *xi,yi,zi* are the real space coordinates (in the units
used for the given parflow run) of a point at which a fixed permeability
value is to be assigned. The variable *valuei* is the actual
permeability value that is known.

Note that the coordinates are not related to the grid in any way.
Conditioning does not require that fixed values be on a grid. The PGS
algorithm will map the given value to the closest grid point and that
will be fixed. This is done for speed reasons. The conditioned turning
bands algorithm does not do this; conditioning is done for every grid
point using the given conditioning data at the location given. Mapping
to grid points for that algorithm does not give any speedup, so there is
no need to do it.

NOTE: The given values should be the actual measured values - adjustment
in the conditioning for the lognormal distribution that is assumed is
taken care of in the algorithms.

The general format for the permeability input is as follows:

list **Geom.Perm.Names** *no default* This key specifies all of the
geometries to which a permeability field will be assigned. These
geometries must cover the entire computational domain.

::

    pfset GeomInput.Names   "background domain concen_region"

string **Geom.geometry\_name.Perm.Type** *no default* This key specifies
which method is to be used to assign permeability data to the named
geometry, *geometry\_name*. It must be either **Constant**,
**TurnBands**, **ParGuass**, or **PFBFile**. The **Constant** value
indicates that a constant is to be assigned to all grid cells within a
geometry. The **TurnBand** value indicates that Tompson’s Turning Bands
method is to be used to assign permeability data to all grid cells
within a geometry . The **ParGauss** value indicates that a Parallel
Gaussian Simulator method is to be used to assign permeability data to
all grid cells within a geometry. The **PFBFile** value indicates that
premeabilities are to be read from the “ParFlow Binary” file. Both the
Turning Bands and Parallel Gaussian Simulators generate a random field
with correlation lengths in the :math:`3` spatial directions given by
:math:`\lambda_x`, :math:`\lambda_y`, and :math:`\lambda_z` with the
geometric mean of the log normal field given by :math:`\mu` and the
standard deviation of the normal field given by :math:`\sigma`. In
generating the field both of these methods can be made to stratify the
data, that is follow the top or bottom surface. The generated field can
also be made so that the data is normal or log normal, with or without
bounds truncation. Turning Bands uses a line process, the number of
lines used and the resolution of the process can be changed as well as
the maximum normalized frequency :math:`K_{\rm max}` and the normalized
frequency increment :math:`\delta K`. The Parallel Gaussian Simulator
uses a search neighborhood, the number of simulated points and the
number of conditioning points can be changed.

::

    pfset Geom.background.Perm.Type   Constant

double **Geom.*geometry\_name*.Perm.Value** *no default* This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Geom.domain.Perm.Value   1.0

double **Geom.*geometry\_name*.Perm.LambdaX** *no default* This key
specifies the x correlation length, :math:`\lambda_x`, of the field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.LambdaX   200.0

double **Geom.*geometry\_name*.Perm.LambdaY** *no default* This key
specifies the y correlation length, :math:`\lambda_y`, of the field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.LambdaY   200.0

double **Geom.*geometry\_name*.Perm.LambdaZ** *no default* This key
specifies the z correlation length, :math:`\lambda_z`, of the field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.LambdaZ   10.0

double **Geom.*geometry\_name*.Perm.GeomMean** *no default* This key
specifies the geometric mean, :math:`\mu`, of the log normal field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.GeomMean   4.56

double **Geom.*geometry\_name*.Perm.Sigma** *no default* This key
specifies the standard deviation, :math:`\sigma`, of the normal field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.Sigma   2.08

integer **Geom.*geometry\_name*.Perm.Seed** *1* This key specifies the
initial seed for the random number generator used to generate the field
for the named geometry, *geometry\_name*, if either the Turning Bands or
Parallel Gaussian Simulator are chosen. This number must be positive.

::

    pfset Geom.domain.Perm.Seed   1

integer **Geom.*geometry\_name*.Perm.NumLines** *100* This key specifies
the number of lines to be used in the Turning Bands algorithm for the
named geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.NumLines   100

double **Geom.*geometry\_name*.Perm.RZeta** *5.0* This key specifies the
resolution of the line processes, in terms of the minimum grid spacing,
to be used in the Turning Bands algorithm for the named geometry,
*geometry\_name*. Large values imply high resolution.

::

    pfset Geom.domain.Perm.RZeta   5.0

double **Geom.*geometry\_name*.Perm.KMax** *100.0* This key specifies
the the maximum normalized frequency, :math:`K_{\rm max}`, to be used in
the Turning Bands algorithm for the named geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.KMax   100.0

double **Geom.*geometry\_name*.Perm.DelK** *0.2* This key specifies the
normalized frequency increment, :math:`\delta K`, to be used in the
Turning Bands algorithm for the named geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.DelK   0.2

integer **Geom.*geometry\_name*.Perm.MaxNPts** *no default* This key
sets limits on the number of simulated points in the search neighborhood
to be used in the Parallel Gaussian Simulator for the named geometry,
*geometry\_name*.

::

    pfset Geom.domain.Perm.MaxNPts   5

integer **Geom.*geometry\_name*.Perm.MaxCpts** *no default* This key
sets limits on the number of external conditioning points in the search
neighborhood to be used in the Parallel Gaussian Simulator for the named
geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.MaxCpts   200

string **Geom.*geometry\_name*.Perm.LogNormal** *“LogTruncated”* The key
specifies when a normal, log normal, truncated normal or truncated log
normal field is to be generated by the method for the named geometry,
*geometry\_name*. This value must be one of **Normal**, **Log**,
**NormalTruncated** or **LogTruncate** and can be used with either
Turning Bands or the Parallel Gaussian Simulator.

::

    pfset Geom.domain.Perm.LogNormal   "LogTruncated"

string **Geom.*geometry\_name*.Perm.StratType** *“Bottom”* This key
specifies the stratification of the permeability field generated by the
method for the named geometry, *geometry\_name*. The value must be one
of **Horizontal**, **Bottom** or **Top** and can be used with either the
Turning Bands or the Parallel Gaussian Simulator.

::

    pfset Geom.domain.Perm.StratType  "Bottom"

double **Geom.*geometry\_name*.Perm.LowCutoff** *no default* This key
specifies the low cutoff value for truncating the generated field for
the named geometry, *geometry\_name*, when either the NormalTruncated or
LogTruncated values are chosen.

::

    pfset Geom.domain.Perm.LowCutoff   0.0

double **Geom.*geometry\_name*.Perm.HighCutoff** *no default* This key
specifies the high cutoff value for truncating the generated field for
the named geometry, *geometry\_name*, when either the NormalTruncated or
LogTruncated values are chosen.

::

    pfset Geom.domain.Perm.HighCutoff   100.0

string **Geom.*geometry\_name*.Perm.FileName** *no default* This key
specifies that permeability values for the specified geometry,
*geometry\_name*, are given according to a user-supplied description in
the “ParFlow Binary” file whose filename is given as the value. For a
description of the ParFlow Binary file format, see § [ParFlow Binary
Files (.pfb)]. The ParFlow Binary file associated with the named
geometry must contain a collection of permeability values corresponding
in a one-to-one manner to the entire computational grid. That is to say,
when the contents of the file are read into the simulator, a complete
permeability description for the entire domain is supplied. Only those
values associated with computational cells residing within the geometry
(as it is represented on the computational grid) will be copied into
data structures used during the course of a simulation. Thus, the values
associated with cells outside of the geounit are irrelevant. For
clarity, consider a couple of different scenarios. For example, the user
may create a file for each geometry such that appropriate permeability
values are given for the geometry and \`\`garbage" values (e.g., some
flag value) are given for the rest of the computational domain. In this
case, a separate binary file is specified for each geometry.
Alternatively, one may place all values representing the permeability
field on the union of the geometries into a single binary file. Note
that the permeability values must be represented in precisely the same
configuration as the computational grid. Then, the same file could be
specified for each geounit in the input file. Or, the computational
domain could be described as a single geouint (in the ParFlow input
file) in which case the permeability values would be read in only once.

::

    pfset Geom.domain.Perm.FileName "domain_perm.pfb"

string **Perm.TensorType** *no default* This key specifies whether the
permeability tensor entries :math:`k_x, k_y` and :math:`k_z` will be
specified as three constants within a set of regions covering the domain
or whether the entries will be specified cell-wise by files. The choices
for this key are **TensorByGeom** and **TensorByFile**.

::

    pfset Perm.TensorType     TensorByGeom

string **Geom.Perm.TensorByGeom.Names** *no default* This key specifies
all of the geometries to which permeability tensor entries will be
assigned. These geometries must cover the entire computational domain.

::

    pfset Geom.Perm.TensorByGeom.Names   "background domain" 

double **Geom.*geometry\_name*.Perm.TensorValX** *no default* This key
specifies the value of :math:`k_x` for the geometry given by
*geometry\_name*.

::

    pfset Geom.domain.Perm.TensorValX   1.0

double **Geom.*geometry\_name*.Perm.TensorValY** *no default* This key
specifies the value of :math:`k_y` for the geometry given by
*geom\_name*.

::

    pfset Geom.domain.Perm.TensorValY   1.0

double **Geom.*geometry\_name*.Perm.TensorValZ** *no default* This key
specifies the value of :math:`k_z` for the geometry given by
*geom\_name*.

::

    pfset Geom.domain.Perm.TensorValZ   1.0

string **Geom.*geometry\_name*.Perm.TensorFileX** *no default* This key
specifies that :math:`k_x` values for the specified geometry,
*geometry\_name*, are given according to a user-supplied description in
the “ParFlow Binary” file whose filename is given as the value. The only
choice for the value of *geometry\_name* is “domain”.

::

    pfset Geom.domain.Perm.TensorByFileX   "perm_x.pfb"

string **Geom.*geometry\_name*.Perm.TensorFileY** *no default* This key
specifies that :math:`k_y` values for the specified geometry,
*geometry\_name*, are given according to a user-supplied description in
the “ParFlow Binary” file whose filename is given as the value. The only
choice for the value of *geometry\_name* is “domain”.

::

    pfset Geom.domain.Perm.TensorByFileY   "perm_y.pfb"

string **Geom.*geometry\_name*.Perm.TensorFileZ** *no default* This key
specifies that :math:`k_z` values for the specified geometry,
*geometry\_name*, are given according to a user-supplied description in
the “ParFlow Binary” file whose filename is given as the value. The only
choice for the value of *geometry\_name* is “domain”.

::

    pfset Geom.domain.Perm.TensorByFileZ   "perm_z.pfb"

Porosity
~~~~~~~~

Here, porosity values are assigned within geounits (specified in
§ [Geometries] above) using one of the methods described below.

The format for this section of input is:

list **Geom.Porosity.GeomNames** *no default* This key specifies all of
the geometries on which a porosity will be assigned. These geometries
must cover the entire computational domain.

::

    pfset Geom.Porosity.GeomNames   "background"

string **Geom.*geometry\_name*.Porosity.Type** *no default* This key
specifies which method is to be used to assign porosity data to the
named geometry, *geometry\_name*. The only choice currently available is
**Constant** which indicates that a constant is to be assigned to all
grid cells within a geometry.

::

    pfset Geom.background.Porosity.Type   Constant

double **Geom.*geometry\_name*.Porosity.Value** *no default* This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Geom.domain.Porosity.Value   1.0

Specific Storage
~~~~~~~~~~~~~~~~

Here, specific storage (:math:`S_s` in Equation [eq:richard]) values are
assigned within geounits (specified in § [Geometries] above) using one
of the methods described below.

The format for this section of input is:

list **Specific Storage.GeomNames** *no default* This key specifies all
of the geometries on which a different specific storage value will be
assigned. These geometries must cover the entire computational domain.

::

    pfset SpecificStorage.GeomNames       "domain"

string **SpecificStorage.Type** *no default* This key specifies which
method is to be used to assign specific storage data. The only choice
currently available is **Constant** which indicates that a constant is
to be assigned to all grid cells within a geometry.

::

    pfset SpecificStorage.Type            Constant

double **Geom.*geometry\_name*.SpecificStorage.Value** *no default* This
key specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Geom.domain.SpecificStorage.Value 1.0e-4

dZMultipliers
~~~~~~~~~~~~~

Here, dZ multipliers (:math:`\delta Z * m`) values are assigned within
geounits (specified in § [Geometries] above) using one of the methods
described below.

The format for this section of input is:

string ** Solver.Nonlinear.VariableDz** *False* This key specifies
whether dZ multipliers are to be used, the default is False. The default
indicates a false or non-active variable dz and each layer thickness is
1.0 [L].

::

    pfset Solver.Nonlinear.VariableDz     True

list **dzScale.GeomNames** *no default* This key specifies which problem
domain is being applied a variable dz subsurface. These geometries must
cover the entire computational domain.

::

    pfset dzScale.GeomNames domain

string **dzScale.Type** *no default* This key specifies which method is
to be used to assign variable vertical grid spacing. The choices
currently available are **Constant** which indicates that a constant is
to be assigned to all grid cells within a geometry, **nzList** which
assigns all layers of a given model to a list value, and **PFBFile**
which reads in values from a distributed pfb file.

::

    pfset dzScale.Type            Constant

list **Specific dzScale.GeomNames** *no default* This key specifies all
of the geometries on which a different dz scaling value will be
assigned. These geometries must cover the entire computational domain.

::

    pfset dzScale.GeomNames       "domain"

double **Geom.*geometry\_name*.dzScale.Value** *no default* This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Geom.domain.dzScale.Value 1.0

string **Geom.*geometry\_name*.dzScale.FileName** *no default* This key
specifies file to be read in for variable dz values for the given
geometry, *geometry\_name*, if the type was set to **PFBFile**.

::

    pfset Geom.domain.dzScale.FileName  vardz.pfb

integer **dzScale.nzListNumber** *no default* This key indicates the
number of layers with variable dz in the subsurface. This value is the
same as the *ComputationalGrid.NZ* key.

::

    pfset dzScale.nzListNumber  10

double **Cell.*nzListNumber*.dzScale.Value** *no default* This key
assigns the thickness of each layer defined by nzListNumber. ParFlow
assigns the layers from the bottom-up (i.e. the bottom of the domain is
layer 0, the top is layer NZ-1). The total domain depth
(*Geom.domain.Upper.Z*) does not change with variable dz. The layer
thickness is calculated by *ComputationalGrid.DZ \*dZScale*.

::

    pfset Cell.0.dzScale.Value 1.0

Example Usage:

::


    #--------------------------------------------
    # Variable dz Assignments
    #------------------------------------------
    # Set VariableDz to be true
    # Indicate number of layers (nzlistnumber), which is the same as nz
    # (1) There is nz*dz = total depth to allocate,  
    # (2) Each layer’s thickness is dz*dzScale, and
    # (3) Assign the layer thickness from the bottom up.
    # In this example nz = 5; dz = 10; total depth 40;
    # Layers 	Thickness [m]
    # 0 		15 			Bottom layer
    # 1		15
    # 2		5
    # 3		4.5			
    # 4 		0.5			Top layer
    pfset Solver.Nonlinear.VariableDz     True
    pfset dzScale.GeomNames            domain
    pfset dzScale.Type            nzList
    pfset dzScale.nzListNumber       5
    pfset Cell.0.dzScale.Value 1.5
    pfset Cell.1.dzScale.Value 1.5
    pfset Cell.2.dzScale.Value 0.5
    pfset Cell.3.dzScale.Value 0.45
    pfset Cell.4.dzScale.Value 0.05

Manning’s Roughness Values
~~~~~~~~~~~~~~~~~~~~~~~~~~

Here, Manning’s roughness values (:math:`n` in Equations [eq:manningsx]
and [eq:manningsy]) are assigned to the upper boundary of the domain
using one of the methods described below.

The format for this section of input is:

list **Mannings.GeomNames** *no default* This key specifies all of the
geometries on which a different Mannings roughness value will be
assigned. Mannings values may be assigned by **PFBFile** or as
**Constant** by geometry. These geometries must cover the entire upper
surface of the computational domain.

::

    pfset Mannings.GeomNames       "domain"

string **Mannings.Type** *no default* This key specifies which method is
to be used to assign Mannings roughness data. The choices currently
available are **Constant** which indicates that a constant is to be
assigned to all grid cells within a geometry and **PFBFile** which
indicates that all values are read in from a distributed, grid-based
ParFlow binary file.

::

    pfset Mannings.Type "Constant"

double **Mannings.Geom.*geometry\_name*.Value** *no default* This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Mannings.Geom.domain.Value 5.52e-6

double **Mannings.FileName** *no default* This key specifies the value
assigned to all points be read in from a ParFlow binary file.

::

    pfset Mannings.FileName roughness.pfb

Complete example of setting Mannings roughness :math:`n` values by
geometry:

::

    pfset Mannings.Type "Constant"
    pfset Mannings.GeomNames "domain"
    pfset Mannings.Geom.domain.Value 5.52e-6

Topographical Slopes
~~~~~~~~~~~~~~~~~~~~

Here, topographical slope values (:math:`S_{f,x}` and :math:`S_{f,y}` in
Equations [eq:manningsx] and [eq:manningsy]) are assigned to the upper
boundary of the domain using one of the methods described below. Note
that due to the negative sign in these equations :math:`S_{f,x}` and
:math:`S_{f,y}` take a sign in the direction *opposite* of the direction
of the slope. That is, negative slopes point “downhill” and positive
slopes “uphill”.

The format for this section of input is:

list **ToposlopesX.GeomNames** *no default* This key specifies all of
the geometries on which a different :math:`x` topographic slope values
will be assigned. Topographic slopes may be assigned by **PFBFile** or
as **Constant** by geometry. These geometries must cover the entire
upper surface of the computational domain.

::

    pfset ToposlopesX.GeomNames       "domain"

list **ToposlopesY.GeomNames** *no default* This key specifies all of
the geometries on which a different :math:`y` topographic slope values
will be assigned. Topographic slopes may be assigned by **PFBFile** or
as **Constant** by geometry. These geometries must cover the entire
upper surface of the computational domain.

::

    pfset ToposlopesY.GeomNames       "domain"

string **ToposlopesX.Type** *no default* This key specifies which method
is to be used to assign topographic slopes. The choices currently
available are **Constant** which indicates that a constant is to be
assigned to all grid cells within a geometry and **PFBFile** which
indicates that all values are read in from a distributed, grid-based
ParFlow binary file.

::

    pfset ToposlopesX.Type "Constant"

double **ToposlopeX.Geom.*geometry\_name*.Value** *no default* This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset ToposlopeX.Geom.domain.Value 0.001

double **ToposlopesX.FileName** *no default* This key specifies the
value assigned to all points be read in from a ParFlow binary file.

::

    pfset TopoSlopesX.FileName lw.1km.slope_x.pfb

double **ToposlopesY.FileName** *no default* This key specifies the
value assigned to all points be read in from a ParFlow binary file.

::

    pfset TopoSlopesY.FileName lw.1km.slope_y.pfb

Example of setting :math:`x` and :math:`y` slopes by geometry:

::

    pfset TopoSlopesX.Type "Constant"
    pfset TopoSlopesX.GeomNames "domain"
    pfset TopoSlopesX.Geom.domain.Value 0.001

    pfset TopoSlopesY.Type "Constant"
    pfset TopoSlopesY.GeomNames "domain"
    pfset TopoSlopesY.Geom.domain.Value -0.001

Example of setting :math:`x` and :math:`y` slopes by file:

::

    pfset TopoSlopesX.Type "PFBFile"
    pfset TopoSlopesX.GeomNames "domain"
    pfset TopoSlopesX.FileName lw.1km.slope_x.pfb

    pfset TopoSlopesY.Type "PFBFile"
    pfset TopoSlopesY.GeomNames "domain"
    pfset TopoSlopesY.FileName lw.1km.slope_y.pfb

Retardation
~~~~~~~~~~~

Here, retardation values are assigned for contaminants within geounits
(specified in § [Geometries] above) using one of the functions described
below. The format for this section of input is:

list **Geom.Retardation.GeomNames** *no default* This key specifies all
of the geometries to which the contaminants will have a retardation
function applied.

::

    pfset GeomInput.Names   "background"

string **Geom.*geometry\_name*.*contaminant\_name*.Retardation.Type**
*no default* This key specifies which function is to be used to compute
the retardation for the named contaminant, *contaminant\_name*, in the
named geometry, *geometry\_name*. The only choice currently available is
**Linear** which indicates that a simple linear retardation function is
to be used to compute the retardation.

::

    pfset Geom.background.tce.Retardation.Type   Linear

double **Geom.*geometry\_name*.*contaminant\_name*.Retardation.Value**
*no default* This key specifies the distribution coefficient for the
linear function used to compute the retardation of the named
contaminant, *contaminant\_name*, in the named geometry,
*geometry\_name*. The value should be scaled by the density of the
material in the geometry.

::

    pfset Geom.domain.Retardation.Value   0.2

Full Multiphase Mobilities
~~~~~~~~~~~~~~~~~~~~~~~~~~

Here we define phase mobilities by specifying the relative permeability
function. Input is specified differently depending on what problem is
being specified. For full multi-phase problems, the following input keys
are used. See the next section for the correct Richards’ equation input
format.

string **Phase.*phase\_name*.Mobility.Type** *no default* This key
specifies whether the mobility for *phase\_name* will be a given
constant or a polynomial of the form, :math:`(S - S_0)^{a}`, where
:math:`S` is saturation, :math:`S_0` is irreducible saturation, and
:math:`a` is some exponent. The possibilities for this key are
**Constant** and **Polynomial**.

::

    pfset Phase.water.Mobility.Type   Constant

double **Phase.*phase\_name*.Mobility.Value** *no default* This key
specifies the constant mobility value for phase *phase\_name*.

::

    pfset Phase.water.Mobility.Value   1.0

double **Phase.*phase\_name*.Mobility.Exponent** *2.0* This key
specifies the exponent used in a polynomial representation of the
relative permeability. Currently, only a value of :math:`2.0` is allowed
for this key.

::

    pfset Phase.water.Mobility.Exponent   2.0

 This key specifies the irreducible saturation used in a polynomial
representation of the relative permeability. Currently, only a value of
0.0 is allowed for this key.

::

    pfset Phase.water.Mobility.IrreducibleSaturation   0.0

Richards’ Equation Relative Permeabilities
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following keys are used to describe relative permeability input for
the Richards’ equation implementation. They will be ignored if a full
two-phase formulation is used.

string **Phase.RelPerm.Type** *no default* This key specifies the type
of relative permeability function that will be used on all specified
geometries. Note that only one type of relative permeability may be used
for the entire problem. However, parameters may be different for that
type in different geometries. For instance, if the problem consists of
three geometries, then **VanGenuchten** may be specified with three
different sets of parameters for the three different goemetries.
However, once **VanGenuchten** is specified, one geometry cannot later
be specified to have **Data** as its relative permeability. The possible
values for this key are **Constant, VanGenuchten, Haverkamp, Data,** and
**Polynomial**.

::

    pfset Phase.RelPerm.Type   Constant

The various possible functions are defined as follows. The **Constant**
specification means that the relative permeability will be constant on
the specified geounit. The **VanGenuchten** specification means that the
relative permeability will be given as a Van Genuchten function with the
form,

.. math::

   \begin{aligned}
   k_r(p) = \frac{(1 - \frac{(\alpha p)^{n-1}}{(1 + (\alpha p)^n)^m})^2}
   {(1 + (\alpha p)^n)^{m/2}},\end{aligned}

where :math:`\alpha` and :math:`n` are soil parameters and
:math:`m = 1 - 1/n`, on each region. The **Haverkamp** specification
means that the relative permeability will be given in the following form
,

.. math::

   \begin{aligned}
   k_r(p) = \frac{A}{A + p^{\gamma}},\end{aligned}

where :math:`A` and :math:`\gamma` are soil parameters, on each region.
The **Data** specification is currently unsupported but will later mean
that data points for the relative permeability curve will be given and
ParFlow will set up the proper interpolation coefficients to get values
between the given data points. The **Polynomial** specification defines
a polynomial relative permeability function for each region of the form,

.. math::

   \begin{aligned}
   k_r(p) = \sum_{i=0}^{degree} c_ip^i.\end{aligned}

list **Phase.RelPerm.GeomNames** *no default* This key specifies the
geometries on which relative permeability will be given. The union of
these geometries must cover the entire computational domain.

::

    pfset Phase.RelPerm.Geonames   domain

double **Geom.*geom\_name*.RelPerm.Value** *no default* This key
specifies the constant relative permeability value on the specified
geometry.

::

    pfset Geom.domain.RelPerm.Value    0.5

integer **Phase.RelPerm.VanGenuchten.File** *0* This key specifies
whether soil parameters for the VanGenuchten function are specified in a
pfb file or by region. The options are either 0 for specification by
region, or 1 for specification in a file. Note that either all
parameters are specified in files (each has their own input file) or
none are specified by files. Parameters specified by files are:
:math:`\alpha` and N.

::

    pfset Phase.RelPerm.VanGenuchten.File   1

string **Geom.*geom\_name*.RelPerm.Alpha.Filename** *no default* This
key specifies a pfb filename containing the alpha parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.RelPerm.Alpha.Filename   alphas.pfb

string **Geom.*geom\_name*.RelPerm.N.Filename** *no default* This key
specifies a pfb filename containing the N parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.RelPerm.N.Filename   Ns.pfb

double **Geom.*geom\_name*.RelPerm.Alpha** *no default* This key
specifies the :math:`\alpha` parameter for the Van Genuchten function
specified on *geom\_name*.

::

    pfset Geom.domain.RelPerm.Alpha  0.005

double **Geom.*geom\_name*.RelPerm.N** *no default* This key specifies
the :math:`N` parameter for the Van Genuchten function specified on
*geom\_name*.

::

    pfset Geom.domain.RelPerm.N   2.0

int **Geom.*geom\_name*.RelPerm.NumSamplePoints** *0* This key specifies
the number of sample points for a spline base interpolation table for
the Van Genuchten function specified on *geom\_name*. If this number is
0 (the default) then the function is evaluated directly. Using the
interpolation table is faster but is less accurate.

::

    pfset Geom.domain.RelPerm.NumSamplePoints  20000

This key specifies the lower value for a spline base interpolation table
for the Van Genuchten function specified on *geom\_name*. The upper
value of the range is 0. This value is used only when the table lookup
method is used (*NumSamplePoints* is greater than 0).

::

    pfset Geom.domain.RelPerm.MinPressureHead -300

double **Geom.*geom\_name*.RelPerm.A** *no default* This key specifies
the :math:`A` parameter for the Haverkamp relative permeability on
*geom\_name*.

::

    pfset Geom.domain.RelPerm.A  1.0

double **Geom.*geom\_name*.RelPerm.Gamma** *no default* This key
specifies the the :math:`\gamma` parameter for the Haverkamp relative
permeability on *geom\_name*.

::

    pfset Geom.domain.RelPerm.Gamma  1.0

integer **Geom.*geom\_name*.RelPerm.Degree** *no default* This key
specifies the degree of the polynomial for the Polynomial relative
permeability given on *geom\_name*.

::

    pfset Geom.domain.RelPerm.Degree  1

This key specifies the *coeff\_number*th coefficient of the Polynomial
relative permeability given on *geom\_name*.

::

    pfset Geom.domain.RelPerm.Coeff.0  0.5
    pfset Geom.domain.RelPerm.Coeff.1  1.0

NOTE: For all these cases, if only one region is to be used (the
domain), the background region should NOT be set as that single region.
Using the background will prevent the upstream weighting from being
correct near Dirichlet boundaries.

Phase Sources
~~~~~~~~~~~~~

The following keys are used to specify phase source terms. The units of
the source term are :math:`1/T`. So, for example, to specify a region
with constant flux rate of :math:`L^3/T`, one must be careful to convert
this rate to the proper units by dividing by the volume of the enclosing
region. For *Richards’ equation* input, the source term must be given as
a flux multiplied by density.

This key specifies the type of source to use for phase *phase\_name*.
Possible values for this key are **Constant** and
**PredefinedFunction**. **Constant** type phase sources specify a
constant phase source value for a given set of regions.
**PredefinedFunction** type phase sources use a preset function (choices
are listed below) to specify the source. Note that the
**PredefinedFunction** type can only be used to set a single source over
the entire domain and not separate sources over different regions.

::

    pfset PhaseSources.water.Type   Constant

list **PhaseSources.*phase\_name*.GeomNames** *no default* This key
specifies the names of the geometries on which source terms will be
specified. This is used only for **Constant** type phase sources.
Regions listed later “overlay” regions listed earlier.

::

    pfset PhaseSources.water.GeomNames   "bottomlayer middlelayer toplayer"

This key specifies the value of a constant source term applied to phase
*phase \_name* on geometry *geom\_name*.

::

    pfset PhaseSources.water.Geom.toplayer.Value   1.0

string **PhaseSources.*phase\_name*.PredefinedFunction** *no default*
This key specifies which of the predefined functions will be used for
the source. Possible values for this key are **X, XPlusYPlusZ,
X3Y2PlusSinXYPlus1,** and **XYZTPlus1PermTensor**.

::

    pfset PhaseSources.water.PredefinedFunction   XPlusYPlusZ

The choices for this key correspond to sources as follows:

**X**:
    :math:`{\rm source}\; = 0.0`

**XPlusYPlusX**:
    :math:`{\rm source}\; = 0.0`

**X3Y2PlusSinXYPlus1**:
    | :math:`{\rm source}\; = -(3x^2 y^2 + y\cos(xy))^2 - (2x^3 y + x\cos(xy))^2 
    - (x^3 y^2 + \sin(xy) + 1) (6x y^2 + 2x^3 -(x^2 +y^2) \sin(xy))`
    | This function type specifies that the source applied over the
    entire domain is as noted above. This corresponds to
    :math:`p=x^{3}y^{2}+\sin(xy)+1` in the problem
    :math:`-\nabla\cdot (p\nabla p)=f`.

**X3Y4PlusX2PlusSinXYCosYPlus1**:
    | :math:`{\rm source}\; = -(3x^22 y^4 + 2x + y\cos(xy)\cos(y))^2 
    - (4x^3 y^3 + x\cos(xy)\cos(y) - \sin(xy)\sin(y))^2 
    - (x^3 y^4 + x^2 + \sin(xy)\cos(y) + 1)
    (6xy^4 + 2 - (x^2 + y^2 + 1)\sin(xy)\cos(y) 
    + 12x^3 y^2 - 2x\cos(xy)\sin(y))`
    | This function type specifies that the source applied over the
    entire domain is as noted above. This corresponds to
    :math:`p=x^{3}y^{4}+x^{2}+\sin (xy)\cos(y) +1` in the problem
    :math:`-\nabla\cdot (p\nabla p)=f`.

**XYZTPlus1**:
    | :math:`{\rm source}\; = xyz - t^2 (x^2 y^2 +x^2 z^2 +y^2 z^2)`
    | This function type specifies that the source applied over the
    entire domain is as noted above. This corresponds to
    :math:`p = xyzt + 1` in the problem
    :math:`\frac{\partial p}{\partial t}-\nabla\cdot (p\nabla p)=f`.

**XYZTPlus1PermTensor**:
    | :math:`{\rm source}\; = xyz - t^2 (x^2 y^2 3 + x^2 z^2 2 + y^2 z^2)`
    | This function type specifies that the source applied over the
    entire domain is as noted above. This corresponds to
    :math:`p = xyzt + 1` in the problem
    :math:`\frac{\partial p}{\partial t}-\nabla\cdot (Kp\nabla p)=f`,
    where :math:`K = diag(1 \;\; 2 \;\; 3)`.

Capillary Pressures
~~~~~~~~~~~~~~~~~~~

Here we define capillary pressure. Note: this section needs to be
defined *only* for multi-phase flow and should not be defined for single
phase and Richards’ equation cases. The format for this section of input
is:

string **CapPressure.*phase\_name*.Type** *“Constant”* This key
specifies the capillary pressure between phase :math:`0` and the named
phase, *phase\_name*. The only choice available is **Constant** which
indicates that a constant capillary pressure exists between the phases.

::

    pfset CapPressure.water.Type   Constant

list **CapPressure.*phase\_name*.GeomNames** *no default* This key
specifies the geometries that capillary pressures will be computed for
in the named phase, *phase\_name*. Regions listed later “overlay”
regions listed earlier. Any geometries not listed will be assigned
:math:`0.0` capillary pressure by ParFlow.

::

    pfset CapPressure.water.GeomNames   "domain"

 This key specifies the value of the capillary pressure in the named
geometry, *geometry\_name*, for the named phase, *phase\_name*.

::

    pfset Geom.domain.CapPressure.water.Value   0.0

*Important note*: the code currently works only for capillary pressure
equal zero.

Saturation
~~~~~~~~~~

This section is *only* relevant to the Richards’ equation cases. All
keys relating to this section will be ignored for other cases. The
following keys are used to define the saturation-pressure curve.

string **Phase.Saturation.Type** *no default* This key specifies the
type of saturation function that will be used on all specified
geometries. Note that only one type of saturation may be used for the
entire problem. However, parameters may be different for that type in
different geometries. For instance, if the problem consists of three
geometries, then **VanGenuchten** may be specified with three different
sets of parameters for the three different goemetries. However, once
**VanGenuchten** is specified, one geometry cannot later be specified to
have **Data** as its saturation. The possible values for this key are
**Constant, VanGenuchten, Haverkamp, Data, Polynomial** and **PFBFile**.

::

    pfset Phase.Saturation.Type   Constant

The various possible functions are defined as follows. The **Constant**
specification means that the saturation will be constant on the
specified geounit. The **VanGenuchten** specification means that the
saturation will be given as a Van Genuchten function with the form,

.. math::

   \begin{aligned}
   s(p) = \frac{s_{sat} - s_{res}}{(1 + (\alpha p)^n)^m} + s_{res},\end{aligned}

where :math:`s_{sat}` is the saturation at saturated conditions,
:math:`s_{res}` is the residual saturation, and :math:`\alpha` and
:math:`n` are soil parameters with :math:`m = 1 - 1/n`, on each region.
The **Haverkamp** specification means that the saturation will be given
in the following form ,

.. math::

   \begin{aligned}
   s(p) = \frac{\alpha(s_{sat} - s_{res})}{A + p^{\gamma}} + s_{res},\end{aligned}

where :math:`A` and :math:`\gamma` are soil parameters, on each region.
The **Data** specification is currently unsupported but will later mean
that data points for the saturation curve will be given and ParFlow will
set up the proper interpolation coefficients to get values between the
given data points. The **Polynomial** specification defines a polynomial
saturation function for each region of the form,

.. math::

   \begin{aligned}
   s(p) = \sum_{i=0}^{degree} c_ip^i.\end{aligned}

The **PFBFile** specification means that the saturation will be taken as
a spatially varying but constant in pressure function given by data in a
ParFlow binary (.pfb) file.

list **Phase.Saturation.GeomNames** *no default* This key specifies the
geometries on which saturation will be given. The union of these
geometries must cover the entire computational domain.

::

    pfset Phase.Saturation.Geonames   domain

double **Geom.*geom\_name*.Saturation.Value** *no default* This key
specifies the constant saturation value on the *geom\_name* region.

::

    pfset Geom.domain.Saturation.Value    0.5

integer **Phase.Saturation.VanGenuchten.File** *0* This key specifies
whether soil parameters for the VanGenuchten function are specified in a
pfb file or by region. The options are either 0 for specification by
region, or 1 for specification in a file. Note that either all
parameters are specified in files (each has their own input file) or
none are specified by files. Parameters specified by files are
:math:`\alpha`, N, SRes, and SSat.

::

    pfset Phase.Saturation.VanGenuchten.File   1

string **Geom.*geom\_name*.Saturation.Alpha.Filename** *no default* This
key specifies a pfb filename containing the alpha parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.Saturation.Filename   alphas.pfb

string **Geom.*geom\_name*.Saturation.N.Filename** *no default* This key
specifies a pfb filename containing the N parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.Saturation.N.Filename   Ns.pfb

string **Geom.*geom\_name*.Saturation.SRes.Filename** *no default* This
key specifies a pfb filename containing the SRes parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.Saturation.SRes.Filename   SRess.pfb

string **Geom.*geom\_name*.Saturation.SSat.Filename** *no default* This
key specifies a pfb filename containing the SSat parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.Saturation.SSat.Filename   SSats.pfb

double **Geom.*geom\_name*.Saturation.Alpha** *no default* This key
specifies the :math:`\alpha` parameter for the Van Genuchten function
specified on *geom\_name*.

::

    pfset Geom.domain.Saturation.Alpha  0.005

double **Geom.*geom\_name*.Saturation.N** *no default* This key
specifies the :math:`N` parameter for the Van Genuchten function
specified on *geom\_name*.

::

    pfset Geom.domain.Saturation.N   2.0

Note that if both a Van Genuchten saturation and relative permeability
are specified, then the soil parameters should be the same for each in
order to have a consistent problem.

double **Geom.*geom\_name*.Saturation.SRes** *no default* This key
specifies the residual saturation on *geom\_name*.

::

    pfset Geom.domain.Saturation.SRes   0.0

double **Geom.*geom\_name*.Saturation.SSat** *no default* This key
specifies the saturation at saturated conditions on *geom\_name*.

::

    pfset Geom.domain.Saturation.SSat   1.0

double **Geom.*geom\_name*.Saturation.A** *no default* This key
specifies the :math:`A` parameter for the Haverkamp saturation on
*geom\_name*.

::

    pfset Geom.domain.Saturation.A   1.0

double **Geom.*geom\_name*.Saturation.Gamma** *no default* This key
specifies the the :math:`\gamma` parameter for the Haverkamp saturation
on *geom\_name*.

::

    pfset Geom.domain.Saturation.Gamma   1.0

integer **Geom.*geom\_name*.Saturation.Degree** *no default* This key
specifies the degree of the polynomial for the Polynomial saturation
given on *geom\_name*.

::

    pfset Geom.domain.Saturation.Degree   1

This key specifies the *coeff\_number*th coefficient of the Polynomial
saturation given on *geom\_name*.

::

    pfset Geom.domain.Saturation.Coeff.0   0.5
    pfset Geom.domain.Saturation.Coeff.1   1.0

string **Geom.*geom\_name*.Saturation.FileName** *no default* This key
specifies the name of the file containing saturation values for the
domain. It is assumed that *geom\_name* is “domain” for this key.

::

    pfset Geom.domain.Saturation.FileName  "domain_sats.pfb"

Internal Boundary Conditions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this section, we define internal Dirichlet boundary conditions by
setting the pressure at points in the domain. The format for this
section of input is:

::

    pfset InternalBC.Names   "fixedvalue"

double **InternalBC.*internal\_bc\_name*.X** *no default* This key
specifies the x-coordinate, :math:`{\rm x}`, of the named,
*internal\_bc\_name*, condition.

::

    pfset InternalBC.fixedheadvalue.X   40.0

double **InternalBC.*internal\_bc\_name*.Y** *no default* This key
specifies the y-coordinate, :math:`{\rm y}`, of the named,
*internal\_bc\_name*, condition.

::

    pfset InternalBC.fixedheadvalue.Y   65.2

double **InternalBC.*internal\_bc\_name*.Z** *no default* This key
specifies the z-coordinate, :math:`{\rm z}`, of the named,
*internal\_bc\_name*, condition.

::

    pfset InternalBC.fixedheadvalue.Z   12.1

double **InternalBC.*internal\_bc\_name*.Value** *no default* This key
specifies the value of the named, *internal\_bc\_name*, condition.

::

    pfset InternalBC.fixedheadvalue.Value   100.0

Boundary Conditions: Pressure
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here we define the pressure boundary conditions. The Dirichlet
conditions below are hydrostatic conditions, and it is assumed that at
each phase interface the pressure is constant. *It is also assumed here
that all phases are distributed within the domain at all times such that
the lighter phases are vertically higher than the heavier phases.*

Boundary condition input is associated with domain patches (see
§ [Domain]). Note that different patches may have different types of
boundary conditions on them.

list **BCPressure.PatchNames** *no default* This key specifies the names
of patches on which pressure boundary conditions will be specified. Note
that these must all be patches on the external boundary of the domain
and these patches must “cover” that external boundary.

::

    pfset BCPressure.PatchNames    "left right front back top bottom"

string **Patch.*patch\_name*.BCPressure.Type** *no default* This key
specifies the type of boundary condition data given for patch
*patch\_name*. Possible values for this key are **DirEquilRefPatch,
DirEquilPLinear, FluxConst, FluxVolumetric, PressureFile, FluxFile,
OverlandFow, OverlandFlowPFB** and **ExactSolution**. The choice
**DirEquilRefPatch** specifies that the pressure on the specified patch
will be in hydrostatic equilibrium with a constant reference pressure
given on a reference patch. The choice **DirEquilPLinear** specifies
that the pressure on the specified patch will be in hydrostatic
equilibrium with pressure given along a piecewise line at elevation
:math:`z=0`. The choice **FluxConst** defines a constant normal flux
boundary condition through the domain patch. This flux must be specified
in units of :math:`[L]/[T]`. For *Richards’ equation*, fluxes must be
specified as a mass flux and given as the above flux multiplied by the
density. Thus, this choice of input type for a Richards’ equation
problem has units of :math:`([L]/[T])([M]/[L]^3)`. The choice
**FluxVolumetric** defines a volumetric flux boundary condition through
the domain patch. The units should be consistent with all other user
input for the problem. For *Richards’ equation* fluxes must be specified
as a mass flux and given as the above flux multiplied by the density.
The choice **PressureFile** defines a hydraulic head boundary condition
that is read from a properly distributed .pfb file. Only the values
needed for the patch are used. The choice **FluxFile** defines a flux
boundary condition that is read form a properly distributed .pfb file
defined on a grid consistent with the pressure field grid. Only the
values needed for the patch are used. The choices **OverlandFlow** and
**OverlandFlowPFB** both turn on fully-coupled overland flow routing as
described in and in § [Overland Flow]. The key **OverlandFlow**
corresponds to a **Value** key with a positive or negative value, to
indicate uniform fluxes (such as rainfall or evapotranspiration) over
the entire domain while the key **OverlandFlowPFB** allows a .pfb file
to contain grid-based, spatially-variable fluxes. The choice
**ExactSolution** specifies that an exact known solution is to be
applied as a Dirichlet boundary condition on the respective patch. Note
that this does not change according to any cycle. Instead, time
dependence is handled by evaluating at the time the boundary condition
value is desired. The solution is specified by using a predefined
function (choices are described below). NOTE: These last three types of
boundary condition input is for *Richards’ equation cases only!*

::

    pfset Patch.top.BCPressure.Type  DirEquilRefPatch

string **Patch.*patch\_name*.BCPressure.Cycle** *no default* This key
specifies the time cycle to which boundary condition data for patch
*patch\_name* corresponds.

::

    pfset Patch.top.BCPressure.Cycle   Constant

string **Patch.*patch\_name*.BCPressure.RefGeom** *no default* This key
specifies the name of the solid on which the reference patch for the
**DirEquilRefPatch** boundary condition data is given. Care should be
taken to make sure the correct solid is specified in cases of layered
domains.

::

    pfset Patch.top.BCPressure.RefGeom   domain

string **Patch.*patch\_name*.BCPressure.RefPatch** *no default* This key
specifies the reference patch on which the **DirEquilRefPatch** boundary
condition data is given. This patch must be on the reference solid
specified by the Patch.*patch\_name*.BCPressure.RefGeom key.

::

    pfset Patch.top.BCPressure.RefPatch    bottom

This key specifies the reference pressure value for the
**DirEquilRefPatch** boundary condition or the constant flux value for
the **FluxConst** boundary condition, or the constant volumetric flux
for the **FluxVolumetric** boundary condition.

::

    pfset Patch.top.BCPressure.alltime.Value  -14.0

no default Note that the reference conditions for types
**DirEquilPLinear** and **DirEquilRefPatch** boundary conditions are for
phase 0 *only*. This key specifies the constant pressure value along the
interface with phase *phase\_name* for cases with two phases present.

::

    pfset Patch.top.BCPressure.alltime.water.IntValue   -13.0

no default This key specifies the lower :math:`x` coordinate of a line
in the xy-plane.

::

    pfset Patch.top.BCPressure.alltime.XLower  0.0

no default This key specifies the lower :math:`y` coordinate of a line
in the xy-plane.

::

    pfset Patch.top.BCPressure.alltime.YLower  0.0

no default This key specifies the upper :math:`x` coordinate of a line
in the xy-plane.

::

    pfset Patch.top.BCPressure.alltime.XUpper  1.0

no default This key specifies the upper :math:`y` coordinate of a line
in the xy-plane.

::

    pfset Patch.top.BCPressure.alltime.YUpper  1.0

no default This key specifies the number of points on which pressure
data is given along the line used in the type **DirEquilPLinear**
boundary conditions.

::

    pfset Patch.top.BCPressure.alltime.NumPoints   2

no default This key specifies a number between 0 and 1 which represents
the location of a point on the line on which data is given for type
**DirEquilPLinear** boundary conditions. Here 0 corresponds to the lower
end of the line, and 1 corresponds to the upper end.

::

    pfset Patch.top.BCPressure.alltime.0.Location   0.0

no default This key specifies the pressure value for phase 0 at point
number *point\_number* and :math:`z=0` for type **DirEquilPLinear**
boundary conditions. All pressure values on the patch are determined by
first projecting the boundary condition coordinate onto the line, then
linearly interpolating between the neighboring point pressure values on
the line.

::

    pfset Patch.top.BCPressure.alltime.0.Value   14.0

no default This key specifies the name of a properly distributed .pfb
file that contains boundary data to be read for types **PressureFile**
and **FluxFile**. For flux data, the data must be defined over a grid
consistent with the pressure field. In both cases, only the values
needed for the patch will be used. The rest of the data is ignored.

::

    pfset Patch.top.BCPressure.alltime.FileName   ocwd_bc.pfb

no default This key specifies the predefined function that will be used
to specify Dirichlet boundary conditions on patch *patch\_name*. Note
that this does not change according to any cycle. Instead, time
dependence is handled by evaluating at the time the boundary condition
value is desired. Choices for this key include **X, XPlusYPlusZ,
X3Y2PlusSinXYPlus1, X3Y4PlusX2PlusSinXYCosYPlus1, XYZTPlus1** and
**XYZTPlus1PermTensor**.

::

    pfset Patch.top.BCPressure.alltime.PredefinedFunction  XPlusYPlusZ

The choices for this key correspond to pressures as follows.

**X**:
    :math:`p = x`

**XPlusYPlusZ**:
    :math:`p = x + y + z`

**X3Y2PlusSinXYPlus1**:
    :math:`p = x^3 y^2 + \sin(xy) + 1`

**X3Y4PlusX2PlusSinXYCosYPlus1**:
    :math:`p = x^3 y^4 + x^2 + \sin(xy)\cos y + 1`

**XYZTPlus1**:
    :math:`p = xyzt + 1`

**XYZTPlus1PermTensor**:
    :math:`p = xyzt + 1`

Example Script:

::


    #---------------------------------------------------------
    # Initial conditions: water pressure [m]
    #---------------------------------------------------------
    # Using a patch is great when you are not using a box domain
    # If using a box domain HydroStaticDepth is fine
    # If your RefPatch is z-lower (bottom of domain), the pressure is positive.
    # If your RefPatch is z-upper (top of domain), the pressure is negative.
    ### Set water table to be at the bottom of the domain, the top layer is initially dry
    pfset ICPressure.Type				HydroStaticPatch
    pfset ICPressure.GeomNames		domain
    pfset Geom.domain.ICPressure.Value	2.2

    pfset Geom.domain.ICPressure.RefGeom	domain
    pfset Geom.domain.ICPressure.RefPatch	z-lower

    ### Using a .pfb to initialize
    pfset ICPressure.Type                                   PFBFile
    pfset ICPressure.GeomNames		 "domain"
    pfset Geom.domain.ICPressure.FileName	press.00090.pfb

    pfset Geom.domain.ICPressure.RefGeom	domain
    pfset Geom.domain.ICPressure.RefPatch	z-upper

Boundary Conditions: Saturation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: this section needs to be defined *only* for multi-phase flow and
should *not* be defined for the single phase and Richards’ equation
cases.

Here we define the boundary conditions for the saturations. Boundary
condition input is associated with domain patches (see § [Domain]). Note
that different patches may have different types of boundary conditions
on them.

list **BCSaturation.PatchNames** *no default* This key specifies the
names of patches on which saturation boundary conditions will be
specified. Note that these must all be patches on the external boundary
of the domain and these patches must “cover” that external boundary.

::

    pfset BCSaturation.PatchNames    "left right front back top bottom"

::

    pfset Patch.left.BCSaturation.water.Type  ConstantWTHeight

double **Patch.*patch\_name*.BCSaturation.*phase\_name*.Value** *no
default* This key specifies either the constant saturation value if
**DirConstant** is selected or the constant water-table height if
**ConstantWTHeight** is selected.

::

    pfset Patch.top.BCSaturation.air.Value 1.0

double **Patch.*patch\_name*.BCSaturation.*phase\_name*.XLower** *no
default* This key specifies the lower :math:`x` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.XLower -10.0

double **Patch.*patch\_name*.BCSaturation.*phase\_name*.YLower** *no
default* This key specifies the lower :math:`y` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.YLower 5.0

double **Patch.*patch\_name*.BCSaturation.*phase\_name*.XUpper** *no
default* This key specifies the upper :math:`x` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.XUpper  125.0

double **Patch.*patch\_name*.BCSaturation.*phase\_name*.YUpper** *no
default* This key specifies the upper :math:`y` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.YUpper  82.0

integer **Patch.*patch\_name*.BCPressure.*phase\_name*.NumPoints** *no
default* This key specifies the number of points on which saturation
data is given along the line used for type **DirEquilPLinear** boundary
conditions.

::

    pfset Patch.left.BCPressure.water.NumPoints 2

double
**Patch.*patch\_name*.BCPressure.*phase\_name*.*point\_number*.Location**
*no default* This key specifies a number between 0 and 1 which
represents the location of a point on the line for which data is given
in type **DirEquilPLinear** boundary conditions. The line is
parameterized so that 0 corresponds to the lower end of the line, and 1
corresponds to the upper end.

::

    pfset Patch.left.BCPressure.water.0.Location 0.333

double
**Patch.*patch\_name*.BCPressure.*phase\_name*.*point\_number*.Value**
*no default* This key specifies the water-table height for the given
point if type **DirEquilPLinear** boundary conditions are selected. All
saturation values on the patch are determined by first projecting the
water-table height value onto the line, then linearly interpolating
between the neighboring water-table height values onto the line.

::

    pfset Patch.left.BCPressure.water.0.Value  4.5

Initial Conditions: Phase Saturations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: this section needs to be defined *only* for multi-phase flow and
should *not* be defined for single phase and Richards’ equation cases.

Here we define initial phase saturation conditions. The format for this
section of input is:

string **ICSaturation.*phase\_name*.Type** *no default* This key
specifies the type of initial condition that will be applied to
different geometries for given phase, *phase\_name*. The only key
currently available is **Constant**. The choice **Constant** will apply
constants values within geometries for the phase.

::

    ICSaturation.water.Type Constant

::

    ICSaturation.water.GeomNames "domain"

double **Geom.*geom\_input\_name*.ICSaturation.*phase\_name*.Value** *no
default* This key specifies the initial condition value assigned to all
points in the named geometry, *geom\_input\_name*, if the type was set
to **Constant**.

::

    Geom.domain.ICSaturation.water.Value 1.0

Initial Conditions: Pressure
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The keys in this section are used to specify pressure initial conditions
for Richards’ equation cases *only*. These keys will be ignored if any
other case is run.

string **ICPressure.Type** *no default* This key specifies the type of
initial condition given. The choices for this key are **Constant,
HydroStaticDepth, HydroStaticPatch** and **PFBFile**. The choice
**Constant** specifies that the initial pressure will be constant over
the regions given. The choice **HydroStaticDepth** specifies that the
initial pressure within a region will be in hydrostatic equilibrium with
a given pressure specified at a given depth. The choice
**HydroStaticPatch** specifies that the initial pressure within a region
will be in hydrostatic equilibrium with a given pressure on a specified
patch. Note that all regions must have the same type of initial data -
different regions cannot have different types of initial data. However,
the parameters for the type may be different. The **PFBFile**
specification means that the initial pressure will be taken as a
spatially varying function given by data in a ParFlow binary (.pfb)
file.

::

    pfset ICPressure.Type   Constant

list **ICPressure.GeomNames** *no default* This key specifies the
geometry names on which the initial pressure data will be given. These
geometries must comprise the entire domain. Note that conditions for
regions that overlap other regions will have unpredictable results. The
regions given must be disjoint.

::

    pfset ICPressure.GeomNames   "toplayer middlelayer bottomlayer"

double **Geom.*geom\_name*.ICPressure.Value** *no default* This key
specifies the initial pressure value for type **Constant** initial
pressures and the reference pressure value for types
**HydroStaticDepth** and **HydroStaticPatch**.

::

    pfset Geom.toplayer.ICPressure.Value  -734.0

double **Geom.*geom\_name*.ICPressure.RefElevation** *no default* This
key specifies the reference elevation on which the reference pressure is
given for type **HydroStaticDepth** initial pressures.

::

    pfset Geom.toplayer.ICPressure.RefElevation  0.0

double **Geom.*geom\_name*.ICPressure.RefGeom** *no default* This key
specifies the geometry on which the reference patch resides for type
**HydroStaticPatch** initial pressures.

::

    pfset Geom.toplayer.ICPressure.RefGeom   bottomlayer

double **Geom.*geom\_name*.ICPressure.RefPatch** *no default* This key
specifies the patch on which the reference pressure is given for type
**HydorStaticPatch** initial pressures.

::

    pfset Geom.toplayer.ICPressure.RefPatch   bottom

string **Geom.*geom\_name*.ICPressure.FileName** *no default* This key
specifies the name of the file containing pressure values for the
domain. It is assumed that *geom\_name* is “domain” for this key.

::

    pfset Geom.domain.ICPressure.FileName  "ic_pressure.pfb"

Initial Conditions: Phase Concentrations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here we define initial concentration conditions for contaminants. The
format for this section of input is:

string **PhaseConcen.*phase\_name*.*contaminant\_name*.Type** *no
default* This key specifies the type of initial condition that will be
applied to different geometries for given phase, *phase\_name*, and the
given contaminant, *contaminant\_name*. The choices for this key are
**Constant** or **PFBFile**. The choice **Constant** will apply
constants values to different geometries. The choice **PFBFile** will
read values from a “ParFlow Binary” file (see § [ParFlow Binary Files
(.pfb)]).

::

    PhaseConcen.water.tce.Type Constant

::

    PhaseConcen.water.GeomNames "ic_concen_region"

double
**PhaseConcen.*phase\_name*.*contaminant\_name*.*geom\_input\_name*.Value**
*no default* This key specifies the initial condition value assigned to
all points in the named geometry, *geom\_input\_name*, if the type was
set to **Constant**.

::

    PhaseConcen.water.tce.ic_concen_region.Value 0.001

string **PhaseConcen.*phase\_name*.*contaminant\_name*.FileName** *no
default* This key specifies the name of the “ParFlow Binary” file which
contains the initial condition values if the type was set to
**PFBFile**.

::

    PhaseConcen.water.tce.FileName "initial_concen_tce.pfb"

Known Exact Solution
~~~~~~~~~~~~~~~~~~~~

For *Richards equation cases only* we allow specification of an exact
solution to be used for testing the code. Only types that have been
coded and predefined are allowed. Note that if this is speccified as
something other than no known solution, corresponding boundary
conditions and phase sources should also be specified.

string **KnownSolution** *no default* This specifies the predefined
function that will be used as the known solution. Possible choices for
this key are **NoKnownSolution, Constant, X, XPlusYPlusZ,
X3Y2PlusSinXYPlus1, X3Y4PlusX2PlusSinXYCosYPlus1, XYZTPlus1** and
**XYZTPlus1PermTensor**.

::

    pfset KnownSolution  XPlusYPlusZ

Choices for this key correspond to solutions as follows.

**NoKnownSolution**:
    No solution is known for this problem.

**Constant**:
    :math:`p = {\rm constant}`

**X**:
    :math:`p = x`

**XPlusYPlusZ**:
    :math:`p = x + y + z`

**X3Y2PlusSinXYPlus1**:
    :math:`p = x^3 y^2 + sin(xy) + 1`

**X3Y4PlusX2PlusSinXYCosYPlus1**:
    :math:`p = x^3 y^4 + x^2 + \sin(xy)\cos y + 1`

**XYZTPlus1**:
    :math:`p = xyzt + 1`

**XYZTPlus1**:
    :math:`p = xyzt + 1`

double **KnownSolution.Value** *no default* This key specifies the
constant value of the known solution for type **Constant** known
solutions.

::

    pfset KnownSolution.Value  1.0

Only for known solution test cases will information on the
:math:`L^2`-norm of the pressure error be printed.

Wells
~~~~~

Here we define wells for the model. The format for this section of input
is:

string **Wells.Names** *no default* This key specifies the names of the
wells for which input data will be given.

::

    Wells.Names "test_well inj_well ext_well"

::

    Wells.test_well.InputType Vertical

string **Wells.*well\_name*.Action** *no default* This key specifies the
pumping action of the well. This key can be either **Injection** or
**Extraction**. A value of **Injection** indicates that this is an
injection well. A value of **Extraction** indicates that this is an
extraction well.

::

    Wells.test_well.Action Injection

double **Wells.*well\_name*.Type** *no default* This key specfies the
mechanism by which the well works (how ParFlow works with the well data)
if the input type key is set to **Vectical**. This key can be either
**Pressure** or **Flux**. A value of **Pressure** indicates that the
data provided for the well is in terms of hydrostatic pressure and
ParFlow will ensure that the computed pressure field satisfies this
condition in the computational cells which define the well. A value of
**Flux** indicates that the data provided is in terms of volumetric flux
rates and ParFlow will ensure that the flux field satisfies this
condition in the computational cells which define the well.

::

    Wells.test_well.Type Flux

string **Wells.*well\_name*.ExtractionType** *no default* This key
specfies the mechanism by which the extraction well works (how ParFlow
works with the well data) if the input type key is set to **Recirc**.
This key can be either **Pressure** or **Flux**. A value of **Pressure**
indicates that the data provided for the well is in terms of hydrostatic
pressure and ParFlow will ensure that the computed pressure field
satisfies this condition in the computational cells which define the
well. A value of **Flux** indicates that the data provided is in terms
of volumetric flux rates and ParFlow will ensure that the flux field
satisfies this condition in the computational cells which define the
well.

::

    Wells.ext_well.ExtractionType Pressure

string **Wells.*well\_name*.InjectionType** *no default* This key
specfies the mechanism by which the injection well works (how ParFlow
works with the well data) if the input type key is set to **Recirc**.
This key can be either **Pressure** or **Flux**. A value of **Pressure**
indicates that the data provided for the well is in terms of hydrostatic
pressure and ParFlow will ensure that the computed pressure field
satisfies this condition in the computational cells which define the
well. A value of **Flux** indicates that the data provided is in terms
of volumetric flux rates and ParFlow will ensure that the flux field
satisfies this condition in the computational cells which define the
well.

::

    Wells.inj_well.InjectionType Flux

double **Wells.*well\_name*.X** *no default* This key specifies the x
location of the vectical well if the input type is set to **Vectical**
or of both the extraction and injection wells if the input type is set
to **Recirc**.

::

    Wells.test_well.X 20.0

double **Wells.*well\_name*.Y** *no default* This key specifies the y
location of the vectical well if the input type is set to **Vectical**
or of both the extraction and injection wells if the input type is set
to **Recirc**.

::

    Wells.test_well.Y 36.5

double **Wells.*well\_name*.ZUpper** *no default* This key specifies the
z location of the upper extent of a vectical well if the input type is
set to **Vectical**.

::

    Wells.test_well.ZUpper 8.0

double **Wells.*well\_name*.ExtractionZUpper** *no default* This key
specifies the z location of the upper extent of a extraction well if the
input type is set to **Recirc**.

::

    Wells.ext_well.ExtractionZUpper 3.0

double **Wells.*well\_name*.InjectionZUpper** *no default* This key
specifies the z location of the upper extent of a injection well if the
input type is set to **Recirc**.

::

    Wells.inj_well.InjectionZUpper 6.0

double **Wells.*well\_name*.ZLower** *no default* This key specifies the
z location of the lower extent of a vectical well if the input type is
set to **Vectical**.

::

    Wells.test_well.ZLower 2.0

double **Wells.*well\_name*.ExtractionZLower** *no default* This key
specifies the z location of the lower extent of a extraction well if the
input type is set to **Recirc**.

::

    Wells.ext_well.ExtractionZLower 1.0

double **Wells.*well\_name*.InjectionZLower** *no default* This key
specifies the z location of the lower extent of a injection well if the
input type is set to **Recirc**.

::

    Wells.inj_well.InjectionZLower 4.0

string **Wells.*well\_name*.Method** *no default* This key specifies a
method by which pressure or flux for a vertical well will be weighted
before assignment to computational cells. This key can only be
**Standard** if the type key is set to **Pressure**; or this key can be
either **Standard**, **Weighted** or **Patterned** if the type key is
set to **Flux**. A value of **Standard** indicates that the pressure or
flux data will be used as is. A value of **Weighted** indicates that the
flux data is to be weighted by the cells permeability divided by the sum
of all cell permeabilities which define the well. The value of
**Patterned** is not implemented.

::

    Wells.test_well.Method Weighted

string **Wells.*well\_name*.ExtractionMethod** *no default* This key
specifies a method by which pressure or flux for an extraction well will
be weighted before assignment to computational cells. This key can only
be **Standard** if the type key is set to **Pressure**; or this key can
be either **Standard**, **Weighted** or **Patterned** if the type key is
set to **Flux**. A value of **Standard** indicates that the pressure or
flux data will be used as is. A value of **Weighted** indicates that the
flux data is to be weighted by the cells permeability divided by the sum
of all cell permeabilities which define the well. The value of
**Patterned** is not implemented.

::

    Wells.ext_well.ExtractionMethod Standard

string **Wells.*well\_name*.InjectionMethod** *no default* This key
specifies a method by which pressure or flux for an injection well will
be weighted before assignment to computational cells. This key can only
be **Standard** if the type key is set to **Pressure**; or this key can
be either **Standard**, **Weighted** or **Patterned** if the type key is
set to **Flux**. A value of **Standard** indicates that the pressure or
flux data will be used as is. A value of **Weighted** indicates that the
flux data is to be weighted by the cells permeability divided by the sum
of all cell permeabilities which define the well. The value of
**Patterned** is not implemented.

::

    Wells.inj_well.InjectionMethod Standard

string **Wells.*well\_name*.Cycle** *no default* This key specifies the
time cycles to which data for the well *well\_name* corresponds.

::

    Wells.test_well.Cycle "all_time"

::

    Wells.test_well.all_time.Pressure.Value 6.0

::

    Wells.ext_well.all_time.Extraction.Pressure.Value 4.5

::

    Wells.inj_well.all_time.Injection.Pressure.Value 10.2

::

    Wells.test_well.all_time.Flux.water.Value 250.0

::

    Wells.ext_well.all_time.Extraction.Flux.water.Value 125.0

::

    Wells.inj_well.all_time.Injection.Flux.water.Value 80.0

double
**Wells.*well\_name*.*interval\_name*.Saturation.*phase\_name*.Value**
*no default* This key specifies the saturation value of a vertical well.

::

    Wells.test_well.all_time.Saturation.water.Value 1.0

double
**Wells.*well\_name*.*interval\_name*.Concentration.*phase\_name*.*contaminant\_name*.Value**
*no default* This key specifies the contaminant value of a vertical
well.

::

    Wells.test_well.all_time.Concentration.water.tce.Value 0.0005

double
**Wells.*well\_name*.*interval\_name*.Injection.Concentration.*phase\_name*.*contaminant\_name*.Fraction**
*no default* This key specifies the fraction of the extracted
contaminant which gets resupplied to the injection well.

::

    Wells.inj_well.all_time.Injection.Concentration.water.tce.Fraction 0.01

Multiple wells assigned to one grid location can occur in several
instances. The current actions taken by the code are as follows:

-  If multiple pressure wells are assigned to one grid cell, the code
   retains only the last set of overlapping well values entered.

-  If multiple flux wells are assigned to one grid cell, the code sums
   the contributions of all overlapping wells to get one effective well
   flux.

-  If multiple pressure and flux wells are assigned to one grid cell,
   the code retains the last set of overlapping hydrostatic pressure
   values entered and sums all the overlapping flux well values to get
   an effective pressure/flux well value.

Code Parameters
~~~~~~~~~~~~~~~

In addition to input keys related to the physics capabilities and
modeling specifics there are some key values used by various algorithms
and general control flags for ParFlow. These are described next :

string **Solver.Linear** *PCG* This key specifies the linear solver used
for solver **IMPES**. Choices for this key are **MGSemi, PPCG, PCG** and
**CGHS**. The choice **MGSemi** is an algebraic mulitgrid linear solver
(not a preconditioned conjugate gradient) which may be less robust than
**PCG** as described in . The choice **PPCG** is a preconditioned
conjugate gradient solver. The choice **PCG** is a conjugate gradient
solver with a multigrid preconditioner. The choice **CGHS** is a
conjugate gradient solver.

::

    pfset Solver.Linear   MGSemi

integer **Solver.SadvectOrder** *2* This key controls the order of the
explicit method used in advancing the saturations. This value can be
either 1 for a standard upwind first order or 2 for a second order
Godunov method.

::

    pfset Solver.SadvectOrder 1

integer **Solver.AdvectOrder** *2* This key controls the order of the
explicit method used in advancing the concentrations. This value can be
either 1 for a standard upwind first order or 2 for a second order
Godunov method.

::

    pfset Solver.AdvectOrder 2

double **Solver.CFL** *0.7* This key gives the value of the weight put
on the computed CFL limit before computing a global timestep value.
Values greater than 1 are not suggested and in fact because this is an
approximation, values slightly less than 1 can also produce
instabilities.

::

    pfset Solver.CFL 0.7

integer **Solver.MaxIter** *1000000* This key gives the maximum number
of iterations that will be allowed for time-stepping. This is to prevent
a run-away simulation.

::

    pfset Solver.MaxIter 100

double **Solver.RelTol** *1.0* This value gives the relative tolerance
for the linear solve algorithm.

::

    pfset Solver.RelTol 1.0

double **Solver.AbsTol** *1E-9* This value gives the absolute tolerance
for the linear solve algorithm.

::

    pfset Solver.AbsTol 1E-8

double **Solver.Drop** *1E-8* This key gives a clipping value for data
written to PFSB files. Data values greater than the negative of this
value and less than the value itself are treated as zero and not written
to PFSB files.

::

    pfset Solver.Drop 1E-6

string **Solver.PrintSubsurf** *True* This key is used to turn on
printing of the subsurface data, Permeability and Porosity. The data is
printed after it is generated and before the main time stepping loop -
only once during the run. The data is written as a PFB file.

::

    pfset Solver.PrintSubsurf False

string **Solver.PrintPressure** *True* This key is used to turn on
printing of the pressure data. The printing of the data is controlled by
values in the timing information section. The data is written as a PFB
file.

::

    pfset Solver.PrintPressure False

string **Solver.PrintVelocities** *False* This key is used to turn on
printing of the x, y and z velocity data. The printing of the data is
controlled by values in the timing information section. The data is
written as a PFB file.

::

    pfset Solver.PrintVelocities True

string **Solver.PrintSaturation** *True* This key is used to turn on
printing of the saturation data. The printing of the data is controlled
by values in the timing information section. The data is written as a
PFB file.

::

    pfset Solver.PrintSaturation False

string **Solver.PrintConcentration** *True* This key is used to turn on
printing of the concentration data. The printing of the data is
controlled by values in the timing information section. The data is
written as a PFSB file.

::

    pfset Solver.PrintConcentration False

string **Solver.PrintWells** *True* This key is used to turn on
collection and printing of the well data. The data is collected at
intervals given by values in the timing information section. Printing
occurs at the end of the run when all collected data is written.

::

    pfset Solver.PrintWells False

string **Solver.PrintLSMSink** *False* This key is used to turn on
printing of the flux array passed from CLM to ParFlow. Printing occurs
at each **DumpInterval** time.

::

    pfset Solver.PrintLSMSink True

string **Solver.WriteSiloSubsurfData** *False* This key is used to
specify printing of the subsurface data, Permeability and Porosity in
silo binary file format. The data is printed after it is generated and
before the main time stepping loop - only once during the run. This data
may be read in by VisIT and other visualization packages.

::

    pfset Solver.WriteSiloSubsurfData True

string **Solver.WriteSiloPressure** *False* This key is used to specify
printing of the saturation data in silo binary format. The printing of
the data is controlled by values in the timing information section. This
data may be read in by VisIT and other visualization packages.

::

    pfset Solver.WriteSiloPressure True

string **Solver.WriteSiloSaturation** *False* This key is used to
specify printing of the saturation data using silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloSaturation True

string **Solver.WriteSiloConcentration** *False* This key is used to
specify printing of the concentration data in silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloConcentration True

string **Solver.WriteSiloVelocities** *False* This key is used to
specify printing of the x, y and z velocity data in silo binary format.
The printing of the data is controlled by values in the timing
information section.

::

    pfset Solver.WriteSiloVelocities True

string **Solver.WriteSiloSlopes** *False* This key is used to specify
printing of the x and y slope data using silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloSlopes  True

string **Solver.WriteSiloMannings** *False* This key is used to specify
printing of the Manning’s roughness data in silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloMannings True

string **Solver.WriteSiloSpecificStorage** *False* This key is used to
specify printing of the specific storage data in silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloSpecificStorage True

string **Solver.WriteSiloMask** *False* This key is used to specify
printing of the mask data using silo binary format. The mask contains
values equal to one for active cells and zero for inactive cells. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloMask  True

string **Solver.WriteSiloEvapTrans** *False* This key is used to specify
printing of the evaporation and rainfall flux data using silo binary
format. This data comes from either clm or from external calls to
ParFlow such as *WRF*. This data is in units of :math:`[L^3 T^{-1}]`.
The printing of the data is controlled by values in the timing
information section.

::

    pfset Solver.WriteSiloEvapTrans  True

string **Solver.WriteSiloEvapTransSum** *False* This key is used to
specify printing of the evaporation and rainfall flux data using silo
binary format as a running, cumulative amount. This data comes from
either clm or from external calls to ParFlow such as *WRF*. This data is
in units of :math:`[L^3]`. The printing of the data is controlled by
values in the timing information section.

::

    pfset Solver.WriteSiloEvapTransSum  True

string **Solver.WriteSiloOverlandSum** *False* This key is used to
specify calculation and printing of the total overland outflow from the
domain using silo binary format as a running cumulative amount. This is
integrated along all domain boundaries and is calculated any location
that slopes at the edge of the domain point outward. This data is in
units of :math:`[L^3]`. The printing of the data is controlled by values
in the timing information section.

::

    pfset Solver.WriteSiloOverlandSum  True

string **Solver.TerrainFollowingGrid** *False* This key specifies that a
terrain-following coordinate transform is used for solver Richards. This
key sets x and y subsurface slopes to be the same as the Topographic
slopes (a value of False sets these subsurface slopes to zero). These
slopes are used in the Darcy fluxes to add a density, gravity -dependent
term. This key will not change the output files (that is the output is
still orthogonal) or the geometries (they will still follow the
computational grid)– these two things are both to do items. This key
only changes solver Richards, not solver Impes.

::

    pfset Solver.TerrainFollowingGrid                        True

SILO Options
~~~~~~~~~~~~

The following keys are used to control how SILO writes data. SILO allows
writing to PDB and HDF5 file formats. SILO also allows data compression
to be used, which can save signicant amounts of disk space for some
problems.

string **SILO.Filetype** *PDB* This key is used to specify the SILO
filetype. Allowed values are PDB and HDF5. Note that you must have
configured SILO with HDF5 in order to use that option.

::

    pfset SILO.Filetype  PDB

string **SILO.CompressionOptions** ** This key is used to specify the
SILO compression options. See the SILO manual for the DB\_SetCompression
command for information on available options. NOTE: the options
avaialable are highly dependent on the configure options when building
SILO.

::

    pfset SILO.CompressionOptions  ``METHOD=GZIP''

Richards’ Equation Solver Parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following keys are used to specify various parameters used by the
linear and nonlinear solvers in the Richards’ equation implementation.
For information about these solvers, see and .

double **Solver.Nonlinear.ResidualTol** *1e-7* This key specifies the
tolerance that measures how much the relative reduction in the nonlinear
residual should be before nonlinear iterations stop. The magnitude of
the residual is measured with the :math:`l^1` (max) norm.

::

    pfset Solver.Nonlinear.ResidualTol   1e-4

double **Solver.Nonlinear.StepTol** *1e-7* This key specifies the
tolerance that measures how small the difference between two consecutive
nonlinear steps can be before nonlinear iterations stop.

::

    pfset Solver.Nonlinear.StepTol   1e-4

integer **Solver.Nonlinear.MaxIter** *15* This key specifies the maximum
number of nonlinear iterations allowed before iterations stop with a
convergence failure.

::

    pfset Solver.Nonlinear.MaxIter   50

integer **Solver.Linear.KrylovDimension** *10* This key specifies the
maximum number of vectors to be used in setting up the Krylov subspace
in the GMRES iterative solver. These vectors are of problem size and it
should be noted that large increases in this parameter can limit problem
sizes. However, increasing this parameter can sometimes help nonlinear
solver convergence.

::

    pfset Solver.Linear.KrylovDimension   15

integer **Solver.Linear.MaxRestarts** *0* This key specifies the number
of restarts allowed to the GMRES solver. Restarts start the development
of the Krylov subspace over using the current iterate as the initial
iterate for the next pass.

::

    pfset Solver.Linear.MaxRestarts   2

::

    pfset Solver.MaxConvergenceFailures 4

string **Solver.Nonlinear.PrintFlag** *HighVerbosity* This key specifies
the amount of informational data that is printed to the
\*.out.kinsol.log file. Choices for this key are **NoVerbosity,
LowVerbosity, NormalVerbosity** and **HighVerbosity**. The choice
**NoVerbosity** prints no statistics about the nonlinear convergence
process. The choice **LowVerbosity** outputs the nonlinear iteration
count, the scaled norm of the nonlinear function, and the number of
function calls. The choice **NormalVerbosity** prints the same as for
**LowVerbosity** and also the global strategy statistics. The choice
**HighVerbosity** prints the same as for **NormalVerbosity** with the
addition of further Krylov iteration statistics.

::

    pfset Solver.Nonlinear.PrintFlag   NormalVerbosity

string **Solver.Nonlinear.EtaChoice** *Walker2* This key specifies how
the linear system tolerance will be selected. The linear system is
solved until a relative residual reduction of :math:`\eta` is achieved.
Linear residuall norms are measured in the :math:`l^2` norm. Choices for
this key include **EtaConstant, Walker1** and **Walker2**. If the choice
**EtaConstant** is specified, then :math:`\eta` will be taken as
constant. The choices **Walker1** and **Walker2** specify choices for
:math:`\eta` developed by Eisenstat and Walker . The choice **Walker1**
specifies that :math:`\eta` will be given by
:math:`| \|F(u^k)\| - \|F(u^{k-1}) + J(u^{k-1})*p \|  |  / \|F(u^{k-1})\|`.
The choice **Walker2** specifies that :math:`\eta` will be given by
:math:`\gamma \|F(u^k)\| / \|F(u^{k-1})\|^{\alpha}`. For both of the
last two choices, :math:`\eta` is never allowed to be less than 1e-4.

::

    pfset Solver.Nonlinear.EtaChoice   EtaConstant

double **Solver.Nonlinear.EtaValue** *1e-4* This key specifies the
constant value of :math:`\eta` for the EtaChoice key **EtaConstant**.

::

    pfset Solver.Nonlinear.EtaValue   1e-7

double **Solver.Nonlinear.EtaAlpha** *2.0* This key specifies the value
of :math:`\alpha` for the case of EtaChoice being **Walker2**.

::

    pfset Solver.Nonlinear.EtaAlpha   1.0

double **Solver.Nonlinear.EtaGamma** *0.9* This key specifies the value
of :math:`\gamma` for the case of EtaChoice being **Walker2**.

::

    pfset Solver.Nonlinear.EtaGamma   0.7

string **Solver.Nonlinear.UseJacobian** *False* This key specifies
whether the Jacobian will be used in matrix-vector products or whether a
matrix-free version of the code will run. Choices for this key are
**False** and **True**. Using the Jacobian will most likely decrease the
number of nonlinear iterations but require more memory to run.

::

    pfset Solver.Nonlinear.UseJacobian   True

double **Solver.Nonlinear.DerivativeEpsilon** *1e-7* This key specifies
the value of :math:`\epsilon` used in approximating the action of the
Jacobian on a vector with approximate directional derivatives of the
nonlinear function. This parameter is only used when the UseJacobian key
is **False**.

::

    pfset Solver.Nonlinear.DerivativeEpsilon   1e-8

string **Solver.Nonlinear.Globalization** *LineSearch* This key
specifies the type of global strategy to use. Possible choices for this
key are **InexactNewton** and **LineSearch**. The choice
**InexactNewton** specifies no global strategy, and the choice
**LineSearch** specifies that a line search strategy should be used
where the nonlinear step can be lengthened or decreased to satisfy
certain criteria.

::

    pfset Solver.Nonlinear.Globalization   LineSearch

string **Solver.Linear.Preconditioner** *MGSemi* This key specifies
which preconditioner to use. Currently, the three choices are **NoPC,
MGSemi, PFMG, PFMGOctree** and **SMG**. The choice **NoPC** specifies
that no preconditioner should be used. The choice **MGSemi** specifies a
semi-coarsening multigrid algorithm which uses a point relaxation
method. The choice **SMG** specifies a semi-coarsening multigrid
algorithm which uses plane relaxations. This method is more robust than
**MGSemi**, but generally requires more memory and compute time. The
choice **PFMGOctree** can be more efficient for problems with large
numbers of inactive cells.

::

    pfset Solver.Linear.Preconditioner   MGSemi

string **Solver.Linear.Preconditioner.SymmetricMat** *Symmetric* This
key specifies whether the preconditioning matrix is symmetric. Choices
fo rthis key are **Symmetric** and **Nonsymmetric**. The choice
**Symmetric** specifies that the symmetric part of the Jacobian will be
used as the preconditioning matrix. The choice **Nonsymmetric**
specifies that the full Jacobian will be used as the preconditioning
matrix. NOTE: ONLY **Symmetric** CAN BE USED IF MGSemi IS THE SPECIFIED
PRECONDITIONER!

::

    pfset Solver.Linear.Preconditioner.SymmetricMat     Symmetric

integer **Solver.Linear.Preconditioner.*precond\_method*.MaxIter** *1*
This key specifies the maximum number of iterations to take in solving
the preconditioner system with *precond\_method* solver.

::

    pfset Solver.Linear.Preconditioner.SMG.MaxIter    2

This key specifies the number of relaxations to take before coarsening
in the specified preconditioner method. Note that this key is only
relevant to the SMG multigrid preconditioner.

::

    pfset Solver.Linear.Preconditioner.SMG.NumPreRelax    2

This key specifies the number of relaxations to take after coarsening in
the specified preconditioner method. Note that this key is only relevant
to the SMG multigrid preconditioner.

::

    pfset Solver.Linear.Preconditioner.SMG.NumPostRelax    0

For the PFMG solver, this key specifies the *Hypre* RAP type. Valid
values are **Galerkin** or **NonGalerkin**

::

    pfset Solver.Linear.Preconditioner.PFMG.RAPType    Galerkin

logical **Solver.EvapTransFile** *False* This key specifies specifies
that the Flux terms for Richards’ equation are read in from a .pfb file.
This file has :math:`[T^-1]` units. Note this key is for a steady-state
flux and should *not* be used in conjunction with the transient key
below.

::

    pfset Solver.EvapTransFile    True

logical **Solver.EvapTransFileTransient** *False* This key specifies
specifies that the Flux terms for Richards’ equation are read in from a
series of flux .pfb file. Each file has :math:`[T^-1]` units. Note this
key should not be used with the key above, only one of these keys should
be set to True at a time, not both.

::

    pfset Solver.EvapTransFileTransient    True

string **Solver.EvapTrans.FileName** *no default* This key specifies
specifies filename for the distributed .pfb file that contains the flux
values for Richards’ equation. This file has :math:`[T^-1]` units. For
the steady-state option (*Solver.EvapTransFile=\ **True***) this key
should be the complete filename. For the transient option
(*Solver.EvapTransFileTransient=\ **True*** then the filename is a
header and ParFlow will load one file per timestep, with the form
filename.00000.pfb.

::

    pfset Solver.EvapTrans.FileName   evap.trans.test.pfb

string **Solver.LSM** *none* This key specifies whether a land surface
model, such as CLM, will be called each solver timestep. Choices for
this key include **none** and **CLM**. Note that CLM must be compiled
and linked at runtime for this option to be active.

::

    pfset Solver.LSM CLM

Spinup Options
~~~~~~~~~~~~~~

These keys allow for *reduced or dampened physics* during model spinup
or initialization. They are **only** intended for these initialization
periods, **not** for regular runtime.

integer **OverlandFlowSpinUp** *0* This key specifies that a
*simplified* form of the overland flow boundary condition (Equation
[eq:overland:sub:`b`\ c]) be used in place of the full equation. This
formulation *removes lateral flow* and drives and ponded water pressures
to zero. While this can be helpful in spinning up the subsurface, this
is no longer coupled subsurface-surface flow. If set to zero (the
default) this key behaves normally.

::

    pfset OverlandFlowSpinUp   1

double **OverlandFlowSpinUpDampP1** *0.0* This key sets :math:`P_1` and
provides exponential dampening to the pressure relationship in the
overland flow equation by adding the following term:
:math:`P_2*exp(\psi*P_2)`

::

    pfset OverlandSpinupDampP1  10.0

double **OverlandFlowSpinUpDampP2** *0.0* This key sets :math:`P_2` and
provides exponential dampening to the pressure relationship in the
overland flow equation adding the following term:
:math:`P_2*exp(\psi*P_2)`

::

    pfset OverlandSpinupDampP2  0.1

CLM Solver Parameters
~~~~~~~~~~~~~~~~~~~~~

string **Solver.CLM.Print1dOut** *False* This key specifies whether the
CLM one dimensional (averaged over each processor) output file is
written or not. Choices for this key include **True** and **False**.
Note that CLM must be compiled and linked at runtime for this option to
be active.

::

    pfset Solver.CLM.Print1dOut   False

integer **Solver.CLM.IstepStart** *1* This key specifies the value of
the counter, *istep* in CLM. This key primarily determines the start of
the output counter for CLM.It is used to restart a run by setting the
key to the ending step of the previous run plus one. Note that CLM must
be compiled and linked at runtime for this option to be active.

::

    pfset Solver.CLM.IstepStart     8761      

String **Solver.CLM.MetForcing** *no default* This key specifies defines
whether 1D (uniform over the domain), 2D (spatially distributed) or 3D
(spatially distributed with multiple timesteps per .pfb forcing file)
forcing data is used. Choices for this key are **1D**, **2D** and
**3D**. This key has no default so the user *must* set it to 1D, 2D or
3D. Failure to set this key will cause CLM to still be run but with
unpredictable values causing CLM to eventually crash. 1D meteorological
forcing files are text files with single columns for each variable and
each timestep per row, while 2D forcing files are distributed ParFlow
binary files, one for each variable and timestep. File names are
specified in the **Solver.CLM.MetFileName** variable below. Note that
CLM must be compiled and linked at runtime for this option to be active.

::

    pfset Solver.CLM.MetForcing   2D       

::

    pfset Solver.CLM.MetFileName                             narr.1hr.txt

String **Solver.CLM.MetFilePath** *no default* This key specifies
defines the location of 1D, 2D or 3D forcing data. For 1D cases, this is
the path to a single forcing file (*e.g.* narr.1hr.txt). For 2D and 3D
cases, this is the path to the directory containing all forcing files.
Note that CLM must be compiled and linked at runtime for this option to
be active.

::

    pfset Solver.CLM.MetFilePath		"path/to/met/forcing/data/"

integer **Solver.CLM.MetFileNT** *no default* This key specifies the
number of timesteps per file for 3D forcing data.

::

    pfset Solver.CLM.MetFileNT	24	

::

    pfset Solver.CLM.ForceVegetation  True

string **Solver.WriteSiloCLM** *False* This key specifies whether the
CLM writes two dimensional binary output files to a silo binary format.
This data may be read in by VisIT and other visualization packages. Note
that CLM and silo must be compiled and linked at runtime for this option
to be active. These files are all written according to the standard
format used for all ParFlow variables, using the *runname*, and *istep*.
Variables are either two-dimensional or over the number of CLM layers
(default of ten).

::

    pfset Solver.WriteSiloCLM True

The output variables are:

eflx\ :sub:`l`\ h\ :sub:`t`\ ot for latent heat flux total
:math:`[W/m^2]` using the silo variable *LatentHeat*;

eflx\ :sub:`l`\ wrad\ :sub:`o`\ ut for outgoing long-wave radiation
:math:`[W/m^2]` using the silo variable *LongWave*;

eflx\ :sub:`s`\ h\ :sub:`t`\ ot for sensible heat flux total
:math:`[W/m^2]` using the silo variable *SensibleHeat*;

eflx\ :sub:`s`\ oil\ :sub:`g`\ rnd for ground heat flux :math:`[W/m^2]`
using the silo variable *GroundHeat*;

qflx\ :sub:`e`\ vap\ :sub:`t`\ ot for total evaporation :math:`[mm/s]`
using the silo variable *EvaporationTotal*;

qflx\ :sub:`e`\ vap\ :sub:`g`\ rnd for ground evaporation without
condensation :math:`[mm/s]` using the silo variable
*EvaporationGroundNoSublimation*;

qflx\ :sub:`e`\ vap\ :sub:`s`\ oi for soil evaporation :math:`[mm/s]`
using the silo variable *EvaporationGround*;

qflx\ :sub:`e`\ vap\ :sub:`v`\ eg for vegetation evaporation
:math:`[mm/s]` using the silo variable *EvaporationCanopy*;

qflx\ :sub:`t`\ ran\ :sub:`v`\ eg for vegetation transpiration
:math:`[mm/s]` using the silo variable *Transpiration*;

qflx\ :sub:`i`\ nfl for soil infiltration :math:`[mm/s]` using the silo
variable *Infiltration*;

swe\ :sub:`o`\ ut for snow water equivalent :math:`[mm]` using the silo
variable *SWE*;

t\ :sub:`g`\ rnd for ground surface temperature :math:`[K]` using the
silo variable *TemperatureGround*; and

t\ :sub:`s`\ oil for soil temperature over all layers :math:`[K]` using
the silo variable *TemperatureSoil*.

string **Solver.PrintCLM** *False* This key specifies whether the CLM
writes two dimensional binary output files to a PFB binary format. Note
that CLM must be compiled and linked at runtime for this option to be
active. These files are all written according to the standard format
used for all ParFlow variables, using the *runname*, and *istep*.
Variables are either two-dimensional or over the number of CLM layers
(default of ten).

::

    pfset Solver.PrintCLM True

The output variables are:

eflx\ :sub:`l`\ h\ :sub:`t`\ ot for latent heat flux total
:math:`[W/m^2]` using the silo variable *LatentHeat*;

eflx\ :sub:`l`\ wrad\ :sub:`o`\ ut for outgoing long-wave radiation
:math:`[W/m^2]` using the silo variable *LongWave*;

eflx\ :sub:`s`\ h\ :sub:`t`\ ot for sensible heat flux total
:math:`[W/m^2]` using the silo variable *SensibleHeat*;

eflx\ :sub:`s`\ oil\ :sub:`g`\ rnd for ground heat flux :math:`[W/m^2]`
using the silo variable *GroundHeat*;

qflx\ :sub:`e`\ vap\ :sub:`t`\ ot for total evaporation :math:`[mm/s]`
using the silo variable *EvaporationTotal*;

qflx\ :sub:`e`\ vap\ :sub:`g`\ rnd for ground evaporation without
sublimation :math:`[mm/s]` using the silo variable
*EvaporationGroundNoSublimation*;

qflx\ :sub:`e`\ vap\ :sub:`s`\ oi for soil evaporation :math:`[mm/s]`
using the silo variable *EvaporationGround*;

qflx\ :sub:`e`\ vap\ :sub:`v`\ eg for vegetation evaporation
:math:`[mm/s]` using the silo variable *EvaporationCanopy*;

qflx\ :sub:`t`\ ran\ :sub:`v`\ eg for vegetation transpiration
:math:`[mm/s]` using the silo variable *Transpiration*;

qflx\ :sub:`i`\ nfl for soil infiltration :math:`[mm/s]` using the silo
variable *Infiltration*;

swe\ :sub:`o`\ ut for snow water equivalent :math:`[mm]` using the silo
variable *SWE*;

t\ :sub:`g`\ rnd for ground surface temperature :math:`[K]` using the
silo variable *TemperatureGround*; and

t\ :sub:`s`\ oil for soil temperature over all layers :math:`[K]` using
the silo variable *TemperatureSoil*.

string **Solver.WriteCLMBinary** *True* This key specifies whether the
CLM writes two dimensional binary output files in a generic binary
format. Note that CLM must be compiled and linked at runtime for this
option to be active.

::

    pfset Solver.WriteCLMBinary False

string **Solver.CLM.BinaryOutDir** *True* This key specifies whether the
CLM writes each set of two dimensional binary output files to a
corresponding directory. These directories my be created before ParFlow
is run (using the tcl script, for example). Choices for this key include
**True** and **False**. Note that CLM must be compiled and linked at
runtime for this option to be active.

::

    pfset Solver.CLM.BinaryOutDir True

These directories are:

/qflx:sub:`t`\ op\ :sub:`s`\ oil for soil flux;

/qflx:sub:`i`\ nfl for infiltration;

/qflx:sub:`e`\ vap\ :sub:`g`\ rnd for ground evaporation;

/eflx:sub:`s`\ oil\ :sub:`g`\ rnd for ground heat flux;

/qflx:sub:`e`\ vap\ :sub:`v`\ eg for vegetation evaporation;

/eflx:sub:`s`\ h\ :sub:`t`\ ot for sensible heat flux;

/eflx:sub:`l`\ h\ :sub:`t`\ ot for latent heat flux;

/qflx:sub:`e`\ vap\ :sub:`t`\ ot for total evaporation;

/t:sub:`g`\ rnd for ground surface temperature;

/qflx:sub:`e`\ vap\ :sub:`s`\ oi for soil evaporation;

/qflx:sub:`t`\ ran\ :sub:`v`\ eg for vegetation transpiration;

/eflx:sub:`l`\ wrad\ :sub:`o`\ ut for outgoing long-wave radiation;

/swe:sub:`o`\ ut for snow water equivalent; and

/diag:sub:`o`\ ut for diagnostics.

string **Solver.CLM.CLMFileDir** *no default* This key specifies what
directory all output from the CLM is written to. This key may be set to
“./” or “” to write output to the ParFlow run directory. This directory
must be created before ParFlow is run. Note that CLM must be compiled
and linked at runtime for this option to be active.

::

    pfset Solver.CLM.CLMFileDir "CLM_Output/"

integer **Solver.CLM.CLMDumpInterval** *1* This key specifies how often
output from the CLM is written. This key is in integer multipliers of
the CLM timestep. Note that CLM must be compiled and linked at runtime
for this option to be active.

::

    pfset Solver.CLM.CLMDumpInterval 2

::

    pfset Solver.CLM.EvapBeta Linear

double **Solver.CLM.ResSat** *0.1* This key specifies the residual
saturation for the :math:`\beta` function in CLM specified above. Note
that CLM must be compiled and linked at runtime for this option to be
active.

::

    pfset Solver.CLM.ResSat  0.15

::

    pfset Solver.CLM.VegWaterStress  Pressure

double **Solver.CLM.WiltingPoint** *0.1* This key specifies the wilting
point for the :math:`\beta_t` function in CLM specified above. Note that
the units for this function are pressure :math:`[m]` for a **Pressure**
formulation and saturation :math:`[-]` for a **Saturation** formulation.
Note that CLM must be compiled and linked at runtime for this option to
be active.

::

    pfset Solver.CLM.WiltingPoint  0.15

double **Solver.CLM.FieldCapacity** *1.0* This key specifies the field
capacity for the :math:`\beta_t` function in CLM specified above. Note
that the units for this function are pressure :math:`[m]` for a
**Pressure** formulation and saturation :math:`[-]` for a **Saturation**
formulation. Note that CLM must be compiled and linked at runtime for
this option to be active.

::

    pfset Solver.CLM.FieldCapacity  0.95

string **Solver.CLM.IrrigationTypes** *none* This key specifies the form
of the irrigation in CLM. The valid types for this key are **none,
Spray, Drip, Instant**.

::

    pfset Solver.CLM.IrrigationTypes Drip

string **Solver.CLM.IrrigationCycle** *Constant* This key specifies the
cycle of the irrigation in CLM. The valid types for this key are
**Constant, Deficit**. Note only **Constant** is currently implemented.
Constant cycle applies irrigation each day from IrrigationStartTime to
IrrigationStopTime in GMT.

::

    pfset Solver.CLM.IrrigationCycle Constant

double **Solver.CLM.IrrigationRate** *no default* This key specifies the
rate of the irrigation in CLM in [mm/s].

::

    pfset Solver.CLM.IrrigationRate 10.

double **Solver.CLM.IrrigationStartTime** *no default* This key
specifies the start time of the irrigation in CLM GMT.

::

    pfset Solver.CLM.IrrigationStartTime 0800

double **Solver.CLM.IrrigationStopTime** *no default* This key specifies
the stop time of the irrigation in CLM GMT.

::

    pfset Solver.CLM.IrrigationStopTime 1200

double **Solver.CLM.IrrigationThreshold** *0.5* This key specifies the
threshold value for the irrigation in CLM [-].

::

    pfset Solver.CLM.IrrigationThreshold 0.2

integer **Solver.CLM.ReuseCount** *1* How many times to reuse a CLM
atmospheric forcing file input. For example timestep=1, reuse =1 is
normal behavior but reuse=2 and timestep=0.5 subdivides the time step
using the same CLM input for both halves instead of needing two files.
This is particually useful for large, distributed runs when the user
wants to run ParFlow at a smaller timestep than the CLM forcing. Forcing
files will be re-used and total fluxes adjusted accordingly without
needing duplicate files.

::

    pfset Solver.CLM.ReuseCount      5

string **Solver.CLM.WriteLogs** *True* When **False**, this disables
writing of the CLM output log files for each processor. For example, in
the clm.tcl test case, if this flag is added **False**,
washita.output.txt.\ *p* and washita.para.out.dat.\ *p* (were *p* is the
processor #) are not created, assuming *washita* is the run name.

::

    pfset Solver.CLM.WriteLogs    False

string **Solver.CLM.WriteLastRST** *False* Controls whether CLM restart
files are sequentially written or whether a single file *restart file
name*.00000.\ *p* is overwritten each time the restart file is output,
where *p* is the processor number. If “True” only one file is
written/overwritten and if “False” outputs are written more frequently.
Compatible with DailyRST and ReuseCount; for the latter, outputs are
written every n steps where n is the value of ReuseCount.

::

    pfset Solver.CLM.WriteLastRST   True

string **Solver.CLM.DailyRST** *True* Controls whether CLM writes daily
restart files (default) or at every time step when set to False; outputs
are numbered according to the istep from ParFlow. If **ReuseCount=n**,
with n greater than 1, the output will be written every n steps (i.e. it
still writes hourly restart files if your time step is 0.5 or 0.25,
etc...). Fully compatible with **WriteLastRST=False** so that each daily
output is overwritten to time 00000 in *restart file name*.00000.p where
*p* is the processor number.

::

    pfset Solver.CLM.DailyRST    False

string **Solver.CLM.SingleFile** *False* Controls whether ParFlow writes
all CLM output variables as a single file per time step. When “True”,
this combines the output of all the CLM output variables into a special
multi-layer PFB with the file extension “.C.pfb”. The first 13 layers
correspond to the 2-D CLM outputs and the remaining layers are the soil
temperatures in each layer. For example, a model with 4 soil layers will
create a SingleFile CLM output with 17 layers at each time step. The
file pseudo code is given below in § [ParFlow Binary Files (.c.pfb)] and
the variables and units are as specified in the multiple PFB and SILO
formats as above.

::

    pfset Solver.CLM.SingleFile   True

integer **Solver.CLM.RootZoneNZ** *10* This key sets the number of soil
layers the ParFlow expects from CLM. It will allocate and format all the
arrays for passing variables to and from CLM accordingly. Note that this
does not set the soil layers in CLM to do that the user needs to change
the value of the parameter nlevsoi in the file clm\_varpar.F90 in the
*PARFLOW\_DIR*\\pfsimulator\\clm directory to reflect the desired
numnber of soil layers and recompile. Most likely the key
Solver.CLM.SoiLayer, described below, will also need to be changed.

::

    pfset Solver.CLM.RootZoneNZ      4

integer **Solver.CLM.SoiLayer** *7* This key sets the soil layer, and
thus the soil depth, that CLM uses for the seasonal temperature
adjustment for all leaf and stem area indices.

::

    pfset Solver.CLM.SoiLayer      4

integer **Solver.CLM.SoilLevels** *10* This key sets the number of soil
levels for CLM.

::

    pfset Solver.CLM.SoilLevels      4

integer **Solver.CLM.LakeLevels** *10* This key sets the number of lake
levels for CLM.

::

    pfset Solver.CLM.LakeLevels      4

ParFlow NetCDF4 Parallel I/O
----------------------------

NetCDF4 parallel I/O is being implemented in ParFlow. As of now only
output capability is implemented. Input functionality will be added in
later version. Currently user has option of printing 3-D time varying
pressure or saturation or both in a single NetCDF file containing
multiple time steps. User should configure ParFlow(pfsimulatior part) “-
-with-netcdf” option and link the appropriate NetCDF4 library. Naming
convention of output files is analogues to binary file names. Following
options are available for NetCDF4 output along with various performance
tuning options. User is advised to explore NetCDF4 chunking and ROMIO
hints option for better I/O performance.

***HDF5 Library version 1.8.16 or higher is required for NetCDF4
parallel I/O***

integer **NetCDF.NumStepsPerFile** * * This key sets number of time
steps user wishes to output in a NetCDF4 file. Once the time step count
increases beyond this number, a new file is automatically created.

::

    pfset NetCDF.NumStepsPerFile    5

string **NetCDF.WritePressure** *False* This key sets pressure variable
to be written in NetCDF4 file.

::

    pfset NetCDF.WritePressure    True

string **NetCDF.WriteSaturation** *False* This key sets saturation
variable to be written in NetCDF4 file.

::

    pfset NetCDF.WriteSaturation    True

string **NetCDF.WriteMannings** *False* This key sets Mannings
coefficients to be written in NetCDF4 file.

::

    pfset NetCDF.WriteMannings	    True

string **NetCDF.WriteSubsurface** *False* This key sets subsurface
data(permeabilities, porosity, specific storage) to be written in
NetCDF4 file.

::

    pfset NetCDF.WriteSubsurface	    True

string **NetCDF.WriteSlopes** *False* This key sets x and y slopes to be
written in NetCDF4 file.

::

    pfset NetCDF.WriteSlopes	    True

string **NetCDF.WriteMask** *False* This key sets mask to be written in
NetCDF4 file.

::

    pfset NetCDF.WriteMask	    True

string **NetCDF.WriteDZMultiplier** *False* This key sets DZ multipliers
to be written in NetCDF4 file.

::

    pfset NetCDF.WriteDZMultiplier	    True

string **NetCDF.WriteEvapTrans** *False* This key sets Evaptrans to be
written in NetCDF4 file.

::

    pfset NetCDF.WriteEvapTrans	    True

string **NetCDF.WriteEvapTransSum** *False* This key sets Evaptrans sum
to be written in NetCDF4 file.

::

    pfset NetCDF.WriteEvapTransSum	    True

string **NetCDF.WriteOverlandSum** *False* This key sets overland sum to
be written in NetCDF4 file.

::

    pfset NetCDF.WriteOverlandSum	    True

string **NetCDF.WriteOverlandBCFlux** *False* This key sets overland bc
flux to be written in NetCDF4 file.

::

    pfset NetCDF.WriteOverlandBCFlux	    True

NetCDF4 Chunking
~~~~~~~~~~~~~~~~

Chunking may have significant impact on I/O. If this key is not set,
default chunking scheme will be used by NetCDF library. Chunks are
hypercube(hyperslab) of any dimension. When chunking is used, chunks are
written in single write operation which can reduce access times. For
more information on chunking, refer to NetCDF4 user guide.

string **NetCDF.Chunking** *False* This key sets chunking for each time
varying 3-D variable in NetCDF4 file.

::

    pfset NetCDF.Chunking    True

Following keys are used only when **NetCDF.Chunking** is set to true.
These keys are used to set chunk sizes in x, y and z direction. A
typical size of chunk in each direction should be equal to number of
grid points in each direction for each processor. e.g. If we are using a
grid of 400(x)X400(y)X30(z) with 2-D domain decomposition of 8X8, then
each core has 50(x)X50(y)X30(z) grid points. These values can be used to
set chunk sizes each direction. For unequal distribution, chunk sizes
should as large as largest value of grid points on the processor. e.g.
If one processor has grid distribution of 40(x)X40(y)X30(z) and another
has 50(x)X50(y)X30(z), the later values should be used to set chunk
sizes in each direction.

integer **NetCDF.ChunkX** *None* This key sets chunking size in
x-direction.

::

    pfset NetCDF.ChunkX    50

integer **NetCDF.ChunkY** *None* This key sets chunking size in
y-direction.

::

    pfset NetCDF.ChunkY    50

integer **NetCDF.ChunkZ** *None* This key sets chunking size in
z-direction.

::

    pfset NetCDF.ChunkZ    30

ROMIO Hints
~~~~~~~~~~~

ROMIO is a poratable MPI-IO implementation developed at Argonne National
Laboratory, USA. Currently it is released as a part of MPICH. ROMIO sets
hints to optimize I/O operations for MPI-IO layer through MPI\_Info
object. This object is passed on to NetCDF4 while creating a file. ROMIO
hints are set in a text file in “key” and “value” pair. *For correct
settings contact your HPC site administrator*. As in chunking, ROMIO
hints can have significant performance impact on I/O.

string **NetCDF.ROMIOhints** *None* This key sets ROMIO hints file to be
passed on to NetCDF4 interface.If this key is set, the file must be
present and readable in experiment directory.

::

    pfset NetCDF.ROMIOhints    romio.hints

An example ROMIO hints file looks as follows.

::

    romio_ds_write disable
    romio_ds_read disable
    romio_cb_write enable
    romio_cb_read enable
    cb_buffer_size 33554432

Node Level Collective I/O
~~~~~~~~~~~~~~~~~~~~~~~~~

A node level collective strategy has been implemented for I/O. One
process on each compute node gathers the data, indices and counts from
the participating processes on same compute node. All the root processes
from each compute node open a parallel NetCDF4 file and write the data.
e.g. If ParFlow is running on 3 compute nodes where each node consists
of 24 processors(cores); only 3 I/O streams to filesystem would be
opened by each root processor each compute node. This strategy could be
particularly useful when ParFlow is running on large number of
processors and every processor participating in I/O may create a
bottleneck. ***Node level collective I/O is currently implemented for
2-D domain decomposition and variables Pressure and Saturation only. All
the other ParFlow NetCDF output Tcl flags should be set to false(default
value). CLM output is independently handled and not affected by this
key. Moreover on speciality architectures, this may not be a portable
feature. Users are advised to test this feature on their machine before
putting into production.***

string **NetCDF.NodeLevelIO** *False* This key sets flag for node level
collective I/O.

::

    pfset NetCDF.NodeLevelIO   True

NetCDF4 Initial Conditions: Pressure
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Analogues to ParFlow binary files, NetCDF4 based option can be used to
set the initial conditions for pressure to be read from an \`\`nc“ file
containing single time step of pressure. The name of the variable in
\`\`nc” file should be \`\`pressure". A sample NetCDF header of an
initial condition file looks as follows. The names of the dimensions are
not important. The order of dimensions is important e.g. *(time, lev,
lat, lon) or (time,z, y, x)*

::

    netcdf initial_condition {
    dimensions:
    	x = 200 ;
    	y = 200 ;
    	z = 40 ;
    	time = UNLIMITED ; // (1 currently)
    variables:
    	double time(time) ;
    	double pressure(time, z, y, x) ;
    }

***Node level collective I/O is currently not implemented for setting
initial conditions.***

string **ICPressure.Type** *no default* This key sets flag for initial
conditions to be read from a NetCDF file.

::

    pfset ICPressure.Type   NCFile
    pfset Geom.domain.ICPressure.FileName   "initial_condition.nc"

NetCDF4 Slopes
~~~~~~~~~~~~~~

NetCDF4 based option can be used slopes to be read from an \`\`nc“ file
containing single time step of slope values. The name of the variable in
\`\`nc” file should be \`\`slopex“ and \`\`slopey” A sample NetCDF
header of slope file looks as follows. The names of the dimensions are
not important. The order of dimensions is important e.g. *(time, lat,
lon) or (time, y, x)*

::

    netcdf slopex {
    dimensions:
    	time = UNLIMITED ; // (1 currently)
    	lon = 41 ;
    	lat = 41 ;
    variables:
      	double time(time) ;
    	double slopex(time, lat, lon) ;
    }
    netcdf slopey {
    dimensions:
    	time = UNLIMITED ; // (1 currently)
    	lon = 41 ;
    	lat = 41 ;
    variables:
    	double time(time) ;
    	double slopey(time, lat, lon) ;
    }

The two NetCDF files can be merged into one single file and can be used
with tcl flags. The variable names should be exactly as mentioned above.
Please refer to \`\`slopes.nc" under Little Washita test case. ***Node
level collective I/O is currently not implemented for setting initial
conditions.***

string **TopoSlopesX.Type** *no default* This key sets flag for slopes
in x direction to be read from a NetCDF file.

::

    pfset TopoSlopesX.Type   NCFile
    pfset TopoSlopesX.FileName   "slopex.nc"

string **TopoSlopesY.Type** *no default* This key sets flag for slopes
in y direction to be read from a NetCDF file.

::

    pfset TopoSlopesY.Type   NCFile
    pfset TopoSlopesy.FileName   "slopey.nc"

NetCDF4 Transient EvapTrans Forcing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Following keys can be used for NetCDF4 based transient evaptrans
forcing. The file should contain forcing for all time steps. For a given
time step, if the forcing is null, zero values could be filled for the
given time step in the \`\`.nc" file. The format of the sample file
looks as follows. The names of the dimensions are not important. The
order of dimensions is important e.g. *(time, lev, lat, lon) or (time,z,
y, x)*

::

    netcdf evap_trans {
    dimensions:
    	time = UNLIMITED ; // (1000 currently)
    	x = 72 ;
    	y = 72 ;
    	z = 3 ;
    variables:
    	double evaptrans(time, z, y, x) ;
    }

***Node level collective I/O is currently not implemented for transient
evaptrans forcing.***

string **NetCDF.EvapTransFileTransient** *False* This key sets flag for
transient evaptrans forcing to be read from a NetCDF file.

::

    pfset NetCDF.EvapTransFileTransient   True

string **NetCDF.EvapTrans.FileName** *no default* This key sets the name
of the NetCDF transient evaptrans forcing file.

::

    pfset NetCDF.EvapTrans.FileName         "evap_trans.nc"

NetCDF4 CLM Output
~~~~~~~~~~~~~~~~~~

Similar to ParFlow binary and silo, following keys can be used to write
output CLM variables in a single NetCDF file containing multiple time
steps.

integer **NetCDF.CLMNumStepsPerFile** *None* This key sets number of
time steps to be written to a single NetCDF file.

::

    pfset NetCDF.CLMNumStepsPerFile 24

string **NetCDF.WriteCLM** *False* This key sets CLM variables to be
written in a NetCDF file.

::

    pfset NetCDF.WriteCLM         True

The output variables are:

eflx\ :sub:`l`\ h\ :sub:`t`\ ot for latent heat flux total
:math:`[W/m^2]` using the silo variable *LatentHeat*;

eflx\ :sub:`l`\ wrad\ :sub:`o`\ ut for outgoing long-wave radiation
:math:`[W/m^2]` using the silo variable *LongWave*;

eflx\ :sub:`s`\ h\ :sub:`t`\ ot for sensible heat flux total
:math:`[W/m^2]` using the silo variable *SensibleHeat*;

eflx\ :sub:`s`\ oil\ :sub:`g`\ rnd for ground heat flux :math:`[W/m^2]`
using the silo variable *GroundHeat*;

qflx\ :sub:`e`\ vap\ :sub:`t`\ ot for total evaporation :math:`[mm/s]`
using the silo variable *EvaporationTotal*;

qflx\ :sub:`e`\ vap\ :sub:`g`\ rnd for ground evaporation without
condensation :math:`[mm/s]` using the silo variable
*EvaporationGroundNoSublimation*;

qflx\ :sub:`e`\ vap\ :sub:`s`\ oi for soil evaporation :math:`[mm/s]`
using the silo variable *EvaporationGround*;

qflx\ :sub:`e`\ vap\ :sub:`v`\ eg for vegetation evaporation
:math:`[mm/s]` using the silo variable *EvaporationCanopy*;

qflx\ :sub:`t`\ ran\ :sub:`v`\ eg for vegetation transpiration
:math:`[mm/s]` using the silo variable *Transpiration*;

qflx\ :sub:`i`\ nfl for soil infiltration :math:`[mm/s]` using the silo
variable *Infiltration*;

swe\ :sub:`o`\ ut for snow water equivalent :math:`[mm]` using the silo
variable *SWE*;

t\ :sub:`g`\ rnd for ground surface temperature :math:`[K]` using the
silo variable *TemperatureGround*; and

t\ :sub:`s`\ oil for soil temperature over all layers :math:`[K]` using
the silo variable *TemperatureSoil*.

NetCDF4 CLM Input/Forcing
~~~~~~~~~~~~~~~~~~~~~~~~~

| NetCDF based meteorological forcing can be used with following TCL
keys. It is built similar to 2D forcing case for CLM with parflow binary
files. All the required forcing variables must be present in one single
NetCDF file spanning entire length of simulation. If the simulation ends
before number of time steps in NetCDF forcing file, next cycle of
simulation can be restarted with same forcing file provided it covers
the time span of this cycle.
| e.g. If the NetCDF forcing file contains 100 time steps and simulation
CLM-ParFlow simulation runs for 10 cycles containing 10 time steps in
each cycle, the same forcing file can be reused. The user has to set
correct value for the key ``Solver.CLM.IstepStart``
| The format of input file looks as follows. The variable names should
match exactly as follows. The names of the dimensions are not important.
The order of dimensions is important e.g. *(time, lev, lat, lon) or
(time,z, y, x)*

::

    netcdf metForcing {
    dimensions:
    	lon = 41 ;
    	lat = 41 ;
    	time = UNLIMITED ; // (72 currently)
    variables:
    	double time(time) ;
    	double APCP(time, lat, lon) ;
    	double DLWR(time, lat, lon) ;
    	double DSWR(time, lat, lon) ;
    	double Press(time, lat, lon) ;
    	double SPFH(time, lat, lon) ;
    	double Temp(time, lat, lon) ;
    	double UGRD(time, lat, lon) ;
    	double VGRD(time, lat, lon) ;

***Note: While using NetCDF based CLM forcing, ``Solver.CLM.MetFileNT``
should be set to its default value of 1***

string **Solver.CLM.MetForcing** *no default* This key sets
meteorological forcing to be read from NetCDF file.

::

    pfset Solver.CLM.MetForcing     NC

Set the name of the input/forcing file as follows.

::

    pfset Solver.CLM.MetFileName   "metForcing.nc"

This file should be present in experiment directory. User may create
soft links in experiment directory in case where data can not be moved.

NetCDF Testing Little Washita Test Case
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The basic NetCDF functionality of output (pressure and saturation) and
initial conditions (pressure) can be tested with following tcl script.
CLM input/output functionality can also be tested with this case.

::

    parflow/test/washita/tcl_scripts/LW_NetCDF_Test.tcl

This test case will be initialized with following initial condition
file, slopes and meteorological forcing.

::

    parflow/test/washita/parflow_input/press.init.nc
    parflow/test/washita/parflow_input/slopes.nc
    parflow/test/washita/clm_input/metForcing.nc

ParFlow Binary Files (.pfb)
---------------------------

The .pfb file format is a binary file format which is used to store
ParFlow grid data. It is written as BIG ENDIAN binary bit ordering . The
format for the file is:

::

    <double : X>    <double : Y>    <double : Z>
    <integer : NX>  <integer : NY>  <integer : NZ>
    <double : DX>   <double : DY>   <double : DZ>

    <integer : num_subgrids>
    FOR subgrid = 0 TO <num_subgrids> - 1
    BEGIN
       <integer : ix>  <integer : iy>  <integer : iz>
       <integer : nx>  <integer : ny>  <integer : nz>
       <integer : rx>  <integer : ry>  <integer : rz>
       FOR k = iz TO iz + <nz> - 1
       BEGIN
          FOR j = iy TO iy + <ny> - 1
          BEGIN
             FOR i = ix TO ix + <nx> - 1
             BEGIN
                <double : data_ijk>
             END
          END
       END
    END

ParFlow CLM Single Output Binary Files (.c.pfb)
-----------------------------------------------

The .pfb file format is a binary file format which is used to store CLM
output data in a single file. It is written as BIG ENDIAN binary bit
ordering . The format for the file is:

::

    <double : X>    <double : Y>    <double : Z>
    <integer : NX>  <integer : NY>  <integer : NZ>
    <double : DX>   <double : DY>   <double : DZ>

    <integer : num_subgrids>
    FOR subgrid = 0 TO <num_subgrids> - 1
    BEGIN
       <integer : ix>  <integer : iy>  <integer : iz>
       <integer : nx>  <integer : ny>  <integer : nz>
       <integer : rx>  <integer : ry>  <integer : rz>
          FOR j = iy TO iy + <ny> - 1
          BEGIN
             FOR i = ix TO ix + <nx> - 1
             BEGIN
                eflx_lh_tot_ij
    	    eflx_lwrad_out_ij
    	    eflx_sh_tot_ij
    	    eflx_soil_grnd_ij
    	    qflx_evap_tot_ij
    	    qflx_evap_grnd_ij
    	    qflx_evap_soi_ij
    	    qflx_evap_veg_ij
    	    qflx_infl_ij
    	    swe_out_ij
    	    t_grnd_ij
         IF (clm_irr_type == 1)  qflx_qirr_ij 
    ELSE IF (clm_irr_type == 3)  qflx_qirr_inst_ij
    ELSE                         NULL
    	    FOR k = 1 TO clm_nz
    	    tsoil_ijk
    	    END
             END
          END
    END

ParFlow Scattered Binary Files (.pfsb)
--------------------------------------

The .pfsb file format is a binary file format which is used to store
ParFlow grid data. This format is used when the grid data is
“scattered”, that is, when most of the data is 0. For data of this type,
the .pfsb file format can reduce storage requirements considerably. The
format for the file is:

::

    <double : X>    <double : Y>    <double : Z>
    <integer : NX>  <integer : NY>  <integer : NZ>
    <double : DX>   <double : DY>   <double : DZ>

    <integer : num_subgrids>
    FOR subgrid = 0 TO <num_subgrids> - 1
    BEGIN
       <integer : ix>  <integer : iy>  <integer : iz>
       <integer : nx>  <integer : ny>  <integer : nz>
       <integer : rx>  <integer : ry>  <integer : rz>
       <integer : num_nonzero_data>
       FOR k = iz TO iz + <nz> - 1
       BEGIN
          FOR j = iy TO iy + <ny> - 1
          BEGIN
             FOR i = ix TO ix + <nx> - 1
             BEGIN
                IF (<data_ijk> > tolerance)
                BEGIN
                   <integer : i>  <integer : j>  <integer : k>
                   <double : data_ijk>
                END
             END
          END
       END
    END

ParFlow Solid Files (.pfsol)
----------------------------

The .pfsol file format is an ASCII file format which is used to define
3D solids. The solids are represented by closed triangulated surfaces,
and surface “patches” may be associated with each solid.

Note that unlike the user input files, the solid file cannot contain
comment lines.

The format for the file is:

::

    <integer : file_version_number>

    <integer : num_vertices>
    # Vertices
    FOR vertex = 0 TO <num_vertices> - 1
    BEGIN
       <real : x>  <real : y>  <real : z>
    END

    # Solids
    <integer : num_solids>
    FOR solid = 0 TO <num_solids> - 1
    BEGIN
       #Triangles
       <integer : num_triangles>
       FOR triangle = 0 TO <num_triangles> - 1
       BEGIN
          <integer : v0> <integer : v1> <integer : v2>
       END

       # Patches
       <integer : num_patches>
       FOR patch = 0 TO <num_patches> - 1
       BEGIN
          <integer : num_patch_triangles>
          FOR patch_triangle = 0 TO <num_patch_triangles> - 1
          BEGIN
             <integer : t>
          END
       END
    END

The field <file:sub:`v`\ ersion\ :sub:`n`\ umber> is used to make file
format changes more manageable. The field <num:sub:`v`\ ertices>
specifies the number of vertices to follow. The fields <x>, <y>, and <z>
define the coordinate of a triangle vertex. The field
<num:sub:`s`\ olids> specifies the number of solids to follow. The field
<num:sub:`t`\ riangles> specifies the number of triangles to follow. The
fields <v0>, <v1>, and <v2> are vertex indexes that specify the 3
vertices of a triangle. Note that the vertices for each triangle MUST be
specified in an order that makes the normal vector point outward from
the domain. The field <num:sub:`p`\ atches> specifies the number of
surface patches to follow. The field
num\ :sub:`p`\ atch\ :sub:`t`\ riangles specifies the number of
triangles indices to follow (these triangles make up the surface patch).
The field <t> is an index of a triangle on the solid solid.

ParFlow .pfsol files can be created from GMS .sol files using the
utility gmssol2pfsol located in the $PARFLOW\ :sub:`D`\ IR/bin
directory. This conversion routine takes any number of GMS .sol files,
concatenates the vertices of the solids defined in the files, throws
away duplicate vertices, then prints out the .pfsol file. Information
relating the solid index in the resulting .pfsol file with the GMS names
and material IDs are printed to stdout.

ParFlow Well Output File (.wells)
---------------------------------

A well output file is produced by ParFlow when wells are defined. The
well output file contains information about the well data being used in
the internal computations and accumulated statistics about the
functioning of the wells.

The header section has the following format:

::

    LINE
    BEGIN
       <real : BackgroundX>
       <real : BackgroundY>
       <real : BackgroundZ>
       <integer : BackgroundNX>
       <integer : BackgroundNY>
       <integer : BackgroundNZ>
       <real : BackgroundDX>
       <real : BackgroundDY>
       <real : BackgroundDZ>
    END

    LINE
    BEGIN
       <integer : number_of_phases>
       <integer : number_of_components>
       <integer : number_of_wells>
    END

    FOR well = 0 TO <number_of_wells> - 1
    BEGIN
       LINE
       BEGIN
          <integer : sequence_number>
       END

       LINE
       BEGIN
          <string : well_name>
       END

       LINE
       BEGIN
          <real : well_x_lower>
          <real : well_y_lower>
          <real : well_z_lower>
          <real : well_x_upper>
          <real : well_y_upper>
          <real : well_z_upper>
          <real : well_diameter>
       END

       LINE
       BEGIN
         <integer : well_type>
         <integer : well_action>
       END
    END

The data section has the following format:

::

    FOR time = 1 TO <number_of_time_intervals>
    BEGIN
       LINE
       BEGIN
          <real : time>
       END

       FOR well = 0 TO <number_of_wells> - 1
       BEGIN
          LINE
          BEGIN
             <integer : sequence_number>
          END

          LINE
          BEGIN
             <integer : SubgridIX>
             <integer : SubgridIY>
             <integer : SubgridIZ>
             <integer : SubgridNX>
             <integer : SubgridNY>
             <integer : SubgridNZ>
             <integer : SubgridRX>
             <integer : SubgridRY>
             <integer : SubgridRZ>
          END

          FOR well = 0 TO <number_of_wells> - 1
          BEGIN
             LINE
             BEGIN
                FOR phase = 0 TO <number_of_phases> - 1
                BEGIN
                   <real : phase_value>
                END
             END

             IF injection well
             BEGIN
                LINE
                BEGIN
                   FOR phase = 0 TO <number_of_phases> - 1
                   BEGIN
                      <real : saturation_value>
                   END
                END

                LINE
                BEGIN
                   FOR phase = 0 TO <number_of_phases> - 1
                   BEGIN
                      FOR component = 0 TO <number_of_components> - 1
                      BEGIN
                         <real : component_value>
                      END
                   END
                END
             END

             LINE
             BEGIN
                FOR phase = 0 TO <number_of_phases> - 1
                BEGIN
                   FOR component = 0 TO <number_of_components> - 1
                   BEGIN
                      <real : component_fraction>
                   END
                END
             END

             LINE
             BEGIN
                FOR phase = 0 TO <number_of_phases> - 1
                BEGIN
                   <real : phase_statistic>
                END
             END

             LINE
             BEGIN
                FOR phase = 0 TO <number_of_phases> - 1
                BEGIN
                   <real : saturation_statistic>
                END
             END

             LINE
             BEGIN
                FOR phase = 0 TO <number_of_phases> - 1
                BEGIN
                   FOR component = 0 TO <number_of_components> - 1
                   BEGIN
                      <real : component_statistic>
                   END
                END
             END

             LINE
             BEGIN
                FOR phase = 0 TO <number_of_phases> - 1
                BEGIN
                   FOR component = 0 TO <number_of_components> - 1
                   BEGIN
                      <real : concentration_data>
                   END
                END
             END
          END
       END
    END

ParFlow Simple ASCII and Simple Binary Files (.sa and .sb)
----------------------------------------------------------

The simple binary,.sa, file format is an ASCII file format which is used
by pftools to write out ParFlow grid data. The simple binary,.sb, file
format is exactly the same, just written as BIG ENDIAN binary bit
ordering . The format for the file is:

::

    <integer : NX>  <integer : NY>  <integer : NZ>

       FOR k = 0 TO  <nz> - 1
       BEGIN
          FOR j = 0 TO  <ny> - 1
          BEGIN
             FOR i = 0 TO  <nx> - 1
             BEGIN
                <double : data_ijk>
             END
          END
       END

GNU Free Documentation License
==============================

Version 1.3, 3 November 2008

Copyright © 2000, 2001, 2002, 2007, 2008 Free Software Foundation, Inc.

<http://fsf.org/>

Everyone is permitted to copy and distribute verbatim copies of this
license document, but changing it is not allowed.

**Preamble**

The purpose of this License is to make a manual, textbook, or other
functional and useful document “free” in the sense of freedom: to assure
everyone the effective freedom to copy and redistribute it, with or
without modifying it, either commercially or noncommercially.
Secondarily, this License preserves for the author and publisher a way
to get credit for their work, while not being considered responsible for
modifications made by others.

This License is a kind of “copyleft”, which means that derivative works
of the document must themselves be free in the same sense. It
complements the GNU General Public License, which is a copyleft license
designed for free software.

We have designed this License in order to use it for manuals for free
software, because free software needs free documentation: a free program
should come with manuals providing the same freedoms that the software
does. But this License is not limited to software manuals; it can be
used for any textual work, regardless of subject matter or whether it is
published as a printed book. We recommend this License principally for
works whose purpose is instruction or reference.

**1. APPLICABILITY AND DEFINITIONS**

This License applies to any manual or other work, in any medium, that
contains a notice placed by the copyright holder saying it can be
distributed under the terms of this License. Such a notice grants a
world-wide, royalty-free license, unlimited in duration, to use that
work under the conditions stated herein. The “**Document**”, below,
refers to any such manual or work. Any member of the public is a
licensee, and is addressed as “**you**”. You accept the license if you
copy, modify or distribute the work in a way requiring permission under
copyright law.

A “**Modified Version**” of the Document means any work containing the
Document or a portion of it, either copied verbatim, or with
modifications and/or translated into another language.

A “**Secondary Section**” is a named appendix or a front-matter section
of the Document that deals exclusively with the relationship of the
publishers or authors of the Document to the Document’s overall subject
(or to related matters) and contains nothing that could fall directly
within that overall subject. (Thus, if the Document is in part a
textbook of mathematics, a Secondary Section may not explain any
mathematics.) The relationship could be a matter of historical
connection with the subject or with related matters, or of legal,
commercial, philosophical, ethical or political position regarding them.

The “**Invariant Sections**” are certain Secondary Sections whose titles
are designated, as being those of Invariant Sections, in the notice that
says that the Document is released under this License. If a section does
not fit the above definition of Secondary then it is not allowed to be
designated as Invariant. The Document may contain zero Invariant
Sections. If the Document does not identify any Invariant Sections then
there are none.

The “**Cover Texts**” are certain short passages of text that are
listed, as Front-Cover Texts or Back-Cover Texts, in the notice that
says that the Document is released under this License. A Front-Cover
Text may be at most 5 words, and a Back-Cover Text may be at most 25
words.

A “**Transparent**” copy of the Document means a machine-readable copy,
represented in a format whose specification is available to the general
public, that is suitable for revising the document straightforwardly
with generic text editors or (for images composed of pixels) generic
paint programs or (for drawings) some widely available drawing editor,
and that is suitable for input to text formatters or for automatic
translation to a variety of formats suitable for input to text
formatters. A copy made in an otherwise Transparent file format whose
markup, or absence of markup, has been arranged to thwart or discourage
subsequent modification by readers is not Transparent. An image format
is not Transparent if used for any substantial amount of text. A copy
that is not “Transparent” is called “**Opaque**”.

Examples of suitable formats for Transparent copies include plain ASCII
without markup, Texinfo input format, LaTeX input format, SGML or XML
using a publicly available DTD, and standard-conforming simple HTML,
PostScript or PDF designed for human modification. Examples of
transparent image formats include PNG, XCF and JPG. Opaque formats
include proprietary formats that can be read and edited only by
proprietary word processors, SGML or XML for which the DTD and/or
processing tools are not generally available, and the machine-generated
HTML, PostScript or PDF produced by some word processors for output
purposes only.

The “**Title Page**” means, for a printed book, the title page itself,
plus such following pages as are needed to hold, legibly, the material
this License requires to appear in the title page. For works in formats
which do not have any title page as such, “Title Page” means the text
near the most prominent appearance of the work’s title, preceding the
beginning of the body of the text.

The “**publisher**” means any person or entity that distributes copies
of the Document to the public.

A section “**Entitled XYZ**” means a named subunit of the Document whose
title either is precisely XYZ or contains XYZ in parentheses following
text that translates XYZ in another language. (Here XYZ stands for a
specific section name mentioned below, such as “**Acknowledgements**”,
“**Dedications**”, “**Endorsements**”, or “**History**”.) To “**Preserve
the Title**” of such a section when you modify the Document means that
it remains a section “Entitled XYZ” according to this definition.

The Document may include Warranty Disclaimers next to the notice which
states that this License applies to the Document. These Warranty
Disclaimers are considered to be included by reference in this License,
but only as regards disclaiming warranties: any other implication that
these Warranty Disclaimers may have is void and has no effect on the
meaning of this License.

**2. VERBATIM COPYING**

You may copy and distribute the Document in any medium, either
commercially or noncommercially, provided that this License, the
copyright notices, and the license notice saying this License applies to
the Document are reproduced in all copies, and that you add no other
conditions whatsoever to those of this License. You may not use
technical measures to obstruct or control the reading or further copying
of the copies you make or distribute. However, you may accept
compensation in exchange for copies. If you distribute a large enough
number of copies you must also follow the conditions in section 3.

You may also lend copies, under the same conditions stated above, and
you may publicly display copies.

**3. COPYING IN QUANTITY**

If you publish printed copies (or copies in media that commonly have
printed covers) of the Document, numbering more than 100, and the
Document’s license notice requires Cover Texts, you must enclose the
copies in covers that carry, clearly and legibly, all these Cover Texts:
Front-Cover Texts on the front cover, and Back-Cover Texts on the back
cover. Both covers must also clearly and legibly identify you as the
publisher of these copies. The front cover must present the full title
with all words of the title equally prominent and visible. You may add
other material on the covers in addition. Copying with changes limited
to the covers, as long as they preserve the title of the Document and
satisfy these conditions, can be treated as verbatim copying in other
respects.

If the required texts for either cover are too voluminous to fit
legibly, you should put the first ones listed (as many as fit
reasonably) on the actual cover, and continue the rest onto adjacent
pages.

If you publish or distribute Opaque copies of the Document numbering
more than 100, you must either include a machine-readable Transparent
copy along with each Opaque copy, or state in or with each Opaque copy a
computer-network location from which the general network-using public
has access to download using public-standard network protocols a
complete Transparent copy of the Document, free of added material. If
you use the latter option, you must take reasonably prudent steps, when
you begin distribution of Opaque copies in quantity, to ensure that this
Transparent copy will remain thus accessible at the stated location
until at least one year after the last time you distribute an Opaque
copy (directly or through your agents or retailers) of that edition to
the public.

It is requested, but not required, that you contact the authors of the
Document well before redistributing any large number of copies, to give
them a chance to provide you with an updated version of the Document.

**4. MODIFICATIONS**

You may copy and distribute a Modified Version of the Document under the
conditions of sections 2 and 3 above, provided that you release the
Modified Version under precisely this License, with the Modified Version
filling the role of the Document, thus licensing distribution and
modification of the Modified Version to whoever possesses a copy of it.
In addition, you must do these things in the Modified Version:

-  Use in the Title Page (and on the covers, if any) a title distinct
   from that of the Document, and from those of previous versions (which
   should, if there were any, be listed in the History section of the
   Document). You may use the same title as a previous version if the
   original publisher of that version gives permission.

-  List on the Title Page, as authors, one or more persons or entities
   responsible for authorship of the modifications in the Modified
   Version, together with at least five of the principal authors of the
   Document (all of its principal authors, if it has fewer than five),
   unless they release you from this requirement.

-  State on the Title page the name of the publisher of the Modified
   Version, as the publisher.

-  Preserve all the copyright notices of the Document.

-  Add an appropriate copyright notice for your modifications adjacent
   to the other copyright notices.

-  Include, immediately after the copyright notices, a license notice
   giving the public permission to use the Modified Version under the
   terms of this License, in the form shown in the Addendum below.

-  Preserve in that license notice the full lists of Invariant Sections
   and required Cover Texts given in the Document’s license notice.

-  Include an unaltered copy of this License.

-  Preserve the section Entitled “History”, Preserve its Title, and add
   to it an item stating at least the title, year, new authors, and
   publisher of the Modified Version as given on the Title Page. If
   there is no section Entitled “History” in the Document, create one
   stating the title, year, authors, and publisher of the Document as
   given on its Title Page, then add an item describing the Modified
   Version as stated in the previous sentence.

-  Preserve the network location, if any, given in the Document for
   public access to a Transparent copy of the Document, and likewise the
   network locations given in the Document for previous versions it was
   based on. These may be placed in the “History” section. You may omit
   a network location for a work that was published at least four years
   before the Document itself, or if the original publisher of the
   version it refers to gives permission.

-  For any section Entitled “Acknowledgements” or “Dedications”,
   Preserve the Title of the section, and preserve in the section all
   the substance and tone of each of the contributor acknowledgements
   and/or dedications given therein.

-  Preserve all the Invariant Sections of the Document, unaltered in
   their text and in their titles. Section numbers or the equivalent are
   not considered part of the section titles.

-  Delete any section Entitled “Endorsements”. Such a section may not be
   included in the Modified Version.

-  Do not retitle any existing section to be Entitled “Endorsements” or
   to conflict in title with any Invariant Section.

-  Preserve any Warranty Disclaimers.

If the Modified Version includes new front-matter sections or appendices
that qualify as Secondary Sections and contain no material copied from
the Document, you may at your option designate some or all of these
sections as invariant. To do this, add their titles to the list of
Invariant Sections in the Modified Version’s license notice. These
titles must be distinct from any other section titles.

You may add a section Entitled “Endorsements”, provided it contains
nothing but endorsements of your Modified Version by various parties—for
example, statements of peer review or that the text has been approved by
an organization as the authoritative definition of a standard.

You may add a passage of up to five words as a Front-Cover Text, and a
passage of up to 25 words as a Back-Cover Text, to the end of the list
of Cover Texts in the Modified Version. Only one passage of Front-Cover
Text and one of Back-Cover Text may be added by (or through arrangements
made by) any one entity. If the Document already includes a cover text
for the same cover, previously added by you or by arrangement made by
the same entity you are acting on behalf of, you may not add another;
but you may replace the old one, on explicit permission from the
previous publisher that added the old one.

The author(s) and publisher(s) of the Document do not by this License
give permission to use their names for publicity for or to assert or
imply endorsement of any Modified Version.

**5. COMBINING DOCUMENTS**

You may combine the Document with other documents released under this
License, under the terms defined in section 4 above for modified
versions, provided that you include in the combination all of the
Invariant Sections of all of the original documents, unmodified, and
list them all as Invariant Sections of your combined work in its license
notice, and that you preserve all their Warranty Disclaimers.

The combined work need only contain one copy of this License, and
multiple identical Invariant Sections may be replaced with a single
copy. If there are multiple Invariant Sections with the same name but
different contents, make the title of each such section unique by adding
at the end of it, in parentheses, the name of the original author or
publisher of that section if known, or else a unique number. Make the
same adjustment to the section titles in the list of Invariant Sections
in the license notice of the combined work.

In the combination, you must combine any sections Entitled “History” in
the various original documents, forming one section Entitled “History”;
likewise combine any sections Entitled “Acknowledgements”, and any
sections Entitled “Dedications”. You must delete all sections Entitled
“Endorsements”.

**6. COLLECTIONS OF DOCUMENTS**

You may make a collection consisting of the Document and other documents
released under this License, and replace the individual copies of this
License in the various documents with a single copy that is included in
the collection, provided that you follow the rules of this License for
verbatim copying of each of the documents in all other respects.

You may extract a single document from such a collection, and distribute
it individually under this License, provided you insert a copy of this
License into the extracted document, and follow this License in all
other respects regarding verbatim copying of that document.

**7. AGGREGATION WITH INDEPENDENT WORKS**

A compilation of the Document or its derivatives with other separate and
independent documents or works, in or on a volume of a storage or
distribution medium, is called an “aggregate” if the copyright resulting
from the compilation is not used to limit the legal rights of the
compilation’s users beyond what the individual works permit. When the
Document is included in an aggregate, this License does not apply to the
other works in the aggregate which are not themselves derivative works
of the Document.

If the Cover Text requirement of section 3 is applicable to these copies
of the Document, then if the Document is less than one half of the
entire aggregate, the Document’s Cover Texts may be placed on covers
that bracket the Document within the aggregate, or the electronic
equivalent of covers if the Document is in electronic form. Otherwise
they must appear on printed covers that bracket the whole aggregate.

**8. TRANSLATION**

Translation is considered a kind of modification, so you may distribute
translations of the Document under the terms of section 4. Replacing
Invariant Sections with translations requires special permission from
their copyright holders, but you may include translations of some or all
Invariant Sections in addition to the original versions of these
Invariant Sections. You may include a translation of this License, and
all the license notices in the Document, and any Warranty Disclaimers,
provided that you also include the original English version of this
License and the original versions of those notices and disclaimers. In
case of a disagreement between the translation and the original version
of this License or a notice or disclaimer, the original version will
prevail.

If a section in the Document is Entitled “Acknowledgements”,
“Dedications”, or “History”, the requirement (section 4) to Preserve its
Title (section 1) will typically require changing the actual title.

**9. TERMINATION**

You may not copy, modify, sublicense, or distribute the Document except
as expressly provided under this License. Any attempt otherwise to copy,
modify, sublicense, or distribute it is void, and will automatically
terminate your rights under this License.

However, if you cease all violation of this License, then your license
from a particular copyright holder is reinstated (a) provisionally,
unless and until the copyright holder explicitly and finally terminates
your license, and (b) permanently, if the copyright holder fails to
notify you of the violation by some reasonable means prior to 60 days
after the cessation.

Moreover, your license from a particular copyright holder is reinstated
permanently if the copyright holder notifies you of the violation by
some reasonable means, this is the first time you have received notice
of violation of this License (for any work) from that copyright holder,
and you cure the violation prior to 30 days after your receipt of the
notice.

Termination of your rights under this section does not terminate the
licenses of parties who have received copies or rights from you under
this License. If your rights have been terminated and not permanently
reinstated, receipt of a copy of some or all of the same material does
not give you any rights to use it.

**10. FUTURE REVISIONS OF THIS LICENSE**

The Free Software Foundation may publish new, revised versions of the
GNU Free Documentation License from time to time. Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns. See http://www.gnu.org/copyleft/.

Each version of the License is given a distinguishing version number. If
the Document specifies that a particular numbered version of this
License “or any later version” applies to it, you have the option of
following the terms and conditions either of that specified version or
of any later version that has been published (not as a draft) by the
Free Software Foundation. If the Document does not specify a version
number of this License, you may choose any version ever published (not
as a draft) by the Free Software Foundation. If the Document specifies
that a proxy can decide which future versions of this License can be
used, that proxy’s public statement of acceptance of a version
permanently authorizes you to choose that version for the Document.

**11. RELICENSING**

“Massive Multiauthor Collaboration Site” (or “MMC Site”) means any World
Wide Web server that publishes copyrightable works and also provides
prominent facilities for anybody to edit those works. A public wiki that
anybody can edit is an example of such a server. A “Massive Multiauthor
Collaboration” (or “MMC”) contained in the site means any set of
copyrightable works thus published on the MMC site.

“CC-BY-SA” means the Creative Commons Attribution-Share Alike 3.0
license published by Creative Commons Corporation, a not-for-profit
corporation with a principal place of business in San Francisco,
California, as well as future copyleft versions of that license
published by that same organization.

“Incorporate” means to publish or republish a Document, in whole or in
part, as part of another Document.

An MMC is “eligible for relicensing” if it is licensed under this
License, and if all works that were first published under this License
somewhere other than this MMC, and subsequently incorporated in whole or
in part into the MMC, (1) had no cover texts or invariant sections, and
(2) were thus incorporated prior to November 1, 2008.

The operator of an MMC Site may republish an MMC contained in the site
under CC-BY-SA on the same site at any time before August 1, 2009,
provided the MMC is eligible for relicensing.

999

Ajami, H., McCabe, M.F., Evans, J.P. and Stisen, S. (2014). Assessing
the impact of model spin-up on surface water-groundwater interactions
using an integrated hydrologic model. *Water Resources Research* **50**
2636:2656, doi:10.1002/2013WR014258.

Ajami,H., M.F. McCabe, and J.P. Evans (2015). Impacts of model
initialization on an integrated surface water–groundwater model. ,
29(17):3790–3801.

Ashby, S.F. and Falgout,R. D. (1996). A parallel multigrid
preconditioned conjugate gradient algorithm for groundwater flow
simulations. , **124**:145–159.

Atchley, A. and Maxwell, R.M. (2011). Influences of subsurface
heterogeneity and vegetation cover on soil moisture, surface
temperature, and evapotranspiration at hillslope scales. *Hydrogeology
Journal* doi:10.1007/s10040-010-0690-1.

Atchley, A.L., Maxwell, R.M. and Navarre-Sitchler, A.K. (2013). Human
Health Risk Assessment of CO 2 Leakage into Overlying Aquifers Using a
Stochastic, Geochemical Reactive Transport Approach. *Environmental
Science and Technology* **47** 5954–5962, doi:10.1021/es400316c.

Atchley, A.L., Maxwell, R.M. and Navarre-Sitchler, A.K. (2013). Using
streamlines to simulate stochastic reactive transport in heterogeneous
aquifers: Kinetic metal release and transport in CO2 impacted drinking
water aquifers. *Advances in Water Resources* **52** 93–106,
doi:10.1016/j.advwatres.2012.09.005.

Bhaskar,A.S., C. Welty, R.M. Maxwell, and A.J. Miller (2015). Untangling
the effects of urban development on subsurface storage in baltimore. ,
51(2):1158–1181.

Barnes,M. C. Welty, and A. Miller (2015). Global topographic slope
enforcement to ensure connectivity and drainage in an urban terrain. ,
0(0):06015017.

Bearup,L.A. R.M. Maxwell and J.E. McCray (2016). Hillslope response to
insect-induced land-cover change: an integrated model of end-member
mixing. , ECO-15-0202.R1.

Beisman,J.J., R. M. Maxwell, A. K. Navarre-Sitchler, C. I. Steefel, and
S. Molins (2015). Parcrunchflow: an efficient, parallel reactive
transport simulation tool for physically and chemically heterogeneous
saturated subsurface environments. , 19(2):403–422.

Bürger, C.M., Kollet, S., Schumacher, J. and Bosel, D. (2012).
Introduction of a web service for cloud computing with the integrated
hydrologic simulation platform ParFlow. *Computers and Geosciences*
**48** 334–336, doi:10.1016/j.cageo.2012.01.007.

Condon, L.E. and Maxwell, R.M. (2013). Implementation of a linear
optimization water allocation algorithm into a fully integrated physical
hydrology model. *Advances in Water Resources* **60** 135–147,
doi:10.1016/j.advwatres.2013.07.012.

Condon, L.E., Maxwell, R.M. and Gangopadhyay, S. (2013). The impact of
subsurface conceptualization on land energy fluxes. *Advances in Water
Resources* **60** 188–203, doi:10.1016/j.advwatres.2013.08.001.

Condon, L.E. and Maxwell, R.M. (2014). Feedbacks between managed
irrigation and water availability: Diagnosing temporal and spatial
patterns using an integrated hydrologic model. *Water Resources
Research* **50** 2600–2616, doi:10.1002/2013WR014868.

Condon, L.E. and Maxwell, R.M. (2014). Groundwater-fed irrigation
impacts spatially distributed temporal scaling behavior of the natural
system: a spatio-temporal framework for understanding water management
impacts. *Environmental Research Letters* **9** 1–9,
doi:10.1088/1748-9326/9/3/034009.

Condon, L.E., A. S. Hering, and R. M. Maxwell (2015). Quantitative
assessment of groundwater controls across major {US} river basins using
a multi-model regression algorithm. , 82:106 – 123.

Condon, L.E., and R. M. Maxwell (2015). Evaluating the relationship
between topography and groundwater using outputs from a
continental-scale integrated hydrology model. , 51(8):6602–6621.

Cui, Z., Welty, C. and Maxwell, R.M. (2014). Modeling nitrogen transport
and transformation in aquifers using a particle-tracking approach.
*Computers and Geosciences* **70** 1–14,
doi:10.1016/j.cageo.2014.05.005.

Dai, Y., X. Zeng, R.E. Dickinson, I. Baker, G.B. Bonan, M.G. Bosilovich,
A.S. Denning, P.A. Dirmeyer, P.R., G. Niu, K.W. Oleson, C.A. Schlosser
and Z.L. Yang (2003). The common land model. *The Bulletin of the
American Meteorological Society* **84**(8):1013–1023.

Daniels, M.H., Maxwell, R.M., Chow, F.K. (2010). An algorithm for flow
direction enforcement using subgrid-scale stream location data, *Journal
of Hydrologic Engineering* **16** 677–683,
doi:10.1061/(ASCE)HE.1943-5584.0000340.

de Barros, F.P.J., Rubin, Y. and Maxwell, R.M. (2009). The concept of
comparative information yield curves and their application to risk-based
site characterization. *Water Resources Research* 45, W06401,
doi:10.1029/2008WR007324.

de Rooij, R., Graham, W. and Maxwell, R.M. (2013). A particle-tracking
scheme for simulating pathlines in coupled surface-subsurface flows.
*Advances in Water Resources* **52** 7–18,
doi:10.1016/j.advwatres.2012.07.022.

Eisenstat, S.C. and Walker, H.F. (1996). Choosing the forcing terms in
an inexact newton method. , **17**(1):16–32.

Nicholas B. Engdahl and Reed M. Maxwell (2015). Quantifying changes in
age distributions and the hydrologic balance of a high-mountain
watershed from climate induced variations in recharge. , 522:152 – 162.

Zhufeng Fang, Heye Bogena, Stefan Kollet, Julian Koch, and Harry
Vereecken (2015). Spatio-temporal validation of long-term 3d
hydrological simulations of a forested catchment using empirical
orthogonal functions and wavelet coherence analysis. , 529, Part 3:1754
– 1767.

Ferguson, I.M. and Maxwell, R.M. (2010). Role of groundwater in
watershed response and land surface feedbacks under climate change.
*Water Resources Research* 46, W00F02, doi:10.1029/2009WR008616.

Ferguson, I.M. and Maxwell, R.M. (2011). Hydrologic and land:energy
feedbacks of agricultural water management practices. *Environmental
Research Letters* **6** 1–7, doi:10.1088/1748-9326/6/1/014006.

Ferguson, I.M. and Maxwell, R.M. (2012). Human impacts on terrestrial
hydrology: climate change versus pumping and irrigation. *Environmental
Research Letters* **7** 1–8, doi:10.1088/1748-9326/7/4/044022.

Forsyth, P.A., Wu, Y.S. and Pruess, K. (1995). Robust Numerical Methods
for Saturated-Unsaturated Flow with Dry Initial Conditions. ,
**17**:25–38.

Frei, S., Fleckenstein, J.H., Kollet, S.J. and Maxwell, R.M. (2009).
Patterns and dynamics of river-aquifer exchange with variably-saturated
flow using a fully-coupled model. *Journal of Hydrology* 375(3-4),
383–393, doi:10.1016/j.jhydrol.2009.06.038.

Haverkamp, R. and Vauclin, M. (1981). A comparative study of three forms
of the Richard equation used for predicting one-dimensional infiltration
in unsaturated soil. , **45**:13–20.

Jennifer L. Jefferson and Reed M. Maxwell (2015). Evaluation of simple
to complex parameterizations of bare ground evaporation. ,
7(3):1075–1092.

Jennifer L. Jefferson, James M. Gilbert, Paul G. Constantine, and
Reed M. Maxwell (2015). Active subspaces for sensitivity analysis and
dimension reduction of an integrated hydrologic model. , 83:127 – 138.

Jones, J.E. and Woodward, C.S. (2001). Newton-krylov-multigrid solvers
for large-scale, highly heterogeneous, variably saturated flow problems.
, **24**:763–774.

Keyes, D.E., McInnes, L.C., Woodward, C., Gropp, W., Myra, E., Pernice,
M., Bell, J., Brown, J., Clo, A., Connors, J., Constantinescu, E.,
Estep, D., Evans, K., Farhat, C., Hakim, A., Hammond, G., Hansen, G.,
Hill, J., Isaac, T., et al. (2013). Multiphysics simulations: Challenges
and opportunities. *International Journal of High Performance Computing
Applications* **27** 4–83, doi:10.1177/1094342012468181.

J. Koch, T. Cornelissen, Z. Fang, H. Bogen, B. H. Diekkrüger, S. Kollet,
and S. Stisen (2016). Inter-comparison of three distributed hydrological
models with respect to seasonal variability of soil moisture patterns at
a small forested catchment. , (533):234–246.

Kollat, J.B., Reed, P.M. and Maxwell, R.M. (2011). Many-objective
groundwater monitoring network design using bias-aware ensemble Kalman
filtering, evolutionary optimization, and visual analytics. *Water
Resources Research*,doi:10.1029/2010WR009194.

Kollet, S.J. (2009). Influence of soil heterogeneity on
evapotranspiration under shallow water table conditions: transient,
stochastic simulations. *Environmental Research Letters* **4** 1–9,
doi:10.1088/1748-9326/4/3/035007.

Kollet, S.J., Cvijanovic, I., Sch"uttemeyer, D., Maxwell, R.M., Moene,
A.F. and Bayer P (2009). The influence of rain sensible heat, subsurface
heat convection and the lower temperature boundary condition on the
energy balance at the land surface. *Vadose Zone Journal*,
doi:10.2136/vzj2009.0005.

Kollet, S. J. and Maxwell, R. M. (2006). Integrated surface-groundwater
flow modeling: A free-surface overland flow boundary condition in a
parallel groundwater flow model. *Advances in Water Resources*,
**29**:945–958 .

Kollet, S.J. and Maxwell, R.M. (2008). Capturing the influence of
groundwater dynamics on land surface processes using an integrated,
distributed watershed model, *Water Resources Research*,**44**: W02402.

Kollet, S.J. and Maxwell, R.M. (2008). Demonstrating fractal scaling of
baseflow residence time distributions using a fully-coupled groundwater
and land surface model. *Geophysical Research Letters*, **35**, L07402.

Kollet, S.J., Maxwell, R.M., Woodward, C.S., Smith, S.G., Vanderborght,
J., Vereecken, H., and Simmer, C. (2010). Proof-of-concept of regional
scale hydrologic simulations at hydrologic resolution utilizing
massively parallel computer resources. *Water Resources Research*, 46,
W04201, doi:10.1029/2009WR008730.

S.J. Kollet (2015). Optimality and inference in hydrology from entropy
production considerations: synthetic hillslope numerical experiments. ,
(12):5123–5149.

Major, E., Benson, D.A., Revielle, J., Ibrahim, H., Dean, A., Maxwell,
R.M., Poeter, E. and Dogan, M. (2011). Comparison of Fickian and
temporally nonlocal transport theories over many scales in an
exhaustively sampled sandstone slab. *Water Resources Research* **47**
1-14, doi:10.1029/2011WR010857.

Maxwell, R.M. (2010). Infiltration in arid environments: Spatial
patterns between subsurface heterogeneity and water-energy balances,
*Vadose Zone Journal* 9, 970–983, doi:10.2136/vzj2010.0014.

Maxwell, R.M. (2013). A terrain-following grid transform and
preconditioner for parallel, large-scale, integrated hydrologic
modeling. *Advances in Water Resources* **53** 109–117,
doi:10.1016/j.advwatres.2012.10.001.

Maxwell, R.M., Carle, S.F and Tompson, A.F.B. (2000). Risk-Based
Management of Contaminated Groundwater: The Role of Geologic
Heterogeneity, Exposure and Cancer Risk in Determining the Performance
of Aquifer Remediation, In *Proceedings of Computational Methods in
Water Resources XII*, Balkema, 533–539.

Maxwell, R.M., Carle, S.F. and Tompson, A.F.B. (2008). Contamination,
risk, and heterogeneity: on the effectiveness of aquifer remediation.
*Environmental Geology*, **54**:1771–1786.

Maxwell, R.M., Chow, F.K. and Kollet, S.J. (2007). The
groundwater-land-surface-atmosphere connection: soil moisture effects on
the atmospheric boundary layer in fully-coupled simulations. *Advances
in Water Resources*, **30**(12):2447–2466 .

Maxwell,R.M., L. E. Condon, and S. J. Kollet (2015). A high-resolution
simulation of groundwater and surface water over most of the continental
us with the integrated hydrologic model parflow v3. , 8(3):923–937.

Maxwell,R.M., L. E. Condon, S. J. Kollet, K. Maher, R. Haggerty, and
M. M. Forrester (2016). The imprint of climate and geology on the
residence times of groundwater. , 43(2):701–708. 2015GL066916.

Maxwell, R.M. and Kollet, S.J. (2008). Quantifying the effects of
three-dimensional subsurface heterogeneity on Hortonian runoff processes
using a coupled numerical, stochastic approach. *Advances in Water
Resources* **31**(5): 807–817.

Maxwell, R.M. and Kollet, S.J. (2008) Interdependence of groundwater
dynamics and land-energy feedbacks under climate change. *Nature
Geoscience* **1**(10): 665–669.

Maxwell, R.M., Lundquist, J.K., Mirocha, J.D., Smith, S.G., Woodward,
C.S. and Tompson, A.F.B. (2011). Development of a coupled
groundwater-atmospheric model. *Monthly Weather Review*
doi:10.1175/2010MWR3392.

Maxwell, R.M. and Miller, N.L. (2005). Development of a coupled land
surface and groundwater model. *Journal of Hydrometeorology*,
**6**(3):233–247.

Maxwell, R.M., Putti, M., Meyerhoff, S., Delfs, J.-O., Ferguson, I.M.,
Ivanov, V., Kim, J., Kolditz, O., Kollet, S.J., Kumar, M., Lopez, S.,
Niu, J., Paniconi, C., Park, Y.-J., Phanikumar, M.S., Shen, C., Sudicky,
E. a. and Sulis, M. (2014). Surface-subsurface model intercomparison: A
first set of benchmark results to diagnose integrated hydrology and
feedbacks. *Water Resources Research* **50** 1531:1549,
doi:10.1002/2013WR013725.

Maxwell, R.M., Tompson, A.F.B. and Kollet, S.J. (2009) A serendipitous,
long-term infiltration experiment: Water and tritium circulation beneath
the CAMBRIC trench at the Nevada Test Site. *Journal of Contaminant
Hydrology* 108(1-2) 12-28, doi:10.1016/j.jconhyd.2009.05.002.

Maxwell, R.M., Welty, C. and Harvey, R.W. (2007). Revisiting the Cape
Cod Bacteria Injection Experiment Using a Stochastic Modeling Approach,
*Environmental Science and Technology*, **41**(15):5548–5558.

Maxwell, R.M., Welty,C. and Tompson, A.F.B. (2003). Streamline-based
simulation of virus transport resulting from long term artificial
recharge in a heterogeneous aquifer *Advances in Water Resources*,
**22**(3):203–221.

Meyerhoff, S.B. and Maxwell, R.M. (2011). Quantifying the effects of
subsurface heterogeneity on hillslope runoff using a stochastic
approach. *Hydrogeology Journal* **19** 1515:1530,
doi:10.1007/s10040-011-0753-y.

Meyerhoff, S.B., Maxwell, R.M., Graham, W.D. and Williams, J.L. (2014).
Improved hydrograph prediction through subsurface characterization:
conditional stochastic hillslope simulations. *Hydrogeology Journal*
doi:10.1007/s10040-014-1112-6.

Meyerhoff, S.B., Maxwell, R.M., Revil, A., Martin, J.B., Karaoulis, M.
and Graham, W.D. (2014). Characterization of groundwater and surface
water mixing in a semiconfined karst aquifer using time-lapse electrical
resistivity tomography. *Water Resources Research* **50** 2566:2585,
doi:10.1002/2013WR013991.

Mikkelson, K.M., Maxwell, R.M., Ferguson, I., Stednick, J.D., McCray,
J.E. and Sharp, J.O. (2013). Mountain pine beetle infestation impacts:
modeling water and energy budgets at the hill-slope scale.
*Ecohydrology* **6** doi:10.1002/eco.278.

Rahman, M., M. Sulis, and S.J. Kollet (2015). Evaluating the
dual-boundary forcing concept in subsurface-land surface interactions of
the hydrological cycle. .

Rahman,M. M. Sulis, and S.J. Kollet (2015). The subsurface-land
surface-atmosphere connection under convective conditions. ,
(83):240–249.

Rihani, J., Maxwell, R.M., Chow, F.K. (2010). Coupling groundwater and
land-surface processes: Idealized simulations to identify effects of
terrain and subsurface heterogeneity on land surface energy fluxes.
*Water Resources Research* 46, W12523, doi:10.1029/2010WR009111.

Reyes,R., R.M. Maxwell, and T. S. Hogue (2015). Impact of lateral flow
and spatial scaling on the simulation of semi-arid urban land surfaces
in an integrated hydrologic and land surface model. .

Rihani, J.F., F. K. Chow, and R. M. Maxwell (2015). Isolating effects of
terrain and soil moisture heterogeneity on the atmospheric boundary
layer: Idealized simulations to diagnose land-atmosphere feedbacks. ,
7(2):915–937.

Seck,A. C. Welty, and R. M. Maxwell (2015). Spin-up behavior and effects
of initial conditions for an integrated hydrologic model. ,
51(4):2188–2210.

Shrestha, P., Sulis, M., Masbou, M., Kollet, S. and Simmer, C. (2014). A
scale-consistent Terrestrial Systems Modeling Platform based on COSMO,
CLM and ParFlow. *Monthly Weather Review* doi:10.1175/MWR-D-14-00029.1.

Shrestha,P., M. Sulis, C. Simmer, and S. Kollet (2015). Impacts of grid
resolution on surface energy fluxes simulated with an integrated
surface-groundwater flow model. , 19:4317–4326.

Siirila, E.R., Navarre-Sitchler, A.K., Maxwell, R.M. and McCray, J.E.
(2012). A quantitative methodology to assess the risks to human health
from CO2 leakage into groundwater. *Advances in Water Resources*,
**36**, 146-164, doi:10.1016/j.advwatres.2010.11.005.

Siirila, E.R. and Maxwell, R.M. (2012). A new perspective on human
health risk assessment: Development of a time dependent methodology and
the effect of varying exposure durations. *Science of The Total
Environment* **431** 221-232, doi:10.1016/j.scitotenv.2012.05.030.

Siirila, E.R. and Maxwell, R.M. (2012). Evaluating effective reaction
rates of kinetically driven solutes in large-scale, statistically
anisotropic media: Human health risk implications. *Water Resources
Research* **48** 1-23, doi:10.1029/2011WR011516.

Srivastava,V., W. Graham, R. Munoz-Carpena, and R. M. Maxwell (2014).
Insights on geologic and vegetative controls over hydrologic behavior of
a large complex basin–global sensitivity analysis of an integrated
parallel hydrologic model. , 519, Part B:2238 – 2257.

Sulis, M., Meyerhoff, S., Paniconi, C., Maxwell, R.M., Putti, M. and
Kollet, S.J. (2010). A comparison of two physics-based numerical models
for simulating surface water-groundwater interactions. *Advances in
Water Resources*, 33(4), 456-467, doi:10.1016/j.advwatres.2010.01.010.

Tompson, A.F.B., Ababou, R. and Gelhar, L.W. (1989). Implementation of
of the three-dimensional turning bands random field generator. ,
**25**(10):2227–2243.

Tompson, A.F.B., Falgout, R.D., Smith, S.G., Bosl, W.J. and Ashby, S.F.
(1998). Analysis of subsurface contaminant migration and remediation
using high performance computing. *Advances in Water Resources*,
**22**(3):203–221.

Tompson, A. F. B., Bruton, C. J. and Pawloski, G. A. eds. (1999b).
*Evaluation of the hydrologic source term from underground nuclear tests
in Frenchman Flat at the Nevada Test Site: The CAMBRIC test*, Lawrence
Livermore National Laboratory, Livermore, CA (UCRL-ID-132300), 360pp.

Tompson, A.F.B., Carle, S.F., Rosenberg, N.D. and Maxwell, R.M. (1999).
Analysis of groundwater migration from artificial recharge in a large
urban aquifer: A simulation perspective, *Water Resources Research*,
**35**(10):2981–2998.

Tompson AFB., Bruton, C.J., Pawloski, G.A., Smith, D.K., Bourcier, W.L.,
Shumaker, D.E., Kersting, A.B., Carle, S.F. and Maxwell, R.M. (2002). On
the evaluation of groundwater contamination from underground nuclear
tests. *Environmental Geology*, **42**(2-3):235–247.

Tompson, A. F. B., Maxwell, R. M., Carle, S. F., Zavarin, M., Pawloski,
G. A. and Shumaker, D. E. (2005). *Evaluation of the Non-Transient
Hydrologic Source Term from the CAMBRIC Underground Nuclear Test in
Frenchman Flat, Nevada Test Site*, Lawrence Livermore National
Laboratory, Livermore, CA, UCRL-TR-217191.

van Genuchten, M.Th.(1980). A closed form equation for predicting the
hydraulic conductivity of unsaturated soils. , **44**:892–898.

Welch, B. (1995) . Prentice Hall.

Woodward, C.S. (1998), A Newton-Krylov-Multigrid solver for variably
saturated flow problems. In *Proceedings of the XIIth International
Conference on Computational Methods in Water Resources*, June.

Woodward, C.S., Grant, K.E., and Maxwell, R.M. (2002). Applications of
Sensitivity Analysis to Uncertainty Quantification for Variably
Saturated Flow. In *Proceedings of the XIVth International Conference on
Computational Methods in Water Resources, Amsterdam*, The Netherlands,
June.

Williams, J.L. and Maxwell, R.M. (2011). Propagating Subsurface
Uncertainty to the Atmosphere Using Fully Coupled Stochastic
Simulations. *Journal of Hydrometeorology* **12** 690-701,
doi:10.1175/2011JHM1363.1.

Williams, J.L., Maxwell, R.M. and Monache, L.D. (2013). Development and
verification of a new wind speed forecasting system using an ensemble
Kalman filter data assimilation technique in a fully coupled hydrologic
and atmospheric model. *Journal of Advances in Modeling Earth Systems*
**5** 785-800, doi:10.1002/jame.20051.

*Endianness*, Wikipedia Entry: http://en.wikipedia.org/wiki/Endianness

[end]

.. [1]
   We use *foo* and *bar* just as placeholders for whatever directory
   you wish you use, see also http://en.wikipedia.org/wiki/Foobar

.. [2]
   http://www.igbp.net

.. |image| image:: IGWMC-logo-new.pdf
