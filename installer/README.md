This is an installer for parflow. Currently only working for Mac's, eventually it should work for any flavor of unix.

Instructions:
If you are on a Mac homebrew is required. You can get it here https://brew.sh/. You will also need a working copy of python3.

First thing we need to do is navigate into the installer directory. 

`cd /PATH/TO/PARFLOW/REPO/installer`

Next we need to install parflow's dependencies

`python3 install_requirements.py`

After that run

`python3 install_parflow.py`

And that's it! The version you have built corresponds to the code in this repo, so if you want to build a different branch or update your version all you have to do is run rebuild_parflow (a command we automatically added for you)

Before you run for the first time either source your user profile or close and open terminal

ONCE YOU HAVE PARFLOW INSTALLED
Firstly, you should no be able to run parflow via command line e.g.

`parflow YOUR_FAVORITE_MODEL.pfidb`

You can also run models via python e.g.

`python3 YOUR_FAVORITE_MODEL.py`

provided you haven't done anything weird with your python environments (see IMPORTANT NOTE ON PFTOOLS AND CONDA/VIRTUALENV for more details). I would recommend you run the line

Lastly, we have added the script

`rebuild_parflow`

This will rerun the whole cmake build but maintain the cache (so it is fast). This is useful if you are either doing active development on parflow or switching between branches

You can also reinstall the requirements by rerunning the python script, or change how your installation is configured in config.py (you will need some understanding of how parflow installations work to make changes that work).

Note that if you move your parflow repo all the paths pointing to stuff will break. In the case you do this, we recommend deleting your installation and doing it again (isn't it nice that will be so easy :) )

IMPORTANT NOTE ON PFTOOLS AND CONDA/VIRTUALENV
This installation path will build pftools from the local version of the code that lives in this repo. This install is at the user level and will persist across different environments unless you specifically reinstall in those environments. However, if you switch conda/virtual env environments and it isn't there for some reason you may need to run rebuild_parflow, or if you dont want to rerun the cmake build you can also use the command install_local_pftools. You can also make conda environments that specifically install a version of pftools that you grabbed from online to overwrite this.

If you want to turn off this installation path installing pftools go to config.py and set -D PARFLOW_ENABLE_PYTHON=FALSE\ in the CMAKE_ARGUMENTS variable.

USING THIS IN SHARED COMPUTE ENVIRONMENTS
This installer will only build a version of parflow that you can use. This is nice, because it can be run in environments that you do not have admin privelages in (think HPC). However, you may want other people to be running from the same version as you. In this case, simply have them download the same version of parflow that you have (you can even give them you commit hash) and pass them your config.py file if you have changed it at all. This is all you should need to do to guarentee their install is the same as yours.

HOW TO CHANGE THE INSTALLATION CONFIGURATION
Most of the settings are in the config.py file. You can change these as you desire, for example to install in a different location. You can also use this file to build with dependencies not included in this installer. For example, if you want to enable the GPU, install those requirements however you are supposed to. Then, add flags turning them on and pointing to where you installed them to the CMAKE_BUILD_ARGUMENTS variable. Make sure to use absolute paths!