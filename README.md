# Catch The Egg

A simple 2D game built with OpenGL and GLUT where you control a basket to catch falling eggs dropped by hens. Catch different types of eggs to score points, avoid the "poop" eggs that penalize your score!

---

## Features

- Two hens moving horizontally at the top of the screen.
- Eggs drop randomly from hens with four types:
  - Normal (white) — 1 point
  - Blue — 5 points
  - Golden — 10 points
  - Poop (brown) — subtracts 10 points
- Wind affects the horizontal fall of eggs.
- Move basket with arrow keys or mouse.
- Pause and resume functionality.
- Timer-based gameplay (60 seconds).
- Right-click menu with Start, Help, High Score, and Exit options.
- Displays points, time left, and high score.

---

## Controls

| Action                 | Key / Input                  |
|------------------------|-----------------------------|
| Start / Restart Game   | `S`                         |
| Pause / Resume         | `P`                         |
| Quit                  | `Q`                         |
| Move Basket Left      | Left Arrow Key / Mouse Move  |
| Move Basket Right     | Right Arrow Key / Mouse Move |
| Open Menu             | Right Mouse Button           |

---

## Requirements

- **Windows OS**
- [FreeGLUT](http://freeglut.sourceforge.net/) or compatible GLUT implementation
- OpenGL libraries

---

## How to Compile

Use a C compiler with OpenGL and GLUT linked. For example, using `gcc` on Windows with freeglut:

```bash
gcc catch_the_egg.c -lfreeglut -lopengl32 -lglu32 -o catch_the_egg.exe
