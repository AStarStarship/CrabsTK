/** Kabuki Theater
    @version 0.x
    @file    ~/Source/Library/kabuki_theater/include.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef KABUKI_THEATER_INCLUDE_H
#define KABUKI_THEATER_INCLUDE_H

#ifdef  DEBUG
#undef  DEBUG
#define DEBUG 1     //< Set to 0 to turn off debug mode.
#endif

#include <__/include.h>
#include <_audio/include.h>
#include <_code/include.h>
#include <_data/include.h>
#include <_dmx/include.h>
#include <_file/include.h>
#include <_g/include.h>
#include <_hmi/include.h>
#include <_id/include.h>
#include <_math/include.h>
#include <_midi/include.h>
#include <_play/include.h>
#include <_pro/include.h>
#include <_serial/include.h>
#include <_shop/include.h>
#include <_tek/include.h>
#include <_vgui/include.h>
#include <_video/include.h>
#include <_web/include.h>

#endif  //< KABUKI_THEATER_INCLUDE_H
