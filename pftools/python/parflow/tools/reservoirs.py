"""reservoirs module

Build ParFlow reservoir inputs from existing reservoir databases.

ParFlow reads reservoir releases from its own simple CSV format (see the
``Reservoirs.<name>.Release_File`` key). This module converts external
datasets into that format and sets the matching ``Reservoirs.*`` keys on a
``Run``, so ParFlow itself never needs to know about any particular dataset.

Typical use, from a model script::

    from datetime import datetime
    from parflow.tools.reservoirs import ResOpsUS, ProjectedGrid

    ResOpsUS("~/data/ResOpsUS").add_to_run(
        run,
        run_dir=run_dir,
        start_date=datetime(2002, 10, 1),
        time_unit="hours",
        grid=ProjectedGrid.conus2(),
        domain_offset=(i0, j0),
        overland_flow_solver="OverlandKinematic",
        slope_x=slope_x,
        slope_y=slope_y,
    )

The release files are written under ``run_dir`` and reused on later calls
with the same inputs, so the model script can call this every time it runs.

Adding another dataset means writing a ``ReservoirDataset`` subclass that
returns reservoir locations and daily outflow and storage in SI units; the
placement, unit conversion, gap handling, caching and key setting are shared.
"""

import csv
import hashlib
import json
import math
import os
import re
from datetime import timedelta
from pathlib import Path

import numpy as np
import pandas as pd

# -----------------------------------------------------------------------------
# Units
# -----------------------------------------------------------------------------

TIME_UNIT_SECONDS = {
    "seconds": 1.0,
    "minutes": 60.0,
    "hours": 3600.0,
    "days": 86400.0,
}

LENGTH_UNIT_METERS = {
    "m": 1.0,
    "km": 1000.0,
}

GAP_POLICIES = ("interpolate", "skip", "fail")

RELEASE_DIRECTORY = "reservoir_releases"
MANIFEST_FILE = "manifest.json"
MANIFEST_VERSION = 1

# -----------------------------------------------------------------------------
# Grids: mapping latitude/longitude to grid cells
# -----------------------------------------------------------------------------


class ProjectedGrid:
    """A regular grid in a map projection, such as CONUS1 or CONUS2.

    Cell (i, j) covers projected coordinates
    ``[origin_x + i * resolution, origin_x + (i + 1) * resolution)`` and the
    same in y, which matches how hf_hydrodata maps latitude/longitude to grid
    cells.

    Requires the optional ``pyproj`` package.

    Args:
        proj (str): PROJ string or any CRS definition pyproj accepts.
        origin_x (float): projected x of the grid's lower-left corner.
        origin_y (float): projected y of the grid's lower-left corner.
        resolution (float): cell size in projected units.
        nx (int): optional number of cells in x, used to check bounds.
        ny (int): optional number of cells in y, used to check bounds.
        name (str): optional name used in messages and the cache manifest.
    """

    def __init__(
        self, proj, origin_x, origin_y, resolution, nx=None, ny=None, name=None
    ):
        self.proj = proj
        self.origin_x = float(origin_x)
        self.origin_y = float(origin_y)
        self.resolution = float(resolution)
        self.nx = nx
        self.ny = ny
        self.name = name or "projected"
        self._transformer = None

    # Grid definitions from the hf_hydrodata grid table
    # (https://hf-hydrodata.readthedocs.io/en/latest/available_grids.html)
    @classmethod
    def conus1(cls):
        return cls(
            "+proj=lcc +lat_1=33 +lat_2=45 +lon_0=-96.0 +lat_0=39 "
            "+a=6378137.0 +b=6356752.31",
            origin_x=-1885055.4995,
            origin_y=-604957.0654,
            resolution=1000.0,
            nx=3342,
            ny=1888,
            name="conus1",
        )

    @classmethod
    def conus2(cls):
        return cls(
            "+proj=lcc +lat_1=30 +lat_2=60 +lon_0=-97.0 "
            "+lat_0=40.0000076294444 +a=6370000.0 +b=6370000",
            origin_x=-2208000.30881173,
            origin_y=-1668999.65483222,
            resolution=1000.0,
            nx=4442,
            ny=3256,
            name="conus2",
        )

    def _get_transformer(self):
        if self._transformer is None:
            try:
                import pyproj
            except ImportError as error:
                raise ImportError(
                    "ProjectedGrid needs the pyproj package "
                    "(pip install pyproj, or pip install pftools[reservoirs])"
                ) from error
            self._transformer = pyproj.Transformer.from_crs(
                "EPSG:4326", pyproj.CRS.from_user_input(self.proj), always_xy=True
            )
        return self._transformer

    def lat_lon_to_cell(self, lat, lon):
        """Return fractional cell coordinates (x, y); floor gives the index."""
        px, py = self._get_transformer().transform(lon, lat)
        return (
            (np.asarray(px) - self.origin_x) / self.resolution,
            (np.asarray(py) - self.origin_y) / self.resolution,
        )

    def cell_to_lat_lon(self, x, y):
        """Inverse of lat_lon_to_cell for fractional cell coordinates."""
        px = self.origin_x + np.asarray(x, dtype=float) * self.resolution
        py = self.origin_y + np.asarray(y, dtype=float) * self.resolution
        lon, lat = self._get_transformer().transform(px, py, direction="INVERSE")
        return lat, lon

    def describe(self):
        return {
            "type": "ProjectedGrid",
            "name": self.name,
            "proj": self.proj,
            "origin_x": self.origin_x,
            "origin_y": self.origin_y,
            "resolution": self.resolution,
        }


