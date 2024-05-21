# PyMCS

This library is a pybind11 based wrapper of the SmarAct MCS C API.

## Installation

1. Build and install the Python library using `python setup.py install`.
1. Blacklist the `ftdi_sio` and `usbserial` kernel modules by writing
    ```
    blacklist ftdi_sio
    blacklist usbserial
    ```
    to `/etc/modprobe.d/blacklist-smaract.conf`.
1. Allow all users to write to the device by writing
    ```
    ATTR{idVendor}=="0403", ATTR{idProduct}=="6001", MODE="666"
    ```
    to `/etc/udev/rules.d/98-smaract.rules`.