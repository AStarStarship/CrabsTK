// Copyright AStarship <https://astarship.net>.
//
// Standalone test for the CrabsTK Env module (TEnvLoad/Get/GetI/Export).
// Builds with a minimal type shim so it runs without the full ASCIICrabs
// config chain.
//
// Build with:
//   g++ -std=c++23 -Wall -Wextra -Wpedantic -Werror
//       -IEnv/Tests -IEnv Env/Tests/EnvTest.cpp -o /tmp/env_tests

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

// _Config.h shim provides CHA/IUC/ISC etc. types.
#include "_Config.h"

// Root _Seams.h (pulled in by EnvVar.h) defines CRABSTK_ENV_VAR.
// SEAM must be >= CRABSTK_ENV_VAR to enable the module.
#define SEAM 100

#include "EnvVar.h"
#include "EnvVar.hxx"

using namespace _;

static int passed = 0;
static int failed = 0;

#define CHECK(name, cond) do { \
  if (cond) { ++passed; printf("  PASS: %s\n", name); } \
  else { ++failed; printf("  FAIL: %s\n", name); } \
} while (0)

/* Write a temp .env file; returns path. */
static CHA* WriteTempEnv(const CHA* content) {
  const CHA* path = "/tmp/crabstk_env_test.env";
  FILE* fp = fopen(path, "w");
  if (fp == nullptr) return nullptr;
  fputs(content, fp);
  fclose(fp);
  return (CHA*)path;
}

void TestBasicLoad() {
  printf("BasicLoad:\n");
  CHA* path = WriteTempEnv(
    "# comment line\n"
    "EMPTY_KEY=\n"
    "SLAVE_HOST=192.168.50.221\n"
    "SLAVE_PORT=2222\n"
    "SLAVE_USERNAME=astarcale\n"
    "SLAVE_PASSWORD=secret123\n"
    "  SPACED_KEY =  spaced value  \n"
    "UNDEFINED_KEY\n"
    "DUP_KEY=first\n"
    "DUP_KEY=second\n"
  );
  CHECK("temp file written", path != nullptr);
  if (path == nullptr) return;

  TEnvTable<8> table;
  memset(&table, 0, sizeof(table));
  EnvStatus st = TEnvLoad<8>(path, &table);
  CHECK("load returns EnvOK", st == EnvOK);
  CHECK("count == 7 (comments/blanks/malformed/dup skipped)", table.count == 7);
  CHECK("count_max == 8", table.count_max == 8);

  const CHA* host = TEnvGet<8>(table, "SLAVE_HOST");
  CHECK("SLAVE_HOST found", host != nullptr);
  CHECK("SLAVE_HOST value", host != nullptr && strcmp(host, "192.168.50.221") == 0);

  ISC port = 0;
  st = TEnvGetI<8>(table, "SLAVE_PORT", &port);
  CHECK("SLAVE_PORT numeric parse", st == EnvOK && port == 2222);

  const CHA* user = TEnvGet<8>(table, "SLAVE_USERNAME");
  CHECK("SLAVE_USERNAME value", user != nullptr && strcmp(user, "astarcale") == 0);

  const CHA* pass = TEnvGet<8>(table, "SLAVE_PASSWORD");
  CHECK("SLAVE_PASSWORD value", pass != nullptr && strcmp(pass, "secret123") == 0);

  // Spaced key/value trimmed.
  const CHA* spaced = TEnvGet<8>(table, "SPACED_KEY");
  CHECK("SPACED_KEY trimmed value", spaced != nullptr && strcmp(spaced, "spaced value") == 0);

  // Empty value.
  const CHA* empty = TEnvGet<8>(table, "EMPTY_KEY");
  CHECK("EMPTY_KEY found", empty != nullptr);
  CHECK("EMPTY_KEY is empty string", empty != nullptr && empty[0] == '\0');
  st = TEnvGetI<8>(table, "EMPTY_KEY", &port);
  CHECK("EMPTY_KEY numeric parse fails", st == EnvEmpty);

  // Duplicate: first wins (DUP_KEY=first).
  const CHA* dup = TEnvGet<8>(table, "DUP_KEY");
  CHECK("DUP_KEY first wins", dup != nullptr && strcmp(dup, "first") == 0);

  // Not found.
  const CHA* missing = TEnvGet<8>(table, "NO_SUCH_KEY");
  CHECK("missing key returns NILP", missing == nullptr);
  st = TEnvGetI<8>(table, "NO_SUCH_KEY", &port);
  CHECK("missing key numeric returns EnvNotFound", st == EnvNotFound);

  // TEnvFind.
  ISN idx = TEnvFind<8>(table, "SLAVE_PORT");
  CHECK("TEnvFind finds SLAVE_PORT", idx >= 0);
  idx = TEnvFind<8>(table, "NO_SUCH_KEY");
  CHECK("TEnvFind missing returns -1", idx == -1);

  // Line numbers.
  idx = TEnvFind<8>(table, "SLAVE_HOST");
  CHECK("SLAVE_HOST on line 3", idx >= 0 && table.entries[idx].line_number == 3);

  unlink("/tmp/crabstk_env_test.env");
}

