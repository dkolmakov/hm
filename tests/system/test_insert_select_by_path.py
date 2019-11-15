import os
import pytest

from common import *


def test_trailing_separator_insert(remove_db, create_db):
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"

    insert(create_db, pwd + "/", cmd, 0)
    stdout = select_by_path(create_db, pwd)

    assert cmd == stdout[0], "Wrong command in the database!"


def test_trailing_separator_select(remove_db, create_db):
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"

    insert(create_db, pwd, cmd, 0)
    stdout = select_by_path(create_db, pwd + "/")

    assert cmd == stdout[0], "Wrong command in the database!"


def test_not_normilized_current_dir(remove_db, create_db):
    cwd = os.getcwd()
    
    pwd_insert = "."
    pwd_select = cwd
    cmd = "some --arbitrary command"

    insert(create_db, pwd_insert, cmd, 0)
    stdout = select_by_path(create_db, pwd_select)

    assert cmd == stdout[0], "Wrong command in the database!"

def test_recursive_select(remove_db, create_db):
    pwd1 = "/some/arbitrary/path"
    cmd1 = "some --arbitrary command"

    pwd2 = "/some/arbitrary/path/inside"
    cmd2 = "some --other command"

    insert(create_db, pwd1, cmd1, 0)
    insert(create_db, pwd2, cmd2, 0)
    
    stdout = select_by_path(create_db, pwd1)

    assert len(stdout) == 1, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"

    stdout = select_by_path(create_db, pwd1, True)

    assert len(stdout) == 2, "Wrong number of commands!"
    assert cmd1 == stdout[0], "Wrong command in the database!"
    assert cmd2 == stdout[1], "Wrong command in the database!"

