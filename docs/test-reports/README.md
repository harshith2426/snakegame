# Test report evidence

This folder contains the executed evidence available so far and the final Part 3 reports as they are completed. Do not treat the planning guide or this placeholder as a completed test report.

- [Day 17 core gameplay test results](day17-gameplay-test-results.md) — automated execution evidence for STP TC-01 through TC-05 and TC-10, with manual-testing limits called out.
- [Day 18 state and persistence test results](day18-state-persistence-results.md) — automated evidence for STP TC-06 through TC-09.
- [Day 19 defect regression results](day19-defect-regression-results.md) — live score HUD defect #39 fix, automated regression suite, and limited UI smoke-test evidence.
- [Updated STP test cases and traceability](test-cases.md) — all ten STP cases mapped to SRS requirements and current evidence.
- [Unit Test Report](unit-test-report.md) — 75 CTest cases with environment, commands, and raw log.
- [System Validation Report](system-validation-report.md) — requirement-group validation, evidence, and limitations.
- [Code Coverage Report](code-coverage-report.md) — GCC line and function coverage for logic exercised by the test target.
- [Branch Coverage Report](branch-coverage-report.md) — independent GCC branch-outcome metrics and uncovered paths.
- [Demo walkthrough checklist](demo-checklist.md) — repeatable sequence for the team's final gameplay demonstration.

The reports separate automated logic tests from manual GUI checks. A test passing in GoogleTest does not imply the equivalent keyboard/UI workflow was manually executed. See each report for scope, exclusions, and evidence paths.

Follow [Part 3 testing and report deliverables](../part3-testing-and-reports.md). The project owner and team should review the reports and confirm the submission artifacts before final sign-off.

- [Requirements review (Issue #16)](requirements-review.md)
- Raw output: [`logs/unit-test-run.log`](logs/unit-test-run.log), [`logs/coverage-configure.log`](logs/coverage-configure.log), [`logs/coverage-build.log`](logs/coverage-build.log), [`logs/gcov-raw.log`](logs/gcov-raw.log), and the compressed gcov JSON snapshots in [`logs/coverage-json/`](logs/coverage-json/).
