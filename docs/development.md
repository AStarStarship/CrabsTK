```
     ____          __                  
    /  /::\        /  /\          __    
   /  /:/\:\      /  /:/_        /__/\   
  /  /:/  \:\    /  /:/ /\       \  \:\  
 /__/:/ \__\:|  /  /:/ /:/_       \  \:\
 \  \:\ /  /:/ /__/:/ /:/ /\  __  \__\:\
  \  \:\  /:/  \  \:\/:/ /:/ /__/\ |  |:|
   \  \:\/:/    \  \::/ /:/  \  \:\|  |:|
    \  \::/      \  \:\/:/    \  \:\__|:|
     \__\/        \  \::/      \__\::::/
                   \__\/           ~~~~  
```

﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿# Development

If you would like to contribute as a developer, way to follow along with development is by viewing the issue commits. Each commit should have a short issue description and #issue_number (@see git commit log). By clicking on the #issue_number it will take you to the issue where the work should be described with some level of detail.

## Vision

The vision of KT is to create consumer electronics, distributed supercomputers, games, servers, and touchscreen plugin widgets to interfacing with apps, games,  websites, and electronics. The vision is to  provide an ultra-fast memory-lean scalable software model from firmware to desktop to server that is textbook `Modern-C++` and to provide tools to integrate multiple APIs into a single SDK with a single style guide.

### North Star Direction

The North Star is the symbolic representation of the project direction intended
to keep the project on track. The North Star is currently pointed towards:

* Completing `kabuki::toolkit::hmi::Unicontroller` and `kabuki::tek::Unicontroller`.
* Creating a GUI library from the hmi library and [Skia](https://skia.org/).

## Third-party Dependencies

One goal of the project is the use a Sloth Script to download all of the third-party APIs and format them to Google C++ Style Guide. This is a harder tasks than it sounds, but Clang has some tools we can use.

## clang-format

In order to prevent valuable wasted programming hours on formatting, it is required that you run clang-format when you save each C++ source file. For  Visual Studio users you will need to install Clang PowerTools and clang-format extensions and set them up to auto-format on save. We're trying to  get a clang-refactor script going to convert all of the code to Google C++  Style Guide.

## Seam Enumeration

The development is enumerated into major and minor seams that get tested in  order from SEAM_01_01_01 through SEAM_P_M_m, where P, M and m are the number of  Page, Major and minor  seams respectively.

A Minor Seam is defined as a collection of one or more classes, and a Major Seam is defined a s collection of one or more namespaces or C modules. Seams  should contain the fewest number of seams as possible, and the seams should as  decoupled as possible. For details about what each seams is and does, please  see the [GitHub Projects](https://github.com/kabuki-starship/kabuki-toolkit.git/projects).
