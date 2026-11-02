# Day 7: Directional input and timed movement

The snake responds to the arrow keys or WASD. The model rejects a direction
that would immediately reverse the current direction. Each movement step
advances exactly one grid cell and keeps the current body length.

`MovementTimer` accumulates elapsed frame time and reports every movement tick
that is due. The controller applies those ticks independently of the render
frame rate. Movement currently runs every 0.15 seconds. The renderer draws the
board background and the head and body from the model's grid coordinates.

This task covers input, grid movement, and fixed-interval timing. At a board
edge the snake pauses in place to keep the model on-grid; wall and self-collision
game-over behavior, food, and growth belong to later sprint tasks.

Build and run the game using the commands in the repository README. Automated
tests cover one-cell movement, body preservation, reversal rejection, valid
turns, staying on-grid at an edge, and timer accumulation across frame intervals.
