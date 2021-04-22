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

*type* **KeyName** [default value] Description

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

| *integer* **FileVersion** [no default] 
| This gives the value of the input file version number that this file fits.

.. code-block:: c

  int FileVersion [no default]


.. code-block:: c

  int FileVersion(char* var)


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

*integer* **Process.Topology.P** [no default] This assigns the process
splits in the *x* direction.

::

    pfset Process.Topology.P        2

*integer* **Process.Topology.Q** [no default] This assigns the process
splits in the *y* direction.

::

    pfset Process.Topology.Q        1

*integer* **Process.Topology.P** [no default] This assigns the process
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

*double* **ComputationalGrid.Lower.X** [no default] This assigns the
lower *x* coordinate location for the computational grid.

::

    pfset   ComputationalGrid.Lower.X  0.0

*double* **ComputationalGrid.Lower.Y** [no default] This assigns the
lower *y* coordinate location for the computational grid.

::

    pfset   ComputationalGrid.Lower.Y  0.0

*double* **ComputationalGrid.Lower.Z** [no default] This assigns the
lower *z* coordinate location for the computational grid.

::

    pfset   ComputationalGrid.Lower.Z  0.0

*integer* **ComputationalGrid.NX** [no default] This assigns the number
of grid cells in the *x* direction for the computational grid.

::

    pfset  ComputationalGrid.NX  10 

*integer* **ComputationalGrid.NY** [no default] This assigns the number
of grid cells in the *y* direction for the computational grid.

::

    pfset  ComputationalGrid.NY  10 

*integer* **ComputationalGrid.NZ** [no default] This assigns the number
of grid cells in the *z* direction for the computational grid.

::

    pfset  ComputationalGrid.NZ  10 

*real* **ComputationalGrid.DX** [no default] This defines the size of
grid cells in the *x* direction. Units are *L* and are defined by the
units of the hydraulic conductivity used in the problem.

::

    pfset  ComputationalGrid.DX  10.0 

*real* **ComputationalGrid.DY** [no default] This defines the size of
grid cells in the *y* direction. Units are *L* and are defined by the
units of the hydraulic conductivity used in the problem.

::

    pfset  ComputationalGrid.DY  10.0 

*real* **ComputationalGrid.DZ** [no default] This defines the size of
grid cells in the *z* direction. Units are *L* and are defined by the
units of the hydraulic conductivity used in the problem.

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

*list* **GeomInput.Names** [no default] This is a list of the geometry
input names which define the containers for all of the geometries
defined for this problem.

::

    pfset GeomInput.Names    "solidinput indinput boxinput"

*string* **GeomInput.*geom\_input\_name*.InputType** [no default] This
defines the input type for the geometry input with *geom\_input\_name*.
This key must be one of: **SolidFile, IndicatorField**, **Box**.

::

    pfset GeomInput.solidinput.InputType  SolidFile

*list* **GeomInput.*geom\_input\_name*.GeomNames** [no default] This is
a list of the names of the geometries defined by the geometry input. For
a geometry input type of Box, the list should contain a single geometry
name. For the SolidFile geometry type this should contain a list with
the same number of gemetries as were defined using GMS. The order of
geometries in the SolidFile should match the names. For IndicatorField
types you need to specify the value in the input field which matches the
name using GeomInput.*geom\_input\_name*.Value.

::

    pfset GeomInput.solidinput.GeomNames "domain bottomlayer \
                                          middlelayer toplayer"

*string* **GeomInput.*geom\_input\_name*.Filename** [no default] For
IndicatorField and SolidFile geometry inputs this key specifies the
input filename which contains the field or solid information.

::

    pfset GeomInput.solidinput.FileName   ocwd.pfsol

*integer* **GeomInput.*geometry\_input\_name*.Value** [no default] For
IndicatorField geometry inputs you need to specify the mapping between
values in the input file and the geometry names. The named geometry will
be defined whereever the input file is equal to the specifed value.

::

    pfset GeomInput.sourceregion.Value   11

For box geometries you need to specify the location of the box. This is
done by defining two corners of the the box.

*double* **Geom.*box\_geom\_name*.Lower.X** [no default] This gives the
lower X real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Lower.X   -1.0

*double* **Geom.*box\_geom\_name*.Lower.Y** [no default] This gives the
lower Y real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Lower.Y   -1.0

*double* **Geom.*box\_geom\_name*.Lower.Z** [no default] This gives the
lower Z real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Lower.Z   -1.0

*double* **Geom.*box\_geom\_name*.Upper.X** [no default] This gives the
upper X real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Upper.X   151.0

*double* **Geom.*box\_geom\_name*.Upper.Y** [no default] This gives the
upper Y real space coordinate value of the previously specified box
geometry of name *box\_geom\_name*.

::

    pfset Geom.background.Upper.Y   171.0

*double* **Geom.*box\_geom\_name*.Upper.Z** [no default] This gives the
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

*integer* **TimingInfo.StartCount** [no default] This key is used to
indicate the time step number that will be associated with the first
advection cycle in a transient problem. The value **-1** indicates that
advection is not to be done. The value **0** indicates that advection
should begin with the given initial conditions. Values greater than
**0** are intended to mean “restart” from some previous “checkpoint”
time-step, but this has not yet been implemented.

::

    pfset TimingInfo.StartCount    0

*double* **TimingInfo.StartTime** [no default] This key is used to
indicate the starting time for the simulation.

::

    pfset TimingInfo.StartTime     0.0

*double* **TimingInfo.StopTime** [no default] This key is used to
indicate the stopping time for the simulation.

::

    pfset TimingInfo.StopTime      100.0

*double* **TimingInfo.DumpInterval** [no default] This key is the real
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

*list* **TimeStep.Type** [no default] This key must be one of:
**Constant** or **Growth**. The value **Constant** defines a constant
time step. The value **Growth** defines a time step that starts as
:math:`dt_0` and is defined for other steps as
:math:`dt^{new} = \gamma dt^{old}` such that :math:`dt^{new} \leq 
dt_{max}` and :math:`dt^{new} \geq dt_{min}`.

::

    pfset TimeStep.Type      Constant

*double* **TimeStep.Value** [no default] This key is used only if a
constant time step is selected and indicates the value of the time step
for all steps taken.

::

    pfset TimeStep.Value      0.001

*double* **TimeStep.InitialStep** [no default] This key specifies the
initial time step :math:`dt_0` if the **Growth** type time step is
selected.

::

    pfset TimeStep.InitialStep    0.001

*double* **TimeStep.GrowthFactor** [no default] This key specifies the
growth factor :math:`\gamma` by which a time step will be multiplied to
get the new time step when the **Growth** type time step is selected.

::

    pfset TimeStep.GrowthFactor      1.5

*double* **TimeStep.MaxStep** [no default] This key specifies the
maximum time step allowed, :math:`dt_{max}`, when the **Growth** type
time step is selected.

::

    pfset TimeStep.MaxStep      86400

*double* **TimeStep.MinStep** [no default] This key specifies the
minimum time step allowed, :math:`dt_{min}`, when the **Growth** type
time step is selected.

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

*list* **CycleNames** [no default] This key is used to specify the named
time cycles to be used in a simulation. It is a list of names and each
name defines a time cycle and the number of items determines the total
number of time cycles specified. Each named cycle is described using a
number of keys defined below.

::

    pfset Cycle.Names constant onoff

*list* **Cycle.*cycle\_name*.Names** [no default] This key is used to
specify the named time intervals for each cycle. It is a list of names
and each name defines a time interval when a specific boundary condition
is applied and the number of items determines the total number of
intervals in that time cycle.

::

    pfset Cycle.onoff.Names "on off"

*integer* **Cycle.*cycle\_name.interval\_name*.Length** [no default]
This key is used to specify the length of a named time intervals. It is
an *integer multiplier* of the value set for the **TimingInfo.BaseUnit**
key described above. The total length of a given time cycle is the sum
of all the intervals multiplied by the base unit.

::

    pfset Cycle.onoff.on.Length             10

*integer* **Cycle.*cycle\_name*.Repeat** [no default] This key is used
to specify the how many times a named time interval repeats. A positive
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

*string* **Domain.GeomName** [no default] This key specifies which of
the named geometries is the problem domain.

::

    pfset Domain.GeomName    domain

Phases and Contaminants
~~~~~~~~~~~~~~~~~~~~~~~

*list* **Phase.Names** [no default] This specifies the names of phases
to be modeled. Currently only 1 or 2 phases may be modeled.

::

    pfset Phase.Names    "water"

*list* **Contaminant.Names** [no default] This specifies the names of
contaminants to be advected.

::

    pfset Contaminants.Names   "tce"

Gravity, Phase Density and Phase Viscosity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*double* **Gravity** [no default] Specifies the gravity constant to be
used.

::

    pfset Gravity	1.0

*string* **Phase.*phase\_name*.Density.Type** [no default] This key
specifies whether density will be a constant value or if it will be
given by an equation of state of the form :math:`(rd)exp(cP)`, where
:math:`P` is pressure, :math:`rd` is the density at atmospheric
pressure, and :math:`c` is the phase compressibility constant. This key
must be either **Constant** or **EquationOfState**.

