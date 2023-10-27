import os
import config
from utils import *
from SystemPackageManager import SystemPackageManager


class ParflowInstaller:

    def __init__(self):
        self.system_package_manager = SystemPackageManager(config.PACKAGE_MANAGER)
        self.package_locations = {}
        self.PARFLOW_ENVIRONMENT_FILE = "set_parflow_env.sh"
        self.REBUILD_PARFLOW_SCRIPT_FILE = "rebuild_parflow.sh"
        self.parflow_source_dir=f"{config.INSTALLATION_ROOT}/../.."

    def install_parflow(self):
        create_directory(config.INSTALLATION_ROOT)
        os.chdir(config.INSTALLATION_ROOT)
        self.set_environment_variables()
        self.write_env_file()
        self.write_rebuild_parflow_script()
        self.add_env_file_to_user_profile()
        self.set_environment()
        self.cmake()
        self.install_pftools()
        print("Installation complete :)\n")

    
    def set_environment(self):
        os.system(f"{config.INSTALLATION_ROOT}/{self.PARFLOW_ENVIRONMENT_FILE}")


    def add_env_file_to_user_profile(self):
        with open(config.USER_PROFILE_FILE, "r+") as user_profile:
            text_to_be_added = f". {config.INSTALLATION_ROOT}/{self.PARFLOW_ENVIRONMENT_FILE}"
            for line in user_profile:
                if text_to_be_added in line:
                    break
            else: # not found, we are at the eof
                user_profile.write(text_to_be_added+"\n")


    def write_env_file(self):
        with open(self.PARFLOW_ENVIRONMENT_FILE, "w") as file:
            file.write(f'export PARFLOW_MPIEXEC_EXTRA_FLAGS="--mca mpi_yield_when_idle 1 --oversubscribe --allow-run-as-root"\n')
            file.write(f'export PATH="{config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}/bin:$PATH"\n')
            file.write(f'export PARFLOW_DIR="{config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}"\n')
            file.write(f'export PF_SRC="{self.parflow_source_dir}"\n')
            file.write(f'alias rebuild_parflow="{config.INSTALLATION_ROOT}/rebuild_parflow.sh"')
        ALL_PERMISSIONS = 0o777
        os.chmod(self.PARFLOW_ENVIRONMENT_FILE, ALL_PERMISSIONS)

    def write_rebuild_parflow_script(self):
        with open(self.REBUILD_PARFLOW_SCRIPT_FILE, "w") as file:
            file.write(f'cd {config.INSTALLATION_ROOT}/..\n')
            file.write('python3 install_parflow.py\n')
        ALL_PERMISSIONS = 0o777
        os.chmod(self.REBUILD_PARFLOW_SCRIPT_FILE, ALL_PERMISSIONS)


    def download_parflow_source(self):
        os.system(f"git clone -b master --single-branch {config.PARFLOW_URL} {config.PARFLOW_SRC_DIR}")
    

    def install_pftools(self):
        print(f"python3 -m pip install {config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}/python")
        os.system(
            f"python3 -m pip install {config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}/python"
        )

    def cmake(self):
        # os.system("python3 -m pip install yaml") # this is a required package
        create_directory(config.PARFLOW_SRC_DIR)
        os.system(
            f"cmake -S {self.parflow_source_dir}\
            -B {config.INSTALLATION_ROOT}/{config.PARFLOW_BUILD_DIR}\
            {config.CMAKE_ARGUMENTS}"
        )


    def set_environment_variables(self):
        for (name, value) in config.CMAKE_ENVIRONMENT_VARIABLES.items():
            os.environ[name] = value


    def install_requirements(self):
        create_directory(config.INSTALLATION_ROOT)
        os.chdir(config.INSTALLATION_ROOT)
        # first install everything we can via our system package manager
        for package in config.REQUIRED_PACKAGES:
            self.system_package_manager.install_package(package)
            self.package_locations[package] = self.system_package_manager.get_package_location(package)
        # next install the packages we need to build and configure from source.
        # The order of these matters!
        self.install_hdf5()
        self.install_netcdf()
        # self.install_silo()
        # self.install_hypre()
        os.chdir(config.INSTALLATION_ROOT)
        self.save_package_locations()

    def install_hypre(self):
        os.chdir(config.INSTALLATION_ROOT)
        create_directory(config.HYPRE_SRC_DIR)
        os.chdir(config.HYPRE_SRC_DIR)
        os.system(f"git clone {config.HYPRE_URL} --single-branch")
        os.chdir("hypre/src")
        os.system(f"./configure --prefix={config.INSTALLATION_ROOT}/{config.HYPRE_DIR} CC=mpicc && make && make install")
        self.package_locations["hypre"] = config.INSTALLATION_ROOT + "/" + config.HYPRE_DIR


    def install_hdf5(self):
        os.chdir(config.INSTALLATION_ROOT)
        create_directory(config.HDF5_SRC_DIR)
        os.chdir(config.HDF5_SRC_DIR)
        os.system(f"curl -L {config.HDF5_URL} | tar --strip-components=1 -xzv && \
                    CC=mpicc ./configure \
                    --prefix={config.INSTALLATION_ROOT}/{config.HDF5_DIR} \
                    --enable-parallel && \
                    make && make install"
                  )
        self.package_locations["hdf5"] = config.INSTALLATION_ROOT + config.HDF5_DIR


    def install_netcdf(self):
        os.chdir(config.INSTALLATION_ROOT)
        create_directory(config.NETCDF_SRC_DIR)
        os.chdir(config.NETCDF_SRC_DIR)
        os.system(f"curl -L {config.NETCDF_URL} | tar --strip-components=1 -xzv && \
                    CC=mpicc CPPFLAGS=-I{config.HDF5_DIR}/include LDFLAGS=-L{config.HDF5_DIR}/lib \
                    ./configure --disable-shared --disable-dap --prefix=${config.NETCDF_DIR} && \
                    make && make install"
                  )
        self.package_locations["netcdf"] = config.INSTALLATION_ROOT + config.NETCDF_DIR

    
    def install_silo(self):
        os.chdir(config.INSTALLATION_ROOT)
        create_directory(config.SILO_DIR)
        os.chdir(config.SILO_DIR)
        os.system(f"curl -L {config.SILO_URL} | tar --strip-components=1 -xzv && \
                    ./configure  --prefix={config.SILO_DIR} --disable-silex --disable-hzip --disable-fpzip && \
                    make && make install"
                  )
        self.package_locations["silo"] = config.INSTALLATION_ROOT + config.SILO_DIR




