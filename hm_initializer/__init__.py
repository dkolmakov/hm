import os    
import sys
import subprocess

def main():
    cwd = os.path.abspath(os.path.dirname(__file__))
    hm_db_path = os.path.abspath(os.path.join(cwd, os.pardir))
    path_modification = "PATH=$PATH:" + hm_db_path + "\n"
    
    if len(sys.argv) > 1:
        with open(sys.argv[1], 'a') as f:
            f.write(path_modification)
    else:
        print(path_modification)
    
    cmd = ' '.join([hm_db_path + '/hm-db', 'configure'] + sys.argv[1:])
    #print(cmd)
    return_code = subprocess.call(cmd, shell=True)

    #print("hm should be initialized here")
