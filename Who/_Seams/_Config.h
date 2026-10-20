// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef PACKAGE_CONFIGURATION
#define PACKAGE_CONFIGURATION 1
#include <_ConfigHeader.h>

// Use a real ASCIICrabs seam number so the _ConfigFooter.h / _Seams.h
// chain is satisfied. We compile the Who type headers (including Random
// for salt generation) at the full release seam.
#define SEAM CRABS_RELEASE

#include <_ConfigDefault.h>
#include <_ConfigFooter.h>
#endif
