import os
import subprocess
from utils import *


class SystemPackageManager:
    def __init__(self, package_manager):
        if package_manager == "AUTO_CONFIGURED":
            # make this instantiate a HomebrewManager child class
            self.package_manager = self.determine_package_manager
            print(f"We determined the package manager to be {self.package_manager}\n")
            if self.package_manager == "brew":
                self.automatic_yes_flag = ""
            else:
                self.automatic_yes_flag = "-y"

    def determine_package_manager(self):
        potential_package_managers = ["brew", "yum", "apt-get"]
        for package_manager in potential_package_managers:
            package_manager_location = subprocess.check_output(["which",  f"{package_manager}"])
            if package_manager_location != "":
                    return package_manager
        raise Exception(
        '''Could not detect your systems package manager. Try going to config.py and
        either setting it manually, but if it is not one of brew, yum, or apt-get this tool
        may not work for you'''
    )


    def get_homebrew_package_location(self, package):
        return run_and_capture_terminal_output(f"brew --prefix {package}")


    def get_package_location(self, package, package_manager="homebrew"):
        if package_manager=="homebrew":
            print("LOCATING PACKAGE" + self.get_homebrew_package_location(package)+"\n")
            return self.get_homebrew_package_location(package)


    def install_package(self, package):
        if self.package_manager == "brew":
            self.brew_install_package(package)


    def brew_install_package(self, package):
        os.system(
            f'''
            {self.package_manager} update && \
            {self.package_manager} install {self.automatic_yes_flag} \
            {package}
            '''
        )