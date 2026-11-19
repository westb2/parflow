# ---------------------------------------------------------
# Testing reservoir storage accounting with a constant
# release rate.
#
# The test checks ReservoirsOutput.csv rather than comparing
# against stored output, on one rank and on several processor
# topologies (so rank 0 does not always own the release cell):
#   * the released volume each step equals Release_Rate * dt
#   * storage changes by exactly intake - release each step
#   * a step never releases storage below Min_Release_Storage,
#     and nothing is released while storage is below it
#   * storage above Max_Storage is spilled in one step, on top
#     of Release_Rate, independent of the timestep size
# ---------------------------------------------------------

from parflow import Run
from parflow.tools.fs import mkdir, get_absolute_path
import pandas as pd
import parflow as pf
from parflow.tools.compare import pf_test_file_with_abs
import numpy as np
import argparse
import os
import sys

run_name = "reservoir_release_accounting"
sloping_slab = Run(run_name, __file__)

# ---------------------------------------------------------

sloping_slab.FileVersion = 4


# Run on several processor topologies (see test/python/CMakeLists.txt) so the
# reservoir output is checked when rank 0 does not own the release cell
parser = argparse.ArgumentParser()
parser.add_argument("-p", "--p", default=1)
parser.add_argument("-q", "--q", default=1)
parser.add_argument("-r", "--r", default=1)
args = parser.parse_args()

sloping_slab.Process.Topology.P = args.p
sloping_slab.Process.Topology.Q = args.q
sloping_slab.Process.Topology.R = args.r

# ---------------------------------------------------------
# Computational Grid
# ---------------------------------------------------------

sloping_slab.ComputationalGrid.Lower.X = 0.0
sloping_slab.ComputationalGrid.Lower.Y = 0.0
sloping_slab.ComputationalGrid.Lower.Z = 0.0

sloping_slab.ComputationalGrid.NX = 5
sloping_slab.ComputationalGrid.NY = 5
sloping_slab.ComputationalGrid.NZ = 1

sloping_slab.ComputationalGrid.DX = 10.0
sloping_slab.ComputationalGrid.DY = 10.0
sloping_slab.ComputationalGrid.DZ = 0.05

# ---------------------------------------------------------
# The Names of the GeomInputs
# ---------------------------------------------------------

sloping_slab.GeomInput.Names = "domaininput"
sloping_slab.GeomInput.domaininput.GeomName = "domain"
sloping_slab.GeomInput.domaininput.InputType = "Box"

# ---------------------------------------------------------
# Domain Geometry
# ---------------------------------------------------------

sloping_slab.Geom.domain.Lower.X = 0.0
sloping_slab.Geom.domain.Lower.Y = 0.0
sloping_slab.Geom.domain.Lower.Z = 0.0

sloping_slab.Geom.domain.Upper.X = 50.0
sloping_slab.Geom.domain.Upper.Y = 50.0
sloping_slab.Geom.domain.Upper.Z = 0.05
sloping_slab.Geom.domain.Patches = "x_lower x_upper y_lower y_upper z_lower z_upper"

# -----------------------------------------------------------------------------
# Perm
# -----------------------------------------------------------------------------

sloping_slab.Geom.Perm.Names = "domain"
sloping_slab.Geom.domain.Perm.Type = "Constant"
sloping_slab.Geom.domain.Perm.Value = 0.0000001

sloping_slab.Perm.TensorType = "TensorByGeom"

sloping_slab.Geom.Perm.TensorByGeom.Names = "domain"

sloping_slab.Geom.domain.Perm.TensorValX = 1.0
sloping_slab.Geom.domain.Perm.TensorValY = 1.0
sloping_slab.Geom.domain.Perm.TensorValZ = 1.0

# -----------------------------------------------------------------------------
# Specific Storage
# -----------------------------------------------------------------------------

sloping_slab.SpecificStorage.Type = "Constant"
sloping_slab.SpecificStorage.GeomNames = "domain"
sloping_slab.Geom.domain.SpecificStorage.Value = 1.0e-4

# -----------------------------------------------------------------------------
# Phases
# -----------------------------------------------------------------------------

sloping_slab.Phase.Names = "water"

sloping_slab.Phase.water.Density.Type = "Constant"
sloping_slab.Phase.water.Density.Value = 1.0

sloping_slab.Phase.water.Viscosity.Type = "Constant"
sloping_slab.Phase.water.Viscosity.Value = 1.0

# -----------------------------------------------------------------------------
# Contaminants
# -----------------------------------------------------------------------------

sloping_slab.Contaminants.Names = ""

# -----------------------------------------------------------------------------
# Retardation
# -----------------------------------------------------------------------------

sloping_slab.Geom.Retardation.GeomNames = ""

