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
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"

    basic_insert_select(pwd, cmd)


def test_select_with_underscores(remove_db):
    pwd = "/some/arbitrary/path_with_underscores"
    cmd = "some command number one"
    
    basic_insert_select(pwd, cmd)


def test_select_with_dots(remove_db):
    pwd = "/some/a.r.b.i.t.r.a.r.y/.path"
    cmd = "some command number two"
    
    basic_insert_select(pwd, cmd)


def test_select_with_hypens(remove_db):
    pwd = "/some/arbitrary/path-with-hyphens"
    cmd = "some command number three"
    
    basic_insert_select(pwd, cmd)


def test_select_with_spaces(remove_db):
    pwd = "/some/arbitrary/path with hyphens"
    cmd = "some command number three"

    basic_insert_select(pwd, cmd)


def test_select_with_cyrillic (remove_db):
    pwd = "/some/arbitrary/path_with_кириллица"
    cmd = "some command number three"

    basic_insert_select(pwd, cmd)
    

def test_recursive_select(remove_db, create_db):
    pwd1 = "/some/arbitrary/path"
    cmd1 = "some --arbitrary command"

    pwd2 = "/some/arbitrary/path/inside"
    cmd2 = "some --other command"

    insert(create_db, pwd1, cmd1, 0)
    insert(create_db, pwd2, cmd2, 0)
    
    stdout = select(create_db, pwd1)

    assert len(stdout) == 1, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"

    stdout = select(create_db, pwd1, True)

    assert len(stdout) == 2, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"
    assert cmd2 == stdout[1], "Wrong command in the database!"
