"""
Remove .pyc files from root and all sub-dirs

Authors:
 + Francesco Racciatti  <racciatti.francesco@gmail.com>
"""


import os
from subprocess import call


def remove_pyc_files(dir_name):
    """
    Remove .pyc files from root and all sub-dirs
    """    
    
    # Remove .pyc file from root
    abspath = os.path.abspath(dir_name)
    shell_cmd = "rm " + abspath + "/*.pyc"
    call(shell_cmd, shell=True)
    
    # Remove .pyc files from sub-dirs
    for path, subdirs, files in os.walk(dir_name):
        for subdir_name in subdirs:
            abspath = os.path.abspath(os.path.join(dir_name, subdir_name))
            shell_cmd = "rm " + abspath + "/*.pyc"
            call(shell_cmd, shell=True)

