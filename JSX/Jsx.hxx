// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsJSXHXX
#define _CrabsJSXHXX
#include <_Config.h>
#include <ROut.hxx>
#include <String.hpp>
#include <List.hpp>
#include <Table.hpp>
#include <Dic.hpp>
#include <Loom.hpp>
#include <Book.hpp>
#if _Seam >= _CrabsTKJSXCore
namespace CTK {
namespace JSX {

// JSX engine — JSON-style typed values on the Crabs Stack, and the Star
// reducer that renders a JSX element tree.
//
// The Crabs Stack is a large POD-indexed ASCII List (TList): the Room's "Floor".
// Items are pushed by index. A JSON object is a TDic built IN PLACE in a
// TListAlloc'd stack region; that region is itself a stack tuple referenced by
// index, so an object "works just like JSON with more types" (the full ASCIICrabs
// type set, not just JSON's). The other hierarchical ASCIICrabs types — Book
// (string keys in a Loom) and Table (the ASCII hash table) — push the same way.
//
// Node encoding (a JSX element = a Dictionary on the stack):
//   "tag"    -> _IUC, blob index (the tag-name string)
//   "nprops" -> _IUC, prop count
//   "prop<i>"-> _IUC, blob index (the prop name)
//   "pv<i>"  -> _IUC, blob index (a static string value)
//            OR _IUD, blob index (a dynamic {var}: value = var-name blob)
//   "tcount" -> _IUC, text-child count
//   "t<i>"   -> _IUC, blob index (a text child)
//   A context is a Dictionary mapping var-name -> _IUC blob index. A dynamic
//   prop resolves at render time: pv(_IUD) -> var-name blob -> context var ->
//   value blob -> string.  This is "a custom Star reducer function with an
//   ASCII Table hash table" (the Dictionary keys are a TTable; TTableFind does
//   the by-name lookup).
//
// The engine is buffer-agnostic and stdlib-free: the caller supplies the RAM
// (the "just allocate 1GB, use the RAM" — a large buffer; the engine does not
// allocate itself). LST/DIC/TBL are ASCIICrabs macros of template-param names,
// so this file uses distinct alias names.

// A JSON-style value engine on a Crabs Stack.
//   ISZ  byte-offset width   (POD; 32-bit addresses a 4GB stack)
//   ISY  count/index width   (POD)
//   DT   type-code width
//   CHS  key char type, CHT  char type, HSH  hash type
template<typename ISZ = ISC, typename ISY = ISC, typename DT = DTB,
         typename CHS = CHR, typename CHT = CHR, typename HSH = IUN>
struct TJsx {
  using JsxLST = TList<ISZ, ISY, DT>;
  using JsxDic = TDic<CHS, CHT, ISZ, ISY, DT, HSH>;
  using JsxTbl = TTable<CHS, CHT, ISZ, ISY, HSH>;
  using JsxBook = TBook<CHS, CHT, ISZ, ISY, DT>;

  JsxLST* stack;  //< The Crabs Stack (the Floor), over a caller-owned buffer.
  ISZ bytes;      //< Total stack buffer size in bytes.

  // Init the stack over a caller-owned buffer (>= total_bytes).
  JsxLST* Init(void* buffer, ISZ total_bytes, ISY total) {
    stack = reinterpret_cast<JsxLST*>(buffer);
    stack->bytes = total_bytes;
    bytes = total_bytes;
    TListInit<ISZ, ISY, DT>(stack, total);
    return stack;
  }

  ISY Count() const { return ISY(stack->map.count); }

  // The stack offset of a tuple (its TListAlloc'd region).
  ISZ AllocOff(ISY idx) const { return TListValuesMap<ISZ, ISY, DT>(stack)[idx]; }

  // Push a scalar (number / char) onto the stack; returns the stack index.
  ISY Push(DT type, IUW value) {
    return TListInsert<ISZ, ISY, DT>(stack, type, value);
  }

  // Push a JSON object (a TDic built in place); returns the stack index.
  ISY AddObject(ISZ dic_bytes, ISY dic_total, ISZ size_keys) {
    ISY idx = TListAlloc<ISZ, ISY, DT>(stack, _CHA, dic_bytes);
    if (idx < 0) return idx;
    JsxDic* dic = Object(idx);
    if (dic == _NilP) return -1;
    TDicInit<CHS, CHT, ISZ, ISY, DT, HSH>(dic, dic_total, size_keys);
    return idx;
  }

