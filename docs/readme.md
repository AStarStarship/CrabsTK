# Kabuki Toolkit Documentation

Please **@see** `~/kabuki-toolkit.mdj` using [StarUML](http://staruml.io) for object models. Documents are split between the Wiki and the `~/docs/` folder and additional documents can be found in the [script2 repo](https://github.com/kabuki-starship/script2). 

## Docs

* [Script Specification RFC](https://github.com/kabuki-starship/script2) - Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification Release for Comment and SCRIPT Script (Script2).
* [ASCII C++ Style Guide](https://github.com/ascii_cpp_style_guide) - A combination of the Google C++ Style Guide and the ASCII Data Specificaiton in Markdown format.
* [Frequently Asked Questions](#faq) - The best place the start.
* [Quick Start Guide](#quick-start-guide) - The second best place the start.

## Frequently Asked Questions

```AsciiArt
__/\\\\\\\\\\\\\\\_____/\\\\\\\\\___________/\\\_______
 _\/\\\///////////____/\\\\\\\\\\\\\______/\\\\/\\\\____
  _\/\\\______________/\\\/////////\\\___/\\\//\////\\\__
   _\/\\\\\\\\\\\_____\/\\\_______\/\\\__/\\\______\//\\\_
    _\/\\\///////______\/\\\\\\\\\\\\\\\_\//\\\______/\\\__
     _\/\\\_____________\/\\\/////////\\\__\///\\\\/\\\\/___
      _\/\\\_____________\/\\\_______\/\\\____\////\\\//_____  
       _\/\\\_____________\/\\\_______\/\\\_______\///\\\\\\__
        _\///______________\///________\///__________\//////___
```

The Doxyfile is used to create the [Kabuki Starship API Docs](https://kabuki.github.io/api/). You may build these yourself for offline viewing. The UML diagrams can be found in the "Kabuki Toolkit Diagrams.mdj" file.

---

* Where should I start?
  * The best place to start is with the [Quickstart Guide](#quick-start-guide).

---

* How do I get started as a developer?
  * The best way to get started as a developer is to go through and clean up the code. This SDK was created from a hodgepodge of various open-source commercial-friendly BSD-style licensed software. There is still a lot of mess to clean up from the integration process. Whatever questions you have other people will no doubt ask as well. Please add these questions to this FAQ and to relevant example projects and API docs.

---

* What is the documentation and documentation process?
  * This SDK uses [Doxygen](http://www.stack.nl/~dimitri/doxygen/). The project is currently setup using D:\Workpace. API docs get exported to the [Kabuki Theater website].

---

* Why does the website look all messed up?
  * The [Kabuki Toolkit website](https://kabuki.github.io/) needs to get fixed ASAP. The CSS is messed up in full-screen mode, but works right in less than about 1024 pixel widths. It's something about the @media section in the CSS.

---

* What is the style guide?
  * The [ASCII C++ Style Guide](https://github.com/kabuki-starship/ascii_cpp_style_guide), which is based on the Google C++ Style Guide and the ASCII Data Specification.

---

* Is there any UML?
  * We're using [StarUML 2.x](http://staruml.io/). The models can be found in the file "~/Documents/Kabuki Starship.mdj". Most of the included modules were not documented correctly and will need to be back annotated. This is a back burner project and is only done when needed. Please feel free to help us write use cases and scenario.

---

## Quick Start Guide

```AsciiArt
________       _____      ______            _____              _____
___  __ \___  ____(_)________  /__   _________  /______ _________  /_
__  / / /  / / /_  /_  ___/_  //_/   __  ___/  __/  __ `/_  ___/  __/
 / /_/ // /_/ /_  / / /__ _  ,<      _(__  )/ /_ / /_/ /_  /   / /_
 \___\_\\__,_/ /_/  \___/ /_/|_|     /____/ \__/ \__,_/ /_/    \__/
```

**1.** Clone the kabuki-toolkit and script2 repos into your workspace or project 3rd-party dependencies directory:

```Console
git clone https://github.com/kabuki-starship/kabuki-toolkit.git
cd kabuki-toolkit
git clone https://github.com/kabuki-starship/script2.git
```

**2.** Open the `kabuki-toolkit.sln` Visual Studio Project.

**3.** Start by looking in the `0_0_00.experiments` project. Open the `global_config.inl` and look at the typedef(s) and seam macros, then set the seam number in the `pch.h` file to one in the `global_config.inl`. Then look through the `test_debug.inl` and `test_release.inl`, the look through the seam tree tests in the files that start with numbers and see how the tests tree builds seams layer by layer.

**4.** In order to save the stack debug data when an DASSERT occurs, set breakpoint in the file `script2_test.cc` at the return line for:

```C++
BOL TestWarn(const CH1* function, const CH1* file, SI4 line);
```

## The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