# -----------------------------------------------------------------------------
# Gravity
# -----------------------------------------------------------------------------

sloping_slab.Gravity = 1.0

# -----------------------------------------------------------------------------
# Setup timing info
# -----------------------------------------------------------------------------

sloping_slab.TimingInfo.BaseUnit = 0.05
sloping_slab.TimingInfo.StartCount = 0
sloping_slab.TimingInfo.StartTime = 0.0
sloping_slab.TimingInfo.StopTime = 1.0
sloping_slab.TimingInfo.DumpInterval = -1
sloping_slab.TimeStep.Type = "Constant"
sloping_slab.TimeStep.Value = 0.05

# -----------------------------------------------------------------------------
# Porosity
# -----------------------------------------------------------------------------

sloping_slab.Geom.Porosity.GeomNames = "domain"
sloping_slab.Geom.domain.Porosity.Type = "Constant"
sloping_slab.Geom.domain.Porosity.Value = 0.01

# -----------------------------------------------------------------------------
# Domain
# -----------------------------------------------------------------------------

sloping_slab.Domain.GeomName = "domain"

# -----------------------------------------------------------------------------
# Relative Permeability
# -----------------------------------------------------------------------------

sloping_slab.Phase.RelPerm.Type = "VanGenuchten"
sloping_slab.Phase.RelPerm.GeomNames = "domain"

sloping_slab.Geom.domain.RelPerm.Alpha = 6.0
sloping_slab.Geom.domain.RelPerm.N = 2.0

# ---------------------------------------------------------
# Saturation
# ---------------------------------------------------------

sloping_slab.Phase.Saturation.Type = "VanGenuchten"
sloping_slab.Phase.Saturation.GeomNames = "domain"

sloping_slab.Geom.domain.Saturation.Alpha = 6.0
sloping_slab.Geom.domain.Saturation.N = 2.0
sloping_slab.Geom.domain.Saturation.SRes = 0.2
sloping_slab.Geom.domain.Saturation.SSat = 1.0

# -----------------------------------------------------------------------------
# Wells
# -----------------------------------------------------------------------------

sloping_slab.Wells.Names = ""

# -----------------------------------------------------------------------------
# Time Cycles
# -----------------------------------------------------------------------------

sloping_slab.Cycle.Names = "constant rainrec"
sloping_slab.Cycle.constant.Names = "alltime"
sloping_slab.Cycle.constant.alltime.Length = 1
sloping_slab.Cycle.constant.Repeat = -1

# rainfall and recession time periods are defined here
# rain for 1 hour, recession for 2 hours

sloping_slab.Cycle.rainrec.Names = "rain rec"
sloping_slab.Cycle.rainrec.rain.Length = 2
sloping_slab.Cycle.rainrec.rec.Length = 300
sloping_slab.Cycle.rainrec.Repeat = -1

# -----------------------------------------------------------------------------
# Boundary Conditions: Pressure
# -----------------------------------------------------------------------------

sloping_slab.BCPressure.PatchNames = sloping_slab.Geom.domain.Patches

sloping_slab.Patch.x_lower.BCPressure.Type = "FluxConst"
sloping_slab.Patch.x_lower.BCPressure.Cycle = "constant"
sloping_slab.Patch.x_lower.BCPressure.alltime.Value = 0.0

sloping_slab.Patch.y_lower.BCPressure.Type = "FluxConst"
sloping_slab.Patch.y_lower.BCPressure.Cycle = "constant"
sloping_slab.Patch.y_lower.BCPressure.alltime.Value = 0.0

sloping_slab.Patch.z_lower.BCPressure.Type = "FluxConst"
sloping_slab.Patch.z_lower.BCPressure.Cycle = "constant"
sloping_slab.Patch.z_lower.BCPressure.alltime.Value = 0.0

sloping_slab.Patch.x_upper.BCPressure.Type = "FluxConst"
sloping_slab.Patch.x_upper.BCPressure.Cycle = "constant"
sloping_slab.Patch.x_upper.BCPressure.alltime.Value = 0.0

sloping_slab.Patch.y_upper.BCPressure.Type = "FluxConst"
sloping_slab.Patch.y_upper.BCPressure.Cycle = "constant"
sloping_slab.Patch.y_upper.BCPressure.alltime.Value = 0.0

## overland flow boundary condition with very heavy rainfall then slight ET
sloping_slab.Patch.z_upper.BCPressure.Type = "OverlandFlow"
sloping_slab.Patch.z_upper.BCPressure.Cycle = "rainrec"
sloping_slab.Patch.z_upper.BCPressure.rain.Value = -0.01
sloping_slab.Patch.z_upper.BCPressure.rec.Value = 0.0000

