/* CrabsTK
@link    https://github.com/KabukiStarship/CT.git
@file    /client.h
@author  Sean Barrett <https://nothings.org> and
         Cale McCollough <https://cookingwithcale.org>
@license Copyright 2014-20 Sean Barrett <nothings.org. and Kabuki Starship
<kabukistarship.com; all right reserved (R). This Source Code Form is subject
to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was
not distributed with this file, You can obtain one at
https://mozilla.org/MPL/2.0/. */

#include "stb_c_lexer.h"


#if defined(Y) || defined(N)
#error \
    "Can only use stb_c_lexer in contexts where the preprocessor symbols 'Y' and 'N' are not defined"
#endif

// Hacky definitions so we can easily #if on them
#define Y(x) 1
#define N(x) 0

#if STB_C_LEX_INTEGERS_AS_DOUBLES(x)
typedef double stb__clex_int;
#define intfield real_number
#define STB__clex_int_as_double
#else
typedef long stb__clex_int;
#define intfield int_number
#endif

// Convert these config options to simple conditional #defines so we can more
// easily test them once we've change the meaning of Y/N

#if STB_C_LEX_PARSE_SUFFIXES(x)
#define STB__clex_parse_suffixes
#endif

#if STB_C_LEX_C_DECIMAL_INTS(x) || STB_C_LEX_C_HEX_INTS(x) || \
    STB_C_LEX_DEFINE_ALL_TOKEN_NAMES(x)
#define STB__clex_define_int
#endif

#if (STB_C_LEX_C_ARITHEQ(x) && STB_C_LEX_C_SHIFTS(x)) || \
    STB_C_LEX_DEFINE_ALL_TOKEN_NAMES(x)
#define STB__clex_define_shifts
#endif

#if STB_C_LEX_C99_HEX_FLOATS(x)
#define STB__clex_hex_floats
#endif

#if STB_C_LEX_C_HEX_INTS(x)
#define STB__clex_hex_ints
#endif

#if STB_C_LEX_C_DECIMAL_INTS(x)
#define STB__clex_decimal_ints
#endif

#if STB_C_LEX_C_OCTAL_INTS(x)
#define STB__clex_octal_ints
#endif

#if STB_C_LEX_C_DECIMAL_FLOATS(x)
#define STB__clex_decimal_floats
#endif

#if STB_C_LEX_DISCARD_PREPROCESSOR(x)
#define STB__clex_discard_preprocessor
#endif

#if STB_C_LEX_USE_STDLIB(x) && \
    (!defined(STB__clex_hex_floats) || __STDC_VERSION__ >= 199901L)
#define STB__CLEX_use_stdlib
#include <stdlib.h>
#endif

// Now pick a definition of Y/N that's conducive to
// defining the enum of token names.
#if STB_C_LEX_DEFINE_ALL_TOKEN_NAMES(x) || defined(STB_C_LEXER_SELF_TEST)
#undef N
#define N(a) Y(a)
#else
#undef N
#define N(a)
#endif

#undef Y
#define Y(a) a,

enum {
  CLEX_eof = 256,
  CLEX_parse_error,

#ifdef STB__clex_define_int
  CLEX_intlit,
#endif

