```
 /\
( /   @ @    ()
 \  __| |__  /      ██████╗██████╗  █████╗ ██████╗ ███████╗
  -/   "   \-      ██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔════╝
 /-|       |-\     ██║     ██████╔╝███████║██████╔╝███████╗
/ /-\     /-\ \    ██║     ██╔══██╗██╔══██║██╔══██╗╚════██║
 / /-`---'-\ \     ╚██████╗██║  ██║██║  ██║██████╔╝███████║
  /         \       ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝
```

# Chinese Room Abstract Stack Machine (Crabs) API Overview

The easiest way to get up to speed with how Crabs works is by learning about how the minor seams are organized.

## Minor Seams

### _0_0_0

Seam develops the Script Integer-to-ASCII (**itoa**) algorithm, the fastest integer to string algorithm in the world along with corresponding string-to-integer functions.

#### Seam Modules

* `kabuki`
  * `crabs`

### _0_0_1

Seam contains 

#### Seam Modules

* `kabuki`
  * `crabs`

### _0_0_2

Develops the Universal Text Formatter (UTF), which contains all of the essential tools to print and scan plain-old-data types in UTF-8, UTF-16, and UTF-32, the end-product of the seam is the Clock utilities for printing and scanning timestamps.

#### Seam Modules

* `kabuki`
  * `crabs`

### _0_0_3

Develops the ASCII Data Types.

#### Seam Modules

* `kabuki`
  * `crabs`


### _0_0_4

Develops the BIn, BOut, Slot, Expr,

#### Seam Modules

* `kabuki`
  * `crabs`

### SEAM 0_0_5

Seam develops the Hardware Abstraction Layer.

#### Seam Modules

* `kabuki`
  * `crabs`
  * `drivers`
  * `features`
  * `hal`
  * `platform`


## Author

* [Cale McCollough](https://calemccollough.github.io)

## License

Copyright 2017-8 (C) [Cale McCollough](mailto:calemccollough@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at http://www.apache.org/licenses/LICENSE-2.0. Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
