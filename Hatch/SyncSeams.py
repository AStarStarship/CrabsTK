#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
#
# SyncSeams.py -- Regenerate seam macros + test scaffolding from a seams YAML.
#
# Usage: python3 SyncSeams.py [seams_yaml]
#
# By default, reads seams.yaml from the same directory as this script.
#
# The YAML file is the SINGLE SOURCE OF TRUTH. Never edit the generated files
# by hand -- they are overwritten on the next sync.
#
# It generates (mode-dependent):
#   _Seams.h       -- SEAM macro definitions (PROJECT_CORE=1, ...) + SEAM_N.
#   _Config.h      -- package config: default SEAM + seam list comment.
#   _Main.hxx      -- single-TU entrypoint listing the seam tests.
#   _Tests.hxx     -- test-tree rollup (museworld / asciicrabs modes).
#   NN.<Seam>.hxx  -- per-seam test files.
#   <folder>/00.Core.hxx -- per-module test entry (crabstk hierarchical).
#
# Ordering: seams are numbered 1..N by a topological sort of each seam's
# `depends:` list (a seam is numbered after everything it depends on). When a
# seam has no `depends`, list order breaks ties. The `release` seam must end up
# the highest number.
#
# Schemas (auto-detected in the root yaml):
#   seams:   -- legacy flat: one module, per-seam `module:` fields.
#   modules: -- hierarchical: the root yaml lists MODULES (in build-up order),
#               each pointing at <folder>/__Seams.yaml which carries the
#               legacy seams: schema. Global numbering = module order x
#               module-seam order. See SyncModules().
#
# Modes:
#   crabstk     -- module-based, CT:: namespace, <Mod>/00.Core.hxx.
#   asciicrabs  -- flat _Seams/, ProjectTest namespace, ASCIICrabs includes.
#   museworld   -- flat root, single TU, MUSETest namespace. The seam test files
#                  live in <root>/_Seams/ and are included directly by
#                  <root>/_Main.hxx (no ASCIICrabs _Package.hxx / Test.hpp
#                  harness -- that lives inside the ASCIICrabs package).
#
# No third-party dependencies: the YAML subset is parsed in-line (pyyaml is not
# available on the build hosts).

import os
import re
import sys


# ---------------------------------------------------------------------------
# Minimal YAML reader (the subset the seams files use: top-level `key: value`
# scalars, list keys (`seams:`, `modules:`, `preamble:`) whose items are either
# `- key: value` dicts or `- scalar` strings, and `#` comments). No pyyaml
# dependency.
# ---------------------------------------------------------------------------

def LoadSeams(yaml_path):
  with open(yaml_path, "r") as f:
    lines = f.readlines()

  data = {}
  current = None    # current dict item (seam / module)
  list_key = None   # top-level key owning the open list
  for raw in lines:
    line = raw.rstrip("\n")
    stripped = line.strip()
    if not stripped or stripped.startswith("#"):
      continue
    # Top-level "key: value" (no leading dash).
    if not line[0].isspace():
      key, _, val = stripped.partition(":")
      key = key.strip()
      val = val.strip()
      if isinstance(current, dict):
        data[list_key].append(current)
        current = None
      if val == "":
        # A list key (seams: / modules: / preamble: / ...).
        list_key = key
        if key not in data:
          data[key] = []
      else:
        list_key = None
        data[key] = _ParseScalar(val)
      continue
    # List item "- key: value" or "- scalar".
    if stripped.startswith("-"):
      if list_key is None:
        raise SystemExit(f"SyncSeams: list item outside a list: {stripped}")
      rest = stripped[1:].strip()
      if rest and rest[0] in "'\"":
        # Quoted scalar: a colon inside the quotes is data, not a key.
        if isinstance(current, dict):
          data[list_key].append(current)
          current = None
        q = rest[0]
        end = rest.find(q, 1)
        data[list_key].append(rest[1:end] if end != -1 else rest)
        continue
      if rest and ":" in rest:
        if isinstance(current, dict):
          data[list_key].append(current)
        current = {}
        k, _, v = rest.partition(":")
        current[k.strip()] = _ParseScalar(v.strip())
      else:
        if isinstance(current, dict):
          data[list_key].append(current)
          current = None
        data[list_key].append(_ParseScalar(rest))
      continue
    # Continuation key inside a dict item (indented, "key: value").
    if current is not None and ":" in stripped:
      k, _, v = stripped.partition(":")
      current[k.strip()] = _ParseScalar(v.strip())
      continue
    raise SystemExit(f"SyncSeams: unparseable line: {stripped}")
  if isinstance(current, dict):
    data[list_key].append(current)
  data.setdefault("seams", [])
  return data


