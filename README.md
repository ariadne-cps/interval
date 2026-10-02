# Ariadne Interval

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Unix Status](https://github.com/ariadne-cps/interval/actions/workflows/unix.yml/badge.svg)](https://github.com/ariadne-cps/interval/actions/workflows/unix.yml)
[![Windows Status](https://github.com/ariadne-cps/interval/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/interval/actions/workflows/win.yml)
[![Coverage Status](https://github.com/ariadne-cps/interval/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/interval/actions/workflows/coverage.yml)
[![codecov](https://codecov.io/gh/ariadne-cps/interval/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/interval)

Ariadne Interval is the standalone C++20 interval layer used by Ariadne. It provides generic interval types over the numeric abstractions supplied by Ariadne Numeric, together with interval predicates, arithmetic, elementary functions and conversions used by the higher-level Ariadne libraries.

## Features

- Generic interval representation over exact, approximate and validated endpoint types.
- Exact intervals over dyadic, decimal, rational and real values.
- Double-precision exact, lower, upper and approximate interval types.
- Interval predicates including containment, disjointness, subset and refinement relations.
- Hull, intersection, splitting, widening and interval conversion operations.
- Arithmetic and elementary functions over supported validated interval types.
- Python bindings exposing the interval types and operations through `pyariadne`.

## Dependencies

Interval depends directly on:

- [ariadne-cps/numeric](https://github.com/ariadne-cps/numeric), included as a Git submodule.

Numeric provides [ariadne-cps/paradigm](https://github.com/ariadne-cps/paradigm) transitively, which in turn provides [ariadne-cps/utility](https://github.com/ariadne-cps/utility). Build configuration and the shared Python binding infrastructure are also provided through the Numeric dependency cone.

Numeric requires:

- [GMP](https://gmplib.org/).
- [MPFR](https://www.mpfr.org/).

## Build

Clone the repository together with its Git submodules:

```bash
git clone --recurse-submodules https://github.com/ariadne-cps/interval.git
cd interval
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
ctest --output-on-failure
```

A C++20 compiler, CMake, GMP and MPFR are required.

## Tutorials

The repository contains matching C++ and Python tutorials for the standalone Interval layer:

- `tutorials/tutorial_interval/tutorial_interval.cpp`
- `python/tutorials/tutorial_interval.py`

CI installs Ariadne Interval, builds and runs the C++ tutorial against that installed package, then runs the Python tutorial against the freshly built `pyariadne` module.

## Coverage

Configure a separate Debug build with coverage enabled:

```bash
mkdir build-coverage
cd build-coverage
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON
cmake --build . --parallel --target coverage
```

On Ubuntu coverage is generated with GCC/lcov. On macOS it is generated with AppleClang/LLVM coverage tools. CI uploads the macOS coverage report to Codecov.

## Contribution guidelines

If you would like to contribute to Interval, please contact the developer:

- Luca Geretti <luca.geretti@univr.it>

## License

Interval is released under the GNU General Public License v3.0.
