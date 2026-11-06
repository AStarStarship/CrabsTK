#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
#
# YamlToYaml.py -- (Re)generate the _Seams.yaml files from the seam test files.
#
# The seam test files (NN.Stem.hxx) are the source of truth for the seam
# SYSTEM (which seams exist, in what order, what their C++ macros are).
# The _Seams.yaml files are the editable surface for human-readable labels.
#
# What this tool does:
#   For each module folder under <project>/_Seams/<Mod>/:
#     1. Scans for numbered seam files: 01.Core.hxx ... NN.Release.hxx.
#     2. The seam NAME is the file stem: 01.Core.hxx -> Core, 03.Search.hxx -> Search.
#     3. The C++ MACRO is read from the file's leading "#if _Seam >= MACRO" gate
#        (the first _Seam comparison in the file). Files with no gate are
#        helpers -- they attach to the preceding gated seam (not their own seam).
#     4. Existing labels in the module _Seams.yaml are PRESERVED (matched by
#        seam name or file). Missing labels are auto-generated from the file stem.
#     5. Existing macros in the yaml are PRESERVED (the yaml is the editable
#        surface; the .hxx gate is the discovery fallback).
#   For the root _Seams.yaml:
#     6. One "- folder: _Seams/<Mod>" entry per module folder (in directory
#        order). Module name is auto-detected from the folder basename.
#     7. Existing "label:" descriptions are preserved (matched by folder).
#     8. Missing folders are added; phantom folders (no dir on disk) are dropped.
#
# Tag rules:
#   - "file:"   = this seam includes a seam test file (NN.Stem.hxx).
#   - "folder:" = this seam imports a sub-seam (a whole module folder).
#   - NEVER both file: and folder: in the same entry.
#   - All tags are UpperCamelCase: Project, Mode, ReleaseModule, DependsOn,
#     Modules, Folder, Label, Seams, Name, Macro, File, Depends.
#
# Usage:
#   python3 YamlToYaml.py <path-to-root-_Seams.yaml>
#
# The root yaml is <project>/_Seams/_Seams.yaml (or wherever the user put it).
# The tool reads the root yaml for project/mode/DependsOn/labels, scans the
# _Seams/ subfolders, and rewrites both the root yaml and every module yaml.
#
# Idempotent: running it twice produces the same output (labels preserved).

import os
import re
import sys


# ---------------------------------------------------------------------------
# YAML parsing (the subset used: top-level "Key: value", list items "- Key: val"
# with continuation "Key: val" lines, # comments, inline [a, b] lists).
# ---------------------------------------------------------------------------

def LoadYaml(path):
  """Parse a _Seams.yaml file. Returns a dict.
  Top-level scalars -> dict[key] = value.
  List keys (Modules:, Seams:) -> dict[key] = [ {k: v, ...}, ... ].
  """
  with open(path, "r") as f:
    lines = f.readlines()

  data = {}
  current = None      # current dict item
  list_key = None     # top-level key owning the open list

  for raw in lines:
    line = raw.rstrip("\n")
    stripped = line.strip()
    if not stripped or stripped.startswith("#"):
      continue
    # Top-level "Key: value" (no leading dash).
    if not line[0].isspace():
      key, _, val = stripped.partition(":")
      key = key.strip()
      val = val.strip()
      if isinstance(current, dict):
        data[list_key].append(current)
        current = None
      if val == "":
        # A list key (Modules: / Seams: / ...).
        list_key = key
        if key not in data:
          data[key] = []
      else:
        list_key = None
        data[key] = _ParseScalar(val)
      continue
    # List item "- Key: value" or "- scalar".
    if stripped.startswith("-"):
      if list_key is None:
        raise SystemExit(f"YamlToYaml: list item outside a list: {stripped}")
      rest = stripped[1:].strip()
      if rest and rest[0] in "'\"":
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
    # Continuation key inside a dict item (indented, "Key: value").
    if current is not None and ":" in stripped:
      k, _, v = stripped.partition(":")
      current[k.strip()] = _ParseScalar(v.strip())
      continue
  if isinstance(current, dict):
    data[list_key].append(current)
  return data


