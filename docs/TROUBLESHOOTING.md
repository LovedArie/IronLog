# Troubleshooting Log

A symptom-indexed log of build/run problems and their fixes. When you hit an
error, **search this file for the error text** — entries are keyed on the
symptom you actually see, not on the order they happened.

> **Using this as a template for a new project?**
> Keep everything from here down to the `── LOGGED ISSUES ──` divider — that part
> is reusable. Delete the project-specific entries *below* the divider and start
> a fresh log for the new project.

---

## How to debug a build/run failure (general method)

Before reaching for a specific fix, find out **which stage failed**. A C++
program passes through three stages, each with its own failure mode and its own
class of fix. Reading the stage first stops you from fixing the wrong thing.

```
compile  →  is this one .cpp valid?        (compiler)   → "error:", "not declared"
link     →  do all the pieces connect?     (linker)     → "undefined reference"
load/run →  can it find its DLLs at start? (OS loader)  → exit 0xC0000135, no window
```

Three habits that resolved every issue in this log:

1. **Read which stage failed.** The error text tells you: a compiler `error:`, a
   linker `undefined reference`, or a launch failure with an exit code. Fix at
   the stage that actually broke.
2. **Verify with artifacts, not theories.** When unsure, look at the real files
   the build produced (generated sources, object files, the exit code) instead
   of guessing. The evidence beats confident reasoning every time.
3. **When targeted fixes keep missing, stop and research.** If two or three
   aimed fixes don't move the error, you have the cause wrong. Search the exact
   error text; read official/community sources; *then* act. One evidence-based
   fix beats six guesses.

---

## Entry format

Every logged issue uses the same four-part shape, because a stuck reader wants
the same things in the same order:

- **SYMPTOM** — the exact error or behavior, copy-pasteable (what you search for).
- **CAUSE** — why it happens (so you understand it, not just paste a fix).
- **FIX** — the precise steps that resolved it.
- **PREVENT** — how to stop it recurring (the part that makes this log compound).

### Blank template (copy this for a new entry)

```markdown
### <short symptom title>

**SYMPTOM**
<exact error text or observed behavior>

**CAUSE**
<why it happens>

**FIX**
<the steps that resolved it>

**PREVENT**
<the habit / setting / check that stops it recurring>
```

---

## ── LOGGED ISSUES (IronLog) ──

*Everything below is specific to this project. Delete it when reusing this file
as a template.*

> **Lessons pre-paid from earlier projects** — already handled in this
> codebase: every `Q_OBJECT` header listed in its CMake target; Qt modules
> added in both `find_package` and `target_link_libraries`; core code takes
> "now" as a parameter instead of reading the clock; only the application
> name is set (no doubled `IronLog/IronLog/` data folder); colours come from
> the palette, never a stylesheet + palette mix. The first four entries below
> are carried over from the Password Manager / TickTimer logs because they
> apply to any Qt app on Windows.

---

### App built fine but won't launch — exit code `-1073741515`

**SYMPTOM**
Build reaches `[100%] Built target IronLog`, but running the `.exe` from a
plain terminal opens no window and prints nothing:
```powershell
.\IronLog.exe; echo "Exit code: $LASTEXITCODE"
# Exit code: -1073741515
```
(`-1073741515` = `0xC0000135`, "a required DLL was not found".)

**CAUSE**
Qt Creator quietly puts Qt's `bin` folder on the DLL path when it launches
a program. A plain terminal doesn't, so Windows can't find `Qt6Core.dll`,
`Qt6Widgets.dll`, or the MinGW runtime DLLs, and the process dies before
`main()`. This is a deploy problem, not a build problem.

**FIX**
From the **Qt command prompt** (Start menu → "Qt 6.11.0 (MinGW 64-bit)"),
in the build folder:
```
windeployqt IronLog.exe
```

**PREVENT**
A clean build says nothing about whether the `.exe` can find its DLLs.
Run from Qt Creator during development; run `windeployqt` once per new
build folder before using a terminal or copying the app to another PC.

---

### "no Qt platform plugin could be initialized"

**SYMPTOM**
`IronLog.exe` starts, then a dialog: *"This application failed to start
because no Qt platform plugin could be initialized."* Console:
`Could not find the Qt platform plugin "windows" in ""`.

**CAUSE**
A GUI Qt app needs `platforms\qwindows.dll` in a subfolder beside the exe.
Copying DLLs by hand misses it.

