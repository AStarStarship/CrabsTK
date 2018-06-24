
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

Kabuki Toolkit (KT) is a cross-platform Firmware-to-Software Development Kit (F2SDK) for 32-bit and 64-bit processors featuring the F2 Foundational Framework that provides:

* UML-compatible interface for cross-language bindings.
* Reduced compile time using hidden C++ template implementations hidden behind a C ABI with object-oriented wrapper classes.
* Seamless operation across assembly boundary.
* Page-Major-Minor Seam (PMMS) Unit Tests provides debug information customized for each seam.

[![GitHub version](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit.svg)](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit)

## Quick Links

* [FAQ](https://github.com/kabuki-starship/kabuki-toolkit/blob/master/docs/faq.md)
  - *Frequently asked questions.*
* [Quickstart Guide](https://github.com/kabuki-starship/kabuki-toolkit/blob/master/docs/quickstart-guide.md)
  - *Check here for the Kabuki Toolkit Wiki and GitHub repository!*
* [Script Specification RFC](https://github.com/kabuki-starship/script/wiki/SCRIPT-Specification-RFC)
  - *Release for Comment for Searle's Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification.*
* [Kabuki Starship Website](https://kabuki-starship.github.io/)
  - *Official Kabuki Toolkit website. We are currently looking for someone to help us fix the CSS on the website. It only works right at < 1024 pixel width so the problem is in the @media section.*

## Kabuki Toolkit Modules

| Module | Description |
|:------:|:------------|
| crabs  | Chinese Room Abstract Stack Machine. |
| tek    | Firmware Toolkit. |
| ai     | Artificial Intelligence Library. |
| al     | Audio Library. |
| app    | Application Framework. |
| data   | Data and logging Library. |
| hmi    | Human-Machine Interfaces. |
| id     | Identification and authorization Library. |
| gl     | Graphics Library. |
| gui    | Graphical User Interface library. |
| math   | Math library. |
| pl     | Plotting library. |
| midi   | Musical Instrument Digital Interface. |
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

## Authors

* [Cale McCollough](https://calemccollough.github.io) <[cale.mccollough@gmail.com](mailto:cale.mccollough@gmail.com)>, and authors of third-party APIs.

## The License

Copyright 2014-18 (C) [Cale McCollough](mailto:calemccollough@gmail.com) and contributors. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
