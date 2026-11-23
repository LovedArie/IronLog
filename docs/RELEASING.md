# Releasing IronLog

> **What this is**: the release routine, with a proof point per step.
> Same idea as TickTimer's `GITHUB.md`, but the build and packaging are
> done by GitHub Actions instead of a script on your PC.

---

## The pipelines

| Workflow | File | Runs when | Does |
|---|---|---|---|
| **CI** | `.github/workflows/ci.yml` | every push to `main`, every pull request | Core-only build (no Qt) · Linux + Qt 6.4 build, tests, app screenshot · Windows + Qt 6.8 LTS MinGW build and tests |
| **Release** | `.github/workflows/release.yml` | a tag like `v0.2.0` is pushed | Checks tag = `project(VERSION)` · builds + tests on Windows · `windeployqt` · smoke-tests the zip with no Qt on PATH · publishes a GitHub Release |

See them run: repo → **Actions** tab.

---

## Cutting a release

1. **Bump the version** in `CMakeLists.txt` → `project(IronLog VERSION 0.2.0 …)`.
   *Proof:* `grep -n "VERSION 0" CMakeLists.txt` shows the new number.
2. **Commit and push** to `main`.
   *Proof:* CI is green on that commit (Actions tab, or the badge in the README).
3. **Tag and push the tag**:
   ```bash
   git tag v0.2.0
   git push origin v0.2.0
   ```
   *Proof:* the **Release** workflow starts in the Actions tab.
4. **Wait for the release job.** It refuses to publish if the tag and
   `CMakeLists.txt` disagree — fix step 1 and re-tag.
   *Proof:* `https://github.com/LovedArie/IronLog/releases/latest` shows
   `IronLog-v0.2.0-windows.zip`.
5. **Download it yourself** and run `IronLog.exe` on your PC.
   *Proof:* *Help → About* shows the new version.

### If a tag was wrong

```bash
git tag -d v0.2.0                 # delete locally
git push origin :refs/tags/v0.2.0 # delete on GitHub
```
Then delete the draft/failed release on GitHub if one was created, fix, and tag again.

---

## Coming later

| Milestone | Added to the Release workflow |
|---|---|
| v0.3 | Android APK (signing key from repo secrets), web (WASM) bundle |
| v0.5 | Deploy server + web files to the VPS over SSH, then fail unless `GET https://ironlog.<domain>/version` answers the new version (backlog I6) |
