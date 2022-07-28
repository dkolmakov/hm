import os    
import sys
import subprocess
import argparse


def main():
    cwd = os.path.abspath(os.path.dirname(__file__))
    hm_db_path = os.path.abspath(os.path.join(cwd, os.pardir))
    path_modification = "PATH=$PATH:" + hm_db_path + "\n"
    
    parser = argparse.ArgumentParser(description='Python wrapper for history manager configurator')
    parser.add_argument('dest', nargs='?', help='A destination file to write history manager\'s configuration')
    parser.add_argument('--home', nargs='?', help='Path to store database of history manager')
    args = parser.parse_args()
    
    if args.dest != None:
        with open(args.dest, 'a') as f:
            f.write(path_modification)
    else:
        print(path_modification)
    
    cmd = ' '.join([hm_db_path + '/hm-db', 'configure'] + sys.argv[1:])
    #print(cmd)
    return_code = subprocess.call(cmd, shell=True)

    #print("hm should be initialized here")
