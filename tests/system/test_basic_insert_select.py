import pytest

from common import *

def test_session_numbering(remove_db):
    name = "test session"
    
    db0, sess_id_0 = create_session("test", name)
    assert sess_id_0 == 1

    db1, sess_id_1 = create_session("test", name)
    assert sess_id_1 == 2


def test_basic_insert_select(remove_db):
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"

    basic_insert_select(cmd, pwd)


def test_select_with_underscores(remove_db):
    pwd = "/some/arbitrary/path_with_underscores"
    cmd = "some command number one"
    
    basic_insert_select(cmd, pwd)


def test_select_with_dots(remove_db):
    pwd = "/some/a.r.b.i.t.r.a.r.y/.path"
    cmd = "some command number two"
    
    basic_insert_select(cmd, pwd)


def test_select_with_hypens(remove_db):
    pwd = "/some/arbitrary/path-with-hyphens"
    cmd = "some command number three"
    
    basic_insert_select(cmd, pwd)


def test_select_with_spaces(remove_db):
    pwd = "/some/arbitrary/path with hyphens"
    cmd = "some command number three"

    basic_insert_select(cmd, pwd)


def test_select_with_cyrillic (remove_db):
    pwd = "/some/arbitrary/path_with_кириллица"
    cmd = "some command number three"

    basic_insert_select(cmd, pwd)
    
