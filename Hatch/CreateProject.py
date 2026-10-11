#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
#
# CreateProject.py -- Bootstrap a new project from the Hatch template.
#
# Usage:
#   python3 CreateProject.py <project_name> <output_dir> [--mode MODE]
#
# Modes:
#   crabstk     CrabsTK project (module-based, CT namespace, MPL headers)
#   asciicrabs  ASCII Crabs project (flat _Seams/, ProjectTest namespace)
#
# Examples:
#   python3 CreateProject.py FVM ~/FreedomGov/FVM --mode crabstk
#   python3 CreateProject.py MyApp ~/Projects/MyApp --mode asciicrabs
#
# What it does:
#   1. Copies template files from template/<mode>/ to the output directory.
#   2. Replaces PROJECT placeholders with the project name.
#   3. Writes seams.yaml with the project name and mode.
#   4. Runs SyncSeams.py to generate the initial seam files.
#   5. Prints next steps.

import os
import sys
import re
import shutil
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATE_DIR = os.path.join(SCRIPT_DIR, "template")


def CopyTree(src_dir, dst_dir):
  """Recursively copy all files from src_dir to dst_dir."""
  os.makedirs(dst_dir, exist_ok=True)
  for entry in sorted(os.listdir(src_dir)):
    src = os.path.join(src_dir, entry)
    dst = os.path.join(dst_dir, entry)
    if os.path.isdir(src):
      CopyTree(src, dst)
    else:
      shutil.copy2(src, dst)


def ReplaceInTree(root_dir, replacements):
  """Replace old->new in all text files under root_dir."""
  changed = []
  for dirpath, _dirs, files in os.walk(root_dir):
    for f in files:
      path = os.path.join(dirpath, f)
      try:
        with open(path, "r") as fh:
          content = fh.read()
      except (UnicodeDecodeError, IsADirectoryError):
        continue
      original = content
      for old, new in replacements:
        content = content.replace(old, new)
      if content != original:
        with open(path, "w") as fh:
          fh.write(content)
        changed.append(os.path.relpath(path, root_dir))
  return changed


def BuildSeamsYaml(project, mode):
  """Generates the initial seams.yaml content for a new project."""
  project_upper = project.upper()
  project_cap = project.capitalize()

  if mode == "crabstk":
    return f"""# Copyright AStarship <https://astarship.net>.
#
# seams.yaml -- single source of truth for the seam system.
#
# Edit this file, then run:
#   python3 SyncSeams.py
#
# SyncSeams.py regenerates the SEAM macros in _Seams/_Seams.h,
# the default SEAM in _Seams/_Config.h, and the TTestTree<> in _Main.cpp.
#
# To add a seam: append an entry to the list below.
# Never renumber existing seams.

project: {project_upper}
mode: crabstk
release: {project_upper}_RELEASE
project_dir: .

seams:
  - name: {project_upper}_CORE
    module: {project_cap}
    label: "Testing {project_cap} core..."

  - name: {project_upper}_RELEASE
    module: {project_cap}
    label: "Release"
"""
  else:
    return f"""# Copyright AStarship <https://astarship.net>.
#
# seams.yaml -- single source of truth for the seam system.
#
# Edit this file, then run:
#   python3 SyncSeams.py
#
# SyncSeams.py regenerates the SEAM macros in _Seams/_Seams.h,
# the default SEAM in _Seams/_Config.h, and the TTestTree<> in _Main.cpp.
#
# To add a seam: append an entry to the list below.
# Never renumber existing seams.

project: {project_upper}
mode: asciicrabs
release: {project_upper}_RELEASE
project_dir: .

seams:
  - name: {project_upper}_CORE
    file: 01.Core.hxx
    label: "Testing {project_cap} core..."

  - name: {project_upper}_RELEASE
    file: 02.Release.hxx
    label: "Release"
"""


