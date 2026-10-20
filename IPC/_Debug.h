// Copyright AStarship <https://astarship.net>.
// Minimal debug hooks for IPC builds (legacy harness compatibility).
// New seam code uses its own printf-based check counters.

#ifndef _IPC_DEBUG_H
#define _IPC_DEBUG_H

#ifdef _DEBUG
#define D_COUT(x) printf x
#else
#define D_COUT(x)
#endif

#endif // _IPC_DEBUG_H
