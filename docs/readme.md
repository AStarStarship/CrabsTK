# Kabuki Toolkit Documentation

Please **@see** `~/kabuki-toolkit.mdj` for [StarUML](http://staruml.io) for object models. Documents are split between the Wiki and the `~/docs/` folder and additional documents can be found in the [script2 repo](https://github.com/kabuki-starship/script2). 

## Docs

* [Script Specification RFC](https://github.com/kabuki-starship/script2) - Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification Release for Comment and SCRIPT Script (Script2).
* [ASCII C++ Style Guide](https://github.com/ascii_cpp_style_guide) - Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification Release for Comment.

## Quick Start Guide

**1.** Clone the kabuki-toolkit and script2 repos into your workspace or project 3rd-party dependencies directory:

```
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

## Frequently Asked Questions

The Doxyfile is used to create the [Kabuki Starship API Docs](https://kabuki.github.io/api/). You may build these yourself for offline viewing. The UML diagrams can be found in the "Kabuki Toolkit Diagrams.mdj" file.

---

* Where should I start?
  * The best place to start is with the [Quickstart Guide](https://github.com/CaleMcCollough/kabuki-script/wiki/02-quickstart-guide).

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
  * All of the Kabuki Toolkit projects use the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). The style guide helps developers to identify the purpose of the code based on it's formatting. Most of the code is not in this format, so we could really use some helps formatting the style. We need to export a Visual Studio C++ code format file and could use some help with that.

---

* Is there any UML?
  * We're using [StarUML 2.x](http://staruml.io/). The models can be found in the file "~/Documents/Kabuki Starship.mdj". Most of the included modules were not documented correctly and will need to be back annotated. This is a back burner project and is only done when needed. Please feel free to help us write use cases and scenario.

---

## Development

```
     ____          __                  
    /  /::\        /  /\          __    
   /  /:/\:\      /  /:/_        /__/\   
  /  /:/  \:\    /  /:/ /\       \  \:\  
 /__/:/ \__\:|  /  /:/ /:/_       \  \:\
 \  \:\ /  /:/ /__/:/ /:/ /\  __  \__\:\
  \  \:\  /:/  \  \:\/:/ /:/ /__/\ |  |:|
   \  \:\/:/    \  \::/ /:/  \  \:\|  |:|
    \  \::/      \  \:\/:/    \  \:\__|:|
     \__\/        \  \::/      \__\::::/
                   \__\/           ~~~~  
```

If you would like to contribute as a developer, way to follow along with development is by viewing the issue commits. Each commit should have a short issue description and #issue_number (@see git commit log). By clicking on the #issue_number it will take you to the issue where the work should be described with some level of detail.

## Vision

The vision of KT is to create consumer electronics, distributed supercomputers, games, servers, and touchscreen plugin widgets to interfacing with apps, games,  websites, and electronics. The vision is to  provide an ultra-fast memory-lean scalable software model from firmware to desktop to server that is textbook `Modern-C++` and to provide tools to integrate multiple APIs into a single SDK with a single style guide.

### North Star Direction

The North Star is the symbolic representation of the project direction intended
to keep the project on track. The North Star is currently pointed towards:

* Completing `kabuki::toolkit::hmi::Unicontroller` and `kabuki::tek::Unicontroller`.
* Creating a GUI library from the hmi library and [Skia](https://skia.org/).

## Third-party Dependencies

One goal of the project is the use a Sloth Script to download all of the third-party APIs and format them to Google C++ Style Guide. This is a harder tasks than it sounds, but Clang has some tools we can use.

## clang-format

In order to prevent valuable wasted programming hours on formatting, it is required that you run clang-format when you save each C++ source file. For  Visual Studio users you will need to install Clang PowerTools and clang-format extensions and set them up to auto-format on save. We're trying to  get a clang-refactor script going to convert all of the code to Google C++  Style Guide.

## Seam Enumeration

The development is enumerated into major and minor seams that get tested in  order from SEAM_01_01_01 through SEAM_P_M_m, where P, M and m are the number of  Page, Major and minor  seams respectively.

A Minor Seam is defined as a collection of one or more classes, and a Major Seam is defined a s collection of one or more namespaces or C modules. Seams  should contain the fewest number of seams as possible, and the seams should as  decoupled as possible. For details about what each seams is and does, please  see the [GitHub Projects](https://github.com/kabuki-starship/kabuki-toolkit.git/projects).

## Release Instructions

There are only three steps to releasing the issue-build for Kabuki Toolkit.

1. Create a pull request for the website.
2. Build toolkit with passing unit test into the `kabuki-starship.github.io/release/head` folder.
3. Compile Doxygen API docs to website.
4. Check in pull request.

# The License

Copyright 2014-9 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