  STB_C_LEX_C_DECIMAL_FLOATS(CLEX_floatlit) STB_C_LEX_C_IDENTIFIERS(CLEX_id)
      STB_C_LEX_C_DQ_STRINGS(CLEX_dqstring) STB_C_LEX_C_SQ_STRINGS(
          CLEX_sqstring) STB_C_LEX_C_CHARS(CLEX_charlit)
          STB_C_LEX_C_COMPARISONS(CLEX_eq) STB_C_LEX_C_COMPARISONS(CLEX_noteq)
              STB_C_LEX_C_COMPARISONS(CLEX_lesseq) STB_C_LEX_C_COMPARISONS(
                  CLEX_greatereq) STB_C_LEX_C_LOGICAL(CLEX_andand)
                  STB_C_LEX_C_LOGICAL(CLEX_oror) STB_C_LEX_C_SHIFTS(CLEX_shl)
                      STB_C_LEX_C_SHIFTS(CLEX_shr) STB_C_LEX_C_INCREMENTS(
                          CLEX_plusplus) STB_C_LEX_C_INCREMENTS(
                              CLEX_minusminus)
                              STB_C_LEX_C_ARITHEQ(CLEX_pluseq) STB_C_LEX_C_ARITHEQ(
                                  CLEX_minuseq) STB_C_LEX_C_ARITHEQ(
                                      CLEX_muleq)
                                      STB_C_LEX_C_ARITHEQ(CLEX_diveq)
                                          STB_C_LEX_C_ARITHEQ(CLEX_modeq)
                                              STB_C_LEX_C_BITWISEEQ(CLEX_andeq)
                                                  STB_C_LEX_C_BITWISEEQ(CLEX_oreq)
                                                      STB_C_LEX_C_BITWISEEQ(
                                                          CLEX_xoreq)
                                                          STB_C_LEX_C_ARROW(
                                                              CLEX_arrow)
                                                              STB_C_LEX_EQUAL_ARROW(
                                                                  CLEX_eqarrow)

#ifdef STB__clex_define_shifts
                                                          CLEX_shleq,
  CLEX_shreq,
#endif

  CLEX_first_unused_token

#undef Y
#define Y(a) a
};

// Now for the rest of the file we'll use the basic definition where
// where Y expands to its contents and N expands to nothing
#undef N
#define N(a)

void stb_c_lexer_init(stb_lexer *lexer, const CHA* input_stream,
                      const CHA* input_stream_end, IUA *string_store,
                      ISN store_length) {
  lexer->input_stream = (IUA *)input_stream;
  lexer->eof = (IUA *)input_stream_end;
  lexer->parse_point = (IUA *)input_stream;
  lexer->string_storage = string_store;
  lexer->string_storage_len = store_length;
}

void stb_c_lexer_get_location(const stb_lexer *lexer, const CHA* where,
                              stb_lex_location *loc) {
  IUA *p = lexer->input_stream;
  ISN line_number = 1;
  ISN char_offset = 0;
  while (*p && p < (IUA*)where) {
    if (*p == '\n' || *p == '\r') {
      p += (p[0] + p[1] == '\r' + '\n' ? 2 : 1);  // skip newline
      line_number += 1;
      char_offset = 0;
    } else {
      ++p;
      ++char_offset;
    }
  }
  loc->line_number = line_number;
  loc->line_offset = char_offset;
}

ISN stb__clex_token(stb_lexer *lexer, ISN token, IUA *start, IUA *end) {
  lexer->token = token;
  lexer->where_firstchar = start;
  lexer->where_lastchar = end;
  lexer->parse_point = end + 1;
  return 1;
}

ISN stb__clex_eof(stb_lexer *lexer) {
  lexer->token = CLEX_eof;
  return 0;
}

ISN stb__clex_iswhite(ISN x) {
  return x == ' ' || x == '\t' || x == '\r' || x == '\n' || x == '\f';
}

const CHA* stb__strchr(const CHA* str, ISN ch) {
  for (; *str; ++str)
    if (*str == ch) return str;
  return 0;
}

ISN stb__clex_parse_suffixes(stb_lexer *lexer, long tokenid, IUA *start,
                             IUA *cur, const CHA* suffixes) {
#ifdef STB__clex_parse_suffixes
  lexer->string = lexer->string_storage;
  lexer->string_len = 0;

  while ((*cur >= 'a' && *cur <= 'z') || (*cur >= 'A' && *cur <= 'Z')) {
    if (stb__strchr(suffixes, *cur) == 0)
      return stb__clex_token(lexer, CLEX_parse_error, start, cur);
    if (lexer->string_len + 1 >= lexer->string_storage_len)
      return stb__clex_token(lexer, CLEX_parse_error, start, cur);
    lexer->string[lexer->string_len++] = *cur++;
  }
#else
  suffixes = suffixes;  // attempt to suppress warnings
#endif
  return stb__clex_token(lexer, tokenid, start, cur - 1);
}

