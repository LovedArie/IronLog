# IronLog — a bodybuilding training log (C++17 + Qt 6 Widgets).
#
# Three layers, one target each (folder names match):
#   ironlog_core      core/     — pure C++17 domain. NO Qt. Builds anywhere.
#   ironlog_services  services/ — controller + JSON store. Qt Core only.
#   IronLog           ui/       — Qt Widgets: window, models, chart.
#   test_*            tests/    — Catch2 tests (catch.hpp ships in include/).
# Arrows only point DOWN: ui -> services -> core. Never upward.
#
# The core builds and tests even on a machine without Qt:
#     cmake -S . -B build -DIRONLOG_BUILD_APP=OFF
cmake_minimum_required(VERSION 3.20)

project(IronLog
    VERSION 0.1.0
    DESCRIPTION "Bodybuilding training log: sets, PRs, weekly volume, e1RM progress"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(IRONLOG_BUILD_APP   "Build the Qt Widgets application" ON)
option(IRONLOG_BUILD_TESTS "Build the Catch2 unit tests"      ON)

# Warnings are free bug reports. Turn them on for our own targets only.
function(ironlog_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# 1. Domain core — pure C++. Everything the app KNOWS about training lives
#    here, so it can be unit-tested fast and reused (e.g. a future mobile UI).
# ---------------------------------------------------------------------------
add_library(ironlog_core STATIC
    src/core/Date.cpp
    src/core/Exercise.cpp
    src/core/SetEntry.cpp
    src/core/Workout.cpp
    src/core/Stats.cpp
    src/core/SampleData.cpp
    include/core/Date.h
    include/core/Exercise.h
    include/core/SetEntry.h
    include/core/Workout.h
    include/core/Stats.h
    include/core/SampleData.h
)
# PUBLIC: anything that links ironlog_core also gets include/ on its path.
target_include_directories(ironlog_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
ironlog_warnings(ironlog_core)

# ---------------------------------------------------------------------------
# 2. Qt application
# ---------------------------------------------------------------------------
if(IRONLOG_BUILD_APP)
    # Adding a Qt module is ALWAYS two edits: this COMPONENTS list AND the
    # target_link_libraries line below. (TickTimer lesson — TROUBLESHOOTING.)
    find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets)
    qt_standard_project_setup()   # turns on AUTOMOC / AUTOUIC for Qt targets

    # Services: the application layer (TrainingController) and technical
    # services (JsonStore). Qt Core only — no widgets — so test_services
    # can test the controller without opening a window.
    add_library(ironlog_services STATIC
        src/services/JsonStore.cpp
        src/services/TrainingController.cpp
        include/services/JsonStore.h
        include/services/QtConvert.h
        # Q_OBJECT headers MUST be listed, or AUTOMOC never scans them and
        # the link fails with "undefined reference to vtable".
        # (Password Manager lesson — see docs/TROUBLESHOOTING.md.)
        include/services/TrainingController.h
    )
    target_link_libraries(ironlog_services PUBLIC ironlog_core Qt6::Core)
    ironlog_warnings(ironlog_services)

    qt_add_executable(IronLog
        src/ui/main.cpp
        src/ui/WorkoutTableModel.cpp
        src/ui/WeeklyVolumeModel.cpp
        src/ui/ProgressChart.cpp
        src/ui/AddExerciseDialog.cpp
        src/ui/MainWindow.cpp
        include/ui/WorkoutTableModel.h   # all Q_OBJECT -> all listed
        include/ui/WeeklyVolumeModel.h
        include/ui/ProgressChart.h
        include/ui/AddExerciseDialog.h
        include/ui/MainWindow.h
    )
    target_link_libraries(IronLog PRIVATE ironlog_services Qt6::Widgets)
    # One source of truth for the version: project(VERSION) above.
    target_compile_definitions(IronLog PRIVATE IRONLOG_VERSION="${PROJECT_VERSION}")
    ironlog_warnings(IronLog)

    # WIN32_EXECUTABLE: no extra black console window behind the app on
    # Windows. MACOSX_BUNDLE: a proper .app on macOS.
    set_target_properties(IronLog PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
    )
endif()

# ---------------------------------------------------------------------------
# 3. Tests — run with:  ctest --test-dir build --output-on-failure
# ---------------------------------------------------------------------------
if(IRONLOG_BUILD_TESTS)
    enable_testing()

    # Catch2's main() compiled once, shared by every test executable.
    add_library(catch_main OBJECT tests/test_main.cpp)
    target_include_directories(catch_main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    add_executable(test_core tests/test_core.cpp $<TARGET_OBJECTS:catch_main>)
    target_link_libraries(test_core PRIVATE ironlog_core)
    add_test(NAME core COMMAND test_core)

    if(IRONLOG_BUILD_APP)
        add_executable(test_services tests/test_services.cpp $<TARGET_OBJECTS:catch_main>)
        target_link_libraries(test_services PRIVATE ironlog_services)
        add_test(NAME services COMMAND test_services)
    endif()
endif()
