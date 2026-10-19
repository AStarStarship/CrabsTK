// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_ENV_VAR_DECL
#define CRABSTK_ENV_VAR_DECL
#include "_Config.h"
#include "_Seams.h"
#if SEAM >= CRABSTK_ENV_VAR
namespace _ {

enum EnvStatus : ISC {
  EnvOK = 0,
  EnvNotFound = -1,
  EnvEmpty = -2,
  EnvCapacity = -3,
  EnvIO = -4
};

/* Bounded, pointer-free .env variable entry.
@link Env.md */
struct TEnvVar {
  CHA key[129];   //< 128 + NUL.
  CHA value[1025];  //< 1024 + NUL.
  IUB line_number;
};

/* Fixed-capacity, pointer-free .env variable table (POD).
@link Env.md */
template<IUC Capacity = 64>
struct TEnvTable {
  static_assert(Capacity > 0 && Capacity <= 256);
  IUC count;
  IUC count_max;  //< Always Capacity.
  TEnvVar entries[Capacity];
};

/* Read a single .env file into the table.
@pre table is zeroed or default-initialized.
@return EnvOK or an explicit status; entries not consumed on failure.
@see TEnvGet, TEnvGetI, TEnvExport */
template<IUC Capacity>
EnvStatus TEnvLoad(const CHA* uri, TEnvTable<Capacity>* table);

/* Search for a key; returns its index or -1 if absent. */
template<IUC Capacity>
ISN TEnvFind(const TEnvTable<Capacity>& table, const CHA* key);

/* Return the value for key, or NILP if absent. */
template<IUC Capacity>
const CHA* TEnvGet(const TEnvTable<Capacity>& table, const CHA* key);

/* Numeric form: parse the value for key as ISC; out unchanged on failure. */
template<IUC Capacity>
EnvStatus TEnvGetI(const TEnvTable<Capacity>& table, const CHA* key,
                   ISC* out);

/* Export all table entries into the process environment via ::setenv.
Skips entries whose key already exists in the environment (process
environment wins). OS-specific hiding from processes/agents is a future
integration point.
@return Number of entries actually exported. */
template<IUC Capacity>
ISN TEnvExport(const TEnvTable<Capacity>& table);

}  //< namespace _
#endif
#endif
