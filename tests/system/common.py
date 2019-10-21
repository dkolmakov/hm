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


def insert(db_obj, pwd, cmd, ret_code):
    db, sess_id = db_obj
    
    db += ".testdb"
    insert_cmd = ['hm-db', 
                  '--db', db, 
                  '-a', '{}'.format(sess_id), "\"1972-01-01 00:00:00\"", pwd, cmd, str(ret_code) ]

    rc, stdout, stderr = run_cmd(insert_cmd)

    assert rc == 0, "Command failed with code {}".format(rc) 


def select(db_obj, pwd, recursive = False):
    db, sess_id = db_obj
    
    db += ".testdb"
    select_cmd = ['hm-db', 
                  '--db', db, 
                  '-d', pwd]
    
    if recursive:
        select_cmd.append('-R')

    rc, stdout, stderr = run_cmd(select_cmd)

    assert rc == 0, "Command failed with code {}".format(rc) 
    
    return stdout.strip().split('\n')


def basic_create_db():
    db = "test"
    name = "test session"
    
    sess_id = create_session(db, name)
 
    print("Session #{} created".format(sess_id))

    return db, sess_id


def basic_insert_select(pwd, cmd):
    db_obj = basic_create_db()

    insert(db_obj, pwd, cmd, 0)
    
    stdout = select(db_obj, pwd)

    for line in stdout:
        print(line)

    assert cmd == stdout[0], "Wrong command in the database!"

