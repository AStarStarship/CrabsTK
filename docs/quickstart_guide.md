```      
   ___        _      _        _             _      ____       _     _      
  / _ \ _   _(_) ___| | _____| |_ __ _ _ __| |_   / ___|_   _(_) __| | ___ 
 | | | | | | | |/ __| |/ / __| __/ _` | '__| __| | |  _| | | | |/ _` |/ _ \
 | |_| | |_| | | (__|   <\__ \ || (_| | |  | |_  | |_| | |_| | | (_| |  __/
  \__\_\\__,_|_|\___|_|\_\___/\__\__,_|_|   \__|  \____|\__,_|_|\__,_|\___|
```

***This Quickstart is broken because I've integrated most of the stuff into one SDK. Just look at the seam_0.***

# Visual Studio Setup
Right now the Visual Studio projects are mapped to "C:\Workspace" at the moment. Please feel free to remap the include and library directories to relative paths. It's a todo.

## Step 1
Download and install [Visual Studio 2017](https://www.visualstudio.com/) with all of the C++ tools, and create folder "C:\workspace".

## Step 2

Download and install Git at https://git-scm.com/, and add the path to `git.exe` into your OS console.

## Step 3

```
git clone https://github.com/google/angle.git
```

## Step 4

Loop through the ~/seam_0 directory for a project that could work for a bootstrap for your project and copy the folder into yoru workspace and modify the "assembly.h" and `pch.h` to suit your needs.

# CMake Setup

## Step 1

Download CMake Executables (Building CMake is outside of the scope of this Guide) and add CC:\Program Files\CMake\bin to the Environment Path Variables.

## Step 2

Open a console in the ~/kabuki folder and build the Kabuki Toolkit Library with the command:

```
cmake ./
```

# 2. Development

What ever questions you have other people no doubt will also have. Please see and modify the [FAQ](https://github.com/kabuki-project/kabuki/tree/master/docs/faq).

### I. Plugin Development Process

To develop plugins with Kabuki Toolkit it is recommended to start out by developing your application as a standalone application that links to a statically compiled version of Kabuki Toolkit. This will dramatically improve compile time as it eliminates a lot of redundant compiling.

# Author

* [Cale McCollough](https://calemccollough.github.io) <[cale.mccollough@gmail.com](mailto:cale.mccollough@gmail.com)>

## License

Copyright 2017-8 (C) [Cale McCollough](mailto:calemccollough@gmail.com) and contributors. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [here](http://www.apache.org/licenses/LICENSE-2.0).

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
