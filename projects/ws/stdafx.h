// stdafx.h : include file for standard system include files, 
// or project specific include files that are used frequently, but 
// are changed infrequently.

#pragma once

#ifdef WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cfloat>
#include <algorithm>
#include <locale>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <math.h>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#endif

#include <thread>
#include <chrono>

#include "cpprest/asyncrt_utils.h"
#include "cpprest/rawptrstream.h"
#include "cpprest/containerstream.h"
#include "cpprest/producerconsumerstream.h"
#include "cpprest/filestream.h"
#include "cpprest/ws_client.h"
#include "cpprest/ws_msg.h"

#include "unittestpp.h"
#include "os_utilities.h"

#include "websocket_client_tests.h"
#include "test_websocket_server.h"