def _ParseScalar(val):
  if val == "" or val is None:
    return ""
  # Inline list: [a, b, c]
  if val.startswith("[") and val.endswith("]"):
    inner = val[1:-1].strip()
    if not inner:
      return []
    return [_ParseScalar(x) for x in inner.split(",")]
  # Quoted string.
  if (val.startswith('"') and val.endswith('"')) or (
      val.startswith("'") and val.endswith("'")):
    return val[1:-1]
  # Boolean.
  if val.lower() in ("true", "false"):
    return val.lower() == "true"
  # Integer.
  try:
    return int(val)
  except ValueError:
    pass
  return val


# ---------------------------------------------------------------------------
# Ordering: topological sort by `depends`, list order for ties.
# ---------------------------------------------------------------------------

def OrderSeams(seams):
  by_name = {s["name"].upper(): s for s in seams}
  # Kahn's algorithm. depends[] entries are seam names.
  indeg = {s["name"].upper(): 0 for s in seams}
  dependents = {s["name"].upper(): [] for s in seams}
  for s in seams:
    deps = s.get("depends") or []
    if isinstance(deps, str):
      deps = [deps]
    for d in deps:
      d = str(d).upper()
      if d in by_name and d != s["name"].upper():
        indeg[s["name"].upper()] += 1
        dependents[d].append(s["name"].upper())

  # Ready set: zero in-degree. Process in original list order (stable).
  order = []
  ready = [s["name"].upper() for s in seams if indeg[s["name"].upper()] == 0]
  seen = set()
  while ready:
    ready.sort(key=lambda n: [s["name"].upper() for s in seams].index(n))
    n = ready.pop(0)
    if n in seen:
      continue
    seen.add(n)
    order.append(by_name[n])
    for dep in dependents[n]:
      indeg[dep] -= 1
      if indeg[dep] == 0:
        ready.append(dep)
  if len(order) != len(seams):
    cycle = [s["name"] for s in seams if s["name"].upper() not in seen]
    raise SystemExit(f"SyncSeams: cycle in depends: {cycle}")
  return order


def FindRelease(seams, release_name):
  for seam in seams:
    if seam["name"].upper() == release_name.upper():
      return seam
  return seams[-1]


def TestFunctionName(project, seam_name):
  """MUSEWORLD_CORE -> Core. FVM_PARSER -> Parser."""
  parts = seam_name.lower().split("_")
  if len(parts) > 1 and parts[0] == project.lower():
    parts = parts[1:]
  return "".join(p.capitalize() for p in parts)


# ---------------------------------------------------------------------------
# _Seams.h
# ---------------------------------------------------------------------------

def GenerateSeamsH(project, ordered_seams, release_seam):
  lines = [
    "// Copyright AStarship <https://astarship.net>.",
    "//",
    "// AUTO-GENERATED by SyncSeams.py from _Seams.yaml.",
    "// Do not edit by hand -- edit _Seams.yaml and re-run SyncSeams.py.",
    "//",
    "#pragma once",
  ]
  guard = f"{project.upper()}_SEAMS_H"
  lines.append(f"#ifndef {guard}")
  lines.append(f"#define {guard} 1")
  lines.append("")
  for i, seam in enumerate(ordered_seams, 1):
    macro = seam["name"].upper()
    lines.append(f"#define {macro}\t\t\t{i}")
  rel_index = ordered_seams.index(release_seam) + 1
  lines.append(f"// Release seam: {release_seam['name'].upper()} = {rel_index}")
  lines.append("")
  lines.append(f"#define SEAM_N {release_seam['name'].upper()}")
  lines.append(f"#endif  // {guard}")
  lines.append("")
  return "\n".join(lines)


# ---------------------------------------------------------------------------
# _Config.h (seam-level)
# ---------------------------------------------------------------------------

