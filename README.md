# FastShuffle

A memory efficient Python module to quickly shuffle parallel text corpora. The main advantage is that it never loads the entire dataset in memory, using memory-mapped file offsets instead.

## Installation

```bash
pip install fastshuffle
```

## Usage

```python
from fastshuffle import file_shuffle

src, tgt = file_shuffle("source.txt", "target.txt")
print(src, tgt)
# source.txt.shuffled target.txt.shuffled
```

## Notes

Source and target must have the same number of lines. No validation checks are made.

## Build

```bash
git clone https://github.com/LibreTranslate/FastShuffle
cd FastShuffle
python setup.py build
```

## Standalone Binary

You can also use fastshuffle as a standalone Windows, macOS or Linux application (but you currently need to build from source, we don't provide binaries).

```
mkdir build
cd build && cmake .. && make -j4
./shuffle source.txt target.txt
```

## Contributing

We welcome pull requests!

## License

AGPLv3
