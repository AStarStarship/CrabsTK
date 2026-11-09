#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
#
# SeamsToYaml.py — reverse-bootstraps the _Seams.yaml files (root + per-module)
# from an existing hand-written _Seams.h seam table.
#
# Why: iGeek's seam table (_Seams/_Seams.h) was written by hand. The Hatch
# system wants _Seams.yaml as the single source of truth. This tool reads the
# existing table, infers the module grouping (by the world name embedded in the
# macro prefix), and emits:
#   - <project_dir>/_Seams.yaml        (root: module list, build-up order)
#   - <project_dir>/<folder>/_Seams.yaml  (per module: the seam list)
# so that `SyncSeams.py _Seams.yaml` (in igeek mode) can regenerate the same
# _Seams.h / _Config.h / _Main.hxx without renumbering.
#
# Usage:
#   python3 SeamsToYaml.py <path-to-_Seams.h> [project] [mode] [base_offset]
#
#   path-to-_Seams.h : the hand-written seam table to read.
#   project          : project name (macro prefix), e.g. "iGeek". Default:
#                      derived from the parent dir name.
#   mode             : "igeek" (default for iGeek) or "crabstk".
#   base_offset      : the number of ASCIICrabs base seams (1..base_offset) that
#                      precede the project's own seams. iGeek = 23 (CRABS_* is
#                      1..23, so iGeek starts at 24). Default: 0 (number from 1).
#
# The tool is read-only with respect to the _Seams.h (it never edits it); it
# only writes the _Seams.yaml files. Re-running is idempotent: existing
# _Seams.yaml files are overwritten with the inferred content (the _Seams.h is
# the source of truth on bootstrap; once you start editing the YAML, stop
# re-running this and edit the YAML instead).

import os
import re
import sys

# Match: #define MACRO_NAME   <number>   (tabs/spaces between).
_DEFINE_RE = re.compile(r"^\s*#define\s+([A-Z][A-Z0-9_]*)\s+(\d+)\s*(//.*)?$")
# Match: #define SEAM_N  SOME_MACRO   (the release seam pointer).
_SEAMN_RE = re.compile(r"^\s*#define\s+SEAM_N\s+([A-Z][A-Z0-9_]*)\s*(//.*)?$")


def ParseSeamsH(path):
  """Return (ordered_seams, release_macro). ordered_seams is a list of
  (macro, number) in file order; release_macro is the SEAM_N target."""
  seams = []
  release = None
  with open(path, "r") as f:
    for raw in f:
      line = raw.rstrip("\n")
      m = _SEAMN_RE.match(line)
      if m:
        release = m.group(1)
        continue
      m = _DEFINE_RE.match(line)
      if m:
        macro, num = m.group(1), ISC(m.group(2))
        # Skip SEAM_N itself (a pointer, not a numbered seam) and guards.
        if macro == "SEAM_N" or macro.endswith("_SEAMS_H") or macro == "PACKAGE_CONFIGURATION":
          continue
        seams.append((macro, num))
  if not seams:
    raise SystemExit(f"SeamsToYaml: no numbered #define seams found in {path}")
  if release is None:
    # Default: the highest-numbered seam is the release.
    release = max(seams, key=lambda x: x[1])[0]
  # Sort by number (the table should already be ascending, but be safe).
  seams.sort(key=lambda x: x[1])
  return seams, release


def InferModule(macro, project):
  """Infer the module (folder group) for a seam macro.

  iGeek convention: IGEEK_<WORLD>_CORE / IGEEK_<WORLD>_RELEASE -> module <World>
  (the PascalCase of the world token, e.g. IGEEK_MAZEWORLD_* -> MazeWorld,
  IGEEK_ARMWORLD_* -> ArmWorld). The bare IGEEK_CORE / IGEEK_RELEASE (no world
  token) -> module "Core" (the RL/agent core, the first iGeek module after
  ASCIICrabs).

  We recover the PascalCase world name from the all-caps token by matching it
  against the known world dirs if provided, else by a CamelCase heuristic on the
  trailing CamelCase-able tokens. The caller passes a world_name_map for exact
  recovery (all-caps -> PascalCase), because the heuristic can't know that
  MAZEWORLD -> MazeWorld (one word) vs ARMWORLD -> ArmWorld (two words).
  """
  prefix = project.upper() + "_"
  if not macro.startswith(prefix):
    # A seam outside the project prefix (shouldn't happen for the project's own
    # table, but be safe) -> its own module.
    return macro.title()
  rest = macro[len(prefix):]
  # The project's own core pair: IGEEK_CORE / IGEEK_RELEASE (no world token)
  # -> the single "Core" module (the RL/agent core, first iGeek module after
  # ASCIICrabs). These are the bare kind names, not <WORLD>_CORE.
  if rest in ("CORE", "RELEASE"):
    return "Core"
  # Strip the trailing _CORE / _RELEASE seam-kind suffix (world seams).
  m = re.match(r"^(.+?)_(CORE|RELEASE)$", rest)
  if not m:
    return rest.title()
  world_token = m.group(1)
  if world_token == "":
    return "Core"
  return world_token