**FIX**
`windeployqt IronLog.exe` (it creates `platforms\`, `styles\`, …).

**PREVENT**
Deploy with `windeployqt`, never by hand-copying DLLs. Note the test exes
(`test_core`, `test_services`) don't need this — they use no GUI.

---

### `undefined reference to vtable for <Class>` (a `Q_OBJECT` class)

**SYMPTOM**
Compiles, then the **link** fails:
`undefined reference to 'vtable for ironlog::TrainingController'` (or to
one of its signals).

**CAUSE**
`Q_OBJECT` classes need code generated by `moc`. CMake's AUTOMOC only runs
moc on headers it can see. Our headers live in `include/`, away from their
`.cpp`, so a header that isn't in the target's source list is never scanned.

**FIX**
List the header in the target, next to its `.cpp`, in `CMakeLists.txt`:
```cmake
add_library(ironlog_services STATIC
    src/services/TrainingController.cpp
    include/services/TrainingController.h   # makes AUTOMOC scan it
)
```

**PREVENT**
New `Q_OBJECT` class = add the `.h` and `.cpp` to CMake in the same edit.

---

### CMake picks the wrong compiler / can't find `Qt6Config.cmake`

**SYMPTOM**
After deleting `build/` and configuring again, CMake reports MSVC or an
MSYS2 GCC instead of Qt's MinGW, or fails with
`Could not find a package configuration file provided by "Qt6"`.

**CAUSE**
The compiler choice is stored **inside** `build/`. Delete it, and CMake falls
back to the system default, which isn't the compiler Qt was built with.

**FIX**
Configure from Qt Creator (it applies the kit), or from the Qt command
prompt with the prefix path:
```
cmake -G "MinGW Makefiles" -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"
```

**PREVENT**
If a build breaks "for no reason" right after a clean, suspect the
toolchain before the code.

---

### `invalid use of incomplete type 'class QJsonArray'`

**SYMPTOM**
A **compile** error in a file that uses `QJsonArray` methods, e.g.
```
tests/test_services.cpp:104: error: invalid use of incomplete type 'class QJsonArray'
```

**CAUSE**
The file included `<QJsonDocument>` and `<QJsonObject>` but not
`<QJsonArray>`. Those headers only **forward-declare** `QJsonArray`
(`class QJsonArray;` — "this type exists"), which is enough to mention the
name but not to call its methods. Calling `.toArray().at(0)` needs the full
class definition.

**FIX**
Add the missing include:
```cpp
#include <QJsonArray>
```

**PREVENT**
"Incomplete type" almost always means "you saw a forward declaration, not
the real header." Include the header for **every** Qt class you call methods
on, even if it compiled in another file (that file may have got it
indirectly).

---

### Labels show `set(s)` / `PR(s)` literally instead of `1 set` / `5 sets`

**SYMPTOM**
UI text reads `17 set(s)` or `1 session(s)` — the plural marker is printed
as-is.

**CAUSE**
`tr("%n set(s)", nullptr, n)` is Qt's plural mechanism, but the *choice*
between "set" and "sets" comes from a **translation file** (`.qm`). With no
translator installed, Qt just substitutes the number and keeps the source
text, `(s)` and all.

**FIX**
Use a small helper that picks the word itself (`services/QtConvert.h`):
```cpp
countOf(n, tr("set"), tr("sets"))   // "1 set", "17 sets"
```

**PREVENT**
Only use `%n` plurals if the project ships `.ts`/`.qm` translation files
(even an English one). Otherwise pluralise in code.

---

### CI Windows job: `Failed to locate XML data for Qt version '6.11.3'`

**SYMPTOM**
The *Install Qt* step of the Windows job fails within a minute:
```
aqt list-qt windows desktop --spec 6.11.* --latest-version   ->  6.11.3
WARNING : Failed to download checksum for the file 'online/qtsdkrepository/
          windows_x86/desktop/qt6_6113/qt6_6113/Updates.xml'.
ERROR   : Failed to locate XML data for Qt version '6.11.3'.
```
The core-only and Linux jobs pass. (The `python.EXE ... EACCES` lines
around it are harmless runner noise.)

**CAUSE**
`jurplel/install-qt-action` uses **aqtinstall** to download Qt. The
wildcard `6.11.*` resolved to the newest patch (6.11.3), but aqtinstall
3.3.0 looked for its package list in a folder layout that doesn't match
where Qt publishes 6.11 builds (per-compiler folders such as
`qt6_6111/qt6_6111_mingw/`). Each aqtinstall release only knows the Qt
versions and layouts it was built for, so very new Qt versions often fail
until aqtinstall catches up.

**FIX**
Pin a version aqtinstall knows: `version: '6.8.3'` (Qt 6 LTS) in
`ci.yml` and `release.yml`. Same MinGW 13.1 compiler family as 6.11.

**PREVENT**
- Never use a wildcard Qt version in CI: `6.11.*` silently became "the
  newest patch", which is exactly the version tooling knows least.
- Pin, and bump deliberately: try `version: '6.11.x'` on a branch when
  aqtinstall releases a new version; merge only if the run is green.
- Cover the version range with different jobs: Linux = 6.4 (apt, oldest
  supported), Windows CI = 6.8 LTS, owner's PC = 6.11.

---

### CI Windows: the first test hangs for 17+ minutes (and never prints anything)

**SYMPTOM**
Windows job: Configure and Build pass, then the Test step prints
`Start 1: core` and nothing else until the job is cancelled. The hanging
test (`test_core`) doesn't even use Qt. Linux jobs pass.

**CAUSE**
The step ran in **Git Bash**, which puts Git for Windows' own folders
first on PATH. A diagnostic step proved it — the first
`libstdc++-6.dll` each shell finds:
```
Git Bash:    /ucrt64/bin/libstdc++-6.dll           <- Git's, different C runtime
PowerShell:  D:\a\IronLog\Qt\Tools\mingw1310_64\bin\libstdc++-6.dll   <- the right one
```
(The runner holds six copies in total: Git, Qt's MinGW, Qt's bin,
`C:\mingw64`, two in Strawberry Perl.) The test loaded Git's runtime,
which doesn't match the MinGW 13.1 it was compiled with; Windows showed a
"procedure entry point not found" popup — on a machine with no screen,
so it waited forever. Same family as "exit code -1073741515": a C++ exe
is only as good as the DLLs it finds at launch.

**FIX**
- Run `ctest` (and `windeployqt`, and the packaged-app smoke test) with
  `shell: pwsh`; PowerShell's PATH starts with Qt's MinGW bin from
  `GITHUB_PATH`.
- `ctest --timeout 120` and `timeout-minutes` on every job, so any future
  hang fails in minutes, not 25.

**PREVENT**
- On Windows CI, anything that RUNS a MinGW-built exe uses PowerShell.
  Git Bash is fine for scripting (`cygpath`, `gh`), not for launching.
- Every CI job and every test gets a timeout. A test that can hang will.
- When a fix "works", keep the evidence: the diagnostic step stays in
  `ci.yml` so the next DLL surprise is visible in the log.

