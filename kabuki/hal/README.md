```
  _   _      _       _      
 |'| |'| U  /"\  u  |"|     
/| |_| |\ \/ _ \/ U | | u   
U|  _  |u / ___ \  \| |/__  
 |_| |_| /_/   \_\  |_____|
 //   \\  \\    >>  //  \\  
(_") ("_)(__)  (__)(_")("_)  
```
# Kabuki VM Hardware Abstraction Layer

The Kabuki VM Hardware Abstraction Layer (HAL) is based on mbed, an IoT platform similar to Arduino for ARM, adapted to work on x64 processors. Starting from the lowest going to highest levels the major components are:

1. **Hardware Abstraction Layer (HAL)** - Abstracts away the hardware to create a low-level virtual machine.
   1. Abstract hardware model for common processor peripherals.
   2. Hardware agnostic code shared between targets processors.
   3. Target implementations for various processors.
2. **Platform** - Platform such as an operating system or bare-metal firmware that creates a higher-level virtual machine.
   1. Platform abstraction layer for common platform features.
   2. Platform agnostic code shared between platforms.
   3. Platform implementation.
3. **Features** - Cross-platform features.
   1. Feature model with common interface for features.
   2. Feature framework shares cross-platform code between features.
   3. Feature implementation.
4. **Toolkit Modules** - Cross-platform toolkit built on top of the features.
   1. Module model.
   3. Crabs layer.
   3. Module implementation.

## License

Copyright 2014-18 (C) [Cale McCollough](mailto:calemccollough@gmail.com). All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
