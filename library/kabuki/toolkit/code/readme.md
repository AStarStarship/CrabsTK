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