class LatLonGrid:
    """A grid that is regular in latitude and longitude.

    Only use this when the model grid really is regular in latitude and
    longitude; most large domains use a projected grid instead.

    Args:
        lat_min, lat_max, lon_min, lon_max (float): edges of the grid.
        nx, ny (int): number of cells. If omitted, the model grid's NX and NY
            are used, i.e. the edges are those of the model domain.
    """

    def __init__(self, lat_min, lat_max, lon_min, lon_max, nx=None, ny=None):
        if lat_max <= lat_min or lon_max <= lon_min:
            raise ValueError("LatLonGrid edges must satisfy min < max")
        self.lat_min = float(lat_min)
        self.lat_max = float(lat_max)
        self.lon_min = float(lon_min)
        self.lon_max = float(lon_max)
        self.nx = nx
        self.ny = ny
        self.name = "latlon"

    def _shape(self):
        if self.nx is None or self.ny is None:
            raise ValueError("LatLonGrid needs nx and ny")
        return self.nx, self.ny

    def lat_lon_to_cell(self, lat, lon):
        nx, ny = self._shape()
        x = (np.asarray(lon, dtype=float) - self.lon_min) / (
            self.lon_max - self.lon_min
        )
        y = (np.asarray(lat, dtype=float) - self.lat_min) / (
            self.lat_max - self.lat_min
        )
        return x * nx, y * ny

    def cell_to_lat_lon(self, x, y):
        nx, ny = self._shape()
        lon = self.lon_min + np.asarray(x, dtype=float) / nx * (
            self.lon_max - self.lon_min
        )
        lat = self.lat_min + np.asarray(y, dtype=float) / ny * (
            self.lat_max - self.lat_min
        )
        return lat, lon

    def describe(self):
        return {
            "type": "LatLonGrid",
            "lat_min": self.lat_min,
            "lat_max": self.lat_max,
            "lon_min": self.lon_min,
            "lon_max": self.lon_max,
            "nx": self.nx,
            "ny": self.ny,
        }


# -----------------------------------------------------------------------------
# Placement corrections
# -----------------------------------------------------------------------------


def load_corrections(*paths):
    """Load placement corrections from CSV files.

    Each file has a ``dam_id`` column and any of ``intake_lat``,
    ``intake_lon``, ``release_lat``, ``release_lon``. Blank values mean "no
    correction" for that field. Other columns (for example notes on where a
    correction came from) are kept but not used. Later files override
    earlier ones field by field.

    Returns:
        dict: dam_id (str) -> dict of corrected fields (floats).
    """
    corrections = {}
    location_fields = ("intake_lat", "intake_lon", "release_lat", "release_lon")
    for path in paths:
        if path is None:
            continue
        with open(path, newline="") as f:
            for row in csv.DictReader(
                line for line in f if not line.lstrip().startswith("#")
            ):
                dam_id = str(row.get("dam_id", "")).strip()
                if not dam_id:
                    continue
                entry = corrections.setdefault(dam_id, {})
                for field in location_fields:
                    value = (row.get(field) or "").strip()
                    if value:
                        entry[field] = float(value)
                for pair in (
                    ("intake_lat", "intake_lon"),
                    ("release_lat", "release_lon"),
                ):
                    if (pair[0] in entry) != (pair[1] in entry):
                        raise ValueError(
                            f"Correction for dam {dam_id} in {path} sets only one "
                            f"of {pair[0]} and {pair[1]}"
                        )
    return corrections


