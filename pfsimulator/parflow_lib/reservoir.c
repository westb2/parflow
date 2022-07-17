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

#include <string.h>


/*****************************************************************************
*
* The functions in this file are for manipulating the ReservoirData structure
*   in ProblemData and work in conjuction with the ReservoirPackage module.
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
  ReservoirData    *well_data;

  well_data = ctalloc(ReservoirData, 1);

  ReservoirDataNumPhases(well_data) = 0;
  ReservoirDataNumContaminants(well_data) = 0;

  ReservoirDataNumReservoirs(well_data) = -1;

  ReservoirDataTimeCycleData(well_data) = NULL;

  ReservoirDataNumPressReservoirs(well_data) = -1;
  ReservoirDataPressReservoirPhysicals(well_data) = NULL;
  ReservoirDataPressReservoirValues(well_data) = NULL;
  ReservoirDataPressReservoirStats(well_data) = NULL;

  ReservoirDataNumFluxReservoirs(well_data) = -1;
  ReservoirDataFluxReservoirPhysicals(well_data) = NULL;
  ReservoirDataFluxReservoirValues(well_data) = NULL;
  ReservoirDataFluxReservoirStats(well_data) = NULL;

  ReservoirDataNumReservoirs(well_data) = -1;
  ReservoirDataReservoirReservoirPhysicals(well_data) = NULL;
  ReservoirDataReservoirReservoirValues(well_data) = NULL;
  ReservoirDataReservoirstats(well_data) = NULL;

  return well_data;
}


/*--------------------------------------------------------------------------
 * FreeReservoirData
 *--------------------------------------------------------------------------*/

