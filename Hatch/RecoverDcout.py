#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
#
# RecoverDcout.py — repair D_COUT calls whose string literal was emptied by a
# botched transform, leaving either:
#   (a) D_COUT( << a << b << );          -> prepend a leading label string
#   (b) D_COUT();                         -> (left for manual label; we add a
#                                             placeholder so it compiles)
#
# For (a) we restore a leading string so the call is a valid stream expression
# and the surviving `<<` args still print. The label is reconstructed from the
# first surviving arg when it's an identifier (e.g. `<< bike.roll` ->
# "roll="), else a generic tag. This restores compilation; the human label is
# approximate and the owner can polish the wording.
import re
import sys

# (a) D_COUT( << ... << );  with no leading string literal.
_MISSING_LEAD = re.compile(
    r'D_COUT\(\s*(<<[^\n]*?)\s*\)\s*;', re.DOTALL)
# (b) D_COUT();  fully empty.
_EMPTY = re.compile(r'D_COUT\(\s*\)\s*;')


def first_arg_label(argexpr):
  """Heuristic label from the first surviving arg expression."""
  a = argexpr.strip()
  # Strip a leading '<< ' and any cast.
  a = re.sub(r'^<<\s*', '', a)
  m = re.match(r'^\((?:double|int|IUD|ISC|IUC|FPC|FPD|unsigned long)\)\s*([\w.]+)', a)
  if m:
    name = m.group(1)
  else:
    m = re.match(r'^([\w.]+)', a)
    name = m.group(1) if m else 'value'
  # __FILE__/__LINE__/#condition are the CHECK-macro fail line -> special-case.
  if 'FILE' in a or 'LINE' in a or 'condition' in a:
    return 'FAIL '
  return name + '='


def fix_file(path, dry=False):
  with open(path) as f:
    src = f.read()
  fixed = 0

  def repl_lead(m):
    nonlocal fixed
    argexpr = m.group(1)
    label = first_arg_label(argexpr)
    fixed += 1
    return f'D_COUT("{label}" {argexpr});'

  out = _MISSING_LEAD.sub(repl_lead, src)
  out = _EMPTY.sub(lambda m: 'D_COUT("(label)");', out)
  if not dry and out != src:
    with open(path, 'w') as f:
      f.write(out)
  return fixed


def main():
  dry = '--dry' in sys.argv
  files = [a for a in sys.argv[1:] if a != '--dry']
  total = 0
  for p in files:
    n = fix_file(p, dry)
    total += n
    print(f"{'[dry] ' if dry else ''}{p}: {n} D_COUT call(s) repaired")
  print(f"\nTotal: {total} repaired {'(dry run)' if dry else ''}")


if __name__ == '__main__':
  main()