# -----------------------------------------------------------------------------
# Datasets
# -----------------------------------------------------------------------------


class ReservoirDataset:
    """Base class for an external reservoir dataset.

    Subclasses provide reservoir locations and daily records in SI units;
    everything else is shared. To support a new dataset, implement
    ``reservoirs`` and ``daily_record`` and set ``name``.
    """

    #: Short name, used for reservoir key names and the cache manifest
    name = "dataset"

    #: Corrections shipped with pftools for this dataset (or None)
    default_corrections_file = None

    def reservoirs(self):
        """Return a DataFrame with columns dam_id (str), dam_name, lat, lon."""
        raise NotImplementedError

    def daily_record(self, dam_id):
        """Return a DataFrame indexed by date (midnight timestamps) with
        columns ``outflow`` [m^3/s] and ``storage`` [m^3]; missing values NaN.
        """
        raise NotImplementedError

    def identity(self):
        """Values identifying this dataset in the cache manifest."""
        return {"dataset": self.name}

    def add_to_run(self, run, **kwargs):
        """Add this dataset's reservoirs to a Run.

        See ``add_reservoirs_to_run`` for the arguments.
        """
        return add_reservoirs_to_run(run, self, **kwargs)


class ResOpsUS(ReservoirDataset):
    """The ResOpsUS dataset of historical US reservoir operations.

    Steyaert, J.C., Condon, L.E., Turner, S.W.D., Voisin, N. (2022), ResOpsUS,
    a dataset of historical reservoir operations in the contiguous United
    States, Scientific Data 9, https://doi.org/10.1038/s41597-022-01134-7.
    Data: https://doi.org/10.5281/zenodo.5367383 (CC BY 4.0).

    Args:
        path (str): the ResOpsUS folder, containing ``attributes`` and
            ``time_series_all``.

    ResOpsUS does not include reservoir capacities, so by default
    ``Max_Storage`` is the largest storage in each reservoir's record; pass
    ``capacities`` to ``add_to_run`` to use other values (e.g. from GRanD).
    """

    name = "ResOpsUS"
    default_corrections_file = (
        Path(__file__).parent / "ref" / "reservoir_corrections_resopsus.csv"
    )

    MCM_TO_M3 = 1.0e6

    def __init__(self, path):
        self.path = Path(os.path.expanduser(str(path)))

    def _attributes_file(self):
        return self.path / "attributes" / "reservoir_attributes.csv"

    def reservoirs(self):
        attributes_file = self._attributes_file()
        if not attributes_file.exists():
            raise FileNotFoundError(
                f"ResOpsUS attributes not found at {attributes_file}; "
                "path should be the ResOpsUS folder"
            )
        table = pd.read_csv(attributes_file, encoding="utf-8-sig")
        return pd.DataFrame(
            {
                "dam_id": table["DAM_ID"].astype(str),
                "dam_name": table["DAM_NAME"].astype(str),
                "lat": table["LAT"].astype(float),
                "lon": table["LONG"].astype(float),
            }
        )

    def daily_record(self, dam_id):
        record_file = self.path / "time_series_all" / f"ResOpsUS_{dam_id}.csv"
        if not record_file.exists():
            return pd.DataFrame(
                {"outflow": [], "storage": []}, index=pd.DatetimeIndex([])
            )
        table = pd.read_csv(
            record_file, na_values=["NA", ""], encoding="utf-8-sig", dtype=str
        )
        # A few files contain stray rows (e.g. a repeated header); drop rows
        # whose date does not parse, and treat unparseable values as missing
        index = pd.to_datetime(table["date"], format="%Y-%m-%d", errors="coerce")
        record = pd.DataFrame(
            {
                "outflow": pd.to_numeric(table["outflow"], errors="coerce").to_numpy(),
                "storage": pd.to_numeric(table["storage"], errors="coerce").to_numpy()
                * self.MCM_TO_M3,
            },
            index=index,
        )
        return record[record.index.notna()].sort_index()

    def identity(self):
        return {"dataset": self.name, "path": str(self.path.resolve())}


# -----------------------------------------------------------------------------
# Building the run inputs
# -----------------------------------------------------------------------------


