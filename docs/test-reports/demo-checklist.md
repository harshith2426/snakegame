# Final demo walkthrough checklist

This checklist prepares a short, repeatable demonstration from a clean checkout. It is a guide for the team; steps not actually performed by the current report run are not represented as test results.

## Prepare

1. Follow [Required installations](../required-installations.md).
2. From an MSYS2 UCRT64 terminal at the repository root, run:

   ```bash
   cmake -S . -B build -G "MinGW Makefiles"
   cmake --build build
   ./build/snakearcade.exe
   ```

3. Confirm the difficulty menu appears and the window shows both score and high-score fields once gameplay starts.

## Walkthrough

1. Select Easy, Medium, and Hard in turn and describe their different movement speeds; start a session with Enter.
2. Move with arrow keys or WASD. Show that the snake advances on a grid and cannot immediately reverse into itself.
3. Steer into food. Show growth, score increase, and food respawn. Keep the snake away from the walls during this step.
4. Press **P** to pause, wait briefly, then press **P** to resume. Point out that the session is preserved.
5. Allow a wall or self collision to end the game. Point out the final score and high score.
6. Press **R** to restart, then **Esc** or **M** from game over to return to the difficulty menu.
7. If a safe temporary test profile is available, demonstrate high-score persistence across runs. Do not overwrite a teammate's saved score during rehearsal.

## Evidence to capture during the team run

- A screenshot showing the live score HUD during gameplay, including a non-zero score after food collection.
- A short recording or screenshots of difficulty selection, pause/resume, and game over/restart.
- The clean-checkout configure/build output and CTest summary.
- A note of presenter, date, tested revision, and any observed deviation.

The current automated reports verify game logic and build behavior. The Day 19 UI smoke run confirmed the HUD labels but did not observe a non-zero score; the team should capture that final visual check during rehearsal.
