roboime-control
===============

Control module for the team. Runs separately from the decision module and dispatches commands to the robots via a usb radio.

Requirements
============

You'll need CMake to generate the project files. Also, you'll need libusb and the 1.0 API headers, ZMQ libs and the zmq.hpp headers.

Configuration
=============
Configuration is stored in a config.json file that stays on the same folder as the executable.
