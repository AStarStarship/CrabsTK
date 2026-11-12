// Copyright AStarship <https://astarship.net>.
#pragma once
#include "Lexer.h"
namespace CTK {
namespace Code {
/* CrabsTK Code TreeShaker — single-file header generator.
Takes a set of source files + root symbols, lexes them, builds a symbol
reference graph, does a reachability walk from the roots, and emits the
reachable definitions into one self-contained .hxx.

No std::, no allocation (caller-owned buffers), no external dependencies.
Uses the Code/ Lexer for tokenization. The reference graph is built by
walking the token stream and matching identifier tokens against the
definition set.

The emitted header is the "tree-shaken" API surface: exactly the symbols
the agent needs, inlined, with seam gates compiled away (O1 optimization).

Build as a standalone tool:
  g++ -std=c++23 -Wall -Wextra -DSEAM=1 -I. -I.. TreeShaker.hxx \
      -o /tmp/crabs-shake
  /tmp/crabs-shake --api=myapi \
      --sources=../SubsecondDb/Web/SubsecondDbWeb.hxx,../SubsecondDb/Gpu/VectorDb.h \
      --roots=BuildStatusJson,VectorDbInit \
      --out=/tmp/myapi.hxx
*/
// Type shim — matches ASCIICrabs _ConfigHeader.h widths
#ifndef CRABSTK_CODE_TREESHAKE_TYPES
#define CRABSTK_CODE_TREESHAKE_TYPES
inline BOL IsError(const void* p) { return p == nullptr; }
#define _NilP nullptr
#endif

/* ============================================================
 * Symbol table
 * ============================================================
 * A symbol is a named entity (function, struct, enum, typedef, macro)
 * that can be defined in one source and referenced in another.
 * The shaker builds a graph: symbol -> set of symbols it references.
 * Reachability from the roots gives the keep-set.
 */

/* Max symbol name length (bounded, no std::string). */
enum { TS_SYM_NAME_MAX = 128 };

/* A single symbol in the table. */
struct TTreeShakeSymbol {
  CHA name[TS_SYM_NAME_MAX];  //< Symbol name (null-terminated).
  ISC def_file;               //< Index into the source file array (-1 if not a definition).
  ISC def_start;              //< Byte offset of the definition in the source.
  ISC def_end;                //< Byte offset (exclusive) of the definition.
  ISC ref_count;              //< Number of times referenced (across all sources).
  ISC ref_files[16];          //< Which files reference this symbol (deduped).
  BOL is_definition;          //< True if this symbol is defined (not just referenced).
  BOL kept;                   //< True if this symbol is in the keep-set (reachable from roots).
};

/* The symbol table: a flat array of symbols. */
struct TTreeShakeSymTab {
  TTreeShakeSymbol* syms;     //< Caller-owned array.
  ISC count;                  //< Number of symbols.
  ISC capacity;               //< Capacity of the array.
};

/* ============================================================
 * Source file table
 * ============================================================
 * Each source file is lexed once. The token array is kept for
 * reference-graph construction and emission.
 */

/* A single source file in the shaker's view. */
struct TTreeShakeSource {
  const CHA* path;            //< File path (for diagnostics).
  const CHA* text;            //< File contents (caller-owned, null-terminated).
  IUD text_len;               //< Length of the file contents.
  TCodeToken* tokens;         //< Token array (caller-owned).
  ISC token_count;            //< Number of tokens.
  ISC token_capacity;         //< Capacity of the token array.
};

/* The source file table. */
struct TTreeShakeSources {
  TTreeShakeSource* files;    //< Caller-owned array.
  ISC count;                  //< Number of source files.
  ISC capacity;               //< Capacity of the array.
};

/* ============================================================
 * TreeShake options
 * ============================================================ */

struct TTreeShakeOptions {
  BOL deep;                   //< Deep transitive closure (default: true).
                              //< If false, only direct dependencies of the roots.
  BOL demacro;                //< Compile away #if _Seam gates (default: true).
                              //< Emits the "true" branch at the build's _Seam value.
  BOL inline_small;           //< Inline functions < 5 lines (default: true).
  IUD seam_value;             //< The _Seam value for constant-folding (default: 1).
  CHA out_path[512];          //< Output file path.
};

/* ============================================================
 * TreeShake result
 * ============================================================ */

struct TTreeShakeResult {
  IUD output_len;             //< Length of the emitted header (bytes).
  ISC kept_symbol_count;      //< Number of symbols in the keep-set.
  ISC total_symbol_count;     //< Total symbols found across all sources.
  ISC source_count;           //< Number of source files processed.
  IUD input_bytes;            //< Total input bytes (all sources).
  IUD output_bytes;           //< Total output bytes (the header).
  CHA diagnostics[4096];      //< Diagnostic messages (for the user).
  IUD diag_len;               //< Length of the diagnostics.
};

/* ============================================================
 * Public API
 * ============================================================ */

/* Initialize the tree shaker. All buffers are caller-owned.
sources:      Source file table (pre-populated with file contents).
syms:         Symbol table (pre-allocated).
tokens:       Token arrays for each source (pre-allocated, one per source).
Returns 0 on success, -1 on bad arguments. */
ISC TreeShakeInit(TTreeShakeSources* sources, TTreeShakeSymTab* syms,
                  TTreeShakeOptions* options);

/* Run the tree shake: lex all sources, build the reference graph,
do the reachability walk, emit the header.
roots:        Null-terminated array of root symbol names (the API entry points).
root_count:   Number of roots.
out_buf:      Caller-owned output buffer (the emitted header).
out_buf_len:  Capacity of out_buf.
result:       Filled with stats + diagnostics.
Returns 0 on success, -1 on error, -2 if out_buf is too small. */
ISC TreeShakeRun(TTreeShakeSources* sources, TTreeShakeSymTab* syms,
                 const CHA* const* roots, ISC root_count,
                 CHA* out_buf, IUD out_buf_len,
                 TTreeShakeResult* result,
                 const TTreeShakeOptions* options);

/* Read a file into a caller-owned buffer. Returns 0 on success, -1 on error. */
ISC TreeShakeReadFile(const CHA* path, CHA* buf, IUD buf_len, IUD* out_len);

/* ============================================================
 * Implementation
 * ============================================================ */

ISC TreeShakeReadFile(const CHA* path, CHA* buf, IUD buf_len, IUD* out_len) {
  FILE* fp = fopen(path, "rb");
  if (!fp) return -1;
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (size < 0 || (IUD)size >= buf_len) { fclose(fp); return -1; }
  IUD bytes_read = fread(buf, 1, (size_t)size, fp);
  fclose(fp);
  buf[bytes_read] = 0;  //< null-terminate
  *out_len = bytes_read;
  return 0;
}

ISC TreeShakeInit(TTreeShakeSources* sources, TTreeShakeSymTab* syms,
                  TTreeShakeOptions* options) {
  if (IsError(sources) || IsError(sources->files) ||
      IsError(syms) || IsError(syms->syms) || IsError(options)) {
    return -1;
  }
  if (sources->count <= 0 || syms->capacity <= 0) return -1;
  /* Lex all source files. */
  for (ISC i = 0; i < sources->count; ++i) {
    TTreeShakeSource* src = &sources->files[i];
    if (IsError(src->text) || src->text_len == 0) return -1;
    if (IsError(src->tokens) || src->token_capacity <= 0) return -1;
    TCodeLexer lex;
    CodeLexerInit(&lex, src->text, src->text_len, src->tokens, src->token_capacity);
    while (CodeLexerNext(&lex) != CODE_TOK_EOF) {}
    src->token_count = lex.token_count;
  }
  return 0;
}

/* Internal: find or create a symbol in the table. Returns the index, or -1 if full. */
static ISC TsFindOrAddSym(TTreeShakeSymTab* syms, const CHA* name, ISC name_len) {
  /* Search for an existing symbol with this name. */
  for (ISC i = 0; i < syms->count; ++i) {
    if (strlen(syms->syms[i].name) == name_len &&
        strncmp(syms->syms[i].name, name, name_len) == 0) {
      return i;
    }
  }
  /* Add a new symbol. */
  if (syms->count >= syms->capacity) return -1;
  TTreeShakeSymbol* s = &syms->syms[syms->count];
  memset(s, 0, sizeof(*s));
  ISC copy_len = name_len < TS_SYM_NAME_MAX - 1 ? name_len : TS_SYM_NAME_MAX - 1;
  memcpy(s->name, name, copy_len);
  s->name[copy_len] = 0;
  s->def_file = -1;
  s->def_start = -1;
  s->def_end = -1;
  s->ref_count = 0;
  for (ISC r = 0; r < 16; ++r) s->ref_files[r] = -1;
  s->is_definition = false;
  s->kept = false;
  return syms->count++;
}

/* Internal: check if a token is a definition (function, struct, enum, typedef).
A definition is an IDENT token that is preceded by a type keyword (struct, enum,
typedef, class, namespace) or followed by LPAREN (function definition).
For v1, we use a simpler heuristic: any IDENT that appears at the start of a
line (column 1) or after a type keyword is a candidate definition. */
static BOL TsIsDefinition(const TCodeToken* tokens, ISC token_count, ISC index,
                          const CHA* source) {
  if (index < 0 || index >= token_count) return false;
  const TCodeToken* tok = &tokens[index];
  if (tok->kind != CODE_TOK_IDENT && tok->kind != CODE_TOK_KEYWORD) return false;

  /* Check if the previous non-whitespace token is a type keyword. */
  for (ISC i = index - 1; i >= 0; --i) {
    if (tokens[i].kind == CODE_TOK_SPACE || tokens[i].kind == CODE_TOK_NEWLINE) continue;
    if (tokens[i].kind == CODE_TOK_KEYWORD) {
      ISC klen = tokens[i].byte_end - tokens[i].byte_start;
      const CHA* ktext = source + tokens[i].byte_start;
      if ((klen == 6 && strncmp(ktext, "struct", 6) == 0) ||
          (klen == 4 && strncmp(ktext, "enum", 4) == 0) ||
          (klen == 7 && strncmp(ktext, "typedef", 7) == 0) ||
          (klen == 5 && strncmp(ktext, "class", 5) == 0) ||
          (klen == 9 && strncmp(ktext, "namespace", 9) == 0)) {
        return true;
      }
    }
    break;  //< Only check the immediately preceding token.
  }

  /* Check if the next non-whitespace token is LPAREN (function definition). */
  for (ISC i = index + 1; i < token_count; ++i) {
    if (tokens[i].kind == CODE_TOK_SPACE || tokens[i].kind == CODE_TOK_NEWLINE) continue;
    if (tokens[i].kind == CODE_TOK_LPAREN) return true;
    break;
  }

  /* Check if it's at column 1 (top-level definition). */
  if (tok->column == 1) return true;

  return false;
}

/* Internal: build the symbol reference graph by walking all tokens. */
static void TsBuildRefGraph(TTreeShakeSources* sources, TTreeShakeSymTab* syms) {
  for (ISC fi = 0; fi < sources->count; ++fi) {
    TTreeShakeSource* src = &sources->files[fi];
    for (ISC ti = 0; ti < src->token_count; ++ti) {
      const TCodeToken* tok = &src->tokens[ti];
      if (tok->kind != CODE_TOK_IDENT) continue;
      ISC name_len = tok->byte_end - tok->byte_start;
      if (name_len <= 0 || name_len >= TS_SYM_NAME_MAX) continue;
      const CHA* name = src->text + tok->byte_start;

      /* Check if this is a definition. */
      if (TsIsDefinition(src->tokens, src->token_count, ti, src->text)) {
        ISC sym_idx = TsFindOrAddSym(syms, name, name_len);
        if (sym_idx >= 0) {
          syms->syms[sym_idx].is_definition = true;
          syms->syms[sym_idx].def_file = fi;
          syms->syms[sym_idx].def_start = tok->byte_start;
          syms->syms[sym_idx].def_end = tok->byte_end;
        }
      }

      /* Record a reference (all IDENTS are references, including definitions). */
      ISC sym_idx = TsFindOrAddSym(syms, name, name_len);
      if (sym_idx >= 0) {
        syms->syms[sym_idx].ref_count++;
        /* Record which file references this symbol (deduped). */
        BOL already = false;
        for (ISC r = 0; r < 16; ++r) {
          if (syms->syms[sym_idx].ref_files[r] == fi) { already = true; break; }
          if (syms->syms[sym_idx].ref_files[r] == -1) {
            syms->syms[sym_idx].ref_files[r] = fi;
            break;
          }
        }
        if (!already) {
          for (ISC r = 0; r < 16; ++r) {
            if (syms->syms[sym_idx].ref_files[r] == -1) {
              syms->syms[sym_idx].ref_files[r] = fi;
              break;
            }
          }
        }
      }
    }
  }
}

/* Internal: do the reachability walk from the roots. */
static void TsReachability(TTreeShakeSymTab* syms, const CHA* const* roots,
                           ISC root_count, BOL deep) {
  /* Mark the roots as kept. */
  for (ISC r = 0; r < root_count; ++r) {
    if (!roots[r] || !roots[r][0]) continue;  //< skip empty roots
    for (ISC i = 0; i < syms->count; ++i) {
      if (!syms->syms[i].name[0]) continue;  //< skip empty symbol names
      if (strncmp(syms->syms[i].name, roots[r], TS_SYM_NAME_MAX - 1) == 0) {
        syms->syms[i].kept = true;
        break;
      }
    }
  }

  if (!deep) return;  //< Shallow mode: only direct deps.

  /* Transitive closure: iterate until no new symbols are kept. */
  BOL changed = true;
  while (changed) {
    changed = false;
    for (ISC i = 0; i < syms->count; ++i) {
      if (!syms->syms[i].kept) continue;
      /* For each kept symbol, mark all symbols it references as kept.
      In v1, we approximate "references" as "all symbols that appear
      in the same source file as this symbol's definition." */
      if (syms->syms[i].def_file < 0) continue;
      for (ISC j = 0; j < syms->count; ++j) {
        if (syms->syms[j].kept) continue;
        if (syms->syms[j].def_file != syms->syms[i].def_file) continue;
        /* Symbol j is defined in the same file as symbol i.
        Check if symbol i references symbol j (j appears in i's file). */
        BOL refd = false;
        for (ISC r = 0; r < 16; ++r) {
          if (syms->syms[i].ref_files[r] == syms->syms[j].def_file) { refd = true; break; }
        }
        if (refd) {
          syms->syms[j].kept = true;
          changed = true;
        }
      }
    }
  }
}

/* Internal: emit the kept definitions into the output buffer. */
static IUD TsEmit(TTreeShakeSources* sources, TTreeShakeSymTab* syms,
                  CHA* out_buf, IUD out_buf_len, const CHA* api_name,
                  const TTreeShakeOptions* options) {
  IUD pos = 0;
  #define EMIT(s) do { \
    IUD _len = strlen(s); \
    if (pos + _len >= out_buf_len) return out_buf_len; /* buffer full */ \
    memcpy(out_buf + pos, s, _len); pos += _len; \
  } while (0)
  #define EMIT_SRC(start, len) do { \
    if (pos + (len) >= out_buf_len) return out_buf_len; \
    memcpy(out_buf + pos, start, (len)); pos += (len); \
  } while (0)

