#pragma once
#ifndef PACKAGE_CONFIGURATION
#define PACKAGE_CONFIGURATION 1
#include "../../ASCIICrabs/_ConfigHeader.h"

// Narrow opt-in for the IMUL 0.1 compiler seam build. -DCRABSTK_IMUL_SEAM
// selects the IMUL test tree; the normal build keeps SCRIPT2_STACK.
#if defined(CRABSTK_IMUL_SEAM) && !defined(SEAM)
#define SEAM CRABSTK_IMUL_CORE
#else
#ifndef SEAM
#define SEAM SCRIPT2_STACK
#endif
#endif

#include "../../ASCIICrabs/_ConfigDefault.h"
#include "../../ASCIICrabs/_ConfigFooter.h"
#endif
