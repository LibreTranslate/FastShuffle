import pytest
from removedup import file_shuffle
import os

def test_shuffle():
    cwd = os.path.dirname(__file__)

    src, tgt = file_shuffle(os.path.join(cwd, "data", "src.txt"), os.path.join(cwd, "data", "tgt.txt"))

    assert os.path.isfile(src)
    assert os.path.isfile(tgt)

    assert sum(1 for _ in open(src)) == 5
    assert sum(1 for _ in open(tgt)) == 5
    
    