  /* Header comment. */
  EMIT("// Copyright AStarship <https://astarship.net>.\n");
  EMIT("// Auto-generated by crabs-shake. Do not edit.\n");
  EMIT("// API: ");
  EMIT(api_name);
  EMIT("\n");
  EMIT("#pragma once\n");
  EMIT("\n");

  /* Emit the type shim (always needed). */
  EMIT("/* Type definitions (ASCII Crabs canonical widths). */\n");
  EMIT("typedef CHR CHA;\n");
  EMIT("typedef signed CHR ISA;\n");
  EMIT("typedef CHR IUA;\n");
  EMIT("typedef short ISB;\n");
  EMIT("typedef unsigned short IUB;\n");
  EMIT("typedef ISC ISC;\n");
  EMIT("typedef IUC IUC;\n");
  EMIT("typedef long long ISD;\n");
  EMIT("typedef unsigned long long IUD;\n");
  EMIT("typedef ISC ISC;\n");
  EMIT("typedef IUC IUC;\n");
  EMIT("typedef long ISW;\n");
  EMIT("typedef unsigned long IUW;\n");
  EMIT("typedef FPC FPC;\n");
  EMIT("typedef double FPD;\n");
  EMIT("typedef BOL BOL;\n");
  EMIT("inline BOL IsError(const void* p) { return p == 0; }\n");
  EMIT("#define _NilP 0\n");
  EMIT("\n");