::

    pfset Phase.water.Density.Type	 Constant

*double* **Phase.*phase\_name*.Density.Value** [no default] This
specifies the value of density if this phase was specified to have a
constant density value for the phase *phase\_name*.

::

    pfset Phase.water.Density.Value   1.0

*double* **Phase.*phase\_name*.Density.ReferenceDensity** [no default]
This key specifies the reference density if an equation of state density
function is specified for the phase *phase\_name*.

::

    pfset Phase.water.Density.ReferenceDensity   1.0

This key specifies the phase compressibility constant if an equation of
state density function is specified for the phase *phase\|-name*.

::

    pfset Phase.water.Density.CompressibilityConstant   1.0

*string* **Phase.*phase\_name*.Viscosity.Type** [Constant] This key
specifies whether viscosity will be a constant value. Currently, the
only choice for this key is **Constant**.

::

    pfset Phase.water.Viscosity.Type   Constant

*double* **Phase.*phase\_name*.Viscosity.Value** [no default] This
specifies the value of viscosity if this phase was specified to have a
constant viscosity value.

::

    pfset Phase.water.Viscosity.Value   1.0

Chemical Reactions
~~~~~~~~~~~~~~~~~~

*double* **Contaminants.*contaminant\_name*.Degradation.Value** [no
default] This key specifies the half-life decay rate of the named
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

*string* **Perm.Conditioning.FileName** [“NA”] This key specifies the
name of the file that contains the conditioning data. The default string
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

*list* **Geom.Perm.Names** [no default] This key specifies all of the
geometries to which a permeability field will be assigned. These
geometries must cover the entire computational domain.

::

    pfset GeomInput.Names   "background domain concen_region"

*string* **Geom.geometry\_name.Perm.Type** [no default] This key
specifies which method is to be used to assign permeability data to the
named geometry, *geometry\_name*. It must be either **Constant**,
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

*double* **Geom.*geometry\_name*.Perm.Value** [no default] This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Geom.domain.Perm.Value   1.0

*double* **Geom.*geometry\_name*.Perm.LambdaX** [no default] This key
specifies the x correlation length, :math:`\lambda_x`, of the field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.LambdaX   200.0

*double* **Geom.*geometry\_name*.Perm.LambdaY** [no default] This key
specifies the y correlation length, :math:`\lambda_y`, of the field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.LambdaY   200.0

*double* **Geom.*geometry\_name*.Perm.LambdaZ** [no default] This key
specifies the z correlation length, :math:`\lambda_z`, of the field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.LambdaZ   10.0

*double* **Geom.*geometry\_name*.Perm.GeomMean** [no default] This key
specifies the geometric mean, :math:`\mu`, of the log normal field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.GeomMean   4.56

*double* **Geom.*geometry\_name*.Perm.Sigma** [no default] This key
specifies the standard deviation, :math:`\sigma`, of the normal field
generated for the named geometry, *geometry\_name*, if either the
Turning Bands or Parallel Gaussian Simulator are chosen.

::

    pfset Geom.domain.Perm.Sigma   2.08

*integer* **Geom.*geometry\_name*.Perm.Seed** [1] This key specifies the
initial seed for the random number generator used to generate the field
for the named geometry, *geometry\_name*, if either the Turning Bands or
Parallel Gaussian Simulator are chosen. This number must be positive.

::

    pfset Geom.domain.Perm.Seed   1

*integer* **Geom.*geometry\_name*.Perm.NumLines** [100] This key
specifies the number of lines to be used in the Turning Bands algorithm
for the named geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.NumLines   100

*double* **Geom.*geometry\_name*.Perm.RZeta** [5.0] This key specifies
the resolution of the line processes, in terms of the minimum grid
spacing, to be used in the Turning Bands algorithm for the named
geometry, *geometry\_name*. Large values imply high resolution.

::

    pfset Geom.domain.Perm.RZeta   5.0

*double* **Geom.*geometry\_name*.Perm.KMax** [100.0] This key specifies
the the maximum normalized frequency, :math:`K_{\rm max}`, to be used in
the Turning Bands algorithm for the named geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.KMax   100.0

*double* **Geom.*geometry\_name*.Perm.DelK** [0.2] This key specifies
the normalized frequency increment, :math:`\delta K`, to be used in the
Turning Bands algorithm for the named geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.DelK   0.2

*integer* **Geom.*geometry\_name*.Perm.MaxNPts** [no default] This key
sets limits on the number of simulated points in the search neighborhood
to be used in the Parallel Gaussian Simulator for the named geometry,
*geometry\_name*.

::

    pfset Geom.domain.Perm.MaxNPts   5

*integer* **Geom.*geometry\_name*.Perm.MaxCpts** [no default] This key
sets limits on the number of external conditioning points in the search
neighborhood to be used in the Parallel Gaussian Simulator for the named
geometry, *geometry\_name*.

::

    pfset Geom.domain.Perm.MaxCpts   200

*string* **Geom.*geometry\_name*.Perm.LogNormal** [“LogTruncated”] The
key specifies when a normal, log normal, truncated normal or truncated
log normal field is to be generated by the method for the named
geometry, *geometry\_name*. This value must be one of **Normal**,
**Log**, **NormalTruncated** or **LogTruncate** and can be used with
either Turning Bands or the Parallel Gaussian Simulator.

::

    pfset Geom.domain.Perm.LogNormal   "LogTruncated"

*string* **Geom.*geometry\_name*.Perm.StratType** [“Bottom”] This key
specifies the stratification of the permeability field generated by the
method for the named geometry, *geometry\_name*. The value must be one
of **Horizontal**, **Bottom** or **Top** and can be used with either the
Turning Bands or the Parallel Gaussian Simulator.

::

    pfset Geom.domain.Perm.StratType  "Bottom"

*double* **Geom.*geometry\_name*.Perm.LowCutoff** [no default] This key
specifies the low cutoff value for truncating the generated field for
the named geometry, *geometry\_name*, when either the NormalTruncated or
LogTruncated values are chosen.

::

    pfset Geom.domain.Perm.LowCutoff   0.0

*double* **Geom.*geometry\_name*.Perm.HighCutoff** [no default] This key
specifies the high cutoff value for truncating the generated field for
the named geometry, *geometry\_name*, when either the NormalTruncated or
LogTruncated values are chosen.

::

    pfset Geom.domain.Perm.HighCutoff   100.0

*string* **Geom.*geometry\_name*.Perm.FileName** [no default] This key
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

*string* **Perm.TensorType** [no default] This key specifies whether the
permeability tensor entries :math:`k_x, k_y` and :math:`k_z` will be
specified as three constants within a set of regions covering the domain
or whether the entries will be specified cell-wise by files. The choices
for this key are **TensorByGeom** and **TensorByFile**.

::

    pfset Perm.TensorType     TensorByGeom

*string* **Geom.Perm.TensorByGeom.Names** [no default] This key
specifies all of the geometries to which permeability tensor entries
will be assigned. These geometries must cover the entire computational
domain.

::

    pfset Geom.Perm.TensorByGeom.Names   "background domain" 

*double* **Geom.*geometry\_name*.Perm.TensorValX** [no default] This key
specifies the value of :math:`k_x` for the geometry given by
*geometry\_name*.

::

    pfset Geom.domain.Perm.TensorValX   1.0

*double* **Geom.*geometry\_name*.Perm.TensorValY** [no default] This key
specifies the value of :math:`k_y` for the geometry given by
*geom\_name*.

::

    pfset Geom.domain.Perm.TensorValY   1.0

*double* **Geom.*geometry\_name*.Perm.TensorValZ** [no default] This key
specifies the value of :math:`k_z` for the geometry given by
*geom\_name*.

::

    pfset Geom.domain.Perm.TensorValZ   1.0

*string* **Geom.*geometry\_name*.Perm.TensorFileX** [no default] This
key specifies that :math:`k_x` values for the specified geometry,
*geometry\_name*, are given according to a user-supplied description in
the “ParFlow Binary” file whose filename is given as the value. The only
choice for the value of *geometry\_name* is “domain”.

::

    pfset Geom.domain.Perm.TensorByFileX   "perm_x.pfb"

*string* **Geom.*geometry\_name*.Perm.TensorFileY** [no default] This
key specifies that :math:`k_y` values for the specified geometry,
*geometry\_name*, are given according to a user-supplied description in
the “ParFlow Binary” file whose filename is given as the value. The only
choice for the value of *geometry\_name* is “domain”.

::

    pfset Geom.domain.Perm.TensorByFileY   "perm_y.pfb"

*string* **Geom.*geometry\_name*.Perm.TensorFileZ** [no default] This
key specifies that :math:`k_z` values for the specified geometry,
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

*list* **Geom.Porosity.GeomNames** [no default] This key specifies all
of the geometries on which a porosity will be assigned. These geometries
must cover the entire computational domain.

::

    pfset Geom.Porosity.GeomNames   "background"