def _ParseScalar(val):
  if val == "" or val is None:
    return ""
  if val.startswith("[") and val.endswith("]"):
    inner = val[1:-1].strip()
    if not inner:
      return []
    return [_ParseScalar(x.strip()) for x in inner.split(",")]
  if (val.startswith('"') and val.endswith('"')) or (
      val.startswith("'") and val.endswith("'")):
    return val[1:-1]
  if val.lower() in ("true", "false"):
    return val.lower() == "true"
  try:
    return int(val)
  except ValueError:
    pass
  return val


# ---------------------------------------------------------------------------
# YAML emission (UpperCamelCase tags).
# ---------------------------------------------------------------------------

def EmitScalar(val):
  if isinstance(val, str):
    if re.search(r"[:#\[\]\"']", val) or val != val.strip():
      return '"' + val.replace('"', '\\"') + '"'
    return val
  if isinstance(val, list):
    return "[" + ", ".join(EmitScalar(x) for x in val) + "]"
  if isinstance(val, bool):
    return "true" if val else "false"
  return str(val)


def EmitRootYaml(project, mode, release_module, depends_on, modules):
  """modules: list of (folder, label). Order = directory scan order."""
  lines = [
    "project: " + project,
    "mode: " + mode,
    "releaseModule: " + release_module,
  ]
  if depends_on:
    lines.append("dependsOn: " + depends_on)
  lines.append("")
  lines.append("modules:")
  for folder, label in modules:
    lines.append("  - folder: " + folder)
    if label:
      lines.append("    label: " + EmitScalar(label))
  lines.append("")
  return "\n".join(lines)


def EmitModuleYaml(module_name, seams, engine=None):
  """seams: list of dicts {Name, Macro, File, Label, Depends(list)}."""
  lines = [
    "# " + module_name + " module seams.",
    "# AUTO-GENERATED by YamlToYaml.py from the seam test files.",
    "# Edit labels here, then re-run YamlToYaml.py (labels are preserved).",
    "# Regenerate: python3 Hatch/YamlToYaml.py <root _Seams.yaml>",
  ]
  if engine:
    if isinstance(engine, list) and len(engine) > 1:
      lines.append("engine: [" + ", ".join('"' + e + '"' for e in engine) + "]")
    elif isinstance(engine, str):
      lines.append("engine: " + engine)
  lines.append("seams:")
  for s in seams:
    lines.append("  - name: " + s["Name"])
    lines.append("    macro: " + s["Macro"])
    if s.get("File"):
      lines.append("    file: " + s["File"])
    if s.get("Depends"):
      lines.append("    depends: [" + ", ".join(s["Depends"]) + "]")
    lines.append("    label: " + EmitScalar(s["Label"]))
  lines.append("")
  return "\n".join(lines)


# ---------------------------------------------------------------------------
# Seam discovery from .hxx files.
# ---------------------------------------------------------------------------

def LeadingGateMacro(path):
  """Return the macro from the FIRST '#if _Seam <op> MACRO' in the file,
  or None if the file has no _Seam gate (a helper file)."""
  try:
    with open(path, "r") as f:
      body = f.read()
  except OSError:
    return None
  for line in body.splitlines():
    m = re.match(r"\s*#if\s+_Seam\s*(==|>=|>|!=)\s*([A-Za-z_]\w*)", line)
    if m:
      return m.group(2)
  return None


def StemFromFilename(fname):
  """01.Core.hxx -> Core, 03.Search.hxx -> Search, 04.PolicyBwd.hxx -> PolicyBwd."""
  parts = fname.split(".")
  # NN.Stem.hxx -> parts = [NN, Stem, hxx]
  if len(parts) >= 3:
    return ".".join(parts[1:-1])
  return parts[0] if parts else fname


def DiscoverSeams(module_dir, project, module_name):
  """Scan a module folder for numbered seam files. Returns a list of seam
  dicts (in file order).

  Every numbered file 01.Core.hxx through NN.Release.hxx is its own seam.
  The seam NAME is the file stem (01.Core.hxx -> Core, 03.Search.hxx -> Search).
  The C++ MACRO is read from the file's leading "#if _Seam <op> MACRO" gate
  (any operator: ==, >=, >, !=). If the file has no gate (a placeholder like
  Web/01.Core.hxx), the macro is left as None -- the caller fills it from the
  existing yaml or synthesizes it.
  """
  files = sorted(
      [f for f in os.listdir(module_dir)
       if re.match(r"^\d+\..*\.hxx$", f)],
      key=lambda f: (int(f.split(".")[0]), f))
  if not files:
    return []
  # Must start with a Core file and end with a Release file.
  if "Core" not in files[0]:
    return []
  if "Release" not in files[-1]:
    return []

  seams = []
  for f in files:
    stem = StemFromFilename(f)
    gate = LeadingGateMacro(os.path.join(module_dir, f))
    seams.append({
        "Name": stem,
        "Macro": gate,  # None if no gate; caller resolves.
        "File": f,
        "Label": None,
    })
  return seams


