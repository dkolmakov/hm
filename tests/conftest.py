import os

import pytest


@pytest.fixture
def remove_db():
    for dirpath, _, files in os.walk('.'):
        for f in files:
            _, ext = os.path.splitext(f)
            if ext == ".testdb": 
                os.remove(os.path.join(dirpath, f)) 