*string* **Geom.*geometry\_name*.Porosity.Type** [no default] This key
specifies which method is to be used to assign porosity data to the
named geometry, *geometry\_name*. The only choice currently available is
**Constant** which indicates that a constant is to be assigned to all
grid cells within a geometry.

::

    pfset Geom.background.Porosity.Type   Constant

*double* **Geom.*geometry\_name*.Porosity.Value** [no default] This key
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

*list* **Specific Storage.GeomNames** [no default] This key specifies
all of the geometries on which a different specific storage value will
be assigned. These geometries must cover the entire computational
domain.

::

    pfset SpecificStorage.GeomNames       "domain"

*string* **SpecificStorage.Type** [no default] This key specifies which
method is to be used to assign specific storage data. The only choice
currently available is **Constant** which indicates that a constant is
to be assigned to all grid cells within a geometry.

::

    pfset SpecificStorage.Type            Constant

*double* **Geom.*geometry\_name*.SpecificStorage.Value** [no default]
This key specifies the value assigned to all points in the named
geometry, *geometry\_name*, if the type was set to constant.

::

    pfset Geom.domain.SpecificStorage.Value 1.0e-4

dZMultipliers
~~~~~~~~~~~~~

Here, dZ multipliers (:math:`\delta Z * m`) values are assigned within
geounits (specified in § [Geometries] above) using one of the methods
described below.

The format for this section of input is:

*string* ** Solver.Nonlinear.VariableDz** [False] This key specifies
whether dZ multipliers are to be used, the default is False. The default
indicates a false or non-active variable dz and each layer thickness is
1.0 [L].

::

    pfset Solver.Nonlinear.VariableDz     True

*list* **dzScale.GeomNames** [no default] This key specifies which
problem domain is being applied a variable dz subsurface. These
geometries must cover the entire computational domain.

::

    pfset dzScale.GeomNames domain

*string* **dzScale.Type** [no default] This key specifies which method
is to be used to assign variable vertical grid spacing. The choices
currently available are **Constant** which indicates that a constant is
to be assigned to all grid cells within a geometry, **nzList** which
assigns all layers of a given model to a list value, and **PFBFile**
which reads in values from a distributed pfb file.

::

    pfset dzScale.Type            Constant

*list* **Specific dzScale.GeomNames** [no default] This key specifies
all of the geometries on which a different dz scaling value will be
assigned. These geometries must cover the entire computational domain.

::

    pfset dzScale.GeomNames       "domain"

*double* **Geom.*geometry\_name*.dzScale.Value** [no default] This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Geom.domain.dzScale.Value 1.0

*string* **Geom.*geometry\_name*.dzScale.FileName** [no default] This
key specifies file to be read in for variable dz values for the given
geometry, *geometry\_name*, if the type was set to **PFBFile**.

::

    pfset Geom.domain.dzScale.FileName  vardz.pfb

*integer* **dzScale.nzListNumber** [no default] This key indicates the
number of layers with variable dz in the subsurface. This value is the
same as the *ComputationalGrid.NZ* key.

::

    pfset dzScale.nzListNumber  10

*double* **Cell.*nzListNumber*.dzScale.Value** [no default] This key
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

*list* **Mannings.GeomNames** [no default] This key specifies all of the
geometries on which a different Mannings roughness value will be
assigned. Mannings values may be assigned by **PFBFile** or as
**Constant** by geometry. These geometries must cover the entire upper
surface of the computational domain.

::

    pfset Mannings.GeomNames       "domain"

*string* **Mannings.Type** [no default] This key specifies which method
is to be used to assign Mannings roughness data. The choices currently
available are **Constant** which indicates that a constant is to be
assigned to all grid cells within a geometry and **PFBFile** which
indicates that all values are read in from a distributed, grid-based
ParFlow binary file.

::

    pfset Mannings.Type "Constant"

*double* **Mannings.Geom.*geometry\_name*.Value** [no default] This key
specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset Mannings.Geom.domain.Value 5.52e-6

*double* **Mannings.FileName** [no default] This key specifies the value
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

*list* **ToposlopesX.GeomNames** [no default] This key specifies all of
the geometries on which a different :math:`x` topographic slope values
will be assigned. Topographic slopes may be assigned by **PFBFile** or
as **Constant** by geometry. These geometries must cover the entire
upper surface of the computational domain.

::

    pfset ToposlopesX.GeomNames       "domain"

*list* **ToposlopesY.GeomNames** [no default] This key specifies all of
the geometries on which a different :math:`y` topographic slope values
will be assigned. Topographic slopes may be assigned by **PFBFile** or
as **Constant** by geometry. These geometries must cover the entire
upper surface of the computational domain.

::

    pfset ToposlopesY.GeomNames       "domain"

*string* **ToposlopesX.Type** [no default] This key specifies which
method is to be used to assign topographic slopes. The choices currently
available are **Constant** which indicates that a constant is to be
assigned to all grid cells within a geometry and **PFBFile** which
indicates that all values are read in from a distributed, grid-based
ParFlow binary file.

::

    pfset ToposlopesX.Type "Constant"

*double* **ToposlopeX.Geom.*geometry\_name*.Value** [no default] This
key specifies the value assigned to all points in the named geometry,
*geometry\_name*, if the type was set to constant.

::

    pfset ToposlopeX.Geom.domain.Value 0.001

*double* **ToposlopesX.FileName** [no default] This key specifies the
value assigned to all points be read in from a ParFlow binary file.

::

    pfset TopoSlopesX.FileName lw.1km.slope_x.pfb

*double* **ToposlopesY.FileName** [no default] This key specifies the
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

*list* **Geom.Retardation.GeomNames** [no default] This key specifies
all of the geometries to which the contaminants will have a retardation
function applied.

::

    pfset GeomInput.Names   "background"

*string* **Geom.*geometry\_name*.*contaminant\_name*.Retardation.Type**
[no default] This key specifies which function is to be used to compute
the retardation for the named contaminant, *contaminant\_name*, in the
named geometry, *geometry\_name*. The only choice currently available is
**Linear** which indicates that a simple linear retardation function is
to be used to compute the retardation.

::

    pfset Geom.background.tce.Retardation.Type   Linear

*double* **Geom.*geometry\_name*.*contaminant\_name*.Retardation.Value**
[no default] This key specifies the distribution coefficient for the
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

*string* **Phase.*phase\_name*.Mobility.Type** [no default] This key
specifies whether the mobility for *phase\_name* will be a given
constant or a polynomial of the form, :math:`(S - S_0)^{a}`, where
:math:`S` is saturation, :math:`S_0` is irreducible saturation, and
:math:`a` is some exponent. The possibilities for this key are
**Constant** and **Polynomial**.

::

    pfset Phase.water.Mobility.Type   Constant

*double* **Phase.*phase\_name*.Mobility.Value** [no default] This key
specifies the constant mobility value for phase *phase\_name*.

::

    pfset Phase.water.Mobility.Value   1.0

*double* **Phase.*phase\_name*.Mobility.Exponent** [2.0] This key
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

*string* **Phase.RelPerm.Type** [no default] This key specifies the type
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

*list* **Phase.RelPerm.GeomNames** [no default] This key specifies the
geometries on which relative permeability will be given. The union of
these geometries must cover the entire computational domain.

::

    pfset Phase.RelPerm.Geonames   domain

*double* **Geom.*geom\_name*.RelPerm.Value** [no default] This key
specifies the constant relative permeability value on the specified
geometry.

::

    pfset Geom.domain.RelPerm.Value    0.5

*integer* **Phase.RelPerm.VanGenuchten.File** [0] This key specifies
whether soil parameters for the VanGenuchten function are specified in a
pfb file or by region. The options are either 0 for specification by
region, or 1 for specification in a file. Note that either all
parameters are specified in files (each has their own input file) or
none are specified by files. Parameters specified by files are:
:math:`\alpha` and N.

::

    pfset Phase.RelPerm.VanGenuchten.File   1

*string* **Geom.*geom\_name*.RelPerm.Alpha.Filename** [no default] This
key specifies a pfb filename containing the alpha parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.RelPerm.Alpha.Filename   alphas.pfb

*string* **Geom.*geom\_name*.RelPerm.N.Filename** [no default] This key
specifies a pfb filename containing the N parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.RelPerm.N.Filename   Ns.pfb

*double* **Geom.*geom\_name*.RelPerm.Alpha** [no default] This key
specifies the :math:`\alpha` parameter for the Van Genuchten function
specified on *geom\_name*.

::

    pfset Geom.domain.RelPerm.Alpha  0.005

*double* **Geom.*geom\_name*.RelPerm.N** [no default] This key specifies
the :math:`N` parameter for the Van Genuchten function specified on
*geom\_name*.

::

    pfset Geom.domain.RelPerm.N   2.0

*int* **Geom.*geom\_name*.RelPerm.NumSamplePoints** [0] This key
specifies the number of sample points for a spline base interpolation
table for the Van Genuchten function specified on *geom\_name*. If this
number is 0 (the default) then the function is evaluated directly. Using
the interpolation table is faster but is less accurate.

::

    pfset Geom.domain.RelPerm.NumSamplePoints  20000

