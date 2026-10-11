# kabuki.osal

There some code in here at one point but the requirements for the gl is very strictly The Operating System abstraction layer has access to all of the kabuki.features, and provides the glue for the features to work on an operating system. This is a pre-Script2 module with little to no work on done on it.

The best example for the osal is the operation of the MIDI ports on Windows, OSX, Android, and iOS. In each system it's just a serial port, but the way they are named is slightly different. Each OS may support different features. The job of the osal is to glue the cross-platform code to the OS and hardware platform target.

# The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
