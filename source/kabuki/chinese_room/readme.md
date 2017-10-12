# Chinese Room
A light-weight cross-platform C++11 SDK core suitable for both software and  firmware based on the Script Protocol and the Chinese room argument.

## Overview
This project is the SDK core for [Kabuki Theater](https://github.com/Kabuki-Toolkit/Kabuki-Theater) and [Kabuki Tek](https://github.com/Kabuki-Toolkit/Kabuki-Tek). Precomipled  C++ libraries cannot contain other pre-compiled libraries, so for this reason  this is a header-only library. All source files are organized into a single  directory, and all unit tests are placed into the TESTS directory. For UML  diagrams please see the ChineseRoom.mdj [StarUML] (http://staruml.io/) file.

## License
Copyright 2017 (C) [Cale McCollough] (mailto:calemccollough@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at [here](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

## Development
The current module under construction is the MonoidRx scanner. The protocol is doing some expensive hashing of the serial data and that is not going to work well for interprocess communication (IPC). The primary need is to get the  UART/SPI/etc communication working but IPC needs to kept in mind. It is too expensive and unnecessary to has interprocess data because it is not being  sent over an unstable connection.

## Todo
* Finish Bag data RW functions.
* Finish Automata.
* Serial PC/mbed demo.
* Look into replacing code with array.h functions (if possible).


