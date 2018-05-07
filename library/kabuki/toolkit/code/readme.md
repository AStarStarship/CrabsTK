# kabuki::toolkit::code API

API consists of coding utilities based on 
[Astyle](http://astyle.sourceforge.net/astyle.html). The Google C++ API is used 
to download all of the third-party libraries, then AStyle is used to format the 
libraries to the Google C++ Style Guide, followed by some custom utilities to 
collapse the folders and refactor the namespaces into the Kabuki Toolkit Module 
Format.

## Quick Links

* [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

google-astyle code base is google-astyle clean, meaning you should always run
google-astyle on top of google-astyle source code before committing.

The command line flags we are using are:

  `--style=linux --indent=spaces=2`

