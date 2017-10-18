# Kabuki Starship Documentation
The Doxyfile is used to create the [Kabuki Starship API Docs]
(https://kabuki.github.io/api/). You may build these 
yourself for offline viewing. The UML diagrams can be found in the 
"Kabuki Toolkit Diagrams.mdj" file.

# Frequently Asked Questions

* Where should I start?
     It is best to start with the [Example projects]
     (https://github.com/Kabuki-Toolkit/kabuki/tree/master/Examples).
     Just find a project that is kind of like what you want to do and modify it.

* How do I get started as a developer?
   The best way to get started as a developer is to go through and clean up the
   code. This SDK was created from a hodgepodge of various open-source
   commercial-friendly BSD-style licensed software. There is still a lot of mess
   to clean up from the integration process. Whatever questions you have other
   people will no doubt ask as well. Please add these questions to this FAQ and
   to relevant example projects and API docs.

* What is the documentation and documentation process?
    This SDK uses [Doxygen](http://www.stack.nl/~dimitri/doxygen/). The project
    is currently setup using D:\Workpace. API docs get exported to the [Kabuki
    Theater website].

* Why does the website look all messed up?
    The [Kabuki Toolkit website](https://kabuki.github.io/) needs to get
    fixed ASAP. The CSS is messed up in full-screen mode, but works right in less
    than about 1024 pixel widths. It's something about the @media section in the
    CSS.

* What is the style guide?
    All of the Kabuki Toolkit projects use the [Google C++ Style Guide]
    (https://google.github.io/styleguide/cppguide.html). The style guide helps
    developers to identify the purpose of the code based on it's formatting.
    Most of the code is not in this format, so we could really use some helps
    formatting the style. We need to export a Visual Studio C++ code format file
    and could use some help with that.

* Where is the \_video code?
    We're shooting for deep integration into [OpenCV](opencv.org). This process
    has not yet begun. Please feel free to help us with this. Please note that
    this SDK is required to be able to work with a C++ DLL for cross-platform
    reasons. All code should have hardware and OS agnostic interface controllers
    so developers can write the code once and it works on all platforms.

* Is there any UML?
    We're using [StarUML 2.x](http://staruml.io/). The models can be found in
    the file "~/Documents/Kabuki Starship.mdj". Most of the included modules were
    not documented correctly and will need to be back annotated. This is a back
    burner project and is only done when needed. Please feel free to help us
    write use cases and scenario.
