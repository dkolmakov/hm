import pytest

from common import *

def test_session_numbering(remove_db):
    db = "test"
    name = "test session"
    
    sess_id_0 = create_session(db, name)
    assert sess_id_0 == 1

    sess_id_1 = create_session(db, name)
    assert sess_id_1 == 2


def basic_insert_select(pwd, cmd):
    db = "test"
    name = "test session"
    
    sess_id = create_session(db, name)
 
    print("Session #{} created".format(sess_id))

    insert(db, sess_id, pwd, cmd, 0)
    
    stdout = select(db, pwd)

    for line in stdout:
        print(line)

    assert cmd == stdout[0], "Wrong command in the database!"


def test_basic_insert_select(remove_db):
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"

    basic_insert_select(pwd, cmd)


def test_select_with_punctuation(remove_db):
    pwd = "/some/arbitrary/path_with_underscores"
    cmd = "some command number one"
    
    basic_insert_select(pwd, cmd)

    pwd = "/some/arbitrary/.path"
    cmd = "some command number two"
    
    basic_insert_select(pwd, cmd)

    pwd = "/some/arbitrary/path-with-hyphens"
    cmd = "some command number three"
    
    basic_insert_select(pwd, cmd)


def test_recursive_select(remove_db):
    pwd1 = "/some/arbitrary/path"
    cmd1 = "some --arbitrary command"

    pwd2 = "/some/arbitrary/path/inside"
    cmd2 = "some --other command"

    db = "test"
    name = "test session"
    
    sess_id = create_session(db, name)
 
    print("Session #{} created".format(sess_id))

    insert(db, sess_id, pwd1, cmd1, 0)
    insert(db, sess_id, pwd2, cmd2, 0)
    
    stdout = select(db, pwd1)

    for line in stdout:
        print(line)
        
    assert len(stdout) == 1, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"

    stdout = select(db, pwd1, True)

    for line in stdout:
        print(line)

    assert len(stdout) == 2, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"
    assert cmd2 == stdout[1], "Wrong command in the database!"
