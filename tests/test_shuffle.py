import pytest
from fastshuffle import file_shuffle
import os

def test_shuffle():
    cwd = os.path.dirname(__file__)

    in_src = os.path.join(cwd, "data", "src.txt")
    in_tgt = os.path.join(cwd, "data", "tgt.txt")
    src, tgt = file_shuffle(in_src, in_tgt)

    assert os.path.isfile(src)
    assert os.path.isfile(tgt)

