# CrabsTK Agent Plan

## Overarching Goal
Complete the migration of `CrabsTK` to the new `ASCIICrabs` schema-first architecture, ensuring the `_Seams` test tree compiles successfully under C++23 (`g++ -std=c++2b`).

## Status: BUILD SUCCESSFUL
The `_Seams` test tree now compiles and links successfully. The resulting `_CrabsTK` executable runs and reports:
```
Unit tests completed successfully! (:-)+=<
```

## Progress So Far
- **Architecture Migration:** Extracted legacy `.inl` files into `.h` and `.hxx` single-translation-unit builds. Created `_Package.hxx` rollups.
- **Branding Migration:** Renamed `KabukiToolkit` to `CrabsTK`. Migrated namespaces from `Kabuki::Toolkit` to `CT`.
- **STB Library Reconstruction:** Repaired `Code/stb_c_lexer.hxx` and `Code/stb_leakcheck.hxx` by re-injecting missing macros, enums, and structs.
- **Config Cleanup:** Resolved `SEAM` and `SEAM_N` collisions.
- **Test Tree Wiring:** Fixed `_Main.cpp` to include `ASCIICrabs/Test.hpp`, `Test.hxx`, `COut.hxx`, `AType.hxx`, `Array.hxx`, `Puff.hxx`, and `Stringf.hxx` in the correct order.
- **Linking:** Successfully linked against `-lGL` for the `Image` module.

## Next Steps (Optional Enhancements)
- **CI/CD:** Set up a build script or Makefile to automate the compilation.
- **Documentation:** Update the README with build instructions.
- **Investor Demo:** Create a demo script that showcases the key features.

## Definition of Done
The command `cd _Seams && g++ -std=c++2b -Wall -Wextra _Main.cpp -I. -I.. -I../../ -o _CrabsTK -lGL` exits with `0` and produces a working executable. **ACHIEVED.**