def LoadExistingModuleYaml(module_dir):
  """Read the existing module _Seams.yaml (if present) and return
  {seam_name: {Macro, Label, File}} for label/macro preservation."""
  path = os.path.join(module_dir, "_Seams.yaml")
  if not os.path.exists(path):
    return {}
  data = LoadYaml(path)
  seams = data.get("seams") or []
  out = {}
  for s in seams:
    if not isinstance(s, dict):
      continue
    name = s.get("name") or s.get("Name") or ""
    out[name] = {
      "Macro": s.get("macro") or s.get("Macro") or "",
      "Label": s.get("label") or s.get("Label") or "",
      "File": s.get("file") or s.get("File") or "",
    }
  return out


def AutoLabel(module_name, stem):
  """Generate a default label from the module name and seam stem."""
  return "Testing {} {}...".format(module_name, stem)


def ResolveModule(project_dir, folder, project, existing_labels):
  """Process one module folder. Returns (module_name, seams_list, engine)."""
  module_name = os.path.basename(folder)
  module_dir = os.path.join(project_dir, folder)
  if not os.path.isdir(module_dir):
    return module_name, [], None

  # Preserve existing labels + macros.
  existing = LoadExistingModuleYaml(module_dir)

  seams = DiscoverSeams(module_dir, project, module_name)
  if not seams:
    return module_name, [], None

  # Merge: preserve macro + label from existing yaml (matched by Name or File).
  for s in seams:
    # Match by Name first, then by File.
    prev = existing.get(s["Name"])
    if prev is None and s.get("File"):
      # Try matching by file (in case the name changed).
      for name, p in existing.items():
        if p["File"] == s["File"]:
          prev = p
          break
    if prev:
      if prev["Macro"]:
        s["Macro"] = prev["Macro"]
      if prev["Label"]:
        s["Label"] = prev["Label"]
    # If the file had no gate (helper) and no preserved macro, synthesize one
    # from the project + module + stem (never overwrite a discovered macro).
    if not s["Macro"]:
      s["Macro"] = "_{}{}{}".format(project, module_name, s["Name"])
    # Auto-label if still missing.
    if not s["Label"]:
      s["Label"] = AutoLabel(module_name, s["Name"])
    # Drop the multi-file "Files" key (yaml uses single "File").
    s.pop("Files", None)

  # Detect engine (top-level .hxx include, same logic as SeamsToYaml.py).
  engine = DetectEngine(project_dir, module_name)

  return module_name, seams, engine


def DetectEngine(project_dir, module_name):
  """Auto-detect the module's engine .hxx include (top of the include chain).
  Returns None when the engine is hand-wired in the seam test files."""
  engine_dir = os.path.join(project_dir, module_name)
  if not os.path.isdir(engine_dir):
    return None
  skip = re.compile(
      r"(Tests|Train|Demo|Main|Smoke|Diag|Probe|Verify|Animator|main)\.hxx$",
      re.I)
  files = []
  for entry in sorted(os.listdir(engine_dir)):
    if not entry.endswith(".hxx") or skip.search(entry):
      continue
    try:
      with open(os.path.join(engine_dir, entry)) as fh:
        body = fh.read()
    except OSError:
      continue
    if re.search(r"\bmain\s*\(", body):
      continue
    files.append(entry)
  if not files:
    return None
  # Build included-by relation.
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
  included_by_someone = set()
  for f, incs in includes.items():
    included_by_someone.update(incs)
  tops = [f for f in files if f not in included_by_someone]
  if len(tops) == 1:
    return "../../" + module_name + "/" + tops[0]
  primary = module_name + ".hxx"
  if primary in tops and os.path.exists(os.path.join(engine_dir, primary)):
    body = open(os.path.join(engine_dir, primary)).read()
    if re.search(r'#include\s+"%s\.h"' % module_name, body):
      return "../../" + module_name + "/" + primary
  return None


