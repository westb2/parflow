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
        if (ReservoirDataPhysicalReleaseTimes(reservoir_data_physical))
        {
          tfree(ReservoirDataPhysicalReleaseTimes(reservoir_data_physical));
        }
        if (ReservoirDataPhysicalReleaseRates(reservoir_data_physical))
        {
          tfree(ReservoirDataPhysicalReleaseRates(reservoir_data_physical));
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
 * ReservoirDataPhysicalComputeReleaseRate
 *
 * Returns the scheduled release rate [L^3 T^-1] for a reservoir at the
 * given time.  For a constant release this is simply the Release_Rate key.
 * For a time series the rate is looked up in the release table:
 *
 *   Step:   the rate in row k applies on the interval (t_k, t_{k+1}].
 *           Intervals are closed on the right because the solver evaluates
 *           sources at the end of each timestep, so a step that ends exactly
 *           on t_{k+1} still uses the rate for the interval it covered.
 *   Linear: the rate is linearly interpolated between rows.
 *
 * Before the first row the first rate is used; after the last row the last
 * rate is held.
 *--------------------------------------------------------------------------*/

double ReservoirDataPhysicalComputeReleaseRate(
                                               ReservoirDataPhysical *reservoir_data_physical,
                                               double                 time)
{
  int n;
  int lo, hi, mid;
  double *times;
  double *rates;
  double weight;

  if (ReservoirDataPhysicalReleaseType(reservoir_data_physical) != RESERVOIR_RELEASE_TIMESERIES)
  {
    return ReservoirDataPhysicalReleaseRate(reservoir_data_physical);
  }

  n = ReservoirDataPhysicalNumReleaseTimes(reservoir_data_physical);
  times = ReservoirDataPhysicalReleaseTimes(reservoir_data_physical);
  rates = ReservoirDataPhysicalReleaseRates(reservoir_data_physical);

  if (ReservoirDataPhysicalReleaseInterpolation(reservoir_data_physical) == RESERVOIR_RELEASE_INTERP_LINEAR)
  {
    if (time <= times[0])
    {
      return rates[0];
    }
    if (time >= times[n - 1])
    {
      return rates[n - 1];
    }
  }
  else
  {
    /* Step: shift time back by a small tolerance so that a step ending on a
     * breakpoint (allowing for roundoff in the accumulated solver time)
     * uses the rate for the interval that ends there.  After the shift the
     * search below is for the last row with times[lo] <= time. */
    time -= RESERVOIR_RELEASE_TIME_TOLERANCE * pfmax(1.0, fabs(time));
    if (time < times[0])
    {
      return rates[0];
    }
    if (time >= times[n - 1])
    {
      return rates[n - 1];
    }
  }

  /* Binary search for lo such that times[lo] <= time < times[lo + 1] */
  lo = 0;
  hi = n - 1;
  while (hi - lo > 1)
  {
    mid = (lo + hi) / 2;
    if (times[mid] <= time)
    {
      lo = mid;
    }
    else
    {
      hi = mid;
    }
  }

  if (ReservoirDataPhysicalReleaseInterpolation(reservoir_data_physical) == RESERVOIR_RELEASE_INTERP_LINEAR)
  {
    weight = (time - times[lo]) / (times[hi] - times[lo]);
    return rates[lo] + weight * (rates[hi] - rates[lo]);
  }

  return rates[lo];
}


/*--------------------------------------------------------------------------
 * ReservoirDataPhysicalComputeStepRelease
 *
 * Returns the release rate [L^3 T^-1] to apply over a timestep of length dt
 * ending at the given time, based on the storage at the start of the step:
 *
 *   - Nothing is released once storage is at or below Min_Release_Storage.
 *   - The scheduled rate (ReservoirDataPhysicalComputeReleaseRate) is released.
 *   - If storage is above Max_Storage, the excess is spilled over this step,
 *     i.e. an extra (storage - max_storage) / dt is released on top of the
 *     scheduled rate, so the amount spilled does not depend on dt.
 *   - The total is capped at (storage - min_release_storage) / dt so a single
 *     step never releases storage below Min_Release_Storage.
 *
 * Inflow during the step is not counted, so the cap is conservative.  If dt
 * is not known (dt <= 0) only the scheduled rate is returned.
 *
 * For time series reservoirs the scheduled rate is also stored in
 * ReleaseRate so it is reported in the reservoir output.
 *--------------------------------------------------------------------------*/

double ReservoirDataPhysicalComputeStepRelease(
                                               ReservoirDataPhysical *reservoir_data_physical,
                                               double                 time,
                                               double                 dt)
{
  double storage = ReservoirDataPhysicalStorage(reservoir_data_physical);
  double min_storage = ReservoirDataPhysicalMinReleaseStorage(reservoir_data_physical);
  double max_storage = ReservoirDataPhysicalMaxStorage(reservoir_data_physical);
  double scheduled_rate = ReservoirDataPhysicalComputeReleaseRate(reservoir_data_physical, time);
  double rate;

  if (ReservoirDataPhysicalReleaseType(reservoir_data_physical) == RESERVOIR_RELEASE_TIMESERIES)
  {
    ReservoirDataPhysicalReleaseRate(reservoir_data_physical) = scheduled_rate;
  }

  if (storage <= min_storage)
  {
    return 0.0;
  }

  rate = scheduled_rate;

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


/*--------------------------------------------------------------------------
 * ParseReservoirReleaseLine
 *
 * Parses "time,rate" (comma and/or whitespace separated) from a line.
 * Returns 1 on success, 0 if the line does not hold two numbers.
 *--------------------------------------------------------------------------*/

static int ParseReservoirReleaseLine(char *line, double *time, double *rate)
{
  char *cursor = line;
  char *end;

  *time = strtod(cursor, &end);
  if (end == cursor)
  {
    return 0;
  }
  cursor = end;
  while (*cursor == ' ' || *cursor == '\t' || *cursor == ',')
  {
    cursor++;
  }
  *rate = strtod(cursor, &end);
  if (end == cursor)
  {
    return 0;
  }
  cursor = end;
  while (*cursor == ' ' || *cursor == '\t' || *cursor == ',' || *cursor == '\r' || *cursor == '\n')
  {
    cursor++;
  }
  /* Anything else left on the line (a third column, stray text) is an error */
  return(*cursor == '\0');
}


/*--------------------------------------------------------------------------
 * ReadReservoirReleaseFile
 *
 * Reads a reservoir release time series from a CSV file with two columns:
 * time and release rate [L^3 T^-1].  Blank lines and lines starting with
 * '#' are ignored, and a single non-numeric header line is allowed before
 * the first row.  Times must be strictly increasing.
 *
 * The file is read on rank 0 and broadcast to all ranks, so this must be
 * called collectively.  On return *times and *rates are allocated with
 * ctalloc and owned by the caller.
 *--------------------------------------------------------------------------*/

void ReadReservoirReleaseFile(
                              char *   filename,
                              char *   reservoir_name,
                              int *    num_times,
                              double **times,
                              double **rates)
{
  int n = 0;
  int status = 0;     /* 0 ok, 1 can't open, 2 bad line, 3 not increasing, 4 empty, 5 negative rate */
  int bad_line = 0;
  double *read_times = NULL;
  double *read_rates = NULL;
  amps_Invoice invoice;

  if (amps_Rank(amps_CommWorld) == 0)
  {
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
      status = 1;
    }
    else
    {
      char line[1024];
      int capacity = 0;
      int line_number = 0;
      int seen_header = 0;
      double t, r;

      while (status == 0 && fgets(line, sizeof(line), file))
      {
        char *start = line;
        line_number++;
        while (*start == ' ' || *start == '\t')
        {
          start++;
        }
        if (*start == '\0' || *start == '\n' || *start == '\r' || *start == '#')
        {
          continue;
        }
        if (!ParseReservoirReleaseLine(start, &t, &r))
        {
          if (n == 0 && !seen_header)
          {
            seen_header = 1;
            continue;
          }
          status = 2;
          bad_line = line_number;
          break;
        }
        if (r < 0.0)
        {
          status = 5;
          bad_line = line_number;
          break;
        }
        if (n > 0 && t <= read_times[n - 1])
        {
          status = 3;
          bad_line = line_number;
          break;
        }
        if (n == capacity)
        {
          capacity = (capacity == 0) ? 256 : 2 * capacity;
          read_times = (double*)realloc(read_times, capacity * sizeof(double));
          read_rates = (double*)realloc(read_rates, capacity * sizeof(double));
        }
        read_times[n] = t;
        read_rates[n] = r;
        n++;
      }
      fclose(file);
      if (status == 0 && n == 0)
      {
        status = 4;
      }
    }
  }

  invoice = amps_NewInvoice("%i%i%i", &status, &bad_line, &n);
  amps_BCast(amps_CommWorld, 0, invoice);
  amps_FreeInvoice(invoice);

  if (status != 0)
  {
    free(read_times);
    free(read_rates);
    if (!amps_Rank(amps_CommWorld))
    {
      switch (status)
      {
        case 1:
          amps_Printf("Error: can't open release file <%s> for reservoir <%s>\n", filename, reservoir_name);
          break;

        case 2:
          amps_Printf("Error: line %d of release file <%s> for reservoir <%s> is not of the form \"time,rate\"\n",
                      bad_line, filename, reservoir_name);
          break;

        case 3:
          amps_Printf("Error: times in release file <%s> for reservoir <%s> must be strictly increasing (line %d)\n",
                      filename, reservoir_name, bad_line);
          break;

        case 5:
          amps_Printf("Error: release rate on line %d of release file <%s> for reservoir <%s> is negative\n",
                      bad_line, filename, reservoir_name);
          break;

        default:
          amps_Printf("Error: release file <%s> for reservoir <%s> has no data rows\n", filename, reservoir_name);
          break;
      }
    }
    exit(1);
  }

  *num_times = n;
  *times = ctalloc(double, n);
  *rates = ctalloc(double, n);

  if (amps_Rank(amps_CommWorld) == 0)
  {
    memcpy(*times, read_times, n * sizeof(double));
    memcpy(*rates, read_rates, n * sizeof(double));
    free(read_times);
    free(read_rates);
  }

  invoice = amps_NewInvoice("%*d%*d", n, *times, n, *rates);
  amps_BCast(amps_CommWorld, 0, invoice);
  amps_FreeInvoice(invoice);
}