void FreeReservoirData(
    ReservoirData *well_data)
{
  ReservoirDataPhysical *well_data_physical;
  ReservoirDataValue    *well_data_value;
  ReservoirDataStat     *well_data_stat;
  int i, cycle_number, interval_division, interval_number;

  TimeCycleData   *time_cycle_data;

  if (well_data)
  {
    if (ReservoirDataNumReservoirs(well_data) > 0)
    {
      time_cycle_data = ReservoirDataTimeCycleData(well_data);

      if (ReservoirDataNumFluxReservoirs(well_data) > 0)
      {
        for (i = 0; i < ReservoirDataNumFluxReservoirs(well_data); i++)
        {
          well_data_stat = ReservoirDataFluxReservoirStat(well_data, i);
          if (ReservoirDataStatDeltaPhases(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaPhases(well_data_stat));
          }
          if (ReservoirDataStatPhaseStats(well_data_stat))
          {
            tfree(ReservoirDataStatPhaseStats(well_data_stat));
          }
          if (ReservoirDataStatDeltaSaturations(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaSaturations(well_data_stat));
          }
          if (ReservoirDataStatSaturationStats(well_data_stat))
          {
            tfree(ReservoirDataStatSaturationStats(well_data_stat));
          }
          if (ReservoirDataStatDeltaContaminants(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaContaminants(well_data_stat));
          }
          if (ReservoirDataStatContaminantStats(well_data_stat))
          {
            tfree(ReservoirDataStatContaminantStats(well_data_stat));
          }
          tfree(well_data_stat);
        }
        if (ReservoirDataFluxReservoirStats(well_data))
        {
          tfree(ReservoirDataFluxReservoirStats(well_data));
        }
        for (i = 0; i < ReservoirDataNumFluxReservoirs(well_data); i++)
        {
          well_data_physical = ReservoirDataFluxReservoirPhysical(well_data, i);
          cycle_number = ReservoirDataPhysicalCycleNumber(well_data_physical);
          interval_division = TimeCycleDataIntervalDivision(time_cycle_data, cycle_number);

          for (interval_number = 0; interval_number < interval_division; interval_number++)
          {
            well_data_value = ReservoirDataFluxReservoirIntervalValue(well_data, i, interval_number);
            if (ReservoirDataValuePhaseValues(well_data_value))
            {
              tfree(ReservoirDataValuePhaseValues(well_data_value));
            }
            if (ReservoirDataPhysicalAction(well_data_physical) == INJECTION_WELL)
            {
              if (ReservoirDataValueSaturationValues(well_data_value))
              {
                tfree(ReservoirDataValueSaturationValues(well_data_value));
              }
              if (ReservoirDataValueContaminantValues(well_data_value))
              {
                tfree(ReservoirDataValueContaminantValues(well_data_value));
              }
            }
            if (ReservoirDataValueContaminantFractions(well_data_value))
            {
              tfree(ReservoirDataValueContaminantFractions(well_data_value));
            }
            tfree(well_data_value);
          }
          if (ReservoirDataFluxReservoirIntervalValues(well_data, i))
          {
            tfree(ReservoirDataFluxReservoirIntervalValues(well_data, i));
          }
        }
        if (ReservoirDataFluxReservoirValues(well_data))
        {
          tfree(ReservoirDataFluxReservoirValues(well_data));
        }
        for (i = 0; i < ReservoirDataNumFluxReservoirs(well_data); i++)
        {
          well_data_physical = ReservoirDataFluxReservoirPhysical(well_data, i);
          if (ReservoirDataPhysicalName(well_data_physical))
          {
            tfree(ReservoirDataPhysicalName(well_data_physical));
          }
          if (ReservoirDataPhysicalSubgrid(well_data_physical))
          {
            FreeSubgrid(ReservoirDataPhysicalSubgrid(well_data_physical));
          }
          tfree(well_data_physical);
        }
        if (ReservoirDataFluxReservoirPhysicals(well_data))
        {
          tfree(ReservoirDataFluxReservoirPhysicals(well_data));
        }
      }

      if (ReservoirDataNumPressReservoirs(well_data) > 0)
      {
        for (i = 0; i < ReservoirDataNumPressReservoirs(well_data); i++)
        {
          well_data_stat = ReservoirDataPressReservoirStat(well_data, i);
          if (ReservoirDataStatDeltaPhases(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaPhases(well_data_stat));
          }
          if (ReservoirDataStatPhaseStats(well_data_stat))
          {
            tfree(ReservoirDataStatPhaseStats(well_data_stat));
          }
          if (ReservoirDataStatDeltaSaturations(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaSaturations(well_data_stat));
          }
          if (ReservoirDataStatSaturationStats(well_data_stat))
          {
            tfree(ReservoirDataStatSaturationStats(well_data_stat));
          }
          if (ReservoirDataStatDeltaContaminants(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaContaminants(well_data_stat));
          }
          if (ReservoirDataStatContaminantStats(well_data_stat))
          {
            tfree(ReservoirDataStatContaminantStats(well_data_stat));
          }
          tfree(well_data_stat);
        }
        if (ReservoirDataPressReservoirStats(well_data))
        {
          tfree(ReservoirDataPressReservoirStats(well_data));
        }
        for (i = 0; i < ReservoirDataNumPressReservoirs(well_data); i++)
        {
          well_data_physical = ReservoirDataPressReservoirPhysical(well_data, i);
          cycle_number = ReservoirDataPhysicalCycleNumber(well_data_physical);
          interval_division = TimeCycleDataIntervalDivision(time_cycle_data, cycle_number);

          for (interval_number = 0; interval_number < interval_division; interval_number++)
          {
            well_data_value = ReservoirDataPressReservoirIntervalValue(well_data, i, interval_number);
            if (ReservoirDataValuePhaseValues(well_data_value))
            {
              tfree(ReservoirDataValuePhaseValues(well_data_value));
            }
            if (ReservoirDataPhysicalAction(well_data_physical) == INJECTION_WELL)
            {
              if (ReservoirDataValueSaturationValues(well_data_value))
              {
                tfree(ReservoirDataValueSaturationValues(well_data_value));
              }
              if (ReservoirDataValueContaminantValues(well_data_value))
              {
                tfree(ReservoirDataValueContaminantValues(well_data_value));
              }
            }
            if (ReservoirDataValueContaminantFractions(well_data_value))
            {
              tfree(ReservoirDataValueContaminantFractions(well_data_value));
            }
            tfree(well_data_value);
          }
          if (ReservoirDataPressReservoirIntervalValues(well_data, i))
          {
            tfree(ReservoirDataPressReservoirIntervalValues(well_data, i));
          }
        }
        if (ReservoirDataPressReservoirValues(well_data))
        {
          tfree(ReservoirDataPressReservoirValues(well_data));
        }
        for (i = 0; i < ReservoirDataNumPressReservoirs(well_data); i++)
        {
          well_data_physical = ReservoirDataPressReservoirPhysical(well_data, i);
          if (ReservoirDataPhysicalName(well_data_physical))
          {
            tfree(ReservoirDataPhysicalName(well_data_physical));
          }
          if (ReservoirDataPhysicalSubgrid(well_data_physical))
          {
            FreeSubgrid(ReservoirDataPhysicalSubgrid(well_data_physical));
          }
          tfree(well_data_physical);
        }
        if (ReservoirDataPressReservoirPhysicals(well_data))
        {
          tfree(ReservoirDataPressReservoirPhysicals(well_data));
        }
      }

      if (ReservoirDataNumReservoirs(well_data) > 0)
      {
        for (i = 0; i < ReservoirDataNumReservoirs(well_data); i++)
        {
          well_data_stat = ReservoirDataReservoirstat(well_data, i);
          if (ReservoirDataStatDeltaPhases(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaPhases(well_data_stat));
          }
          if (ReservoirDataStatPhaseStats(well_data_stat))
          {
            tfree(ReservoirDataStatPhaseStats(well_data_stat));
          }
          if (ReservoirDataStatDeltaSaturations(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaSaturations(well_data_stat));
          }
          if (ReservoirDataStatSaturationStats(well_data_stat))
          {
            tfree(ReservoirDataStatSaturationStats(well_data_stat));
          }
          if (ReservoirDataStatDeltaContaminants(well_data_stat))
          {
            tfree(ReservoirDataStatDeltaContaminants(well_data_stat));
          }
          if (ReservoirDataStatContaminantStats(well_data_stat))
          {
            tfree(ReservoirDataStatContaminantStats(well_data_stat));
          }
          tfree(well_data_stat);
        }
        if (ReservoirDataReservoirstats(well_data))
        {
          tfree(ReservoirDataReservoirstats(well_data));
        }
        for (i = 0; i < ReservoirDataNumReservoirs(well_data); i++)
        {
          well_data_physical = ReservoirDataReservoirReservoirPhysical(well_data, i);
          cycle_number = ReservoirDataPhysicalCycleNumber(well_data_physical);
          interval_division = TimeCycleDataIntervalDivision(time_cycle_data, cycle_number);

          for (interval_number = 0; interval_number < interval_division; interval_number++)
          {
            well_data_value = ReservoirDataReservoirReservoirIntervalValue(well_data, i, interval_number);
            if (ReservoirDataValuePhaseValues(well_data_value))
            {
              tfree(ReservoirDataValuePhaseValues(well_data_value));
            }
            if (ReservoirDataPhysicalAction(well_data_physical) == INJECTION_WELL)
            {
              if (ReservoirDataValueSaturationValues(well_data_value))
              {
                tfree(ReservoirDataValueSaturationValues(well_data_value));
              }
              if (ReservoirDataValueContaminantValues(well_data_value))
              {
                tfree(ReservoirDataValueContaminantValues(well_data_value));
              }
            }
            if (ReservoirDataValueContaminantFractions(well_data_value))
            {
              tfree(ReservoirDataValueContaminantFractions(well_data_value));
            }
            tfree(well_data_value);
          }
          if (ReservoirDataReservoirReservoirIntervalValues(well_data, i))
          {
            tfree(ReservoirDataReservoirReservoirIntervalValues(well_data, i));
          }
        }
        if (ReservoirDataReservoirReservoirValues(well_data))
        {
          tfree(ReservoirDataReservoirReservoirValues(well_data));
        }
        for (i = 0; i < ReservoirDataNumReservoirs(well_data); i++)
        {
          well_data_physical = ReservoirDataReservoirReservoirPhysical(well_data, i);
          if (ReservoirDataPhysicalName(well_data_physical))
          {
            tfree(ReservoirDataPhysicalName(well_data_physical));
          }
          if (ReservoirDataPhysicalSubgrid(well_data_physical))
          {
            FreeSubgrid(ReservoirDataPhysicalSubgrid(well_data_physical));
          }
          tfree(well_data_physical);
        }
        if (ReservoirDataReservoirReservoirPhysicals(well_data))
        {
          tfree(ReservoirDataReservoirReservoirPhysicals(well_data));
        }
      }
      FreeTimeCycleData(time_cycle_data);
    }
    tfree(well_data);
  }
}