def _cell_outlet(i, j, slope_x, slope_y, overland_flow_solver):
    """Return the cell (i, j) drains to, following the slope convention of
    the overland flow solver, or None if it has no downhill neighbour."""
    ny, nx = slope_x.shape
    candidates = []
    if overland_flow_solver == "OverlandKinematic":
        # Face slopes are stored in the cell on the lower side of the face
        if i < nx - 1 and slope_x[j, i] < 0:
            candidates.append((-slope_x[j, i], (i + 1, j)))
        if i > 0 and slope_x[j, i - 1] > 0:
            candidates.append((slope_x[j, i - 1], (i - 1, j)))
        if j < ny - 1 and slope_y[j, i] < 0:
            candidates.append((-slope_y[j, i], (i, j + 1)))
        if j > 0 and slope_y[j - 1, i] > 0:
            candidates.append((slope_y[j - 1, i], (i, j - 1)))
    else:
        # Cell-centred slopes: water moves down the steeper of the x and y
        # slopes, away from the side the slope rises towards
        sx = slope_x[j, i]
        sy = slope_y[j, i]
        if abs(sx) >= abs(sy) and sx != 0:
            target = (i - int(np.sign(sx)), j)
            candidates.append((abs(sx), target))
        elif sy != 0:
            target = (i, j - int(np.sign(sy)))
            candidates.append((abs(sy), target))
    candidates = [c for c in candidates if 0 <= c[1][0] < nx and 0 <= c[1][1] < ny]
    if not candidates:
        return None
    return max(candidates, key=lambda c: c[0])[1]


def _snap_to_channel(i, j, flow_accumulation, radius):
    """Move (i, j) to the cell with the most flow accumulation within radius.

    The dam's own cell is kept unless another cell has strictly more
    accumulation; among equal maxima the one nearest the dam is used.
    """
    ny, nx = flow_accumulation.shape
    i_lo, i_hi = max(0, i - radius), min(nx, i + radius + 1)
    j_lo, j_hi = max(0, j - radius), min(ny, j + radius + 1)
    window = flow_accumulation[j_lo:j_hi, i_lo:i_hi]
    best = np.nanmax(window)
    if not best > flow_accumulation[j, i]:
        return i, j
    jj, ii = np.nonzero(window == best)
    distance = (i_lo + ii - i) ** 2 + (j_lo + jj - j) ** 2
    k = int(np.argmin(distance))
    return int(i_lo + ii[k]), int(j_lo + jj[k])


def _longest_gap(series):
    """Length of the longest run of missing values in a series."""
    longest = current = 0
    for missing in series.isna().to_numpy():
        current = current + 1 if missing else 0
        longest = max(longest, current)
    return longest


def _fill_gaps(series, policy, max_fill_days):
    """Apply the gap policy to a daily series. Returns (series, n_filled) or
    raises ValueError if the gaps cannot be handled."""
    missing = int(series.isna().sum())
    if missing == 0:
        return series, 0
    if policy == "fail":
        raise ValueError(f"{missing} missing daily values")
    if policy == "skip":
        raise ValueError(f"{missing} missing daily values (gap_policy='skip')")
    if series.notna().sum() == 0:
        raise ValueError("no data in the simulation period")
    longest = _longest_gap(series)
    if longest > max_fill_days:
        raise ValueError(
            f"a gap of {longest} days is longer than max_fill_days={max_fill_days}"
        )
    filled = series.interpolate(method="time", limit_direction="both")
    return filled, missing


def _array_hash(array):
    if array is None:
        return None
    array = np.ascontiguousarray(np.asarray(array, dtype=float))
    return hashlib.sha256(array.tobytes() + str(array.shape).encode()).hexdigest()


def _file_hash(path):
    if path is None or not Path(path).exists():
        return None
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def _run_value(run, *names):
    obj = run
    for name in names:
        obj = getattr(obj, name)
    return obj


