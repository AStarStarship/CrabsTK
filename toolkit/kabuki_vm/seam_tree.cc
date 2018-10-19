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

#include <kabuki/f2/seam_0_0_0_f2.h>
/*
#include <kabuki/hal/seam_0_0_1_hal.h>

#include <kabuki/drivers/seam_0_0_2_drivers.h>

#include <kabuki/platform/seam_0_0_3_platform.h>

#include <kabuki/features/filecabinent/seam_0_0_4_filecabinent.h>

#include <kabuki/features/crabs/seam_0_0_5_crabs.h>

#include <kabuki/features/tls/seam_0_0_6_tls.h>

#include <kabuki/osal/seam_0_0_7_osal.h>

#include <kabuki/f2sdk/data/seam_0_0_8_data.h>

#include <kabuki/f2sdk/who/seam_0_0_9_who.h>

#include <kabuki/f2sdk/pro/seam_0_0_10_pro.h>

#include <kabuki/f2sdk/uweb/seam_0_0_11_uweb.h>

#include <kabuki/f2sdk/orm/seam_0_0_12_orm.h>

#include <kabuki/f2sdk/ai/seam_0_0_13_ai_math.h>

#include <kabuki/features/hmi/seam_0_1_0_hmi.h>

#include <kabuki/features/audio/seam_0_2_0_audio.h>

#include <kabuki/f2sdk/adsp/seam_0_2_1_adsp.h>

#include <kabuki/features/midi/seam_0_2_2_midi.h>

#include <kabuki/f2sdk/music/seam_0_2_3_music.h>

#include <kabuki/f2sdk/ai/seam_0_2_4_ai_math.h>

#include <kabuki/features/lights/seam_0_3_0_lights.h>

#include <kabuki/features/graphics/seam_0_3_1_graphics.h>

#include <kabuki/f2sdk/drawing/seam_0_3_2_drawing.h>

#include <kabuki/f2sdk/gui/seam_0_3_3_gui.h>

#include <kabuki/f2sdk/plotting/seam_0_3_4_plotting.h>

#include <kabuki/features/camera/seam_0_3_5_camera.h>

#include <kabuki/features/webcam/seam_0_4_0_webcam.h>

#include <kabuki/f2sdk/video/seam_0_4_1_video.h>

#include <kabuki/f2sdk/ai/seam_0_4_2_ai_math.h>
*/
using namespace _;

int main(int arg_count, char** args) {
  enum { kSize = 1024 };
  char seam_log[kSize];
  const char* result = ArgsToString(arg_count, args);
  result = _0_0_0_F2(seam_log, seam_log + kSize - 1, result);
  return result == nullptr ? APP_EXIT_SUCCESS ? APP_EXIT_FAILURE;
  /*
  return _::SeamTreeTest<Seam_0_0_0_F2
                         // Seam_0_0_1_HAL,
                         // Seam_0_0_2_Drivers,
                         // Seam_0_0_3_Platform,
                         // Seam_0_0_4_0_Crabs,
                         // Seam_0_0_4_1_FileCabinet,
                         // Seam_0_0_4_2_TLS,
                         // Seam_0_0_5_OSAL,
                         // Seam_0_0_6_Data,
                         // Seam_0_0_7_Who,
                         // Seam_0_0_8_Pro,
                         // Seam_0_0_9_uWeb,
                         // Seam_0_0_10_ORM,
                         // Seam_0_0_11_AI_Math,
                         // Seam_0_1_0_HMI,
                         // Seam_0_2_3_Audio,
                         // Seam_0_2_3_ADSP,
                         // Seam_0_2_3_MIDI,
                         // Seam_0_2_3_Music,
                         // Seam_0_2_5_AI_Math,
                         // Seam_0_3_0_Lights,
                         // Seam_0_3_1_Graphics,
                         // Seam_0_3_2_GUI,
                         // Seam_0_3_3_Plotting,
                         // Seam_0_3_5_Camera,
                         // Seam_0_4_0_Webcam,
                         // Seam_0_4_1_Video,
                         // Seam_0_4_2_AI_Math
                         >(seam_log, kSeamLogSize, args);*/
}
