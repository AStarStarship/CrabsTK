// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_JSX_TRANSFORM_HXX
#define CRABS_JSX_TRANSFORM_HXX
#include <_Config.h>
#if defined(_CrabsTKJSXCore) && _Seam >= _CrabsTKJSXCore
namespace CTK {
namespace JSX {

// JSX-to-C++ transformer (Gate C). Parses a JSX source string and emits an
// includable C++ builder that constructs the element tree on the Crabs Stack
// using the TJsx engine.
//
// v1 grammar (see Agents/MaryReport.md §1):
//   <root>...</root>
//   <root attr="static" dyn={var}>...</root>
//   <root/>                         (self-closing)
//   <parent><child/><child2 a="v">text</child2></parent>
//
//   - Exactly one root. Whitespace before/after the root is consumed.
//   - Tag names: [a-zA-Z][a-zA-Z0-9:_-]*  (upper/lower case are just names).
//   - Attributes: name="static" or name={identifier} (context var).
//   - Text children: raw bytes; internal spaces preserved. A '>' inside a
//     text run is rejected (the v1 renderer does not HTML-escape; a stray
//     '>' would emit malformed HTML). Pure-whitespace inter-element gaps are
//     dropped; mixed text runs keep their internal spaces.
//   - Self-closing: <tag/>  or  <tag a="v"/>
//   - Bounded: source <= 65536 bytes, depth <= 64, elements <= 4096.
//
// Explicitly rejected (rc=-3, byte-offset diagnostic):
//   empty tag, mismatched close (<a></b>), unclosed element, self-close
//   junk, unterminated quote, unterminated brace, empty brace, non-identifier
//   in {}, spread, fragment, comment, trailing garbage, '>' in text,
//   depth/element/source limits.
//
// Emitted builder contract (MaryReport.md §2):
//   inline ISC Build<Name>(TJsx<ISC,ISC,DTB,CHR,CHR,IUC>* j, ISC ctx,
//                          ISC* out_root)
//   0 on success (*out_root = valid root), negative on failure
//   (*out_root = -1). No main, no new/delete, no printf, no static state,
//   no hardcoded user data. Caller owns the engine + context + arena.
//
// Stdlib-free. The emitter bounds every write and reserves the terminator.
// C++ literal escaping is done here (quotes, backslashes, control bytes);
// HTML escaping is the renderer's job (Illya's lane), NOT the transformer's.

// JSX transform result: emitted C++ source + diagnostics.
// (Existing fields preserved; error_offset is additive.)
struct TJsxTransformResult {
  CHA* output;            //< Emitted C++ source (caller-owned buffer).
  ISC output_len;         //< Length in bytes (NUL-terminated).
  ISC element_count;      //< Number of elements in the JSX tree.
  ISC error_count;        //< Number of parse errors (0 or 1: first is fatal).
  const CHA* first_error; //< Static message or "".
  ISC error_offset;       //< NEW: byte offset of the error in source, or -1.
};

// Internal parser/emitter state. Every emit is bounded; the emitter tracks
// out_n against out_cap and sets overflow_ (rc=-4) on any would-be write
// past the reserved terminator.
class TJsxParser {
  public:
  const CHA* src;
  ISC src_len;
  CHA* out;
  IUD out_cap;
  ISC out_n;
  ISC pos;
  ISC elem;
  ISC err;
  ISC err_off;
  const CHA* first_err;
  BOL overflow;
  ISC depth;

  TJsxParser(const CHA* s, ISC sl, CHA* o, IUD oc)
    : src(s), src_len(sl), out(o), out_cap(oc), out_n(0), pos(0), elem(0),
      err(0), err_off(-1), first_err(""), overflow(false), depth(0) {}

  // ---- Bounded emitters (reserve the terminator at out_cap-1) ----
  void emitc(CHA c) {
    if (overflow) return;
    if (out_n + 1 >= out_cap) { overflow = true; return; }
    out[out_n++] = c;
  }
  void emits(const CHA* s, ISC len) {
    if (overflow) return;
    for (ISC i = 0; i < len; i++) {
      if (out_n + 1 >= out_cap) { overflow = true; return; }
      out[out_n++] = s[i];
    }
  }
  void emit(const CHA* s) {
    ISC i = 0;
    while (s[i]) {
      if (out_n + 1 >= out_cap) { overflow = true; return; }
      out[out_n++] = s[i++];
    }
  }
  // Emit a C++ string literal body (the bytes between the quotes), escaping
  // for the C++ lexer. The caller emits the surrounding quotes.
  void emit_lit(const CHA* s, ISC len) {
    for (ISC i = 0; i < len; i++) {
      IUA b = (IUA)s[i];
      switch (b) {
        case '\\': emitc('\\'); emitc('\\'); break;
        case '"':  emitc('\\'); emitc('"');  break;
        case '\n': emitc('\\'); emitc('n');  break;
        case '\r': emitc('\\'); emitc('r');  break;
        case '\t': emitc('\\'); emitc('t');  break;
        default:
          if (b < 0x20 || b == 0x7f) {
            // \xNN for other control bytes and DEL.
            emitc('\\'); emitc('x');
            IUA hi = (IUA)((b >> 4) & 0xf);
            IUA lo = (IUA)(b & 0xf);
            emitc((CHA)(hi < 10 ? '0' + hi : 'a' + hi - 10));
            emitc((CHA)(lo < 10 ? '0' + lo : 'a' + lo - 10));
          } else {
            emitc((CHA)b);  // printable ASCII + raw UTF-8 bytes preserved.
          }
      }
    }
  }
  void indent() { for (ISC i = 0; i < depth; i++) emitc(' '); }