def RecoverWorldNames(project_dir):
  """Build an all-caps -> PascalCase map from the actual _Seams/<World>/ dirs,
  so IGEEK_MAZEWORLD_* recovers to MazeWorld exactly (not MazeWorld vs
  ArmWorld ambiguity)."""
  world_map = {}
  seams_dir = os.path.join(project_dir, "_Seams")
  if os.path.isdir(seams_dir):
    for entry in sorted(os.listdir(seams_dir)):
      full = os.path.join(seams_dir, entry)
      if os.path.isdir(full) and entry[0].isupper():
        world_map[entry.upper()] = entry  # MAZEWORLD -> MazeWorld
  return world_map


def WriteRootYaml(path, project, mode, modules, release_module):
  lines = [
    "# _Seams.yaml — ROOT: module list (build-up order).",
    "#",
    "# AUTO-BOOTSTRAPPED by SeamsToYaml.py from _Seams.h. Do not hand-edit the",
    "# ordering here without re-running SyncSeams.py; the module order defines",
    "# the contiguous global seam numbering.",
    "#",
    "# Regenerate: python3 Hatch/SyncSeams.py _Seams.yaml",
    "#",
    "project: " + project,
    "mode: " + mode,
    "release_module: " + release_module,
    "",
    "modules:",
  ]
  for m in modules:
    lines.append("  - name: " + m["name"])
    if m.get("folder"):
      lines.append("    folder: " + m["folder"])
    lines.append('    label: "' + m.get("label", m["name"] + " module") + '"')
  lines.append("")
  with open(path, "w") as f:
    f.write("\n".join(lines))


def DetectSeamFiles(folder_abs):
  """Detect a module's actual Core + Release seam file names. Returns
  (core_file, release_file) basenames, defaulting to 01.Core.hxx / 02.Release.hxx.
  A module may name its release file 02.Release.hxx (most worlds) or a later
  number (e.g. CardsWorld uses 07.Release.hxx after 02..06 intermediate files)."""
  core = release = None
  if os.path.isdir(folder_abs):
    for entry in sorted(os.listdir(folder_abs)):
      if entry.startswith("0") and entry.endswith(".Core.hxx") and core is None:
        core = entry
      if entry.endswith(".Release.hxx") and release is None:
        release = entry
  return (core or "01.Core.hxx", release or "02.Release.hxx")


