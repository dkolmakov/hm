import pytest

from common import *

def test_basic_insert_select(remove_db):
    sess_name = "Some name"
    cmd = "some --arbitrary command"

    insert_select_by_session_name(cmd, sess_name)

def test_insert_select_underscores(remove_db):
    sess_name = "Some_name_with_underscores"
    cmd = "some --arbitrary command"

    insert_select_by_session_name(cmd, sess_name)

def test_insert_select_dots(remove_db):
    sess_name = "Some.name.with.dots"
    cmd = "some --arbitrary command"

    insert_select_by_session_name(cmd, sess_name)
   
def test_insert_select_hypens(remove_db):
    sess_name = "Some-name-with-dots"
    cmd = "some --arbitrary command"

    insert_select_by_session_name(cmd, sess_name)

def test_insert_select_for_several_sessions(remove_db):
    sess_name = "test-name"
    first_sess = basic_create_db(sess_name)
    second_sess = basic_create_db(sess_name)
    
    pwd1 = "/some/arbitrary/path"
    cmd1 = "some --arbitrary command"

    pwd2 = "/some/arbitrary/path/inside"
    cmd2 = "some --other command"

    insert(first_sess, pwd1, cmd1, 0)
    insert(second_sess, pwd2, cmd2, 0)
    
    stdout = select_by_session(first_sess, sess_name)

    assert len(stdout) == 2, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"
    assert cmd2 == stdout[1], "Wrong command in the database!"