  // Build "el<N>" into buf (capacity >= 16). Returns length (no NUL).
  ISC var_name(ISC idx, CHA* buf) {
    buf[0] = 'e'; buf[1] = 'l';
    ISC vi = 2;
    CHA tmp[12];
    ISC tn = 0;
    ISC t = idx;
    if (t == 0) tmp[tn++] = '0';
    else { while (t > 0) { tmp[tn++] = (CHA)('0' + (t % 10)); t /= 10; } }
    for (ISC k = tn; k > 0; k--) buf[vi++] = tmp[k - 1];
    return vi;  // full numeric ID, no % 10.
  }

  void set_err(const CHA* msg) {
    if (err) return;  // first error is fatal; record its offset once.
    err++;
    err_off = pos;
    first_err = msg;
  }

  BOL is_tag_char(CHA c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '-' || c == '_' || c == ':';
  }
  BOL is_ident_start(CHA c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }
  BOL is_ident_char(CHA c) {
    return is_ident_start(c) || (c >= '0' && c <= '9');
  }
  void skip_ws() {
    while (pos < src_len && (src[pos] == ' ' || src[pos] == '\t' ||
           src[pos] == '\n' || src[pos] == '\r')) pos++;
  }

  // Parse a single element starting at '<'. Returns the element index, or -1
  // on error (err/err_off set). Reserves the index before children.
  ISC parse_element() {
    skip_ws();
    if (pos >= src_len || src[pos] != '<') { set_err("expected '<'"); return -1; }
    if (depth > 64) { set_err("nesting too deep"); return -1; }
    if (elem >= 4096) { set_err("too many elements"); return -1; }
    pos++;  // skip '<'

    // Reject fragment '<>' and comment '{/*' up front.
    if (pos < src_len && src[pos] == '>') { set_err("fragments not supported"); return -1; }
    if (pos + 2 < src_len && src[pos] == '{' && src[pos+1] == '/' && src[pos+2] == '*') {
      set_err("comments not supported"); return -1;
    }

    // Tag name.
    ISC tag_start = pos;
    while (pos < src_len && is_tag_char(src[pos])) pos++;
    ISC tag_len = pos - tag_start;
    if (tag_len == 0) { set_err("empty tag"); return -1; }

    ISC my_idx = elem;
    elem++;  // reserve before children.

    CHA vn[16];
    ISC vn_len = var_name(my_idx, vn);

    // Emit: ISC elN = j->AddObject(ISC(65536), ISC(64), ISC(4096));
    indent();
    emit("ISC ");
    emits(vn, vn_len);
    emit(" = j->AddObject(ISC(65536), ISC(64), ISC(4096));\n");
    // AddObject can fail (arena exhausted); return a unique negative code.
    indent();
    emit("if (");
    emits(vn, vn_len);
    emit(" < 0) return -");
    {
      ISC code = 2 + my_idx;  // -2, -3, -4, ... unique per node.
      CHA cb[12]; ISC cn = 0;
      ISC t = code;
      if (t == 0) cb[cn++] = '0';
      else { while (t > 0) { cb[cn++] = (CHA)('0' + (t % 10)); t /= 10; } }
      for (ISC k = cn; k > 0; k--) emitc(cb[k - 1]);
    }
    emit(";\n");

    // Emit: j->SetTag(elN, (const CHR*)"tag");
    indent();
    emit("j->SetTag(");
    emits(vn, vn_len);
    emit(", (const CHR*)\"");
    emit_lit(src + tag_start, tag_len);
    emit("\");\n");

    // Attributes.
    for (;;) {
      skip_ws();
      if (pos >= src_len) { set_err("unterminated tag"); return -1; }
      if (src[pos] == '>') break;
      if (src[pos] == '/') {
        // Self-closing: must be "/>".
        if (pos + 1 >= src_len || src[pos + 1] != '>') {
          set_err("expected '/>'"); return -1;
        }
        pos += 2;
        return my_idx;
      }
      if (src[pos] == '{' && pos + 1 < src_len && src[pos + 1] == '.') {
        set_err("spreads not supported"); return -1;
      }
      // Attribute name.
      if (!is_ident_start(src[pos])) { set_err("invalid attribute name"); return -1; }
      ISC attr_start = pos;
      while (pos < src_len && is_ident_char(src[pos])) pos++;
      ISC attr_len = pos - attr_start;
      skip_ws();
      if (pos >= src_len || src[pos] != '=') { set_err("expected '=' after attribute"); return -1; }
      pos++;  // skip '='
      skip_ws();
      if (pos >= src_len) { set_err("unterminated attribute value"); return -1; }
      if (src[pos] == '"') {
        // Static string value.
        pos++;  // skip opening quote.
        ISC val_start = pos;
        while (pos < src_len && src[pos] != '"') pos++;
        if (pos >= src_len) { set_err("unterminated string value"); return -1; }
        ISC val_len = pos - val_start;
        pos++;  // skip closing quote.
        indent();
        emit("j->SetProp(");
        emits(vn, vn_len);
        emit(", (const CHR*)\"");
        emit_lit(src + attr_start, attr_len);
        emit("\", (const CHR*)\"");
        emit_lit(src + val_start, val_len);
        emit("\");\n");
      } else if (src[pos] == '{') {
        // Dynamic value: {identifier}.
        pos++;  // skip '{'.
        ISC var_start = pos;
        while (pos < src_len && is_ident_char(src[pos])) pos++;
        ISC var_len = pos - var_start;
        if (var_len == 0) { set_err("empty binding"); return -1; }
        if (pos >= src_len || src[pos] != '}') {
          set_err("unterminated or non-identifier binding"); return -1;
        }
        pos++;  // skip '}'.
        indent();
        emit("j->SetDynProp(");
        emits(vn, vn_len);
        emit(", (const CHR*)\"");
        emit_lit(src + attr_start, attr_len);
        emit("\", (const CHR*)\"");
        emit_lit(src + var_start, var_len);
        emit("\");\n");
      } else {
        set_err("invalid attribute value (expected quote or brace)"); return -1;
      }
    }
    // Here src[pos] == '>'; it is a normal (non-self-closing) open.
    pos++;  // skip '>'.

    // Children.
    depth++;
    for (;;) {
      if (pos >= src_len) { set_err("unclosed element (EOF)"); return -1; }
      if (src[pos] == '<') {
        // Look ahead for '</' (close) vs '<' (nested element).
        if (pos + 1 < src_len && src[pos + 1] == '/') {
          // Closing tag: must match this element's name.
          pos += 2;  // skip '</'.
          ISC close_start = pos;
          while (pos < src_len && is_tag_char(src[pos])) pos++;
          ISC close_len = pos - close_start;
          if (close_len != tag_len) { set_err("mismatched closing tag"); return -1; }
          for (ISC k = 0; k < tag_len; k++) {
            if (src[close_start + k] != src[tag_start + k]) {
              set_err("mismatched closing tag"); return -1;
            }
          }
          if (pos >= src_len || src[pos] != '>') { set_err("unterminated closing tag"); return -1; }
          pos++;  // skip '>'.
          depth--;
          return my_idx;
        }
        // Nested element.
        ISC child_idx = parse_element();
        if (child_idx < 0) return -1;
        indent();
        emit("j->SetChild(");
        emits(vn, vn_len);
        emit(", ");
        {
          CHA cvn[16];
          ISC cvn_len = var_name(child_idx, cvn);
          emits(cvn, cvn_len);
        }
        emit(");\n");
      } else {
        // Text run: bytes until the next '<'.
        ISC text_start = pos;
        while (pos < src_len && src[pos] != '<') pos++;
        ISC text_len = pos - text_start;
        // Reject '>' in text (the v1 renderer does not HTML-escape).
        for (ISC k = 0; k < text_len; k++) {
          if (src[text_start + k] == '>') { set_err("unexpected '>' in text"); return -1; }
        }
        // Pure-whitespace run: drop it (inter-element gap). Mixed run: keep
        // internal spaces (semantic spaces preserved).
        BOL any_nonws = false;
        for (ISC k = 0; k < text_len; k++) {
          CHA c = src[text_start + k];
          if (c != ' ' && c != '\t' && c != '\n' && c != '\r') { any_nonws = true; break; }
        }
        if (any_nonws) {
          // Trim leading/trailing whitespace of the run but keep internal.
          ISC ts = text_start, te = text_start + text_len;
          while (ts < te && (src[ts] == ' ' || src[ts] == '\t' ||
                 src[ts] == '\n' || src[ts] == '\r')) ts++;
          while (te > ts && (src[te - 1] == ' ' || src[te - 1] == '\t' ||
                 src[te - 1] == '\n' || src[te - 1] == '\r')) te--;
          if (te > ts) {
            indent();
            emit("j->AddText(");
            emits(vn, vn_len);
            emit(", (const CHR*)\"");
            emit_lit(src + ts, te - ts);
            emit("\");\n");
          }
        }
      }
    }
  }
};

// Parse JSX source and emit C++ that builds the element tree as an
// includable builder function. The builder name is Build + <builder_name>
// (the caller derives <builder_name> as the CamelCase source stem).
//
// Returns:
//   0   success (out_result->output has the emitted C++ source).
//   -1  invalid arguments.
//   -2  source too long (> 65536 bytes).
//   -3  parse error (out_result->first_error + error_offset).
//   -4  output buffer overflow.
inline ISC JsxToCpp(const CHA* source, ISC source_len,
                    const CHA* builder_name,
                    CHA* out_buf, IUD out_buf_len,
                    TJsxTransformResult* out_result) {
  if (!source || !out_buf || !out_result || !builder_name) return -1;
  if (out_buf_len < 256) return -1;
  if (source_len == 0) {
    // NUL-terminated source: compute the length (bounded scan).
    ISC i = 0;
    while (source[i] && i < 65536) i++;
    source_len = i;
  }
  if (source_len > 65536) return -2;

  out_result->output = out_buf;
  out_result->output_len = 0;
  out_result->element_count = 0;
  out_result->error_count = 0;
  out_result->first_error = "";
  out_result->error_offset = -1;

  TJsxParser p(source, source_len, out_buf, out_buf_len);

  // Upper-cased builder name for the include guard.
  CHA upper[64];
  ISC upper_len = 0;
  {
    ISC i = 0;
    while (builder_name[i] && upper_len < 63) {
      CHA c = builder_name[i];
      if (c >= 'a' && c <= 'z') c = (CHA)(c - 'a' + 'A');
      upper[upper_len++] = c;
      i++;
    }
    upper[upper_len] = 0;
  }

  // Emit the includable builder header.
  p.emit("// Generated by JsxToCpp — do not edit.\n");
  p.emit("#pragma once\n");
  p.emit("#ifndef JSXGEN_");
  p.emits(upper, upper_len);
  p.emit("_HXX\n");
  p.emit("#define JSXGEN_");
  p.emits(upper, upper_len);
  p.emit("_HXX\n");
  p.emit("#include <_Seams.hxx>\n");
  p.emit("#include \"Jsx.hxx\"\n");
  p.emit("namespace CTK {\n");
  p.emit("inline ISC Build");
  p.emit(builder_name);
  p.emit("(TJsx<ISC, ISC, DTB, CHR, CHR, IUC>* j, ISC ctx, ISC* out_root) {\n");
  p.emit("  if (!j || !out_root) return -1;\n");
  p.emit("  *out_root = -1;\n");

  // Parse the single root element.
  p.depth = 2;  // two levels of indent for the body.
  ISC root_idx = p.parse_element();
  if (root_idx < 0) {
    // No partial code on failure: leave the buffer empty.
    out_buf[0] = 0;
    out_result->output_len = 0;
    out_result->error_count = 1;
    out_result->first_error = p.first_err;
    out_result->error_offset = p.err_off;
    return -3;
  }

  // After the root, only whitespace may remain.
  p.skip_ws();
  if (p.pos < p.src_len) {
    out_buf[0] = 0;
    out_result->output_len = 0;
    out_result->error_count = 1;
    out_result->first_error = "trailing content after root";
    out_result->error_offset = p.pos;
    return -3;
  }

  // Emit the success return: *out_root = el<root>; return 0;
  p.indent();
  p.emit("*out_root = ");
  {
    CHA rvn[16];
    ISC rvn_len = p.var_name(root_idx, rvn);
    p.emits(rvn, rvn_len);
  }
  p.emit(";\n");
  p.emit("  return 0;\n");
  p.emit("}\n");
  p.emit("}  // namespace CTK\n");
  p.emit("#endif  //< JSXGEN_");
  p.emits(upper, upper_len);
  p.emit("_HXX\n");

  if (p.overflow) {
    out_buf[0] = 0;
    out_result->output_len = 0;
    out_result->error_count = 1;
    out_result->first_error = "output buffer overflow";
    out_result->error_offset = -1;
    return -4;
  }

  p.out[p.out_n] = 0;
  out_result->output_len = p.out_n;
  out_result->element_count = p.elem;
  return 0;
}

}  //< namespace JSX
}  //< namespace CTK
#endif  //< _Seam >= _CrabsTKJSXCore
#endif  //< _CrabsJSXTransformHXX