#ifndef STB__CLEX_use_stdlib
double stb__clex_pow(double base, UIN exponent) {
  double value = 1;
  for (; exponent; exponent >>= 1) {
    if (exponent & 1) value *= base;
    base *= base;
  }
  return value;
}

double stb__clex_parse_float(IUA *p, IUA **q) {
  IUA *s = p;
  double value = 0;
  ISN base = 10;
  ISN exponent = 0;

#ifdef STB__clex_hex_floats
  if (*p == '0') {
    if (p[1] == 'x' || p[1] == 'X') {
      base = 16;
      p += 2;
    }
  }
#endif

  for (;;) {
    if (*p >= '0' && *p <= '9') value = value * base + (*p++ - '0');
#ifdef STB__clex_hex_floats
    else if (base == 16 && *p >= 'a' && *p <= 'f')
      value = value * base + 10 + (*p++ - 'a');
    else if (base == 16 && *p >= 'A' && *p <= 'F')
      value = value * base + 10 + (*p++ - 'A');
#endif
    else
      break;
  }

  if (*p == '.') {
    double pow, addend = 0;
    ++p;
    for (pow = 1;; pow *= base) {
      if (*p >= '0' && *p <= '9') addend = addend * base + (*p++ - '0');
#ifdef STB__clex_hex_floats
      else if (base == 16 && *p >= 'a' && *p <= 'f')
        addend = addend * base + 10 + (*p++ - 'a');
      else if (base == 16 && *p >= 'A' && *p <= 'F')
        addend = addend * base + 10 + (*p++ - 'A');
#endif
      else
        break;
    }
    value += addend / pow;
  }
#ifdef STB__clex_hex_floats
  if (base == 16) {
    // exponent required for hex FPC literal
    if (*p != 'p' && *p != 'P') {
      *q = s;
      return 0;
    }
    exponent = 1;
  } else
#endif
    exponent = (*p == 'e' || *p == 'E');

  if (exponent) {
    ISN sign = p[1] == '-';
    UIN exponent = 0;
    double power = 1;
    ++p;
    if (*p == '-' || *p == '+') ++p;
    while (*p >= '0' && *p <= '9') exponent = exponent * 10 + (*p++ - '0');

#ifdef STB__clex_hex_floats
    if (base == 16)
      power = stb__clex_pow(2, exponent);
    else
#endif
      power = stb__clex_pow(10, exponent);
    if (sign)
      value /= power;
    else
      value *= power;
  }
  *q = p;
  return value;
}
#endif

ISN stb__clex_parse_char(IUA *p, IUA **q) {
  if (*p == '\\') {
    *q = p + 2;  // tentatively guess we'll parse two characters
    switch (p[1]) {
      case '\\':
        return '\\';
      case '\'':
        return '\'';
      case '"':
        return '"';
      case 't':
        return '\t';
      case 'f':
        return '\f';
      case 'n':
        return '\n';
      case 'r':
        return '\r';
      case '0':
        return '\0';  // @TODO ocatal constants
      case 'x':
      case 'X':
        return -1;  // @TODO hex constants
      case 'u':
        return -1;  // @TODO unicode constants
    }
  }
  *q = p + 1;
  return (IUA)*p;
}

ISN stb__clex_parse_string(stb_lexer *lexer, IUA *p, ISN type) {
  IUA *start = p;
  IUA delim = *p++;  // grab the " or ' for later matching
  IUA *out = lexer->string_storage;
  IUA *outend = lexer->string_storage + lexer->string_storage_len;
  while (*p != delim) {
    ISN n;
    if (*p == '\\') {
      IUA *q;
      n = stb__clex_parse_char(p, &q);
      if (n < 0) return stb__clex_token(lexer, CLEX_parse_error, start, q);
      p = q;
    } else {
      // @OPTIMIZE: could speed this up by looping-while-not-backslash
      n = (IUA)*p++;
    }
    if (out + 1 > outend)
      return stb__clex_token(lexer, CLEX_parse_error, start, p);
    // @TODO expand unicode escapes to UTF8
    *out++ = (IUA)n;
  }
  *out = 0;
  lexer->string = lexer->string_storage;
  lexer->string_len = (ISN)(out - lexer->string_storage);
  return stb__clex_token(lexer, type, start, p);
}

