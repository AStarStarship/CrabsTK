﻿﻿﻿# 1 Overview

All of the Kabuki Toolkit project files are broken at the moment because I deleted all the CMake files like an idiot.

## I. Table of Content
1. Overview
    1. Table of Content
2. Setup
3. Development
    1. Plugin Development Process

# 1. Setup
Right now the Visual Studio projects are mapped to "C:\Workspace" at the moment. Please feel free to remap the include and library directories to relative paths. It's a todo.

## Step 1
Download and install [Visual Studio 2017](https://www.visualstudio.com/) with all of the C++ tools, and create folder "C:\Workspace".

## Step 2
```
cd d:\workspace
git clone https://github.com/cpputest/cpputest.git
```

## Step 2B
You will more than likely need to recompile CppUTest, but only if your compiler complains. Open up the solution file in the "C:/Workspace/cpputest" root director and build it or run the make file.

## Step 3
```
git clone https://github.com/google/angle.git
```

## Step 4
(Optional) Copy "~/source/kabuki/chinese_room/include/assembly.h" and "~/source/kabuki/chinese_room/include/this.h" into your source code root for your project and modify the assembly settings to suit your needs using the instructions in the file.

## Step 5
Download CMake Executables (Building CMake is outside of the scope of this Guide) and add CC:\Program Files\CMake\bin to the Environment Path Variables.

## Step 6
Open a console in the ~/source/kabuki folder and build the Kabuki Toolkit Library with the command:

```
cmake ./
```

## Step 7
Create a new C++ Visual Studio project in "C:/Workspace" and add "C:/cpputest/lib/cpputest.lib" and "Winmm.lib" to the Linker Additional Dependencies (@see Google "How to Add Linker Additional Dependencies in Visual Studio").

## Final Step
Once you've gotten your project setup check out the [Example projects](https://github.com/kabuki-project/kabuki/tree/master/projects) and copy the files you need into your new project and remap the #include files. Also check out the [CMake Tutorial](https://cmake.org/cmake-tutorial/) and [CMake and Visual Studio](https://cognitivewaves.wordpress.com/cmake-and-visual-studio/).

# 2. Development
What ever questions you have other people no doubt will also have. Please see and modify the [FAQ](https://github.com/kabuki-project/kabuki/tree/master/docs/faq).

## I. Plugin Development Process
To develop plugins with Kabuki Toolkit it is recommended to start out by developing your application as a standalone application that links to a statically compiled version of Kabuki Toolkit. This will dramatically improve compile time as it eliminates a lot of redundant compiling.

## Author

* [Cale McCollough](https://calemccollough.github.io) <[cale.mccollough@gmail.com](mailto:cale.mccollough@gmail.com)>

## License

Copyright 2017 (C) [Cale McCollough](mailto:calemccollough@gmail.com) and contributors. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [here](http://www.apache.org/licenses/LICENSE-2.0).

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

