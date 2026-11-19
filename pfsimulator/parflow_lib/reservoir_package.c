/*BHEADER*********************************************************************
 *
 *  Copyright (c) 1995-2009, Lawrence Livermore National Security,
 *  LLC. Produced at the Lawrence Livermore National Laboratory. Written
 *  by the Parflow Team (see the CONTRIBUTORS file)
 *  <parflow@lists.llnl.gov> CODE-OCEC-08-103. All rights reserved.
 *
 *  This file is part of Parflow. For details, see
 *  http://www.llnl.gov/casc/parflow
 *
 *  Please read the COPYRIGHT file or Our Notice and the LICENSE file
 *  for the GNU Lesser General Public License.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License (as published
 *  by the Free Software Foundation) version 2.1 dated February 1999.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms
 *  and conditions of the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 *  USA
 **********************************************************************EHEADER*/

#include "parflow.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/*--------------------------------------------------------------------------
 * Structures
 *--------------------------------------------------------------------------*/
typedef struct {
  /* reservoir info */
  int num_reservoirs;

  int       *type;
  void     **data;

  int overland_flow_solver;
  NameArray reservoir_names;
} PublicXtra;

typedef void InstanceXtra;

typedef struct {
  char    *name;
  double intake_x_location, intake_y_location;
  int has_secondary_intake_cell;
  double secondary_intake_x_location, secondary_intake_y_location;
  double release_x_location, release_y_location;
  double max_storage, min_release_storage, storage, release_rate;
  int release_type;
  int release_interpolation;
  int num_release_times;
  double   *release_times;
  double   *release_rates;
} Type0;                      /* basic vertical reservoir */

/*--------------------------------------------------------------------------
 * ReservoirPackage
 *--------------------------------------------------------------------------*/

/** @brief Returns true if cell (i, j) lies in the interior of the subgrid
 * (i.e. is owned by this rank rather than being a ghost cell).
 */
static bool CellIsOnSubgrid(int i, int j, Subgrid* subgrid)
{
  return i >= SubgridIX(subgrid) && i < SubgridIX(subgrid) + SubgridNX(subgrid) &&
         j >= SubgridIY(subgrid) && j < SubgridIY(subgrid) + SubgridNY(subgrid);
}

/** @brief Sets the slopes at the outlet faces of a cell to 0 to stop flow.
 * Assumes an overlandkinematic boundary condition, where the slope on a face
 * is stored in the cell on the lower side of that face.  Each slope is only
 * changed by the rank that owns it; call UpdateReservoirSlopeGhosts afterwards
 * so neighbouring ranks see the change.
 * @param i the x index of the cell in question
 * @param j the y index of the cell in question
 * @return Null, but modifies the problem data x and y slopes
 */
void StopOutletFlowAtCellOverlandKinematic(int i, int j, ProblemData* problem_data, Grid* grid)
{
  Vector      *slope_x = ProblemDataTSlopeX(problem_data);
  Vector      *slope_y = ProblemDataTSlopeY(problem_data);
  Subvector   *slope_x_subvector;
  Subvector   *slope_y_subvector;
  int index;
  double *slope_x_ptr;
  double *slope_y_ptr;
  int subgrid_index;
  Subgrid *subgrid;

  ForSubgridI(subgrid_index, GridSubgrids(grid))
  {
    subgrid = GridSubgrid(grid, subgrid_index);
    slope_x_subvector = VectorSubvector(slope_x, subgrid_index);
    slope_y_subvector = VectorSubvector(slope_y, subgrid_index);
    slope_x_ptr = SubvectorData(slope_x_subvector);
    slope_y_ptr = SubvectorData(slope_y_subvector);

    // East face: slope_x at (i, j). Negative slope flows east, out of the cell
    if (CellIsOnSubgrid(i, j, subgrid))
    {
      index = SubvectorEltIndex(slope_x_subvector, i, j, 0);
      if (slope_x_ptr[index] < 0)
      {
        slope_x_ptr[index] = 0;
      }
    }
    // West face: slope_x at (i - 1, j). Positive slope flows west, out of the cell
    if (CellIsOnSubgrid(i - 1, j, subgrid))
    {
      index = SubvectorEltIndex(slope_x_subvector, i - 1, j, 0);
      if (slope_x_ptr[index] > 0)
      {
        slope_x_ptr[index] = 0;
      }
    }
    // North face: slope_y at (i, j). Negative slope flows north, out of the cell
    if (CellIsOnSubgrid(i, j, subgrid))
    {
      index = SubvectorEltIndex(slope_y_subvector, i, j, 0);
      if (slope_y_ptr[index] < 0)
      {
        slope_y_ptr[index] = 0;
      }
    }
    // South face: slope_y at (i, j - 1). Positive slope flows south, out of the cell
    if (CellIsOnSubgrid(i, j - 1, subgrid))
    {
      index = SubvectorEltIndex(slope_y_subvector, i, j - 1, 0);
      if (slope_y_ptr[index] > 0)
      {
        slope_y_ptr[index] = 0;
      }
    }
  }
}

