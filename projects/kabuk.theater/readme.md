# Kabuki Theater Source Root
Source code is divided in between public facing headers in the 
"/.../Source/kabuki-theater" folder and implementation files in the 
"/.../Source/kabuki-theater-impl" folder. One caveat of C++ is that compiled 
libraries can not have dependencies on other libraries.

## SDK Modules
| Name     | Description |
|:--------:|:------------|
| _        | Chinese Room SDK core.|
| _al      | Open Audio Library.|
| _audio   | Audio.|
| _app     | Application framework.|
| _code    | Coding utilities.|
| _g       | Graphics.|
| _gui     | Raster graphical User Interfaces.|
| _hmi     | Human-machine Interfaces.|
| _id      | Identification.|
| _math    | Run-time and compile-time math and geometry.|
| _play    | Music, lighting, and creativity.|
| _pro     | Projects, tasks, and scheduling.|
| _vg      | Vector graphics.|
| _vgui    | Vector graphical user interfaces.|

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
