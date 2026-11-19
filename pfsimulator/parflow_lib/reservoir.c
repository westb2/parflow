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
#include "../amps/mpi1/amps.h"
#include <fcntl.h>
#include <errno.h>


#include <string.h>


/*****************************************************************************
*
* The functions in this file are for manipulating the ReservoirData structure
*   in ProblemData and work in conjunction with the ReservoirPackage module.
*
* Because of the times things are called, the New function is twinky
* (it was basically put in to be symmetric with the New/Free paradigm
* used through out the code) and is invoked by SetProblemData.  The Alloc
* function actually allocates the data within the sub-structures and is
* invoked by the ReservoirPackage (which has the data needed to compute the array
* sizes and such).  The Free is smart enough to clean up after both the New
* and Alloc functions and is called by SetProblemData.
*
*****************************************************************************/


/*--------------------------------------------------------------------------
 * NewReservoirData
 *--------------------------------------------------------------------------*/

ReservoirData *NewReservoirData()
{
  ReservoirData    *reservoir_data;

  reservoir_data = ctalloc(ReservoirData, 1);

  ReservoirDataNumReservoirs(reservoir_data) = -1;
  ReservoirDataTimestep(reservoir_data) = 0.0;
  ReservoirDataOutputHeaderWritten(reservoir_data) = 0;

  ReservoirDataReservoirPhysicals(reservoir_data) = NULL;


  return reservoir_data;
}


/*--------------------------------------------------------------------------
 * FreeReservoirData
 *--------------------------------------------------------------------------*/

void FreeReservoirData(
                       ReservoirData *reservoir_data)
{
  ReservoirDataPhysical *reservoir_data_physical;
  int i;

  if (reservoir_data)
  {
    if (ReservoirDataNumReservoirs(reservoir_data) > 0)
    {
      for (i = 0; i < ReservoirDataNumReservoirs(reservoir_data); i++)
      {
        reservoir_data_physical = ReservoirDataReservoirPhysical(reservoir_data, i);
        if (ReservoirDataPhysicalName(reservoir_data_physical))
        {
          tfree(ReservoirDataPhysicalName(reservoir_data_physical));
        }
        if (ReservoirDataPhysicalIntakeSubgrid(reservoir_data_physical))
        {
          FreeSubgrid(ReservoirDataPhysicalIntakeSubgrid(reservoir_data_physical));
        }
        if (ReservoirDataPhysicalReleaseSubgrid(reservoir_data_physical))
        {
          FreeSubgrid(ReservoirDataPhysicalReleaseSubgrid(reservoir_data_physical));
        }
        if (ReservoirDataPhysicalSecondaryIntakeSubgrid(reservoir_data_physical))
        {
          FreeSubgrid(ReservoirDataPhysicalSecondaryIntakeSubgrid(reservoir_data_physical));
        }
        tfree(reservoir_data_physical);
      }
      if (ReservoirDataReservoirPhysicals(reservoir_data))
      {
        tfree(ReservoirDataReservoirPhysicals(reservoir_data));
      }
    }
    tfree(reservoir_data);
  }
}


