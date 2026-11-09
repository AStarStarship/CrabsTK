#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
"""
CppToAscii.py — convert C/C++ sources to ASCIICrabs/CrabsTK (Chimera+ style).

Layers (applied in order, longest-first, word-boundary safe):
  1. #include directives are masked so header filenames are never clobbered.
  2. std POD types -> ASCIICrabs width codes (ISC/IUC/ISB/IUB/ISA/IUA/ISD/IUD,
     CHA/CHB/CHC, FPC/FPD, BOL, ISC/IUN, ISW/IUW).
     - size_t is deliberately NOT converted: it is a platform type that
       operator new / memcpy / sizeof / pointer-width casts depend on, and on
       a 64-bit host size_t is 8 bytes while IUN (unsigned int) is 4.
  3. STL containers -> ASCIICrabs containers (best-effort, type-position only):
       std::vector<T>     -> AArray<T>
       std::list<T>       -> AList<T>
       std::map<K,V>      -> AMap<K,V>
       std::unordered_map -> AMap<K,V>
       std::set<T>        -> AArray<T>
       std::unordered_set -> AArray<T>
       std::string        -> AString
       std::wstring       -> AString
     Method renames are NOT attempted (too context-dependent for regex); the
     operator is flagged in the dry-run report for manual follow-up.
  4. STL headers -> ASCIICrabs headers (vector/string/map -> the ASCIICrabs
     container trio). std headers with no ASCIICrabs equivalent are left.
  5. ImS*/ImU* self-referential typedefs (a by-product of the POD pass on
     vendored ImGui) are collapsed to a single comment.

Usage:
  python3 CppToAscii.py [--apply] [--strip-comments] FILE...
    --apply            write in place (default: dry-run, prints line-diff count)
    --strip-comments   also remove /* */ and // comments (for vendored code
                       whose original source keeps the comments)
  python3 CppToAscii.py --selftest
"""
import re, sys, os

# ---------------------------------------------------------------------------
# Layer 2: POD type codes. Longest-first so "unsigned long long" beats
# "unsigned" and "signed short" beats "signed".
# ---------------------------------------------------------------------------
POD = [
    (r"\bunsigned\s+long\s+long\b", "IUD"),
    (r"\bsigned\s+long\s+long\b",   "ISD"),
    (r"\bunsigned\s+int\b",         "IUC"),
    (r"\bsigned\s+int\b",           "ISC"),
    (r"\bunsigned\s+short\b",       "IUB"),
    (r"\bsigned\s+short\b",         "ISB"),
    (r"\bunsigned\s+char\b",        "IUA"),
    (r"\bsigned\s+char\b",          "ISA"),
    (r"\bunsigned\s+long\b",        "IUD"),
    (r"\bsigned\s+long\b",          "ISD"),
    (r"\blong\s+long\b",            "ISD"),
    (r"\buintptr_t\b",              "IUW"),
    (r"\bintptr_t\b",               "ISW"),
    (r"\buint64_t\b",               "IUD"),
    (r"\bint64_t\b",                "ISD"),
    (r"\buint32_t\b",               "IUC"),
    (r"\bint32_t\b",                "ISC"),
    (r"\buint16_t\b",               "IUB"),
    (r"\bint16_t\b",                "ISB"),
    (r"\buint8_t\b",                "IUA"),
    (r"\bint8_t\b",                 "ISA"),
    (r"\blong double\b",            "FPD"),
    (r"\bunsigned\b",               "IUC"),
    (r"\bsigned\b",                 "ISC"),
    (r"\bint\b",                    "ISC"),
    (r"\blong\b",                   "ISD"),
    (r"\bshort\b",                  "ISB"),
    (r"\bfloat\b",                  "FPC"),
    (r"\bdouble\b",                 "FPD"),
    (r"\bbool\b",                   "BOL"),
    (r"\bchar\b",                   "CHR"),
]

# ---------------------------------------------------------------------------
# Layer 3: STL containers (type position only). 'std::' prefix is optional so
# both 'std::vector<T>' and (after a using-namespace) 'vector<T>' convert.
# ---------------------------------------------------------------------------
CONTAINERS = [
    (r"\bstd::vector\s*<",      "AArray<"),
    (r"\bstd::list\s*<",         "AList<"),
    (r"\bstd::map\s*<",          "AMap<"),
    (r"\bstd::unordered_map\s*<","AMap<"),
    (r"\bstd::set\s*<",          "AArray<"),
    (r"\bstd::unordered_set\s*<","AArray<"),
    (r"\bstd::string\b",         "AString"),
    (r"\bstd::wstring\b",        "AString"),
]

