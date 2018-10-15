/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/seam_tree.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */
#pragma once
#include <pch.h>

#include <kabuki/f2/seam_f2.h>

using namespace _;

int main(int arg_count, char** args) {
  enum { kSeamLogSize = 1024 };
  char seam_log[kSeamLogSize];
  return TestTree<Kabuki__Terminal___Kabuki__F2,                    //< 0_0_0
                  Kabuki__Terminal___Kabuki__HAL,                   //< 0_0_1
                  Kabuki__Terminal___Kabuki__Drivers,               //< 0_0_2
                  Kabuki__Terminal___Kabuki__Platform,              //< 0_0_3
                  Kabuki__Terminal___Kabuki__Features__Crabs,       //< 0_0_4
                  Kabuki__Terminal___Kabuki__Features__HMI,         //< 0_0_5
                  Kabuki__Terminal___Kabuki__Features__Filesystem,  //< 0_0_6
                  Kabuki__Terminal___Kabuki__OSAL,                  //< 0_0_7
                  Kabuki__Terminal___Kabuki__F2SDK__Data,           //< 0_0_8
                  Kabuki__Terminal___Kabuki__F2SDK__Who,            //< 0_0_9
                  Kabuki__Terminal___Kabuki__F2SDK__Pro,            //< 0_0_10
                  Kabuki__Terminal___Kabuki__F2SDK__AI_Math,        //< 0_0_11
                  Kabuki__Terminal___Kabuki__F2SDK__uWeb,           //< 0_0_12
                  Kabuki__Terminal___Kabuki__F2SDK__ORM,            //< 0_0_13
                  Kabuki__Sound___Kabuki__F2SDK__Audio,             //< 0_1_3
                  Kabuki__Sound___Kabuki__F2SDK__ADSP,              //< 0_1_3
                  Kabuki__Sound___Kabuki__F2SDK__MIDI,              //< 0_1_3
                  Kabuki__Sound___Kabuki__F2SDK__Music,             //< 0_1_3
                  Kabuki__Sound___Kabuki__F2SDK__AI_Math,           //< 0_2_5
                  Kabuki__Sight___Kabuki__F2SDK__Lights,            //< 0_2_0
                  Kabuki__Sight___Kabuki__F2SDK__Graphics,          //< 0_2_1
                  Kabuki__Sight___Kabuki__F2SDK__GUI,               //< 0_2_2
                  Kabuki__Sight___Kabuki__F2SDK__Plotting,          //< 0_2_3
                  Kabuki__Sight___Kabuki__F2SDK__Videos,            //< 0_2_4
                  Kabuki__Sight___Kabuki__F2SDK__Camera,            //< 0_2_5
                  Kabuki__Sight___Kabuki__F2SDK__Webcam,            //< 0_2_6
                  Kabuki__Sight___Kabuki__F2SDK__AI_Math            //< 0_2_7
                  >(seam_log, kSeamLogSize, args);
}
