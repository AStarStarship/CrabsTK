# Kabuki Theater Source Root
Modules names start with underscores in order to reduce typing. You may delete unused modules if that does not break your dependencies.

## Project Organization
The primary development environment is Visual Studio using [Symmetry Station](https://github.com/Kabuki-Toolkit/SymmetryStation). Please see the [Symmetry Station Quick Start Guide](https://github.com/Kabuki-Toolkit/SymmetryStation/wiki/Quick-Start-Guide). Each folder in the project should have ReadMe.md file in it that should tell you what you need to know. Source header files are separated from their precompiled .lib and .dll files. Development instructions are in the ReadMe.md files in the /.../Source/KabukiTheater-Impl/ folder and subfolders. If you choose to not use Symmetry Station, you may need to update the projects libaries to the most recent updates that have not been synced across projects yet.

## SDK Modules
| Name     | Description |
|:--------:|:------------|
| _Audio   | Audio.|
| _App     | Application framework.|
| _File    | Files and compressed.|
| _G       | Graphics engine.|
| _GUI     | Graphical User Interfaces.|
| _Id      | Identification.|
| _IO      | Input/Ouput: Audio and Serial.|
| _Math    | Math and geometry.|
| _Net     | Networking.|
| _Theater | Music, lighting, and creativity.|

## License ##
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

