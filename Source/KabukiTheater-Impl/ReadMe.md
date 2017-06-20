# Kabuki Theater Impementation
This folder contains the compiled-library-side implementation of Kabuki Theater.

# Development

## Todo List
1. Each folder in the KabukiToolkit-Impl folder needs to get a TESTS directory, and that directory shall contain all of the unit tests. There are some unit tests still in the .cpp impl files. They need to get copied into a file with a name like "ExampleClassTests.hpp"
1. Each module folder should have a ReadMe.md file in it. These files should get updated with useful info about the module or subfolder. Please feel free to add info that is not clear to you.
1. Every class has a sub for an inline void print (I2P::Terminal& slot) function. They need to get detailed.
1. Everything needs to have a C interface for wrapping into C#/Python/etc.
1. Demo apps need to work.
1. Documentation needs to be updated/written. What are you confused about?
1. Doxygen API References need to be updated with better info.
1. This software was originally written in Java and C#, and there is some some stuff that needs to get fixed; please inspect the destructors.
1. There are still some unit tests written in JUCE (www.juce.com) in the _UI modeule, and those need to get ported ASAP.

## Author
* [Cale McCollough](https://calemccollough.github.io)
* [Contributors please add your names here](mailto::your@email.address)

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