def DetectEngineFiles(project_dir, module_name, seams_dir=None):
  """A world's engine is the set of <World>/*.hxx that form it, minus the
  test/train/demo/smoke/diag/probe files. Returns the INCLUDE-CHAIN TOPS: the
  engine .hxx files that are NOT included by another engine .hxx in the same
  world (pulling a top transitively pulls its deps). Most worlds have a single
  top (<World>.hxx); multi-file engines (ArmWorld) have a top that includes the
  rest (ArmGym.hxx -> ArmWorld.hxx -> ArmPhysics.hxx).

  Returns [] (no auto-engine) when the world's 01.Core.hxx already includes
  files from its own engine dir (../../<World>/...) — the engine is wired in
  the user-owned seam unit there (e.g. CarWorld pulls it via HeadlessTests.hxx),
  so the _Tests.hxx stub must not double-include it."""
  engine_dir = os.path.join(project_dir, module_name)
  if not os.path.isdir(engine_dir):
    return []
  # If the world's 01.Core.hxx already pulls from its engine dir, skip.
  if seams_dir:
    core = os.path.join(seams_dir, module_name, "01.Core.hxx")
    if os.path.exists(core):
      with open(core) as fh:
        body = fh.read()
      if re.search(r'#include\s+"\.\./\.\./' + re.escape(module_name) + r"/", body):
        return []
  skip = re.compile(r"(Tests|Train|Demo|Main|Smoke|Diag|Probe|Verify|Animator|main)\.hxx$", re.I)
  files = []
  for entry in sorted(os.listdir(engine_dir)):
    if not entry.endswith(".hxx") or skip.search(entry):
      continue
    # A .hxx with a main() is a standalone demo/program, not an engine
    # component (e.g. ArmWorld/armworld.hxx) — exclude it.
    try:
      with open(os.path.join(engine_dir, entry)) as fh:
        body = fh.read()
    except OSError:
      continue
    if re.search(r"\bmain\s*\(", body):
      continue
    files.append(entry)
  # Build an "included-by" relation: file A includes file B (same dir).
  includes = {}
  for f in files:
    path = os.path.join(engine_dir, f)
    try:
      with open(path) as fh:
        body = fh.read()
    except OSError:
        continue
    incs = set(re.findall(r'#include\s+"([^"]+\.hxx)"', body))
    includes[f] = {i for i in incs if i in files}
  # Tops: files not included by any other file in the set.
  included_by_someone = set()
  for f, incs in includes.items():
    included_by_someone.update(incs)
  tops = [f for f in files if f not in included_by_someone]
  # Only auto-wire a CLEAN SINGLE-top engine (one file that transitively pulls
  # the rest — e.g. MazeWorld.hxx, ArmWorld's ArmGym.hxx). Complex multi-file
  # worlds (PacWorld, Classical, ...) have many tops — their engine is wired in
  # the user-owned 01.Core.hxx instead.
  # Auto-wire the engine when it is unambiguous:
  #  - exactly one chain-top (e.g. MazeWorld.hxx), OR
  #  - <World>.hxx is present among the tops AND pulls its own header (the
  #    canonical impl that is self-sufficient, e.g. TypingWorld.hxx which does
  #    #include "TypingWorld.h" and defines the engine methods). Multi-file
  #    worlds with many independent tops (PacWorld, Classical) have neither, so
  #    their engine stays hand-wired in 01.Core.hxx.
  if len(tops) == 1:
    return [f"../../{module_name}/{tops[0]}"]
  primary = module_name + ".hxx"
  if primary in tops and os.path.exists(
      os.path.join(engine_dir, primary)):
    body = open(os.path.join(engine_dir, primary)).read()
    # Self-sufficient if it includes its own header or defines the engine body.
    if re.search(r'#include\s+"%s\.h"' % module_name, body):
      return [f"../../{module_name}/{primary}"]
  return []


def WriteModuleYaml(path, module_name, seams, folder_abs=None, engine=None):
  """Write a module's _Seams.yaml. seams: list of (macro, number, kind, dep).
  Emits a `file:` field per seam (its actual source file) so SyncSeams igeek
  mode can include the right files (e.g. CardsWorld's 07.Release.hxx), and an
  `engine:` field (the world's primary engine .hxx include) so the _Tests.hxx
  stub can pull the engine once."""
  core_file, release_file = ("01.Core.hxx", "02.Release.hxx")
  if folder_abs:
    core_file, release_file = DetectSeamFiles(folder_abs)
  lines = [
    "# _Seams.yaml — " + module_name + " module seams.",
    "# AUTO-BOOTSTRAPPED by SeamsToYaml.py from _Seams.h.",
    "# Re-generate: python3 Hatch/SyncSeams.py _Seams.yaml",
  ]
  if engine:
    if len(engine) == 1:
      lines.append("engine: " + engine[0])
    else:
      lines.append("engine: [" + ", ".join(engine) + "]")
  lines.append("seams:")
  for macro, _num, kind, dep in seams:
    lines.append("  - name: " + macro)
    if dep:
      lines.append("    depends: [" + dep + "]")
    seam_file = release_file if kind == "RELEASE" else core_file
    lines.append("    file: " + seam_file)
    lines.append('    label: "Testing ' + module_name + ' ' + kind.lower() + '..."')
  lines.append("")
  with open(path, "w") as f:
    f.write("\n".join(lines))