  // Push a Book (string keys in a Loom) in place; returns the stack index.
  ISY AddBook(ISZ bok_bytes, ISY total, ISZ size_keys) {
    ISY idx = TListAlloc<ISZ, ISY, DT>(stack, _CHA, bok_bytes);
    if (idx < 0) return idx;
    JsxBook* bok = Book(idx);
    if (bok == _NilP) return -1;
    TBookInit<CHS, CHT, ISZ, ISY, DT>(bok, total, size_keys);
    return idx;
  }

  // Push a Table (the ASCII hash table) in place; returns the stack index.
  ISY AddTable(ISZ tbl_bytes, ISY total) {
    ISY idx = TListAlloc<ISZ, ISY, DT>(stack, _CHA, tbl_bytes);
    if (idx < 0) return idx;
    JsxTbl* tbl = Table(idx);
    if (tbl == _NilP) return -1;
    TTableInit<CHS, CHT, ISZ, ISY, HSH>(tbl, total);
    return idx;
  }

  // The dic/book/table at a stack index (_NilP if not a valid object).
  JsxDic* Object(ISY idx) const {
    if (idx < 0 || idx >= stack->map.count) return _NilP;
    return TRef<JsxDic>(stack, AllocOff(idx));
  }
  JsxBook* Book(ISY idx) const {
    if (idx < 0 || idx >= stack->map.count) return _NilP;
    return TRef<JsxBook>(stack, AllocOff(idx));
  }
  JsxTbl* Table(ISY idx) const {
    if (idx < 0 || idx >= stack->map.count) return _NilP;
    return TRef<JsxTbl>(stack, AllocOff(idx));
  }

  // Set a key in the object at obj_idx; returns the key position or error.
  // If the key already exists, updates the value in place (no duplicate append).
  ISY Set(ISY obj_idx, const CHS* key, DT type, IUW value) {
    JsxDic* dic = Object(obj_idx);
    if (dic == _NilP) return -AErrorInvalidIndex;
    JsxTbl* keys = TDicKeys<CHS, CHT, ISZ, ISY, DT, HSH>(dic);
    ISY pos = TTableFind<CHS, CHT, ISZ, ISY, HSH>(keys, key);
    if (pos >= 0) {
      // Key exists: update the value in place. Write the correct width
      // for the stored type (IUC=4 bytes, IUD=8 bytes) to avoid corrupting
      // adjacent entries.
      ISZ off = TDicValueOffset_NC<CHS, CHT, ISZ, ISY, DT, HSH>(dic, pos);
      DTB stored_type = TDicType_NC<CHS, CHT, ISZ, ISY, DT, HSH>(
        dic, ISY(dic->values.map.total), pos);
      if (IUC(stored_type) == _IUD) {
        *TRef<IUD>(dic, off) = IUD(IUW(value));
      } else {
        *TRef<IUC>(dic, off) = IUC(IUW(value));
      }
      return pos;
    }
    // Key absent: append a new entry.
    return TDicInsert<CHS, CHT, ISZ, ISY, DT, HSH>(dic, key, type, value);
  }

  // Key count in the object (excludes the index-0 empty init key).
  ISY Size(ISY obj_idx) const {
    JsxDic* dic = Object(obj_idx);
    return dic ? ISY(dic->values.map.count) - ISY(1) : ISY(0);
  }

  // Look up a key by name; returns the key position (>=0) or -1 if absent.
  ISY Find(ISY obj_idx, const CHS* key) const {
    JsxDic* dic = Object(obj_idx);
    if (dic == _NilP) return -1;
    JsxTbl* keys = TDicKeys<CHS, CHT, ISZ, ISY, DT, HSH>(dic);
    ISY pos = TTableFind<CHS, CHT, ISZ, ISY, HSH>(keys, key);
    return pos < 0 ? -1 : pos;
  }

  const CHS* KeyAt(ISY obj_idx, ISY pos) const {
    JsxDic* dic = Object(obj_idx);
    if (dic == _NilP) return _NilP;
    return TDicKey_NC<CHS, CHT, ISZ, ISY, DT, HSH>(dic, pos);
  }

  // The type code of the value at a key position (0-indexed key position).
  // Key position 0 is the empty init sentinel; real keys start at 1.
  DT TypeAt(ISY obj_idx, ISY pos) const {
    JsxDic* dic = Object(obj_idx);
    if (dic == _NilP || pos < 0) return DT(0);
    return TDicType_NC<CHS, CHT, ISZ, ISY, DT, HSH>(
      dic, ISY(dic->values.map.total), pos);
  }