/** @brief Sets the slopes of a cell to 0 to stop flow out of it.
 * Assumes an overlandflow boundary condition, where slopes are cell centered.
 * Only the rank that owns the cell changes it; call UpdateReservoirSlopeGhosts
 * afterwards so neighbouring ranks see the change.
 * @param i the x index of the cell in question
 * @param j the y index of the cell in question
 * @return Null, but modifies the problem data x and y slopes
 */
void StopOutletFlowAtCellOverlandFlow(int i, int j, ProblemData* problem_data, Grid* grid)
{
  Vector      *slope_x = ProblemDataTSlopeX(problem_data);
  Vector      *slope_y = ProblemDataTSlopeY(problem_data);
  Subvector   *slope_x_subvector;
  Subvector   *slope_y_subvector;
  double *slope_x_ptr;
  double *slope_y_ptr;
  int subgrid_index;
  Subgrid *subgrid;

  ForSubgridI(subgrid_index, GridSubgrids(grid))
  {
    subgrid = GridSubgrid(grid, subgrid_index);
    if (CellIsOnSubgrid(i, j, subgrid))
    {
      slope_x_subvector = VectorSubvector(slope_x, subgrid_index);
      slope_y_subvector = VectorSubvector(slope_y, subgrid_index);
      slope_x_ptr = SubvectorData(slope_x_subvector);
      slope_y_ptr = SubvectorData(slope_y_subvector);
      slope_x_ptr[SubvectorEltIndex(slope_x_subvector, i, j, 0)] = 0;
      slope_y_ptr[SubvectorEltIndex(slope_y_subvector, i, j, 0)] = 0;
    }
  }
}

/** @brief Copies the slopes changed by the StopOutletFlow functions into the
 * ghost cells of neighbouring ranks.  Must be called on all ranks.
 */
static void UpdateReservoirSlopeGhosts(ProblemData* problem_data)
{
  VectorUpdateCommHandle *handle;

  handle = InitVectorUpdate(ProblemDataTSlopeX(problem_data), VectorUpdateAll);
  FinalizeVectorUpdate(handle);
  handle = InitVectorUpdate(ProblemDataTSlopeY(problem_data), VectorUpdateAll);
  FinalizeVectorUpdate(handle);
}

