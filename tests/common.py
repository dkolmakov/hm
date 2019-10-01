import os
import subprocess

import pytest


def run_cmd(cmd_list):
    process = subprocess.Popen(cmd_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=1, universal_newlines=True)

    print("Running command: {}".format(subprocess.list2cmdline(process.args)))

    stdout, stderr = process.communicate()
    rc = process.wait()
    
    return rc, stdout, stderr


def create_session(db, name):
    db += ".testdb"
    db = os.path.join(os.path.dirname(os.path.abspath(__file__)), db) 
    
    create_session_cmd = ['hm-db', 
                  '--db', db,
                  '-s', name ]

    rc, stdout, stderr = run_cmd(create_session_cmd)
    
    assert rc == 0, "Command failed with code {}".format(rc) 

    sess_id = int(stdout)

    return sess_id


def insert(db, sess_id, pwd, cmd, ret_code):
    db += ".testdb"
    insert_cmd = ['hm-db', 
                  '--db', db, 
                  '-a', '{}'.format(sess_id), pwd, cmd, str(ret_code) ]

    rc, stdout, stderr = run_cmd(insert_cmd)

    assert rc == 0, "Command failed with code {}".format(rc) 


def select(db, pwd, recursive = False):
    db += ".testdb"
    select_cmd = ['hm-db', 
                  '--db', db, 
                  '-d', pwd]
    
    if recursive:
        select_cmd.append('-R')

    rc, stdout, stderr = run_cmd(select_cmd)

    assert rc == 0, "Command failed with code {}".format(rc) 
    
    return stdout.strip().split('\n')