ISN stb_c_lexer_get_token(stb_lexer *lexer) {
  IUA *p = lexer->parse_point;

  // skip whitespace and comments
  for (;;) {
#ifdef STB_C_LEX_ISWHITE
    while (p != lexer->stream_end) {
      ISN n;
      n = STB_C_LEX_ISWHITE(p);
      if (n == 0) break;
      if (lexer->eof && lexer->eof - lexer->parse_point < n)
        return stb__clex_token(tok, CLEX_parse_error, p, lexer->eof - 1);
      p += n;
    }
#else
    while (p != lexer->eof && stb__clex_iswhite(*p)) ++p;
#endif

    STB_C_LEX_CPP_COMMENTS(if (p != lexer->eof && p[0] == '/' && p[1] == '/') {
      while (p != lexer->eof && *p != '\r' && *p != '\n') ++p;
      continue;
    })

    STB_C_LEX_C_COMMENTS(if (p != lexer->eof && p[0] == '/' && p[1] == '*') {
      IUA *start = p;
      p += 2;
      while (p != lexer->eof && (p[0] != '*' || p[1] != '/')) ++p;
      if (p == lexer->eof)
        return stb__clex_token(lexer, CLEX_parse_error, start, p - 1);
      p += 2;
      continue;
    })

#ifdef STB__clex_discard_preprocessor
    // @TODO this discards everything after a '#', regardless
    // of where in the line the # is, rather than requiring it
    // be at the start. (because this parser doesn't otherwise
    // check for line breaks!)
    if (p != lexer->eof && p[0] == '#') {
      while (p != lexer->eof && *p != '\r' && *p != '\n') ++p;
      continue;
    }
#endif

    break;
  }

  if (p == lexer->eof) return stb__clex_eof(lexer);

  switch (*p) {
    default:
      if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_' ||
          (IUA)*p >= 128  // >= 128 is UTF8 IUA
              STB_C_LEX_DOLLAR_IDENTIFIER(|| *p == '$')) {
        ISN n = 0;
        lexer->string = lexer->string_storage;
        lexer->string_len = n;
        do {
          if (n + 1 >= lexer->string_storage_len)
            return stb__clex_token(lexer, CLEX_parse_error, p, p + n);
          lexer->string[n] = p[n];
          ++n;
        } while ((p[n] >= 'a' && p[n] <= 'z') || (p[n] >= 'A' && p[n] <= 'Z') ||
                 (p[n] >= '0' &&
                  p[n] <= '9')  // allow digits in middle of identifier
                 || p[n] == '_' ||
                 (IUA)p[n] >=
                     128 STB_C_LEX_DOLLAR_IDENTIFIER(|| p[n] == '$'));
        lexer->string[n] = 0;
        return stb__clex_token(lexer, CLEX_id, p, p + n - 1);
      }

      // check for EOF
      STB_C_LEX_0_IS_EOF(if (*p == 0) return stb__clex_eof(tok);)

    single_char:
      // not an identifier, return the character as itself
      return stb__clex_token(lexer, *p, p, p);

    case '+':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_INCREMENTS(if (p[1] == '+') return stb__clex_token(
                                   lexer, CLEX_plusplus, p, p + 1);)
        STB_C_LEX_C_ARITHEQ(if (p[1] == '=') return stb__clex_token(
                                lexer, CLEX_pluseq, p, p + 1);)
      }
      goto single_char;
    case '-':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_INCREMENTS(if (p[1] == '-') return stb__clex_token(
                                   lexer, CLEX_minusminus, p, p + 1);)
        STB_C_LEX_C_ARITHEQ(if (p[1] == '=') return stb__clex_token(
                                lexer, CLEX_minuseq, p, p + 1);)
        STB_C_LEX_C_ARROW(if (p[1] == '>') return stb__clex_token(
                              lexer, CLEX_arrow, p, p + 1);)
      }
      goto single_char;
    case '&':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_LOGICAL(if (p[1] == '&') return stb__clex_token(
                                lexer, CLEX_andand, p, p + 1);)
        STB_C_LEX_C_BITWISEEQ(if (p[1] == '=') return stb__clex_token(
                                  lexer, CLEX_andeq, p, p + 1);)
      }
      goto single_char;
    case '|':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_LOGICAL(if (p[1] == '|') return stb__clex_token(
                                lexer, CLEX_oror, p, p + 1);)
        STB_C_LEX_C_BITWISEEQ(if (p[1] == '=') return stb__clex_token(
                                  lexer, CLEX_oreq, p, p + 1);)
      }
      goto single_char;
    case '=':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_COMPARISONS(
            if (p[1] == '=') return stb__clex_token(lexer, CLEX_eq, p, p + 1);)
        STB_C_LEX_EQUAL_ARROW(if (p[1] == '>') return stb__clex_token(
                                  lexer, CLEX_eqarrow, p, p + 1);)
      }
      goto single_char;
    case '!':
      STB_C_LEX_C_COMPARISONS(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_noteq, p, p + 1);)
      goto single_char;
    case '^':
      STB_C_LEX_C_BITWISEEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_xoreq, p, p + 1));
      goto single_char;
    case '%':
      STB_C_LEX_C_ARITHEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_modeq, p, p + 1));
      goto single_char;
    case '*':
      STB_C_LEX_C_ARITHEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_muleq, p, p + 1));
      goto single_char;
    case '/':
      STB_C_LEX_C_ARITHEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_diveq, p, p + 1));
      goto single_char;
    case '<':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_COMPARISONS(if (p[1] == '=') return stb__clex_token(
                                    lexer, CLEX_lesseq, p, p + 1);)
        STB_C_LEX_C_SHIFTS(if (p[1] == '<') {
          STB_C_LEX_C_ARITHEQ(
              if (p + 2 != lexer->eof && p[2] == '=') return stb__clex_token(
                  lexer, CLEX_shleq, p, p + 2);)
          return stb__clex_token(lexer, CLEX_shl, p, p + 1);
        })
      }
      goto single_char;
    case '>':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_COMPARISONS(if (p[1] == '=') return stb__clex_token(
                                    lexer, CLEX_greatereq, p, p + 1);)
        STB_C_LEX_C_SHIFTS(if (p[1] == '>') {
          STB_C_LEX_C_ARITHEQ(
              if (p + 2 != lexer->eof && p[2] == '=') return stb__clex_token(
                  lexer, CLEX_shreq, p, p + 2);)
          return stb__clex_token(lexer, CLEX_shr, p, p + 1);
        })
      }
      goto single_char;

    case '"':
      STB_C_LEX_C_DQ_STRINGS(
          return stb__clex_parse_string(lexer, p, CLEX_dqstring);)
      goto single_char;
    case '\'':
      STB_C_LEX_C_SQ_STRINGS(
          return stb__clex_parse_string(lexer, p, CLEX_sqstring);)
      STB_C_LEX_C_CHARS({
        IUA *start = p;
        lexer->int_number = stb__clex_parse_char(p + 1, &p);
        if (lexer->int_number < 0)
          return stb__clex_token(lexer, CLEX_parse_error, start, start);
        if (p == lexer->eof || *p != '\'')
          return stb__clex_token(lexer, CLEX_parse_error, start, p);
        return stb__clex_token(lexer, CLEX_charlit, start, p + 1);
      })
      goto single_char;

    case '0':