# ---------------------------------------------------------
# Mannings coefficient
# ---------------------------------------------------------

sloping_slab.Mannings.Type = "Constant"
sloping_slab.Mannings.GeomNames = "domain"
sloping_slab.Mannings.Geom.domain.Value = 3.0e-6

# -----------------------------------------------------------------------------
# Phase sources:
# -----------------------------------------------------------------------------

sloping_slab.PhaseSources.water.Type = "Constant"
sloping_slab.PhaseSources.water.GeomNames = "domain"
sloping_slab.PhaseSources.water.Geom.domain.Value = 0.0

# -----------------------------------------------------------------------------
# Exact solution specification for error calculations
# -----------------------------------------------------------------------------

sloping_slab.KnownSolution = "NoKnownSolution"

# -----------------------------------------------------------------------------
# Set solver parameters
# -----------------------------------------------------------------------------

sloping_slab.Solver = "Richards"
sloping_slab.Solver.MaxIter = 2500

sloping_slab.Solver.Nonlinear.MaxIter = 50
sloping_slab.Solver.Nonlinear.ResidualTol = 1e-9
sloping_slab.Solver.Nonlinear.EtaChoice = "EtaConstant"
sloping_slab.Solver.Nonlinear.EtaValue = 0.01
sloping_slab.Solver.Nonlinear.UseJacobian = False

sloping_slab.Solver.Nonlinear.DerivativeEpsilon = 1e-15
sloping_slab.Solver.Nonlinear.StepTol = 1e-20
sloping_slab.Solver.Nonlinear.Globalization = "LineSearch"
sloping_slab.Solver.Linear.KrylovDimension = 20
sloping_slab.Solver.Linear.MaxRestart = 2

sloping_slab.Solver.Linear.Preconditioner = "PFMG"
sloping_slab.Solver.PrintSubsurf = False
sloping_slab.Solver.Drop = 1e-20
sloping_slab.Solver.AbsTol = 1e-10

sloping_slab.Solver.OverlandKinematic.Epsilon = 1e-5

sloping_slab.Solver.WriteSiloSubsurfData = False
sloping_slab.Solver.WriteSiloPressure = False
sloping_slab.Solver.WriteSiloSlopes = False

sloping_slab.Solver.WriteSiloSaturation = False
sloping_slab.Solver.WriteSiloConcentration = False

# ---------------------------------------------------------
# Initial conditions: water pressure
# ---------------------------------------------------------

# set water table to be at the bottom of the domain, the top layer is initially dry
sloping_slab.ICPressure.Type = "HydroStaticPatch"
sloping_slab.ICPressure.GeomNames = "domain"
sloping_slab.Geom.domain.ICPressure.Value = -3.0

sloping_slab.Geom.domain.ICPressure.RefGeom = "domain"
sloping_slab.Geom.domain.ICPressure.RefPatch = "z_upper"


#   #### Set the slopes. Will flow in -x direction
sloping_slab.TopoSlopesX.Type = "Constant"
sloping_slab.TopoSlopesX.GeomNames = "domain"
sloping_slab.TopoSlopesX.Geom.domain.Value = -0.01

sloping_slab.TopoSlopesY.Type = "Constant"
sloping_slab.TopoSlopesY.GeomNames = "domain"
sloping_slab.TopoSlopesY.Geom.domain.Value = 0.0

# turn on analytical jacobian and re-test
sloping_slab.Solver.Nonlinear.UseJacobian = True
sloping_slab.Solver.Linear.Preconditioner.PCMatrixType = "PFSymmetric"


sloping_slab.Solver.PrintReservoirs = True

DT = sloping_slab.TimeStep.Value
RELEASE_RATE = 1000.0
MIN_STORAGE = 4000000.0
MAX_STORAGE = 7000000.0

sloping_slab.Reservoirs.Names = "reservoir"
sloping_slab.Reservoirs.Overland_Flow_Solver = "OverlandFlow"

sloping_slab.Reservoirs.reservoir.Intake_X = 25
sloping_slab.Reservoirs.reservoir.Intake_Y = 25
sloping_slab.Reservoirs.reservoir.Secondary_Intake_X = 25
sloping_slab.Reservoirs.reservoir.Secondary_Intake_Y = 35
sloping_slab.Reservoirs.reservoir.Release_X = 35
sloping_slab.Reservoirs.reservoir.Release_Y = 25
sloping_slab.Reservoirs.reservoir.Has_Secondary_Intake_Cell = 1

sloping_slab.Reservoirs.reservoir.Release_Rate = RELEASE_RATE
sloping_slab.Reservoirs.reservoir.Max_Storage = MAX_STORAGE
sloping_slab.Reservoirs.reservoir.Min_Release_Storage = MIN_STORAGE


