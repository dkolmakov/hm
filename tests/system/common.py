import os
import subprocess

import pytest


def run_cmd(cmd_list):
    process = subprocess.Popen(cmd_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=1, universal_newlines=True)

    print("Running command: {}".format(subprocess.list2cmdline(process.args)))

    stdout, stderr = process.communicate()
    rc = process.wait()
    
    return rc, stdout, stderr


def create_session(db, name = None):
    db += ".testdb"
    db = os.path.join(os.path.dirname(os.path.abspath(__file__)), db) 
    
    create_session_cmd = ['hm-db', db, 'session' ]

    if name:
        create_session_cmd.append(name)

    rc, stdout, stderr = run_cmd(create_session_cmd)
    
    assert rc == 0, "Command failed with code {}".format(rc) 

    sess_id = int(stdout)

    return db, sess_id


def insert(db_obj, pwd, cmd, ret_code):
    db, sess_id = db_obj
    
    insert_cmd = ['hm-db', db, 
                  'add', str(sess_id), "\"1972-01-01 00:00:00\"", pwd, cmd, str(ret_code) ]

    rc, stdout, stderr = run_cmd(insert_cmd)

    assert rc == 0, "Command failed with code {}".format(rc) 


def select_by_path(db_obj, pwd, recursive = False):
    db, sess_id = db_obj
    
    select_cmd = ['hm-db', db, 
                  'select', str(sess_id),
                  '-d']

    if pwd:
        select_cmd.append(pwd)
    
    if recursive:
        select_cmd.append('-R')


    rc, stdout, stderr = run_cmd(select_cmd)

    assert rc == 0, "Command failed with code {}".format(rc)
    
    return stdout.strip().split('\n')


def select_by_session(db_obj, sname = None, return_code = 0):
    db, sess_id = db_obj
    
    select_cmd = ['hm-db', db, 
                  'select', str(sess_id),
                  '-s']

    if sname:
        select_cmd.append(sname)

    rc, stdout, stderr = run_cmd(select_cmd)

    assert rc == return_code, "Expected return code {} but got {}".format(return_code, rc)
    
    return stdout.strip().split('\n')

def select_by_session_and_path(db_obj, path=None, recursive=False, sname=None):
    db, sess_id = db_obj
    
    select_cmd = ['hm-db', db, 
                  'select', str(sess_id)]

    select_cmd.append('-d')
    if path:
        select_cmd.append(path)
    if recursive:
        select_cmd.append('-R')

    select_cmd.append('-s')
    if sname:
        select_cmd.append(sname)

    rc, stdout, stderr = run_cmd(select_cmd)

    assert rc == 0, "Command failed with code {}".format(rc)
    
    return stdout.strip().split('\n')

def basic_create_db(name = None):
    db, sess_id = create_session("test", name)
 
    print("Session #{} created".format(sess_id))

    return db, sess_id


def basic_insert_select(cmd, pwd, recursive = False):
    db_obj = basic_create_db()

    insert(db_obj, pwd, cmd, 0)
    
    stdout = select_by_path(db_obj, pwd, recursive)

    for line in stdout:
        print(line)

    assert cmd == stdout[0], "Wrong command in the database!"