This key specifies the lower value for a spline base interpolation table
for the Van Genuchten function specified on *geom\_name*. The upper
value of the range is 0. This value is used only when the table lookup
method is used (*NumSamplePoints* is greater than 0).

::

    pfset Geom.domain.RelPerm.MinPressureHead -300

*double* **Geom.*geom\_name*.RelPerm.A** [no default] This key specifies
the :math:`A` parameter for the Haverkamp relative permeability on
*geom\_name*.

::

    pfset Geom.domain.RelPerm.A  1.0

*double* **Geom.*geom\_name*.RelPerm.Gamma** [no default] This key
specifies the the :math:`\gamma` parameter for the Haverkamp relative
permeability on *geom\_name*.

::

    pfset Geom.domain.RelPerm.Gamma  1.0

*integer* **Geom.*geom\_name*.RelPerm.Degree** [no default] This key
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

*list* **PhaseSources.*phase\_name*.GeomNames** [no default] This key
specifies the names of the geometries on which source terms will be
specified. This is used only for **Constant** type phase sources.
Regions listed later “overlay” regions listed earlier.

::

    pfset PhaseSources.water.GeomNames   "bottomlayer middlelayer toplayer"

This key specifies the value of a constant source term applied to phase
*phase \_name* on geometry *geom\_name*.

::

    pfset PhaseSources.water.Geom.toplayer.Value   1.0

*string* **PhaseSources.*phase\_name*.PredefinedFunction** [no default]
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

*string* **CapPressure.*phase\_name*.Type** [“Constant”] This key
specifies the capillary pressure between phase :math:`0` and the named
phase, *phase\_name*. The only choice available is **Constant** which
indicates that a constant capillary pressure exists between the phases.

::

    pfset CapPressure.water.Type   Constant

*list* **CapPressure.*phase\_name*.GeomNames** [no default] This key
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

*string* **Phase.Saturation.Type** [no default] This key specifies the
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

*list* **Phase.Saturation.GeomNames** [no default] This key specifies
the geometries on which saturation will be given. The union of these
geometries must cover the entire computational domain.

::

    pfset Phase.Saturation.Geonames   domain

*double* **Geom.*geom\_name*.Saturation.Value** [no default] This key
specifies the constant saturation value on the *geom\_name* region.

::

    pfset Geom.domain.Saturation.Value    0.5

*integer* **Phase.Saturation.VanGenuchten.File** [0] This key specifies
whether soil parameters for the VanGenuchten function are specified in a
pfb file or by region. The options are either 0 for specification by
region, or 1 for specification in a file. Note that either all
parameters are specified in files (each has their own input file) or
none are specified by files. Parameters specified by files are
:math:`\alpha`, N, SRes, and SSat.

::

    pfset Phase.Saturation.VanGenuchten.File   1

*string* **Geom.*geom\_name*.Saturation.Alpha.Filename** [no default]
This key specifies a pfb filename containing the alpha parameters for
the VanGenuchten function cell-by-cell. The ONLY option for *geom\_name*
is “domain”.

::

    pfset Geom.domain.Saturation.Filename   alphas.pfb

*string* **Geom.*geom\_name*.Saturation.N.Filename** [no default] This
key specifies a pfb filename containing the N parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.Saturation.N.Filename   Ns.pfb

*string* **Geom.*geom\_name*.Saturation.SRes.Filename** [no default]
This key specifies a pfb filename containing the SRes parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.Saturation.SRes.Filename   SRess.pfb

*string* **Geom.*geom\_name*.Saturation.SSat.Filename** [no default]
This key specifies a pfb filename containing the SSat parameters for the
VanGenuchten function cell-by-cell. The ONLY option for *geom\_name* is
“domain”.

::

    pfset Geom.domain.Saturation.SSat.Filename   SSats.pfb

*double* **Geom.*geom\_name*.Saturation.Alpha** [no default] This key
specifies the :math:`\alpha` parameter for the Van Genuchten function
specified on *geom\_name*.

::

    pfset Geom.domain.Saturation.Alpha  0.005

*double* **Geom.*geom\_name*.Saturation.N** [no default] This key
specifies the :math:`N` parameter for the Van Genuchten function
specified on *geom\_name*.

::

    pfset Geom.domain.Saturation.N   2.0

Note that if both a Van Genuchten saturation and relative permeability
are specified, then the soil parameters should be the same for each in
order to have a consistent problem.

*double* **Geom.*geom\_name*.Saturation.SRes** [no default] This key
specifies the residual saturation on *geom\_name*.

::

    pfset Geom.domain.Saturation.SRes   0.0

*double* **Geom.*geom\_name*.Saturation.SSat** [no default] This key
specifies the saturation at saturated conditions on *geom\_name*.

::

    pfset Geom.domain.Saturation.SSat   1.0

*double* **Geom.*geom\_name*.Saturation.A** [no default] This key
specifies the :math:`A` parameter for the Haverkamp saturation on
*geom\_name*.

::

    pfset Geom.domain.Saturation.A   1.0

*double* **Geom.*geom\_name*.Saturation.Gamma** [no default] This key
specifies the the :math:`\gamma` parameter for the Haverkamp saturation
on *geom\_name*.

::

    pfset Geom.domain.Saturation.Gamma   1.0

*integer* **Geom.*geom\_name*.Saturation.Degree** [no default] This key
specifies the degree of the polynomial for the Polynomial saturation
given on *geom\_name*.

::

    pfset Geom.domain.Saturation.Degree   1

This key specifies the *coeff\_number*th coefficient of the Polynomial
saturation given on *geom\_name*.

::

    pfset Geom.domain.Saturation.Coeff.0   0.5
    pfset Geom.domain.Saturation.Coeff.1   1.0

*string* **Geom.*geom\_name*.Saturation.FileName** [no default] This key
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

*double* **InternalBC.*internal\_bc\_name*.X** [no default] This key
specifies the x-coordinate, :math:`{\rm x}`, of the named,
*internal\_bc\_name*, condition.

::

    pfset InternalBC.fixedheadvalue.X   40.0

*double* **InternalBC.*internal\_bc\_name*.Y** [no default] This key
specifies the y-coordinate, :math:`{\rm y}`, of the named,
*internal\_bc\_name*, condition.

::

    pfset InternalBC.fixedheadvalue.Y   65.2

*double* **InternalBC.*internal\_bc\_name*.Z** [no default] This key
specifies the z-coordinate, :math:`{\rm z}`, of the named,
*internal\_bc\_name*, condition.

::

    pfset InternalBC.fixedheadvalue.Z   12.1

*double* **InternalBC.*internal\_bc\_name*.Value** [no default] This key
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

*list* **BCPressure.PatchNames** [no default] This key specifies the
names of patches on which pressure boundary conditions will be
specified. Note that these must all be patches on the external boundary
of the domain and these patches must “cover” that external boundary.

::

    pfset BCPressure.PatchNames    "left right front back top bottom"

*string* **Patch.*patch\_name*.BCPressure.Type** [no default] This key
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

*string* **Patch.*patch\_name*.BCPressure.Cycle** [no default] This key
specifies the time cycle to which boundary condition data for patch
*patch\_name* corresponds.

::

    pfset Patch.top.BCPressure.Cycle   Constant

*string* **Patch.*patch\_name*.BCPressure.RefGeom** [no default] This
key specifies the name of the solid on which the reference patch for the
**DirEquilRefPatch** boundary condition data is given. Care should be
taken to make sure the correct solid is specified in cases of layered
domains.

::

    pfset Patch.top.BCPressure.RefGeom   domain

*string* **Patch.*patch\_name*.BCPressure.RefPatch** [no default] This
key specifies the reference patch on which the **DirEquilRefPatch**
boundary condition data is given. This patch must be on the reference
solid specified by the Patch.*patch\_name*.BCPressure.RefGeom key.

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

*list* **BCSaturation.PatchNames** [no default] This key specifies the
names of patches on which saturation boundary conditions will be
specified. Note that these must all be patches on the external boundary
of the domain and these patches must “cover” that external boundary.

::

    pfset BCSaturation.PatchNames    "left right front back top bottom"

::

    pfset Patch.left.BCSaturation.water.Type  ConstantWTHeight

*double* **Patch.*patch\_name*.BCSaturation.*phase\_name*.Value** [no
default] This key specifies either the constant saturation value if
**DirConstant** is selected or the constant water-table height if
**ConstantWTHeight** is selected.

::

    pfset Patch.top.BCSaturation.air.Value 1.0

*double* **Patch.*patch\_name*.BCSaturation.*phase\_name*.XLower** [no
default] This key specifies the lower :math:`x` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.XLower -10.0

*double* **Patch.*patch\_name*.BCSaturation.*phase\_name*.YLower** [no
default] This key specifies the lower :math:`y` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.YLower 5.0

*double* **Patch.*patch\_name*.BCSaturation.*phase\_name*.XUpper** [no
default] This key specifies the upper :math:`x` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.XUpper  125.0

*double* **Patch.*patch\_name*.BCSaturation.*phase\_name*.YUpper** [no
default] This key specifies the upper :math:`y` coordinate of a line in
the xy-plane if type **PLinearWTHeight** boundary conditions are
specified.

