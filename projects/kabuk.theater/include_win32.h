/** Kabuki Starship
    @version 0.x
    @file    ~/Source/Library/kabuki_theater/include_win32.h
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

#ifndef KABUKI_THEATER_INCLUDE_WIN32_H
#define KABUKI_THEATER_INCLUDE_WIN32_H

#ifdef  DEBUG
#undef  DEBUG
#define DEBUG 1     //< Set to 0 to turn off debug mode.
#endif

#include <app/include_win32.h>
#include <audio/include_win32.h>
#include <code/include_win32.h>
#include <data/include_win32.h>
#include <dmx/include_win32.h>
#include <file/include_win32.h>
#include <gl/include_win32.h>
#include <gui/include_win32.h>
#include <hmi/include_win32.h>
#include <id/include_win32.h>
#include <math/include_win32.h>
#include <midi/include_win32.h>
#include <play/include_win32.h>
#include <pro/include_win32.h>
#include <serial/include_win32.h>
#include <shop/include_win32.h>
#include <tek/include_win32.h>
#include <vgui/include_win32.h>
#include <video/include_win32.h>
#include <web/include_win32.h>

#endif  //< KABUKI_THEATER_INCLUDE_WIN32_H