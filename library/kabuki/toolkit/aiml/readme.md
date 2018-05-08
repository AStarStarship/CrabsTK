# kabuki::aiml

Module contains Artificial Intelligence and Machine Learning building blocks.

## Development

### Localality of Polymorphoism

Polymohpism intails adding (almost always) a 32-bit vtable pointer, which can 
either be problematic or something you may have in there without polymorphism. 
These vtable pointers may interfere with Locality of Reference, thus I have 
nammed it Locality of Polymorhpism, or they may increase locality of reference 
by tightly packing up all of your funciton pointers into ROM for you.

#### Banned Uses

* Tasks like calculating vectorized data such as summing perceptron inputs.
* When polymorphic overhead occupies more than about 1% of RAM or ROM and 
  other methods are available.

#### Welcomed Uses

* Embedded Systems.
* When vtable overhead is negligable.

## Author

* [Cale McCollough](https://calemccollough.github.io)

## License
Copyright 2017-8 (C) [Cale McCollough](mailto:calemccollough@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