  // The type code + value offset at a key position (0-indexed).
  TATypeValue<ISZ, DT> ValueAt(ISY obj_idx, ISY pos) const {
    JsxDic* dic = Object(obj_idx);
    if (dic == _NilP || pos < 0) return { DT(0), ISZ(0) };
    DT t = TypeAt(obj_idx, pos);
    ISZ off = TDicValueOffset_NC<CHS, CHT, ISZ, ISY, DT, HSH>(dic, pos);
    return { t, off };
  }

  // A pointer to the value at a key position (0-indexed, cast to POD type).
  template<typename T>
  T* ValuePtr(ISY obj_idx, ISY pos) const {
    JsxDic* dic = Object(obj_idx);
    if (dic == _NilP || pos < 0) return _NilP;
    ISZ off = TDicValueOffset_NC<CHS, CHT, ISZ, ISY, DT, HSH>(dic, pos);
    return TRef<T>(dic, off);
  }

  // ---- String blobs on the stack (tag names, prop values, text) ----
  // Layout: [size word (alloc size, set by TListAlloc)] [len word] [chars + 0].
  ISY PutString(const CHS* text) {
    ISZ n = 0;
    while (text[n] != 0) n++;
    ISZ alloc = 2 * ISZ(sizeof(ISZ)) + n + 1;
    ISY idx = TListAlloc<ISZ, ISY, DT>(stack, STR_, alloc);
    if (idx < 0) return idx;
    ISZ off = AllocOff(idx);
    TRef<ISZ>(stack, off + ISZ(sizeof(ISZ)))[0] = n;
    for (ISZ i = 0; i <= n; i++) TRef<CHS>(stack, off + 2 * ISZ(sizeof(ISZ)))[i] = text[i];
    return idx;
  }
  const CHS* GetStrPtr(ISY idx) const {
    return TRef<CHS>(stack, AllocOff(idx) + 2 * ISZ(sizeof(ISZ)));
  }
  ISZ GetStrLen(ISY idx) const {
    return TRef<ISZ>(stack, AllocOff(idx) + ISZ(sizeof(ISZ)))[0];
  }

  // ---- Element node helpers (a JSX element = a Dictionary on the stack) ----
  // Build a "prefix<i>" key into a single-threaded scratch buffer.
  const CHS* KeyFmt(const CHS* prefix, ISY i) const {
    static char buf[80];
    ISZ p = 0;
    while (prefix[p] != 0) buf[p++] = (char)prefix[p];
    char tmp[24];
    ISZ tn = 0;
    if (i == 0) tmp[tn++] = '0';
    else {
      ISY t = i;
      while (t > 0) { tmp[tn++] = (char)('0' + (t % 10)); t /= 10; }
    }
    for (ISZ k = tn; k > 0; k--) buf[p++] = tmp[k - 1];
    buf[p] = 0;
    return (const CHS*)buf;
  }

  ISY NodeInt(ISY obj, const CHS* key) const {
    ISY pos = Find(obj, key);
    if (pos < 0) return 0;
    return IUC(*ValuePtr<IUC>(obj, pos));
  }
  void NodeIntSet(ISY obj, const CHS* key, ISY v) {
    Set(obj, key, _IUC, IUW(IUC(v)));
  }
  void SetTag(ISY el, const CHS* tag) {
    Set(el, "tag", _IUC, IUW(IUC(PutString(tag))));
  }
  // Set a static string prop (name, value); prop index = current nprops.
  ISY SetProp(ISY el, const CHS* name, const CHS* value) {
    ISY i = NodeInt(el, "nprops");
    Set(el, KeyFmt("prop", i), _IUC, IUW(IUC(PutString(name))));
    Set(el, KeyFmt("pv", i), _IUC, IUW(IUC(PutString(value))));
    NodeIntSet(el, "nprops", i + 1);
    return i;
  }
  // Set a dynamic prop {var}; resolved from the context at render time.
  ISY SetDynProp(ISY el, const CHS* name, const CHS* var) {
    ISY i = NodeInt(el, "nprops");
    Set(el, KeyFmt("prop", i), _IUC, IUW(IUC(PutString(name))));
    Set(el, KeyFmt("pv", i), _IUD, IUW(IUC(PutString(var))));
    NodeIntSet(el, "nprops", i + 1);
    return i;
  }
  // Append a text child (unified child array: type _IUC = text blob).
  ISY AddText(ISY el, const CHS* text) {
    ISY i = NodeInt(el, "ccount");
    Set(el, KeyFmt("child", i), _IUC, IUW(IUC(PutString(text))));
    NodeIntSet(el, "ccount", i + 1);
    return i;
  }
  // Append a child element (unified child array: type _IUD = element index).
  ISY SetChild(ISY el, ISY child_idx) {
    ISY i = NodeInt(el, "ccount");
    Set(el, KeyFmt("child", i), _IUD, IUW(IUC(child_idx)));
    NodeIntSet(el, "ccount", i + 1);
    return i;
  }

