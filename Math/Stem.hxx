// Copyright AStarship <https://astarship.net>.
//
// Stem.hxx — implementations for Stem.h.
//
// Binary I/O: temporary stdlib wrapper (fopen/fread/fwrite/fclose).
// This file owns the OS I/O shim, same as File.hxx owns its FILE* shim.
// To be replaced by the ASCIICrabs File API when the seam system
// supports standalone tools.

#include "Stem.h"
#include <stdio.h>

namespace CTK {
namespace Math {
namespace Stem {

IUD ReadBinary(const CHA* path, void* buf, IUD bytes, IUD offset) {
  FILE* fp = fopen(path, "rb");
  if (fp == nullptr) return 0;
  if (offset > 0) {
    if (fseek(fp, (long)offset, SEEK_SET) != 0) {
      fclose(fp);
      return 0;
    }
  }
  size_t got = fread(buf, 1, (size_t)bytes, fp);
  fclose(fp);
  return (IUD)got;
}

IUD ReadBinary(const CHA* path, void* buf, IUD bytes) {
  return ReadBinary(path, buf, bytes, 0);
}

BOL WriteBinary(const CHA* path, const void* buf, IUD bytes) {
  FILE* fp = fopen(path, "wb");
  if (fp == nullptr) return false;
  size_t wrote = fwrite(buf, 1, (size_t)bytes, fp);
  fclose(fp);
  return wrote == (size_t)bytes;
}

}  // namespace Stem
}  // namespace Math
}  // namespace CTK