::

    pfset Patch.left.BCSaturation.water.YUpper  82.0

*integer* **Patch.*patch\_name*.BCPressure.*phase\_name*.NumPoints** [no
default] This key specifies the number of points on which saturation
data is given along the line used for type **DirEquilPLinear** boundary
conditions.

::

    pfset Patch.left.BCPressure.water.NumPoints 2

*double*
**Patch.*patch\_name*.BCPressure.*phase\_name*.*point\_number*.Location**
[no default] This key specifies a number between 0 and 1 which
represents the location of a point on the line for which data is given
in type **DirEquilPLinear** boundary conditions. The line is
parameterized so that 0 corresponds to the lower end of the line, and 1
corresponds to the upper end.

::

    pfset Patch.left.BCPressure.water.0.Location 0.333

*double*
**Patch.*patch\_name*.BCPressure.*phase\_name*.*point\_number*.Value**
[no default] This key specifies the water-table height for the given
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

*string* **ICSaturation.*phase\_name*.Type** [no default] This key
specifies the type of initial condition that will be applied to
different geometries for given phase, *phase\_name*. The only key
currently available is **Constant**. The choice **Constant** will apply
constants values within geometries for the phase.

::

    ICSaturation.water.Type Constant

::

    ICSaturation.water.GeomNames "domain"

*double* **Geom.*geom\_input\_name*.ICSaturation.*phase\_name*.Value**
[no default] This key specifies the initial condition value assigned to
all points in the named geometry, *geom\_input\_name*, if the type was
set to **Constant**.

::

    Geom.domain.ICSaturation.water.Value 1.0

Initial Conditions: Pressure
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The keys in this section are used to specify pressure initial conditions
for Richards’ equation cases *only*. These keys will be ignored if any
other case is run.

*string* **ICPressure.Type** [no default] This key specifies the type of
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

*list* **ICPressure.GeomNames** [no default] This key specifies the
geometry names on which the initial pressure data will be given. These
geometries must comprise the entire domain. Note that conditions for
regions that overlap other regions will have unpredictable results. The
regions given must be disjoint.

::

    pfset ICPressure.GeomNames   "toplayer middlelayer bottomlayer"

*double* **Geom.*geom\_name*.ICPressure.Value** [no default] This key
specifies the initial pressure value for type **Constant** initial
pressures and the reference pressure value for types
**HydroStaticDepth** and **HydroStaticPatch**.

::

    pfset Geom.toplayer.ICPressure.Value  -734.0

*double* **Geom.*geom\_name*.ICPressure.RefElevation** [no default] This
key specifies the reference elevation on which the reference pressure is
given for type **HydroStaticDepth** initial pressures.

::

    pfset Geom.toplayer.ICPressure.RefElevation  0.0

*double* **Geom.*geom\_name*.ICPressure.RefGeom** [no default] This key
specifies the geometry on which the reference patch resides for type
**HydroStaticPatch** initial pressures.

::

    pfset Geom.toplayer.ICPressure.RefGeom   bottomlayer

*double* **Geom.*geom\_name*.ICPressure.RefPatch** [no default] This key
specifies the patch on which the reference pressure is given for type
**HydorStaticPatch** initial pressures.

::

    pfset Geom.toplayer.ICPressure.RefPatch   bottom

*string* **Geom.*geom\_name*.ICPressure.FileName** [no default] This key
specifies the name of the file containing pressure values for the
domain. It is assumed that *geom\_name* is “domain” for this key.

::

    pfset Geom.domain.ICPressure.FileName  "ic_pressure.pfb"

Initial Conditions: Phase Concentrations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here we define initial concentration conditions for contaminants. The
format for this section of input is:

*string* **PhaseConcen.*phase\_name*.*contaminant\_name*.Type** [no
default] This key specifies the type of initial condition that will be
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

*double*
**PhaseConcen.*phase\_name*.*contaminant\_name*.*geom\_input\_name*.Value**
[no default] This key specifies the initial condition value assigned to
all points in the named geometry, *geom\_input\_name*, if the type was
set to **Constant**.

::

    PhaseConcen.water.tce.ic_concen_region.Value 0.001

*string* **PhaseConcen.*phase\_name*.*contaminant\_name*.FileName** [no
default] This key specifies the name of the “ParFlow Binary” file which
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

*string* **KnownSolution** [no default] This specifies the predefined
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

*double* **KnownSolution.Value** [no default] This key specifies the
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

*string* **Wells.Names** [no default] This key specifies the names of
the wells for which input data will be given.

::

    Wells.Names "test_well inj_well ext_well"

::

    Wells.test_well.InputType Vertical

*string* **Wells.*well\_name*.Action** [no default] This key specifies
the pumping action of the well. This key can be either **Injection** or
**Extraction**. A value of **Injection** indicates that this is an
injection well. A value of **Extraction** indicates that this is an
extraction well.

::

    Wells.test_well.Action Injection

*double* **Wells.*well\_name*.Type** [no default] This key specfies the
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

*string* **Wells.*well\_name*.ExtractionType** [no default] This key
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

*string* **Wells.*well\_name*.InjectionType** [no default] This key
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

*double* **Wells.*well\_name*.X** [no default] This key specifies the x
location of the vectical well if the input type is set to **Vectical**
or of both the extraction and injection wells if the input type is set
to **Recirc**.

::

    Wells.test_well.X 20.0

*double* **Wells.*well\_name*.Y** [no default] This key specifies the y
location of the vectical well if the input type is set to **Vectical**
or of both the extraction and injection wells if the input type is set
to **Recirc**.

::

    Wells.test_well.Y 36.5

*double* **Wells.*well\_name*.ZUpper** [no default] This key specifies
the z location of the upper extent of a vectical well if the input type
is set to **Vectical**.

::

    Wells.test_well.ZUpper 8.0

*double* **Wells.*well\_name*.ExtractionZUpper** [no default] This key
specifies the z location of the upper extent of a extraction well if the
input type is set to **Recirc**.

::

    Wells.ext_well.ExtractionZUpper 3.0

*double* **Wells.*well\_name*.InjectionZUpper** [no default] This key
specifies the z location of the upper extent of a injection well if the
input type is set to **Recirc**.

::

    Wells.inj_well.InjectionZUpper 6.0

*double* **Wells.*well\_name*.ZLower** [no default] This key specifies
the z location of the lower extent of a vectical well if the input type
is set to **Vectical**.

::

    Wells.test_well.ZLower 2.0

*double* **Wells.*well\_name*.ExtractionZLower** [no default] This key
specifies the z location of the lower extent of a extraction well if the
input type is set to **Recirc**.

::

    Wells.ext_well.ExtractionZLower 1.0

*double* **Wells.*well\_name*.InjectionZLower** [no default] This key
specifies the z location of the lower extent of a injection well if the
input type is set to **Recirc**.

::

    Wells.inj_well.InjectionZLower 4.0

*string* **Wells.*well\_name*.Method** [no default] This key specifies a
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

*string* **Wells.*well\_name*.ExtractionMethod** [no default] This key
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

*string* **Wells.*well\_name*.InjectionMethod** [no default] This key
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

*string* **Wells.*well\_name*.Cycle** [no default] This key specifies
the time cycles to which data for the well *well\_name* corresponds.

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

*double*
**Wells.*well\_name*.*interval\_name*.Saturation.*phase\_name*.Value**
[no default] This key specifies the saturation value of a vertical well.

::

    Wells.test_well.all_time.Saturation.water.Value 1.0

*double*
**Wells.*well\_name*.*interval\_name*.Concentration.*phase\_name*.*contaminant\_name*.Value**
[no default] This key specifies the contaminant value of a vertical
well.

::

    Wells.test_well.all_time.Concentration.water.tce.Value 0.0005

*double*
**Wells.*well\_name*.*interval\_name*.Injection.Concentration.*phase\_name*.*contaminant\_name*.Fraction**
[no default] This key specifies the fraction of the extracted
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

*string* **Solver.Linear** [PCG] This key specifies the linear solver
used for solver **IMPES**. Choices for this key are **MGSemi, PPCG,
PCG** and **CGHS**. The choice **MGSemi** is an algebraic mulitgrid
linear solver (not a preconditioned conjugate gradient) which may be
less robust than **PCG** as described in . The choice **PPCG** is a
preconditioned conjugate gradient solver. The choice **PCG** is a
conjugate gradient solver with a multigrid preconditioner. The choice
**CGHS** is a conjugate gradient solver.

::

    pfset Solver.Linear   MGSemi

*integer* **Solver.SadvectOrder** [2] This key controls the order of the
explicit method used in advancing the saturations. This value can be
either 1 for a standard upwind first order or 2 for a second order
Godunov method.

::

    pfset Solver.SadvectOrder 1

*integer* **Solver.AdvectOrder** [2] This key controls the order of the
explicit method used in advancing the concentrations. This value can be
either 1 for a standard upwind first order or 2 for a second order
Godunov method.

::

    pfset Solver.AdvectOrder 2

*double* **Solver.CFL** [0.7] This key gives the value of the weight put
on the computed CFL limit before computing a global timestep value.
Values greater than 1 are not suggested and in fact because this is an
approximation, values slightly less than 1 can also produce
instabilities.

