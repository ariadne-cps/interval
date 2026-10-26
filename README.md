# Ariadne Interval

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Unix Status](https://github.com/ariadne-cps/interval/actions/workflows/unix.yml/badge.svg)](https://github.com/ariadne-cps/interval/actions/workflows/unix.yml)
[![Windows Status](https://github.com/ariadne-cps/interval/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/interval/actions/workflows/win.yml)
[![Sanitizers Status](https://github.com/ariadne-cps/interval/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/interval/actions/workflows/sanitizers.yml)
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

Interval depends on [ariadne-cps/numeric](https://github.com/ariadne-cps/numeric), included as a Git submodule. Its transitive dependencies are managed through the repository submodules.

## Build

Clone the repository together with its Git submodules:

```bash
git clone --recurse-submodules https://github.com/ariadne-cps/interval.git
cd interval
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel --target ariadne-interval bindings
cmake --install build-release --prefix install
```

A C++20 compiler and CMake are required. Python 3.11+ with
development headers is required for the optional Python bindings;
disable them with `-DBUILD_PYTHON_BINDINGS=OFF` if they are not needed.

## Tests

Build and run C++ and Python tests in a separate Debug build:

```bash
python -m pip install pytest
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug -DPYTHON_ENFORCE_TESTS=ON
cmake --build build-debug --parallel --target tests
ctest --test-dir build-debug -LE python --output-on-failure
ctest --test-dir build-debug -L python --output-on-failure
```

## Sanitizers

The Sanitizers workflow runs separate Debug jobs with ThreadSanitizer
(TSan) and AddressSanitizer + UndefinedBehaviorSanitizer (ASan + UBSan)
on Ubuntu/Clang. Python bindings are disabled in these jobs.

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
