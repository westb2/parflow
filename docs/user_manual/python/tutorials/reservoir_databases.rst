.. _reservoir_databases:

Reservoirs from Existing Databases
==================================

.. _reservoir_databases_intro:

Introduction
------------

ParFlow reads reservoir releases from its own simple CSV format (see
``Reservoirs.<name>.Release_Type`` and ``Release_File`` in :ref:`Reservoirs`).
The ``parflow.tools.reservoirs`` module converts existing reservoir datasets
into that format and sets the matching ``Reservoirs`` keys, so historical
simulations can use observed releases without writing the inputs by hand.
ParFlow itself never reads the dataset, and the run directory holds
everything the run needs.

Currently supported datasets:

- **ResOpsUS** (Steyaert et al., 2022): daily operations for about 680 large
  reservoirs in the contiguous US. Download it from
  `Zenodo <https://doi.org/10.5281/zenodo.5367383>`_ and pass the
  ``ResOpsUS`` folder (the one containing ``attributes`` and
  ``time_series_all``).

.. _reservoir_databases_usage:

Usage
-----

Call ``add_to_run`` in the model script after setting the computational grid
and timing information:

.. code-block:: python3

    from datetime import datetime
    from parflow import Run, read_pfb
    from parflow.tools.reservoirs import ResOpsUS, ProjectedGrid

    run = Run("my_run", __file__)
    # ... ComputationalGrid, TimingInfo and the rest of the model ...

    summary = ResOpsUS("~/data/ResOpsUS").add_to_run(
        run,
        run_dir=run_dir,                      # the directory ParFlow runs in
        start_date=datetime(2002, 10, 1),     # date of TimingInfo.StartTime
        time_unit="hours",                    # the model's time unit
        grid=ProjectedGrid.conus2(),
        domain_offset=(i0, j0),               # model cell (0, 0) within CONUS2
        overland_flow_solver="OverlandKinematic",
        slope_x=read_pfb("slope_x.pfb")[0],
        slope_y=read_pfb("slope_y.pfb")[0],
    )

    run.run(working_directory=run_dir)

This writes one release file per reservoir to
``<run_dir>/reservoir_releases`` and sets ``Reservoirs.Names`` and each
reservoir's keys. It prints a summary of the reservoirs added and of those
left out, with the reason; the same information is returned as a dictionary.

The release files are reused when the function is called again with the same
inputs, so it is fine to leave the call in the model script. A cached run
directory can be copied to another machine and run there without the
dataset. Pass ``overwrite=True`` to force a rebuild.

Placing reservoirs on the grid
------------------------------

The ``grid`` argument says how latitude and longitude map to grid cells:

- ``ProjectedGrid.conus1()`` and ``ProjectedGrid.conus2()``: the National
  ParFlow model grids, using the projections and origins published by
  hf_hydrodata. For a model domain that is part of one of these grids, set
  ``domain_offset`` to the (i, j) of the domain's first cell.
- ``ProjectedGrid(proj, origin_x, origin_y, resolution)``: any other
  projected grid, given a PROJ string and the projected coordinates of the
  lower-left corner.
- ``LatLonGrid(lat_min, lat_max, lon_min, lon_max)``: a grid that is regular
  in latitude and longitude, given its edges. Only use this when the model
  grid really is a latitude/longitude grid.

``ProjectedGrid`` needs the ``pyproj`` package (``pip install pyproj``).

Each reservoir's intake is the cell containing the dam. If a
``flow_accumulation`` array is given, the intake moves to the cell with the
most accumulation within ``snap_radius`` cells (2 by default), since mapped
dam locations often miss the modelled channel. The release is the cell the
intake drains to, found from ``slope_x`` and ``slope_y`` with the convention
of the chosen overland flow solver.

Dams outside the domain, and reservoirs whose release would fall outside the
domain or in a cell already used by another reservoir, are left out.

Placement corrections
~~~~~~~~~~~~~~~~~~~~~

Where the automatic placement is wrong, give corrected locations in a CSV
file:

.. code-block:: text

    dam_id,intake_lat,intake_lon,release_lat,release_lon,note
    610,36.0161,-114.7377,36.0127,-114.7425,moved onto the channel
    1234,,,40.1000,-105.2000,only the release needed moving

and pass it as ``corrections_file``. Locations are latitude and longitude,
so a correction works on any grid. A corrected intake is used as given
(it is not snapped). Leave a pair blank to keep the automatic placement;
other columns are ignored.

pftools also ships a corrections file for each dataset (for ResOpsUS,
``parflow/tools/ref/reservoir_corrections_resopsus.csv``), applied unless
``use_default_corrections=False``. A user file overrides it field by field.
Corrections that would help others can be contributed to the shipped file.

Units, times and gaps
---------------------

- Release rates are daily mean outflows converted to model units, using
  ``time_unit`` (seconds, minutes, hours or days) and ``length_unit``
  (``m`` by default). Each day's rate applies from the start of that day,
  using ``Release_Interpolation = Step``.
- ``start_date`` is the date and time of model time ``TimingInfo.StartTime``.
  Only the days covered by ``StartTime`` to ``StopTime`` are written.
- ``Storage`` is the observed storage on the start date. ResOpsUS has no
  capacity column, so ``Max_Storage`` is the largest storage in each
  record unless ``capacities`` (dam ID to cubic metres) is given.
  ``Min_Release_Storage`` is ``min_release_storage_fraction`` times
  ``Max_Storage`` (0 by default).
- Missing days are handled by ``gap_policy``: ``interpolate`` (the default)
  fills them linearly in time, ``skip`` leaves the reservoir out, and
  ``fail`` stops with an error. With ``interpolate``, gaps longer than
  ``max_fill_days`` (30 by default) are not filled and the reservoir is left
  out. The same limit applies to filling the initial storage.

.. note::

   Observed outflows already include any spills. If the modelled storage
   rises above ``Max_Storage``, ParFlow spills the excess on top of the
   observed release, so the same water can be released twice. Check the
   storage in ``ReservoirsOutput.csv`` when using observed releases.

Adding another dataset
----------------------

Each dataset is a subclass of ``ReservoirDataset`` that provides:

- ``reservoirs()``: a table of ``dam_id``, ``dam_name``, ``lat`` and ``lon``;
- ``daily_record(dam_id)``: daily ``outflow`` in m\ :sup:`3`/s and
  ``storage`` in m\ :sup:`3`, indexed by date.

Placement, corrections, unit conversion, gap handling, caching and setting the
keys are shared, so a new dataset only needs these two methods, a ``name``,
and optionally a shipped corrections file.