void         ReservoirPackage(
                              ProblemData *problem_data)
{
  PFModule         *this_module = ThisPFModule;
  PublicXtra       *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);

  Type0            *dummy0;

  Subgrid          *new_intake_subgrid;
  Subgrid          *new_secondary_intake_subgrid;
  Subgrid          *new_release_subgrid;

  ReservoirData         *reservoir_data = ProblemDataReservoirData(problem_data);
  ReservoirDataPhysical *reservoir_data_physical;

  int i, reservoir_index;

  int intake_ix, intake_iy;
  int secondary_intake_ix, secondary_intake_iy;
  int release_ix, release_iy;
  int iz_lower;
  int nx, ny, nz;
  int rx, ry, rz;
  int current_mpi_rank;
  int intake_cell_rank, secondary_intake_cell_rank, release_cell_rank;
  int release_cell_inactive;
  int split_color;
  int grid_nz;

  double release_subgrid_volume;
  bool part_of_reservoir_lives_on_this_rank;

  /* Allocate the reservoir data */

  ReservoirDataNumReservoirs(reservoir_data) = (public_xtra->num_reservoirs);
  //it feels like there should be a better way to get the grid than this,but I couldn't find one...
  Grid* grid = VectorGrid(problem_data->rsz);
  grid_nz = grid->background->nz;

  if ((public_xtra->num_reservoirs) > 0)
  {
    ReservoirDataReservoirPhysicals(reservoir_data) = ctalloc(ReservoirDataPhysical *, (public_xtra->num_reservoirs));
  }

  reservoir_index = 0;

  if ((public_xtra->num_reservoirs) > 0)
  {
    /* Load the reservoir data */
    for (i = 0; i < (public_xtra->num_reservoirs); i++)
    {
      dummy0 = (Type0*)(public_xtra->data[i]);
      intake_ix = IndexSpaceX((dummy0->intake_x_location), 0);
      intake_iy = IndexSpaceY((dummy0->intake_y_location), 0);
      secondary_intake_ix = IndexSpaceX((dummy0->secondary_intake_x_location), 0);
      secondary_intake_iy = IndexSpaceY((dummy0->secondary_intake_y_location), 0);
      release_ix = IndexSpaceX((dummy0->release_x_location), 0);
      release_iy = IndexSpaceY((dummy0->release_y_location), 0);

      // -1 means "not on this rank"; the max reduction below finds the owning rank
      intake_cell_rank = -1;
      secondary_intake_cell_rank = -1;
      release_cell_rank = -1;

      iz_lower = grid_nz - 1;

      nx = 1;
      ny = 1;
      nz = 1;

      rx = 0;
      ry = 0;
      rz = 0;
      // amps_Rank returns 0 when MPI is not used
      current_mpi_rank = amps_Rank(amps_CommWorld);

      new_intake_subgrid = NewSubgrid(intake_ix, intake_iy, iz_lower,
                                      nx, ny, nz,
                                      rx, ry, rz,
                                      current_mpi_rank);

      new_secondary_intake_subgrid = NewSubgrid(secondary_intake_ix, secondary_intake_iy, iz_lower,
                                                nx, ny, nz,
                                                rx, ry, rz,
                                                current_mpi_rank);

      new_release_subgrid = NewSubgrid(release_ix, release_iy, iz_lower,
                                       nx, ny, nz,
                                       rx, ry, rz,
                                       current_mpi_rank);

      if (SubgridIntersectsCurrentRank(new_intake_subgrid, grid))
      {
        intake_cell_rank = current_mpi_rank;
      }

      if (SubgridIntersectsCurrentRank(new_secondary_intake_subgrid, grid))
      {
        secondary_intake_cell_rank = current_mpi_rank;
      }
      release_subgrid_volume = 1;
      release_cell_inactive = 0;
      if (SubgridIntersectsCurrentRank(new_release_subgrid, grid))
      {
        release_cell_rank = current_mpi_rank;
        release_subgrid_volume = CalculateLocalSubgridVolume(new_release_subgrid, problem_data);
        // The volume only counts active cells; releasing into an inactive
        // cell would divide by zero in PhaseSource
        release_cell_inactive = (release_subgrid_volume <= 0.0);
      }
      // If we are multiprocessor we need to do some reductions to determine the correct ranks
      // for the reservoirs
#ifdef PARFLOW_HAVE_MPI
      amps_Invoice reservoir_cells_invoice = amps_NewInvoice("%i%i%i%i", &intake_cell_rank, &secondary_intake_cell_rank,
                                                             &release_cell_rank, &release_cell_inactive);
      amps_AllReduce(amps_CommWorld, reservoir_cells_invoice, amps_Max);
      amps_FreeInvoice(reservoir_cells_invoice);

      part_of_reservoir_lives_on_this_rank =
        (release_cell_rank == current_mpi_rank) ||
        (intake_cell_rank == current_mpi_rank) ||
        (secondary_intake_cell_rank == current_mpi_rank);

      MPI_Comm new_reservoir_communicator;
      split_color = part_of_reservoir_lives_on_this_rank ? 1 : MPI_UNDEFINED;
      MPI_Comm_split(amps_CommWorld, split_color, current_mpi_rank, &new_reservoir_communicator);
#endif
      // Every rank has the reduced values, so all ranks stop together
      if (intake_cell_rank < 0)
      {
        InputError("Error: the intake cell of reservoir <%s> (Intake_X, Intake_Y) is outside the computational grid%s\n",
                   dummy0->name, "");
      }
      if (dummy0->has_secondary_intake_cell && secondary_intake_cell_rank < 0)
      {
        InputError("Error: the secondary intake cell of reservoir <%s> (Secondary_Intake_X, Secondary_Intake_Y) is outside the computational grid%s\n",
                   dummy0->name, "");
      }
      if (release_cell_rank < 0)
      {
        InputError("Error: the release cell of reservoir <%s> (Release_X, Release_Y) is outside the computational grid%s\n",
                   dummy0->name, "");
      }
      if (release_cell_inactive)
      {
        InputError("Error: the release cell of reservoir <%s> (Release_X, Release_Y) is not an active cell of the domain%s\n",
                   dummy0->name, "");
      }
      // edit the slopes to prevent stuff running through the reservoir
      if (public_xtra->overland_flow_solver == OVERLAND_FLOW)
      {
        StopOutletFlowAtCellOverlandFlow(intake_ix, intake_iy, problem_data, grid);
        if (dummy0->has_secondary_intake_cell)
        {
          StopOutletFlowAtCellOverlandFlow(secondary_intake_ix, secondary_intake_iy, problem_data, grid);
        }
      }
      else if (public_xtra->overland_flow_solver == OVERLAND_KINEMATIC)
      {
        StopOutletFlowAtCellOverlandKinematic(intake_ix, intake_iy, problem_data, grid);
        if (dummy0->has_secondary_intake_cell)
        {
          StopOutletFlowAtCellOverlandKinematic(secondary_intake_ix, secondary_intake_iy, problem_data, grid);
        }
      }

      reservoir_data_physical = ctalloc(ReservoirDataPhysical, 1);
      ReservoirDataPhysicalName(reservoir_data_physical) = ctalloc(char, strlen((dummy0->name)) + 1);
      strcpy(ReservoirDataPhysicalName(reservoir_data_physical), (dummy0->name));
      ReservoirDataPhysicalIntakeCellMpiRank(reservoir_data_physical) = (intake_cell_rank);
      ReservoirDataPhysicalIntakeXLower(reservoir_data_physical) = (dummy0->intake_x_location);
      ReservoirDataPhysicalIntakeYLower(reservoir_data_physical) = (dummy0->intake_y_location);
      ReservoirDataPhysicalSecondaryIntakeCellMpiRank(reservoir_data_physical) = (secondary_intake_cell_rank);
      ReservoirDataPhysicalSecondaryIntakeXLower(reservoir_data_physical) = (dummy0->secondary_intake_x_location);
      ReservoirDataPhysicalSecondaryIntakeYLower(reservoir_data_physical) = (dummy0->secondary_intake_y_location);
      ReservoirDataPhysicalHasSecondaryIntakeCell(reservoir_data_physical) = (dummy0->has_secondary_intake_cell);
      ReservoirDataPhysicalReleaseCellMpiRank(reservoir_data_physical) = (release_cell_rank);
      ReservoirDataPhysicalReleaseXLower(reservoir_data_physical) = (dummy0->release_x_location);
      ReservoirDataPhysicalReleaseYLower(reservoir_data_physical) = (dummy0->release_y_location);
      ReservoirDataPhysicalIntakeXUpper(reservoir_data_physical) = (dummy0->intake_x_location);
      ReservoirDataPhysicalIntakeYUpper(reservoir_data_physical) = (dummy0->intake_y_location);
      ReservoirDataPhysicalSecondaryIntakeXUpper(reservoir_data_physical) = (dummy0->secondary_intake_x_location);
      ReservoirDataPhysicalSecondaryIntakeYUpper(reservoir_data_physical) = (dummy0->secondary_intake_y_location);
      ReservoirDataPhysicalReleaseXUpper(reservoir_data_physical) = (dummy0->release_x_location);
      ReservoirDataPhysicalReleaseYUpper(reservoir_data_physical) = (dummy0->release_y_location);
      ReservoirDataPhysicalMaxStorage(reservoir_data_physical) = (dummy0->max_storage);
      ReservoirDataPhysicalMinReleaseStorage(reservoir_data_physical) = (dummy0->min_release_storage);
      ReservoirDataPhysicalIntakeAmountSinceLastPrint(reservoir_data_physical) = (0);
      ReservoirDataPhysicalReleaseAmountSinceLastPrint(reservoir_data_physical) = (0);
      ReservoirDataPhysicalReleaseAmountInSolver(reservoir_data_physical) = (0);
      ReservoirDataPhysicalReleaseRate(reservoir_data_physical) = (dummy0->release_rate);
      ReservoirDataPhysicalReleaseType(reservoir_data_physical) = (dummy0->release_type);
      ReservoirDataPhysicalReleaseInterpolation(reservoir_data_physical) = (dummy0->release_interpolation);
      ReservoirDataPhysicalNumReleaseTimes(reservoir_data_physical) = (dummy0->num_release_times);
      if (dummy0->num_release_times > 0)
      {
        // Copy so that ReservoirData owns its own schedule independent of the PublicXtra
        int n = dummy0->num_release_times;
        ReservoirDataPhysicalReleaseTimes(reservoir_data_physical) = ctalloc(double, n);
        ReservoirDataPhysicalReleaseRates(reservoir_data_physical) = ctalloc(double, n);
        memcpy(ReservoirDataPhysicalReleaseTimes(reservoir_data_physical), dummy0->release_times, n * sizeof(double));
        memcpy(ReservoirDataPhysicalReleaseRates(reservoir_data_physical), dummy0->release_rates, n * sizeof(double));
      }
      ReservoirDataPhysicalStorage(reservoir_data_physical) = (dummy0->storage);
      ReservoirDataPhysicalIntakeSubgrid(reservoir_data_physical) = new_intake_subgrid;
      ReservoirDataPhysicalSecondaryIntakeSubgrid(reservoir_data_physical) = new_secondary_intake_subgrid;
      ReservoirDataPhysicalReleaseSubgrid(reservoir_data_physical) = new_release_subgrid;
      ReservoirDataPhysicalSize(reservoir_data_physical) = release_subgrid_volume;
      ReservoirDataReservoirPhysical(reservoir_data, reservoir_index) = reservoir_data_physical;
#ifdef PARFLOW_HAVE_MPI
      reservoir_data_physical->mpi_communicator = new_reservoir_communicator;
#endif
      /* Put in values for this reservoir */
      reservoir_index++;
    }
    ReservoirDataNumReservoirs(reservoir_data) = (reservoir_index);

    // The slope edits above were made only on the owning ranks
    UpdateReservoirSlopeGhosts(problem_data);
  }
}


