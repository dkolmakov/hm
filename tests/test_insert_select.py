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

    insert(db, sess_id, pwd, cmd)
    
    stdout = select(db, "\"" + pwd + "\"")

    for line in stdout:
        print(line)

    assert cmd == stdout.strip(), "Wrong command in the database!"


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
