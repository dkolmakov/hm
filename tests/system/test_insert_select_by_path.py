import os
import pytest

from common import *


def test_trailing_separator_insert(remove_db, create_db):
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"

    insert(create_db, pwd + "/", cmd, 0)
    stdout = select(create_db, pwd)

    assert cmd == stdout[0], "Wrong command in the database!"


def test_trailing_separator_select(remove_db, create_db):
    pwd = "/some/arbitrary/path"
    cmd = "some --arbitrary command"

    insert(create_db, pwd, cmd, 0)
    stdout = select(create_db, pwd + "/")

    assert cmd == stdout[0], "Wrong command in the database!"


def test_not_normilized_current_dir(remove_db, create_db):
    cwd = os.getcwd()
    
    pwd_insert = "."
    pwd_select = cwd
    cmd = "some --arbitrary command"

    insert(create_db, pwd_insert, cmd, 0)
    stdout = select(create_db, pwd_select)

    assert cmd == stdout[0], "Wrong command in the database!"


