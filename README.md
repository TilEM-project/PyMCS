# PyMCS

This library is a pybind11 based wrapper of the SmarAct MCS C API.

## Installation

1. Make a build directory: `mkdir build && cd build`
1. Configure the project: `cmake ..`
1. Install the MCS library: `sudo cmake --build . --target mcs-install-lib`
1. Build and install the Python library: `python setup.py install`