/*--------------------------------------------------------------------------
 * ReservoirPackageInitInstanceXtra
 *--------------------------------------------------------------------------*/
PFModule *ReservoirPackageInitInstanceXtra()
{
  PFModule      *this_module = ThisPFModule;
  InstanceXtra  *instance_xtra;

  instance_xtra = NULL;

  PFModuleInstanceXtra(this_module) = instance_xtra;
  return this_module;
}


/*--------------------------------------------------------------------------
 * ReservoirPackageFreeInstanceXtra
 *--------------------------------------------------------------------------*/

void  ReservoirPackageFreeInstanceXtra()
{
  PFModule      *this_module = ThisPFModule;
  InstanceXtra  *instance_xtra = (InstanceXtra*)PFModuleInstanceXtra(this_module);

  if (instance_xtra)
  {
    tfree(instance_xtra);
  }
}


/*--------------------------------------------------------------------------
 * ReservoirPackageNewPublicXtra
 *--------------------------------------------------------------------------*/

PFModule  *ReservoirPackageNewPublicXtra()
{
  PFModule      *this_module = ThisPFModule;
  PublicXtra    *public_xtra;

  Type0         *dummy0;


  char *reservoir_names;
  char * EMPTY_NAMES_LIST = "";
  char *reservoir_name;

  char key[IDB_MAX_KEY_LEN];

  int num_reservoirs;

  char          *switch_name;
  int switch_value;


  public_xtra = ctalloc(PublicXtra, 1);

  reservoir_names = GetStringDefault("Reservoirs.Names", EMPTY_NAMES_LIST);

  public_xtra->reservoir_names = NA_NewNameArray(reservoir_names);

  num_reservoirs = NA_Sizeof(public_xtra->reservoir_names);


  if (num_reservoirs > 0)
  {
    (public_xtra->type) = ctalloc(int, num_reservoirs);
    (public_xtra->data) = ctalloc(void *, num_reservoirs);
    NameArray overland_flow_solver_na = NA_NewNameArray("OverlandFlow OverlandKinematic");
    sprintf(key, "Reservoirs.Overland_Flow_Solver");
    switch_name = GetString(key);
    switch_value = NA_NameToIndexExitOnError(overland_flow_solver_na, switch_name, key);
    NA_FreeNameArray(overland_flow_solver_na);
    switch (switch_value)
    {
      case 0:
      {
        public_xtra->overland_flow_solver = OVERLAND_FLOW;
        break;
      }

      case 1:
      {
        public_xtra->overland_flow_solver = OVERLAND_KINEMATIC;
        break;
      }

      default:
      {
        InputError("Reservoirs.Overland_Flow_Solver must be one of OverlandFlow or OverlandKinematic, not %s%s\n", switch_name, "");
      }
    }

    for (int i = 0; i < num_reservoirs; i++)
    {
      reservoir_name = NA_IndexToName(public_xtra->reservoir_names, i);

      dummy0 = ctalloc(Type0, 1);
      dummy0->name = strdup(reservoir_name);

      sprintf(key, "Reservoirs.%s.Release_X", reservoir_name);
      dummy0->release_x_location = GetDouble(key);

      sprintf(key, "Reservoirs.%s.Release_Y", reservoir_name);
      dummy0->release_y_location = GetDouble(key);

      sprintf(key, "Reservoirs.%s.Intake_X", reservoir_name);
      dummy0->intake_x_location = GetDouble(key);

      sprintf(key, "Reservoirs.%s.Intake_Y", reservoir_name);
      dummy0->intake_y_location = GetDouble(key);

      sprintf(key, "Reservoirs.%s.Has_Secondary_Intake_Cell", reservoir_name);
      dummy0->has_secondary_intake_cell = GetInt(key);
      // I would use a name array to do this validation, but the input is an int. Long term there
      // needs to be a fix for bool values that are nested under a dynamic key (e.g. the reservoir name)
      // within the table reader code
      if (!(dummy0->has_secondary_intake_cell == 0 || dummy0->has_secondary_intake_cell == 1))
      {
        InputError("Reservoirs.%s.HasSecondaryIntakeCell must be one of 0 (if false) or 1 (if true)", reservoir_name, "");
      }

      if (dummy0->has_secondary_intake_cell)
      {
        sprintf(key, "Reservoirs.%s.Secondary_Intake_X", reservoir_name);
        dummy0->secondary_intake_x_location = GetDouble(key);

        sprintf(key, "Reservoirs.%s.Secondary_Intake_Y", reservoir_name);
        dummy0->secondary_intake_y_location = GetDouble(key);
      }
      else
      {
        dummy0->secondary_intake_x_location = -1;
        dummy0->secondary_intake_y_location = -1;
      };
      sprintf(key, "Reservoirs.%s.Min_Release_Storage", reservoir_name);
      dummy0->min_release_storage = GetDouble(key);

      NameArray release_type_na = NA_NewNameArray("Constant TimeSeries");
      sprintf(key, "Reservoirs.%s.Release_Type", reservoir_name);
      switch_name = GetStringDefault(key, "Constant");
      dummy0->release_type = NA_NameToIndexExitOnError(release_type_na, switch_name, key);
      NA_FreeNameArray(release_type_na);

      dummy0->release_interpolation = RESERVOIR_RELEASE_INTERP_STEP;
      dummy0->num_release_times = 0;
      dummy0->release_times = NULL;
      dummy0->release_rates = NULL;

      if (dummy0->release_type == RESERVOIR_RELEASE_TIMESERIES)
      {
        NameArray release_interp_na = NA_NewNameArray("Step Linear");
        sprintf(key, "Reservoirs.%s.Release_Interpolation", reservoir_name);
        switch_name = GetStringDefault(key, "Step");
        dummy0->release_interpolation = NA_NameToIndexExitOnError(release_interp_na, switch_name, key);
        NA_FreeNameArray(release_interp_na);

        sprintf(key, "Reservoirs.%s.Release_File", reservoir_name);
        char *release_filename = GetString(key);
        ReadReservoirReleaseFile(release_filename, reservoir_name,
                                 &(dummy0->num_release_times),
                                 &(dummy0->release_times),
                                 &(dummy0->release_rates));

        // Release_Rate is not used for a time series; it is overwritten each
        // step with the scheduled rate so it shows up in the reservoir output
        dummy0->release_rate = dummy0->release_rates[0];
      }
      else
      {
        sprintf(key, "Reservoirs.%s.Release_Rate", reservoir_name);
        dummy0->release_rate = GetDouble(key);
      }

      sprintf(key, "Reservoirs.%s.Max_Storage", reservoir_name);
      dummy0->max_storage = GetDouble(key);

      sprintf(key, "Reservoirs.%s.Storage", reservoir_name);
      dummy0->storage = GetDouble(key);

      (public_xtra->num_reservoirs)++;
      (public_xtra->data[i]) = (void*)dummy0;
    }
  }


  PFModulePublicXtra(this_module) = public_xtra;
  return this_module;
}


