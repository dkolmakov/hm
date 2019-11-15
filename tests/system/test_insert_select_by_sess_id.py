import pytest

from common import *

def insert_select_by_session_name(cmd, sname):
    pwd = "/some/arbitrary/path"
    
    db_obj = basic_create_db(sname)

    insert(db_obj, pwd, cmd, 0)
    
    stdout = select_by_session(db_obj, sname)

    for line in stdout:
        print(line)

    assert cmd == stdout[0], "Wrong command in the database!"


def prepare_several_sessions(sess_name):
    first_sess = basic_create_db(sess_name)
    second_sess = basic_create_db(sess_name)
    third_sess = basic_create_db("Other session name")
    
    pwd1 = "/some/arbitrary/path"
    cmd1 = "some --arbitrary command"

    pwd2 = "/some/arbitrary/path/inside"
    cmd2 = "some --other command"

    pwd3 = "/some/arbitrary/path"
    cmd3 = "not valid --other-session command"

    insert(first_sess, pwd1, cmd1, 0)
    insert(second_sess, pwd2, cmd2, 0)
    insert(third_sess, pwd3, cmd3, 0)
    
    return first_sess, cmd1, second_sess, cmd2, pwd1

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
    first_sess, cmd1, _, cmd2, _ = prepare_several_sessions(sess_name)
    
    stdout = select_by_session(first_sess, sess_name)

    assert len(stdout) == 2, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"
    assert cmd2 == stdout[1], "Wrong command in the database!"

def test_by_session_default_arg(remove_db):
    sess_name = "test-name"
    first_sess, cmd1, _, cmd2, _ = prepare_several_sessions(sess_name)

    stdout = select_by_session(first_sess)

    assert len(stdout) == 2, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"
    assert cmd2 == stdout[1], "Wrong command in the database!"

def test_by_session_and_by_path(remove_db):
    sess_name = "test-name"
    first_sess, cmd1, _, cmd2, path = prepare_several_sessions(sess_name)

    stdout = select_by_session_and_path(first_sess, path)

    assert len(stdout) == 1, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"

def test_by_session_and_by_path_rec(remove_db):
    sess_name = "test-name"
    first_sess, cmd1, _, cmd2, path = prepare_several_sessions(sess_name)

    stdout = select_by_session_and_path(first_sess, path, recursive=True)

    assert len(stdout) == 2, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"
    assert cmd2 == stdout[1], "Wrong command in the database!"

def test_select_without_session_name(remove_db):
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"
    db_obj = basic_create_db()

    insert(db_obj, pwd, cmd, 0)
    
    stdout = select_by_session(db_obj, return_code = 3)

