# 2D Graphics Editor in C

A menu-driven 2D Graphics Editor built in C using a 2D character array as the drawing canvas.

## Features
- Canvas filled with `_`, shapes drawn with `*`
- Draw: Line, Rectangle, Circle, Triangle
- Delete and Modify any shape by ID
- Bresenham's line algorithm, Midpoint circle algorithm

## How to Compile & Run
```bash
gcc main.c -o graphics_editor -lm
./graphics_editor
```

## Canvas Coordinates
- Columns (x): 0 to 59
- Rows    (y): 0 to 24

## Menu Options
| Option | Action |
|--------|--------|
| 1 | Draw Line |
| 2 | Draw Rectangle |
| 3 | Draw Circle |
| 4 | Draw Triangle |
| 5 | Delete Shape |
| 6 | Modify Shape |
| 7 | List All Shapes |
| 8 | Display Canvas |
| 9 | Clear All |
| 0 | Exit |