def add_reservoirs_to_run(
    run,
    dataset,
    run_dir,
    start_date,
    time_unit,
    grid,
    overland_flow_solver,
    domain_offset=(0, 0),
    length_unit="m",
    slope_x=None,
    slope_y=None,
    flow_accumulation=None,
    snap_radius=2,
    corrections_file=None,
    use_default_corrections=True,
    dam_ids=None,
    capacities=None,
    min_release_storage_fraction=0.0,
    gap_policy="interpolate",
    max_fill_days=30,
    overwrite=False,
    verbose=True,
):
    """Add the reservoirs from a dataset to a Run.

    Writes one release time series per reservoir under
    ``<run_dir>/reservoir_releases`` and sets the ``Reservoirs.*`` keys.
    Reservoirs whose dam lies outside the model domain are left out. If a
    previous call with the same inputs already wrote the files, they are
    reused and only the keys are set.

    Args:
        run (Run): the run to add reservoirs to. Its ComputationalGrid
            (Lower.X/Y, DX, DY, NX, NY) and TimingInfo (StartTime, StopTime)
            must already be set.
        dataset (ReservoirDataset): e.g. ``ResOpsUS(path)``.
        run_dir (str): the directory ParFlow will run in.
        start_date (datetime): the date and time that model time
            ``TimingInfo.StartTime`` corresponds to.
        time_unit (str): the model's time unit: seconds, minutes, hours or days.
        grid (ProjectedGrid or LatLonGrid): how latitude/longitude maps to
            grid cells.
        overland_flow_solver (str): OverlandFlow or OverlandKinematic; sets
            Reservoirs.Overland_Flow_Solver and the slope convention used to
            find release cells.
        domain_offset (tuple): (i, j) of the model's first cell within
            ``grid``, for a model domain that is a subset of a larger grid.
        length_unit (str): the model's length unit, m or km.
        slope_x, slope_y (ndarray): 2D (ny, nx) slopes of the model domain,
            e.g. ``read_pfb(file)[0]``. Used to put each release in the cell
            downstream of its intake. Needed unless every reservoir has a
            release correction.
        flow_accumulation (ndarray): optional 2D (ny, nx) flow accumulation
            of the model domain. If given, intakes are moved to the cell with
            the most accumulation within ``snap_radius`` cells of the dam.
        snap_radius (int): search radius in cells for snapping.
        corrections_file (str): optional CSV of placement corrections (see
            ``load_corrections``); overrides the corrections shipped with
            pftools for the dataset.
        use_default_corrections (bool): apply the corrections shipped with
            pftools for this dataset.
        dam_ids (list): optional subset of dam IDs to include.
        capacities (dict): optional dam_id -> capacity [m^3] used for
            Max_Storage. Defaults to the largest storage in each record.
        min_release_storage_fraction (float): Min_Release_Storage as a
            fraction of Max_Storage.
        gap_policy (str): what to do with missing daily values in the
            simulation period: ``interpolate`` (fill linearly in time and
            report it), ``skip`` (leave the reservoir out) or ``fail``.
        max_fill_days (int): with ``interpolate``, the longest run of
            missing days that is filled, for outflow in the simulation period
            and for the storage on the start day; reservoirs with a longer
            gap are left out.
        overwrite (bool): rebuild even if cached files match the inputs.
        verbose (bool): print a summary.

    Returns:
        dict: summary with ``placed`` (list of dicts) and ``skipped``
        (dam_id -> reason).
    """
    if time_unit not in TIME_UNIT_SECONDS:
        raise ValueError(f"time_unit must be one of {list(TIME_UNIT_SECONDS)}")
    if length_unit not in LENGTH_UNIT_METERS:
        raise ValueError(f"length_unit must be one of {list(LENGTH_UNIT_METERS)}")
    if gap_policy not in GAP_POLICIES:
        raise ValueError(f"gap_policy must be one of {list(GAP_POLICIES)}")
    if overland_flow_solver not in ("OverlandFlow", "OverlandKinematic"):
        raise ValueError(
            "overland_flow_solver must be OverlandFlow or OverlandKinematic"
        )

    try:
        nx = int(_run_value(run, "ComputationalGrid", "NX"))
        ny = int(_run_value(run, "ComputationalGrid", "NY"))
        dx = float(_run_value(run, "ComputationalGrid", "DX"))
        dy = float(_run_value(run, "ComputationalGrid", "DY"))
        lower_x = float(_run_value(run, "ComputationalGrid", "Lower", "X"))
        lower_y = float(_run_value(run, "ComputationalGrid", "Lower", "Y"))
        start_time = float(_run_value(run, "TimingInfo", "StartTime"))
        stop_time = float(_run_value(run, "TimingInfo", "StopTime"))
    except (TypeError, ValueError) as error:
        raise ValueError(
            "Set ComputationalGrid (Lower.X/Y, DX, DY, NX, NY) and TimingInfo "
            "(StartTime, StopTime) on the run before adding reservoirs"
        ) from error

    if isinstance(grid, LatLonGrid) and (grid.nx is None or grid.ny is None):
        grid = LatLonGrid(
            grid.lat_min, grid.lat_max, grid.lon_min, grid.lon_max, nx, ny
        )

    for label, array in (
        ("slope_x", slope_x),
        ("slope_y", slope_y),
        ("flow_accumulation", flow_accumulation),
    ):
        if array is not None:
            array = np.squeeze(np.asarray(array))
            if array.shape != (ny, nx):
                raise ValueError(
                    f"{label} has shape {array.shape}, expected (ny, nx) = {(ny, nx)}"
                )
    slope_x = None if slope_x is None else np.squeeze(np.asarray(slope_x, float))
    slope_y = None if slope_y is None else np.squeeze(np.asarray(slope_y, float))
    if flow_accumulation is not None:
        flow_accumulation = np.squeeze(np.asarray(flow_accumulation, float))
    if (slope_x is None) != (slope_y is None):
        raise ValueError("Give both slope_x and slope_y, or neither")

    correction_files = []
    if use_default_corrections and dataset.default_corrections_file is not None:
        correction_files.append(dataset.default_corrections_file)
    if corrections_file is not None:
        correction_files.append(Path(os.path.expanduser(str(corrections_file))))

    run_dir = Path(os.path.expanduser(str(run_dir)))
    release_dir = run_dir / RELEASE_DIRECTORY
    manifest_path = release_dir / MANIFEST_FILE

    inputs = {
        "manifest_version": MANIFEST_VERSION,
        "dataset": dataset.identity(),
        "start_date": start_date.isoformat(),
        "time_unit": time_unit,
        "length_unit": length_unit,
        "grid": grid.describe(),
        "domain_offset": list(domain_offset),
        "model_grid": [nx, ny, dx, dy, lower_x, lower_y],
        "timing": [start_time, stop_time],
        "overland_flow_solver": overland_flow_solver,
        "slopes": [_array_hash(slope_x), _array_hash(slope_y)],
        "flow_accumulation": _array_hash(flow_accumulation),
        "snap_radius": snap_radius,
        "corrections": [_file_hash(p) for p in correction_files],
        "dam_ids": None if dam_ids is None else sorted(str(d) for d in dam_ids),
        "capacities": (
            None
            if capacities is None
            else {str(k): float(v) for k, v in sorted(capacities.items())}
        ),
        "min_release_storage_fraction": min_release_storage_fraction,
        "gap_policy": gap_policy,
        "max_fill_days": max_fill_days,
    }

    summary = None
    if not overwrite and manifest_path.exists():
        try:
            manifest = json.loads(manifest_path.read_text())
        except ValueError:
            manifest = None
        if manifest is not None and manifest.get("inputs") == inputs:
            files_present = all(
                (run_dir / r["keys"]["Release_File"]).exists()
                for r in manifest["summary"]["placed"]
            )
            if files_present:
                summary = manifest["summary"]
                summary["from_cache"] = True

    if summary is None:
        summary = _build(
            dataset=dataset,
            run_dir=run_dir,
            release_dir=release_dir,
            start_date=start_date,
            start_time=start_time,
            stop_time=stop_time,
            unit_seconds=TIME_UNIT_SECONDS[time_unit],
            length_m=LENGTH_UNIT_METERS[length_unit],
            grid=grid,
            domain_offset=domain_offset,
            model_grid=(nx, ny, dx, dy, lower_x, lower_y),
            overland_flow_solver=overland_flow_solver,
            slope_x=slope_x,
            slope_y=slope_y,
            flow_accumulation=flow_accumulation,
            snap_radius=snap_radius,
            corrections=load_corrections(*correction_files),
            dam_ids=dam_ids,
            capacities=capacities,
            min_release_storage_fraction=min_release_storage_fraction,
            gap_policy=gap_policy,
            max_fill_days=max_fill_days,
        )
        release_dir.mkdir(parents=True, exist_ok=True)
        manifest_path.write_text(
            json.dumps({"inputs": inputs, "summary": summary}, indent=2)
        )
        summary["from_cache"] = False

    _set_keys(run, summary["placed"], overland_flow_solver)

    if verbose:
        _print_summary(dataset, summary)
    return summary


