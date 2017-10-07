# Overview
Kabuki Toolkit currently only has projects for Visual Studio because I deleted all the CMake files like an idiot.

## Kabuki Toolkit Modules
| Name    | Description  |
|:-------:|:-------------|
| _       | Chinese Room.|
| al      | Audio Library.|
| app     | Application framework.|
| biz     | Business eCommerce.|
| code    | Coding utilities.|
| data    | Data and data logging.|
| gl      | Graphics Library.|
| gui     | Graphical user interfaces.|
| hmi     | Human-machine Interfaces.|
| json    | JSON.|
| id      | Identification.|
| math    | Math.|
| midi    | MIDI.|
| tek     | Firmware toolkit.|
| pplx    | Parallel Patterns Library.|
| pro     | Projects, tasks, and scheduling.|
| process | Threads and interprocess IO.|
| serial  | Serial IO.|
| utils   | Utilities.|
| video   | Video.|
| web     | WWW and IoT.|

## Setup
Right now the Visual Studio projects are mapped to "D:\Workspace" at the moment. Please feel free to remap the include and library directories to relative paths. It's a todo.

### Step 1
Download and install [Visual Studio 2017](https://www.visualstudio.com/) with all of the C++ tools, and create folder "D:\Workspace".

### Step 2
```
cd d:\workspace
git clone https://github.com/cpputest/cpputest.git
```

### Step 2B
You might need to recompile CppUTest if your compiler complains. Open up the solution file in the cpputest root director and build it or run the make file.

### Step 3
```
git clone https://github.com/google/angle.git
```

### Step 4
Add the "/.../Workspace/cpputest/include", and "/.../Workspace/angle/include" to your project's "Additional include directories" and add "/.../cpputest/lib/cpputest.lib" and "Winmm.lib" to the linker Additional Dependencies.

### Step 5
Copy "/.../Source/chinese_room/include/assembly.h" into your source code root for your project and modify the assembly settings to suit your needs using the instructions in the file.

# Step 6
Once you've gotten your project setup, please check out the [Example projects](https://github.com/kabuki-project/kabuki-toolkit/tree/master/projects).

## Development
Whatever questions you have other people no doubt will also have. Please see and modify the [[FAQ]].

## Plugin Development Process
To develop plugins with Kabuki Toolkit it is recommended to start out by developing your application as a standalone application that links to a statically compiled version of Kabuki Toolkit. This will dramaticaly improve compile time as it eliminates a lot of redundant compiling.
