# Overview
Kabuki Toolkit currently only has projects for Visual Studio, but will be adding Eclipse and NetBeans soon.

## Kabuki Toolkit Modules
| Module       | Description |
| chinese_room | Chinese Room SDK core.|
| app          | Application framework.|
| al           | Open Audio Library.|
| audio        | Audio utilities.|
| dmx          | DMX lighting.|
| file         | File IO.|
| gl           | Raster graphics library.|
| gui          | Graphical User Interface.|
| hmi          | Human-Machine Interface.|
| id           | Identification.|
| math         | Compile and run-time math.|
| midi         | MIDI adapters.|
| pro       
| serial       | UART devices.|
| tek          | Firmware toolkit.|
| web          | Networking and the WWW.|

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
Copy "/.../Source/\_/assembly_settings.hpp" into your source code root for your project and modify the assembly settings to suit your needs using the instructions in the file.

# Step 6
Once you've gotten your project setup, please check out the [Example projects](https://github.com/Kabuki-Toolkit/Kabuki-Theater/tree/master/Examples).

## Development
Whatever questions you have other people no doubt will also have. Please see and modify the [[FAQ]].

## SDK Organization
Kabuki Theater does not use pimps (though there are still some getting cleaned up). Platform specific harnesses are placed in the following tree structure:

```
+ kabuki
    + theater
        + android
           
```

## Plugin Development Process
To develop plugins with Kabuki Toolkit it is recommended to start out by developing your application as a standalone application that links to a statically compiled version of Kabuki Theater. This will dramaticaly improve compile time as it eliminates a lot of redundant compiling.
