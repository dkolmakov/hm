import pytest

from common import *

def test_basic_insert_select(remove_db):
    sess_name = "Some name"
    cmd = "some --arbitrary command"

    insert_select_by_session_name(cmd, sess_name)

   