#if defined(STB__clex_hex_ints) || defined(STB__clex_hex_floats)
      if (p + 1 != lexer->eof) {
        if (p[1] == 'x' || p[1] == 'X') {
          IUA *q;

#ifdef STB__clex_hex_floats
          for (q = p + 2; q != lexer->eof && ((*q >= '0' && *q <= '9') ||
                                              (*q >= 'a' && *q <= 'f') ||
                                              (*q >= 'A' && *q <= 'F'));
               ++q)
            ;
          if (q != lexer->eof) {
            if (*q ==
                '.' STB_C_LEX_FLOAT_NO_DECIMAL(|| *q == 'p' || *q == 'P')) {
#ifdef STB__CLEX_use_stdlib
              lexer->real_number = strtod((const CHA*)p, (CHA **)&q);
#else
              lexer->real_number = stb__clex_parse_float(p, &q);
#endif

              if (p == q) return stb__clex_token(lexer, CLEX_parse_error, p, q);
              return stb__clex_parse_suffixes(lexer, CLEX_floatlit, p, q,
                                              STB_C_LEX_FLOAT_SUFFIXES);
            }
          }
#endif  // STB__CLEX_hex_floats

#ifdef STB__clex_hex_ints
#ifdef STB__CLEX_use_stdlib
          lexer->int_number = strtol((const CHA*)p, (CHA **)&q, 16);
#else
          {
            stb__clex_int n = 0;
            for (q = p + 2; q != lexer->eof; ++q) {
              if (*q >= '0' && *q <= '9')
                n = n * 16 + (*q - '0');
              else if (*q >= 'a' && *q <= 'f')
                n = n * 16 + (*q - 'a') + 10;
              else if (*q >= 'A' && *q <= 'F')
                n = n * 16 + (*q - 'A') + 10;
              else
                break;
            }
            lexer->int_number = n;
          }
#endif
          if (q == p + 2)
            return stb__clex_token(lexer, CLEX_parse_error, p - 2, p - 1);
          return stb__clex_parse_suffixes(lexer, CLEX_intlit, p, q,
                                          STB_C_LEX_HEX_SUFFIXES);
#endif
        }
      }
