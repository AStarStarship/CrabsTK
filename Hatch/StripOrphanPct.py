#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
# StripOrphanPct.py — remove orphaned % format specifiers from inside D_COUT
# string literals. After the printf->D_COUT mass replacement, some literals
# keep % specifiers with no matching value (the arg was dropped or is now a
# separate `<<`). Those print garbage; stripping the % token leaves the
# surrounding words and the real `<<` args still stream.
import re
import sys

spec = re.compile(r'%(?:\d+)?(?:\.\d+)?(?:[hl]{1,2}|z|ll)?[-+0 #]?(?:\d+)?[diuffeEgGscpxX]|%%')


def strip_line(ln):
  if 'D_COUT(' not in ln or '"' not in ln:
    return ln, 0
  n = 0

  def repl(m):
    nonlocal n
    s = m.group(0)  # the quoted string, e.g. "FORCES drive_speed=%.6f..."
    inner = s[1:-1]  # strip the surrounding quotes
    # Replace each orphaned specifier in the inner text.
    def sub_spec(mm):
      nonlocal n
      t = mm.group(0)
      if t == '%%':
        n += 1
        return '%'
      n += 1
      return ''
    new_inner = spec.sub(sub_spec, inner)
    return '"' + new_inner + '"'

  return re.sub(r'"(.*?)"', repl, ln), n


def main():
  total = 0
  for p in sys.argv[1:]:
    try:
      with open(p) as f:
        src = f.read()
    except FileNotFoundError:
      print(f"{p}: (missing, skipped)")
      continue
    out = []
    changed = 0
    for ln in src.split('\n'):
      ln2, n = strip_line(ln)
      out.append(ln2)
      changed += n
    new = '\n'.join(out)
    if new != src:
      with open(p, 'w') as f:
        f.write(new)
    total += changed
    print(f"{p}: {changed} orphaned %specifier(s) stripped")
  print(f"\nTotal: {total}")


if __name__ == '__main__':
  main()
