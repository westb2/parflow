#ifndef PARFLOW_CONFIG_H
#define PARFLOW_CONFIG_H

/*
 * Parflow configure file.
 */

/* Use PARFLOW_ versions; other names are deprecated */
/* TODO should get rid of non prefixed versions of these flags; CPP
   symbols should be in PARFLOW namespace.  Should make naming consistent as well */

#define PARFLOW_AMPS_LAYER
#define AMPS

/* #undef AMPS_SPLIT_FILE */

#define PARFLOW_HAVE_TCL
#define HAVE_TCL

/* #undef PARFLOW_HAVE_MPI */
/* #undef HAVE_MPI */

#define PARFLOW_HAVE_GETTIMEOFDAY
#define CASC_HAVE_GETTIMEOFDAY

/* #undef PARFLOW_HAVE_BIG_ENDIAN */
/* #undef CASC_HAVE_BIGENDIAN */

#define PARFLOW_HAVE_CLM
#define HAVE_CLM

/* #undef PARFLOW_HAVE_HYPRE */
/* #undef HAVE_HYPRE */

#define PARFLOW_HYPRE_VERSION_MAJOR 
#define PARFLOW_HYPRE_VERSION_MINOR 
#define PARFLOW_HYPRE_VERSION_PATCH 

/* #undef PARFLOW_HAVE_SUNDIALS */
/* #undef HAVE_SUNDIALS */

/* #undef PARFLOW_ENABLE_TIMING */
/* #undef PF_TIMING */

/* #undef PARFLOW_HAVE_SILO */
/* #undef HAVE_SILO */

/* #undef PARFLOW_HAVE_SLURM */
/* #undef HAVE_SLURM */

/* #undef PARFLOW_HAVE_NETCDF */

/* #undef PARFLOW_HAVE_HDF5 */
/* #undef HAVE_HDF5 */

/* #undef PARFLOW_HAVE_OAS3 */
/* #undef HAVE_OAS3 */

/* #undef PARFLOW_HAVE_MALLOC_H */
/* #undef HAVE_MALLOC_H */

/* #undef PARFLOW_HAVE_MALLINFO2 */
/* #undef PARFLOW_HAVE_MALLINFO */

/* #undef PARFLOW_HAVE_ETRACE */

/* #undef PARFLOW_HAVE_CUDA */

/* #undef PARFLOW_HAVE_KOKKOS */

/* PARFLOW_HAVE_RMM is not defined here because because it is only set 
   for the compilation of parflow_exe, parflow_lib, and kinsol sources. 
   RMM must be initialized before it is used, so, for example, 
   the amps tests would fail if used across all compiles.              */

/* #undef PARFLOW_HAVE_OMP */

#endif // PARFLOW_CONFIG_H
