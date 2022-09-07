import os    
import sys
import subprocess
import argparse

HM_HOME=os.path.expanduser("~/.hm")

def main():
    parser = argparse.ArgumentParser(description='Python wrapper for history manager configurator')
    parser.add_argument('--home', nargs='?', default=HM_HOME, help='Path to store database of history manager')
    args = parser.parse_args()

    config_filename = os.path.join(args.home, "configuration") 
        
    # Print a link to history manager configuration
    configuration_tips = "\nTo enable history manager (hm) in the current session execute:\n"
    configuration_tips += "\n\t. {}\n".format(config_filename);
    configuration_tips += "\nand modify ~/.bashrc to enable it by default in future sessions:\n"
    configuration_tips += "\n\techo \". {}\" >> ~/.bashrc\n".format(config_filename);
    print(configuration_tips)

    # Create history manager home directory (command database will be placed there)
    if not os.path.exists(args.home):
        os.makedirs(args.home)

    # Initialize configuration file with PATH environment variable modification
    cwd = os.path.abspath(os.path.dirname(__file__))
    hm_db_path = os.path.abspath(os.path.join(cwd, os.pardir))
    path_modification = "PATH=$PATH:" + hm_db_path + "\n"
    
    with open(config_filename, 'w') as f:
        f.write(path_modification)

    # Call configuration procedure od hm-db binary
    cmd = ' '.join([hm_db_path + '/hm-db', 'configure', config_filename, '--home', args.home])
    return_code = subprocess.call(cmd, shell=True)
