# 2D Graphics Editor — Input Cheatsheet

## Canvas Size
- Columns (left to right) : 0 to 59
- Rows    (top to bottom) : 0 to 24
- Center of canvas        : col=30, row=12

---

## Menu Options
| Type | Number |
|------|--------|
| Draw Line        | 1 |
| Draw Rectangle   | 2 |
| Draw Circle      | 3 |
| Draw Triangle    | 4 |
| Delete Shape     | 5 |
| Modify Shape     | 6 |
| List All Shapes  | 7 |
| Display Canvas   | 8 |
| Clear All        | 9 |
| Exit             | 0 |

---

## What to Type for Each Shape

### Line (Choice: 1)
```
Enter start point (col row): 5 5
Enter end   point (col row): 50 20
```

### Rectangle (Choice: 2)
```
Enter top-left  (col row): 5 3
Enter bot-right (col row): 40 20
```

### Circle (Choice: 3)
```
Enter center (col row): 30 12
Enter radius          : 8
```

### Triangle (Choice: 4)
```
Enter vertex 1 (col row): 30 2
Enter vertex 2 (col row): 5 22
Enter vertex 3 (col row): 55 22
```

### Delete (Choice: 5)
```
Enter shape ID to delete: 0
```
(Type the ID number shown in the list)

### Modify (Choice: 6)
```
Enter shape ID to modify: 0
```
(Then enter new coordinates just like drawing)

---

## Safe Input Values (won't go out of bounds)

| Shape     | Input                                      |
|-----------|--------------------------------------------|
| Line      | 5 5 → 50 20                                |
| Rectangle | top-left: 5 3 → bot-right: 40 20          |
| Circle    | center: 30 12, radius: 8                   |
| Triangle  | 30 2 then 5 22 then 55 22                  |

---

## Understanding Coordinates
- First number  = col = how far RIGHT  (0=left edge, 59=right edge)
- Second number = row = how far DOWN   (0=top edge,  24=bottom edge)

Example: 30 12 = center of canvas

---

## How to Compile and Run
```bash
gcc main.c -o graphics_editor -lm
./graphics_editor
```

---

## Daily GitHub Push (from browser)
1. Go to your repo on github.com
2. Click main.c → click pencil icon ✏️
3. Add a small comment like: // Day 2
4. Scroll down → click Commit changes
```