# ---------------------------------------------------------------------------
# Main.
# ---------------------------------------------------------------------------

def Main():
  if len(sys.argv) < 2:
    print(__doc__)
    return 1
  root_yaml = os.path.abspath(sys.argv[1])
  if not os.path.exists(root_yaml):
    print(f"Error: {root_yaml} not found")
    return 1

  # The root yaml lives in <project>/_Seams/_Seams.yaml (or the user may
  # point at a different location; the project dir is the parent of the
  # _Seams/ dir that contains the module subfolders).
  # Convention: the root yaml is inside _Seams/, so project_dir = parent.
  # But some projects put it at <project>/_Seams.yaml. Detect both.
  root_dir = os.path.dirname(root_yaml)
  if os.path.basename(root_dir) == "_Seams":
    project_dir = os.path.dirname(root_dir)
  else:
    project_dir = root_dir

  data = LoadYaml(root_yaml)
  project = str(data.get("project") or data.get("Project") or "PROJECT")
  mode = str(data.get("mode") or data.get("Mode") or "crabstk")
  release_module = str(data.get("releaseModule") or data.get("release_module")
                      or data.get("ReleaseModule") or "")
  depends_on = str(data.get("dependsOn") or data.get("DependsOn") or "")

  # Existing modules (folder -> label) from the root yaml, for preservation.
  existing_modules = data.get("modules") or data.get("Modules") or []
  existing_labels = {}
  for m in existing_modules:
    if isinstance(m, dict):
      folder = str(m.get("folder") or m.get("Folder") or "")
      label = str(m.get("label") or m.get("Label") or "")
      if folder:
        existing_labels[folder] = label

  # Scan _Seams/ for module subfolders.
  seams_dir = os.path.join(project_dir, "_Seams")
  if not os.path.isdir(seams_dir):
    # Fallback: the root yaml's own dir might be the seams dir.
    seams_dir = root_dir
  if not os.path.isdir(seams_dir):
    print(f"Error: _Seams/ dir not found under {project_dir}")
    return 1

  # Discover all module folders (uppercase-first subdirs).
  module_folders = []
  for entry in sorted(os.listdir(seams_dir)):
    full = os.path.join(seams_dir, entry)
    if not os.path.isdir(full):
      continue
    if not entry[0].isupper():
      continue
    module_folders.append("_Seams/" + entry)

  # Resolve each module (seams + engine + labels).
  resolved = []
  for folder in module_folders:
    module_name, seams, engine = ResolveModule(project_dir, folder, project,
                                                existing_labels)
    if not seams:
      print(f"  [skip] {module_name}: no valid seam files")
      continue
    resolved.append((module_name, folder, seams, engine,
                     existing_labels.get(folder, "")))

  if not resolved:
    print("Error: no valid seam modules found")
    return 1

  # The release module: the last module whose last seam is a Release.
  # (If release_module was specified in the root yaml, use that; otherwise
  # default to the last module.)
  if not release_module:
    release_module = resolved[-1][0]

  # Write module yamls.
  for module_name, folder, seams, engine, _label in resolved:
    module_dir = os.path.join(project_dir, folder)
    ypath = os.path.join(module_dir, "_Seams.yaml")
    content = EmitModuleYaml(module_name, seams, engine=engine)
    with open(ypath, "w") as f:
      f.write(content)
    print(f"Wrote {os.path.relpath(ypath, project_dir)} "
          f"({len(seams)} seams)")

  # Write root yaml.
  root_content = EmitRootYaml(project, mode, release_module, depends_on,
                              [(folder, existing_labels.get(folder, ""))
                               for _name, folder, _s, _e, _l in resolved])
  with open(root_yaml, "w") as f:
    f.write(root_content)
  print(f"Wrote {os.path.relpath(root_yaml, project_dir)} "
        f"({len(resolved)} modules)")

  # Print the seam table for verification.
  print(f"\nSeam table ({len(resolved)} modules):")
  for module_name, folder, seams, _engine, _label in resolved:
    print(f"  {module_name}:")
    for s in seams:
      print(f"    {s['Name']:<20} {s['Macro']:<28} {s['File']}")

  print(f"\nNext: python3 Hatch/SyncSeams.py {os.path.relpath(root_yaml, project_dir)}")
  return 0


if __name__ == "__main__":
  sys.exit(Main())