def GenerateConfigH(project, ordered_seams, release_seam, mode,
                    module_rows=None):
  release_macro = release_seam["name"].upper()
  lines = [
    "// Copyright AStarship <https://astarship.net>.",
    "//",
    "// AUTO-GENERATED by SyncSeams.py from _Seams.yaml.",
    "// Do not edit by hand -- edit _Seams.yaml and re-run SyncSeams.py.",
    "//",
    "// To debug a specific seam, set SEAM to that seam's number. The seam at",
    "// that number runs its full checks; every lower seam is compiled in (and",
    "// its fast checks run) so the module builds layer by layer on top of its",
    "// dependencies. All higher seams are not compiled.",
    "",
    "#pragma once",
    "#ifndef PACKAGE_CONFIGURATION",
    "#define PACKAGE_CONFIGURATION 1",
  ]
  # The seam table comes from the package root _Seams.h (angle-bracket include
  # resolved by the build's include path).
  if mode == "museworld":
    lines.append('#include "../../ASCIICrabs/_ConfigHeader.h"')
    lines.append('#include "../../ASCIICrabs/_ConfigDefault.h"')
    lines.append('#include "../../ASCIICrabs/_ConfigFooter.h"')
  else:
    lines.append("#include <_Seams.h>")
  lines.append("")
  # The in-house single-TU rollup: pulls in every ASCIICrabs implementation
  # (.hxx) needed by the test primitives and the print/streaming layer.
  lines.append('#include "../../ASCIICrabs/_Package.hxx"')
  lines.append("")
  # The single-TU build needs the ASCIICrabs test primitives (TTestTree,
  # SeamResult, ArgsToString, Test/TestEQ, TestFail) available to the seam
  # files. The declarations come with _Package.hxx; this re-asserts the
  # templates so the seam files can instantiate TTestTree.
  lines.append('#include "../../ASCIICrabs/Test.hpp"')
  lines.append("")
  lines.append("#ifndef SEAM")
  lines.append(f"#define SEAM {release_macro}")
  lines.append("#endif")
  lines.append("")
  lines.append("/* Seams:")
  for i, seam in enumerate(ordered_seams, 1):
    marker = "  (release)" if seam is release_seam else ""
    lines.append(f"#define {seam['name'].upper()}\t\t\t{i}{marker}")
  lines.append("*/")
  lines.append("")
  # Hierarchical mode: where each module's seams (and code) live. Build
  # metadata -- add a folder to the include path when compiling against a
  # module's headers.
  if module_rows:
    lines.append("/* Module folders (build metadata; paths relative to the project root):")
    width = max(len(name) for name, _ in module_rows)
    for name, folder in module_rows:
      lines.append(f"  {name:<{width}} -> {folder}")
    lines.append("*/")
    lines.append("")
  lines.append("#endif  // PACKAGE_CONFIGURATION")
  lines.append("")
  return "\n".join(lines)


# ---------------------------------------------------------------------------
# _Main.hxx (single-TU entrypoint)
# ---------------------------------------------------------------------------

def GenerateMainCpp(project, ordered_seams, release_seam, mode):
  project_upper = project.upper()
  if mode == "museworld":
    # Flat root: the .hxx seam files live in <root>/_Seams/ and are included
    # directly (no ASCIICrabs _Package.hxx / Test.hpp harness).
    return f"""// Copyright AStarship <https://astarship.net>.
//
// AUTO-GENERATED by SyncSeams.py from _Seams.yaml.
// Do not edit by hand -- edit _Seams.yaml and re-run SyncSeams.py.
//
// {project_upper} single translation unit. One .hxx entry, many seams: SEAM
// selects the layer under test (see _Seams.h); every lower seam is compiled
// in and its fast checks run, and the deepest seam's full checks run when
// SEAM == its number. The exe's single .cpp is the generated wrapper that
// includes this file (one .cpp = one .exe). Run: ./MUSEWorld (exit 0 = all
// checks passed).
#include "_Config.h"
#include "_Seams/_Tests.hxx"

using namespace ::_;

ISN main(ISN arg_count, CHA** args) {{
  const CHA* argss = ArgsToString(arg_count, args);
  return SeamResult({project}Tests(argss));
}}
"""
  # Fallback: asciicrabs / crabstk (unchanged legacy behavior).
  if mode == "crabstk":
    modules = {}
    for seam in ordered_seams:
      mod = seam.get("module", project.capitalize())
      modules.setdefault(mod, []).append(seam)
    module_includes = "\n".join(f'#include "{mod}/00.Core.hxx"' for mod in modules)
    test_fns = ", ".join(
        f"{seam.get('module', project.capitalize())}::Test::"
        f"{TestFunctionName(project, seam['name'])}"
        for seam in ordered_seams
    )
    return f"""#include <_Config.h>
#include <_Package.hxx>
{module_includes}
using namespace _;
using namespace CT;
ISN main(ISN arg_count, CHA** args) {{
  return TTestTree<{test_fns}>(arg_count, args);
}}
"""
  return f"""#include "pch.h"
#include <_Config.h>
#include <_Tests.hxx>
using namespace ::_;
ISN main(ISN arg_count, CHA** args) {{
  return TTestTree<ProjectTests>(arg_count, args);
}}
"""