def CreateProject(project_name, output_dir, mode):
  project_upper = project_name.upper()
  project_cap = project_name.capitalize()
  template_dir = os.path.join(TEMPLATE_DIR, mode)

  print(f"Creating {mode.upper()} project: {project_name}")
  print(f"Output directory: {output_dir}")
  print(f"Template: {template_dir}")
  print()

  if not os.path.exists(template_dir):
    print(f"Error: template directory not found: {template_dir}")
    return 1

  # Create output directories.
  os.makedirs(os.path.join(output_dir, "_Seams"), exist_ok=True)

  # Copy template files.
  CopyTree(template_dir, output_dir)

  # Build replacement list.
  # Order matters: longer/more-specific patterns first.
  replacements = [
    # CrabsTK mode: PROJECT_CORE, PROJECT_RELEASE, etc.
    ("PROJECT_CORE", f"{project_upper}_CORE"),
    ("PROJECT_RELEASE", f"{project_upper}_RELEASE"),
    # CrabsTK mode: PROJECTTest -> NameTest
    ("PROJECTTest", f"{project_name}Test"),
    # CrabsTK mode: ProjectTests -> NameTests
    ("ProjectTests", f"{project_name}Tests"),
    # CrabsTK mode: namespace Project -> namespace Name
    ("namespace Project {", f"namespace {project_cap} {{"),
    ("namespace ProjectTest {", f"namespace {project_cap}Test {{"),
    # CMakeLists: project (PROJECT)
    ("project (PROJECT)", f"project ({project_upper})"),
    ("add_executable(PROJECT _Main.cpp)", f"add_executable({project_upper} _Main.cpp)"),
    # Generic: whole-word PROJECT -> NAME
  ]

  changed = ReplaceInTree(output_dir, replacements)

  # Handle whole-word PROJECT replacement carefully (avoid double-replacing
  # already-substituted names).
  for dirpath, _dirs, files in os.walk(output_dir):
    for f in files:
      path = os.path.join(dirpath, f)
      try:
        with open(path, "r") as fh:
          content = fh.read()
      except (UnicodeDecodeError, IsADirectoryError):
        continue
      new_content = re.sub(r"\bPROJECT\b", project_upper, content)
      if new_content != content:
        with open(path, "w") as fh:
          fh.write(new_content)

  # Write seams.yaml (overwriting the template copy).
  seams_yaml = BuildSeamsYaml(project_name, mode)
  yaml_path = os.path.join(output_dir, "seams.yaml")
  with open(yaml_path, "w") as fh:
    fh.write(seams_yaml)

  # Copy SyncSeams.py to the project (so the user can run it locally).
  sync_src = os.path.join(SCRIPT_DIR, "SyncSeams.py")
  if os.path.exists(sync_src):
    shutil.copy2(sync_src, os.path.join(output_dir, "SyncSeams.py"))

  # Run SyncSeams.py to generate initial seam files.
  sync_script = os.path.join(output_dir, "SyncSeams.py")
  result = subprocess.run(
    [sys.executable, sync_script, yaml_path],
    capture_output=True, text=True,
  )
  if result.stdout:
    print(result.stdout, end="")
  if result.returncode != 0:
    print(f"Warning: SyncSeams.py returned {result.returncode}")
    if result.stderr:
      print(result.stderr, end="")
  else:
    print()

  # Print file list.
  print(f"Project created at: {output_dir}")
  print(f"\nFiles:")
  for dirpath, _dirs, files in os.walk(output_dir):
    for f in sorted(files):
      path = os.path.join(dirpath, f)
      rel = os.path.relpath(path, output_dir)
      print(f"  {rel} ({os.path.getsize(path)} bytes)")

  # Next steps.
  print(f"\nNext steps:")
  if mode == "crabstk":
    print(f"  1. Add your module source files next to _Package.hxx.")
    print(f"  2. Edit _Package.hxx to include your module headers.")
    print(f"  3. Edit _Seams/<Module>/00.Core.h to add your test code.")
    print(f"  4. Add more seams in seams.yaml, then run: python3 SyncSeams.py")
    print(f"  5. Build: cd {os.path.join(output_dir, '_Seams')} && cmake . && make")
  else:
    print(f"  1. Fix the ASCIICrabs include paths in _ConfigFooter.h,")
    print(f"     _ConfigHeader.h, _Package.hxx, and _Test.h.")
    print(f"  2. Add your source files next to _Package.hxx.")
    print(f"  3. Edit _Seams/01.Core.hxx to add your test code.")
    print(f"  4. Add more seams in seams.yaml, then run: python3 SyncSeams.py")
    print(f"  5. Build: cd {os.path.join(output_dir, '_Seams')} && cmake . && make")

  return 0


def ParseArgs(argv):
  if len(argv) < 3:
    script = os.path.basename(argv[0])
    print(f"Usage: python3 {script} <project_name> <output_dir> [--mode MODE]")
    print()
    print(f"Modes:")
    print(f"  crabstk     CrabsTK project (module-based, CT namespace)")
    print(f"  asciicrabs  ASCII Crabs project (flat _Seams, ProjectTest)")
    print()
    print(f"Examples:")
    print(f"  python3 {script} FVM ~/FreedomGov/FVM --mode crabstk")
    print(f"  python3 {script} MyApp ~/Projects/MyApp --mode asciicrabs")
    return None

  project_name = argv[1]
  output_dir = argv[2]
  mode = "crabstk"  # default

  args = argv[3:]
  for i, arg in enumerate(args):
    if arg == "--mode" and i + 1 < len(args):
      mode = args[i + 1].lower()
    elif arg.startswith("--mode="):
      mode = arg.split("=", 1)[1].lower()

  if mode not in ("crabstk", "asciicrabs"):
    print(f"Error: unknown mode '{mode}'. Use 'crabstk' or 'asciicrabs'.")
    return None

  return project_name, output_dir, mode


def Main():
  parsed = ParseArgs(sys.argv)
  if parsed is None:
    return 1
  project_name, output_dir, mode = parsed
  return CreateProject(project_name, output_dir, mode)


if __name__ == "__main__":
  sys.exit(Main())