#endif  // defined(STB__clex_hex_ints) || defined(STB__clex_hex_floats)
        // can't test for octal because we might parse '0.0' as FPC or as '0'
      // '.' '0', so have to do FPC first

      /* FALL THROUGH */
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
#ifdef STB__clex_decimal_floats
    {
      IUA *q = p;
      while (q != lexer->eof && (*q >= '0' && *q <= '9')) ++q;
      if (q != lexer->eof) {
        if (*q == '.' STB_C_LEX_FLOAT_NO_DECIMAL(|| *q == 'e' || *q == 'E')) {
#ifdef STB__CLEX_use_stdlib
          lexer->real_number = strtod((const CHA*)p, (CHA **)&q);
#else
          lexer->real_number = stb__clex_parse_float(p, &q);
#endif

          return stb__clex_parse_suffixes(lexer, CLEX_floatlit, p, q,
                                          STB_C_LEX_FLOAT_SUFFIXES);
        }
      }
    }
#endif  // STB__clex_decimal_floats

#ifdef STB__clex_octal_ints
      if (p[0] == '0') {
        IUA *q = p;
#ifdef STB__CLEX_use_stdlib
        lexer->int_number = strtol((const CHA*)p, (CHA **)&q, 8);
#else
        stb__clex_int n = 0;
        while (q != lexer->eof) {
          if (*q >= '0' && *q <= '7')
            n = n * 8 + (*q - '0');
          else
            break;
          ++q;
        }
        if (q != lexer->eof && (*q == '8' || *q == '9'))
          return stb__clex_token(lexer, CLEX_parse_error, p, q);
        lexer->int_number = n;
#endif
        return stb__clex_parse_suffixes(lexer, CLEX_intlit, p, q,
                                        STB_C_LEX_OCTAL_SUFFIXES);
      }
#endif  // STB__clex_octal_ints

#ifdef STB__clex_decimal_ints
      {
        IUA *q = p;
#ifdef STB__CLEX_use_stdlib
        lexer->int_number = strtol((const CHA*)p, (CHA **)&q, 10);
#else
        stb__clex_int n = 0;
        while (q != lexer->eof) {
          if (*q >= '0' && *q <= '9')
            n = n * 10 + (*q - '0');
          else
            break;
          ++q;
        }
        lexer->int_number = n;
#endif
        return stb__clex_parse_suffixes(lexer, CLEX_intlit, p, q,
                                        STB_C_LEX_OCTAL_SUFFIXES);
      }