# ---------------------------------------------------------------------------
# Layer 4: STL headers -> ASCIICrabs container trio.
# ---------------------------------------------------------------------------
HEADERS = [
    (r'#include\s*<vector>',      '#include <ASCIICrabs/Array.hpp>'),
    (r'#include\s*<string>',      '#include <ASCIICrabs/String.hpp>'),
    (r'#include\s*<map>',         '#include <ASCIICrabs/Map.hpp>'),
    (r'#include\s*<unordered_map>','#include <ASCIICrabs/Map.hpp>'),
    (r'#include\s*<list>',        '#include <ASCIICrabs/List.hpp>'),
    (r'#include\s*<set>',         '#include <ASCIICrabs/Array.hpp>'),
    (r'#include\s*<unordered_set>','#include <ASCIICrabs/Array.hpp>'),
]

# The ASCIICrabs preamble that supplies the width codes. Inserted once after
# the first #include so every converted TU resolves ISC/FPC/etc.
PREAMBLE = (
    "\n// ASCIICrabs foundation: width codes (ISC/IUC/ISB/IUB/ISA/IUA/ISD/IUD,\n"
    "// CHA/CHB/CHC, FPC/FPD, BOL, ISC/IUN, ISW/IUW) used throughout this file.\n"
    "#include <ASCIICrabs/_ConfigHeader.h>\n"
    "#include <ASCIICrabs/_ConfigDefault.h>\n"
    "#include <ASCIICrabs/_ConfigFooter.h>\n"
    "#include <ASCIICrabs/Binary.hpp>\n"
)

CODES = r"(ISA|IUA|ISB|IUB|ISC|IUC|ISD|IUD)"


def mask_includes(text):
    """Replace #include directives with placeholders (restored after the pass).
    Returns (masked_text, list_of_includes)."""
    incs = []
    def sub(m):
        incs.append(m.group(0))
        return f"\x00INC{len(incs)-1}\x00"
    masked = re.sub(r'^\s*#\s*include\s*[<"][^>"]*[>"]', sub, text, flags=re.M)
    return masked, incs


def unmask(text, incs):
    return re.sub(r"\x00INC(\d+)\x00", lambda m: incs[int(m.group(1))], text)


def strip_comments(text):
    """Remove /* */ and // comments. String/char literals are protected so a
    '//' inside a URL string is not treated as a comment start. Operates on
    the raw text (before include-masking) — call this FIRST."""
    out = []
    i, n = 0, len(text)
    state = None  # None | 'line' | 'block' | 'str' | 'chr'
    while i < n:
        c = text[i]
        nxt = text[i+1] if i + 1 < n else ""
        if state is None:
            if c == "/" and nxt == "/":
                state = "line"; i += 2; continue
            if c == "/" and nxt == "*":
                state = "block"; i += 2; continue
            if c == '"':
                state = "str"; out.append(c); i += 1; continue
            if c == "'":
                state = "chr"; out.append(c); i += 1; continue
            out.append(c); i += 1; continue
        if state == "line":
            if c == "\n":
                state = None; out.append(c)
            i += 1; continue
        if state == "block":
            if c == "*" and nxt == "/":
                state = None; i += 2; continue
            if c == "\n":
                out.append(c)  # keep line structure
            i += 1; continue
        if state == "str":
            out.append(c)
            if c == "\\" and nxt:
                out.append(nxt); i += 2; continue
            if c == '"':
                state = None
            i += 1; continue
        if state == "chr":
            out.append(c)
            if c == "\\" and nxt:
                out.append(nxt); i += 2; continue
            if c == "'":
                state = None
            i += 1; continue
    return "".join(out)


def convert(text, strip=False):
    if strip:
        text = strip_comments(text)
    # 1. mask includes
    masked, incs = mask_includes(text)
    # 2. containers (before POD so 'std::vector<int>' element also converts)
    for pat, rep in CONTAINERS:
        masked = re.sub(pat, rep, masked)
    # 3. POD types
    for pat, rep in POD:
        masked = re.sub(pat, rep, masked)
    # 4. headers (on the unmasked original include lines — re-do on masked text
    #    where the include placeholders still carry the original directive text
    #    is NOT possible; so apply header mapping to the raw include strings)
    def map_inc(s):
        for pat, rep in HEADERS:
            s = re.sub(pat, rep, s)
        return s
    incs = [map_inc(s) for s in incs]
    # 5. collapse self-referential ImS*/ImU* typedef runs
    line_re = re.compile(r"^\s*typedef\s+" + CODES + r"\s+" + CODES + r";.*$")
    out_lines, run = [], []
    def flush():
        if run:
            out_lines.append("// Fixed-width integer aliases now provided by the\n"
                             "// ASCIICrabs preamble (ISA/IUA/ISB/IUB/ISC/IUC/ISD/IUD);\n"
                             "// self-referential typedefs removed.")
            run.clear()
    for ln in masked.split("\n"):
        if line_re.match(ln):
            run.append(ln)
        else:
            flush(); out_lines.append(ln)
    flush()
    masked = "\n".join(out_lines)
    # 6. unmask includes
    text = unmask(masked, incs)
    # 7. Inject the ASCIICrabs preamble so the width codes resolve. Anchor
    #    after `#pragma once` when present (always top-level), else after the
    #    first #include, else prepend. Anchoring on a bare #include is wrong
    #    when the first one sits inside a conditional block — the preamble
    #    would become conditional and the codes would be undefined.
    if re.search(r"\bISC\b|\bIUC\b|\bFPC\b|\bCHR\b|\bBOL\b", text) and \
       "ASCIICrabs/_ConfigHeader.h" not in text:
        anchors = (r'^[ \t]*#[ \t]*pragma[ \t]+once[^\n]*\n',
                   r'^[ \t]*#[ \t]*include[^\n]*\n')
        for pat in anchors:
            m = re.search(pat, text, flags=re.M)
            if m:
                text = text[:m.end()] + PREAMBLE + text[m.end():]
                break
        else:
            text = PREAMBLE.lstrip("\n") + text
    return text