def GenerateCrabstkMainHxx(project, module_info):
  """Hierarchical crabstk entrypoint: one include per module (in module
  order, folder-relative to _Seams/) + the full test tree. Dormant
  modules are skipped (their 00.Core.hxx exists but is not compiled).

  The tree is a plain TestCase[] passed to SeamTreeTest (the same
  function TTestTree delegates to in ASCIICrabs Test.hpp) rather than
  TTestTree's template parameter pack: GCC (>=13) rejects
  namespace-qualified function names as template arguments for
  function-pointer type parameters (unqualified names work, qualified
  ones do not), and every module test here is qualified."""
  incs = []
  fns = []
  for mi in module_info:
    if not mi["seams"] or mi["dormant"]:
      continue
    rel = os.path.relpath(os.path.join(mi["folder"], "00.Core.hxx"), "_Seams")
    incs.append(f'#include "{rel.replace(os.sep, "/")}"')
    for s in mi["seams"]:
      fns.append(f'CT::{mi["name"]}::Test::{TestFunctionName(project, s["name"])}')
  module_includes = "\n".join(incs)
  tree_entries = "\n".join(f"  {fn}," for fn in fns)
  return f"""#include "pch.h"
#include <_Config.h>
#include <_Package.hxx>
// Single-TU implementations the seam tests link against (same set as the
// pre-reorg _Seams/_Main.cpp entry; _Package.hxx is NOT pulled in here —
// the root config deliberately excludes the full ASCIICrabs machine).
#include <ASCIICrabs/COut.h>
#include <ASCIICrabs/COut.hxx>
#include <ASCIICrabs/AType.hxx>
#include <ASCIICrabs/Array.hxx>
#include <ASCIICrabs/Puff.hxx>
#include <ASCIICrabs/Test.hpp>
#include <ASCIICrabs/Test.hxx>
#include <ASCIICrabs/Stringf.hxx>
{module_includes}
using namespace _;
using namespace CT;
namespace {{
::TestCase CrabsTkTree[] = {{
{tree_entries}
}};
}}
ISN main(ISN arg_count, CHA** args) {{
  return SeamTreeTest(arg_count, args, CrabsTkTree,
                      sizeof(CrabsTkTree) / sizeof(CrabsTkTree[0]));
}}
"""


# ---------------------------------------------------------------------------
# _Tests.hxx (museworld + asciicrabs)
# ---------------------------------------------------------------------------

def GenerateTestsHxx(project, ordered_seams, release_seam, mode):
  if mode not in ("museworld", "asciicrabs"):
    return None
  ns = "MUSETest" if mode == "museworld" else "ProjectTest"
  fn = f"{project}Tests"
  includes = "\n".join(f'#include "{seam["file"]}"' for seam in ordered_seams)
  test_fns = ", ".join(
      f"{ns}::{TestFunctionName(project, seam['name'])}" for seam in ordered_seams
  )
  return f"""// Copyright AStarship <https://astarship.net>.
//
// AUTO-GENERATED by SyncSeams.py from _Seams.yaml.
#include <_Config.h>
//
{includes}
//
#include <Test.hpp>
using namespace ::_;

inline const CHA* {fn}(const CHA* args) {{
  return TTestTree<{test_fns}>(args);
}}
"""


# ---------------------------------------------------------------------------
# Per-seam .hxx files
# ---------------------------------------------------------------------------

def GenerateSeamHxx(project, ordered_seams, release_seam, mode):
  ns = "MUSETest" if mode == "museworld" else "ProjectTest"
  templates = {}
  for i, seam in enumerate(ordered_seams, 1):
    macro = seam["name"].upper()
    fn = TestFunctionName(project, seam["name"])
    label = seam.get("label", f"Testing {seam['name']}...")
    is_release = seam is release_seam
    check_file = seam.get("check_file")
    if not is_release:
      body = f"""  A_TEST_BEGIN;
  D_COUT(Headingf("{label}"))"""
      if check_file:
        body += f"\n  MUSETestRun{fn}();"
    else:
      body = f"  // {label} (release seam): runs the full suite once all lower"
      body += f"\n  // seams pass. No checks of its own."

    header = f"""// Copyright AStarship <https://astarship.net>.
//
// AUTO-GENERATED by SyncSeams.py from _Seams.yaml.
#if SEAM >= {macro}
#if SEAM == {macro}
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
"""
    if check_file:
      # The check file carries its own includes + a MUSETest namespace that
      # defines MUSETestRun{fn}(). It must be at file scope (not inside the
      # test function), so include it here; the test function below lives in
      # the same MUSETest namespace and calls MUSETestRun{fn}() unqualified.
      content = (
          header
          + f'#if SEAM >= {macro}\n#include "{check_file}"\n#endif\n\n'
          + f"namespace {ns} {{\n\n"
          + f"inline const CHA* {fn}(const CHA* args) {{\n"
          + f"#if SEAM >= {macro}\n{body}\n#endif\n"
          + "  return NILP;\n}\n\n"
          + f"}}  //< namespace {ns}\n"
      )
    else:
      content = (
          header
          + f"namespace {ns} {{\n\n"
          + f"inline const CHA* {fn}(const CHA* args) {{\n"
          + f"#if SEAM >= {macro}\n{body}\n#endif\n"
          + "  return NILP;\n}\n\n"
          + f"}}  //< namespace {ns}\n"
      )
    templates[seam["file"]] = content
  return templates


