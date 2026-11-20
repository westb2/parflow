"""
Unit tests for parflow.tools.reservoirs, which converts reservoir datasets
such as ResOpsUS into ParFlow reservoir inputs.

The tests write a small dataset in the ResOpsUS layout to a temporary folder,
so they do not need the real dataset.
"""

import os
import shutil
import sys
import tempfile
import unittest
from datetime import datetime

import numpy as np
import pandas as pd

rootdir = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../.."))
sys.path.insert(0, rootdir)

from parflow import Run
from parflow.tools.reservoirs import (
    LatLonGrid,
    ProjectedGrid,
    ResOpsUS,
    add_reservoirs_to_run,
    load_corrections,
)

try:
    import pyproj  # noqa: F401

    HAVE_PYPROJ = True
except ImportError:
    HAVE_PYPROJ = False

# A 10 x 10 model grid of 100 m cells covering 0..1 degree in lat and lon
NX = NY = 10
LAT_MIN, LON_MIN = 40.0, -100.0


def cell_center(i, j):
    """Latitude/longitude of the centre of model cell (i, j)."""
    return LAT_MIN + (j + 0.5) / NY, LON_MIN + (i + 0.5) / NX


def write_resopsus(path, dams, records):
    """Write a minimal ResOpsUS-style dataset.

    dams: list of (dam_id, name, lat, lon)
    records: dam_id -> DataFrame with date, storage [MCM], outflow [m^3/s]
    """
    os.makedirs(os.path.join(path, "attributes"))
    os.makedirs(os.path.join(path, "time_series_all"))
    with open(os.path.join(path, "attributes", "reservoir_attributes.csv"), "w") as f:
        f.write(
            "DAM_ID,DAM_NAME,STATE,AGENCY_CODE,LONG,LAT,TIME_SERIES_START,"
            "TIME_SERIES_END,INCONSISTENCIES_NOTED\n"
        )
        for dam_id, name, lat, lon in dams:
            f.write(f"{dam_id},{name},Somewhere,XX,{lon},{lat},1/1/00,12/31/20,NA\n")
    for dam_id, record in records.items():
        table = pd.DataFrame(
            {
                "date": record["date"],
                "storage": record["storage"],
                "inflow": np.nan,
                "outflow": record["outflow"],
                "evaporation": np.nan,
                "elevation": np.nan,
            }
        )
        table.to_csv(
            os.path.join(path, "time_series_all", f"ResOpsUS_{dam_id}.csv"),
            index=False,
            na_rep="NA",
        )


def make_record(start, days, storage=100.0, outflow=10.0):
    dates = pd.date_range(start, periods=days, freq="D")
    return {
        "date": dates.strftime("%Y-%m-%d"),
        "storage": np.full(days, storage),
        "outflow": np.full(days, outflow),
    }


def make_run(stop_time=72.0):
    # Run parses the command line; give it a clean one under test runners
    argv, sys.argv = sys.argv, sys.argv[:1]
    try:
        run = Run("reservoir_unit_test")
    finally:
        sys.argv = argv
    run.ComputationalGrid.Lower.X = 0.0
    run.ComputationalGrid.Lower.Y = 0.0
    run.ComputationalGrid.DX = 100.0
    run.ComputationalGrid.DY = 100.0
    run.ComputationalGrid.NX = NX
    run.ComputationalGrid.NY = NY
    run.TimingInfo.StartTime = 0.0
    run.TimingInfo.StopTime = stop_time
    return run


def read_release(run_dir, reservoir):
    return pd.read_csv(
        os.path.join(run_dir, reservoir["keys"]["Release_File"]), comment="#"
    )


