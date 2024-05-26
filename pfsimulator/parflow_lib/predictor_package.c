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

#define safe_xgboost(call) {  \
  int err = (call); \
  if (err != 0) { \
    fprintf(stderr, "%s:%d: error in %s: %s\n", __FILE__, __LINE__, #call, XGBGetLastError());  \
    exit(1); \
  } \
}
/*--------------------------------------------------------------------------
 * Structures
 *--------------------------------------------------------------------------*/

typedef struct {
    BoosterHandle booster;
} PublicXtra;

typedef void InstanceXtra;


/*--------------------------------------------------------------------------
 * SurfaceFlowPredictorPackage
 *--------------------------------------------------------------------------*/

void         SurfaceFlowPredictorPackage(
                         ProblemData *problem_data)
{
  PFModule         *this_module = ThisPFModule;
  PublicXtra       *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);

  BoosterHandle booster;
  const char *model_path = "/Users/ben/Documents/GitHub/surface-slow-predictor/first_predictor.ubj";

  // create booster handle first
  safe_xgboost(XGBoosterCreate(NULL, 0, &booster));

  // load model
  safe_xgboost(XGBoosterLoadModel(booster, model_path));
  public_xtra->booster = booster;
}


/*--------------------------------------------------------------------------
 * SurfaceFlowPredictorPackageInitInstanceXtra
 *--------------------------------------------------------------------------*/

PFModule *SurfaceFlowPredictorPackageInitInstanceXtra()
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
 * SurfaceFlowPredictorPackageFreeInstanceXtra
 *--------------------------------------------------------------------------*/

void  SurfaceFlowPredictorPackageFreeInstanceXtra()
{
  PFModule      *this_module = ThisPFModule;
  InstanceXtra  *instance_xtra = (InstanceXtra*)PFModuleInstanceXtra(this_module);

  if (instance_xtra)
  {
    tfree(instance_xtra);
  }
}


/*--------------------------------------------------------------------------
 * SurfaceFlowPredictorPackageNewPublicXtra
 *--------------------------------------------------------------------------*/

PFModule  *SurfaceFlowPredictorPackageNewPublicXtra()
{
  PFModule      *this_module = ThisPFModule;
  PublicXtra    *public_xtra;
  public_xtra = ctalloc(PublicXtra, 1);

  PFModulePublicXtra(this_module) = public_xtra;
  return this_module;
}


/*-------------------------------------------------------------------------
 * SurfaceFlowPredictorPackageFreePublicXtra
 *-------------------------------------------------------------------------*/

void  SurfaceFlowPredictorPackageFreePublicXtra()
{
  PFModule    *this_module = ThisPFModule;
  PublicXtra  *public_xtra = (PublicXtra*)PFModulePublicXtra(this_module);


}

float MakeSurfaceFlowPrediction(
        SurfaceFlowPredictor *surface_flow_predictor)
{
  float pressure = -1.1192490384242657;
  float saturation = 0.5458777476994235;
  float volume = 21780.522133;
  float max_volume = 39900.0;
  float flux_in = -19.013231;
  float we_predicted = -0.04546;
  float porosity = 0.399;
  float specific_storage = 0.0001;
  float mannings = 0.015841225;

  float sample[1][9] =
          {{pressure,
            saturation,
            volume,
            max_volume,
            flux_in,
            we_predicted,
            porosity,
            specific_storage,
            mannings
           }};

  // Convert 2D array to DMatrix
  DMatrixHandle h_matrix;
  safe_xgboost(XGDMatrixCreateFromMat((float *)sample, 1, 9, -1, &h_matrix));

  // Make prediction
  bst_ulong out_len;
  const float *f;
  safe_xgboost(XGBoosterPredict(booster, h_matrix, 1, 0, -1, &out_len, &f));
  return *f;
}

/*--------------------------------------------------------------------------
 * SurfaceFlowPredictorPackageSizeOfTempData
 *--------------------------------------------------------------------------*/

int  SurfaceFlowPredictorPackageSizeOfTempData()
{
  return 0;
}