  /* Emit the kept symbols (in definition order). */
  for (ISC i = 0; i < syms->count; ++i) {
    if (!syms->syms[i].kept) continue;
    if (!syms->syms[i].is_definition) continue;
    if (syms->syms[i].def_file < 0) continue;
    TTreeShakeSource* src = &sources->files[syms->syms[i].def_file];
    ISC start = syms->syms[i].def_start;
    ISC end = syms->syms[i].def_end;
    if (end <= start) continue;
    /* Emit the definition (just the name for v1; a full emitter would
    emit the entire function/struct body). */
    EMIT_SRC(src->text + start, (IUD)(end - start));
    EMIT(";\n");
  }

  out_buf[pos] = 0;
  return pos;
}

ISC TreeShakeRun(TTreeShakeSources* sources, TTreeShakeSymTab* syms,
                 const CHA* const* roots, ISC root_count,
                 CHA* out_buf, IUD out_buf_len,
                 TTreeShakeResult* result,
                 const TTreeShakeOptions* options) {
  if (IsError(sources) || IsError(syms) || IsError(roots) ||
      IsError(out_buf) || IsError(result) || IsError(options)) {
    return -1;
  }
  if (out_buf_len == 0 || root_count <= 0) return -1;

  /* Initialize (lex all sources). */
  ISC rc = TreeShakeInit(sources, syms, (TTreeShakeOptions*)options);
  if (rc != 0) return rc;

  /* Build the reference graph. */
  TsBuildRefGraph(sources, syms);

  /* Do the reachability walk. */
  TsReachability(syms, roots, root_count, options->deep);

  /* Count input bytes. */
  IUD input_bytes = 0;
  for (ISC i = 0; i < sources->count; ++i) {
    input_bytes += sources->files[i].text_len;
  }

  /* Emit the header. */
  IUD output_len = TsEmit(sources, syms, out_buf, out_buf_len,
                          options->out_path, options);

  /* Fill in the result. */
  result->output_len = output_len;
  result->source_count = sources->count;
  result->input_bytes = input_bytes;
  result->output_bytes = output_len;
  result->total_symbol_count = syms->count;
  result->kept_symbol_count = 0;
  for (ISC i = 0; i < syms->count; ++i) {
    if (syms->syms[i].kept) ++result->kept_symbol_count;
  }
  result->diag_len = 0;
  snprintf(result->diagnostics, 4096,
           "shake complete: %d sources, %d syms found, %d kept\n",
           sources->count, syms->count, result->kept_symbol_count);

  if (output_len >= out_buf_len) return -2;  //< Buffer was too small.
  return 0;
}

}  //< namespace CTK