class TestReservoirs(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.data = os.path.join(self.tmp, "ResOpsUS")
        self.run_dir = os.path.join(self.tmp, "run")
        os.makedirs(self.run_dir)
        self.grid = LatLonGrid(LAT_MIN, LAT_MIN + 1, LON_MIN, LON_MIN + 1)
        # Uniform slopes draining in +x: slope_x < 0
        self.slope_x = np.full((NY, NX), -0.01)
        self.slope_y = np.zeros((NY, NX))

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def add(self, run=None, **kwargs):
        options = dict(
            run_dir=self.run_dir,
            start_date=datetime(2000, 1, 2),
            time_unit="hours",
            grid=self.grid,
            overland_flow_solver="OverlandKinematic",
            slope_x=self.slope_x,
            slope_y=self.slope_y,
            use_default_corrections=False,
            verbose=False,
        )
        options.update(kwargs)
        return add_reservoirs_to_run(run or make_run(), ResOpsUS(self.data), **options)

    # -------------------------------------------------------------------------

    def test_units_times_and_keys(self):
        lat, lon = cell_center(3, 4)
        write_resopsus(
            self.data,
            [(7, "Test Dam", lat, lon)],
            {7: make_record("2000-01-01", 10, storage=2.0, outflow=10.0)},
        )
        run = make_run()
        summary = self.add(run)
        self.assertEqual(len(summary["placed"]), 1)
        reservoir = summary["placed"][0]
        self.assertEqual(reservoir["intake_cell"], [3, 4])
        # Slopes drain east, so the release is the next cell in +x
        self.assertEqual(reservoir["release_cell"], [4, 4])

        release = read_release(self.run_dir, reservoir)
        # 3 days of 24 hours starting at model time 0
        self.assertEqual(list(release["time"]), [0.0, 24.0, 48.0])
        # 10 m^3/s = 36000 m^3/hour
        np.testing.assert_allclose(release["release_rate"], 36000.0)

        keys = run.Reservoirs["ResOpsUS_7"]
        self.assertEqual(run.Reservoirs.Overland_Flow_Solver, "OverlandKinematic")
        self.assertIn("ResOpsUS_7", run.Reservoirs.Names)
        self.assertAlmostEqual(keys.Intake_X, 350.0)
        self.assertAlmostEqual(keys.Intake_Y, 450.0)
        self.assertAlmostEqual(keys.Release_X, 450.0)
        self.assertAlmostEqual(keys.Storage, 2.0e6)
        self.assertAlmostEqual(keys.Max_Storage, 2.0e6)
        self.assertEqual(keys.Release_Type, "TimeSeries")

    def test_start_time_offset_and_partial_days(self):
        lat, lon = cell_center(1, 1)
        record = make_record("2000-01-01", 10)
        record["outflow"] = np.arange(10, dtype=float)
        write_resopsus(self.data, [(1, "A", lat, lon)], {1: record})
        run = make_run(stop_time=124.0)
        run.TimingInfo.StartTime = 100.0
        # Model time 100 is noon on 2 January; the run ends 1 day later
        summary = self.add(run, start_date=datetime(2000, 1, 2, 12))
        release = read_release(self.run_dir, summary["placed"][0])
        # Rows at the start of 2 and 3 January, in model time
        self.assertEqual(list(release["time"]), [88.0, 112.0])
        np.testing.assert_allclose(release["release_rate"], [1 * 3600.0, 2 * 3600.0])

    def test_gap_policies(self):
        lat, lon = cell_center(2, 2)
        record = make_record("2000-01-01", 10)
        record["outflow"][2] = np.nan  # 3 January, inside the run
        write_resopsus(self.data, [(5, "Gappy", lat, lon)], {5: record})

        summary = self.add(gap_policy="interpolate")
        self.assertEqual(summary["placed"][0]["outflow_days_filled"], 1)

        summary = self.add(gap_policy="skip", overwrite=True)
        self.assertEqual(summary["placed"], [])
        self.assertIn("5", summary["skipped"])

        with self.assertRaises(ValueError):
            self.add(gap_policy="fail", overwrite=True)

    def test_long_gaps_are_not_filled(self):
        lat, lon = cell_center(2, 2)
        record = make_record("2000-01-01", 40)
        record["outflow"][3:13] = np.nan  # 10 days from 4 January
        write_resopsus(self.data, [(5, "Gappy", lat, lon)], {5: record})
        run = make_run(stop_time=24.0 * 30)
        summary = self.add(run, max_fill_days=10)
        self.assertEqual(summary["placed"][0]["outflow_days_filled"], 10)
        summary = self.add(make_run(stop_time=24.0 * 30), max_fill_days=9)
        self.assertEqual(summary["placed"], [])
        self.assertIn("max_fill_days", summary["skipped"]["5"])

    def test_initial_storage_gap(self):
        lat, lon = cell_center(2, 2)
        record = make_record("2000-01-01", 40)
        record["storage"][0:5] = np.nan  # no storage until 6 January
        record["storage"][5:] = 3.0
        write_resopsus(self.data, [(5, "A", lat, lon)], {5: record})
        # The run starts on 2 January: 4 days from the first storage value
        summary = self.add(max_fill_days=4)
        self.assertTrue(summary["placed"][0]["storage_filled"])
        self.assertAlmostEqual(summary["placed"][0]["keys"]["Storage"], 3.0e6)
        summary = self.add(max_fill_days=3)
        self.assertIn("storage", summary["skipped"]["5"])

    def test_outside_domain_and_offset(self):
        inside_lat, inside_lon = cell_center(5, 5)
        write_resopsus(
            self.data,
            [(1, "Inside", inside_lat, inside_lon), (2, "Outside", 10.0, 10.0)],
            {
                1: make_record("2000-01-01", 10),
                2: make_record("2000-01-01", 10),
            },
        )
        summary = self.add()
        self.assertEqual([r["dam_id"] for r in summary["placed"]], ["1"])
        self.assertEqual(summary["skipped"]["2"], "outside the model domain")

        # A model domain starting at cell (3, 2) of a larger grid
        big = LatLonGrid(LAT_MIN, LAT_MIN + 1, LON_MIN, LON_MIN + 1, nx=NX, ny=NY)
        summary = self.add(
            grid=big,
            domain_offset=(3, 2),
            slope_x=None,
            slope_y=None,
            corrections_file=self.write_corrections(
                "dam_id,release_lat,release_lon\n1,%f,%f\n" % cell_center(6, 5)
            ),
            overwrite=True,
        )
        self.assertEqual(summary["placed"][0]["intake_cell"], [2, 3])
        self.assertEqual(summary["placed"][0]["release_cell"], [3, 3])

    def test_outlet_conventions(self):
        lat, lon = cell_center(4, 4)
        write_resopsus(
            self.data, [(1, "A", lat, lon)], {1: make_record("2000-01-01", 10)}
        )
        # OverlandKinematic: the south face slope of (4, 4) is stored in (4, 3);
        # a positive value there drains south
        slope_x = np.zeros((NY, NX))
        slope_y = np.zeros((NY, NX))
        slope_y[3, 4] = 0.05
        summary = self.add(slope_x=slope_x, slope_y=slope_y)
        self.assertEqual(summary["placed"][0]["release_cell"], [4, 3])

        # OverlandFlow: cell-centred slopes; positive slope_x rises to the
        # east, so water flows west
        slope_x = np.zeros((NY, NX))
        slope_x[4, 4] = 0.02
        summary = self.add(
            slope_x=slope_x,
            slope_y=np.zeros((NY, NX)),
            overland_flow_solver="OverlandFlow",
            overwrite=True,
        )
        self.assertEqual(summary["placed"][0]["release_cell"], [3, 4])

    def test_snapping(self):
        lat, lon = cell_center(4, 4)
        write_resopsus(
            self.data, [(1, "A", lat, lon)], {1: make_record("2000-01-01", 10)}
        )
        accumulation = np.ones((NY, NX))
        accumulation[5, 6] = 100.0  # the river, two cells east and one north
        summary = self.add(flow_accumulation=accumulation, snap_radius=2)
        self.assertEqual(summary["placed"][0]["intake_cell"], [6, 5])
        summary = self.add(flow_accumulation=accumulation, snap_radius=1)
        self.assertEqual(summary["placed"][0]["intake_cell"], [4, 4])

    def write_corrections(self, text, name="corrections.csv"):
        path = os.path.join(self.tmp, name)
        with open(path, "w") as f:
            f.write(text)
        return path

    def test_corrections(self):
        lat, lon = cell_center(1, 1)
        write_resopsus(
            self.data, [(9, "A", lat, lon)], {9: make_record("2000-01-01", 10)}
        )
        accumulation = np.ones((NY, NX))
        accumulation[1, 2] = 100.0
        corrections = self.write_corrections(
            "# a comment\n"
            "dam_id,intake_lat,intake_lon,release_lat,release_lon,note\n"
            "9,%f,%f,%f,%f,moved by hand\n" % (cell_center(7, 7) + cell_center(7, 6))
        )
        summary = self.add(corrections_file=corrections, flow_accumulation=accumulation)
        placed = summary["placed"][0]
        # A corrected intake is used as given, not snapped
        self.assertEqual(placed["intake_cell"], [7, 7])
        self.assertEqual(placed["release_cell"], [7, 6])
        self.assertEqual(placed["intake_source"], "correction")

    def test_load_corrections_merges_and_validates(self):
        first = self.write_corrections(
            "dam_id,intake_lat,intake_lon\n1,40.1,-99.9\n2,40.2,-99.8\n", "a.csv"
        )
        second = self.write_corrections(
            "dam_id,release_lat,release_lon,intake_lat,intake_lon\n1,40.5,-99.5,,\n",
            "b.csv",
        )
        corrections = load_corrections(first, second)
        self.assertEqual(corrections["1"]["intake_lat"], 40.1)
        self.assertEqual(corrections["1"]["release_lat"], 40.5)
        self.assertNotIn("release_lat", corrections["2"])
        bad = self.write_corrections("dam_id,intake_lat\n3,40.0\n", "bad.csv")
        with self.assertRaises(ValueError):
            load_corrections(bad)

    def test_shipped_corrections_file_loads(self):
        self.assertTrue(ResOpsUS.default_corrections_file.exists())
        self.assertIsInstance(load_corrections(ResOpsUS.default_corrections_file), dict)

    def test_cache(self):
        lat, lon = cell_center(3, 3)
        write_resopsus(
            self.data, [(1, "A", lat, lon)], {1: make_record("2000-01-01", 10)}
        )
        first = self.add()
        self.assertFalse(first["from_cache"])
        second = self.add()
        self.assertTrue(second["from_cache"])
        self.assertEqual(first["placed"], second["placed"])

        # The cached keys are still set on a fresh Run
        run = make_run()
        self.add(run)
        self.assertIn("ResOpsUS_1", run.Reservoirs.Names)

        # Changing an input rebuilds
        self.assertFalse(self.add(gap_policy="skip")["from_cache"])
        # A missing release file rebuilds
        os.remove(
            os.path.join(self.run_dir, second["placed"][0]["keys"]["Release_File"])
        )
        self.assertFalse(self.add(gap_policy="skip")["from_cache"])

    def test_active_mask(self):
        write_resopsus(
            self.data,
            [
                (1, "Intake inactive", *cell_center(1, 1)),
                (2, "Release inactive", *cell_center(5, 5)),
                (3, "Fine", *cell_center(2, 8)),
            ],
            {d: make_record("2000-01-01", 10) for d in (1, 2, 3)},
        )
        mask = np.ones((NY, NX))
        mask[1, 1] = 0  # intake of dam 1
        mask[5, 6] = 0  # release of dam 2 (slopes drain east)
        summary = self.add(active_mask=mask)
        self.assertEqual([r["dam_id"] for r in summary["placed"]], ["3"])
        self.assertEqual(summary["skipped"]["1"], "intake cell is not an active cell")
        self.assertEqual(summary["skipped"]["2"], "release cell is not an active cell")

    def test_snapping_skips_inactive_cells(self):
        write_resopsus(
            self.data,
            [(1, "A", *cell_center(4, 4))],
            {1: make_record("2000-01-01", 10)},
        )
        accumulation = np.ones((NY, NX))
        accumulation[5, 5] = 100.0  # biggest, but inactive
        accumulation[4, 6] = 50.0
        mask = np.ones((NY, NX))
        mask[5, 5] = 0
        summary = self.add(flow_accumulation=accumulation, active_mask=mask)
        self.assertEqual(summary["placed"][0]["intake_cell"], [6, 4])

    def test_negative_outflow_reported_and_clipped(self):
        record = make_record("2000-01-01", 10, outflow=10.0)
        record["outflow"][1] = -5.0  # 2 January, the first day of the run
        write_resopsus(self.data, [(1, "Pumped", *cell_center(3, 3))], {1: record})
        summary = self.add()
        placed = summary["placed"][0]
        self.assertEqual(placed["negative_outflow_days"], 1)
        release = read_release(self.run_dir, placed)
        self.assertEqual(release["release_rate"].iloc[0], 0.0)
        self.assertEqual(release["release_rate"].iloc[1], 36000.0)

    def test_daily_record_has_unique_dates(self):
        record = make_record("2000-01-01", 5)
        write_resopsus(self.data, [(1, "A", *cell_center(3, 3))], {1: record})
        path = os.path.join(self.data, "time_series_all", "ResOpsUS_1.csv")
        with open(path, "a") as f:
            f.write("date,NA,NA,NA,NA,NA\n2000-01-02,7.0,NA,99.0,NA,NA\n")
        daily = ResOpsUS(self.data).daily_record("1")
        self.assertTrue(daily.index.is_unique)
        self.assertEqual(len(daily), 5)
        # The first value for a repeated date is kept
        self.assertEqual(daily.loc["2000-01-02", "outflow"], 10.0)

    def test_shared_cell_is_skipped(self):
        lat, lon = cell_center(3, 3)
        write_resopsus(
            self.data,
            [(1, "A", lat, lon), (2, "B", lat, lon)],
            {
                1: make_record("2000-01-01", 10),
                2: make_record("2000-01-01", 10),
            },
        )
        summary = self.add()
        self.assertEqual(len(summary["placed"]), 1)
        self.assertIn("shares a cell", summary["skipped"]["2"])


@unittest.skipUnless(HAVE_PYPROJ, "pyproj is not installed")
class TestProjectedGridPresets(unittest.TestCase):
    """Check the CONUS presets against reference points from hf_hydrodata's
    own grid tests (tests/hf_hydrodata/test_grid.py), which map between
    latitude/longitude and the same grids."""

    def test_conus1(self):
        grid = ProjectedGrid.conus1()
        lat, lon = grid.cell_to_lat_lon(10.0, 10.0)
        self.assertAlmostEqual(float(lat), 31.759219, places=6)
        self.assertAlmostEqual(float(lon), -115.902573, places=6)
        lat, lon = grid.cell_to_lat_lon(10.5, 10.5)
        self.assertAlmostEqual(float(lat), 31.764588, places=6)
        self.assertAlmostEqual(float(lon), -115.898577, places=6)
        x, y = grid.lat_lon_to_cell(49.1423, -76.3369)
        self.assertEqual((round(float(x)), round(float(y))), (3324, 1888))

    def test_conus2(self):
        grid = ProjectedGrid.conus2()
        lat, lon = grid.cell_to_lat_lon(0, 0)
        self.assertEqual((round(float(lat), 2), round(float(lon), 2)), (22.36, -117.85))
        x, y = grid.lat_lon_to_cell(31.65, -115.98)
        self.assertEqual((round(float(x)), round(float(y))), (441, 970))

    def test_round_trip(self):
        grid = ProjectedGrid.conus2()
        x, y = grid.lat_lon_to_cell(39.0, -105.0)
        lat, lon = grid.cell_to_lat_lon(x, y)
        self.assertAlmostEqual(float(lat), 39.0, places=8)
        self.assertAlmostEqual(float(lon), -105.0, places=8)


if __name__ == "__main__":
    unittest.main()