# ---------------------------------------------------------------------------
# Hierarchical modules: schema (root yaml with `modules:`)
# ---------------------------------------------------------------------------
# Root __Seams.yaml lists MODULES in build-up order; each module points at
# <folder>/__Seams.yaml (the legacy seams: schema). Global numbering =
# module order x module-seam order. The release module must be last; a module
# may be flagged dormant (numbered + scaffolded, but not compiled into the
# rollup). 00.Core.hxx is merge-safe: user code between
# // SYNC-KEEP-BEGIN / // SYNC-KEEP-END is preserved byte-for-byte; on the
# first sync, markers are seeded around any existing non-placeholder body,
# and a frozen leading /* ... */ header block is kept.

def SyncModules(project_dir, data, mode):
  project = str(data.get("project", "PROJECT"))
  if "seams" in data and data["seams"]:
    raise SystemExit(
        "SyncSeams: root yaml has both modules: and seams: -- pick one "
        "(modules: for hierarchical, seams: for legacy flat).")
  modules = data.get("modules") or []
  if not modules:
    raise SystemExit("SyncSeams: modules: list is empty")
  for m in modules:
    if not isinstance(m, dict) or not m.get("name"):
      raise SystemExit(f"SyncSeams: a module in modules: has no name: {m}")
  release_module = str(data.get("release_module", modules[-1]["name"]))
  if modules[-1]["name"] != release_module:
    raise SystemExit(
        f"SyncSeams: release module {release_module} must be the LAST entry "
        f"in modules: (last is {modules[-1]['name']}).")

  module_info = []
  all_seams = []
  seen = {}
  for m in modules:
    name = str(m["name"])
    folder = m.get("folder") or f"_Seams/{name}"
    if os.path.isabs(str(folder)):
      raise SystemExit(f"SyncSeams: module {name}: folder must be relative")
    folder = str(folder)
    mpath = os.path.join(project_dir, folder, "__Seams.yaml")
    if not os.path.exists(mpath):
      raise SystemExit(
          f"SyncSeams: module {name}: {os.path.relpath(mpath, project_dir)} "
          f"not found.")
    mdata = LoadSeams(mpath)
    mseams = mdata.get("seams") or []
    for s in mseams:
      s["module"] = name
    ordered_m = OrderSeams(mseams)
    for s in ordered_m:
      key = s["name"].upper()
      if key in seen:
        raise SystemExit(
            f"SyncSeams: duplicate seam name {key} (modules "
            f"{seen[key]} and {name}).")
      seen[key] = name
      all_seams.append(s)
    module_info.append({
        "name": name,
        "folder": folder,
        "seams": ordered_m,
        "dormant": bool(m.get("dormant")),
        "mdata": mdata,
    })

  if not all_seams:
    raise SystemExit("SyncSeams: no seams found in any module")

  # Global depends validation: every dep must exist and sit STRICTLY earlier
  # in the global table (a forward or same-module-later dep is a hard error;
  # per-module Kahn already ordered same-module deps correctly).
  for i, s in enumerate(all_seams):
    deps = s.get("depends") or []
    if isinstance(deps, str):
      deps = [deps]
    for d in deps:
      d = str(d).upper()
      if d not in seen:
        raise SystemExit(
            f"SyncSeams: module {s['module']}: seam {s['name']} depends on "
            f"unknown seam {d}.")
      dep_idx = next(j for j, x in enumerate(all_seams)
                     if x["name"].upper() == d)
      if dep_idx >= i:
        raise SystemExit(
            f"SyncSeams: module {s['module']}: seam {s['name']} depends on "
            f"{d}, which is not earlier in the global table.")

  release_mod = next(mi for mi in module_info
                     if mi["name"] == release_module)
  release_seam = release_mod["seams"][-1] if release_mod["seams"] \
      else all_seams[-1]
  if not release_mod["seams"]:
    print(f"SyncSeams: note: release module {release_module} has no seams; "
          f"using {all_seams[-1]['name'].upper()} as the release seam.")

  seams_dir = os.path.join(project_dir, "_Seams")
  os.makedirs(seams_dir, exist_ok=True)
  written = []

  def Write(root, rel_path, content):
    path = os.path.join(root, rel_path)
    os.makedirs(os.path.dirname(path), exist_ok=True) if os.path.dirname(
        path) else None
    with open(path, "w") as f:
      f.write(content)
    written.append(os.path.relpath(path, project_dir))

  # Seam table: project root + _Seams/ (matches the flat crabstk layout).
  table = GenerateSeamsH(project, all_seams, release_seam)
  Write(project_dir, "_Seams.h", table)
  Write(seams_dir, "_Seams.h", table)
  Write(seams_dir, "_Config.h", GenerateConfigH(
      project, all_seams, release_seam, mode,
      module_rows=[(mi["name"], mi["folder"]) for mi in module_info]))
  Write(seams_dir, "_Main.hxx", GenerateCrabstkMainHxx(project, module_info))
  for mi in module_info:
    if not mi["seams"]:
      continue
    Write(project_dir, f"{mi['folder']}/00.Core.hxx",
          GenerateModuleCoreHxx(project, project_dir, mi, release_seam))

  print(f"Synced {len(written)} files from {os.path.basename(__file__)} "
        f"({len(module_info)} modules):")
  for w in written:
    print(f"  {w}")
  print(f"\nSeam table (module order x depends):")
  for i, seam in enumerate(all_seams, 1):
    marker = "  (release)" if seam is release_seam else ""
    dormant = "  (dormant)" if seam["module"] == release_module and \
        next(mi for mi in module_info if mi["name"] == seam["module"])[
            "dormant"] else ""
    print(f"  {i}. {seam['name'].upper()} = {i}  [{seam['module']}]{marker}")


