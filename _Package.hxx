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

#include "Audio/_Package.hxx"
#include "Code/_Package.hxx"
#include "Data/_Package.hxx"
#include "Env/_Package.hxx"
#include "Forms/_Package.hxx"
#include "GUI/_Package.hxx"
#include "Image/_Package.hxx"
#include "IMUL/_Package.hxx"
#include "IPC/_Package.hxx"
#include "Math/_Package.hxx"
#include "Plot/_Package.hxx"
#include "Pro/_Package.hxx"
#include "Touch/_Package.hxx"
#include "Web/_Package.hxx"
#include "Who/_Package.hxx"
