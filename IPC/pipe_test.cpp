// Copyright AStarship <https://astarship.net>.
/// In-process pipe test for inter-process RPC protocol testing.
///
/// Tests the Pipe class by writing a known payload from master to slave,
/// then verifying the slave reads back the exact same data.
///
/// Usage: ./pipe_test
/// Expected: "PIPE_TEST_PASS\n" on success, "PIPE_TEST_FAIL\n" on failure.

#include "Pipe.hxx"
#include "_Config.h"
#include <cstdio>
#include <cstring>
#include <cerrno>

int main() {
  printf("Starting in-process pipe test for inter-process RPC protocol...\n");

  // Get the shared pipe instance (master/slave shared memory)
  _::Pipe& pipe = _::Pipe::Master();

  if (!pipe) {
    printf("FAIL: Could not create pipe pair\n");
    return 1;
  }

  // Data to send
  const char test_payload[] = "Hello, Crabs RPC Pipeline!";
  ISN payload_size = ISN(strlen(test_payload));

  printf("Payload: '%s' (%d bytes)\n", test_payload, payload_size);

  // Master writes to pipe (via the shared pipe instance)
  ISN bytes_written = pipe.Write(test_payload, payload_size);
  if (bytes_written < 0) {
    printf("FAIL: Master write returned error (%d)\n", bytes_written);
    return 1;
  }
  printf("Master wrote: %d bytes\n", bytes_written);

  // Check if data is available
  if (!pipe.HasData()) {
    printf("FAIL: Pipe reports no data available after master write\n");
    return 1;
  }
  printf("Pipe reports data available: true\n");

  // Slave reads from pipe (same shared instance)
  char read_buf[256];
  ISN bytes_read = pipe.Read(read_buf, sizeof(read_buf));
  if (bytes_read < 0) {
    printf("FAIL: Slave read returned error (%d)\n", bytes_read);
    return 1;
  }
  printf("Slave read: %d bytes\n", bytes_read);

  // Null-terminate and compare
  read_buf[bytes_read] = '\0';

  // Compare
  if (strcmp(read_buf, test_payload) == 0) {
    printf("PASS: Data matches! '%s'\n", read_buf);
    printf("PIPE_TEST_PASS\n");
    return 0;
  } else {
    printf("FAIL: Data mismatch!\n");
    printf("  Expected: '%s'\n", test_payload);
    printf("  Got:      '%s'\n", read_buf);
    printf("PIPE_TEST_FAIL\n");
    return 1;
  }
}