# Kabuki Theater
A modern C++11 software development kit and app framework built on top of [Simple Fast Media Layer](http://www.sfml-dev.org/) and the [Chinese Room](https://github.com/Kabuki-Toolkit/ChineseRoom).

## Quick Links
* [Quickstart Guide](https://github.com/Kabuki-Toolkit/Kabuki-Theater/wiki/Quickstart-Guide.md) -
    Check here for the Kabuki Theater wiki and github repository!
* [Kabuki Toolkit Website](https://kabuki-toolkit.github.io/) - Official Kabuki Toolkit website. We are currently
    looking for someone to help us fix the CSS on the website. It only works right at < 1024 pixel width so the
    problem is in the @media section.

## Project Organization
The primary development environment is Visual Studio using [Symmetry Station](https://github.com/Kabuki-Toolkit/SymmetryStation). Please see the [Symmetry Station Quick Start Guide](https://github.com/Kabuki-Toolkit/SymmetryStation/wiki/Quick-Start-Guide). Each folder in the project should have ReadMe.md file in it that should tell you what you need to know. Source header files are separated from their precompiled .lib and .dll source implementation. Development instructions are in the ReadMe.md files in the /.../Source/KabukiTheater-Impl/ folder and subfolders. If you choose to not use Symmetry Station, you may need to update the projects libraries to the most recent updates that have not been synced across projects yet.

## Project Status: Alpha

## SDK Modules
| Name     | Description |
|:--------:|:------------|
| __       | System core, IO, and Networking.|
| _App     | Application framework.|
| _Audio   | Audio.|
| _File    | Files and archives.|
| _G       | Graphics engine.|
| _GUI     | Graphical User Interfaces.|
| _Id      | Identification.|
| _Math    | Math and geometry.|
| _Theater | Music, lighting, and creativity.|

## License
Copyright 2017 (C) [Cale McCollough](mailto:calemccollough@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
