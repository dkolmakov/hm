import pytest

from common import *

def test_session_numbering(remove_db):
    db = "test"
    name = "test session"
    
    sess_id_0 = create_session(db, name)
    assert sess_id_0 == 1

    sess_id_1 = create_session(db, name)
    assert sess_id_1 == 2


def test_basic_insert_select(remove_db):
    db = "test"
    name = "test session"
    
    sess_id = create_session(db, name)
    assert sess_id == 1
 
    print("Session #{} created".format(sess_id))

    pwd = "some/arbitrary/path"
    cmd = "some --arbitrary command"

    insert(db, sess_id, pwd, cmd)
    
    stdout = select(db, pwd)

    print(stdout)

    assert cmd == stdout, "Wrong command in the database!"

 
