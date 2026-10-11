# kabuki.drivers

Kabuki Toolkit breaks up boost::asio into two portions, based on the organization of mbed, a hardware abstraction layer and drivers to work on the Hardware Abstraction Layer (HAL).

# Development Strategy

1. Convert boost::asio to kabuki.hal and kabuki.drivers half by hand and half using projects.kabuki.sloth.
1. Repeat the prior step using a fully-automated #Sloth script.
1. Convert boost::filesystem to kabuki.features.filecabinet half by hand and half using projects.kabuki.sloth.
1. Repeat the prior step using a fully-automated #Sloth script.

# The License

Copyright 2014-19 (C) Cale Jamison McCollough <<cale@astartup.net>>. All rights reserved (R).

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0). Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
