# Day 13: Difficulty Levels — Manual GUI Validation

## Validation details

- Validation date: 2026-11-12
- Related issue: #46
- Test case: TC-07
- Feature: Difficulty selection and movement speed
- Evidence type: Manual GUI testing

## Test results

| Difficulty | Expected movement interval | Actual observation | Result |
|---|---|---|---|
| Easy | 0.20 seconds | Snake movement was visibly the slowest | Pass (qualitative observation) |
| Medium | 0.15 seconds | Snake movement was visibly faster than Easy | Pass (qualitative observation) |
| Hard | 0.08 seconds | Snake movement was visibly the fastest | Pass (qualitative observation) |

## Limitations

The manual test confirmed the relative movement speeds through gameplay. Exact movement intervals were not independently measured using a stopwatch or frame-by-frame analysis. The numerical intervals are expected values covered by automated tests, not manual measurements.
