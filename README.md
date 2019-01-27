
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

Kabuki Toolkit (KT) is a modern embedded-C++ toolkit for making intelligent connected technologies, apps, plugins, games, servers, firmware, and art with Script2.

[![GitHub version](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit.svg)](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit)

## Quick Links

* [Script2](https://github.com/kabuki-starship/script2.md)
  - *Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Script (Script2) is the foundation of KT so it's the best place to start learning.*
* [Kabuki Starship Website](https://kabuki-starship.github.io/)
* [Documents & FAQ](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/faq.md)
  - *Check out the `/docs/readme.md` for answers to Frequently asked questions and .*

## kabuki Modules

All currently broken. Sorry. Please contribute as a developer. Thanks.

| Module    | Description |
|:---------:|:------------|
| toolkit   | Firmware-to-Software Development toolkit. |
| features  | System features. |
| hal       | Hardware Abstraction Layer. |
| osal      | Operating System Abstraction Layer. |
| platform  | Hardware platform. |
| tek       | Kabuki tek firmware. |

## kabuki.drivers
| Module     | Description |
|:----------:|:------------|
| ethernet   | Basic TCP support. |
| serial     | UART/Serial driver. |
| ...        | Many more drivers coming soon. |

## kabuki.features

| Module      | Description |
|:-----------:|:------------|
| ai          | Artificial Intelligence library. |
| filecabinet | Long-term memory and filesystem library. |
| math        | (Coming soon) Math library. |
| graphics    | Graphics library. |
| midi        | Musical instrument digital interface. |
| sound       | Sound library. |

## kabuki.toolkit Modules

| Module   | Description |
|:--------:|:------------|
| av       | Audio-visual library. |
| adsp     | Audio Digital Signal Processing library. |
| data     | Database library. |
| drawing  | Vector drawing library. |
| gui      | Graphical user interface. |
| plotting | Plotting library. |
| orm      | Object Relational Mapping library. |
| pro      | Project library. |
| uweb     | HTTP server and web microframework. |
| video    | Video library. |
| who      | Identification and authorization library. |

## projects.kabuki

| Module        | Description |
|:-------------:|:------------|
| anyled        | Early-stage universal PWM and addressable LED controller. |
| av_studio     | Early-stage Audio-visual Studio app. |
| cards         | Early-stage playing card game app. |
| connections   | Early-stage Contact list, trading, and chat app. |
| igeek         | Early-stage Interactive Genetic Evolutionary Environment for KabukiNet. |
| kabuki_vm     | Early-stage Kabuki Virtual Machine (VM). |
| kabukinet     | Early-stage KabukiNet Partial and Ordinary Differential Equation neural net (PODEN2) architecture. |
| script2       | Oversight project for Script2. |
| sloth         | Early-stage SDK automation utility for integrating and refactoring third-party libraries. |
| typecraft     | Early-stage typing tutor based on the Minecraft world. |

## projects.kabuki.tek

| Module        | Description |
|:-------------:|:------------|
| anyled        | Early-stage universal PWM and addressable LED controller. |
| autopilot     | Early-stage drone autopilot. |
| oregon_cooler | Early-state smart-home cooling system and automatic watering system. |
| unicontroller | Early-stage universal Script2, MIDI, DMX, and OSC controller. |

### 3rd Party APIs

* [ANGLE](https://github.com/google/angle) - Graphics and Windowing System.
* [boost asio](https://www.boost.org/doc/libs/1_68_0/doc/html/boost_asio.html) - Asynchronous IO.
* [boost beast](https://www.boost.org/doc/libs/1_69_0/libs/beast/doc/html/index.html) - C++11 web microframework.
* [boost filesystem](https://www.boost.org/doc/libs/1_68_0/doc/html/boost_filesystem.html) - Cross-platform filesystem.
* [Cairo](https://www.cairographics.org/) - 2D graphics library.
* [PLplot](https://github.com/PLplot/PLplot) - Math plotting library.
* [PortAudio](http://www.portaudio.com/) - Cross-platform audio driver.
* [Maximilian](https://github.com/micknoise/Maximilian) - Audio DSP and synthesis library.
* [RtMIDI](https://github.com/thestk/rtmidi) - Cross-platform MIDI driver.
* [mbed](https://www.mbed.com/en/) - Firmware abstraction layer.

### Kabuki VM Seam Package Diagram

![Kabuki VM Package Diagram](https://raw.githubusercontent.com/kabuki-starship/kabuki-toolkit/master/docs/kabuki_package_diagram.jpg)

**@note** *Package Diagram uses `C++` dependency inclusion, meaning that when a module/namespace is included all of the module's/namespace's dependencies are also included.*

#### Kabuki VM Seams

*     0_0    Frontal Cortex
	* 0_0_00 script2
	* 0_0_01 kabuki.hal
	* 0_0_02 kabuki.drivers
	* 0_0_03 kabuki.features.filecabinet
	* 0_0_04 kabuki.toolkit.data
	* 0_0_05 kabuki.toolkit.who
	* 0_0_06 kabuki.toolkit.pro
	* 0_0_07 kabuki.features.tls
	* 0_0_08 kabuki.toolkit.uweb
	* 0_0_09 kabuki.toolkit.orm
	* 0_2_10 kabuki.features.midi
	* 0_0_11 kabuki.features.math
	* 0_0_12 kabuki.features.ai
*     0_1    Somatosensory Cortex
	* 0_1_00 kabuki.features.touch
	* 0_1_01 kabuki.tek
*     0_2    Auditory Cortex
	* 0_2_01 kabuki.features.sound
	* 0_2_02 kabuki.features.adsp
	* 0_2_03 kabuki.toolkit.music
*     0_3    Visual Cortex
	* 0_3_00 kabuki.features.lights
	* 0_3_01 kabuki.features.graphics
	* 0_3_02 kabuki.features.camera
	* 0_3_03 kabuki.features.webcam
	* 0_3_04 kabuki.features.drawing
	* 0_3_05 kabuki.toolkit.plotting
	* 0_3_06 kabuki.toolkit.gui
	* 0_3_07 kabuki.toolkit.video
	* 0_3_08 kabuki.features.ai

## Vision and Mission Statement

The Vision of KT is to provide a bloat-free DLL-friendly Embedded-C++11 Virtual Machine, Firmware-to-Software Development Kit (F2SDK), dynamic functional programming environment, Jupyter Notebooks, and a real-time smartphone operating system that is the quality of Google Fuchsia, JUCE, Java, and Julia but with a commercially-permissive license.

The Mission is to target the minimum C++ version required to compile the third-party dependencies, convert as much code to Script2 as possible over time and mash it up with mbed for x86_64 and RISC-V to allow for maximized firmware and web server distribution, and to provide at least one demo application in each language that we bind too to assist in making cross-language bindings feel as native as possible.

# The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