void TestCapacity() {
  printf("Capacity:\n");
  CHA buf[2048];
  buf[0] = '\0';
  // Write 10 entries but table capacity is 4.
  int offset = 0;
  for (int i = 0; i < 10; ++i) {
    offset += snprintf(buf + offset, sizeof(buf) - (size_t)offset, "KEY_%d=val%d\n", i, i);
  }
  CHA* path = WriteTempEnv(buf);
  CHECK("capacity temp written", path != nullptr);
  if (path == nullptr) return;

  TEnvTable<4> table;
  memset(&table, 0, sizeof(table));
  EnvStatus st = TEnvLoad<4>(path, &table);
  CHECK("over-capacity returns EnvCapacity", st == EnvCapacity);

  unlink("/tmp/crabstk_env_test.env");
}

void TestExport() {
  printf("Export:\n");
  // Clean slate.
  unsetenv("CRABSTK_TEST_HOST");
  unsetenv("CRABSTK_TEST_PORT");
  unsetenv("CRABSTK_TEST_EXISTING");
  setenv("CRABSTK_TEST_EXISTING", "already_set", 1);

  CHA* path = WriteTempEnv(
    "CRABSTK_TEST_HOST=10.0.0.1\n"
    "CRABSTK_TEST_PORT=8080\n"
    "CRABSTK_TEST_EXISTING=should_not_override\n"
  );
  CHECK("export temp written", path != nullptr);
  if (path == nullptr) return;

  TEnvTable<4> table;
  memset(&table, 0, sizeof(table));
  EnvStatus st = TEnvLoad<4>(path, &table);
  CHECK("export load OK", st == EnvOK && table.count == 3);

  ISN exported = TEnvExport<4>(table);
  CHECK("exported 2 (existing skipped)", exported == 2);

  const CHA* h = getenv("CRABSTK_TEST_HOST");
  CHECK("CRABSTK_TEST_HOST in env", h != nullptr && strcmp(h, "10.0.0.1") == 0);
  const CHA* p = getenv("CRABSTK_TEST_PORT");
  CHECK("CRABSTK_TEST_PORT in env", p != nullptr && strcmp(p, "8080") == 0);
  const CHA* e = getenv("CRABSTK_TEST_EXISTING");
  CHECK("existing key not overridden", e != nullptr && strcmp(e, "already_set") == 0);

  unsetenv("CRABSTK_TEST_HOST");
  unsetenv("CRABSTK_TEST_PORT");
  unsetenv("CRABSTK_TEST_EXISTING");
  unlink("/tmp/crabstk_env_test.env");
}

void TestMissingFile() {
  printf("MissingFile:\n");
  TEnvTable<4> table;
  memset(&table, 0, sizeof(table));
  EnvStatus st = TEnvLoad<4>("/tmp/crabstk_env_missing_file.env", &table);
  CHECK("missing file returns EnvIO", st == EnvIO);
  CHECK("missing file count untouched", table.count == 0);
}

int main() {
  printf("CrabsTK Env tests\n");
  TestBasicLoad();
  TestCapacity();
  TestExport();
  TestMissingFile();
  printf("\n%d passed, %d failed\n", passed, failed);
  return failed == 0 ? 0 : 1;
}
