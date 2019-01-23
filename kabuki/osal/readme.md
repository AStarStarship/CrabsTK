# kabuki.toolkit.drawing

There some code in here at one point but the requirements for the gl is very strictly regulated to Cairo and ANGLE now. The future GUI toolkit will be mapping the kabuki.features.hmi (Human-machine Interface) library which is good for easy mapping of controls over a serial line or the IoT. It provides the ability to multiplex pages of controls on buttons that use touchscreen widgets on PC or mobile device.

# Development

The process is:

* Use most popular cairo GUI toolkit, strip it's internals out, and map them to the kabuki.features.hmi library.

The work to be done is:

* Get Cairo working with SDL2 in a DLL with ANGLE.
* I made a primitive wire frame 3D model viewer demo a long time ago that is good for testing the gl.
* The math library is coupled to the gl, pl (Plotting Library) and the AI libraries on this seam.
* 

# The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