def _build(
    dataset,
    run_dir,
    release_dir,
    start_date,
    start_time,
    stop_time,
    unit_seconds,
    length_m,
    grid,
    domain_offset,
    model_grid,
    overland_flow_solver,
    slope_x,
    slope_y,
    flow_accumulation,
    snap_radius,
    corrections,
    dam_ids,
    capacities,
    min_release_storage_fraction,
    gap_policy,
    max_fill_days,
):
    nx, ny, dx, dy, lower_x, lower_y = model_grid
    i0, j0 = domain_offset
    rate_scale = unit_seconds / length_m**3  # m^3/s -> L^3/T
    volume_scale = 1.0 / length_m**3  # m^3 -> L^3

    # Days whose daily mean applies at some point in the simulation
    end_date = start_date + timedelta(seconds=(stop_time - start_time) * unit_seconds)
    first_day = pd.Timestamp(start_date).normalize()
    last_day = pd.Timestamp(end_date).normalize()
    if pd.Timestamp(end_date) == last_day and last_day > first_day:
        last_day -= pd.Timedelta(days=1)
    days = pd.date_range(first_day, last_day, freq="D")

    def model_time(timestamp):
        seconds = (timestamp - pd.Timestamp(start_date)).total_seconds()
        return start_time + seconds / unit_seconds

    def to_model_cell(lat, lon):
        x, y = grid.lat_lon_to_cell(lat, lon)
        return int(math.floor(float(x))) - i0, int(math.floor(float(y))) - j0

    def inside(i, j):
        return 0 <= i < nx and 0 <= j < ny

    table = dataset.reservoirs()
    if dam_ids is not None:
        wanted = {str(d) for d in dam_ids}
        table = table[table["dam_id"].isin(wanted)]

    placed = []
    skipped = {}
    occupied = {}
    release_dir.mkdir(parents=True, exist_ok=True)
    for old_file in release_dir.glob("*.csv"):
        old_file.unlink()

    for row in table.itertuples(index=False):
        dam_id = str(row.dam_id)
        correction = corrections.get(dam_id, {})

        # Intake: corrected location, or the dam snapped to the channel
        if "intake_lat" in correction:
            i, j = to_model_cell(correction["intake_lat"], correction["intake_lon"])
            intake_source = "correction"
        else:
            i, j = to_model_cell(row.lat, row.lon)
            intake_source = "dam location"
            if inside(i, j) and flow_accumulation is not None and snap_radius > 0:
                i, j = _snap_to_channel(i, j, flow_accumulation, snap_radius)
                intake_source = "snapped to channel"
        if not inside(i, j):
            skipped[dam_id] = "outside the model domain"
            continue

        # Release: corrected location, or the cell the intake drains to
        if "release_lat" in correction:
            ri, rj = to_model_cell(correction["release_lat"], correction["release_lon"])
        elif slope_x is not None:
            outlet = _cell_outlet(i, j, slope_x, slope_y, overland_flow_solver)
            if outlet is None:
                skipped[dam_id] = "intake cell has no downhill neighbour"
                continue
            ri, rj = outlet
        else:
            skipped[dam_id] = "no slopes or release correction to place the release"
            continue
        if not inside(ri, rj):
            skipped[dam_id] = "release cell outside the model domain"
            continue
        if (ri, rj) == (i, j):
            skipped[dam_id] = "release cell is the intake cell"
            continue
        clash = [occupied.get(c) for c in ((i, j), (ri, rj)) if c in occupied]
        if clash:
            skipped[dam_id] = f"shares a cell with reservoir {clash[0]}"
            continue

        # Records
        record = dataset.daily_record(dam_id)
        record = record[~record.index.duplicated(keep="first")]
        if record["outflow"].notna().sum() == 0:
            skipped[dam_id] = "no outflow record"
            continue
        if record["storage"].notna().sum() == 0:
            skipped[dam_id] = "no storage record"
            continue
        window = record.reindex(days)
        try:
            outflow, outflow_filled = _fill_gaps(
                window["outflow"], gap_policy, max_fill_days
            )
        except ValueError as error:
            if gap_policy == "fail":
                raise ValueError(f"Reservoir {dam_id} outflow: {error}") from error
            skipped[dam_id] = f"outflow: {error}"
            continue
        # Initial storage: the start day's value, filled from the record
        storage_series = record["storage"]
        if first_day in storage_series.index and not math.isnan(
            storage_series.loc[first_day]
        ):
            storage = float(storage_series.loc[first_day])
            storage_filled = False
        elif gap_policy == "interpolate":
            valid = storage_series.dropna()
            before = valid[valid.index < first_day]
            after = valid[valid.index > first_day]
            # Days spanned by the gap the start day falls in
            gap_start = before.index[-1] if len(before) else None
            gap_end = after.index[0] if len(after) else None
            if gap_start is not None and gap_end is not None:
                gap_days = (gap_end - gap_start).days - 1
            else:
                nearest = gap_start if gap_start is not None else gap_end
                gap_days = abs((first_day - nearest).days)
            if gap_days > max_fill_days:
                skipped[dam_id] = (
                    f"storage: a gap of {gap_days} days around the start is "
                    f"longer than max_fill_days={max_fill_days}"
                )
                continue
            filled = storage_series.reindex(
                storage_series.index.union([first_day])
            ).interpolate(method="time", limit_direction="both")
            storage = float(filled.loc[first_day])
            storage_filled = True
        elif gap_policy == "fail":
            raise ValueError(f"Reservoir {dam_id}: no storage on {first_day.date()}")
        else:
            skipped[dam_id] = f"no storage on {first_day.date()}"
            continue

        if capacities is not None and dam_id in {str(k) for k in capacities}:
            capacity = float({str(k): v for k, v in capacities.items()}[dam_id])
            capacity_source = "capacities"
        else:
            capacity = float(storage_series.max())
            capacity_source = "record maximum"

        name = f"{dataset.name}_{dam_id}"
        release_file = Path(RELEASE_DIRECTORY) / f"{name}.csv"
        with open(run_dir / release_file, "w") as f:
            f.write(
                f"# {dataset.name} dam {dam_id} ({row.dam_name}); daily mean outflow "
                f"from {start_date.isoformat()}\n"
            )
            f.write("time,release_rate\n")
            for day, value in zip(days, outflow.to_numpy()):
                f.write(
                    f"{model_time(day)!r},{max(float(value), 0.0) * rate_scale!r}\n"
                )

        occupied[(i, j)] = dam_id
        occupied[(ri, rj)] = dam_id
        max_storage = capacity * volume_scale
        placed.append(
            {
                "name": name,
                "dam_id": dam_id,
                "dam_name": str(row.dam_name),
                "intake_cell": [i, j],
                "release_cell": [ri, rj],
                "intake_source": intake_source,
                "capacity_source": capacity_source,
                "outflow_days_filled": outflow_filled,
                "storage_filled": storage_filled,
                "keys": {
                    "Intake_X": lower_x + (i + 0.5) * dx,
                    "Intake_Y": lower_y + (j + 0.5) * dy,
                    "Release_X": lower_x + (ri + 0.5) * dx,
                    "Release_Y": lower_y + (rj + 0.5) * dy,
                    "Has_Secondary_Intake_Cell": 0,
                    "Secondary_Intake_X": -1.0,
                    "Secondary_Intake_Y": -1.0,
                    "Max_Storage": max_storage,
                    "Storage": storage * volume_scale,
                    "Min_Release_Storage": max_storage * min_release_storage_fraction,
                    "Release_Type": "TimeSeries",
                    "Release_File": str(release_file),
                    "Release_Interpolation": "Step",
                },
            }
        )

    return {"placed": placed, "skipped": skipped}


