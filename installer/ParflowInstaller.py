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

    def install_parflow(self, use_local_source_code=False):
        create_directory(config.INSTALLATION_ROOT)
        os.chdir(config.INSTALLATION_ROOT)
        self.set_environment_variables()
        self.write_env_file()
        self.write_rebuild_parflow_script()
        self.add_env_file_to_user_profile()
        if use_local_source_code:
            self.cmake(parflow_source=config.LOCAL_PARFLOW_SRC)
        else:
            self.download_parflow_source()
            self.cmake(parflow_source=f"{config.INSTALLATION_ROOT}/{config.PARFLOW_SRC_DIR}")
        print("INSTALLATION COMPLETE!\n")
        print("Please add the following lines to your bashrc (or other profile) file\n\n")
        print(f'export PATH="{config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}/bin:$PATH"\n')
        print(f'export PARFLOW_DIR="{config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}"\n\n')
        print("You can also find this information where you installed parflow if you need to find it later")


    def add_env_file_to_user_profile(self):
        with open(config.USER_PROFILE_FILE, "r+") as user_profile:
            text_to_be_added = f"{config.INSTALLATION_ROOT}/{self.PARFLOW_ENVIRONMENT_FILE}"
            for line in user_profile:
                if text_to_be_added in line:
                    break
            else: # not found, we are at the eof
                user_profile.write(text_to_be_added+"\n")


    def write_env_file(self):
        with open(self.PARFLOW_ENVIRONMENT_FILE, "w") as file:
            file.write(f'export PARFLOW_MPIEXEC_EXTRA_FLAGS="--mca mpi_yield_when_idle 1 --oversubscribe --allow-run-as-root"')
            file.write(f'export PATH="{config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}/bin:$PATH"\n')
            file.write(f'export PARFLOW_DIR="{config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}"')
            file.write(f'alias rebuild_parflow="{config.INSTALLATION_ROOT}/{config.PARFLOW_INSTALLATION_DIR}/rebuild_parflow.sh"')


    def write_rebuild_parflow_script(self):
        with open(self.REBUILD_PARFLOW_SCRIPT_FILE, "w") as file:
            file.write("cwd=$(pwd)\n")
            file.write(f'cd {config.INSTALLATION_ROOT}\n')
            file.write('python3 install_parflow.py\n')
            file.write("cd $(cwd)")


    def download_parflow_source(self):
        os.system(f"git clone -b master --single-branch {config.PARFLOW_URL} {config.PARFLOW_SRC_DIR}")
    

    def install_pftools(self):
        create_directory(config.INSTALLATION_ROOT)
        os.chdir(config.INSTALLATION_ROOT)
        run_and_capture_terminal_output(
            f"python3 -m pip install \
            {config.INSTALLATION_ROOT}/{config.PARFLOW_BUILD_DIR}/pftools/python"
        )

    def cmake(self, parflow_source):
        create_directory(config.PARFLOW_SRC_DIR)
        os.system(
            f"cmake -S {parflow_source}\
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
        self.install_silo()
        self.install_hypre()
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