void WriteReservoirs(
                     char *         file_prefix,
                     Problem *      problem,
                     ReservoirData *reservoir_data,
                     double         time,
                     int            write_header)
{
  ReservoirDataPhysical *reservoir_data_physical;


  int reservoir;

  FILE             *file;

  char file_suffix[5] = "csv";
  char filename[255];

  int p;

  if (ReservoirDataNumReservoirs(reservoir_data) > 0)
  {
    p = amps_Rank(amps_CommWorld);
    /* The header is normally written with the initial conditions.  If those
     * are not printed, write it with the first row instead, unless the file
     * already exists (e.g. a restarted run appending to earlier output). */
    if (write_header || !ReservoirDataOutputHeaderWritten(reservoir_data))
    {
      ReservoirDataOutputHeaderWritten(reservoir_data) = 1;
      sprintf(filename, "%s.%s", file_prefix, file_suffix);
      int file_exists = 0;
      if (!write_header && p == 0)
      {
        FILE *existing = fopen(filename, "r");
        if (existing)
        {
          file_exists = 1;
          fclose(existing);
        }
      }
      if (p == 0 && !file_exists)
      {
        file = fopen(filename, "w");
        fprintf(file, "time");
        fprintf(file, ",name");
        fprintf(file, ",storage");
        fprintf(file, ",intake_amount_since_last_row");
        fprintf(file, ",release_amount_since_last_row");
        fprintf(file, ",release_rate");
        fprintf(file, "\n");
        fclose(file);
      }
    }
    /* Rank 0 may not own any part of a reservoir, so its copy of the
     * reservoir state can be stale.  The rank that owns the release cell
     * always has current values; it contributes them and every other rank
     * contributes zero, and a single sum over all ranks delivers them to
     * rank 0.  This only runs when reservoir output is written, so it adds
     * no communication to ordinary timesteps.  All ranks must call
     * WriteReservoirs. */
    int num_reservoirs = ReservoirDataNumReservoirs(reservoir_data);
    int num_values = 3 * num_reservoirs;
    double *values = ctalloc(double, num_values);

    for (reservoir = 0; reservoir < num_reservoirs; reservoir++)
    {
      reservoir_data_physical = ReservoirDataReservoirPhysical(reservoir_data, reservoir);
#ifdef PARFLOW_HAVE_MPI
      if (ReservoirDataPhysicalReleaseCellMpiRank(reservoir_data_physical) == p)
#endif
      {
        values[3 * reservoir] = ReservoirDataPhysicalStorage(reservoir_data_physical);
        values[3 * reservoir + 1] = ReservoirDataPhysicalIntakeAmountSinceLastPrint(reservoir_data_physical);
        values[3 * reservoir + 2] = ReservoirDataPhysicalReleaseAmountSinceLastPrint(reservoir_data_physical);
      }
      /* Every rank resets its running totals for the next output row */
      ReservoirDataPhysicalReleaseAmountSinceLastPrint(reservoir_data_physical) = 0;
      ReservoirDataPhysicalIntakeAmountSinceLastPrint(reservoir_data_physical) = 0;
    }

#ifdef PARFLOW_HAVE_MPI
    amps_Invoice invoice = amps_NewInvoice("%*d", num_values, values);
    amps_AllReduce(amps_CommWorld, invoice, amps_Add);
    amps_FreeInvoice(invoice);
#endif

    if (p == 0)
    {
      sprintf(filename, "%s.%s", file_prefix, file_suffix);
      file = fopen(filename, "a");
      if (file == NULL)
      {
        amps_Printf("Error: can't open output file %s\n", filename);
        exit(1);
      }
      for (reservoir = 0; reservoir < num_reservoirs; reservoir++)
      {
        reservoir_data_physical = ReservoirDataReservoirPhysical(reservoir_data, reservoir);
        fprintf(file, "%f", time);
        fprintf(file, ",%s", ReservoirDataPhysicalName(reservoir_data_physical));
        fprintf(file, ",%f", values[3 * reservoir]);
        fprintf(file, ",%f", values[3 * reservoir + 1]);
        fprintf(file, ",%f", values[3 * reservoir + 2]);
        fprintf(file, ",%f", ReservoirDataPhysicalReleaseRate(reservoir_data_physical));
        fprintf(file, "\n");
      }
      fclose(file);
    }

    tfree(values);
  }
}



/*--------------------------------------------------------------------------
 * ReservoirDataPhysicalComputeStepRelease
 *
 * Returns the release rate [L^3 T^-1] to apply over a timestep of length dt,
 * based on the storage at the start of the step:
 *
 *   - Nothing is released once storage is at or below Min_Release_Storage.
 *   - Otherwise Release_Rate is released.
 *   - If storage is above Max_Storage, the excess is spilled over this step,
 *     i.e. an extra (storage - max_storage) / dt is released on top of
 *     Release_Rate, so the amount spilled does not depend on dt.
 *   - The total is capped at (storage - min_release_storage) / dt so a single
 *     step never releases storage below Min_Release_Storage.
 *
 * Inflow during the step is not counted, so the cap is conservative.  If dt
 * is not known (dt <= 0) only Release_Rate is returned.
 *--------------------------------------------------------------------------*/

double ReservoirDataPhysicalComputeStepRelease(
                                               ReservoirDataPhysical *reservoir_data_physical,
                                               double                 dt)
{
  double storage = ReservoirDataPhysicalStorage(reservoir_data_physical);
  double min_storage = ReservoirDataPhysicalMinReleaseStorage(reservoir_data_physical);
  double max_storage = ReservoirDataPhysicalMaxStorage(reservoir_data_physical);
  double rate;

  if (storage <= min_storage)
  {
    return 0.0;
  }

  rate = ReservoirDataPhysicalReleaseRate(reservoir_data_physical);

  if (dt > 0.0)
  {
    if (storage > max_storage)
    {
      rate += (storage - max_storage) / dt;
    }
    rate = pfmin(rate, (storage - min_storage) / dt);
  }

  return pfmax(rate, 0.0);
}
