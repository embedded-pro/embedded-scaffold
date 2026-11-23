# embedded-scaffold — Agent Rules

<!-- shared:begin 0.2.0 sha256:9aee198041110762 -->
Canonical rules for Claude, Copilot and sub-agents. Sections marked *(shared)* come from embedded-pro/embedded-agent-kit:
edit them there, not here. *(repo)* sections belong to this repository. *Exceptions* overrides a shared rule.

## Behaviour *(shared)*

- **Be as terse as possible.** No preamble, summary or narration. Report file paths and pass/fail.
- Everything you write — code, docs, agent files, PR text — uses the fewest words that stay unambiguous.
- Prose (docs, comments, agent files, commit and PR text) follows ASD-STE100 Simplified Technical English:
  one word, one meaning; active voice; one instruction per sentence; sentences ≤ 20 words (procedures) or ≤ 25 (descriptions).
- Ambiguous request: ask. Clear request: act.
- Do only what was asked. No unrelated refactors, no extra features.
- Batch reads, don't re-read, prefer targeted edits.
- Never change a shared section. Add a deviation to *Exceptions* only when the user approves it.

## Workflow *(shared)*

1. Multi-file or architectural change: update the design document first. Get approval before you write code.
2. Read *Domain* and *Build*. Read each `.github/instructions/*.instructions.md` whose `applyTo` matches a file you change.
3. Find the nearest existing pattern. Follow it.
4. Per behaviour: write a failing test, write the minimum code that passes it, refactor with all tests green.
5. Add each new file to its `CMakeLists.txt`.
6. Update the documentation for each behavioural change.
7. Run the *Build* commands until build and tests pass. Never disable, skip or weaken a test to get green.
8. Non-trivial change: run the `reviewer` agent, or ask the user to run it. Fix each CRITICAL and WARNING finding.
9. Report changed files, build pass/fail, test pass/fail.

## Memory *(shared)*

Scope: production code. Tests and host tools may use the heap.

- Forbidden: `new`/`delete`/`malloc`/`free`, `make_unique`/`make_shared`,
  `std::vector`/`string`/`deque`/`list`/`map`/`set`, recursion.
- Use: `infra::BoundedVector<T>::WithMaxSize<N>`, `infra::BoundedString::WithStorage<N>`,
  `infra::BoundedDeque`, `infra::BoundedList`/`IntrusiveList`, `std::array`, `std::optional`, `std::variant`.

## Interfaces & errors *(shared)*

- No virtual destructors: each adds a deleting destructor and thunks to flash.
  Interface = pure virtual methods + `protected: ~I() = default;` (non-virtual).
- Public `virtual ~Base() = default;` only on a base destroyed through a base pointer:
  `delete`/`std::unique_ptr<Base>`, `infra::PolymorphicVariant<Base, ...>`, `infra::OptionalForPolymorphicObjects<Base, ...>`.
  Put it on that base, not on every interface.
- Not needed for `std::variant`, `std::optional<Concrete>`, `infra::SharedPtr`: they destroy the concrete type.
- No exceptions. `std::optional`, `bool` or status enum; `really_assert` for preconditions. Never swallow an error.
- Constructor injection; no global state.

## Optimisation *(shared)*

- No `#pragma GCC optimize`, no `optimize` attribute: GCC won't inline across differing options.
  The consumer sets flags per translation unit.
- `OPTIMIZE_FOR_SPEED` only on hot functions defined in a header.
- Hot paths: no heap, no virtual dispatch, no blocking.

## Style *(shared)*

- `.clang-format` is authoritative. Allman, 4 spaces, `{}` init.
- PascalCase types/methods; camelCase members and enum values; lowercase namespaces.
  No `I` prefix; implementations end in `Impl`.
- `#pragma once` only.
- Fixed-width integers. `const`/`constexpr`-correct. Functions ≤ 30 lines (hard limit 50).
- Comments: non-obvious *why* only, one line. No `TODO`/`FIXME`, commented-out code, or restating the code.
  Fix or delete a comment the change makes stale.

## Testing *(shared)*

- `TEST_F` for fixtures, `TYPED_TEST` for types, plain `TEST()` for stateless checks.
- `StrictMock` only. `EXPECT_NEAR` with explicit tolerance. Fixture in an anonymous namespace, macros outside.
- Heap allowed in tests.

## Documentation *(shared)*

- Update docs before or with any behavioural change.
- One fact, one place: link, never restate.
- Visuals: Mermaid or ASCII.
<!-- shared:end -->

## Domain *(repo)*

- Template for embedded C++ projects on Cortex-M, with a host build for tests. The `example_*` components show
  the conventions; a project replaces them.
- `core/` — libraries only, no entry points. A component has `interfaces/`, `implementations/`,
  `implementations/test/` and `test_doubles/`. Copy `core/example_component/` for a new one.
- `core/platform_abstraction/` — `platform::Platform`, the seam to the hardware, and `PlatformMock` for tests.
- `core/blinky_cli/` — the reference portable application, written against `platform::Platform`.
- `targets/<app>/Main.cpp` — one entry point for every platform; it includes `PLATFORM_IMPL_HEADER`.
  `targets/platform_implementations/{host,st,ti}/` hold the `PlatformImpl`s; `SCAFFOLD_TARGET_PLATFORM` selects one.
- `tools/` — host-only tools on `core/` libraries. Copy `tools/example_tool/` for a new one.
- `integration_tests/` — cucumber-cpp: `features/`, `steps/`, `support/`, `hooks/`, `main/`.
- `infra/` — submodules: `embedded-infra-lib`, `numerical-toolbox`, `can-lite`, `hal`. Reuse
  `numerical-toolbox` for PID, filters and math.
- *Memory* also covers `targets/` and every ISR-reachable path.
- New peripheral: add a method to `platform::Platform`, implement it in each `PlatformImpl`, update `PlatformMock`.
- New board: copy a `PlatformImpl`, wire its peripherals through `infra/hal/<vendor>`, add a preset.
- Only templates have method bodies in headers.
- Timing-critical loop: state its cycle budget; check it with `arm-none-eabi-objdump -d -C`.
- Includes are rooted at the repository root. CMake libraries: `embedded_scaffold.<area>.<layer>`.
- Unit tests: `core/<component>/implementations/test/Test{Name}.cpp`, registered with `emil_add_test`.
- Documents: `documentation/{architecture,design,theory,requirements}/`; start a new one from
  `documentation/templates/`.

## Build *(repo)*

```bash
git submodule update --init --recursive
cmake --preset host && cmake --build --preset host-Debug
ctest --preset host
cmake --preset EK-TM4C123GXL && cmake --build --preset EK-TM4C123GXL-Debug
cmake --preset STM32F429I-DISC1 && cmake --build --preset STM32F429I-DISC1-Debug
```

Presets: `host`, `coverage`, `EK-TM4C1294XL`, `EK-TM4C123GXL`, `STM32F407G-DISC1`, `STM32F429I-DISC1`, `NUCLEO-H563ZI`.

## Exceptions *(repo)*

None.
