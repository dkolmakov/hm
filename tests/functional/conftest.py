import os
import pytest

from common import *

@pytest.fixture
def remove_db():
    for dirpath, _, files in os.walk('.'):
        for f in files:
            _, ext = os.path.splitext(f)
            if ext == ".testdb": 
                os.remove(os.path.join(dirpath, f)) 


@pytest.fixture
def create_db():
    return basic_create_db()