def run_case(case_name):
    run_dir = get_absolute_path(
        f"test_output/{run_name}_{case_name}_{args.p}_{args.q}_{args.r}"
    )
    mkdir(run_dir)
    sloping_slab.run(working_directory=run_dir)
    output = pd.read_csv(os.path.join(run_dir, "ReservoirsOutput.csv"))
    return output.sort_values("time").reset_index(drop=True)


passed = True


def check(condition, message):
    global passed
    if not condition:
        print(f"FAILED: {message}")
        passed = False


def check_mass_balance(output, case_name):
    # Values are printed with 6 decimals; allow for that rounding
    storage_change = output["storage"].diff().iloc[1:].to_numpy()
    net_in = (
        (
            output["intake_amount_since_last_row"]
            - output["release_amount_since_last_row"]
        )
        .iloc[1:]
        .to_numpy()
    )
    check(
        np.allclose(storage_change, net_in, atol=1e-4),
        f"{case_name}: storage change does not equal intake - release",
    )


# -----------------------------------------------------------------------------
# Case 1: storage well between the limits. Every step releases
# Release_Rate * dt and the output is consistent.
# -----------------------------------------------------------------------------
sloping_slab.Reservoirs.reservoir.Storage = 5500000.0

output = run_case("basic")
check(len(output) == 21, f"basic: expected 21 output rows, got {len(output)}")
released = output["release_amount_since_last_row"].to_numpy()[1:]
check(
    np.allclose(released, RELEASE_RATE * DT, atol=1e-4),
    "basic: released volume per step does not equal Release_Rate * dt",
)
check(
    np.allclose(output["release_rate"].to_numpy(), RELEASE_RATE),
    "basic: reported release rate is not Release_Rate",
)
check_mass_balance(output, "basic")

# -----------------------------------------------------------------------------
# Case 2: storage is just above Min_Release_Storage. A step may only release
# down to the minimum, never below it.
# -----------------------------------------------------------------------------
sloping_slab.Reservoirs.reservoir.Storage = MIN_STORAGE + 10.0

output = run_case("min_storage_cap")
released = output["release_amount_since_last_row"].to_numpy()[1:]
# Release_Rate * dt = 50, but only 10 is available
check(
    np.isclose(released[0], 10.0, atol=1e-4),
    f"min_storage_cap: first step released {released[0]}, expected 10",
)
check(
    np.all(output["storage"].to_numpy() >= MIN_STORAGE - 1e-4),
    "min_storage_cap: storage fell below Min_Release_Storage",
)
check_mass_balance(output, "min_storage_cap")

# -----------------------------------------------------------------------------
# Case 3: storage starts below Min_Release_Storage. Nothing is released and
# storage is never drawn down.
# -----------------------------------------------------------------------------
sloping_slab.Reservoirs.reservoir.Storage = MIN_STORAGE - 100.0

output = run_case("below_min_storage")
released = output["release_amount_since_last_row"].to_numpy()[1:]
check(np.all(released == 0), "below_min_storage: water was released")
check(
    np.all(np.diff(output["storage"].to_numpy()) >= 0),
    "below_min_storage: storage decreased",
)
check_mass_balance(output, "below_min_storage")

# -----------------------------------------------------------------------------
# Case 4: storage starts above Max_Storage. The excess is spilled in the first
# step on top of Release_Rate, and the amount spilled does not depend on the
# timestep size.
# -----------------------------------------------------------------------------
sloping_slab.Reservoirs.reservoir.Storage = MAX_STORAGE + 100.0

for dt in [DT, 2 * DT]:
    sloping_slab.TimeStep.Value = dt
    case_name = f"spill_dt_{dt}"
    output = run_case(case_name)
    first_step = output.iloc[1]
    scheduled = RELEASE_RATE * dt
    expected_release = scheduled + 100.0
    check(
        np.isclose(
            first_step["release_amount_since_last_row"], expected_release, atol=1e-4
        ),
        f"{case_name}: first step released "
        f"{first_step['release_amount_since_last_row']}, expected {expected_release}",
    )
    # The excess is gone after one step; storage is Max_Storage less the
    # normal release, plus whatever flowed in
    expected_storage = (
        MAX_STORAGE - scheduled + first_step["intake_amount_since_last_row"]
    )
    check(
        np.isclose(first_step["storage"], expected_storage, atol=1e-4),
        f"{case_name}: storage after spilling is {first_step['storage']}, "
        f"expected {expected_storage}",
    )
    check_mass_balance(output, case_name)
sloping_slab.TimeStep.Value = DT

if passed:
    print(f"{run_name} : PASSED")
else:
    print(f"{run_name} : FAILED")
    sys.exit(1)
