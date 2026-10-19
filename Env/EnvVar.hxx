// Copyright AStarship <https://astarship.net>.
#include "EnvVar.h"
#if SEAM >= CRABSTK_ENV_VAR
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

namespace _ {

/* Trim leading/trailing whitespace in place; returns length. */
static IUC EnvTrim(CHA* s) {
  IUC len = 0;
  while (s[len] != '\0' && s[len] <= ' ' && s[len] >= 0) ++len;
  if (len > 0) memmove(s, s + len, (size_t)(IUC)strlen(s + len) + 1);
  IUC total = (IUC)strlen(s);
  while (total > 0 && (s[total - 1] <= ' ' && s[total - 1] >= 0)) {
    s[--total] = '\0';
  }
  return total;
}

template<IUC Capacity>
EnvStatus TEnvLoad(const CHA* uri, TEnvTable<Capacity>* table) {
  if (IsError(uri) || IsError(table)) {
    errno = EINVAL;
    return EnvIO;
  }

  FILE* fp = fopen(uri, "r");
  if (fp == nullptr) {
    errno = ENOENT;
    return EnvIO;
  }

  CHA line[2048];
  IUD line_no = 0;
  IUC count = 0;

  while (fgets(line, (int)sizeof(line), fp) != nullptr) {
    ++line_no;

    // Strip newline.
    IUC len = (IUC)strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }

    // Skip blank lines and comments.
    EnvTrim(line);
    if (line[0] == '\0' || line[0] == '#') continue;

    // Find key/value separator.
    CHA* eq = strchr(line, '=');
    if (eq == nullptr) continue;  // malformed line; skip.
    *eq = '\0';

    CHA* key = line;
    CHA* value = eq + 1;
    EnvTrim(key);
    EnvTrim(value);

    if (key[0] == '\0') continue;  // empty key.

    // First occurrence wins; skip duplicates.
    {
      BOL is_dup = false;
      for (IUC j = 0; j < count; ++j) {
        if (strcmp(table->entries[j].key, key) == 0) {
          is_dup = true;
          break;
        }
      }
      if (is_dup) continue;
    }

    if (count >= Capacity) {
      fclose(fp);
      errno = ENOSPC;
      return EnvCapacity;
    }

    TEnvVar& entry = table->entries[count];
    IUC key_len = (IUC)strlen(key);
    IUC val_len = (IUC)strlen(value);

    if (key_len > 128) {
      fclose(fp);
      errno = ENAMETOOLONG;
      return EnvCapacity;
    }
    if (val_len > 1024) {
      fclose(fp);
      errno = ENAMETOOLONG;
      return EnvCapacity;
    }

    memset(&entry, 0, sizeof(entry));
    memcpy(entry.key, key, key_len + 1);
    memcpy(entry.value, value, val_len + 1);
    entry.line_number = (IUB)line_no;
    ++count;
  }

  fclose(fp);
  table->count = count;
  table->count_max = Capacity;
  return EnvOK;
}

template<IUC Capacity>
ISN TEnvFind(const TEnvTable<Capacity>& table, const CHA* key) {
  if (IsError(key)) return -1;
  for (IUC i = 0; i < table.count; ++i) {
    if (strcmp(table.entries[i].key, key) == 0) {
      return (ISN)i;
    }
  }
  return -1;
}

template<IUC Capacity>
const CHA* TEnvGet(const TEnvTable<Capacity>& table, const CHA* key) {
  ISN idx = TEnvFind<Capacity>(table, key);
  if (idx < 0) return nullptr;
  return table.entries[idx].value;
}

template<IUC Capacity>
EnvStatus TEnvGetI(const TEnvTable<Capacity>& table, const CHA* key,
                   ISC* out) {
  const CHA* value = TEnvGet<Capacity>(table, key);
  if (value == nullptr) return EnvNotFound;
  if (value[0] == '\0') return EnvEmpty;

  errno = 0;
  CHA* end = nullptr;
  long result = strtol(value, &end, 10);
  if (errno != 0 || end == value || *end != '\0') {
    return EnvIO;
  }
  *out = (ISC)result;
  return EnvOK;
}

template<IUC Capacity>
ISN TEnvExport(const TEnvTable<Capacity>& table) {
  ISN exported = 0;
  for (IUC i = 0; i < table.count; ++i) {
    const CHA* key = table.entries[i].key;
    const CHA* value = table.entries[i].value;
    // Process environment wins; skip if key already set.
    if (getenv(key) != nullptr) continue;
    if (setenv(key, value, 1) != 0) continue;
    ++exported;
  }
  return exported;
}

// Explicit template instantiations for common capacities.
template EnvStatus TEnvLoad<16>(const CHA*, TEnvTable<16>*);
template EnvStatus TEnvLoad<32>(const CHA*, TEnvTable<32>*);
template EnvStatus TEnvLoad<64>(const CHA*, TEnvTable<64>*);
template EnvStatus TEnvLoad<128>(const CHA*, TEnvTable<128>*);
template EnvStatus TEnvLoad<256>(const CHA*, TEnvTable<256>*);

template ISN TEnvFind<16>(const TEnvTable<16>&, const CHA*);
template ISN TEnvFind<32>(const TEnvTable<32>&, const CHA*);
template ISN TEnvFind<64>(const TEnvTable<64>&, const CHA*);
template ISN TEnvFind<128>(const TEnvTable<128>&, const CHA*);
template ISN TEnvFind<256>(const TEnvTable<256>&, const CHA*);

template const CHA* TEnvGet<16>(const TEnvTable<16>&, const CHA*);
template const CHA* TEnvGet<32>(const TEnvTable<32>&, const CHA*);
template const CHA* TEnvGet<64>(const TEnvTable<64>&, const CHA*);
template const CHA* TEnvGet<128>(const TEnvTable<128>&, const CHA*);
template const CHA* TEnvGet<256>(const TEnvTable<256>&, const CHA*);

template EnvStatus TEnvGetI<16>(const TEnvTable<16>&, const CHA*, ISC*);
template EnvStatus TEnvGetI<32>(const TEnvTable<32>&, const CHA*, ISC*);
template EnvStatus TEnvGetI<64>(const TEnvTable<64>&, const CHA*, ISC*);
template EnvStatus TEnvGetI<128>(const TEnvTable<128>&, const CHA*, ISC*);
template EnvStatus TEnvGetI<256>(const TEnvTable<256>&, const CHA*, ISC*);

template ISN TEnvExport<16>(const TEnvTable<16>&);
template ISN TEnvExport<32>(const TEnvTable<32>&);
template ISN TEnvExport<64>(const TEnvTable<64>&);
template ISN TEnvExport<128>(const TEnvTable<128>&);
template ISN TEnvExport<256>(const TEnvTable<256>&);

}  //< namespace _
#endif
