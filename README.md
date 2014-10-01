roboime-control
===============

Control module for the team. Runs separately from the decision module and dispatches commands to the robots via a usb radio.

Requirements
============

You'll need CMake to generate the project files. Also, you'll need libusb and the 1.0 API headers (NOT 0.1), ZeroMQ libs and the zmq.hpp headers, Google Protobuf and a relatively recent version of the boost libraries. To compile, you need a compiler that implements C++11 initializer lists (e.g. not Visual Studio <2013)

Configuration
=============
Configuration is stored in a config.json file that stays on the same folder as the executable. All the notable configurations are in the default config distributed.
