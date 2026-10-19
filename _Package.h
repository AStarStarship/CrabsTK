// Copyright AStarship <https://astarship.net>.
// CrabsTK root package — declaration/template layer.
//
// This file is the single-TU *declaration* rollup. It pulls in the
// public headers (types, templates, declarations) for every module.
// The *implementation* rollup is _Package.hxx (included by the
// single-TU build, not by consumers).
//
// Dead _Impl.h includes were removed (2026-10-12): none of those
// files ever existed.

#pragma once

#include "Data/_Package.h"
#include "Plot/_Package.h"
#include "Image/_Package.h"
#include "Code/_Package.h"
#include "Forms/_Package.h"
#include "Env/_Package.h"