::

    pfset Solver.CFL 0.7

*integer* **Solver.MaxIter** [1000000] This key gives the maximum number
of iterations that will be allowed for time-stepping. This is to prevent
a run-away simulation.

::

    pfset Solver.MaxIter 100

*double* **Solver.RelTol** [1.0] This value gives the relative tolerance
for the linear solve algorithm.

::

    pfset Solver.RelTol 1.0

*double* **Solver.AbsTol** [1E-9] This value gives the absolute
tolerance for the linear solve algorithm.

::

    pfset Solver.AbsTol 1E-8

*double* **Solver.Drop** [1E-8] This key gives a clipping value for data
written to PFSB files. Data values greater than the negative of this
value and less than the value itself are treated as zero and not written
to PFSB files.

::

    pfset Solver.Drop 1E-6

*string* **Solver.PrintSubsurf** [True] This key is used to turn on
printing of the subsurface data, Permeability and Porosity. The data is
printed after it is generated and before the main time stepping loop -
only once during the run. The data is written as a PFB file.

::

    pfset Solver.PrintSubsurf False

*string* **Solver.PrintPressure** [True] This key is used to turn on
printing of the pressure data. The printing of the data is controlled by
values in the timing information section. The data is written as a PFB
file.

::

    pfset Solver.PrintPressure False

*string* **Solver.PrintVelocities** [False] This key is used to turn on
printing of the x, y and z velocity data. The printing of the data is
controlled by values in the timing information section. The data is
written as a PFB file.

::

    pfset Solver.PrintVelocities True

*string* **Solver.PrintSaturation** [True] This key is used to turn on
printing of the saturation data. The printing of the data is controlled
by values in the timing information section. The data is written as a
PFB file.

::

    pfset Solver.PrintSaturation False

*string* **Solver.PrintConcentration** [True] This key is used to turn
on printing of the concentration data. The printing of the data is
controlled by values in the timing information section. The data is
written as a PFSB file.

::

    pfset Solver.PrintConcentration False

*string* **Solver.PrintWells** [True] This key is used to turn on
collection and printing of the well data. The data is collected at
intervals given by values in the timing information section. Printing
occurs at the end of the run when all collected data is written.

::

    pfset Solver.PrintWells False

*string* **Solver.PrintLSMSink** [False] This key is used to turn on
printing of the flux array passed from CLM to ParFlow. Printing occurs
at each **DumpInterval** time.

::

    pfset Solver.PrintLSMSink True

*string* **Solver.WriteSiloSubsurfData** [False] This key is used to
specify printing of the subsurface data, Permeability and Porosity in
silo binary file format. The data is printed after it is generated and
before the main time stepping loop - only once during the run. This data
may be read in by VisIT and other visualization packages.

::

    pfset Solver.WriteSiloSubsurfData True

*string* **Solver.WriteSiloPressure** [False] This key is used to
specify printing of the saturation data in silo binary format. The
printing of the data is controlled by values in the timing information
section. This data may be read in by VisIT and other visualization
packages.

::

    pfset Solver.WriteSiloPressure True

*string* **Solver.WriteSiloSaturation** [False] This key is used to
specify printing of the saturation data using silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloSaturation True

*string* **Solver.WriteSiloConcentration** [False] This key is used to
specify printing of the concentration data in silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloConcentration True

*string* **Solver.WriteSiloVelocities** [False] This key is used to
specify printing of the x, y and z velocity data in silo binary format.
The printing of the data is controlled by values in the timing
information section.

::

    pfset Solver.WriteSiloVelocities True

*string* **Solver.WriteSiloSlopes** [False] This key is used to specify
printing of the x and y slope data using silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloSlopes  True

*string* **Solver.WriteSiloMannings** [False] This key is used to
specify printing of the Manning’s roughness data in silo binary format.
The printing of the data is controlled by values in the timing
information section.

::

    pfset Solver.WriteSiloMannings True

*string* **Solver.WriteSiloSpecificStorage** [False] This key is used to
specify printing of the specific storage data in silo binary format. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloSpecificStorage True

*string* **Solver.WriteSiloMask** [False] This key is used to specify
printing of the mask data using silo binary format. The mask contains
values equal to one for active cells and zero for inactive cells. The
printing of the data is controlled by values in the timing information
section.

::

    pfset Solver.WriteSiloMask  True

*string* **Solver.WriteSiloEvapTrans** [False] This key is used to
specify printing of the evaporation and rainfall flux data using silo
binary format. This data comes from either clm or from external calls to
ParFlow such as *WRF*. This data is in units of :math:`[L^3 T^{-1}]`.
The printing of the data is controlled by values in the timing
information section.

::

    pfset Solver.WriteSiloEvapTrans  True

*string* **Solver.WriteSiloEvapTransSum** [False] This key is used to
specify printing of the evaporation and rainfall flux data using silo
binary format as a running, cumulative amount. This data comes from
either clm or from external calls to ParFlow such as *WRF*. This data is
in units of :math:`[L^3]`. The printing of the data is controlled by
values in the timing information section.

::

    pfset Solver.WriteSiloEvapTransSum  True

*string* **Solver.WriteSiloOverlandSum** [False] This key is used to
specify calculation and printing of the total overland outflow from the
domain using silo binary format as a running cumulative amount. This is
integrated along all domain boundaries and is calculated any location
that slopes at the edge of the domain point outward. This data is in
units of :math:`[L^3]`. The printing of the data is controlled by values
in the timing information section.

::

    pfset Solver.WriteSiloOverlandSum  True

*string* **Solver.TerrainFollowingGrid** [False] This key specifies that
a terrain-following coordinate transform is used for solver Richards.
This key sets x and y subsurface slopes to be the same as the
Topographic slopes (a value of False sets these subsurface slopes to
zero). These slopes are used in the Darcy fluxes to add a density,
gravity -dependent term. This key will not change the output files (that
is the output is still orthogonal) or the geometries (they will still
follow the computational grid)– these two things are both to do items.
This key only changes solver Richards, not solver Impes.

::

    pfset Solver.TerrainFollowingGrid                        True

SILO Options
~~~~~~~~~~~~

The following keys are used to control how SILO writes data. SILO allows
writing to PDB and HDF5 file formats. SILO also allows data compression
to be used, which can save signicant amounts of disk space for some
problems.

*string* **SILO.Filetype** [PDB] This key is used to specify the SILO
filetype. Allowed values are PDB and HDF5. Note that you must have
configured SILO with HDF5 in order to use that option.

::

    pfset SILO.Filetype  PDB

*string* **SILO.CompressionOptions** [] This key is used to specify the
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

*double* **Solver.Nonlinear.ResidualTol** [1e-7] This key specifies the
tolerance that measures how much the relative reduction in the nonlinear
residual should be before nonlinear iterations stop. The magnitude of
the residual is measured with the :math:`l^1` (max) norm.

::

    pfset Solver.Nonlinear.ResidualTol   1e-4

*double* **Solver.Nonlinear.StepTol** [1e-7] This key specifies the
tolerance that measures how small the difference between two consecutive
nonlinear steps can be before nonlinear iterations stop.

::

    pfset Solver.Nonlinear.StepTol   1e-4

*integer* **Solver.Nonlinear.MaxIter** [15] This key specifies the
maximum number of nonlinear iterations allowed before iterations stop
with a convergence failure.

::

    pfset Solver.Nonlinear.MaxIter   50

*integer* **Solver.Linear.KrylovDimension** [10] This key specifies the
maximum number of vectors to be used in setting up the Krylov subspace
in the GMRES iterative solver. These vectors are of problem size and it
should be noted that large increases in this parameter can limit problem
sizes. However, increasing this parameter can sometimes help nonlinear
solver convergence.

::

    pfset Solver.Linear.KrylovDimension   15

*integer* **Solver.Linear.MaxRestarts** [0] This key specifies the
number of restarts allowed to the GMRES solver. Restarts start the
development of the Krylov subspace over using the current iterate as the
initial iterate for the next pass.

::

    pfset Solver.Linear.MaxRestarts   2

::

    pfset Solver.MaxConvergenceFailures 4

*string* **Solver.Nonlinear.PrintFlag** [HighVerbosity] This key
specifies the amount of informational data that is printed to the
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

*string* **Solver.Nonlinear.EtaChoice** [Walker2] This key specifies how
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

*double* **Solver.Nonlinear.EtaValue** [1e-4] This key specifies the
constant value of :math:`\eta` for the EtaChoice key **EtaConstant**.

::

    pfset Solver.Nonlinear.EtaValue   1e-7

*double* **Solver.Nonlinear.EtaAlpha** [2.0] This key specifies the
value of :math:`\alpha` for the case of EtaChoice being **Walker2**.

::

    pfset Solver.Nonlinear.EtaAlpha   1.0

*double* **Solver.Nonlinear.EtaGamma** [0.9] This key specifies the
value of :math:`\gamma` for the case of EtaChoice being **Walker2**.

::

    pfset Solver.Nonlinear.EtaGamma   0.7

