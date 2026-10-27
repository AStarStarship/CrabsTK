// Copyright AStarship <https://astarship.net>.
// Minimal debug hooks for IPC builds (legacy harness compatibility).
// New seam code uses its own printf-based check counters.

#ifndef _IPC_DEBUG_H
#define _IPC_DEBUG_H

// Guarded so the root rollup can keep the ASCIICrabs D_COUT (pulled in
// before the IPC seam files); standalone IPC builds still get this one.
#ifndef D_COUT
#ifdef _DEBUG
#define D_COUT(x) printf x
#else
#define D_COUT(x)
#endif
#endif

#endif // _IPC_DEBUG_H
