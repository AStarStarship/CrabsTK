// Copyright AStarship <https://astarship.net>.
#pragma once
#include "../_Config.h"
#if _Seam == _CrabsTKJSXCore
#include <ASCIICrabs/_Debug.h>
#else
#include <ASCIICrabs/_Release.h>
#endif
#include <ASCIICrabs/Test.h>
#include "Jsx.hxx"
#include "JsxTransform.hxx"
namespace CTKJSXTest {
using namespace ::_;
using namespace ::CTK;
  
// Shared caller-owned stack buffer (4 MiB). One TJsx is re-Init'd per call so
// every direct-tree test starts from a clean slate. Static so the 4 MiB region
// never lands on the call stack.
inline TJsx<ISC, ISC, DTB, CHR, CHR, IUN>& JsxRegrFresh() {
  static CHA buf[1 << 22];
  static TJsx<ISC, ISC, DTB, CHR, CHR, IUN> j;
  j.Init(buf, ISC(1 << 22), ISC(1 << 16));
  return j;
}

// Null-terminated substring search (no stdlib container). Returns the match or
// _NilP. `hay` must be NUL-terminated (all emission/render buffers here are).
inline const CHA* JsxRegrContains(const CHA* hay, const CHA* needle) {
  if (hay == _NilP || needle == _NilP) return _NilP;
  ISC nl = 0;
  while (needle[nl] != 0) nl++;
  if (nl == 0) return hay;
  for (ISC i = 0; hay[i] != 0; i++) {
    ISC k = 0;
    while (k < nl && hay[i + k] == needle[k]) k++;
    if (k == nl) return hay + i;
  }
  return _NilP;
}

inline ISC JsxRegrLen(const CHA* s) {
  ISC n = 0;
  while (s != _NilP && s[n] != 0) n++;
  return n;
}

// Full string equality over two NUL-terminated strings.
inline BOL JsxRegrEq(const CHA* a, const CHA* b) {
  return JsxRegrContains(a, b) != _NilP && JsxRegrLen(a) == JsxRegrLen(b);
}

// ============================================================================
// Batch 1 -- small, independent assertions against the current product headers
// ============================================================================

// (1) Transformer capacity / overflow: with a guarded buffer the transform must
// never write past the declared capacity, and when the output does not fit it
// must report a nonzero status (no "successful partial C++"). Cases: zero, one,
// one-short, exact, plus a headroom positive control.
inline const CHA* JsxRegr_CapacityGuard(const CHA* args) {
  CHA big[8192];
  TJsxTransformResult r{};
  ISC rc = JsxToCpp("<a/>", 0, big, sizeof(big), &r);
  if (rc < 0) return "could not measure <a/> emission on an 8192 buffer";
  ISC need = r.output_len;
  if (need < 64) return "measured <a/> emission is implausibly small (<64)";

  struct TCase { const CHA* name; ISC cap; ISC mode; };
  const TCase cases[] = {
    { "zero", 0, 0 },
    { "one", 1, 0 },
    { "one_short", need - 1, 0 },
    { "exact", need, 0 },
    { "headroom", need + 32, 1 },
  };
  for (const TCase& c : cases) {
    ISC cap = c.cap;
    ISC gsize = (cap > 0 ? cap : 1) + 8;
    if (gsize > 4096) gsize = 4096;  // keep the guard stack-allocable
    CHA guard[gsize];
    for (ISC i = 0; i < gsize; i++) guard[i] = (CHA)0x90;
    TJsxTransformResult rr{};
    ISC rc2 = JsxToCpp("<a/>", 0, guard, cap, &rr);
    // Invariant A: no byte at or beyond the declared capacity may change.
    for (ISC i = cap; i < gsize; i++)
      if (guard[i] != (CHA)0x90) {
        static CHA m[160];
        snprintf(m, sizeof(m), "%s: guard byte clobbered at index %d (got %d, want 144)",
                 c.name, i, (ISC)guard[i]);
        return m;
      }
    // Invariant B: status must reflect whether the output fit.
    if (c.mode == 0) {
      if (rc2 == 0) {
        static CHA m[180];
        snprintf(m, sizeof(m),
                 "%s: overflow (cap=%d < need=%d) reported success (rc=0, len=%d)",
                 c.name, cap, need, rr.output_len);
        return m;
      }
    } else {
      if (rc2 != 0 || rr.output_len != need) {
        static CHA m[160];
        snprintf(m, sizeof(m),
                 "%s: expected full emission (rc=0, len=%d), got rc=%d len=%d",
                 c.name, need, rc2, rr.output_len);
        return m;
      }
    }
  }
  return _NilP;
}

// (2) Grammar: malformed JSX must be REJECTED (nonzero rc or error_count > 0),
// not accepted with a silent success. The runner enforces a per-test timeout as
// a hang backstop.
inline const CHA* JsxRegr_GrammarReject(const CHA* args) {
  const CHA* bad[] = {
    "<a></b>",        // mismatched closing tag
    "<a>",            // unclosed opening tag
    "<a/>junk",       // trailing garbage after a self-closing tag
    "<a x=\"unterm",  // unterminated attribute string, no closing quote
    "<a x={v>",       // truncated (unclosed) dynamic binding brace
    "</a>",            // lone closing tag, no opening delimiter
    "<a x={1+2}>",    // unsupported arithmetic expression in a binding
  };
  for (const CHA* s : bad) {
    CHA out[8192];
    TJsxTransformResult r{};
    ISC rc = JsxToCpp(s, 0, out, sizeof(out), &r);
    if (rc == 0 && r.error_count == 0) {
      static CHA m[200];
      snprintf(m, sizeof(m), "malformed JSX accepted (rc=0, errors=0): %s", s);
      return m;
    }
  }
  return _NilP;
}

// (3) Multi-digit child references: a root with 11 children (el1..el11) must be
// linked to el10/el11 by name, never wrapped (el10 -> el0) and never to the
// root itself.
inline const CHA* JsxRegr_MultiDigitChildren(const CHA* args) {
  const CHA* src = "<r><a/><a/><a/><a/><a/><a/><a/><a/><a/><a/><a/></r>";
  CHA out[8192];
  TJsxTransformResult r{};
  ISC rc = JsxToCpp(src, 0, out, sizeof(out), &r);
  if (rc != 0) {
    static CHA m[160];
    snprintf(m, sizeof(m), "root with 11 children failed to parse (rc=%d, err=%s)",
             rc, r.first_error ? r.first_error : "nil");
    return m;
  }
  if (r.element_count != 12) {
    static CHA m[128];
    snprintf(m, sizeof(m), "element_count=%d, expected 12 (root + 11 children)",
             r.element_count);
    return m;
  }
  if (JsxRegrContains(out, "j.SetChild(el0, el0)") != _NilP)
    return "root el0 is linked to itself (child index wrapped)";
  if (JsxRegrContains(out, "j.SetChild(el0, el10)") == _NilP)
    return "child el10 not referenced by name (multi-digit index wrapped)";
  if (JsxRegrContains(out, "j.SetChild(el0, el11)") == _NilP)
    return "child el11 not referenced by name (multi-digit index wrapped)";
  return _NilP;
}

// (4) Direct runtime context sizing + resolution: build a vars=16 context
// (the sizing the reported bug exercises) and resolve a binding. If the context
// cannot be allocated, that IS the finding (context sizing bug); if it can, the
// dynamic prop must resolve to the bound value at render time.
inline const CHA* JsxRegr_ContextSizing(const CHA* args) {
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN>& j = JsxRegrFresh();
  ISC ctx = j.NewContext(ISC(16));
  if (ctx < 0)
    return "context vars=16: NewContext(16) failed to allocate (context sizing/init bug)";
  j.SetVar(ctx, "name", "Captain");
  ISC pos = j.Find(ctx, "name");
  if (pos < 0)
    return "context vars=16: 'name' not found after SetVar (context init bug)";
  ISC el = j.AddObject(ISC(16384), ISC(32), ISC(2048));
  if (el < 0) return "AddObject for the element failed";
  j.SetTag(el, "Welcome");
  j.SetDynProp(el, "name", "name");
  CHA out[256];
  ISC n = j.Render(out, ISC(256), ctx, el);
  const CHA* want = "<Welcome name=\"Captain\"></Welcome>";
  if (n != JsxRegrLen(want) || !JsxRegrEq(out, want)) {
    static CHA m[320];
    snprintf(m, sizeof(m), "vars=16 render mismatch: want \"%s\" got \"%s\"", want, out);
    return m;
  }
  return _NilP;
}

// (5) HTML escaping: user-controlled text must be escaped so a hostile value
// cannot inject markup. & < > in a text child render as entities.
inline const CHA* JsxRegr_HtmlEscape(const CHA* args) {
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN>& j = JsxRegrFresh();
  ISC ctx = j.NewContext(ISC(8));
  ISC el = j.AddObject(ISC(16384), ISC(32), ISC(2048));
  if (el < 0) return "AddObject failed";
  j.SetTag(el, "x");
  j.AddText(el, "A<B>&C");        // text containing < > &
  CHA out[256];
  for (ISC i = 0; i < 256; i++) out[i] = 0;
  ISC n = j.Render(out, ISC(256), ctx, el);
  const CHA* want = "<x>A&lt;B&gt;&amp;C</x>";
  if (n != JsxRegrLen(want) || !JsxRegrEq(out, want)) {
    static CHA m[320];
    snprintf(m, sizeof(m), "escape mismatch: want \"%s\" got \"%s\"", want, out);
    return m;
  }
  return _NilP;
}

// (5b) Semantic spaces: "Hello <b>bold</b> world" keeps BOTH spaces -- the space
// before <b> and the space after </b>. The transformer must not trim them.
// Asserted at the emission level (the AddText literals carry the spaces).
inline const CHA* JsxRegr_SpacePreserve(const CHA* args) {
  const CHA* src = "<r>Hello <b>bold</b> world</r>";
  CHA out[8192];
  TJsxTransformResult r{};
  ISC rc = JsxToCpp(src, 0, out, sizeof(out), &r);
  if (rc != 0) {
    static CHA m[160];
    snprintf(m, sizeof(m), "space-preservation fixture failed to parse (rc=%d)", rc);
    return m;
  }
  if (JsxRegrContains(out, "AddText(el0, (const CHR*)\"Hello \"") == _NilP)
    return "leading text 'Hello ' lost its trailing space (transformer trimmed it)";
  if (JsxRegrContains(out, "AddText(el0, (const CHR*)\" world\")") == _NilP)
    return "trailing text ' world' lost its leading space (transformer trimmed it)";
  return _NilP;
}

// (6) Storage: an in-place update of an existing scalar key must not corrupt a
// neighboring entry, must store the new value, and must not append a duplicate
// key. Checked at both the IUC (3-byte) and IUB (2-byte) widths, each on its own
// node so the widths do not interact.
inline const CHA* JsxRegr_ScalarUpdateIsolation(const CHA* args) {
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN>& j = JsxRegrFresh();

  // IUC in-place update isolation.
  ISC o1 = j.AddObject(ISC(16384), ISC(32), ISC(2048));
  if (o1 < 0) return "AddObject (IUC node) failed";
  j.Set(o1, "a", _IUC, IUW(IUC(100)));
  j.Set(o1, "b", _IUC, IUW(IUC(200)));
  if (j.Set(o1, "a", _IUC, IUW(IUC(101))) < 0) return "in-place IUC update failed";
  ISC fa = j.Find(o1, "a"), fb = j.Find(o1, "b");
  if (fa < 0 || fb < 0) return "keys lost after in-place IUC update";
  if (IUC(*j.ValuePtr<IUC>(o1, fa)) != IUC(101))
    return "in-place IUC update did not store the new value";
  if (IUC(*j.ValuePtr<IUC>(o1, fb)) != IUC(200))
    return "in-place IUC update of 'a' corrupted neighbor 'b'";
  if (j.Size(o1) != ISC(2))
    return "in-place IUC update appended a duplicate key (Size != 2)";

  // IUB in-place update isolation (the reported clobber is at this width).
  ISC o2 = j.AddObject(ISC(16384), ISC(32), ISC(2048));
  if (o2 < 0) return "AddObject (IUB node) failed";
  j.Set(o2, "hi", _IUB, IUW(IUB(100)));
  j.Set(o2, "lo", _IUB, IUW(IUB(200)));
  if (j.Set(o2, "lo", _IUB, IUW(IUB(201))) < 0) return "in-place IUB update failed";
  ISC fh = j.Find(o2, "hi"), fl = j.Find(o2, "lo");
  if (fh < 0 || fl < 0) return "keys lost after in-place IUB update";
  if (IUB(*j.ValuePtr<IUB>(o2, fh)) != IUB(100))
    return "in-place IUB update of 'lo' clobbered neighbor 'hi'";
  if (IUB(*j.ValuePtr<IUB>(o2, fl)) != IUB(201))
    return "in-place IUB update did not store the new value";
  if (j.Size(o2) != ISC(2))
    return "in-place IUB update appended a duplicate key (Size != 2)";
  return _NilP;
}

// (6b) Allocation exhaustion: fill the stack until AddObject fails, then further
// pushes must fail safely (negative index) without crashing or corrupting.
// Crash-prone -> the runner invokes this in its own subprocess.
inline const CHA* JsxRegr_ExhaustionFailsSafely(const CHA* args) {
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN>& j = JsxRegrFresh();
  ISC last = 0;
  ISC iters = 0;
  while (iters < 100000) {
    last = j.AddObject(ISC(4096), ISC(16), ISC(256));
    if (last < 0) break;  // exhausted
    iters++;
  }
  if (last >= 0) return "AddObject never failed on a 4 MiB stack (unexpected)";
  // After exhaustion, additional pushes must fail safely, not crash/corrupt.
  ISC s = j.PutString("x");
  if (s >= 0) return "PutString succeeded after the stack was exhausted";
  ISC more = j.AddObject(ISC(4096), ISC(16), ISC(256));
  if (more >= 0) return "AddObject succeeded after the stack was exhausted";
  return _NilP;
}

// (6c) Invalid handles: rendering / lookup / access with an out-of-range or
// negative element index must fail safely (0 / _NilP / -1), never crash.
// Crash-prone -> the runner invokes this in its own subprocess.
inline const CHA* JsxRegr_InvalidHandleFailsSafely(const CHA* args) {
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN>& j = JsxRegrFresh();
  ISC ctx = j.NewContext(ISC(8));
  ISC el = j.AddObject(ISC(16384), ISC(32), ISC(2048));
  if (el < 0) return "AddObject failed";
  j.SetTag(el, "x");
  j.AddText(el, "hi");
  CHA out[64];
  for (ISC i = 0; i < 64; i++) out[i] = 0;
  ISC bad = el + 999;  // an element index that does not exist
  if (j.Object(bad) != _NilP) return "Object(invalid index) returned non-_NilP";
  if (j.Find(bad, "x") >= 0) return "Find(invalid object index) returned >= 0";
  ISC n = j.Render(out, ISC(64), ctx, bad);
  if (n != 0) {
    static CHA m[128];
    snprintf(m, sizeof(m), "Render(invalid element index) returned %d, expected 0", n);
    return m;
  }
  // Negative index must also fail safely.
  if (j.Find(ISC(-1), "x") >= 0) return "Find(negative index) returned >= 0";
  if (j.Object(ISC(-1)) != _NilP) return "Object(negative index) returned non-_NilP";
  return _NilP;
}

// (6d) Zero render capacity: Render(out, 0, ...) must not write any byte and
// must report a safe result. The current renderer null-terminates out[0] even
// when cap=0, writing past a zero-size buffer.
// Crash-prone -> the runner invokes this in its own subprocess.
inline const CHA* JsxRegr_ZeroRenderCapacity(const CHA* args) {
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN>& j = JsxRegrFresh();
  ISC ctx = j.NewContext(ISC(8));
  ISC el = j.AddObject(ISC(16384), ISC(32), ISC(2048));
  if (el < 0) return "AddObject failed";
  j.SetTag(el, "x");
  j.AddText(el, "hi");
  // A guard region the renderer must not touch at cap=0.
  CHA guard[8];
  for (ISC i = 0; i < 8; i++) guard[i] = (CHA)0x90;
  ISC n = j.Render(guard, ISC(0), ctx, el);
  if (guard[0] != (CHA)0x90) {
    static CHA m[128];
    snprintf(m, sizeof(m), "Render(cap=0) wrote to the buffer (guard[0]=%d, want 144)",
             (ISC)guard[0]);
    return m;
  }
  if (n > 0) {
    static CHA m[128];
    snprintf(m, sizeof(m), "Render(cap=0) reported %d bytes written, expected 0", n);
    return m;
  }
  return _NilP;
}

// ===============================================================
// registry
// ===============================================================

struct TJsxRegrEntry {
  const CHA* name;
  const CHA* (*fn)(const CHA* args);
};

// The harness iterates this table and may run any single entry by name.
inline const TJsxRegrEntry* JsxRegrEntries() {
  static const TJsxRegrEntry e[] = {
    { "capacity_guard",            JsxRegr_CapacityGuard },
    { "grammar_reject",            JsxRegr_GrammarReject },
    { "multidigit_children",       JsxRegr_MultiDigitChildren },
    { "context_sizing",            JsxRegr_ContextSizing },
    { "html_escape",               JsxRegr_HtmlEscape },
    { "space_preserve",            JsxRegr_SpacePreserve },
    { "scalar_update_isolation",   JsxRegr_ScalarUpdateIsolation },
    { "exhaustion_fails_safely",   JsxRegr_ExhaustionFailsSafely },
    { "invalid_handle_fails_safely", JsxRegr_InvalidHandleFailsSafely },
    { "zero_render_capacity",      JsxRegr_ZeroRenderCapacity },
    { _NilP, _NilP },
  };
  return e;
}

inline const CHA* Core(const CHA* args) {
#if _Seam >= _CrabsTKJSXCore
  _ATestBegin;
  const CTK::TJsxRegrEntry* e = JsxRegrEntries();
  for (; e->name; ++e) {
    const CHA* err = e->fn(args);
    if (err) return err;
  }
#endif
  return _NilP;
}
}  //< namespace CTKJSXTest