# ---------------------------------------------------------------------------
# Merge-safe <folder>/00.Core.hxx
# ---------------------------------------------------------------------------

def ExtractLeadingComment(text):
  """Return the frozen leading /* ... */ block (MPL/Doxygen header) if the
  file carries one above its first directive, else None."""
  lines = text.splitlines()
  first_code = None
  for i, ln in enumerate(lines):
    s = ln.strip()
    if s.startswith("#") or s.startswith("namespace") or s.startswith("using"):
      first_code = i
      break
  window = lines[:first_code] if first_code is not None else lines
  for i, ln in enumerate(window):
    if ln.strip().startswith("/*"):
      j = i
      while j < len(window) and "*/" not in window[j]:
        j += 1
      if j < len(window):
        return window[i:j + 1]
  return None


_FN_RE = re.compile(r"^inline const CHA\* (\w+)\(const CHA\* args\) \{\s*$")


def ExtractFunctionBodies(text):
  """fn name -> lines between its opening brace and its closing brace."""
  bodies = {}
  lines = text.splitlines()
  i = 0
  while i < len(lines):
    m = _FN_RE.match(lines[i])
    if m:
      fn = m.group(1)
      i += 1
      body = []
      while i < len(lines) and lines[i].strip() != "}":
        body.append(lines[i])
        i += 1
      bodies[fn] = body
    i += 1
  return bodies


def SeedBody(fn_lines, is_release):
  """User lines to place between the SYNC-KEEP markers for this function.

  Marker-present files keep their block byte-for-byte. First-run files get
  markers seeded around the non-placeholder remainder of the old body (the
  skeleton lines -- gating, A_TEST_BEGIN, D_COUT, placeholder, returns --
  are regenerated, everything else is user code)."""
  if fn_lines and "// SYNC-KEEP-BEGIN" in fn_lines and \
      "// SYNC-KEEP-END" in fn_lines:
    b = fn_lines.index("// SYNC-KEEP-BEGIN")
    e = fn_lines.index("// SYNC-KEEP-END")
    return list(fn_lines[b + 1:e])
  if fn_lines:
    kept = []
    for ln in fn_lines:
      s = ln.strip()
      if not s:
        continue
      if s.startswith("#if SEAM") or s.startswith("#elif SEAM") \
          or s == "#endif":
        continue
      if s == "A_TEST_BEGIN;":
        continue
      if s.startswith("D_COUT(Headingf("):
        continue
      if s == "// Your test code here.":
        continue
      if re.match(r"^return\b", s):
        continue
      kept.append(ln)
    if kept:
      return kept
  if is_release:
    return []
  return ["  // Your test code here."]


