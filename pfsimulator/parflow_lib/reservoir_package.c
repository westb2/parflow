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
/*--------------------------------------------------------------------------
 * Structures
 *--------------------------------------------------------------------------*/

typedef struct {
    int num_phases;
    int num_contaminants;

    /* well info */
    int num_units;
    int num_reservoirs;
    void     **data;

    /* Timing Cycle information */

    NameArray reservoir_names;
} PublicXtra;

typedef void InstanceXtra;

typedef struct {
    char    *name;
    int action;
    int mechanism;
    double xlocation;
    double ylocation;
    double z_lower, z_upper;
    int method;
    int cycle_number;
    double **phase_values;
    double **saturation_values;
    double **contaminant_values;
} Type0;                      /* basic vertical well */



/*--------------------------------------------------------------------------
 * ReservoirPackage
 *--------------------------------------------------------------------------*/

void         ReservoirPackage(
    ProblemData *problem_data)
{
  PFModule         *this_module = ThisPFModule;
  PublicXtra       *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);

  Type0            *dummy0;

  Subgrid          *new_subgrid;

  WellData         *well_data = ProblemDataWellData(problem_data);
  WellDataPhysical *well_data_physical;
  WellDataValue    *well_data_value;
  WellDataStat     *well_data_stat = NULL;

  int i, sequence_number, reservoir;

  int ix, iy;
  int iz_lower, iz_upper;
  int nx, ny, nz;
  double dx, dy, dz;
  int rx, ry, rz;
  int process;
  int reservoir_action, action, mechanism, method;
  double          **phase_values;
  double subgrid_volume;
  double x_lower, x_upper, y_lower, y_upper,
      z_lower, z_upper;

  /* Allocate the well data */
  WellDataNumPhases(well_data) = (public_xtra->num_phases);
  WellDataNumContaminants(well_data) = (public_xtra->num_contaminants);

  WellDataNumWells(well_data) += (public_xtra->num_reservoirs);

  if ((public_xtra->num_reservoirs) > 0)
  {
    WellDataNumReservoirWells(well_data) = (public_xtra->num_reservoirs);
    if ((public_xtra->num_reservoirs) > 0)
      WellDataReservoirWellPhysicals(well_data) = ctalloc(WellDataPhysical *, (public_xtra->num_reservoirs));
    WellDataReservoirWellValues(well_data) = ctalloc(WellDataValue * *, (public_xtra->num_reservoirs));
    WellDataReservoirWellStats(well_data) = ctalloc(WellDataStat *, (public_xtra->num_reservoirs));
  }

  reservoir = 0;
  sequence_number = 0;

  if ((public_xtra->num_units) > 0)
  {
    /* Load the reservoir data */
    for (i = 0; i < (public_xtra->num_units); i++)
    {
      dummy0 = (Type0*)(public_xtra->data[i]);

      ix = IndexSpaceX((dummy0->xlocation), 0);
      iy = IndexSpaceY((dummy0->ylocation), 0);
      iz_lower = IndexSpaceZ((dummy0->z_lower), 0);
      iz_upper = IndexSpaceZ((dummy0->z_upper), 0);

      nx = 1;
      ny = 1;
      nz = iz_upper - iz_lower + 1;

      rx = 0;
      ry = 0;
      rz = 0;

      process = amps_Rank(amps_CommWorld);

      new_subgrid = NewSubgrid(ix, iy, iz_lower,
                               nx, ny, nz,
                               rx, ry, rz,
                               process);

      dx = SubgridDX(new_subgrid);
      dy = SubgridDY(new_subgrid);
      dz = SubgridDZ(new_subgrid);

      subgrid_volume = (nx * dx) * (ny * dy) * (nz * dz);

      /* Put in physical data for this well */
      well_data_physical = ctalloc(WellDataPhysical, 1);
      WellDataPhysicalNumber(well_data_physical) = sequence_number;
      WellDataPhysicalName(well_data_physical) = ctalloc(char, strlen((dummy0->name)) + 1);
      strcpy(WellDataPhysicalName(well_data_physical), (dummy0->name));
      WellDataPhysicalXLower(well_data_physical) = (dummy0->xlocation);
      WellDataPhysicalYLower(well_data_physical) = (dummy0->ylocation);
      WellDataPhysicalZLower(well_data_physical) = (dummy0->z_lower);
      WellDataPhysicalXUpper(well_data_physical) = (dummy0->xlocation);
      WellDataPhysicalYUpper(well_data_physical) = (dummy0->ylocation);
      WellDataPhysicalZUpper(well_data_physical) = (dummy0->z_upper);
      WellDataPhysicalDiameter(well_data_physical) = pfmin(dx, dy);
      WellDataPhysicalSubgrid(well_data_physical) = new_subgrid;
      WellDataPhysicalSize(well_data_physical) = subgrid_volume;
      WellDataPhysicalAction(well_data_physical) = (dummy0->action);
      WellDataPhysicalMethod(well_data_physical) = (dummy0->method);
      WellDataPhysicalAveragePermeabilityX(well_data_physical) = 0.0;
      WellDataPhysicalAveragePermeabilityY(well_data_physical) = 0.0;
      WellDataPhysicalAveragePermeabilityZ(well_data_physical) = 0.0;
      WellDataPressWellPhysical(well_data, reservoir) = well_data_physical;

      /* Put in informational statistics for this well */
      well_data_stat = ctalloc(WellDataStat, 1);
      WellDataPressWellStat(well_data, reservoir) = well_data_stat;
      reservoir++;
      sequence_number++;
      break;
    }

  }
}



