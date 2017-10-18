# Chinese Room
A light-weight cross-platforman Embedded C++1x SDK core suitable for both software and firmware based on Searle's Chinese Room, Interprocess, and Telemetry Protocol (Script) and mimicry sciences.

## Overview
This project is the SDK core for [Kabuki Starship](https://github.com/kabuki-project) and [Kabuki Toolkit](https://github.com/kabuki-project/). The SDK is based on an Embedded C++1x core DLL with bindings for various languages. All library functions are precompilable C Compatible function calls, and all OOP, Function, etc programming is only used for application side (which is not 100% coverage yet. #WorkInProgress). All module source files are organized into a single  directory without sub-folders, all projects (like example projects) in a single folder, and all unit tests are placed into the ~/source/kabuki/TESTS directory. For UML  diagrams please see the ~/docs/kabuki-toolkit_diagrams.mdj [StarUML] (http://staruml.io/) file.

## License
Copyright 2017 (C) [Cale McCollough] (mailto:calemccollough@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at [here](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

## Development
The current module under construction is the Automata scanner. The protocol is doing some expensive hashing of the serial data and that is not going to work well for interprocess communication (IPC). The primary need is to get the  UART/SPI/etc communication working but IPC needs to kept in mind. It is too expensive and unnecessary to has interprocess data because it is not being  sent over an unstable connection.

## Todo
* Finish Bag data RW functions.
* Finish Automata.
* Serial PC/mbed demo.
* Look into replacing code with array.h functions (if possible).
