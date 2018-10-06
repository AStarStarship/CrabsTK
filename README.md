  
```
  /\
 ( /   @ @    ()   _  __     _           _    _   _____           _ _    _ _   
  \\ __| |__  /   | |/ /__ _| |__  _   _| | _(_) |_   _|__   ___ | | | _(_) |_
   \/   "   \/    | ' // _` | '_ \| | | | |/ / |   | |/ _ \ / _ \| | |/ / | __|
  /-|       |-\   | . \ (_| | |_) | |_| |   <| |   | | (_) | (_) | |   <| | |_
 / /-\     /-\ \  |_|\_\__,_|_.__/ \__,_|_|\_\_|   |_|\___/ \___/|_|_|\_\_|\__|
  / /-`---'-\ \  
   /         \                    ♫♪.ılılıll|̲̅̅●̲̅̅|̲̅̅=̲̅̅|̲̅̅●̲̅̅|llılılı.♫♪
```

![Under Construction Image](https://github.com/kabuki-starship/kabuki-toolkit/wiki/under_construction.png)

Kabuki Toolkit (KT) is a cross-platform Firmware-to-Software Development Kit (F2SDK) and Virtual Machine for 32-bit and 64-bit processors featuring the F2 Foundational Framework (F2), Chinese Room Abstract Stack Machine (Crabs), and Kabuki Virtual Machine (VM) that provides:

* Standard Seam Trees provide in-order unit tests for Agile and Issue Driven Development with debug information customized for each tree node best described as a bulleted list of seams with SI Units to denote tree depth in the order: LAYER_{MILLI_MICRO_NANO_PICO_FEMTO}__SPRINT_SCRUM.
* Modern Embedded-C++1x core with Doxygen API docs.
* ASCII Data Types and the Crabs Factory operate seamlessly across assembly boundary with optimal RAM usage and CPU cache performance from ROM-able ASCII Contiguous Objects.
* Rapid compile time using C Application Binary Interface (ABI) with separated C++ templates for cross-language bindings.
* Code automatically formatted to Google C++ Style Guide upon save and formatted to not fight clang-format.
* Deep Internet-of-Things integration using the SCRIPT Software Defined Networking Protocol where everything is A=A*B.
* Artificial Intelligence and Deep Learning library with graph-based neural-intelligent networks coming soon.
* Modeled after AI philosophy and ASCII mimicry of the Chinese Room Thought Experiment and C0 Control Codes.

## ASCII Data Types

ASCII Data Types provide a suitable replacement for the C++ std library suitable for embedded systems and AI that facilitates:

* All of the C++ POD types.
* Year 2038-safe 32-bit, 64- bit and dual-32-bit with 16-year epoch and sub-second tick timestamps.
* All data types are 64-bit aligned so they may be rapidly copied from one system to another on homo-endian system.
* Optional MSB variant encoding provides fast data compression similar to UTF-8.
* Contiguous Objects
  * UTF-8, UTF-16, and UTF-32 strings.
  * Stack - A stack of POD types in the form of a bounded-sized array.
  * Array - A multidimensional array with Stack of dimensions.
  * Loom - A homogeneous array of UTF-8, UTF-16, or UTF-32 strings.
  * Table - A hash table of contiguous mappings.
  * Map - A sparse map of unsigned integers to ASCII Data Types.
  * Book - A multidictionary (i.e. unordered map) without hash table.
  * Dictionary - A dictionary of ASCII Data Types with hash table.
  * B-Sequence - Describes the order and maximum sizes of a Byte-Sequence of ASCII Data Types.
  * Expression - Asynchronous Chinese Room Script Expressions capable of concurrently executing scripts in multiple language in real-time.

[![GitHub version](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit.svg)](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit)

## Quick Links

* [FAQ](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/faq.md)
  - *Frequently asked questions.*
* [Quickstart Guide](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/quickstart-guide.md)
  - *Check here for the Kabuki Toolkit Wiki and GitHub repository!*
* [Script Specification RFC](https://github.com/kabuki-starship/script/wiki/SCRIPT-Specification-RFC)
  - *Release for Comment for Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification.*
* [Kabuki Starship Website](https://kabuki-starship.github.io/)
  - *Official Kabuki Toolkit website. We are currently looking for someone to help us fix the CSS on the website. It only works right at < 1024 pixel width so the problem is in the @media section.*

## Kabuki Toolkit Modules

| Module | Description |
|:------:|:------------|
| crabs  | Chinese Room Abstract Stack Machine. |
| tek    | Firmware toolkit. |
| ai     | Artificial Intelligence library. |
| al     | Audio library. |
| app    | Application framework. |
| data   | Data and logging library. |
| hmi    | Human-Machine interface. |
| id     | Identification and authorization library. |
| gl     | Graphics library. |
| gui    | Graphical user interface. |
| math   | Math library. |
| pl     | Plotting library. |
| midi   | Musical instrument digital interface. |
| pro    | Project and scheduling library. |
| store  | Store and inventory library. |

### 3rd Party APIs

* [boost asio and filesystem](https://www.boost.org/)
* [SDL](https://www.libsdl.org/)
* [Cairo](https://www.cairographics.org/)
* [PLplot](https://github.com/PLplot/PLplot)
* [ANGLE](https://github.com/google/angle)
* [PortAudio](http://www.portaudio.com/)
* [Maximilian](https://github.com/micknoise/Maximilian)
* [RtMIDI](https://github.com/thestk/rtmidi)
* [Static Math](https://github.com/Morwenn/static_math)
* [mbed](https://www.mbed.com/en/)

### Kabuki VM Seam Package Diagram

![Kabuki VM Package Diagram](https://raw.githubusercontent.com/kabuki-starship/kabuki-toolkit/master/docs/kabuki_vm_package_diagram.jpg)

**@note** *Package Diagram uses `C++` dependency inclusion, meaning that when a module/namespace is included all of the module's/namespace's dependencies are also included.*

## Author

* [Cale McCollough](https://calemccollough.github.io) <[cale.mccollough@gmail.com](mailto:cale.mccollough@gmail.com)>
* Authors of third-party APIs attributed in source headers.

## The License

Copyright 2014-18 (C) [Cale McCollough](mailto:calemccollough@gmail.com) and contributors. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