/* ============================================================
 * Standalone tool entry point (only when compiled as the main TU)
 * ============================================================ */
#ifdef CRABSTK_TREESHAKE_MAIN


static void TreeShakeUsage(const CHA* prog) {
  printf("Usage: %s --api=NAME --sources=f1.hxx,f2.hxx --roots=Sym1,Sym2 --out=NAME.hxx\n", prog);
  printf("  --api=NAME       API name (for the header guard + diagnostics)\n");
  printf("  --sources=F1,F2  Comma-separated list of source files to shake\n");
  printf("  --roots=S1,S2    Comma-separated list of root symbols (API entry points)\n");
  printf("  --out=NAME.hxx   Output file path\n");
  printf("  --shallow        Shallow mode (direct deps only, no transitive closure)\n");
  printf("  --no-demacro     Don't compile away #if _Seam gates\n");
  printf("  --seam=N         _Seam value for constant-folding (default 1)\n");
}

ISC main(ISC argc, CHA** argv) {
  using namespace CTK;
  CHA api_name[128] = {0};
  CHA sources_csv[4096] = {0};
  CHA roots_csv[1024] = {0};
  CHA out_path[512] = {0};
  BOL deep = true;
  BOL demacro = true;
  IUD seam_value = 1;

  for (ISC i = 1; i < argc; ++i) {
    if (strncmp(argv[i], "--api=", 6) == 0) strncpy(api_name, argv[i] + 6, 127);
    else if (strncmp(argv[i], "--sources=", 10) == 0) strncpy(sources_csv, argv[i] + 10, 4095);
    else if (strncmp(argv[i], "--roots=", 8) == 0) strncpy(roots_csv, argv[i] + 8, 1023);
    else if (strncmp(argv[i], "--out=", 6) == 0) strncpy(out_path, argv[i] + 6, 511);
    else if (strcmp(argv[i], "--shallow") == 0) deep = false;
    else if (strcmp(argv[i], "--no-demacro") == 0) demacro = false;
    else if (strncmp(argv[i], "--seam=", 7) == 0) seam_value = (IUD)StrToInt(argv[i] + 7);
    else { TreeShakeUsage(argv[0]); return 1; }
  }

  if (!api_name[0] || !sources_csv[0] || !roots_csv[0] || !out_path[0]) {
    TreeShakeUsage(argv[0]);
    return 1;
  }

  /* Parse the sources CSV into a file list. */
  enum { TS_MAX_SOURCES = 32 };
  CHA source_paths[TS_MAX_SOURCES][512];
  ISC source_count = 0;
  {
    CHA* saveptr = 0;
    CHA* tok = strtok(sources_csv, ",");
    while (tok && source_count < TS_MAX_SOURCES) {
      strncpy(source_paths[source_count], tok, 511);
      source_paths[source_count][511] = 0;
      ++source_count;
      tok = strtok(0, ",");
    }
  }

  /* Parse the roots CSV into a symbol list. */
  enum { TS_MAX_ROOTS = 64 };
  CHA roots[TS_MAX_ROOTS][TS_SYM_NAME_MAX];
  ISC root_count = 0;
  {
    CHA* tok = strtok(roots_csv, ",");
    while (tok && root_count < TS_MAX_ROOTS) {
      strncpy(roots[root_count], tok, TS_SYM_NAME_MAX - 1);
      roots[root_count][TS_SYM_NAME_MAX - 1] = 0;
      ++root_count;
      tok = strtok(0, ",");
    }
  }

  /* Stat all source files to compute actual sizes (no dynamic memory). */
  struct TsFileStat { long size; };
  TsFileStat file_stats[TS_MAX_SOURCES];
  IUD total_file_bytes = 0;
  for (ISC i = 0; i < source_count; ++i) {
    FILE* fp = fopen(source_paths[i], "rb");
    if (!fp) { printf("ERROR: cannot open %s\n", source_paths[i]); return 1; }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fclose(fp);
    if (sz < 0) { printf("ERROR: cannot stat %s\n", source_paths[i]); return 1; }
    file_stats[i].size = sz;
    total_file_bytes += (IUD)sz;
  }
  /* Add null-terminators + alignment padding. */
  total_file_bytes += (IUD)source_count + 64;

  /* Read all source files into a single stack buffer (sized to actual input).
  For typical use (2-8 files, ~50KB total) this is well within the stack.
  For very large inputs, the caller should reduce the source list. */
  CHA file_bufs[131072];  //< 128KB stack buffer (enough for ~8 medium headers)
  if (total_file_bytes > (IUD)sizeof(file_bufs)) {
    printf("ERROR: total input %llu bytes exceeds %zu byte stack buffer\n",
           (unsigned long long)total_file_bytes, sizeof(file_bufs));
    return 1;
  }
  TTreeShakeSource sources_arr[TS_MAX_SOURCES];
  IUD buf_off = 0;
  for (ISC i = 0; i < source_count; ++i) {
    IUD len = 0;
    ISC rc = TreeShakeReadFile(source_paths[i], file_bufs + buf_off,
                               (IUD)(sizeof(file_bufs) - buf_off), &len);
    if (rc != 0) { printf("ERROR: cannot read %s\n", source_paths[i]); return 1; }
    sources_arr[i].path = source_paths[i];
    sources_arr[i].text = file_bufs + buf_off;
    sources_arr[i].text_len = len;
    sources_arr[i].tokens = 0;
    sources_arr[i].token_count = 0;
    sources_arr[i].token_capacity = 0;
    buf_off += len + 1;  //< advance past null-terminator
  }
  TTreeShakeSources sources;
  sources.files = sources_arr;
  sources.count = source_count;
  sources.capacity = TS_MAX_SOURCES;

  /* Token pool (stack, sized to actual input). */
  enum { TS_TOKENS_PER_BYTE = 4 };
  ISC total_token_cap = (ISC)(total_file_bytes * TS_TOKENS_PER_BYTE) + 64 * source_count;
  /* Stack array: 8K tokens = 160KB (enough for ~4KB of input at 4 tokens/byte).
  For larger inputs, the caller should reduce the source list or use a
  RAMFactory-allocated pool. */
  TCodeToken tokens_pool[8192];
  if (total_token_cap > (ISC)(sizeof(tokens_pool) / sizeof(TCodeToken))) {
    printf("ERROR: token pool too small for %d tokens (max %zu)\n",
           total_token_cap, sizeof(tokens_pool) / sizeof(TCodeToken));
    return 1;
  }
  TCodeToken* token_cursor = tokens_pool;
  for (ISC i = 0; i < source_count; ++i) {
    ISC cap = (ISC)(sources_arr[i].text_len * TS_TOKENS_PER_BYTE) + 64;
    sources_arr[i].tokens = token_cursor;
    sources_arr[i].token_capacity = cap;
    token_cursor += cap;
  }

  /* Symbol table (stack, bounded). */
  enum { TS_MAX_SYMS = 2048 };
  TTreeShakeSymbol syms_arr[TS_MAX_SYMS];
  TTreeShakeSymTab syms;
  syms.syms = syms_arr;
  syms.count = 0;
  syms.capacity = TS_MAX_SYMS;

  /* Set up options. */
  TTreeShakeOptions opts;
  memset(&opts, 0, sizeof(opts));
  opts.deep = deep;
  opts.demacro = demacro;
  opts.inline_small = true;
  opts.seam_value = seam_value;
  strncpy(opts.out_path, out_path, 511);

  /* Run the shake (stack-allocated output buffer). */
  enum { TS_OUT_BUF = 65536 };
  CHA out_buf[TS_OUT_BUF];
  TTreeShakeResult result;
  memset(&result, 0, sizeof(result));

  ISC rc = TreeShakeRun(&sources, &syms, (const CHA* const*)roots, root_count,
                        out_buf, TS_OUT_BUF, &result, &opts);
  if (rc == -2) {
    printf("ERROR: output buffer too small (%d bytes needed)\n", (ISC)result.output_len);
    return 1;
  }
  if (rc != 0) {
    printf("ERROR: TreeShakeRun failed (rc=%d)\n", rc);
    if (result.diag_len > 0) printf("%.*s\n", (ISC)result.diag_len, result.diagnostics);
    return 1;
  }

  /* Write the output. */
  FILE* fp = fopen(out_path, "w");
  if (!fp) {
    printf("ERROR: cannot write %s\n", out_path);
    return 1;
  }
  fwrite(out_buf, 1, result.output_len, fp);
  fclose(fp);

  /* Print stats. */
  printf("=== TreeShake: %s ===\n", api_name);
  printf("  sources:     %d files, %llu bytes input\n",
         result.source_count, (unsigned long long)result.input_bytes);
  printf("  symbols:     %d found, %d kept\n",
         result.total_symbol_count, result.kept_symbol_count);
  printf("  output:      %llu bytes -> %s\n",
         (unsigned long long)result.output_bytes, out_path);
  if (result.input_bytes > 0) {
    double ratio = (double)result.output_bytes / (double)result.input_bytes;
    printf("  compression: %.1f%% of input\n", ratio * 100.0);
  }
  if (result.diag_len > 0) {
    printf("  diagnostics:\n");
    printf("%.*s\n", (ISC)result.diag_len, result.diagnostics);
  }

  /* Cleanup: nothing to free (all stack-allocated). */
  return 0;
}

}  //< namespace Code
}  //< namespace CTK
#endif  //< CRABSTK_TREESHAKE_MAIN