def _set_keys(run, placed, overland_flow_solver):
    if not placed:
        return
    names = run.Reservoirs.Names
    if not names:
        names = []
    elif isinstance(names, str):
        names = names.split()
    else:
        names = list(names)
    for reservoir in placed:
        if reservoir["name"] not in names:
            names.append(reservoir["name"])
    run.Reservoirs.Names = names
    run.Reservoirs.Overland_Flow_Solver = overland_flow_solver
    for reservoir in placed:
        item = run.Reservoirs[reservoir["name"]]
        for key, value in reservoir["keys"].items():
            setattr(item, key, value)


def _print_summary(dataset, summary):
    placed = summary["placed"]
    skipped = summary["skipped"]
    source = "cached files" if summary.get("from_cache") else "dataset"
    print(
        f"{dataset.name}: added {len(placed)} reservoirs from {source}, "
        f"left out {len(skipped)}"
    )
    filled = [r for r in placed if r["outflow_days_filled"] or r["storage_filled"]]
    for r in filled:
        print(
            f"  {r['name']}: filled {r['outflow_days_filled']} days of outflow"
            + (", and the initial storage," if r["storage_filled"] else "")
            + " by interpolation"
        )
    # Group reasons that differ only in their numbers (e.g. days missing)
    reasons = {}
    for reason in skipped.values():
        general = re.sub(r"\d+", "N", reason)
        reasons[general] = reasons.get(general, 0) + 1
    for reason, count in sorted(reasons.items(), key=lambda x: -x[1]):
        print(f"  left out {count}: {reason}")
