# Part 3: Testing and report deliverables

This guide turns the Part 3 testing requirements into a repeatable checklist for the whole team. It is a plan and report template; it does not claim that tests have already been executed or that coverage reports already exist. Keep the approved Combined SRS/SAD/Test Plan PDF as the source of expected behavior.

## Scope and traceability

Run every applicable test case in the Software Test Plan (STP), including its preconditions, steps, and expected results. Preserve the test case IDs. Record the implemented SRS requirement IDs for each case and note any approved deviations from the SRS/SAD. The backlog issues link their work to the relevant requirement groups; use the STP's case IDs in the final evidence.

For each execution, record:

- test case ID and linked SRS requirement ID(s)
- build/commit identifier, date, tester, and environment
- setup and input data
- expected result and observed result
- Pass, Fail, Blocked, or Not Run, with a short reason
- defect issue/PR link and retest result when applicable
- screenshot, log, or other evidence path when the case needs it

Do not mark an unexecuted case as passed. If an STP case is not applicable, explain why and get the team's agreement before excluding it.

## Required reports

Create the following evidence under `docs/test-reports/` when execution is complete. Use Markdown for the narrative and include the generated tool output as appropriate.

| Deliverable | Minimum contents |
| --- | --- |
| Updated test-case document | Current test steps and expected results; STP case IDs; SRS traceability; actual result, status, date, and evidence for each execution; deviations and defect links. |
| Unit Test Report | Commit/build environment; test framework and command; cases run; pass/fail totals; failures with diagnosis; test log. |
| System Validation Report | Requirement-by-requirement validation matrix; linked STP cases; actual result/status; evidence; open defects, exclusions, and deviations. |
| Code Coverage Report | Exact source revision and commands; tool/version; line and function coverage totals and per-file output; exclusions with reasons. |
| Branch Coverage Report | Exact source revision and commands; tool/version; branch totals and per-file output; uncovered branches and exclusions with reasons. |

Suggested initial names:

- `docs/test-reports/test-cases.md`
- `docs/test-reports/unit-test-report.md`
- `docs/test-reports/system-validation-report.md`
- `docs/test-reports/code-coverage-report.md`
- `docs/test-reports/branch-coverage-report.md`
- `docs/test-reports/logs/` for raw test and coverage tool output

## Reproducible execution

Use the shared Windows/MSYS2 UCRT64 setup in [Required installations](required-installations.md). From the repository root, configure, build, and run the automated suite with:

```bash
cmake -S . -B build -G "MinGW Makefiles" -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Also execute the applicable end-to-end/manual cases against the built game. Record the exact commands and environment in the reports. CI success is useful evidence for automated tests, but it does not replace system validation or manual STP cases.

### Coverage tooling

The current repository configures GoogleTest and CTest, but coverage instrumentation/report generation is a separate deliverable. Before reporting coverage, add and review a reproducible coverage configuration for the team's compiler, run it against the same test suite, and retain the raw output. For the GCC/MinGW toolchain, the team can use `--coverage` instrumentation and `gcov`/`gcovr` if installed; record exact tool versions and commands. Verify that the generated branch metrics count decision outcomes and that third-party FetchContent code and generated build files are excluded. Do not infer branch coverage from line coverage.

If the current Windows toolchain cannot produce reliable branch metrics, document the limitation and use an agreed compiler/toolchain configuration; do not publish fabricated percentages. Keep line/code coverage and branch coverage as separate reports.

## Completion checklist

- [ ] All applicable STP cases executed and their actual results recorded.
- [ ] Failed cases have linked defect issues; fixes have a recorded retest.
- [ ] Test-case document includes SRS traceability and evidence.
- [ ] Unit Test Report is complete and reproducible.
- [ ] System Validation Report covers each applicable requirement.
- [ ] Code Coverage Report includes reproducible line/function results.
- [ ] Branch Coverage Report includes reproducible branch results.
- [ ] Reports and relevant logs are committed under `docs/test-reports/`.
- [ ] README links to the final reports; limitations/deviations are explicit.
- [ ] Team reviews and merges the report PR; no draft/template is presented as an executed result.

## Part 2 planning context

Development is planned in two main sprints. A third sprint is optional spillover for unfinished development, not a replacement testing sprint. Part 3 testing and report preparation runs alongside spillover as needed. The GitHub project board retains the original Day numbers in issue descriptions for historical traceability while using requirement-oriented issue titles and separate Sprint, Phase, priority, and Story Points fields.