*string* **Solver.Nonlinear.UseJacobian** [False] This key specifies
whether the Jacobian will be used in matrix-vector products or whether a
matrix-free version of the code will run. Choices for this key are
**False** and **True**. Using the Jacobian will most likely decrease the
number of nonlinear iterations but require more memory to run.

::

    pfset Solver.Nonlinear.UseJacobian   True

*double* **Solver.Nonlinear.DerivativeEpsilon** [1e-7] This key
specifies the value of :math:`\epsilon` used in approximating the action
of the Jacobian on a vector with approximate directional derivatives of
the nonlinear function. This parameter is only used when the UseJacobian
key is **False**.

::

    pfset Solver.Nonlinear.DerivativeEpsilon   1e-8

*string* **Solver.Nonlinear.Globalization** [LineSearch] This key
specifies the type of global strategy to use. Possible choices for this
key are **InexactNewton** and **LineSearch**. The choice
**InexactNewton** specifies no global strategy, and the choice
**LineSearch** specifies that a line search strategy should be used
where the nonlinear step can be lengthened or decreased to satisfy
certain criteria.

::

    pfset Solver.Nonlinear.Globalization   LineSearch

*string* **Solver.Linear.Preconditioner** [MGSemi] This key specifies
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

*string* **Solver.Linear.Preconditioner.SymmetricMat** [Symmetric] This
key specifies whether the preconditioning matrix is symmetric. Choices
fo rthis key are **Symmetric** and **Nonsymmetric**. The choice
**Symmetric** specifies that the symmetric part of the Jacobian will be
used as the preconditioning matrix. The choice **Nonsymmetric**
specifies that the full Jacobian will be used as the preconditioning
matrix. NOTE: ONLY **Symmetric** CAN BE USED IF MGSemi IS THE SPECIFIED
PRECONDITIONER!

::

    pfset Solver.Linear.Preconditioner.SymmetricMat     Symmetric

*integer* **Solver.Linear.Preconditioner.*precond\_method*.MaxIter** [1]
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

*logical* **Solver.EvapTransFile** [False] This key specifies specifies
that the Flux terms for Richards’ equation are read in from a .pfb file.
This file has :math:`[T^-1]` units. Note this key is for a steady-state
flux and should *not* be used in conjunction with the transient key
below.

::

    pfset Solver.EvapTransFile    True

*logical* **Solver.EvapTransFileTransient** [False] This key specifies
specifies that the Flux terms for Richards’ equation are read in from a
series of flux .pfb file. Each file has :math:`[T^-1]` units. Note this
key should not be used with the key above, only one of these keys should
be set to True at a time, not both.

::

    pfset Solver.EvapTransFileTransient    True

