# Kabuki Theater Headers
Kabuki Theater is seperated into pre-compiled and header only files. C++ does not allow libraries 
to include other precompiled libraries, so all dependencies on other modules can only use 
templated classes. The libary is divided into template-only libraries, and precompiled libraries. 
OpenGLES code may be used inside of a pre-compiled library, as they just gets translated into 
memory reads and writes. OpenAL is a library, so it can only go into an executable.
