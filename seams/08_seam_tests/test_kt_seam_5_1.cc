#pragma once

#include "nmist_importer.h"

#include "../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../../cpputest/include/CppUTest/TestHarness.h"


TEST_GROUP (MnistTestGroup) {
};

TEST (MnistTestGroup, NmistImporterTests) {
    cout << "\nTesting EndianSwap";

    static const uint32_t expected_endian = ((uint32_t)'a' << 24)
        | ((uint32_t)'b' << 16)
        | ((uint32_t)'d' << 8)
        | ((uint32_t)'d');

    CHECK_EQUAL (expected_endian, EndianSwap (EndianSwap (expected_endian)))

    MnistImport ();
}