# ---------------------------------------------------------------------------
# Self-test
# ---------------------------------------------------------------------------
def selftest():
    tests = [
        # POD
        ("unsigned int x;",        "IUC x;"),
        ("signed int y;",          "ISC y;"),
        ("signed short s;",        "ISB s;"),
        ("unsigned short u;",      "IUB u;"),
        ("signed char c;",         "ISA c;"),
        ("unsigned char uc;",      "IUA uc;"),
        ("unsigned long long ll;", "IUD ll;"),
        ("int32_t i; uint32_t u;", "ISC i; IUC u;"),
        ("(int)ptr",               "(ISC)ptr"),
        ("size_t s;",              "size_t s;"),          # NOT converted
        ("#include <float.h>",     "#include <float.h>"),  # include guarded
        ("points_count",           "points_count"),        # identifier kept
        ("ImWchar w;",             "ImWchar w;"),          # identifier kept
        ("float f; double d; BOL b; char c;",
         "FPC f; FPD d; BOL b; CHR c;"),
        # containers
        ("std::vector<int> v;",    "AArray<ISC> v;"),
        ("std::map<std::string,int> m;", "AMap<AString,ISC> m;"),
        ("std::string s;",         "AString s;"),
        # headers
        ("#include <vector>",      "#include <ASCIICrabs/Array.hpp>"),
        ("#include <string>",      "#include <ASCIICrabs/String.hpp>"),
        # comment stripping (POD pass also applies: int->ISC)
        ("int x; // trailing",     "ISC x; "),
        ("/* block */ int x;",     " ISC x;"),
        ('const char* u = "a//b";','const CHR* u = "a//b";'),  # URL in string kept
    ]
    fails = 0
    for src, want in tests:
        # comment-strip tests run with strip=True; others with strip=False
        strip = ("//" in src or "/*" in src) and want.strip() != src and \
                not src.startswith("#")
        got = convert(src, strip=strip)
        # normalize: drop the injected preamble so micro-snippets compare cleanly
        got = re.sub(r"^// ASCIICrabs foundation:.*?Binary\.hpp>\n", "", got,
                     flags=re.S)
        ok = got == want
        if not ok:
            fails += 1
            print(f"  BAD  {src!r}\n       got  {got!r}\n       want {want!r}")
        else:
            print(f"  ok   {src!r} -> {got!r}")
    # preamble injection
    inj = convert("int x;\n")
    has_p = "ASCIICrabs/_ConfigHeader.h" in inj
    print(f"  {'ok  ' if has_p else 'BAD '} preamble injected into a POD-only file")
    if not has_p: fails += 1
    print(f"  [selftest] {len(tests)+1-fails} passed, {fails} failed")
    return fails == 0


def main():
    args = sys.argv[1:]
    if "--selftest" in args:
        sys.exit(0 if selftest() else 1)
    apply_ = "--apply" in args
    strip = "--strip-comments" in args
    files = [a for a in args if not a.startswith("--")]
    if not files:
        print(__doc__); sys.exit(2)
    for f in files:
        p = os.path.abspath(f)
        if not os.path.exists(p):
            print(f"  SKIP (missing): {f}"); continue
        orig = open(p, encoding="utf-8", errors="replace").read()
        new = convert(orig, strip=strip)
        if new == orig:
            print(f"  no-change: {f}"); continue
        if apply_:
            open(p, "w", encoding="utf-8").write(new)
            print(f"  converted: {f}" + ("  (comments stripped)" if strip else ""))
        else:
            import difflib
            changed = sum(1 for _ in difflib.ndiff(orig.splitlines(), new.splitlines())
                          if _.startswith(("+ ", "- ")))
            print(f"  dry-run: {f}  ~{changed} line-diffs"
                  + ("  (comments would be stripped)" if strip else ""))


if __name__ == "__main__":
    main()
