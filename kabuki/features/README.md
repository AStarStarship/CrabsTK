```
  _____ U _____ u    _       _____    _   _    ____    U _____ u ____     
 |" ___|\| ___"|/U  /"\  u  |_ " _|U |"|u| |U |  _"\ u \| ___"|// __"| u  
U| |_  u |  _|"   \/ _ \/     | |   \| |\| | \| |_) |/  |  _|" <\___ \/   
\|  _|/  | |___   / ___ \    /| |\   | |_| |  |  _ <    | |___  u___) |   
 |_|     |_____| /_/   \_\  u |_|U  <<\___/   |_| \_\   |_____| |____/>>  
 )(\\,-  <<   >>  \\    >>  _// \\_(__) )(    //   \\_  <<   >>  )(  (__)
(__)(_/ (__) (__)(__)  (__)(__) (__)   (__)  (__)  (__)(__) (__)(__)      
```
# Kabuki VM Platform

The Kabuki VM Features are based on mbed, an IoT platform similar to Arduino for ARM, adapted to work on x64 processors. Starting from the lowest going to highest levels the major components are:

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