*string* **Solver.EvapTrans.FileName** [no default] This key specifies
specifies filename for the distributed .pfb file that contains the flux
values for Richards’ equation. This file has :math:`[T^-1]` units. For
the steady-state option (*Solver.EvapTransFile=\ **True***) this key
should be the complete filename. For the transient option
(*Solver.EvapTransFileTransient=\ **True*** then the filename is a
header and ParFlow will load one file per timestep, with the form
filename.00000.pfb.

::

    pfset Solver.EvapTrans.FileName   evap.trans.test.pfb

*string* **Solver.LSM** [none] This key specifies whether a land surface
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

*integer* **OverlandFlowSpinUp** [0] This key specifies that a
*simplified* form of the overland flow boundary condition (Equation
[eq:overland:sub:`b`\ c]) be used in place of the full equation. This
formulation *removes lateral flow* and drives and ponded water pressures
to zero. While this can be helpful in spinning up the subsurface, this
is no longer coupled subsurface-surface flow. If set to zero (the
default) this key behaves normally.

::

    pfset OverlandFlowSpinUp   1

*double* **OverlandFlowSpinUpDampP1** [0.0] This key sets :math:`P_1`
and provides exponential dampening to the pressure relationship in the
overland flow equation by adding the following term:
:math:`P_2*exp(\psi*P_2)`

::

    pfset OverlandSpinupDampP1  10.0

*double* **OverlandFlowSpinUpDampP2** [0.0] This key sets :math:`P_2`
and provides exponential dampening to the pressure relationship in the
overland flow equation adding the following term:
:math:`P_2*exp(\psi*P_2)`

::

    pfset OverlandSpinupDampP2  0.1

CLM Solver Parameters
~~~~~~~~~~~~~~~~~~~~~

*string* **Solver.CLM.Print1dOut** [False] This key specifies whether
the CLM one dimensional (averaged over each processor) output file is
written or not. Choices for this key include **True** and **False**.
Note that CLM must be compiled and linked at runtime for this option to
be active.

::

    pfset Solver.CLM.Print1dOut   False

*integer* **Solver.CLM.IstepStart** [1] This key specifies the value of
the counter, *istep* in CLM. This key primarily determines the start of
the output counter for CLM.It is used to restart a run by setting the
key to the ending step of the previous run plus one. Note that CLM must
be compiled and linked at runtime for this option to be active.

::

    pfset Solver.CLM.IstepStart     8761      

*String* **Solver.CLM.MetForcing** [no default] This key specifies
defines whether 1D (uniform over the domain), 2D (spatially distributed)
or 3D (spatially distributed with multiple timesteps per .pfb forcing
file) forcing data is used. Choices for this key are **1D**, **2D** and
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

*String* **Solver.CLM.MetFilePath** [no default] This key specifies
defines the location of 1D, 2D or 3D forcing data. For 1D cases, this is
the path to a single forcing file (*e.g.* narr.1hr.txt). For 2D and 3D
cases, this is the path to the directory containing all forcing files.
Note that CLM must be compiled and linked at runtime for this option to
be active.

::

    pfset Solver.CLM.MetFilePath		"path/to/met/forcing/data/"

*integer* **Solver.CLM.MetFileNT** [no default] This key specifies the
number of timesteps per file for 3D forcing data.

::

    pfset Solver.CLM.MetFileNT	24	

::

    pfset Solver.CLM.ForceVegetation  True

*string* **Solver.WriteSiloCLM** [False] This key specifies whether the
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

*string* **Solver.PrintCLM** [False] This key specifies whether the CLM
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

*string* **Solver.WriteCLMBinary** [True] This key specifies whether the
CLM writes two dimensional binary output files in a generic binary
format. Note that CLM must be compiled and linked at runtime for this
option to be active.

::

    pfset Solver.WriteCLMBinary False

*string* **Solver.CLM.BinaryOutDir** [True] This key specifies whether
the CLM writes each set of two dimensional binary output files to a
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

*string* **Solver.CLM.CLMFileDir** [no default] This key specifies what
directory all output from the CLM is written to. This key may be set to
“./” or “” to write output to the ParFlow run directory. This directory
must be created before ParFlow is run. Note that CLM must be compiled
and linked at runtime for this option to be active.

::

    pfset Solver.CLM.CLMFileDir "CLM_Output/"

*integer* **Solver.CLM.CLMDumpInterval** [1] This key specifies how
often output from the CLM is written. This key is in integer multipliers
of the CLM timestep. Note that CLM must be compiled and linked at
runtime for this option to be active.

::

    pfset Solver.CLM.CLMDumpInterval 2

::

    pfset Solver.CLM.EvapBeta Linear

*double* **Solver.CLM.ResSat** [0.1] This key specifies the residual
saturation for the :math:`\beta` function in CLM specified above. Note
that CLM must be compiled and linked at runtime for this option to be
active.

::

    pfset Solver.CLM.ResSat  0.15

::

    pfset Solver.CLM.VegWaterStress  Pressure

*double* **Solver.CLM.WiltingPoint** [0.1] This key specifies the
wilting point for the :math:`\beta_t` function in CLM specified above.
Note that the units for this function are pressure :math:`[m]` for a
**Pressure** formulation and saturation :math:`[-]` for a **Saturation**
formulation. Note that CLM must be compiled and linked at runtime for
this option to be active.

::

    pfset Solver.CLM.WiltingPoint  0.15

*double* **Solver.CLM.FieldCapacity** [1.0] This key specifies the field
capacity for the :math:`\beta_t` function in CLM specified above. Note
that the units for this function are pressure :math:`[m]` for a
**Pressure** formulation and saturation :math:`[-]` for a **Saturation**
formulation. Note that CLM must be compiled and linked at runtime for
this option to be active.

::

    pfset Solver.CLM.FieldCapacity  0.95

*string* **Solver.CLM.IrrigationTypes** [none] This key specifies the
form of the irrigation in CLM. The valid types for this key are **none,
Spray, Drip, Instant**.

::

    pfset Solver.CLM.IrrigationTypes Drip

*string* **Solver.CLM.IrrigationCycle** [Constant] This key specifies
the cycle of the irrigation in CLM. The valid types for this key are
**Constant, Deficit**. Note only **Constant** is currently implemented.
Constant cycle applies irrigation each day from IrrigationStartTime to
IrrigationStopTime in GMT.

::

    pfset Solver.CLM.IrrigationCycle Constant

*double* **Solver.CLM.IrrigationRate** [no default] This key specifies
the rate of the irrigation in CLM in [mm/s].

::

    pfset Solver.CLM.IrrigationRate 10.

*double* **Solver.CLM.IrrigationStartTime** [no default] This key
specifies the start time of the irrigation in CLM GMT.

::

    pfset Solver.CLM.IrrigationStartTime 0800

*double* **Solver.CLM.IrrigationStopTime** [no default] This key
specifies the stop time of the irrigation in CLM GMT.

::

    pfset Solver.CLM.IrrigationStopTime 1200

*double* **Solver.CLM.IrrigationThreshold** [0.5] This key specifies the
threshold value for the irrigation in CLM [-].

::

    pfset Solver.CLM.IrrigationThreshold 0.2

*integer* **Solver.CLM.ReuseCount** [1] How many times to reuse a CLM
atmospheric forcing file input. For example timestep=1, reuse =1 is
normal behavior but reuse=2 and timestep=0.5 subdivides the time step
using the same CLM input for both halves instead of needing two files.
This is particually useful for large, distributed runs when the user
wants to run ParFlow at a smaller timestep than the CLM forcing. Forcing
files will be re-used and total fluxes adjusted accordingly without
needing duplicate files.

::

    pfset Solver.CLM.ReuseCount      5

*string* **Solver.CLM.WriteLogs** [True] When **False**, this disables
writing of the CLM output log files for each processor. For example, in
the clm.tcl test case, if this flag is added **False**,
washita.output.txt.\ *p* and washita.para.out.dat.\ *p* (were *p* is the
processor #) are not created, assuming *washita* is the run name.

::

    pfset Solver.CLM.WriteLogs    False

*string* **Solver.CLM.WriteLastRST** [False] Controls whether CLM
restart files are sequentially written or whether a single file *restart
file name*.00000.\ *p* is overwritten each time the restart file is
output, where *p* is the processor number. If “True” only one file is
written/overwritten and if “False” outputs are written more frequently.
Compatible with DailyRST and ReuseCount; for the latter, outputs are
written every n steps where n is the value of ReuseCount.

::

    pfset Solver.CLM.WriteLastRST   True

*string* **Solver.CLM.DailyRST** [True] Controls whether CLM writes
daily restart files (default) or at every time step when set to False;
outputs are numbered according to the istep from ParFlow. If
**ReuseCount=n**, with n greater than 1, the output will be written
every n steps (i.e. it still writes hourly restart files if your time
step is 0.5 or 0.25, etc...). Fully compatible with
**WriteLastRST=False** so that each daily output is overwritten to time
00000 in *restart file name*.00000.p where *p* is the processor number.

::

    pfset Solver.CLM.DailyRST    False

*string* **Solver.CLM.SingleFile** [False] Controls whether ParFlow
writes all CLM output variables as a single file per time step. When
“True”, this combines the output of all the CLM output variables into a
special multi-layer PFB with the file extension “.C.pfb”. The first 13
layers correspond to the 2-D CLM outputs and the remaining layers are
the soil temperatures in each layer. For example, a model with 4 soil
layers will create a SingleFile CLM output with 17 layers at each time
step. The file pseudo code is given below in § [ParFlow Binary Files
(.c.pfb)] and the variables and units are as specified in the multiple
PFB and SILO formats as above.

::

    pfset Solver.CLM.SingleFile   True

*integer* **Solver.CLM.RootZoneNZ** [10] This key sets the number of
soil layers the ParFlow expects from CLM. It will allocate and format
all the arrays for passing variables to and from CLM accordingly. Note
that this does not set the soil layers in CLM to do that the user needs
to change the value of the parameter nlevsoi in the file clm\_varpar.F90
in the *PARFLOW\_DIR*\\pfsimulator\\clm directory to reflect the desired
numnber of soil layers and recompile. Most likely the key
Solver.CLM.SoiLayer, described below, will also need to be changed.

::

    pfset Solver.CLM.RootZoneNZ      4

*integer* **Solver.CLM.SoiLayer** [7] This key sets the soil layer, and
thus the soil depth, that CLM uses for the seasonal temperature
adjustment for all leaf and stem area indices.

::

    pfset Solver.CLM.SoiLayer      4

*integer* **Solver.CLM.SoilLevels** [10] This key sets the number of
soil levels for CLM.

::

    pfset Solver.CLM.SoilLevels      4

*integer* **Solver.CLM.LakeLevels** [10] This key sets the number of
lake levels for CLM.

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

*integer* **NetCDF.NumStepsPerFile** [ ] This key sets number of time
steps user wishes to output in a NetCDF4 file. Once the time step count
increases beyond this number, a new file is automatically created.

::

    pfset NetCDF.NumStepsPerFile    5

*string* **NetCDF.WritePressure** [False] This key sets pressure
variable to be written in NetCDF4 file.

::

    pfset NetCDF.WritePressure    True

*string* **NetCDF.WriteSaturation** [False] This key sets saturation
variable to be written in NetCDF4 file.

::

    pfset NetCDF.WriteSaturation    True

*string* **NetCDF.WriteMannings** [False] This key sets Mannings
coefficients to be written in NetCDF4 file.

::

    pfset NetCDF.WriteMannings	    True

*string* **NetCDF.WriteSubsurface** [False] This key sets subsurface
data(permeabilities, porosity, specific storage) to be written in
NetCDF4 file.

::

    pfset NetCDF.WriteSubsurface	    True

*string* **NetCDF.WriteSlopes** [False] This key sets x and y slopes to
be written in NetCDF4 file.

::

    pfset NetCDF.WriteSlopes	    True

*string* **NetCDF.WriteMask** [False] This key sets mask to be written
in NetCDF4 file.

::

    pfset NetCDF.WriteMask	    True

*string* **NetCDF.WriteDZMultiplier** [False] This key sets DZ
multipliers to be written in NetCDF4 file.

::

    pfset NetCDF.WriteDZMultiplier	    True

*string* **NetCDF.WriteEvapTrans** [False] This key sets Evaptrans to be
written in NetCDF4 file.

::

    pfset NetCDF.WriteEvapTrans	    True

*string* **NetCDF.WriteEvapTransSum** [False] This key sets Evaptrans
sum to be written in NetCDF4 file.

::

    pfset NetCDF.WriteEvapTransSum	    True

*string* **NetCDF.WriteOverlandSum** [False] This key sets overland sum
to be written in NetCDF4 file.

::

    pfset NetCDF.WriteOverlandSum	    True

*string* **NetCDF.WriteOverlandBCFlux** [False] This key sets overland
bc flux to be written in NetCDF4 file.

::

    pfset NetCDF.WriteOverlandBCFlux	    True

NetCDF4 Chunking
~~~~~~~~~~~~~~~~

Chunking may have significant impact on I/O. If this key is not set,
default chunking scheme will be used by NetCDF library. Chunks are
hypercube(hyperslab) of any dimension. When chunking is used, chunks are
written in single write operation which can reduce access times. For
more information on chunking, refer to NetCDF4 user guide.

*string* **NetCDF.Chunking** [False] This key sets chunking for each
time varying 3-D variable in NetCDF4 file.

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

*integer* **NetCDF.ChunkX** [None] This key sets chunking size in
x-direction.

::

    pfset NetCDF.ChunkX    50

*integer* **NetCDF.ChunkY** [None] This key sets chunking size in
y-direction.

::

    pfset NetCDF.ChunkY    50

*integer* **NetCDF.ChunkZ** [None] This key sets chunking size in
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

*string* **NetCDF.ROMIOhints** [None] This key sets ROMIO hints file to
be passed on to NetCDF4 interface.If this key is set, the file must be
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

*string* **NetCDF.NodeLevelIO** [False] This key sets flag for node
level collective I/O.

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

*string* **ICPressure.Type** [no default] This key sets flag for initial
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

*string* **TopoSlopesX.Type** [no default] This key sets flag for slopes
in x direction to be read from a NetCDF file.

::

    pfset TopoSlopesX.Type   NCFile
    pfset TopoSlopesX.FileName   "slopex.nc"

*string* **TopoSlopesY.Type** [no default] This key sets flag for slopes
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

*string* **NetCDF.EvapTransFileTransient** [False] This key sets flag
for transient evaptrans forcing to be read from a NetCDF file.

::

    pfset NetCDF.EvapTransFileTransient   True

*string* **NetCDF.EvapTrans.FileName** [no default] This key sets the
name of the NetCDF transient evaptrans forcing file.

::

    pfset NetCDF.EvapTrans.FileName         "evap_trans.nc"

NetCDF4 CLM Output
~~~~~~~~~~~~~~~~~~

Similar to ParFlow binary and silo, following keys can be used to write
output CLM variables in a single NetCDF file containing multiple time
steps.

*integer* **NetCDF.CLMNumStepsPerFile** [None] This key sets number of
time steps to be written to a single NetCDF file.

::

    pfset NetCDF.CLMNumStepsPerFile 24

*string* **NetCDF.WriteCLM** [False] This key sets CLM variables to be
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

*string* **Solver.CLM.MetForcing** [no default] This key sets
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

.. |image| image:: IGWMC-logo-new.pdf
