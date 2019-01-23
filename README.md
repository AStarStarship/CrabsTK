
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

* [FAQ](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/faq.md)
  - *Frequently asked questions.*
* [Quickstart Guide](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/quickstart-guide.md)
  - *Check here for the Kabuki Toolkit Wiki and GitHub repository!*
* [Script2](https://github.com/kabuki-starship/script2.md)
  - *Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Script (Script2).*
* [Kabuki Starship Website](https://kabuki-starship.github.io/)

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
| touch       | Human-Machine interface. |

## kabuki.toolkit Modules

| Module   | Description |
|:--------:|:------------|
| adsp     | Audio Digital Signal Processing library. |
| data     | Database library. |
| drawing  | Vector drawing library. |
| gui      | Graphical user interface. |
| plotting | Plotting library. |
| pro      | Project library. |
| uweb     | HTTP server and web microframework. |
| who      | Identification and authorization library. |

## kabuki.tek Firmware

| Module        | Description |
|:-------------:|:------------|
| anyled        | Firmware to control PWM and addressable LEDs. |
| unicontroller | A universal controller firmware for MIDI, Script2, DMX, and OSC. |
| oregon_cooler | Automatic watering system for plants and smart-home cooling system (coming soon). |

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

*     0_0 Frontal Cortex
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

The Vision of KT is to provide a bloat-free DLL-friendly Embedded-C++11 Virtual Machine, Firmware-to-Software Development Kit, dynamic functional programming language, Jupyter Notebooks, and a real-time smartphone operating system that is the quality of Google Fuchsia, JUCE, Java, and Julia but with a commercially-permissive license.

The Mission is to target the minimum C++ version required to compile the third-party dependencies, convert as much to Script2 as possible and mash it up with mbed for x86_64 and RISC-V to allow for maximized firmware and web server distribution, and to provide at least one demo application in each language that we bind too to assist in making cross-language bindings feel as native as possible.

## Development Strategy

It's pretty easy to get lost in KT. The trick is to put blinders on and only work on one thing at a time. This makes having a single repo over multiple repos a good thing right now still because it forces you to only work on one features at a time in an agile fashion using the night-and-day principle.

### Task 1

Get NanoGUI and NavoVG working with ANGLE.

### Task 2

Get PLPlot with a Cairo backend working with ANGLE.

### Task 3

Get IMGUI working with ANGLE.

### Task 4

**@dependency** Task

Rename NanoGUI to kabuki.toolkit.gui.retained and convert to Script2 C++ API.

### Task 5

Rename IMGUI to kabuki.toolkit.gui.intermediate and convert to Script2 C++ API.

### Task 6

Rename PLPlot to kabuki.toolkit.plotting and convert to Script2 C++ API.

### Task 7

Combine common kabuki.toolkit.gui.retained and kabuki.toolkit.gui.intermediate code into kabuki.Features.Touch and abstract away rendering engine to use Cairo or NanoVG.

### Task 8

Get Maximillian working with PortAudio.

### Task 9

Rename PortAudio to kabuki.features.audio and convert to Script2 C++ API.

### Task 10

*Under Development:* **Kabuki Contacts**

Develop Kabuki Contacts by binding NanoGUI to NanoVG **and** PLPlot with a Cairo back-end running on ANGLE and replace all of the C++ std lib code with Script2 C++ API. NanoVG is pretty small so this process **HOPEFULLY** shouldn't take very long. The end-goal is to ASCII C++ Style Guide compliance refactoring with the #Sloth and to not be reliant on a slow rendering engine. We will have to use a dedicated rendering engine for DLL plugins that don't require the highest frame-rates and to work with Skia when high frame-rates are needed and to be able to switch from Retained to Intermediate Mode GUI.

### Task 11

*Under Development:* **Kabuki Cards**

Develop Kabuki Cards and a good Xamarin app with server with my **SUPER** basic 3D engine and interface with the A-Startup store so we can sell products.

### Task 12

Rename Maximillian to kabuki.Features.ADSP and convert to Script2 C++ API.

### Task 13

Split boost.asio into to hal and Drivers and convert to Script2 C++ API and rename boost.beast to kabuki.toolkit.uweb.

# The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
