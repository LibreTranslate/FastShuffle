import pytest
from fastshuffle import file_shuffle, file_shuffle_sample
import os

def test_shuffle():
    cwd = os.path.dirname(__file__)

    in_src = os.path.join(cwd, "data", "src.txt")
    in_tgt = os.path.join(cwd, "data", "tgt.txt")
    src, tgt = file_shuffle(in_src, in_tgt)

    assert os.path.isfile(src)
    assert os.path.isfile(tgt)

    assert sum(1 for _ in open(src)) == sum(1 for _ in open(in_src))
    assert sum(1 for _ in open(tgt)) == sum(1 for _ in open(in_tgt))
    

def test_shuffle_sample():
    cwd = os.path.dirname(__file__)

    in_src = os.path.join(cwd, "data", "src.txt")
    in_tgt = os.path.join(cwd, "data", "tgt.txt")
    src, tgt, ssrc, stgt = file_shuffle_sample(in_src, in_tgt, 1)

    assert os.path.isfile(src)
    assert os.path.isfile(tgt)
    assert os.path.isfile(ssrc)
    assert os.path.isfile(stgt)
    
    
