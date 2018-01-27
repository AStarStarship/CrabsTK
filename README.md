﻿﻿﻿﻿# Kabuki Toolkit

Kabuki Toolkit (KT) is an Apache 2.0 Licensed cross-platform software and 
firmware toolkit with Embedded-C++1x core for making just about anything. KT 
contains a compliant SCRIPT Protocol implementation built on top of some of the 
best commercial friendly open-source libraries available on the internet all 
formatted using the 
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## Script
Searle's Chinese Room, Interprocess, and Telemetry (SCRIPT) Protocol is a 
socket-based object-oriented software-defined networking protocol and abstract stack machine. It can be described as a mix of network protocol with JavaScript, JSON, Lua Script, and LISP with data compression that can handle a lot more messages than  [Open Sound Control](https://en.wikipedia.org/wiki/Open_Sound_Control). Using  Script you can easily create higher-level software defined networks using a  cross-language embedded-friendly abstract object stack with multiple expression  input and output parameter sockets that may change at runtime using sockets. Script is currently interpreted and we're looking for some help optimizing  away the virtual function overhead and translating Interprocess LISP to ASM.

## Quick Links

* [FAQ](https://github.com/kabuki-starship/kabuki-toolkit/blob/master/docs/faq.md) 
  *- Check here for the Kabuki Toolkit Wiki and GitHub repository!*
* [Quickstart Guide](https://github.com/kabuki-starship/kabuki-toolkit/blob/master/docs/quickstart-guide.md)
  - *Check here for the Kabuki Toolkit Wiki and GitHub repository!*
* [Kabuki Starship Website](https://kabuki-starship.github.io/)
  - *Official Kabuki Toolkit website. We are currently looking for someone to help
  us fix the CSS on the website. It only works right at < 1024 pixel width so 
  the problem is in the @media section.*
* [Script Specification RFC](https://github.com/kabuki-starship/kabuki-toolkit/wiki/SCRIPT-Specification-RFC)
  - *Release for Comment for Searle's Chinese Room, Interprocess, and Telemetry (SCRIPT) 
  Specification for Script and Interprocess LISP.

## SDK Modules

|   Name | Description  |
|-------:|:-------------|
|     al | Audio Library.|
|    app | Application framework.|
|   code | Coding utilities.|
|   data | Data and data logging.|
|     gl | Graphics Library.|
|    gui | Graphical user interfaces.|
|    hmi | Human-machine Interfaces.|
|   json | JSON.|
|     id | Identification.|
|   math | Math.|
|   midi | MIDI.|
|    tek | Firmware toolkit.|
|    pro | Projects, tasks, and scheduling.|
| script | Chinese Room Automata.|
|  store | Store and inventory.|
|  video | Video.|
|    web | WWW and IoT.|

### 3rd Party APIs

* [Google C++ API](https://github.com/Microsoft/cpprestsdk)
* [ANGLE](https://github.com/google/angle)
* [nanovg](https://github.com/memononen/nanovg)
* [nanogui](https://github.com/wjakob/nanogui)
* [stb](https://github.com/nothings/stb)
* [Maximilian](https://github.com/micknoise/Maximilian)
* [Static Math](https://github.com/Morwenn/static_math)
* [mbed](https://www.mbed.com/en/)
* [OpenAL-Soft](https://github.com/kcat/openal-soft)
* [PortAudio](http://www.portaudio.com/)
* [RtMIDI](https://github.com/thestk/rtmidi)
* [AStyle](http://astyle.sourceforge.net/)
* [SFML (Replacing with SDL)](https://www.sfml-dev.org/)

## Author

* [Cale McCollough](https://calemccollough.github.io) <[cale.mccollough@gmail.com](mailto:cale.mccollough@gmail.com)>

## License

Copyright 2017 (C) [Cale McCollough](mailto:calemccollough@gmail.com) and contributors. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [here](http://www.apache.org/licenses/LICENSE-2.0).

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
