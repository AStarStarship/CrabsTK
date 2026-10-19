// Copyright AStarship <https://astarship.net>.
// CrabsTK root implementation rollup — single-TU build.
//
// Pro/ and Touch/ were removed from the rollup (2026-10-12): they
// reference a deleted Script2 API (TString<T>, TStack<T>, Op, Expr,
// CHN, system()) and do not compile. Data/ was added — it was missing
// from the rollup despite being a clean, verified module.

#include "Code/_Package.hxx"
#include "Data/_Package.hxx"
#include "Forms/_Package.hxx"
#include "Image/_Package.hxx"
#include "Plot/_Package.hxx"
#include "Env/_Package.hxx"
