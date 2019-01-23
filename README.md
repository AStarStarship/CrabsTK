
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

Kabuki Toolkit (KT) is a cross-platform Embedded-C++11 Firmware-to-Software Development Kit (F2SDK), Virtual Machine for 32-bit and 64-bit processors, and software toolkit featuring:

* Script^2 IoT and Seam Tree Test nanoframework that provides custom debug code for each branch and leaf.
* VM based on mbed with modular features.
*

[![GitHub version](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit.svg)](https://badge.fury.io/gh/kabuki-starship%2Fkabuki-toolkit)

## Quick Links

* [FAQ](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/faq.md)
  - *Frequently asked questions.*
* [Quickstart Guide](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/quickstart-guide.md)
  - *Check here for the Kabuki Toolkit Wiki and GitHub repository!*
* [Script Specification RFC](https://github.com/kabuki-starship/script2/blob/master/docs/script_specification_rfc.md)
  - *Release for Comment for Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification.*
* [Kabuki Starship Website](https://kabuki-starship.github.io/)
  - *We are currently in need of some help fixing the CSS on the website. It only works right at < 1024 pixel width so the problem is in the @media section. Please make pull request to issue #1 in [https://github.com/kabuki-starship/kabuki-starship.github.io/issues](https://github.com/kabuki-starship/kabuki-starship.github.io/issues).*

## kabuki Modules

| Module    | Description |
|:---------:|:------------|
| F2SDK     | Firmware-to-Software Development SDK. |
| Features  | System features. |
| HAL       | Hardware Abstraction Layer. |
| OSAL      | Operating System Abstraction Layer. |
| Platform  | Hardware platform. |
| Tek       | Kabuki tek firmware. |

## kabuki.drivers
| Module     | Description |
|:----------:|:------------|
| Ethernet   | Basic TCP support. |
| Serial     | UART/Serial driver. |
| ...        | Many more drivers coming soon. |

## kabuki.features

| Module     | Description |
|:----------:|:------------|
| Audio      | Audio library. |
| AI         | Artificial Intelligence library. |
| LTM        | Long-term memory and filesystem library. |
| Math       | (Coming soon) Math library. |
| Graphics   | Graphics library. |
| MIDI       | Musical instrument digital interface. |
| Touch      | Human-Machine interface. |

## kabuki.f2sdk Modules

| Module   | Description |
|:--------:|:------------|
| ADSP     | Audio Digital Signal Processing library. |
| App      | Application framework. |
| STM      | Short-term memory data structures library. |
| Drawing  | Vector drawing library. |
| GUI      | Graphical user interface. |
| Plotting | Plotting library. |
| Pro      | Project library. |
| uWeb     | HTTP server and web microframework. |
| Who      | Identification and authorization library. |

## kabuki.tek Firmware

All currently broken. Sorry. Please contribute as a developer. Thanks.

| Module        | Description |
|:-------------:|:------------|
| AnyLED        | Firmware to control PWM and addressable LEDs. |
| Unicontroller | A universal controller firmware for MIDI, DMX, SCRIPT SDNP, and OSC. |
| OregonCooler  | Automatic watering system for plants and smart-home cooling system (coming soon). |

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

The Mission is to target GCC 4.7, the minimum C++ version required to compile boost, to allow for maximized firmware and web server distribution, and to provide at least one demo application in each language that we bind too to assist in making cross-language bindings feel as native as possible.

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

Rename NanoGUI to Kabuki.SDK.GUI.Retained and convert to Script2 C++ API.

### Task 5

Rename IMGUI to Kabuki.SDK.GUI.Intermediate and convert to Script2 C++ API.

### Task 6

Rename PLPlot to Kabuki.SDK.Plotting and convert to Script2 C++ API.

### Task 7

Combine common Kabuki.SDK.GUI.Retained and Kabuki.SDK.GUI.Intermediate code into Kabuki.Feature.Touch and abstract away rendering engine to use Cairo or NanoVG.

### Task 8

Get Maximillian working with PortAudio.

### Task 9

Rename PortAudio to Kabuki.Feature.Audio and convert to Script2 C++ API.

### Task 10

*Under Development:* **Kabuki Contacts**

Develop Kabuki Contacts by binding NanoGUI to NanoVG **and** PLPlot with a Cairo back-end running on ANGLE and replace all of the C++ std lib code with Script2 C++ API. NanoVG is pretty small so this process **HOPEFULLY** shouldn't take very long. The end-goal is to ASCII C++ Style Guide compliance refactoring with the #Sloth and to not be reliant on a slow rendering engine. We will have to use a dedicated rendering engine for DLL plugins that don't require the highest frame-rates and to work with Skia when high frame-rates are needed and to be able to switch from Retained to Intermediate Mode GUI.

### Task 11

*Under Development:* **Kabuki Cards**

Develop Kabuki Cards and a good Xamarin app with server with my **SUPER** basic 3D engine and interface with the A-Startup store so we can sell products.

### Task 12

Rename Maximillian to Kabuki.Feature.ADSP and convert to Script2 C++ API.

### Task 13

Split boost.asio into to HAL and Drivers and convert to Script2 C++ API and rename boost.beast to Kabuki.SDK.uWeb.

# The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