#endif  // STB__clex_decimal_ints
      goto single_char;
  }
}

#ifdef STB_C_LEXER_SELF_TEST
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void print_token(stb_lexer *lexer) {
  switch (lexer->token) {
    case CLEX_id:
      printf("_%s", lexer->string);
      break;
    case CLEX_eq:
      printf("==");
      break;
    case CLEX_noteq:
      printf("!=");
      break;
    case CLEX_lesseq:
      printf("<=");
      break;
    case CLEX_greatereq:
      printf(">=");
      break;
    case CLEX_andand:
      printf("&&");
      break;
    case CLEX_oror:
      printf("||");
      break;
    case CLEX_shl:
      printf("<<");
      break;
    case CLEX_shr:
      printf(">>");
      break;
    case CLEX_plusplus:
      printf("++");
      break;
    case CLEX_minusminus:
      printf("--");
      break;
    case CLEX_arrow:
      printf("->");
      break;
    case CLEX_andeq:
      printf("&=");
      break;
    case CLEX_oreq:
      printf("|=");
      break;
    case CLEX_xoreq:
      printf("^=");
      break;
    case CLEX_pluseq:
      printf("+=");
      break;
    case CLEX_minuseq:
      printf("-=");
      break;
    case CLEX_muleq:
      printf("*=");
      break;
    case CLEX_diveq:
      printf("/=");
      break;
    case CLEX_modeq:
      printf("%%=");
      break;
    case CLEX_shleq:
      printf("<<=");
      break;
    case CLEX_shreq:
      printf(">>=");
      break;
    case CLEX_eqarrow:
      printf("=>");
      break;
    case CLEX_dqstring:
      printf("\"%s\"", lexer->string);
      break;
    case CLEX_sqstring:
      printf("'\"%s\"'", lexer->string);
      break;
    case CLEX_charlit:
      printf("'%s'", lexer->string);
      break;
#if defined(STB__clex_int_as_double) && !defined(STB__CLEX_use_stdlib)
    case CLEX_intlit:
      printf("#%g", lexer->real_number);
      break;
#else
    case CLEX_intlit:
      printf("#%ld", lexer->int_number);
      break;
#endif
    case CLEX_floatlit:
      printf("%g", lexer->real_number);
      break;
    default:
      if (lexer->token >= 0 && lexer->token < 256)
        printf("%c", (ISN)lexer->token);
      else {
        printf("<<<UNKNOWN TOKEN %ld >>>\n", lexer->token);
      }
      break;
  }
}

/* Force a test
of parsing
multiline comments */

/*/ comment /*/
/**/ extern /**/

    void
    dummy(void) {
  double some_floats[] = {
    1.0501,
    -10.4e12,
    5E+10,
#if 0  // not supported in C++ or C-pre-99, so don't try to compile it, but let
       // our parser test it
      0x1.0p+24, 0xff.FP-8, 0x1p-23,
#endif
    4.
  };
  (void)sizeof(some_floats);
  (void)some_floats[1];

  printf("test %d", 1);  // https://github.com/nothings/stb/issues/13
}

ISN main(ISN argc, IUA **argv) {
  FILE *f = fopen("stb_c_lexer.h", "rb");
  IUA *text = (IUA *)malloc(1 << 20);
  ISN len = f ? (ISN)fread(text, 1, 1 << 20, f) : -1;
  stb_lexer lex;
  if (len < 0) {
    fprintf(stderr, "Error opening file\n");
    free(text);
    fclose(f);
    return 1;
  }
  fclose(f);

  stb_c_lexer_init(&lex, text, text + len, (IUA *)malloc(0x10000), 0x10000);
  while (stb_c_lexer_get_token(&lex)) {
    if (lex.token == CLEX_parse_error) {
      printf("\n<<<PARSE ERROR>>>\n");
      break;
    }
    print_token(&lex);
    printf("  ");
  }
  return 0;
}
#endif