  // ---- Context (var -> string blob) ----
  ISY NewContext(ISY vars) { return AddObject(ISZ(4096), vars, ISZ(256)); }
  void SetVar(ISY ctx, const CHS* var, const CHS* value) {
    Set(ctx, var, _IUC, IUW(IUC(PutString(value))));
  }
  // Resolve a dynamic-prop value (a var-name blob) to its string via the context.
  const CHS* CtxStr(ISY ctx, ISY var_blob, ISZ* out_len) const {
    const CHS* var = GetStrPtr(var_blob);
    ISY pos = Find(ctx, var);
    if (pos < 0) { if (out_len) *out_len = 0; return (const CHS*)""; }
    ISZ blob = IUC(*ValuePtr<IUC>(ctx, pos));
    if (out_len) *out_len = GetStrLen(blob);
    return GetStrPtr(blob);
  }

  // ---- The Star reducer: render an element node to a string ----
  // out is a caller-owned CHA buffer of >= cap. Returns the length written.
  ISZ Render(CHA* out, ISZ cap, ISY ctx, ISY el) const {
    ISZ n = 0;
    auto Emit = [&](const CHS* s, ISZ m) {
      for (ISZ k = 0; k < m && n < cap - 1; k++) out[n++] = s[k];
      out[n] = 0;
    };
    auto EmitC = [&](CHA c) { if (n < cap - 1) out[n++] = c; out[n] = 0; };
    auto EmitBlob = [&](ISY blob) { Emit(GetStrPtr(blob), GetStrLen(blob)); };

    ISY tagpos = Find(el, "tag");
    if (tagpos < 0) return 0;
    EmitC('<');
    EmitBlob(IUC(*ValuePtr<IUC>(el, tagpos)));
    ISY np = NodeInt(el, "nprops");
    for (ISY i = 0; i < np; i++) {
      ISY namepos = Find(el, KeyFmt("prop", i));
      ISY pvpos = Find(el, KeyFmt("pv", i));
      if (namepos < 0 || pvpos < 0) continue;
      EmitC(' ');
      EmitBlob(IUC(*ValuePtr<IUC>(el, namepos)));
      EmitC('=');
      EmitC('"');
      if (IUC(TypeAt(el, pvpos)) == _IUD) {
        ISZ L = 0;
        const CHS* s = CtxStr(ctx, IUC(*ValuePtr<IUC>(el, pvpos)), &L);
        Emit(s, L);
      } else {
        EmitBlob(IUC(*ValuePtr<IUC>(el, pvpos)));
      }
      EmitC('"');
    }
    EmitC('>');
    ISY nc = NodeInt(el, "ccount");
    for (ISY i = 0; i < nc; i++) {
      ISY cpos = Find(el, KeyFmt("child", i));
      if (cpos < 0) continue;
      if (IUC(TypeAt(el, cpos)) == _IUD) {
        // Child element: render recursively.
        ISY child = IUC(*ValuePtr<IUC>(el, cpos));
        Render(out + n, ISZ(cap - n), ctx, child);
        n += GetStrLen2(out + n);
      } else {
        // Text child: emit the blob.
        EmitBlob(IUC(*ValuePtr<IUC>(el, cpos)));
      }
    }
    EmitC('<');
    EmitC('/');
    EmitBlob(IUC(*ValuePtr<IUC>(el, tagpos)));
    EmitC('>');
    return n;
  }