/*--------------------------------------------------------------------------
 * PrintReservoirData
 *--------------------------------------------------------------------------*/

void PrintReservoirData(
    ReservoirData *   well_data,
    unsigned int print_mask)
{
  TimeCycleData    *time_cycle_data;

  ReservoirDataPhysical *well_data_physical;
  ReservoirDataValue    *well_data_value;
  ReservoirDataStat     *well_data_stat;

  Subgrid          *subgrid;

  int cycle_number, interval_division, interval_number;
  int well, phase, concentration, indx;
  double value, stat;

  amps_Printf("Reservoir Information\n");
  if (ReservoirDataNumReservoirs(well_data) == -1)
  {
    amps_Printf("Reservoirs have not been setup.\n");
  }
  else if (ReservoirDataNumReservoirs(well_data) == 0)
  {
    amps_Printf("No Reservoirs.\n");
  }
  else
  {
    time_cycle_data = ReservoirDataTimeCycleData(well_data);

    PrintTimeCycleData(time_cycle_data);

    if (ReservoirDataNumFluxReservoirs(well_data) > 0)
    {
      amps_Printf("Info on Flux Reservoirs :\n");
      for (well = 0; well < ReservoirDataNumFluxReservoirs(well_data); well++)
      {
        amps_Printf(" Flux Reservoir Number : %02d\n", well);

        if ((print_mask & WELLDATA_PRINTPHYSICAL))
        {
          amps_Printf("  Reservoir Physical Data :\n");
          well_data_physical = ReservoirDataFluxReservoirPhysical(well_data, well);

          amps_Printf("   sequence number = %2d\n",
                      ReservoirDataPhysicalNumber(well_data_physical));
          amps_Printf("   name = %s\n",
                      ReservoirDataPhysicalName(well_data_physical));
          amps_Printf("   x_lower, y_lower, z_lower = %f %f %f\n",
                      ReservoirDataPhysicalXLower(well_data_physical),
                      ReservoirDataPhysicalYLower(well_data_physical),
                      ReservoirDataPhysicalZLower(well_data_physical));
          amps_Printf("   x_upper, y_upper, z_upper = %f %f %f\n",
                      ReservoirDataPhysicalXUpper(well_data_physical),
                      ReservoirDataPhysicalYUpper(well_data_physical),
                      ReservoirDataPhysicalZUpper(well_data_physical));
          amps_Printf("   diameter = %f\n",
                      ReservoirDataPhysicalDiameter(well_data_physical));

          subgrid = ReservoirDataPhysicalSubgrid(well_data_physical);
          amps_Printf("   (ix, iy, iz) = (%d, %d, %d)\n",
                      SubgridIX(subgrid),
                      SubgridIY(subgrid),
                      SubgridIZ(subgrid));
          amps_Printf("   (nx, ny, nz) = (%d, %d, %d)\n",
                      SubgridNX(subgrid),
                      SubgridNY(subgrid),
                      SubgridNZ(subgrid));
          amps_Printf("   (rx, ry, rz) = (%d, %d, %d)\n",
                      SubgridRX(subgrid),
                      SubgridRY(subgrid),
                      SubgridRZ(subgrid));
          amps_Printf("   process = %d\n",
                      SubgridProcess(subgrid));
          amps_Printf("   size = %f\n",
                      ReservoirDataPhysicalSize(well_data_physical));
          amps_Printf("   action = %d\n",
                      ReservoirDataPhysicalAction(well_data_physical));
          amps_Printf("   method = %d\n",
                      ReservoirDataPhysicalMethod(well_data_physical));
        }

        if ((print_mask & WELLDATA_PRINTVALUES))
        {
          amps_Printf("  Reservoir Values :\n");
          well_data_physical = ReservoirDataFluxReservoirPhysical(well_data, well);
          cycle_number = ReservoirDataPhysicalCycleNumber(well_data_physical);
          interval_division = TimeCycleDataIntervalDivision(time_cycle_data, cycle_number);

          for (interval_number = 0; interval_number < interval_division; interval_number++)
          {
            amps_Printf("  Value[%2d] :\n", interval_number);
            well_data_value = ReservoirDataFluxReservoirIntervalValue(well_data, well, interval_number);

            if (ReservoirDataValuePhaseValues(well_data_value))
            {
              for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
              {
                value = ReservoirDataValuePhaseValue(well_data_value, phase);
                amps_Printf("   value for phase %01d = %f\n", phase, value);
              }
            }
            else
            {
              amps_Printf("   no phase values present.\n");
            }
            if (ReservoirDataPhysicalAction(well_data_physical) == INJECTION_WELL)
            {
              if (ReservoirDataValueSaturationValues(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  value = ReservoirDataValueSaturationValue(well_data_value, phase);
                  amps_Printf("   s_bar[%01d] = %f\n", phase, value);
                }
              }
              else
              {
                amps_Printf("   no saturation values present.\n");
              }
              if (ReservoirDataValueDeltaSaturationPtrs(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  value = ReservoirDataValueDeltaSaturationPtr(well_data_value, phase);
                  amps_Printf("   delta_saturations[%01d] = %f\n", phase, value);
                }
              }
              else
              {
                amps_Printf("   no saturation delta values present.\n");
              }
              if (ReservoirDataValueContaminantValues(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
                  {
                    indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                    value = ReservoirDataValueContaminantValue(well_data_value, indx);
                    amps_Printf("   c_bar[%01d][%02d] = %f\n", phase, concentration, value);
                  }
                }
              }
              else
              {
                amps_Printf("   no contaminant values present.\n");
              }
              if (ReservoirDataValueDeltaContaminantPtrs(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
                  {
                    indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                    value = ReservoirDataValueDeltaContaminantPtr(well_data_value, indx);
                    amps_Printf("  delta_concentration[%01d][%02d] = %f\n", phase, concentration, value);
                  }
                }
              }
              else
              {
                amps_Printf("   no concentration delta values present.\n");
              }
            }
            if (ReservoirDataValueContaminantFractions(well_data_value))
            {
              for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
              {
                for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
                {
                  indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                  value = ReservoirDataValueContaminantFraction(well_data_value, indx);
                  amps_Printf("   relevant_fraction[%01d][%02d] = %f\n", phase, concentration, value);
                }
              }
            }
            else
            {
              amps_Printf("   no relevant component fractions present.\n");
            }
          }
        }

        if ((print_mask & WELLDATA_PRINTSTATS))
        {
          amps_Printf("  Reservoir Stats :\n");
          well_data_stat = ReservoirDataFluxReservoirStat(well_data, well);
          if (ReservoirDataStatDeltaPhases(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatDeltaPhase(well_data_stat, phase);
              amps_Printf("  delta_p[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatPhaseStats(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatPhaseStat(well_data_stat, phase);
              amps_Printf("  phase[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatDeltaSaturations(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatDeltaSaturation(well_data_stat, phase);
              amps_Printf("  delta_s[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatSaturationStats(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatSaturationStat(well_data_stat, phase);
              amps_Printf("  saturation[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatDeltaContaminants(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                stat = ReservoirDataStatDeltaContaminant(well_data_stat, indx);
                amps_Printf("  delta_c[%01d][%02d] = %f\n", phase, concentration, stat);
              }
            }
          }
          if (ReservoirDataStatContaminantStats(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                stat = ReservoirDataStatContaminantStat(well_data_stat, indx);
                amps_Printf("  concentration[%01d][%02d] = %f\n", phase, concentration, stat);
              }
            }
          }
        }
      }
    }
    else
    {
      amps_Printf("No Flux Reservoirs.\n");
    }

    if (ReservoirDataNumPressReservoirs(well_data) > 0)
    {
      amps_Printf("Info on Pressure Reservoirs :\n");
      for (well = 0; well < ReservoirDataNumPressReservoirs(well_data); well++)
      {
        amps_Printf(" Pressure Reservoir Number : %2d\n", well);
        if ((print_mask & WELLDATA_PRINTPHYSICAL))
        {
          amps_Printf("  Reservoir Physical Data :\n");
          well_data_physical = ReservoirDataPressReservoirPhysical(well_data, well);

          amps_Printf("   sequence number = %2d\n",
                      ReservoirDataPhysicalNumber(well_data_physical));
          amps_Printf("   name = %s\n",
                      ReservoirDataPhysicalName(well_data_physical));
          amps_Printf("   x_lower, y_lower, z_lower = %f %f %f\n",
                      ReservoirDataPhysicalXLower(well_data_physical),
                      ReservoirDataPhysicalYLower(well_data_physical),
                      ReservoirDataPhysicalZLower(well_data_physical));
          amps_Printf("   x_upper, y_upper, z_upper = %f %f %f\n",
                      ReservoirDataPhysicalXUpper(well_data_physical),
                      ReservoirDataPhysicalYUpper(well_data_physical),
                      ReservoirDataPhysicalZUpper(well_data_physical));
          amps_Printf("   diameter = %f\n",
                      ReservoirDataPhysicalDiameter(well_data_physical));

          subgrid = ReservoirDataPhysicalSubgrid(well_data_physical);
          amps_Printf("   (ix, iy, iz) = (%d, %d, %d)\n",
                      SubgridIX(subgrid),
                      SubgridIY(subgrid),
                      SubgridIZ(subgrid));
          amps_Printf("   (nx, ny, nz) = (%d, %d, %d)\n",
                      SubgridNX(subgrid),
                      SubgridNY(subgrid),
                      SubgridNZ(subgrid));
          amps_Printf("   (rx, ry, rz) = (%d, %d, %d)\n",
                      SubgridRX(subgrid),
                      SubgridRY(subgrid),
                      SubgridRZ(subgrid));
          amps_Printf("   process = %d\n",
                      SubgridProcess(subgrid));
          amps_Printf("   size = %f\n",
                      ReservoirDataPhysicalSize(well_data_physical));
          amps_Printf("   action = %d\n",
                      ReservoirDataPhysicalAction(well_data_physical));
          amps_Printf("   method = %d\n",
                      ReservoirDataPhysicalMethod(well_data_physical));
        }

        if ((print_mask & WELLDATA_PRINTVALUES))
        {
          amps_Printf("  Reservoir Values :\n");
          well_data_physical = ReservoirDataPressReservoirPhysical(well_data, well);
          cycle_number = ReservoirDataPhysicalCycleNumber(well_data_physical);
          interval_division = TimeCycleDataIntervalDivision(time_cycle_data, cycle_number);

          for (interval_number = 0; interval_number < interval_division; interval_number++)
          {
            amps_Printf("  Value[%2d] :\n", interval_number);

            well_data_value = ReservoirDataPressReservoirIntervalValue(well_data, well, interval_number);

            if (ReservoirDataValuePhaseValues(well_data_value))
            {
              phase = 0;
              {
                value = ReservoirDataValuePhaseValue(well_data_value, phase);
                amps_Printf("   value for phase %01d = %f\n", phase, value);
              }
            }
            else
            {
              amps_Printf("   no phase values present.\n");
            }
            if (ReservoirDataPhysicalAction(well_data_physical) == INJECTION_WELL)
            {
              if (ReservoirDataValueSaturationValues(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  value = ReservoirDataValueSaturationValue(well_data_value, phase);
                  amps_Printf("   s_bar[%01d] = %f\n", phase, value);
                }
              }
              else
              {
                amps_Printf("   no saturation values present.\n");
              }
              if (ReservoirDataValueDeltaSaturationPtrs(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  value = ReservoirDataValueDeltaSaturationPtr(well_data_value, phase);
                  amps_Printf("   delta_saturations[%01d] = %f\n", phase, value);
                }
              }
              else
              {
                amps_Printf("   no saturation delta values present.\n");
              }
              if (ReservoirDataValueContaminantValues(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
                  {
                    indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                    value = ReservoirDataValueContaminantValue(well_data_value, indx);
                    amps_Printf("   c_bar[%01d][%02d] = %f\n", phase, concentration, value);
                  }
                }
              }
              else
              {
                amps_Printf("   no component values present.\n");
              }
              if (ReservoirDataValueDeltaContaminantPtrs(well_data_value))
              {
                for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
                {
                  for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
                  {
                    indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                    value = ReservoirDataValueDeltaContaminantPtr(well_data_value, indx);
                    amps_Printf("  delta_concentration[%01d][%02d] = %f\n", phase, concentration, value);
                  }
                }
              }
              else
              {
                amps_Printf("   no concentration delta values present.\n");
              }
            }
            if (ReservoirDataValueContaminantFractions(well_data_value))
            {
              for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
              {
                for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
                {
                  indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                  value = ReservoirDataValueContaminantFraction(well_data_value, indx);
                  amps_Printf("   relevant_fraction[%01d][%02d] = %f\n", phase, concentration, value);
                }
              }
            }
            else
            {
              amps_Printf("   no relevant component values present.\n");
            }
          }
        }

        if ((print_mask & WELLDATA_PRINTSTATS))
        {
          amps_Printf("  Reservoir Stats :\n");
          well_data_stat = ReservoirDataPressReservoirStat(well_data, well);
          if (ReservoirDataStatDeltaPhases(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatDeltaPhase(well_data_stat, phase);
              amps_Printf("  delta_p[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatPhaseStats(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatPhaseStat(well_data_stat, phase);
              amps_Printf("  phase[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatDeltaSaturations(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatDeltaSaturation(well_data_stat, phase);
              amps_Printf("  delta_s[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatSaturationStats(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              stat = ReservoirDataStatSaturationStat(well_data_stat, phase);
              amps_Printf("  saturation[%01d] = %f\n", phase, stat);
            }
          }
          if (ReservoirDataStatDeltaContaminants(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                stat = ReservoirDataStatDeltaContaminant(well_data_stat, indx);
                amps_Printf("  delta_c[%01d][%02d] = %f\n", phase, concentration, stat);
              }
            }
          }
          if (ReservoirDataStatContaminantStats(well_data_stat))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                stat = ReservoirDataStatContaminantStat(well_data_stat, indx);
                amps_Printf("  concentration[%01d][%02d] = %f\n", phase, concentration, stat);
              }
            }
          }
        }
      }
    }
    else
    {
      amps_Printf("No Pressure Reservoirs.\n");
    }
  }
}

/*--------------------------------------------------------------------------
 * WriteReservoirs
 *--------------------------------------------------------------------------*/

void WriteReservoirs(
    char *    file_prefix,
    Problem * problem,
    ReservoirData *well_data,
    double    time,
    int       write_header)
{
  TimeCycleData    *time_cycle_data;
  ReservoirDataPhysical *well_data_physical;
  ReservoirDataValue    *well_data_value;
  ReservoirDataStat     *well_data_stat;

  Subgrid          *subgrid;

  int cycle_number, interval_number;
  int well, phase, concentration, indx;
  double value, stat;

  FILE             *file;

  char file_suffix[7] = "wells";
  char filename[255];

  int p;
  if (ReservoirDataNumReservoirs(well_data) > 0)
  {
    p = amps_Rank(amps_CommWorld);

    time_cycle_data = ReservoirDataTimeCycleData(well_data);

    if (p == 0)
    {
      sprintf(filename, "%s.%s", file_prefix, file_suffix);

      if (write_header)
      {
        file = fopen(filename, "w");
      }
      else
      {
        file = fopen(filename, "a");
      }

      if (file == NULL)
      {
        amps_Printf("Error: can't open output file %s\n", filename);
        exit(1);
      }

      if (write_header)
      {
        fprintf(file, "%f %f %f %d %d %d %f %f %f\n",
                BackgroundX(GlobalsBackground),
                BackgroundY(GlobalsBackground),
                BackgroundZ(GlobalsBackground),
                BackgroundNX(GlobalsBackground),
                BackgroundNY(GlobalsBackground),
                BackgroundNZ(GlobalsBackground),
                BackgroundDX(GlobalsBackground),
                BackgroundDY(GlobalsBackground),
                BackgroundDZ(GlobalsBackground));

        fprintf(file, "%d %d %d\n",
                ReservoirDataNumPhases(well_data),
                ReservoirDataNumContaminants(well_data),
                ReservoirDataNumReservoirs(well_data));

        for (well = 0; well < ReservoirDataNumFluxReservoirs(well_data); well++)
        {
          well_data_physical = ReservoirDataFluxReservoirPhysical(well_data, well);

          fprintf(file, "%2d\n", ReservoirDataPhysicalNumber(well_data_physical));

          fprintf(file, "%d\n", (int)strlen(ReservoirDataPhysicalName(well_data_physical)));
          fprintf(file, "%s\n", ReservoirDataPhysicalName(well_data_physical));

          fprintf(file, "%f %f %f %f %f %f %f\n",
                  ReservoirDataPhysicalXLower(well_data_physical),
                  ReservoirDataPhysicalYLower(well_data_physical),
                  ReservoirDataPhysicalZLower(well_data_physical),
                  ReservoirDataPhysicalXUpper(well_data_physical),
                  ReservoirDataPhysicalYUpper(well_data_physical),
                  ReservoirDataPhysicalZUpper(well_data_physical),
                  ReservoirDataPhysicalDiameter(well_data_physical));
          fprintf(file, "1 %1d %1d\n",
                  ReservoirDataPhysicalAction(well_data_physical),
                  ReservoirDataPhysicalMethod(well_data_physical));
        }
        for (well = 0; well < ReservoirDataNumPressReservoirs(well_data); well++)
        {
          well_data_physical = ReservoirDataPressReservoirPhysical(well_data, well);

          fprintf(file, "%2d\n", ReservoirDataPhysicalNumber(well_data_physical));

          fprintf(file, "%d\n", (int)strlen(ReservoirDataPhysicalName(well_data_physical)));
          fprintf(file, "%s\n", ReservoirDataPhysicalName(well_data_physical));

          fprintf(file, "%f %f %f %f %f %f %f\n",
                  ReservoirDataPhysicalXLower(well_data_physical),
                  ReservoirDataPhysicalYLower(well_data_physical),
                  ReservoirDataPhysicalZLower(well_data_physical),
                  ReservoirDataPhysicalXUpper(well_data_physical),
                  ReservoirDataPhysicalYUpper(well_data_physical),
                  ReservoirDataPhysicalZUpper(well_data_physical),
                  ReservoirDataPhysicalDiameter(well_data_physical));
          fprintf(file, "0 %1d %1d\n",
                  ReservoirDataPhysicalAction(well_data_physical),
                  ReservoirDataPhysicalMethod(well_data_physical));
        }
      }

      fprintf(file, "%f\n", time);

      for (well = 0; well < ReservoirDataNumFluxReservoirs(well_data); well++)
      {
        /* Write out important current physical data */
        well_data_physical = ReservoirDataFluxReservoirPhysical(well_data, well);
        fprintf(file, "%2d\n", ReservoirDataPhysicalNumber(well_data_physical));
        subgrid = ReservoirDataPhysicalSubgrid(well_data_physical);
        fprintf(file, "%d %d %d %d %d %d %d %d %d\n",
                SubgridIX(subgrid),
                SubgridIY(subgrid),
                SubgridIZ(subgrid),
                SubgridNX(subgrid),
                SubgridNY(subgrid),
                SubgridNZ(subgrid),
                SubgridRX(subgrid),
                SubgridRY(subgrid),
                SubgridRZ(subgrid));

        /* Write out the current well values */
        cycle_number = ReservoirDataPhysicalCycleNumber(well_data_physical);
        interval_number = TimeCycleDataComputeIntervalNumber(problem, time, time_cycle_data, cycle_number);

        well_data_value = ReservoirDataFluxReservoirIntervalValue(well_data, well, interval_number);

        if (ReservoirDataValuePhaseValues(well_data_value))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            value = ReservoirDataValuePhaseValue(well_data_value, phase);
            fprintf(file, " %f", value);
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataPhysicalAction(well_data_physical) == INJECTION_WELL)
        {
          if (ReservoirDataValueDeltaSaturationPtrs(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              value = ReservoirDataValueDeltaSaturationPtr(well_data_value, phase);
              fprintf(file, " %f", value);
            }
            fprintf(file, "\n");
          }
          else if (ReservoirDataValueSaturationValues(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              value = ReservoirDataValueSaturationValue(well_data_value, phase);
              fprintf(file, " %f", value);
            }
            fprintf(file, "\n");
          }
          if (ReservoirDataValueDeltaContaminantPtrs(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                value = ReservoirDataValueContaminantFraction(well_data_value, indx)
                        * fabs(ReservoirDataValueDeltaContaminantPtr(well_data_value, indx))
                        / ReservoirDataPhysicalSize(well_data_physical);
                fprintf(file, " %f", value);
              }
            }
            fprintf(file, "\n");
          }
          else if (ReservoirDataValueContaminantValues(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                value = ReservoirDataValueContaminantValue(well_data_value, indx);
                fprintf(file, " %f", value);
              }
            }
            fprintf(file, "\n");
          }
        }
        if (ReservoirDataValueContaminantFractions(well_data_value))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
            {
              indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
              value = ReservoirDataValueContaminantFraction(well_data_value, indx);
              fprintf(file, " %f", value);
            }
          }
          fprintf(file, "\n");
        }

        /* Write out the current well statistics */
        well_data_stat = ReservoirDataFluxReservoirStat(well_data, well);
        if (ReservoirDataStatPhaseStats(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            stat = ReservoirDataStatPhaseStat(well_data_stat, phase);
            fprintf(file, " %f", stat);
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataStatSaturationStats(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            stat = ReservoirDataStatSaturationStat(well_data_stat, phase);
            fprintf(file, " %f", stat);
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataStatContaminantStats(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
            {
              indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
              stat = ReservoirDataStatContaminantStat(well_data_stat, indx);
              fprintf(file, " %f", stat);
            }
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataStatDeltaPhases(well_data_stat) && ReservoirDataStatDeltaContaminants(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
            {
              indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
              if (ReservoirDataStatDeltaPhase(well_data_stat, phase) == 0.0)
              {
                stat = 0.0;
              }
              else
              {
                stat = ReservoirDataStatDeltaContaminant(well_data_stat, indx) / ReservoirDataStatDeltaPhase(well_data_stat, phase);
              }
              fprintf(file, " %f", stat);
            }
          }
          fprintf(file, "\n");
        }
      }

      for (well = 0; well < ReservoirDataNumPressReservoirs(well_data); well++)
      {
        /* Write out important current physical data */
        well_data_physical = ReservoirDataPressReservoirPhysical(well_data, well);
        fprintf(file, "%2d\n", ReservoirDataPhysicalNumber(well_data_physical));
        subgrid = ReservoirDataPhysicalSubgrid(well_data_physical);
        fprintf(file, "%d %d %d %d %d %d %d %d %d\n",
                SubgridIX(subgrid),
                SubgridIY(subgrid),
                SubgridIZ(subgrid),
                SubgridNX(subgrid),
                SubgridNY(subgrid),
                SubgridNZ(subgrid),
                SubgridRX(subgrid),
                SubgridRY(subgrid),
                SubgridRZ(subgrid));

        /* Write out the current well values */
        cycle_number = ReservoirDataPhysicalCycleNumber(well_data_physical);
        interval_number = TimeCycleDataComputeIntervalNumber(problem,
                                                             time, time_cycle_data, cycle_number);

        well_data_value = ReservoirDataPressReservoirIntervalValue(well_data, well,
                                                         interval_number);

        if (ReservoirDataValuePhaseValues(well_data_value))
        {
          phase = 0;
          {
            value = ReservoirDataValuePhaseValue(well_data_value, phase);
            fprintf(file, " %f", value);
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataPhysicalAction(well_data_physical) == INJECTION_WELL)
        {
          if (ReservoirDataValueDeltaSaturationPtrs(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              value = ReservoirDataValueDeltaSaturationPtr(well_data_value, phase);
              fprintf(file, " %f", value);
            }
            fprintf(file, "\n");
          }
          else if (ReservoirDataValueSaturationValues(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              value = ReservoirDataValueSaturationValue(well_data_value, phase);
              fprintf(file, " %f", value);
            }
            fprintf(file, "\n");
          }
          if (ReservoirDataValueDeltaContaminantPtrs(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                value = ReservoirDataValueContaminantFraction(well_data_value, indx)
                        * fabs(ReservoirDataValueDeltaContaminantPtr(well_data_value, indx))
                        / ReservoirDataPhysicalSize(well_data_physical);
                fprintf(file, " %f", value);
              }
            }
            fprintf(file, "\n");
          }
          else if (ReservoirDataValueContaminantValues(well_data_value))
          {
            for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
            {
              for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
              {
                indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
                value = ReservoirDataValueContaminantValue(well_data_value, indx);
                fprintf(file, " %f", value);
              }
            }
            fprintf(file, "\n");
          }
        }
        if (ReservoirDataValueContaminantFractions(well_data_value))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
            {
              indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
              value = ReservoirDataValueContaminantFraction(well_data_value, indx);
              fprintf(file, " %f", value);
            }
          }
          fprintf(file, "\n");
        }

        /* Write out the current well statistics */
        well_data_stat = ReservoirDataPressReservoirStat(well_data, well);
        if (ReservoirDataStatPhaseStats(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            stat = ReservoirDataStatPhaseStat(well_data_stat, phase);
            fprintf(file, " %f", stat);
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataStatSaturationStats(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            stat = ReservoirDataStatSaturationStat(well_data_stat, phase);
            fprintf(file, " %f", stat);
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataStatContaminantStats(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
            {
              indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
              stat = ReservoirDataStatContaminantStat(well_data_stat, indx);
              fprintf(file, " %f", stat);
            }
          }
          fprintf(file, "\n");
        }
        if (ReservoirDataStatDeltaPhases(well_data_stat) && ReservoirDataStatDeltaContaminants(well_data_stat))
        {
          for (phase = 0; phase < ReservoirDataNumPhases(well_data); phase++)
          {
            for (concentration = 0; concentration < ReservoirDataNumContaminants(well_data); concentration++)
            {
              indx = phase * ReservoirDataNumContaminants(well_data) + concentration;
              if (ReservoirDataStatDeltaPhase(well_data_stat, phase) == 0.0)
              {
                stat = 0.0;
              }
              else
              {
                stat = ReservoirDataStatDeltaContaminant(well_data_stat, indx) / ReservoirDataStatDeltaPhase(well_data_stat, phase);
              }
              fprintf(file, " %f", stat);
            }
          }
          fprintf(file, "\n");
        }
      }

      fclose(file);
    }
  }
}
