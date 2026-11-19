# ---------------------------------------------------------
# Testing reservoirs built from an external database with
# parflow.tools.reservoirs (here a small made-up dataset in
# the ResOpsUS layout, so the real dataset is not needed).
#
# The run starts at 23:30, so a day boundary falls inside
# the one-hour run. The test checks that:
#   * dams inside the domain are placed on the right cells
#     and dams outside it are left out
#   * daily mean outflow in m^3/s is converted to model
#     units (m^3/hour) and switches at midnight
#   * initial storage comes from the record
#   * a second call reuses the cached release files
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

run_name = "reservoir_database_resopsus"
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

from datetime import datetime

from parflow.tools.reservoirs import LatLonGrid, ResOpsUS

DT = sloping_slab.TimeStep.Value
NX = sloping_slab.ComputationalGrid.NX
NY = sloping_slab.ComputationalGrid.NY

# The model domain covers 1 x 1 degree; cell (i, j) has its centre here
LAT_MIN, LON_MIN = 40.0, -100.0


def cell_center(i, j):
    return LAT_MIN + (j + 0.5) / NY, LON_MIN + (i + 0.5) / NX


# Daily means: 0.1 m^3/s on 1 January, 0.2 m^3/s on 2 January
OUTFLOW = {"2000-01-01": 0.1, "2000-01-02": 0.2, "2000-01-03": 0.3}
STORAGE_MCM = {"2000-01-01": 4.9, "2000-01-02": 5.0, "2000-01-03": 5.0}


def write_dataset(path):
    os.makedirs(os.path.join(path, "attributes"), exist_ok=True)
    os.makedirs(os.path.join(path, "time_series_all"), exist_ok=True)
    lat, lon = cell_center(2, 2)
    with open(os.path.join(path, "attributes", "reservoir_attributes.csv"), "w") as f:
        f.write(
            "DAM_ID,DAM_NAME,STATE,AGENCY_CODE,LONG,LAT,TIME_SERIES_START,"
            "TIME_SERIES_END,INCONSISTENCIES_NOTED\n"
        )
        f.write(f"101,Inside Dam,Somewhere,XX,{lon},{lat},1/1/00,1/3/00,NA\n")
        f.write("102,Outside Dam,Somewhere,XX,-80.0,35.0,1/1/00,1/3/00,NA\n")
    for dam_id in (101, 102):
        with open(
            os.path.join(path, "time_series_all", f"ResOpsUS_{dam_id}.csv"), "w"
        ) as f:
            f.write("date,storage,inflow,outflow,evaporation,elevation\n")
            for day in OUTFLOW:
                f.write(f"{day},{STORAGE_MCM[day]},NA,{OUTFLOW[day]},NA,NA\n")


run_dir = get_absolute_path(f"test_output/{run_name}")
mkdir(run_dir)
dataset_dir = os.path.join(run_dir, "ResOpsUS")
write_dataset(dataset_dir)

# Uniform slopes as set on the run: -0.01 in x, 0 in y
slope_x = np.full((NY, NX), -0.01)
slope_y = np.zeros((NY, NX))


def add_reservoirs():
    return ResOpsUS(dataset_dir).add_to_run(
        sloping_slab,
        run_dir=run_dir,
        start_date=datetime(2000, 1, 1, 23, 30),
        time_unit="hours",
        grid=LatLonGrid(LAT_MIN, LAT_MIN + 1, LON_MIN, LON_MIN + 1),
        overland_flow_solver="OverlandFlow",
        slope_x=slope_x,
        slope_y=slope_y,
        overwrite=True,
    )


passed = True


def check(condition, message):
    global passed
    if not condition:
        print(f"FAILED: {message}")
        passed = False


summary = add_reservoirs()
check(
    [r["name"] for r in summary["placed"]] == ["ResOpsUS_101"],
    f"expected only ResOpsUS_101 to be placed, got {summary['placed']}",
)
check("102" in summary["skipped"], "the dam outside the domain was not left out")
placed = summary["placed"][0]
check(placed["intake_cell"] == [2, 2], f"intake cell {placed['intake_cell']}")
# Cell-centred slope_x < 0 drains in +x
check(placed["release_cell"] == [3, 2], f"release cell {placed['release_cell']}")

sloping_slab.Solver.PrintReservoirs = True
sloping_slab.run(working_directory=run_dir)

output = pd.read_csv(os.path.join(run_dir, "ReservoirsOutput.csv"))
output = output.sort_values("time").reset_index(drop=True)
times = output["time"].to_numpy()
later = times > 0
# 23:30 + t hours: until midnight (t <= 0.5) the 1 January mean applies
expected_rate = np.where(times <= 0.5 + 1e-9, 0.1 * 3600, 0.2 * 3600)
check(
    np.allclose(output["release_rate"].to_numpy()[later], expected_rate[later]),
    "reported release rate does not follow the daily means",
)
check(
    np.allclose(
        output["release_amount_since_last_row"].to_numpy()[later],
        expected_rate[later] * DT,
        atol=1e-4,
    ),
    "released volume per step does not equal rate * dt",
)
check(
    np.isclose(output["storage"].iloc[0], 4.9e6),
    f"initial storage {output['storage'].iloc[0]}, expected 4.9e6",
)

# Calling again with the same inputs reuses the cached files
again = ResOpsUS(dataset_dir).add_to_run(
    sloping_slab,
    run_dir=run_dir,
    start_date=datetime(2000, 1, 1, 23, 30),
    time_unit="hours",
    grid=LatLonGrid(LAT_MIN, LAT_MIN + 1, LON_MIN, LON_MIN + 1),
    overland_flow_solver="OverlandFlow",
    slope_x=slope_x,
    slope_y=slope_y,
)
check(again["from_cache"], "second call did not reuse the cached release files")

if passed:
    print(f"{run_name} : PASSED")
else:
    print(f"{run_name} : FAILED")
    sys.exit(1)
