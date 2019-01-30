# Development

```AsciiArt
     ____           __
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

If you would like to contribute as a developer, way to follow along with development is by viewing the issue commits. Each commit should have a short issue description and #issue_number (@see git commit log). By clicking on the #issue_number it will take you to the issue where the work should be described with some level of detail.

## clang-format

In order to prevent valuable wasted programming hours on formatting, it is required that you run clang-format when you save each C++ source file. For Visual Studio users you will need to install Clang PowerTools or clang-format extensions and set them up to auto-format on save. We're trying to  get a clang-refactor script going to convert all of the code to Google C++  Style Guide.

## Seam Tree and Seam Graph Crash Coarse

Seam Tress and graphs are an advanced test and mocking technique. There is a chapter about it in the book `Script^2 and Kabuki Toolkit` but was rapidly evolving over the past year and a half so I'm just doing to give you a one paragraph overview and tell you to ask me questions on Slack:

Seam Tree indexes 1 through N where N is the number of seams, are macros that turn on and off C++ code, and allow us to enumberate work to be done using alphabetical order. Seams are useful for blocking out a lot of code and re-enabling code bit-by-bit to test it. The technique is a best-practice for organizing the order that your software layers stack up and allows you to peal back the layers and issolate paticular seams with quick-and-easy-to-write Seam Tree Tests.

The easiest way to learn this is to open up the `/kabuki-toolkit.sln` Visual Studio Solution starting with `0_0_00.script2` and start changing the `SEAM` index in the `/script2/pch.h` file. After you've checked out the Script2 seam tree tests then look at the other projects that start with numbers in `/kabuki-toolkit.sln`, and compare them to the `/projects/` directory. Each demo project has code that relies on different seam, such as a library with console seam tree test that then gets graphics and a GUI layered over top of it. Seam trees are carefully organized to avoid having to perform unneccissary typing to create and modify the seams.

Graphs can be created from seam nodes, but trees are required for testing in-order. If a seam is spread across multiple seams, one seam must be designated as a Parent seam, and the the rest must be child seams, such as the `kabuki.features.math library`. Seam tree indexes are positive integers and code can enabled or disabled in a range of values:

```C++
#define KABUKI_TOOLKIT_AI_1 123
#define KABUKI_TOOLKIT_MATH_1 124
#define KABUKI_TOOLKIT_AI_2 125
#define KABUKI_TOOLKIT_MATH_3 126
#define KABUKI_TOOLKIT_AI_3 127
#define KABUKI_TOOLKIT_MATH_4 128

#deine SEAM KABUKI_TOOLKIT_AI_1

#if SEAM >= KABUKI_TOOLKIT_AI_1
void Foo () {}
#endif

#if SEAM >= KABUKI_TOOLKIT_AI_2 && SEAM <= KABUKI_TOOLKIT_AI_3
void Bar () {}
#endif
```

It's best to think of a child node a slice of that seam, and in which the slices for another sparce seam index list (i.e. non-contiguous seam indexes).

```C++
#if SEAM >= KABUKI_TOOLKIT_MATH_1 && SEAM <= KABUKI_TOOLKIT_MATH_4
void FooBar () {}
#endif
```

Each application created has it's own Seam Tree. Script2 is always going to be `0_0_0` or `0_0_00`. The purpose of the extra zero is to alphabetically sort the seams into enumberated order. The seams are set up to minimze typing. You should be able to instantly understand by comparing the list of seams in the `/README.md` to the `/docs/kabuki_vm_package_diagram.jpg`.

## Seam Trees Commit Logs and Pull Requests

KT uses a practice called Issue-driven Development (IDD) applied to Seam Trees and Seam Graphs. IDD is a practice of only completing one issue at a time and only doing work after or right around when you create an Issue-ticket, and tying of issue tickets to the commit log by copying and pasting the issue ticket headline inlcuding the #TicketNumber (i.e. #1, #2, ...). Please **@see** the KT commit log to examples. The benifit of IDD is that it creates institutionalized knowedge from the clean commit logs and comments.

### Steps

**1.** Fork KT on Github and create an Issue Ticket in the main repo similar to the following:

**Issue Title:** 0_1_02.3:Add feature XYZ to 

**2.** Create a branch:

```Console
git checkout -b Issue123
```

**3.** Bring the issue to completion with a passing unit test then commit it to your fork:

```Console
git pull origin master
git add --all
git push origin Issue123
```

**4.** Create a Pull Request.

### Updating the API Reference

**1.** Clone the Kabuki Starship website:

```Console
git clone https://github.com/kabuki-starship/kabuki-starship.github.io.git
```

**2.** Create an issue ticket for to update the API docs:

**Issue Ticket Title:** Update API Reference for kabuki-toolkit.

**Issue Ticket Body:** *Blank*

**3.** Create a branch for the issue.

```Console
git checkout -b Issue123
```

**4.** Open the `/docs/Doxyfile` configuration file in Doxygen and export to the `kabuki-starship.github.io/kabuki-toolkit/api` folder.

**5.** Push the changes out to your repo.

```Console
git pull origin master
git add --all
git push origin Issue123
```

**6.** Create a pull request on GithHub.

## The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
