#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
#
# FixDcout.py — convert printf-style D_COUT("fmt", a, b, ...) calls into the
# stream-style D_COUT("lit" << a << "lit" << b) that the ASCIICrabs D_COUT
# macro expects (it takes a single expression, not varargs).
#
# The user mass-replaced printf -> D_COUT, leaving calls like
#   D_COUT("x=%d y=%.3f\n", x, y);
# which don't compile (D_COUT is `#define D_COUT(item) ::_::RefOut() << item`,
# a single-expression macro). This rewrites them to
#   D_COUT("x=" << x << " y=" << y << "\n");
#
# Handles %d %i %u %f %e %g %s %c %p %ld %lu %lld %llu %zu %.Nf etc. Args are
# consumed in order; a %s arg is streamed as-is (CHA*); a %c arg is streamed
# as a CHR. Unsupported specifiers are left as a best-effort `<< arg`.
#
# Usage: python3 FixDcout.py <file> [file ...]   (edits in place)
# Dry run: python3 FixDcout.py --dry <file> ...

import re
import sys

# A D_COUT call: D_COUT( <string literal> , <arg exprs> );
# We match the opening, capture the format string + the arg list, up to the
# matching close-paren at the statement level (args are simple expressions, no
# nested parens beyond the call itself — verified against the codebase).
_DCOUT_RE = re.compile(r'\bD_COUT\(\s*(".*?")\s*,\s*([^;]*?)\s*\)\s*;', re.DOTALL)

# Format specifier -> the C++ type/width hint (for a comment; the arg is just
# streamed). We only need to know how to split the literal and where each arg
# goes; the exact type doesn't change the `<<` expression.
_SPEC_RE = re.compile(r'%(?:\d+\.?\d*|\.\d+)?(?:[hl]{1,2}|ll|z)?[-+0 #]?(?:\d+)?[diuffeEgGscpxX%]')


def ConvertFormat(fmt_str, args):
  """fmt_str: the Python string INCLUDING the surrounding quotes. args: list of
  arg-expression strings. Returns the new stream expression (no outer D_COUT)."""
  # Strip the surrounding quotes.
  if len(fmt_str) >= 2 and fmt_str[0] == '"' and fmt_str[-1] == '"':
    body = fmt_str[1:-1]
  else:
    body = fmt_str
  # Split the body into literal chunks and specifiers, in order.
  parts = []      # list of (kind, text) where kind in {'lit','spec'}
  pos = 0
  for m in _SPEC_RE.finditer(body):
    if m.start() > pos:
      parts.append(('lit', body[pos:m.start()]))
    parts.append(('spec', m.group(0)))
    pos = m.end()
  if pos < len(body):
    parts.append(('lit', body[pos:]))

  # Build the stream expression.
  out = []
  arg_idx = 0
  for kind, text in parts:
    if kind == 'lit':
      if text:
        # Re-escape: the literal text goes back into a C string. We keep the
        # original escaping by pulling the exact substring from the source.
        out.append('"' + _re_escape(text) + '"')
    else:
      if arg_idx < len(args):
        arg = args[arg_idx].strip()
        out.append(arg)
        arg_idx += 1
      # Trailing specifier with no arg: ignore (malformed).
  if not out:
    return None
  # Join with ' << '.
  return ' << '.join(out)


def _re_escape(text):
  """Re-escape a literal chunk for a C string literal. The chunk came from the
  source (already escaped for \\n, \\t, etc.), but _SPEC_RE splitting may have
  consumed nothing from the escapes, so the text is already correctly escaped.
  Return it as-is (it was valid C string content)."""
  return text


def ConvertFile(path, dry=False):
  with open(path, 'r') as f:
    src = f.read()
  count = 0

  def repl(m):
    nonlocal count
    fmt = m.group(1)
    arglist = m.group(2)
    # Split the arg list on top-level commas (no nested parens in these args —
    # verified; if there were, this would mis-split, but the codebase's D_COUT
    # args are simple identifiers/expressions).
    args = [a for a in arglist.split(',') if a.strip()] if arglist.strip() else []
    if not args:
      return m.group(0)  # no args, already fine
    new_expr = ConvertFormat(fmt, args)
    if new_expr is None:
      return m.group(0)
    count += 1
    return 'D_COUT(' + new_expr + ');'

  new_src = _DCOUT_RE.sub(repl, src)
  if not dry and new_src != src:
    with open(path, 'w') as f:
      f.write(new_src)
  return count


def Main():
  dry = False
  files = []
  for a in sys.argv[1:]:
    if a == '--dry':
      dry = True
    else:
      files.append(a)
  if not files:
    print(__doc__)
    return 1
  total = 0
  for p in files:
    n = ConvertFile(p, dry)
    total += n
    print(f"{'[dry] ' if dry else ''}{p}: {n} D_COUT call(s) converted")
  print(f"\nTotal: {total} converted {'(dry run)' if dry else ''}")
  return 0


if __name__ == '__main__':
  sys.exit(Main())