  // Helper: length of a null-terminated string (for advancing after child render).
  static ISZ GetStrLen2(const CHA* s) {
    ISZ n = 0;
    while (s[n] != 0) n++;
    return n;
  }
};

// Self-test over a caller-provided buffer (stdlib-free). Returns 1 on full
// success. Exercises: the JSON object + heterogeneous scalars + by-name lookup;
// Book and Table push (the "hierarchical types all working"); and a hand-built
// JSX element rendered by the Star reducer (static + dynamic props + text).
inline int JsxSelfTest(void* buffer, ISC bytes, ISC dic_bytes = 65536) {
  TJsx<ISC, ISC, DTB, CHR, CHR, IUN> j;
  j.Init(buffer, bytes, ISC(1 << 16));
  int ok = 1;

  // (1) JSON object + heterogeneous scalars + by-name lookup.
  ISC obj = j.AddObject(dic_bytes, ISC(16), ISC(1024));
  if (obj < 0) return 0;
  j.Set(obj, (const CHR*)"alpha", _IUC, IUW(IUC(1)));
  j.Set(obj, (const CHR*)"beta", _CHA, IUW(CHA('x')));
  j.Set(obj, (const CHR*)"gamma", _IUD, IUW(IUD(123456789)));
  ok &= (j.Size(obj) == ISC(3));
  ISC pa = j.Find(obj, (const CHR*)"alpha");
  ISC pb = j.Find(obj, (const CHR*)"beta");
  ISC pg = j.Find(obj, (const CHR*)"gamma");
  ok &= (pa > 0 && pb > 0 && pg > 0);
  ok &= (IUC(*j.ValuePtr<IUC>(obj, pa)) == 1);
  ok &= ((char)*j.ValuePtr<CHA>(obj, pb) == 'x');
  ok &= (IUD(*j.ValuePtr<IUD>(obj, pg)) == IUD(123456789));
  ok &= (IUC(j.ValueAt(obj, pa).type) == _IUC);
  ok &= (IUC(j.ValueAt(obj, pg).type) == _IUD);
  ok &= (j.Find(obj, (const CHR*)"nope") < 0);

  // (2) Book push: string keys in a Loom, values by position.
  ISC bok = j.AddBook(ISC(8192), ISC(16), ISC(256));
  ok &= (bok >= 0);
  ISC bpos = TBookInsert<CHR, CHR, ISC, ISC, DTB>(
    j.Book(bok), (const CHR*)"title", (DTB)_IUC, IUW(IUC(7)));
  ok &= (bpos >= 0);
  ok &= (TBookCount<CHR, CHR, ISC, ISC, DTB>(j.Book(bok)) == ISC(1));
  ok &= (TBookKey_NC<CHR, CHR, ISC, ISC, DTB>(j.Book(bok), bpos) != _NilP);

  // (3) Table push: the ASCII hash table (Star-reducer backing).
  ISC tbl = j.AddTable(ISC(8192), ISC(64));
  ok &= (tbl >= 0);
  TTableAppend<CHR, CHR, ISC, ISC, IUN>(j.Table(tbl), (const CHR*)"route");
  TTableAppend<CHR, CHR, ISC, ISC, IUN>(j.Table(tbl), (const CHR*)"health");
  ok &= (TTableFind<CHR, CHR, ISC, ISC, IUN>(j.Table(tbl), (const CHR*)"route") >= 0);
  ok &= (TTableFind<CHR, CHR, ISC, ISC, IUN>(j.Table(tbl), (const CHR*)"health") >= 0);
  ok &= (TTableFind<CHR, CHR, ISC, ISC, IUN>(j.Table(tbl), (const CHR*)"missing") < 0);

  // (4) A JSX element, rendered by the Star reducer (static + dynamic + text).
  ISC ctx = j.NewContext(ISC(8));
  j.SetVar(ctx, (const CHR*)"name", (const CHR*)"Alice");
  ISC el = j.AddObject(ISC(16384), ISC(32), ISC(2048));
  j.SetTag(el, (const CHR*)"Welcome");
  j.SetDynProp(el, (const CHR*)"name", (const CHR*)"name");  // {name}
  j.SetProp(el, (const CHR*)"city", (const CHR*)"Springfield");
  j.AddText(el, (const CHR*)"Hello");
  CHA out[256];
  ISC n = j.Render(out, ISC(256), ctx, el);
  ok &= (n > 0);
  // Expected: <Welcome name="Alice" city="Springfield">Hello</Welcome>
  const CHA* want = "<Welcome name=\"Alice\" city=\"Springfield\">Hello</Welcome>";
  ISC wl = 0;
  while (want[wl] != 0) wl++;
  ok &= (n == wl);
  for (ISC k = 0; k < wl; k++) ok &= (out[k] == want[k]);

  return ok;
}

}  //< namespace JSX
}  //< namespace CTK
#endif  //< _Seam >= _CrabsTKJSXCore
#endif  //< _CrabsJSXHXX
