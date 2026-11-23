// test_main.cpp — Catch2's main(), compiled ONCE and shared by every test
// executable (it's an OBJECT library in CMakeLists.txt). catch.hpp is huge;
// keeping main() in its own file means editing a test doesn't recompile it.
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