def Main():
  if len(sys.argv) < 2:
    print(__doc__)
    return 1
  seams_h = os.path.abspath(sys.argv[1])
  if not os.path.exists(seams_h):
    print(f"Error: {seams_h} not found")
    return 1
  # Project dir: the dir containing _Seams/ (seams_h is _Seams/_Seams.h).
  # _Seams.h lives in <project>/_Seams/, so project_dir = parent of _Seams/.
  seams_dir = os.path.dirname(seams_h)
  project_dir = os.path.dirname(seams_dir)
  project = sys.argv[2] if len(sys.argv) > 2 else os.path.basename(project_dir)
  mode = sys.argv[3] if len(sys.argv) > 3 else "igeek"
  base_offset = ISC(sys.argv[4]) if len(sys.argv) > 4 else 0

  seams, release = ParseSeamsH(seams_h)
  world_map = RecoverWorldNames(project_dir)

  print(f"Project:   {project}")
  print(f"Mode:      {mode}")
  print(f"Dir:       {project_dir}")
  print(f"Base:      {base_offset} (ASCIICrabs base seams preceding the project)")
  print(f"Seams:     {len(seams)}  (release = {release})")
  print()

  # Group seams into modules, preserving the table's order (module order =
  # first-appearance order of each module in the numbered table).
  modules = []          # [{name, folder, seams:[(macro,num,kind,dep)]}]
  by_name = {}
  for macro, num in seams:
    world_token = InferModule(macro, project)
    # Recover exact PascalCase from the _Seams/<World>/ dirs.
    module = world_map.get(world_token.upper(), world_token.title())
    kind = "RELEASE" if macro.endswith("_RELEASE") else "CORE"
    # Dependency: a RELEASE depends on its own module's CORE (contiguous pair);
    # a world's CORE depends on IGEEK_CORE (the RL core) when it's a world.
    dep = None
    if kind == "RELEASE":
      core_macro = macro.replace("_RELEASE", "_CORE")
      dep = core_macro
    elif module != "Core":
      # World CORE depends on the RL core.
      dep = project.upper() + "_CORE"
    if module not in by_name:
      # All modules (Core + worlds) live in _Seams/<Module>/ now — the Core
      # test files were moved from the _Seams/ root into _Seams/Core/.
      folder = f"_Seams/{module}"
      by_name[module] = {
          "name": module,
          "folder": folder,
          "seams": [],
          "label": module + " module",
      }
      modules.append(by_name[module])
    by_name[module]["seams"].append((macro, num, kind, dep))

  # The release module must be last. If the release seam's module isn't the
  # last module, move it to the end (the release is the terminal module).
  release_module_name = next(
      m["name"] for m in modules for (mm, _n, _k, _d) in m["seams"] if mm == release)
  for i, m in enumerate(modules):
    if m["name"] == release_module_name:
      modules.pop(i)
      modules.append(m)
      break

  # Write the root YAML + per-module YAMLs.
  root_path = os.path.join(project_dir, "_Seams.yaml")
  WriteRootYaml(root_path, project, mode, modules, release_module_name)
  print(f"Wrote {os.path.relpath(root_path, project_dir)}")
  for m in modules:
    folder = m.get("folder") or f"_Seams/{m['name']}"
    mpath = os.path.join(project_dir, folder, "_Seams.yaml")
    os.makedirs(os.path.dirname(mpath), exist_ok=True)
    engine = None if m["name"] == "Core" else DetectEngineFiles(
        project_dir, m["name"], seams_dir=os.path.join(project_dir, "_Seams"))
    WriteModuleYaml(mpath, m["name"], m["seams"],
                    folder_abs=os.path.join(project_dir, folder),
                    engine=engine)
    print(f"Wrote {os.path.relpath(mpath, project_dir)} "
          f"({len(m['seams'])} seams)")

  print()
  print("Module order (defines contiguous global numbering):")
  running = base_offset
  for m in modules:
    start = running + 1
    running += len(m["seams"])
    end = running
    marker = "  (release)" if m["name"] == release_module_name else ""
    print(f"  {m['name']:<16} {start}..{end}  [{len(m['seams'])} seams]{marker}")
  print()
  print("Next: python3 Hatch/SyncSeams.py " + os.path.relpath(root_path, project_dir))
  return 0


if __name__ == "__main__":
  sys.exit(Main())
