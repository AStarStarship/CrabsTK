// Copyright AStarship <https://astarship.net>.
// CrabsTK root implementation rollup — single-TU build.
//
// Pro/ and Touch/ were removed from the rollup (2026-10-12): they
// reference a deleted Script2 API (TString<T>, TStack<T>, Op, Expr,
// CHN, system()) and do not compile. Data/ was added — it was missing
// from the rollup despite being a clean, verified module.

// The root _Config.h is deliberately narrow (types only: _ConfigHeader.h),
// so the single-TU rollup supplies the rest of the canonical ASCIICrabs
// config trio: _ConfigDefault.h (USING_STR/LARGEST_CHAR selectors),
// _ConfigFooter.h (type-size enumerators _ECE/_ECD/_ECC/_ECB, ISX/ISM/ISV,
// self-contained with #ifndef-guarded defaults), and Binary.hpp's
// IsError/IsErrorSocket predicates (used by the Code/Env/IMUL .hxx files).
// All three headers are pragma-once, so this is safe in any chain.

#include <ASCIICrabs/_ConfigDefault.h>
#include <ASCIICrabs/_ConfigFooter.h>
#include <ASCIICrabs/Binary.hpp>

#include "Audio/_Seams.hxx"
#include "Code/_Seams.hxx"
#include "Data/_Seams.hxx"
#include "Env/_Seams.hxx"
#include "Forms/_Seams.hxx"
#include "GUI/_Seams.hxx"
#include "Image/_Seams.hxx"
#include "IMUL/_Seams.hxx"
#include "IPC/_Seams.hxx"
#include "Math/_Seams.hxx"
#include "Plot/_Seams.hxx"
#include "Pro/_Seams.hxx"
#include "Touch/_Seams.hxx"
#include "Web/_Seams.hxx"
#include "Who/_Seams.hxx"