/*--------------------------------------------------------------------------
 * ReservoirPackageInitInstanceXtra
 *--------------------------------------------------------------------------*/

PFModule *ReservoirPackageInitInstanceXtra()
{
  PFModule      *this_module = ThisPFModule;
  InstanceXtra  *instance_xtra;

#if 0
  if (PFModuleInstanceXtra(this_module) == NULL)
    instance_xtra = ctalloc(InstanceXtra, 1);
  else
    instance_xtra = (InstanceXtra*)PFModuleInstanceXtra(this_module);
#endif
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

PFModule  *ReservoirPackageNewPublicXtra(
    int num_phases,
    int num_contaminants)
{
  PFModule      *this_module = ThisPFModule;
  PublicXtra    *public_xtra;

  Type0         *dummy0;

  int num_units;
  int i;


  char *reservoir_names;
  char *reservoir_name;
  char key[IDB_MAX_KEY_LEN];

  NameArray inputtype_na;
  NameArray action_na;
  NameArray mechanism_na;
  NameArray methodpress_na;
  NameArray methodflux_na;

  inputtype_na = NA_NewNameArray("Vertical Recirc");
  action_na = NA_NewNameArray("Injection Extraction");
  mechanism_na = NA_NewNameArray("Pressure Flux");
  methodpress_na = NA_NewNameArray("Standard");
  methodflux_na = NA_NewNameArray("Standard Weighted Patterned");

  public_xtra = ctalloc(PublicXtra, 1);


  (public_xtra->num_phases) = num_phases;
  (public_xtra->num_contaminants) = num_contaminants;

  reservoir_names = GetString("Reservoirs.Names");

  public_xtra->reservoir_names = NA_NewNameArray(reservoir_names);

  num_units = NA_Sizeof(public_xtra->reservoir_names);


  public_xtra->num_units = num_units;

  public_xtra->num_reservoirs = 0;

  if (num_units > 0)
  {
    (public_xtra->data) = ctalloc(void *, num_units);

    for (i = 0; i < num_units; i++)
    {
      reservoir_name = NA_IndexToName(public_xtra->reservoir_names, i);

      dummy0 = ctalloc(Type0, 1);

      /*** Read in the physical data for the well ***/
      dummy0->name = strdup(reservoir_name);

      sprintf(key, "Reservoirs.X", reservoir_name);
      dummy0->xlocation = GetDouble(key);

      sprintf(key, "Reservoirs.Y", reservoir_name);
      dummy0->ylocation = GetDouble(key);

      sprintf(key, "Reservoirs.ZUpper", reservoir_name);
      dummy0->z_upper = GetDouble(key);

      sprintf(key, "Reservoirs.ZLower", reservoir_name);
      dummy0->z_lower = GetDouble(key);

      /*** Bump the counter for the number of reservoirs ***/

      (public_xtra->num_reservoirs)++;

      (public_xtra->data[i]) = (void*)dummy0;

      break;
    }
  }
  NA_FreeNameArray(methodflux_na);
  NA_FreeNameArray(methodpress_na);
  NA_FreeNameArray(mechanism_na);
  NA_FreeNameArray(action_na);
  NA_FreeNameArray(inputtype_na);

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

  int num_units, num_cycles;
  int i, interval_number, interval_division;

  if (public_xtra)
  {
    NA_FreeNameArray(public_xtra->reservoir_names);

    /* Free the well information */
    num_units = (public_xtra->num_units);
    if (num_units > 0)
    {
      for (i = 0; i < num_units; i++)
      {
        dummy0 = (Type0*)(public_xtra->data[i]);


        for (interval_number = 0; interval_number < interval_division; interval_number++)
        {
          if ((dummy0->contaminant_values))
          {
            if ((dummy0->contaminant_values[interval_number]))
            {
              tfree((dummy0->contaminant_values[interval_number]));
            }
          }
          if ((dummy0->saturation_values))
          {
            if ((dummy0->saturation_values[interval_number]))
            {
              tfree((dummy0->saturation_values[interval_number]));
            }
          }
          if ((dummy0->phase_values))
          {
            if ((dummy0->phase_values[interval_number]))
            {
              tfree((dummy0->phase_values[interval_number]));
            }
          }
        }
        if ((dummy0->contaminant_values))
        {
          tfree((dummy0->contaminant_values));
        }
        if ((dummy0->saturation_values))
        {
          tfree((dummy0->saturation_values));
        }
        if ((dummy0->phase_values))
        {
          tfree((dummy0->phase_values));
        }
        if ((dummy0->name))
        {
          tfree((dummy0->name));
        }

        tfree(dummy0);

        break;
      }
      tfree(public_xtra->data);
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