def GenerateModuleCoreHxx(project, project_dir, mi, release_seam):
  name, folder = mi["name"], mi["folder"]
  seams = mi["seams"]
  existing = ""
  p = os.path.join(project_dir, folder, "00.Core.hxx")
  if os.path.exists(p):
    with open(p, "r") as f:
      existing = f.read()
  header = ExtractLeadingComment(existing)
  bodies = ExtractFunctionBodies(existing)
  preamble = [str(x) for x in (mi["mdata"].get("preamble") or [])]

  lines = [
    "// Copyright AStarship <https://astarship.net>.",
    "//",
    f"// AUTO-GENERATED by SyncSeams.py from {folder}/__Seams.yaml.",
    "// Do not edit by hand -- edit __Seams.yaml and re-run SyncSeams.py.",
    "// Code between the SYNC-KEEP markers is preserved on every sync.",
  ]
  if header:
    lines.append("")
    lines.extend(header)
  lines += ["#pragma once", "#include <_Config.h>", ""]
  for s in seams:
    macro = s["name"].upper()
    lines.append(f"#if SEAM == {macro}")
    lines.append("#include <ASCIICrabs/_Debug.h>")
    lines.append(f"#elif SEAM > {macro}")
    lines.append("#include <ASCIICrabs/_Release.h>")
    lines.append("#endif")
  lines.append("#include <ASCIICrabs/Test.h>")
  lines.append("")
  lines.extend(preamble)
  if preamble:
    lines.append("")
  lines += ["using namespace _;", "namespace CT {",
            f"namespace {name} {{", "namespace Test {", ""]
  legacy_core_consumed = False
  for s in seams:
    macro = s["name"].upper()
    fn = TestFunctionName(project, s["name"])
    label = s.get("label") or f"Testing {s['name']}..."
    is_release = s is release_seam
    old = bodies.get(fn)
    if old is None and fn != "Core" and not legacy_core_consumed:
      # Legacy files named their single entry point `Core`; seed its body
      # into the first function that has no exact-name match, once.
      old = bodies.get("Core")
      legacy_core_consumed = True
    kept = SeedBody(old, is_release)
    lines.append(f"inline const CHA* {fn}(const CHA* args) {{")
    if is_release:
      lines.append(f"#if SEAM == {macro}")
      lines.append("// SYNC-KEEP-BEGIN")
      lines.extend(kept)
      lines.append("// SYNC-KEEP-END")
      lines.append("#endif")
    else:
      lines.append(f"#if SEAM >= {macro}")
      lines.append("  A_TEST_BEGIN;")
      lines.append(f'  D_COUT(Headingf("{label}"));')
      lines.append("")
      lines.append("// SYNC-KEEP-BEGIN")
      lines.extend(kept)
      lines.append("// SYNC-KEEP-END")
      lines.append("#endif")
    lines.append("  return NILP;")
    lines.append("}")
    lines.append("")
  lines += ["}  //< namespace Test", f"}}  //< namespace {name}",
            "}  //< namespace CT", ""]
  return "\n".join(lines)


# ---------------------------------------------------------------------------
# Main sync
# ---------------------------------------------------------------------------

