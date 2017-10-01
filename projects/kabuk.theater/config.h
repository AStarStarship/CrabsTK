/** Kabuki Theater
    @version 0.9
    @file    /.../KabukiTheater/KabukitToolkit-Config.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#ifndef KABUKI_THEATER_CONFIG_H
#define KABUKI_THEATER_CONFIG_H

#include <_/config.h>

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
using std::vector;
using std::string;
using std::map;

typedef unsigned int uid_t;

#if ASSEMBLY_TYPE == SINGLE_DLL
#ifdef EXPORT_KABUKI_THEATER
#define KABUKI_THEATER __declspec (dllexport) 
#else
#define KABUKI_THEATER __declspec (dllimport) 
#endif
#else
#define AL_API
#define AUDIO_API
#define DATA_API
#define DMX_API
#define FILE_API
#define GL_API
#define HMI_API
#define ID_API
#define MATH_API
#define MIDI_API
#define PLAY_API
#define PRO_API
#define SERIAL_API
#define SHOP_API
#define TEK_API
#define VGUI_API
#define VIDEO_API
#define WEB_API
#endif

/*
#if ASSEMBLY_TYPE == DYNAMICALLY_LINKED_LIBRARY
    #if defined (_WIN32) || defined (__WIN32__)
        #define DLL_CALLCONV __stdcall
        // The following ifdef block is the standard way of creating macros which make exporting 
        // from a DLL simpler. All files within this DLL are compiled with the KABUKI_THEATER_EXPORTS
        // symbol defined on the command line. this symbol should not be defined on any project
        // that uses this DLL. This way any other project whose source files include this file see 
        // __KabukiTheater functions as being imported from a DLL, wheras this DLL sees symbols
        // defined with this macro as being exported.
        #ifdef KABUKI_THEATER_EXPORTS
            #define __KabukiTheater __declspec (dllexport)
        #else
            #define __KabukiTheater __declspec (dllimport)
        #endif // KABUKI_THEATER_EXPORTS
    #else 
        // try the gcc visibility support (see http://gcc.gnu.org/wiki/Visibility)
        #if defined (__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
            #ifndef GCC_HASCLASSVISIBILITY
                #define GCC_HASCLASSVISIBILITY
            #endif
        #endif // __GNUC__
        #define DLL_CALLCONV
        #if defined (GCC_HASCLASSVISIBILITY)
            #define __KabukiTheater __attribute__ ((visibility("default")))
        #else
            #define __KabukiTheater
        #endif		
    #endif // WIN32 / !WIN32
#else
    #define KABUKI_THEATER
    #define DLL_CALLCONV
#endif // __KabukiTheaterLib
*/

#endif  //< KABUKI_THEATER_CONFIG_H
