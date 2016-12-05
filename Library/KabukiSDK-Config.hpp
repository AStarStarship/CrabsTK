/** Kabuki Software Development Kit
    @version 0.9
    @file    /.../Kabuki_Toolkit/KabukitToolkit-Config.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#pragma once

#include <FreeI2P.hpp>
using namespace I2P;

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
using namespace std;

#ifdef _ExportKabukiSDK
#define _KabukiSDK __declspec (dllexport) 
#else
#define _KabukiSDK __declspec (dllimport) 
#endif

/*
#if _UseDynamicLibrary
	#if defined (_WIN32) || defined (__WIN32__)
		#define DLL_CALLCONV __stdcall
		// The following ifdef block is the standard way of creating macros which make exporting 
		// from a DLL simpler. All files within this DLL are compiled with the _KabukiSDK_Exports
		// symbol defined on the command line. this symbol should not be defined on any project
		// that uses this DLL. This way any other project whose source files include this file see 
		// __KabukiSDK functions as being imported from a DLL, wheras this DLL sees symbols
		// defined with this macro as being exported.
		#ifdef _KabukiSDK_Exports
			#define __KabukiSDK __declspec (dllexport)
		#else
			#define __KabukiSDK __declspec (dllimport)
		#endif // _KabukiSDK_Exports
	#else 
		// try the gcc visibility support (see http://gcc.gnu.org/wiki/Visibility)
		#if defined (__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
			#ifndef GCC_HASCLASSVISIBILITY
				#define GCC_HASCLASSVISIBILITY
			#endif
		#endif // __GNUC__
		#define DLL_CALLCONV
		#if defined (GCC_HASCLASSVISIBILITY)
			#define __KabukiSDK __attribute__ ((visibility("default")))
		#else
			#define __KabukiSDK
		#endif		
	#endif // WIN32 / !WIN32
#else
	#define _KabukiSDK
	#define DLL_CALLCONV
#endif // __KabukiSDKLib
*/
