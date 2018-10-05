# `~/kabuki`

This folder contains the Kabuki Toolkit library source code, a Visual Studio 2017 Project (kabuki_toolkit.sln), Eclipse Project, and CMake Project (CMakeLists.txt). Please @see `~/docs/quickstart-guide.md`.

## Module Descriptions

### CRABS

Chinese Room Abstract Stack Machine is a virtual machine.

### drivers

Drivers for common hardware peripherals.

#### drivers

* Serial
* I2C
* CAN
* Ethernet
* WiFi

### features

Common platform features.

#### Features

* Filesystem
* RtAudio
* RtMidi

### hal

Hardware Abstraction Layer handles the lowest of the low level stuff.

### platform

Platforms include bare metal and operating systems.

#### Target Platforms

* Windows
* mbed OS
* mbed Bare Metal.
* Unix (Consists of Unix, OSX, and Linux)
* OSX
* Win32
* Android
* iOS

### sloth

Utility for refactoring multiple third-party libraries into a single unified SDK.

### tek

Firmware toolkit

#### Gadget firmware

* Unicontroller - A universal controller and mixer for byte-steam devices like MIDI and DMX using Script.
* AnyLED - A universal LED light controller that can control any LED.
* FlowerBed - A automated plant water.

### toolkit

General software and firmware toolkit.

## Author

* [Cale McCollough](https://calemccollough.github.io) <[cale.mccollough@gmail.com](mailto:cale.mccollough@gmail.com)>
* Authors of third-party APIs attributed in source headers.

## The License

Copyright 2014-18 (C) [Cale McCollough](mailto:calemccollough@gmail.com) and contributors. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
