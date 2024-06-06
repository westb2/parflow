import os
import shutil


# If you are here because your user profile location was not detected, figure it what it
# is and either add it to the list right below or set the USER_PROFILE_FILE variable
def detect_user_profile():
    possible_locations = [
        "~/.zprofile",
        "~/.bashrc",
        "~/.profile"
    ]
    possible_locations = [os.path.expanduser(location) for location in possible_locations]
    for location in possible_locations:
        if os.path.isfile(location):
            return location
    raise Exception(
        '''Could not detect your user profile location. Try going to config.py and
        either setting it manually or adding it to the possible locations list'''
        )

USER_PROFILE_FILE = detect_user_profile()

INSTALLATION_ROOT = os.path.expanduser("~/parflow_installation")
# INSTALLATION_ROOT = f"{os.getcwd()}/installation"

# These are the required packages for Mac
REQUIRED_PACKAGES = {
    "brew": [
        # "curl",
        # "libcurl4",
        "git",
        "vim",
        "gfortran",
        # "libopenblas-dev",
        # "liblapack-dev",
        # "openssh-client",
        # "openssh-server",
        # "openmpi-bin",
        # "libopenmpi-dev",
        "open-mpi",
        "python3",
        "tcl-tk"
        # "tcl-dev",
        # "tk-dev",
    ],
    "yum": [
        "gcc",
        "gcc-c++",
        "gcc-gfortran",
        "tcl",
        "tcl-devel",
        "openmpi",
        "openmpi-devel",
        "hypre-openmpi",
        "hypre-openmpi-devel",
        "hfd5",
        "hdf5-devel",
        "hdf5-openmpi",
        "hdf5-openmpi-devel",
        "zlib",
        "zlib-devel"
    ],
    "apt-get": [
        "tzdata",
        "build-essential",
        "curl",
        "libcurl4",
        "git",
        "vim",
        "gfortran",
        "libopenblas-dev",
        "liblapack-dev",
        "openssh-client",
        "openssh-server",
        "openmpi-bin",
        "libopenmpi-dev",
        "python3",
        "python3-pip",
        "python3-venv",
        "tcl-dev",
        "tk-dev"
    ]
}


PACKAGE_MANAGER = "AUTO_CONFIGURED"


# LOCAL_PARFLOW_SRC = "/Users/ben/Documents/Github/parflow"
LOCAL_PARFLOW_SRC = f"{INSTALLATION_ROOT}/../.."
PARFLOW_SRC_DIR="parflow_src"
PARFLOW_URL = "https://github.com/parflow/parflow.git"
PARFLOW_BUILD_DIR="cmake_build"
PARFLOW_INSTALLATION_DIR="parflow"
HDF5_URL="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.12/hdf5-1.12.0/src/hdf5-1.12.0.tar.gz"
HDF5_SRC_DIR = "hdf5-src"
HDF5_DIR="hdf5"
NETCDF_URL="https://github.com/Unidata/netcdf-c/archive/v4.7.4.tar.gz"
NETCDF_SRC_DIR = "netcdf_src"
NETCDF_DIR="netcdf"
SILO_URL="https://wci.llnl.gov/sites/wci/files/2021-01/silo-4.10.2.tgz"
SILO_SRC_DIR = "silo_src"
SILO_DIR = "silo"
HYPRE_URL = "https://github.com/hypre-space/hypre.git"
HYPRE_SRC_DIR="hypre_src"
HYPRE_DIR="hypre"
CURL_LOCATION=shutil.which("curl")

CMAKE_ENVIRONMENT_VARIABLES = {
    "PARFLOW_MPIEXEC_EXTRA_FLAGS": "--mca mpi_yield_when_idle 1 --oversubscribe --allow-run-as-root"
}

CMAKE_ARGUMENTS = {
    "CMAKE_BUILD_TYPE": "Release",
    "CURL_LIBRARY": f"{CURL_LOCATION}",
    "PARFLOW_ENABLE_HDF5": f"TRUE",
    "HDF5_ROOT": f"{INSTALLATION_ROOT}/{HDF5_DIR}",
    "PARFLOW_ENABLE_HYPRE": f"TRUE",
    "HYPRE_ROOT": f"{INSTALLATION_ROOT}/{HYPRE_DIR}",
    "PARFLOW_HAVE_CLM": f"TRUE",
    "PARFLOW_ENABLE_PYTHON": f"TRUE",
    "PARFLOW_ENABLE_TIMING": f"TRUE",
    "PARFLOW_ENABLE_PROFILING": f"TRUE",
    "PARFLOW_AMPS_LAYER": f"mpi1",
    "PARFLOW_AMPS_SEQUENTIAL_IO": f"TRUE",
    "PARFLOW_ENABLE_NETCDF": f"FALSE",
    # "NETCDF_DIR": f"{INSTALLATION_ROOT}/{NETCDF_DIR} ",
    "PARFLOW_ENABLE_SILO": f"FALSE",
    # "SILO_ROOT": f"{INSTALLATION_ROOT}/{SILO_DIR}",
    "CMAKE_POLICY_DEFAULT_CMP0144": f"NEW",
     "CMAKE_C_FLAGS": f"-Wall -Werror",
}



#Add the following lines back once we fix the install scripts


                                    # -D PARFLOW_PYTHON_VIRTUAL_ENV=TRUE\