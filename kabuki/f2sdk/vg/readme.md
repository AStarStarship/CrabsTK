# Vector Graphics Library

There some code in here at one point but the requirements for the gl is very strictly regulated to Cairo and ANGLE now. The future GUI toolkit will be mapping the kabuki.features.hmi (Human-machine Interface) library which is good for easy mapping of controls over a serial line or the IoT. It provides the ability to multiplex pages of controls on buttons that use touchscreen widgets on PC or mobile device.

# Development

The process is:

* Use most popular cairo GUI toolkit, strip it's internals out, and map them to the kabuki.features.hmi library.

The work to be done is:

* Get Cairo working with SDL2 in a DLL with ANGLE.
* I made a primitive wire frame 3D model viewer demo a long time ago that is good for testing the gl.
* The math library is coupled to the gl, pl (Plotting Library) and the AI libraries on this seam.
* 