/*-------------------------------------------------------------------------
 * ReservoirPackageFreePublicXtra
 *-------------------------------------------------------------------------*/

void  ReservoirPackageFreePublicXtra()
{
  PFModule    *this_module = ThisPFModule;
  PublicXtra  *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);

  Type0         *dummy0;

  int num_reservoirs;
  int i;

  if (public_xtra)
  {
    NA_FreeNameArray(public_xtra->reservoir_names);

    /* Free the reservoir information */
    num_reservoirs = (public_xtra->num_reservoirs);
    if (num_reservoirs > 0)
    {
      for (i = 0; i < num_reservoirs; i++)
      {
        dummy0 = (Type0*)(public_xtra->data[i]);

        if ((dummy0->name))
        {
          tfree((dummy0->name));
        }
        if ((dummy0->release_times))
        {
          tfree((dummy0->release_times));
        }
        if ((dummy0->release_rates))
        {
          tfree((dummy0->release_rates));
        }
        tfree(dummy0);
      }
      tfree(public_xtra->data);
      tfree(public_xtra->type);
    }
    tfree(public_xtra);
  }
}


/*--------------------------------------------------------------------------
 * ReservoirPackageSizeOfTempData
 *--------------------------------------------------------------------------*/

int  ReservoirPackageSizeOfTempData()
{
  return 0;
}
