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
    cmd = "some command"
    
    basic_insert_select(cmd, pwd)


def test_select_with_dots(remove_db):
    pwd = "/some/a.r.b.i.t.r.a.r.y/.path"
    cmd = "some command"
    
    basic_insert_select(cmd, pwd)


def test_select_with_hypens(remove_db):
    pwd = "/some/arbitrary/path-with-hyphens"
    cmd = "some command"
    
    basic_insert_select(cmd, pwd)


def test_select_with_spaces(remove_db):
    pwd = "/some/arbitrary/path with hyphens"
    cmd = "some command"

    basic_insert_select(cmd, pwd)


def test_select_with_cyrillic (remove_db):
    pwd = "/some/arbitrary/path_with_кириллица"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_numbers (remove_db):
   pwd = "/some/arbitrary/path_with/12345"
   cmd = "some command"

   basic_insert_select(cmd, pwd)

def test_select_with_percent (remove_db):
    pwd = "/some/arbitrary/path_with_%"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_exclamation_mark (remove_db):
    pwd = "/some/arbitrary/path_with_!"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_at (remove_db):
    pwd = "/some/arbitrary/path_with_@"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_hash (remove_db):
    pwd = "/some/arbitrary/path_with_#"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_dollar (remove_db):
    pwd = "/some/arbitrary/path_with_$"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_caret (remove_db):
    pwd = "/some/arbitrary/path_with_^"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_ampersand (remove_db):
    pwd = "/some/arbitrary/path_with_&"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_question_mark (remove_db):
    pwd = "/some/arbitrary/path_with_?"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_parenthesises (remove_db):
    pwd = "/some/arbitrary/path_with_()"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_plus (remove_db):
    pwd = "/some/arbitrary/path_with_+"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_acute (remove_db):
    pwd = "/some/arbitrary/path_with_don't"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_equals (remove_db):
    pwd = "/some/arbitrary/path_with_="
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_curly_braces (remove_db):
    pwd = "/some/arbitrary/path_with_{}"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_square_brackets (remove_db):
    pwd = "/some/arbitrary/path_with_[]"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_colon (remove_db):
    pwd = "/some/arbitrary/path_with_:"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_semi_colon (remove_db):
    pwd = "/some/arbitrary/path_with_;"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_comma (remove_db):
    pwd = "/some/arbitrary/path_with_,"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_number_mark (remove_db):
    pwd = "/some/arbitrary/path_with_№"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_pass_min1 (remove_db):
    pwd = "/"
    cmd = "some command"

    basic_insert_select(cmd, pwd)

def test_select_with_pass_min_plus_1 (remove_db):
    pwd = "/a"
    cmd = "some command"

    basic_insert_select(cmd, pwd)
