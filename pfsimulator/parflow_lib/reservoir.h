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

#ifndef _RESERVOIR_HEADER
#define _RESERVOIR_HEADER


/*----------------------------------------------------------------
 * Reservoir Physical Values structure
 *----------------------------------------------------------------*/

typedef struct {
    int number;
    char          *name;
//  char* release_curve_file;
//  TimeSeries * release_curve;
    double intake_x_lower, intake_y_lower, z_lower;
    double intake_x_upper, intake_y_upper, z_upper;
    int has_secondary_intake_cell;
    double secondary_intake_x_lower, secondary_intake_y_lower;
    double secondary_intake_x_upper, secondary_intake_y_upper;
    double release_x_lower, release_y_lower, release_z_lower;
    double release_x_upper, release_y_upper, release_z_upper;
    double diameter;
    double Max_Storage, Min_Release_Storage, current_storage, release_rate;
    double intake_amount_since_last_print, release_amount_since_last_print, release_amount_in_solver;
    Subgrid       *intake_subgrid;
    Subgrid       *secondary_intake_subgrid;
    Subgrid       *release_subgrid;
    double size;
} ReservoirDataPhysical;

/*------------------------------------------------------------------
 * Reservoir Data structure
 *----------------------------------------------------------------*/

typedef struct {
    int num_reservoirs;

    /* Pressure reservoir section */
    int num_press_reservoirs;

    ReservoirDataPhysical  **press_reservoir_physicals;

    /* Flux reservoir section */
    int num_flux_reservoirs;

    ReservoirDataPhysical  **flux_reservoir_physicals;


    /* time info */
    TimeCycleData      *time_cycle_data;
} ReservoirData;

/*--------------------------------------------------------------------------
 * Accessor macros: ReservoirDataPhysical
 *--------------------------------------------------------------------------*/
#define ReservoirDataPhysicalNumber(reservoir_data_physical) \
  ((reservoir_data_physical)->number)

//#define ReservoirDataPhysicalReleaseCurveFile(reservoir_data_physical) \
//  ((reservoir_data_physical)->release_curve_file)

#define ReservoirDataPhysicalName(reservoir_data_physical) \
  ((reservoir_data_physical)->name)

#define ReservoirDataPhysicalIntakeXLower(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_x_lower)

#define ReservoirDataPhysicalIntakeYLower(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_y_lower)

#define ReservoirDataPhysicalSecondaryIntakeXLower(reservoir_data_physical) \
  ((reservoir_data_physical)->secondary_intake_x_lower)

#define ReservoirDataPhysicalSecondaryIntakeYLower(reservoir_data_physical) \
  ((reservoir_data_physical)->secondary_intake_y_lower)


#define ReservoirDataPhysicalIntakeXUpper(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_x_upper)

#define ReservoirDataPhysicalIntakeYUpper(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_y_upper)

#define ReservoirDataPhysicalSecondaryIntakeXUpper(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_x_upper)

#define ReservoirDataPhysicalSecondaryIntakeYUpper(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_y_upper)


#define ReservoirDataPhysicalIntakeSubgrid(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_subgrid)

#define ReservoirDataPhysicalSecondaryIntakeSubgrid(reservoir_data_physical) \
  ((reservoir_data_physical)->secondary_intake_subgrid)

#define ReservoirDataPhysicalHasSecondaryIntakeCell(reservoir_data_physical) \
  ((reservoir_data_physical)->has_secondary_intake_cell)

#define ReservoirDataPhysicalReleaseXLower(reservoir_data_physical) \
  ((reservoir_data_physical)->release_x_lower)

#define ReservoirDataPhysicalReleaseYLower(reservoir_data_physical) \
  ((reservoir_data_physical)->release_y_lower)

#define ReservoirDataPhysicalReleaseXUpper(reservoir_data_physical) \
  ((reservoir_data_physical)->release_x_upper)

#define ReservoirDataPhysicalReleaseYUpper(reservoir_data_physical) \
  ((reservoir_data_physical)->release_y_upper)


