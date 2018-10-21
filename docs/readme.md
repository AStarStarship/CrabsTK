# Kabuki Toolkit Documentation

Please see `~/docs/kabuki-toolkit.mdj` for [StarUML](http://staruml.io) for object models. Some of the docs are currently in .docx format. These documents need to be converted into Markdown format as soon as possible and more code samples added. There is a productivity programming multimap in here too which is undergoing some changes at the moment. We're focusing on making money through saving time.

## Organization

Documents are split between the Wiki and the ~/docs folder.  Documents that are specific to Toolkit Versions are placed in the ~/docs folder and documents that are independent of version go in the Wiki.

## Docs

* [Quick Start Guide](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/quickstart-guide.md) - A short guide to get you up and running.
* [FAQ](https://github.com/kabuki-starship/kabuki-toolkit.git/blob/master/docs/faq.md) - Frequently asked questions
* [Script Specification RFC](https://github.com/kabuki-starship/script) - Serial Chinese Room, Interprocess, and Telemetry (SCRIPT) Specification Release for Comment.

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

﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿# Development

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


## Author

* [Cale McCollough](https://calemccollough.github.io)

## License

Copyright 2017-8 (C) [Cale McCollough](mailto:calemccollough@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