def Sync(project_dir, yaml_path, mode):
  data = LoadSeams(yaml_path)
  project = data.get("project", "PROJECT")
  if data.get("modules"):
    if mode != "crabstk":
      raise SystemExit("SyncSeams: the modules: schema requires mode: crabstk")
    return SyncModules(project_dir, data, mode)
  seams = data.get("seams", [])
  release_name = data.get("release", seams[-1]["name"])
  ordered = OrderSeams(seams)
  release_seam = FindRelease(ordered, release_name)

  # Validate: the release seam must be the highest number.
  rel_index = ordered.index(release_seam)
  if rel_index != len(ordered) - 1:
    raise SystemExit(
        f"SyncSeams: release seam {release_seam['name']} is not last in the "
        f"ordered table (position {rel_index + 1} of {len(ordered)}).")

  seams_dir = os.path.join(project_dir, "_Seams")
  os.makedirs(seams_dir, exist_ok=True)
  written = []

  def Write(root, rel_path, content):
    path = os.path.join(root, rel_path)
    os.makedirs(os.path.dirname(path), exist_ok=True) if os.path.dirname(
        path) else None
    with open(path, "w") as f:
      f.write(content)
    written.append(os.path.relpath(path, project_dir))

  # _Seams.h -> project root (the package's seam table).
  Write(project_dir, "_Seams.h", GenerateSeamsH(project, ordered, release_seam))

  if mode == "museworld":
    # _Config.h + _Main.hxx -> project root (single TU lives in the root).
    # _Tests.hxx + per-seam .hxx -> <root>/_Seams/.
    Write(project_dir, "_Config.h",
          GenerateConfigH(project, ordered, release_seam, mode))
    Write(project_dir, "_Main.hxx",
          GenerateMainCpp(project, ordered, release_seam, mode))
    tests = GenerateTestsHxx(project, ordered, release_seam, mode)
    if tests:
      Write(seams_dir, "_Tests.hxx", tests)
    for fname, fcontent in GenerateSeamHxx(
            project, ordered, release_seam, mode).items():
      Write(seams_dir, fname, fcontent)
  else:
    Write(seams_dir, "_Seams.h",
          GenerateSeamsH(project, ordered, release_seam))
    Write(seams_dir, "_Config.h",
          GenerateConfigH(project, ordered, release_seam, mode))
    Write(seams_dir, "_Main.hxx",
          GenerateMainCpp(project, ordered, release_seam, mode))
    tests = GenerateTestsHxx(project, ordered, release_seam, mode)
    if tests:
      Write(seams_dir, "_Tests.hxx", tests)
    if mode == "asciicrabs":
      for fname, fcontent in GenerateSeamHxx(
              project, ordered, release_seam, mode).items():
        Write(seams_dir, fname, fcontent)
    else:
      # crabstk: one 00.Core.hxx per module.
      modules = {}
      for seam in ordered:
        mod = seam.get("module", project.capitalize())
        modules.setdefault(mod, []).append(seam)
      for mod, mod_seams in modules.items():
        lines = ["#pragma once", '#include <_Config.h>', ""]
        for seam in mod_seams:
          macro = seam["name"].upper()
          lines.append(f"#if SEAM == {macro}")
          lines.append('#include <ASCIICrabs/_Debug.h>')
          lines.append(f"#elif SEAM > {macro}")
          lines.append('#include <ASCIICrabs/_Release.h>')
          lines.append("#endif")
        lines.append('#include <ASCIICrabs/Test.h>')
        lines.append("")
        lines.append("using namespace _;")
        lines.append("namespace CT {")
        lines.append(f"namespace {mod} {{")
        lines.append("namespace Test {")
        lines.append("")
        for seam in mod_seams:
          macro = seam["name"].upper()
          fn = TestFunctionName(project, seam["name"])
          label = seam.get("label", f"Testing {seam['name']}...")
          is_release = seam is release_seam
          lines.append(f"inline const CHA* {fn}(const CHA* args) {{")
          if is_release:
            lines.append(f"#if SEAM == {macro}")
            lines.append("#endif")
          else:
            lines.append(f"#if SEAM >= {macro}")
            lines.append("  A_TEST_BEGIN;")
            lines.append(f'  D_COUT(Headingf("{label}"));')
            lines.append("")
            lines.append("  // Your test code here.")
            lines.append("")
            lines.append("#endif")
          lines.append("  return NILP;")
          lines.append("}")
          lines.append("")
        lines.append("}  //< namespace Test")
        lines.append(f"}}  //< namespace {mod}")
        lines.append("}  //< namespace CT")
        lines.append("")
        Write(seams_dir, f"{mod}/00.Core.hxx", "\n".join(lines))

  print(f"Synced {len(written)} files from {os.path.basename(yaml_path)}:")
  for w in written:
    print(f"  {w}")
  print(f"\nSeam table (ordered by depends):")
  for i, seam in enumerate(ordered, 1):
    marker = "  (release)" if seam is release_seam else ""
    print(f"  {i}. {seam['name'].upper()} = {i}{marker}")


def Main():
  script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
  if len(sys.argv) >= 2:
    yaml_path = os.path.abspath(sys.argv[1])
  else:
    yaml_path = os.path.join(script_dir, "seams.yaml")
  if not os.path.exists(yaml_path):
    print(f"Error: {yaml_path} not found")
    return 1
  data = LoadSeams(yaml_path)
  project = data.get("project", "PROJECT")
  mode = data.get("mode", "asciicrabs")
  project_dir = data.get("project_dir", ".")
  if project_dir == ".":
    project_dir = os.path.dirname(os.path.abspath(yaml_path))
  elif not os.path.isabs(project_dir):
    project_dir = os.path.abspath(project_dir)
  print(f"Project: {project}")
  print(f"Mode:    {mode}")
  print(f"Dir:     {project_dir}")
  print()
  Sync(project_dir, yaml_path, mode)
  return 0


if __name__ == "__main__":
  sys.exit(Main())
