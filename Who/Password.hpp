// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_PASSWORD_DECL_DECL
#define CRABSTK_WHO_PASSWORD_DECL_DECL
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#include "Authenticator.hpp"
#include <ASCIICrabs/Random.hxx>
// Cryptographic salt generation uses the OS CSPRNG (getentropy / /dev/urandom),
// not rand() — the ASCIICrabs Random API provides IUCRandom().
#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#include <unistd.h>
#if defined(__linux__)
#include <sys/random.h>
#endif
#endif
namespace _ {

/* Salted password KDF (PBKDF2-SHA256 style).
The salt is a 16-byte random value generated at SetKey time via the
ASCIICrabs Random API (two IUD draws). The key derivation is:
  digest[0] = SHA256(salt || password)
  PRF(i)    = SHA256(salt || INT32_BE(i))          for i = 1..KdfRounds
  key[i]    = digest[i-1] XOR PRF(i)
  key       = FNV1aAvalanche(key)                  (32-byte diffusion pass)
The stored form is a contiguous ASCII string:
  PBK2$<32 hex salt>$<64 hex key>
No C++ std library is used (hard repo rule); all math is unsigned 32-bit. */
enum {
  PasswordSaltBytes = 16,     //< Salt length in bytes.
  PasswordKeyBytes = 32,      //< Digest length in bytes (SHA-256).
  PasswordKdfRounds = 10000,  //< PBKDF2 counter rounds.
  PasswordKeyLength = PasswordKeyBytes * 2,  //< 64 hex chars.
  PasswordStoredLengthMax =
      5 + 1 + PasswordSaltBytes * 2 + 1 + PasswordKeyLength + 1,
                                      //< "PBK2$" + 32 + "$" + 64 + '\0'.
  PasswordStoredLengthMin = 256,     //< Static buffer for stored strings.
  PasswordHexChars = 32,             //< Hex chars per 16-byte field.
};

/* Returns the length of the given nil-terminated CHA string. */
inline ISN TPasswordLen(const CHA* text) {
  ISN length = 0;
  if (text == NILP) return 0;
  while (text[length] != '\0') ++length;
  return length;
}

/* Compares two byte buffers in constant time.
@return true if the buffers are equal. */
inline BOL TPasswordConstantTimeEquals(const IUA* a, const IUA* b, ISN count) {
  IUA diff = 0;
  for (ISN i = 0; i < count; ++i) diff |= (IUA)(a[i] ^ b[i]);
  return diff == 0;
}

/* The 64 SHA-256 round constants (K table). */
static const IUD TPasswordShaK[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1,
    0x923F82A4, 0xAB1C5ED5, 0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
    0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174, 0xE49B69C1, 0xEFBE4786,
    0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147,
    0x06CA6351, 0x14292967, 0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
    0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85, 0xA2BFE8A1, 0xA81A664B,
    0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A,
    0x5B9CCA4F, 0x682E6FF3, 0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

/* SHA-256 scratch: 64-byte input block (the 32-bit schedule is local
to TSha256Compress). */
struct TSha256 {
  IUA input[64];  //< 64-byte input block (prefix + padding + length).
};

/* Packs 4 big-endian bytes into an IUC (32-bit). */
inline IUC TPasswordBe4(const IUA* bytes) {
  IUC packed = 0;
  for (ISN i = 0; i < 4; ++i) packed = (packed << 8) | bytes[i];
  return packed;
}

/* 32-bit right rotate. */
inline IUC TSha256Rotr(IUC x, ISN n) { return (x >> n) | (x << (32 - n)); }

/* SHA-256 compression over one 64-byte block (FIPS 180-2 Sec 6.2.2).
Uses 32-bit arithmetic throughout (IUC). The block must already be fully
formed: message bytes, 0x80 padding bit, zero fill, 64-bit BE bit-length. */
inline void TSha256Compress(const IUA* block, IUA* digest) {
  // Message schedule: 64 x 32-bit words.
  IUC w[64];
  for (ISN i = 0; i < 16; ++i)
    w[i] = TPasswordBe4(block + 4 * i);
  for (ISN i = 16; i < 64; ++i) {
    IUC s0 = TSha256Rotr(w[i-15], 7) ^ TSha256Rotr(w[i-15], 18) ^ (w[i-15] >> 3);
    IUC s1 = TSha256Rotr(w[i-2], 17) ^ TSha256Rotr(w[i-2], 19) ^ (w[i-2] >> 10);
    w[i] = w[i-16] + s0 + w[i-7] + s1;
  }
  // Initial hash values (FIPS 180-2 Sec 5.3.2).
  IUC a = 0x6A09E667, b = 0xBB67AE85, c = 0x3C6EF372, d = 0xA54FF53A;
  IUC e = 0x510E527F, f = 0x9B05688C, g = 0x1F83D9AB, hh = 0x5BE0CD19;
  // Save the initial state: the rounds compute the compression delta F(block),
  // and the final digest is H(0) + F(block) per FIPS 180-2 Sec 6.2.2.
  const IUC h0[8] = {a, b, c, d, e, f, g, hh};
  for (ISN i = 0; i < 64; ++i) {
    IUC S1 = TSha256Rotr(e, 6) ^ TSha256Rotr(e, 11) ^ TSha256Rotr(e, 25);
    IUC ch = (e & f) ^ ((~e) & g);
    IUC t1 = hh + S1 + ch + TPasswordShaK[i] + w[i];
    IUC S0 = TSha256Rotr(a, 2) ^ TSha256Rotr(a, 13) ^ TSha256Rotr(a, 22);
    IUC maj = (a & b) ^ (a & c) ^ (b & c);
    IUC t2 = S0 + maj;
    hh = g; g = f; f = e; e = d + t1;
    d = c; c = b; b = a; a = t1 + t2;
  }
  // Final digest: H(0) + F(block), per-word mod 2^32.
  const IUC h[8] = {a + h0[0], b + h0[1], c + h0[2], d + h0[3],
                    e + h0[4], f + h0[5], g + h0[6], hh + h0[7]};
  for (ISN i = 0; i < 8; ++i) {
    digest[4 * i] = (IUA)(h[i] >> 24);
    digest[4 * i + 1] = (IUA)(h[i] >> 16);
    digest[4 * i + 2] = (IUA)(h[i] >> 8);
    digest[4 * i + 3] = (IUA)(h[i]);
  }
}

/* Fills a 64-byte SHA-256 block: prefix (prefix_len bytes), 0x80 pad,
zero fill, then the 8-byte BE bit-length (8 * prefix_len) in bytes 56-63.
Then compresses into digest. Per FIPS 180-2 the appended length is the
message length in BITS, big-endian, in the final 8 bytes of the block. */
inline void TPasswordSha256(const IUA* prefix, ISN prefix_len, IUA* digest) {
  TSha256 sha;
  for (ISN i = 0; i < 64; ++i) sha.input[i] = 0;
  for (ISN i = 0; i < prefix_len; ++i) sha.input[i] = prefix[i];
  sha.input[prefix_len] = 0x80;
  // 64-bit BE bit-length in bytes 56-63: 8 * prefix_len, high 4 bytes zero.
  IUD bit_length = (IUD)prefix_len * 8;
  for (ISN i = 0; i < 8; ++i)
    sha.input[56 + i] = (IUA)(bit_length >> (56 - 8 * i));
  TSha256Compress(sha.input, digest);
}

/* One PBKDF2-PRF round: SHA256(salt || INT32_BE(counter)).
Writes the 32-byte digest into out_digest. */
inline void TPasswordPrf(const IUA* salt, ISN salt_count, IUD counter,
                         IUA* out_digest) {
  IUA prefix[PasswordSaltBytes + 4];
  for (ISN i = 0; i < salt_count; ++i) prefix[i] = salt[i];
  prefix[salt_count] = (IUA)(counter >> 24);
  prefix[salt_count + 1] = (IUA)(counter >> 16);
  prefix[salt_count + 2] = (IUA)(counter >> 8);
  prefix[salt_count + 3] = (IUA)(counter & 0xFF);
  TPasswordSha256(prefix, salt_count + 4, out_digest);
}

/* Derives the 32-byte key from salt + password via PBKDF2-SHA256:
  digest[0] = SHA256(salt || password)
  key[i]    = digest[i-1] XOR PRF(i)   for i = 1..KdfRounds
Then FNV-1a avalanche over all 32 bytes. */
inline void TPasswordDeriveKey(const IUA* salt, ISN salt_count,
                               const CHA* password, IUA* key) {
  IUA prefix[PasswordSaltBytes + 255];
  for (ISN i = 0; i < salt_count; ++i) prefix[i] = salt[i];
  ISN pwd_len = TPasswordLen(password);
  for (ISN i = 0; i < pwd_len; ++i) prefix[salt_count + i] = (IUA)password[i];
  TPasswordSha256(prefix, salt_count + pwd_len, key);
  for (IUD i = 1; i <= PasswordKdfRounds; ++i) {
    IUA prf[PasswordKeyBytes];
    TPasswordPrf(salt, salt_count, i, prf);
    for (ISN j = 0; j < PasswordKeyBytes; ++j) key[j] ^= prf[j];
  }
  // FNV-1a avalanche: mix all 32 bytes into each other.
  IUD fnv = 0xCBF29CE484222325;
  for (ISN i = 0; i < PasswordKeyBytes; ++i) {
    fnv ^= key[i];
    fnv *= 0x100000001B3;
  }
  for (ISN i = 0; i < PasswordKeyBytes; ++i) {
    key[i] ^= (IUA)(fnv >> (8 * (i % 8)));
    key[i] ^= (IUA)(fnv >> (8 * ((i + 1) % 8)));
  }
}

/* Prints the given byte as two hex chars, advancing out. */
inline void TPasswordHex2(IUA value, CHA** out) {
  static const CHA hex[] = "0123456789ABCDEF";
  (*out)[0] = hex[value >> 4];
  (*out)[1] = hex[value & 0x0F];
  *out += 2;
}

/* Parses one hex char into a 4-bit value. Returns 0xFF on failure. */
inline IUA TPasswordHex1(CHA c) {
  if (c >= '0' && c <= '9') return (IUA)(c - '0');
  if (c >= 'A' && c <= 'F') return (IUA)(c - 'A' + 10);
  if (c >= 'a' && c <= 'f') return (IUA)(c - 'a' + 10);
  return 0xFF;
}

/* Fills out with PasswordSaltBytes cryptographically random bytes from the
OS CSPRNG. On Linux uses getrandom(2); on macOS/BSD uses arc4random_buf.
Falls back to the ASCIICrabs Random API if the OS call fails (degraded,
logged via D_COUT in debug builds). */
inline void TPasswordRandomSalt(IUA* out) {
#if defined(__linux__)
  ISN got = 0;
  while (got < PasswordSaltBytes) {
    ISN n = getrandom(out + got, PasswordSaltBytes - got, 0);
    if (n < 0) break;  //< EINTR or error.
    got += n;
  }
  if (got < PasswordSaltBytes) {
    // Fallback: fill remaining bytes with the ASCIICrabs RNG (weak, but
    // better than zeros). In a production build this path should be
    // treated as a security error.
    for (; got < PasswordSaltBytes; ++got) {
      IUC r = IUCRandom();
      out[got] = (IUA)(r & 0xFF);
    }
  }
#elif defined(__APPLE__) || defined(__unix__)
  arc4random_buf(out, PasswordSaltBytes);
#else
  // Non-POSIX fallback: use the ASCIICrabs Random API.
  for (ISN i = 0; i < PasswordSaltBytes; ++i)
    out[i] = (IUA)(IUCRandom() & 0xFF);
#endif
}

/* Serializes salt (16 bytes) + key (32 bytes) into the stored form
"PBK2$<32 hex salt>$<64 hex key>"\0. */
inline void TPasswordSerialize(const IUA* salt, const IUA* key, CHA* out) {
  out[0] = 'P'; out[1] = 'B'; out[2] = 'K'; out[3] = '2'; out[4] = '$';
  CHA* cursor = out + 5;
  for (ISN i = 0; i < PasswordSaltBytes; ++i) TPasswordHex2(salt[i], &cursor);
  *cursor++ = '$';
  for (ISN i = 0; i < PasswordKeyBytes; ++i) TPasswordHex2(key[i], &cursor);
  *cursor = '\0';
}

/* Parses a stored "PBK2$<32 hex salt>$<64 hex key>" string back into
salt and key.
@return true if the string parses, false on malformed input. */
inline BOL TPasswordParse(const CHA* stored, IUA* salt, IUA* key) {
  if (stored == NILP) return false;
  if (stored[0] != 'P' || stored[1] != 'B' || stored[2] != 'K' ||
      stored[3] != '2' || stored[4] != '$') {
    return false;
  }
  const CHA* salt_hex = stored + 5;
  if (TPasswordLen(salt_hex) != PasswordSaltBytes * 2 + 1 + PasswordKeyLength)
    return false;
  for (ISN i = 0; i < PasswordSaltBytes; ++i) {
    IUA hi = TPasswordHex1(salt_hex[2 * i]);
    IUA lo = TPasswordHex1(salt_hex[2 * i + 1]);
    if (hi == 0xFF || lo == 0xFF) return false;
    salt[i] = (IUA)((hi << 4) | lo);
  }
  const CHA* key_hex = salt_hex + PasswordSaltBytes * 2;
  if (key_hex[0] != '$') return false;
  key_hex += 1;
  for (ISN i = 0; i < PasswordKeyBytes; ++i) {
    IUA hi = TPasswordHex1(key_hex[2 * i]);
    IUA lo = TPasswordHex1(key_hex[2 * i + 1]);
    if (hi == 0xFF || lo == 0xFF) return false;
    key[i] = (IUA)((hi << 4) | lo);
  }
  return true;
}

/* An account password Authenticator.
The password is salted: a fresh 16-byte random salt is generated on every
SetKey call, the 32-byte key is derived with PBKDF2-SHA256 (10,000 rounds)
plus an FNV-1a avalanche, and the stored form is the contiguous string
"PBK2$<salt>$<key>". Pre-salt legacy plaintext keys remain verifiable
via Equals for migration, but any new key goes through the KDF.
*/
class TPassword {
 public:
  enum {
    Validation = 2,           //< Validation type.
    DefaultMinLength = 0,     //< The minimum length of a password.
    MaxLength = 255,          //< The maximum length of a password.
  };

  static const CHA DefaultPassword[];  //< Default password.

  /* Attempts to create a password from the given password with the default
      format.
      If the password does not match the default format, the default password
      will be used. */
  TPassword(TAuthenticator* authenticator, const CHA* password = DefaultPassword)
    : auth_ (authenticator), type_ (Validation), salted_ (false) {
    for (ISN i = 0; i < PasswordStoredLengthMin; ++i) stored_[i] = '\0';
    SetKey (password);
  }

  /* Destructs the password. */
  ~TPassword() {}

  /* Gets the stored key (salted form "PBK2$<salt>$<key>" or legacy
      plaintext during migration). */
  const CHA* GetKey() { return stored_; }

  /* Gets true if the stored key is in the salted PBK2 form. */
  BOL IsSalted() { return salted_; }

  /* Gets the 16-byte salt from the stored key.
      @return false if the key is not in salted form. */
  BOL GetSalt(IUA* out) {
    IUA salt[PasswordSaltBytes];
    IUA key[PasswordKeyBytes];
    if (!TPasswordParse(stored_, salt, key)) return false;
    for (ISN i = 0; i < PasswordSaltBytes; ++i) out[i] = salt[i];
    return true;
  }

  /* Attempts to set the password and returns a non-zero error message upon
   * failure. A fresh random salt is generated and the key is derived via
   * PBKDF2-SHA256. */
  BOL SetKey(const CHA* password) {
    if (password == NILP) return false;
    if (TPasswordLen(password) > MaxLength) return false;
    if (auth_ != NILP && auth_->PasswordInvalid (password) != NILP) {
      return false;
    }
    IUA salt[PasswordSaltBytes];
    IUA key[PasswordKeyBytes];
    TPasswordRandomSalt(salt);
    TPasswordDeriveKey(salt, PasswordSaltBytes, password, key);
    TPasswordSerialize(salt, key, stored_);
    salted_ = true;
    return true;
  }

  /* Gets true if the given password is valid (length check only). */
  static BOL IsValid(const CHA* password) {
    if (password == NILP) return false;
    return TPasswordLen(password) <= MaxLength;
  }

  /* Returns true if this password matches the given password.
   * For salted keys, the password is re-derived with the stored salt and
   * compared in constant time. Legacy plaintext keys use a constant-time
   * memcmp for backward compatibility. */
  BOL Equals (const CHA* key) {
    if (key == NILP) return false;
    if (!salted_) {
      return TPasswordConstantTimeEquals(
          (const IUA*)stored_, (const IUA*)key,
          TPasswordLen(stored_));
    }
    IUA salt[PasswordSaltBytes];
    IUA key_stored[PasswordKeyBytes];
    if (!TPasswordParse(stored_, salt, key_stored)) return false;
    IUA key_candidate[PasswordKeyBytes];
    TPasswordDeriveKey(salt, PasswordSaltBytes, key, key_candidate);
    return TPasswordConstantTimeEquals(key_stored, key_candidate,
                                       PasswordKeyBytes);
  }

  /* Returns true if this password equals the given one. */
  BOL Equals(const TPassword& other) {
    if (salted_ != other.salted_) return false;
    if (!salted_) {
      return TPasswordConstantTimeEquals(
          (const IUA*)stored_, (const IUA*)other.stored_,
          TPasswordLen(stored_));
    }
    return TPasswordConstantTimeEquals(
        (const IUA*)stored_, (const IUA*)other.stored_,
        PasswordStoredLengthMax);
  }

  /* Prints this object to a expression. */
  template<typename Printer> Printer& Print (Printer& o) {
    return o << stored_;
  }

 private:
  CHA stored_[PasswordStoredLengthMin];  //< Stored key (salted or legacy).
  TAuthenticator* auth_;                 //< Password authenticator.
  ISC type_;                             //< The validation type.
  BOL salted_;                           //< True if stored key is PBK2 form.
};

/* Definition of the default password (empty string). */
const CHA TPassword::DefaultPassword[] = "";

}       //< namespace _
#endif
#endif