#define ReservoirDataPhysicalReleaseSubgrid(reservoir_data_physical) \
  ((reservoir_data_physical)->release_subgrid)

#define ReservoirDataPhysicalDiameter(reservoir_data_physical) \
  ((reservoir_data_physical)->diameter)

#define ReservoirDataPhysicalSize(reservoir_data_physical) \
  ((reservoir_data_physical)->size)


#define ReservoirDataPhysicalMaxCapacity(reservoir_data_physical) \
  ((reservoir_data_physical)->Max_Storage)

#define ReservoirDataPhysicalCurrentCapacity(reservoir_data_physical) \
  ((reservoir_data_physical)->current_storage)

#define ReservoirDataPhysicalReleaseAmountSinceLastPrint(reservoir_data_physical) \
  ((reservoir_data_physical)->release_amount_since_last_print)

#define ReservoirDataPhysicalIntakeAmountSinceLastPrint(reservoir_data_physical) \
  ((reservoir_data_physical)->intake_amount_since_last_print)

#define ReservoirDataPhysicalReleaseAmountInSolver(reservoir_data_physical) \
  ((reservoir_data_physical)->release_amount_in_solver)

#define ReservoirDataPhysicalMinReleaseCapacity(reservoir_data_physical) \
  ((reservoir_data_physical)->Min_Release_Storage)

#define ReservoirDataPhysicalReleaseRate(reservoir_data_physical) \
  ((reservoir_data_physical)->release_rate)



/*--------------------------------------------------------------------------
 * Accessor macros: ReservoirData
 *--------------------------------------------------------------------------*/
#define ReservoirDataNumReservoirs(reservoir_data) ((reservoir_data)->num_reservoirs)

#define ReservoirDataTimeCycleData(reservoir_data) ((reservoir_data)->time_cycle_data)

/*-------------------------- Pressure reservoir data ----------------------------*/
#define ReservoirDataNumPressReservoirs(reservoir_data) ((reservoir_data)->num_press_reservoirs)

#define ReservoirDataPressReservoirPhysicals(reservoir_data) \
  ((reservoir_data)->press_reservoir_physicals)
#define ReservoirDataPressReservoirPhysical(reservoir_data, i) \
  ((reservoir_data)->press_reservoir_physicals[i])


#define ReservoirDataPressReservoirIntervalValue(reservoir_data, i, interval_number) \
  (((reservoir_data)->press_reservoir_values[i])[interval_number])


#define ReservoirDataPressReservoirStat(reservoir_data, i) \
  ((reservoir_data)->press_reservoir_stats[i])

/*---------------------------- Flux reservoir data ------------------------------*/
#define ReservoirDataNumFluxReservoirs(reservoir_data)      ((reservoir_data)->num_flux_reservoirs)

#define ReservoirDataFluxReservoirPhysicals(reservoir_data) \
  ((reservoir_data)->flux_reservoir_physicals)
#define ReservoirDataFluxReservoirPhysical(reservoir_data, i) \
  ((reservoir_data)->flux_reservoir_physicals[i])




/*--------------------------------------------------------------------------
 * Reservoir Data constants used in the program.
 *--------------------------------------------------------------------------*/

/*       Actions        */
#define INJECTION_RESERVOIR  0
#define EXTRACTION_RESERVOIR 1

#define RESERVOIR_ON_STATUS 1
#define RESERVOIR_OFF_STATUS 1

/*       Methods        */
/* These should match  with */
/* the Name Array Defs from */
/* the  Reservoir Package input. */
#define PRESS_STANDARD 0
#define FLUX_STANDARD  0
#define FLUX_WEIGHTED  1
#define FLUX_PATTERNED 2

/*     Write Options      */
#define RESERVOIRDATA_DONTWRITEHEADER 0
#define RESERVOIRDATA_WRITEHEADER     1

/*     Print Flags      */
#define RESERVOIRDATA_PRINTPHYSICAL  0x0001
#define RESERVOIRDATA_PRINTVALUES    0x0002
#define RESERVOIRDATA_PRINTSTATS     0x0004

#endif