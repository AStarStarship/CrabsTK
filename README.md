﻿﻿﻿﻿# Kabuki Toolkit

Kabuki Toolkit (KT) is a modern cross-platform C++1x software and firmware toolkit for making intelligent connected technologies with Script and the Kabuki VM. KT is built on top of some of the  best commercial-friendly open-source C and C++ libraries formatted using the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). #WorkInProgress

## Searle's Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification (Script)

Script is a software-defined networking protocol and embedded-friendly family of languages that facilitates artificial intelligence and machine learning (AIML) using functional programming and natural language processing (NLP). Script is best described as a mix of JavaScript, Python, and LISP. JavaScript because it's asynchronous with a round-robin scheduler, Python because it has native dictionaries, and Script has a rather unique dialect of LISP called Interprocess LISP.

The Chinese Room is a famous artificial intelligence philosophy thought experiment invented by the philosopher John Searle. The experiment highlights the difference between strong vs weak AI and how a normal everyday room in an office or network of rooms can form to create a turning equivalent machine that can fluently speak any language. Kabuki Script is the reference Script

## Quick Links

* [FAQ](https://github.com/kabuki-starship/kabuki-toolkit/blob/master/docs/faq.md) 
  - *Frequently asked questions.*
* [Quickstart Guide](https://github.com/kabuki-starship/kabuki-toolkit/blob/master/docs/quickstart-guide.md)
  - *Check here for the Kabuki Toolkit Wiki and GitHub repository!*
* [Script Specification RFC](https://github.com/kabuki-starship/script/wiki/SCRIPT-Specification-RFC)
  - *Release for Comment for Searle's Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification.*
* [Kabuki Starship Website](https://kabuki-starship.github.io/)
  - *Official Kabuki Toolkit website. We are currently looking for someone to help us fix the CSS on the website. It only works right at < 1024 pixel width so the problem is in the @media section.*
* [Chinese Room Wiki](https://en.wikipedia.org/wiki/Chinese_room)
  - *Good place to start learning some philosophy.*

## kabuki Modules

|    Name | Description    |
|--------:|:---------------|
|     tek | Firmware toolkit. |
| toolkit | General purpose SDK. |

### kabuki::toolkit Modules

|   Name | Description    |
|-------:|:---------------|
|     al | Audio library. |
|    app | Application framework. |
|   code | Coding utilities library. |
|  crabs | Chinese room abstract stack machine library. |
|   data | Data and logging library. |
|    hmi | Human-machine interfaces library. |
|     id | Identification library. |
|  igeek | Interactive Genetic Evolutionary Environment for Kabuki. |
|      g | Graphics library. |
|    gui | Graphical User Interface library. |
|   math | Math library. |
|   midi | MIDI library. |
|    pro | Projects, tasks, and scheduling library. |
|  store | Store and inventory library. |

### Kabuki VM Package and Seam Diagram

![Kabuki VM Package and Seam Diagram](https://raw.githubusercontent.com/kabuki-starship/kabuki-toolkit/master/docs/kabuki_vm_package_diagram.jpg)

* **@note** *Package Diagram uses C++ dependency inclusion, meaning that when 
a module/namespace is included all of the module's/namespace's dependencies are 
also included.*

### 3rd Party APIs

* [Google C++ API](https://github.com/Microsoft/cpprestsdk)
* [ANGLE](https://github.com/google/angle)
* [SDL](https://www.libsdl.org/)
* [stb](https://github.com/nothings/stb)
* [Maximilian](https://github.com/micknoise/Maximilian)
* [Static Math](https://github.com/Morwenn/static_math)
* [mbed](https://www.mbed.com/en/)
* [Serial Communication Library](https://github.com/wjwwood/serial)
* [OpenAL-Soft](https://github.com/kcat/openal-soft)
* [PortAudio](http://www.portaudio.com/)
* [RtMIDI](https://github.com/thestk/rtmidi)
* [AStyle](http://astyle.sourceforge.net/)

## Author

* [Cale McCollough](https://calemccollough.github.io) <[cale.mccollough@gmail.com](mailto:cale.mccollough@gmail.com)>

## The License

Copyright 2014-18 (C) [Cale McCollough](mailto:calemccollough@gmail.com) and contributors. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [here](http://www.apache.org/licenses/LICENSE-2.0).

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
