/*
 * =====================================================
 *  2D Graphics Editor in C
 *  Canvas: 2D char array filled with '_'
 *  Shapes drawn with '*'
 * =====================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* ── Canvas dimensions ── */
#define ROWS 25
#define COLS 60

/* ── Max shapes we can store ── */
#define MAX_SHAPES 50

/* ── Shape type codes ── */
#define SHAPE_LINE      1
#define SHAPE_RECTANGLE 2
#define SHAPE_CIRCLE    3
#define SHAPE_TRIANGLE  4

/* ── Canvas ── */
char canvas[ROWS][COLS];

/* ── Shape record ── */
typedef struct {
    int type;
    int active;   /* 1 = on canvas, 0 = deleted */
    /* Line      : x1,y1 -> x2,y2                      */
    /* Rectangle : x1,y1 (top-left) x2,y2 (bottom-right)*/
    /* Circle    : cx,cy,r                               */
    /* Triangle  : three vertices (x1,y1)(x2,y2)(x3,y3) */
    int x1, y1, x2, y2, x3, y3, r;
} Shape;

Shape shapes[MAX_SHAPES];
int   shape_count = 0;

/* ════════════════════════════════════════════
   CANVAS UTILITIES
   ════════════════════════════════════════════ */

void init_canvas(void) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            canvas[i][j] = '_';
}

void display_canvas(void) {
    printf("\n");
    /* Column ruler */
    printf("   ");
    for (int j = 0; j < COLS; j++) printf("%d", j % 10);
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("%2d ", i);
        for (int j = 0; j < COLS; j++)
            putchar(canvas[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* Safe pixel setter – ignores out-of-bounds */
void set_pixel(int row, int col, char ch) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS)
        canvas[row][col] = ch;
}

/* Rebuild canvas from all active shapes */
void rebuild_canvas(void) {
    init_canvas();
    for (int i = 0; i < shape_count; i++)
        if (shapes[i].active)
            ; /* will call draw_shape below – declared after */
    /* Forward-declaration trick: we call a helper after defining it */
}

/* ════════════════════════════════════════════
   DRAWING ALGORITHMS
   ════════════════════════════════════════════ */

/* Bresenham's line */
void draw_line_pixels(int x1, int y1, int x2, int y2, char ch) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        set_pixel(y1, x1, ch);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

/* Rectangle – 4 edges */
void draw_rect_pixels(int x1, int y1, int x2, int y2, char ch) {
    draw_line_pixels(x1, y1, x2, y1, ch); /* top    */
    draw_line_pixels(x1, y2, x2, y2, ch); /* bottom */
    draw_line_pixels(x1, y1, x1, y2, ch); /* left   */
    draw_line_pixels(x2, y1, x2, y2, ch); /* right  */
}

/* Midpoint circle */
void draw_circle_pixels(int cx, int cy, int r, char ch) {
    int x = 0, y = r, d = 1 - r;
    while (x <= y) {
        set_pixel(cy + y, cx + x, ch);
        set_pixel(cy - y, cx + x, ch);
        set_pixel(cy + y, cx - x, ch);
        set_pixel(cy - y, cx - x, ch);
        set_pixel(cy + x, cx + y, ch);
        set_pixel(cy - x, cx + y, ch);
        set_pixel(cy + x, cx - y, ch);
        set_pixel(cy - x, cx - y, ch);
        if (d < 0) d += 2 * x + 3;
        else     { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

/* Triangle – 3 lines */
void draw_triangle_pixels(int x1, int y1, int x2, int y2,
                           int x3, int y3, char ch) {
    draw_line_pixels(x1, y1, x2, y2, ch);
    draw_line_pixels(x2, y2, x3, y3, ch);
    draw_line_pixels(x3, y3, x1, y1, ch);
}

/* ════════════════════════════════════════════
   DRAW / ERASE A SHAPE RECORD
   ════════════════════════════════════════════ */

void render_shape(Shape *s, char ch) {
    switch (s->type) {
        case SHAPE_LINE:
            draw_line_pixels(s->x1, s->y1, s->x2, s->y2, ch);
            break;
        case SHAPE_RECTANGLE:
            draw_rect_pixels(s->x1, s->y1, s->x2, s->y2, ch);
            break;
        case SHAPE_CIRCLE:
            draw_circle_pixels(s->x1, s->y1, s->r, ch);
            break;
        case SHAPE_TRIANGLE:
            draw_triangle_pixels(s->x1, s->y1, s->x2, s->y2,
                                  s->x3, s->y3, ch);
            break;
    }
}

/* Full rebuild from shape list */
void full_rebuild(void) {
    init_canvas();
    for (int i = 0; i < shape_count; i++)
        if (shapes[i].active)
            render_shape(&shapes[i], '*');
}

/* ════════════════════════════════════════════
   ADD SHAPE HELPERS
   ════════════════════════════════════════════ */

int add_shape(void) {
    if (shape_count >= MAX_SHAPES) {
        printf("Canvas is full! Delete some shapes first.\n");
        return -1;
    }
    return shape_count++;   /* returns index */
}

void add_line(void) {
    int x1, y1, x2, y2;
    printf("Enter start point (col row): "); scanf("%d %d", &x1, &y1);
    printf("Enter end   point (col row): "); scanf("%d %d", &x2, &y2);

    int idx = add_shape(); if (idx < 0) return;
    shapes[idx] = (Shape){ SHAPE_LINE, 1, x1, y1, x2, y2, 0, 0, 0 };
    render_shape(&shapes[idx], '*');
    printf("Line added (ID %d).\n", idx);
}

void add_rectangle(void) {
    int x1, y1, x2, y2;
    printf("Enter top-left  (col row): "); scanf("%d %d", &x1, &y1);
    printf("Enter bot-right (col row): "); scanf("%d %d", &x2, &y2);

    int idx = add_shape(); if (idx < 0) return;
    shapes[idx] = (Shape){ SHAPE_RECTANGLE, 1, x1, y1, x2, y2, 0, 0, 0 };
    render_shape(&shapes[idx], '*');
    printf("Rectangle added (ID %d).\n", idx);
}

void add_circle(void) {
    int cx, cy, r;
    printf("Enter center (col row): "); scanf("%d %d", &cx, &cy);
    printf("Enter radius          : "); scanf("%d", &r);

    int idx = add_shape(); if (idx < 0) return;
    shapes[idx] = (Shape){ SHAPE_CIRCLE, 1, cx, cy, 0, 0, 0, 0, r };
    render_shape(&shapes[idx], '*');
    printf("Circle added (ID %d).\n", idx);
}

void add_triangle(void) {
    int x1, y1, x2, y2, x3, y3;
    printf("Enter vertex 1 (col row): "); scanf("%d %d", &x1, &y1);
    printf("Enter vertex 2 (col row): "); scanf("%d %d", &x2, &y2);
    printf("Enter vertex 3 (col row): "); scanf("%d %d", &x3, &y3);

    int idx = add_shape(); if (idx < 0) return;
    shapes[idx] = (Shape){ SHAPE_TRIANGLE, 1, x1, y1, x2, y2, x3, y3, 0 };
    render_shape(&shapes[idx], '*');
    printf("Triangle added (ID %d).\n", idx);
}

/* ════════════════════════════════════════════
   LIST SHAPES
   ════════════════════════════════════════════ */

const char *type_name(int t) {
    switch (t) {
        case SHAPE_LINE:      return "Line";
        case SHAPE_RECTANGLE: return "Rectangle";
        case SHAPE_CIRCLE:    return "Circle";
        case SHAPE_TRIANGLE:  return "Triangle";
        default:              return "Unknown";
    }
}

void list_shapes(void) {
    int found = 0;
    printf("\n%-4s %-12s  Details\n", "ID", "Type");
    printf("--------------------------------------------\n");
    for (int i = 0; i < shape_count; i++) {
        if (!shapes[i].active) continue;
        found = 1;
        printf("%-4d %-12s  ", i, type_name(shapes[i].type));
        switch (shapes[i].type) {
            case SHAPE_LINE:
                printf("(%d,%d)->(%d,%d)", shapes[i].x1, shapes[i].y1,
                                           shapes[i].x2, shapes[i].y2);
                break;
            case SHAPE_RECTANGLE:
                printf("TL(%d,%d) BR(%d,%d)", shapes[i].x1, shapes[i].y1,
                                               shapes[i].x2, shapes[i].y2);
                break;
            case SHAPE_CIRCLE:
                printf("C(%d,%d) R=%d", shapes[i].x1, shapes[i].y1,
                                         shapes[i].r);
                break;
            case SHAPE_TRIANGLE:
                printf("(%d,%d) (%d,%d) (%d,%d)",
                        shapes[i].x1, shapes[i].y1,
                        shapes[i].x2, shapes[i].y2,
                        shapes[i].x3, shapes[i].y3);
                break;
        }
        printf("\n");
    }
    if (!found) printf("  (no shapes on canvas)\n");
    printf("\n");
}

/* ════════════════════════════════════════════
   DELETE SHAPE
   ════════════════════════════════════════════ */

void delete_shape(void) {
    list_shapes();
    int id;
    printf("Enter shape ID to delete (-1 to cancel): ");
    scanf("%d", &id);
    if (id < 0 || id >= shape_count || !shapes[id].active) {
        printf("Invalid ID.\n"); return;
    }
    shapes[id].active = 0;
    full_rebuild();
    printf("Shape %d deleted.\n", id);
}

/* ════════════════════════════════════════════
   MODIFY SHAPE
   ════════════════════════════════════════════ */

void modify_shape(void) {
    list_shapes();
    int id;
    printf("Enter shape ID to modify (-1 to cancel): ");
    scanf("%d", &id);
    if (id < 0 || id >= shape_count || !shapes[id].active) {
        printf("Invalid ID.\n"); return;
    }

    /* Erase old shape, re-enter parameters, redraw */
    shapes[id].active = 0;
    full_rebuild();

    printf("Re-enter parameters for %s:\n", type_name(shapes[id].type));
    shapes[id].active = 1;   /* keep type, just update coords */

    switch (shapes[id].type) {
        case SHAPE_LINE:
            printf("New start (col row): "); scanf("%d %d", &shapes[id].x1, &shapes[id].y1);
            printf("New end   (col row): "); scanf("%d %d", &shapes[id].x2, &shapes[id].y2);
            break;
        case SHAPE_RECTANGLE:
            printf("New top-left  (col row): "); scanf("%d %d", &shapes[id].x1, &shapes[id].y1);
            printf("New bot-right (col row): "); scanf("%d %d", &shapes[id].x2, &shapes[id].y2);
            break;
        case SHAPE_CIRCLE:
            printf("New center (col row): "); scanf("%d %d", &shapes[id].x1, &shapes[id].y1);
            printf("New radius           : "); scanf("%d", &shapes[id].r);
            break;
        case SHAPE_TRIANGLE:
            printf("New vertex 1 (col row): "); scanf("%d %d", &shapes[id].x1, &shapes[id].y1);
            printf("New vertex 2 (col row): "); scanf("%d %d", &shapes[id].x2, &shapes[id].y2);
            printf("New vertex 3 (col row): "); scanf("%d %d", &shapes[id].x3, &shapes[id].y3);
            break;
    }

    full_rebuild();
    printf("Shape %d updated.\n", id);
}

/* ════════════════════════════════════════════
   CLEAR CANVAS
   ════════════════════════════════════════════ */

void clear_all(void) {
    char confirm;
    printf("Clear ALL shapes? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm == 'y' || confirm == 'Y') {
        shape_count = 0;
        init_canvas();
        printf("Canvas cleared.\n");
    }
}

/* ════════════════════════════════════════════
   MENU
   ════════════════════════════════════════════ */

void print_menu(void) {
    printf("╔══════════════════════════════╗\n");
    printf("║   2D Graphics Editor (C)     ║\n");
    printf("╠══════════════════════════════╣\n");
    printf("║  1. Draw Line                ║\n");
    printf("║  2. Draw Rectangle           ║\n");
    printf("║  3. Draw Circle              ║\n");
    printf("║  4. Draw Triangle            ║\n");
    printf("║  5. Delete Shape             ║\n");
    printf("║  6. Modify Shape             ║\n");
    printf("║  7. List All Shapes          ║\n");
    printf("║  8. Display Canvas           ║\n");
    printf("║  9. Clear All                ║\n");
    printf("║  0. Exit                     ║\n");
    printf("╚══════════════════════════════╝\n");
    printf("Choice: ");
}

/* ════════════════════════════════════════════
   MAIN
   ════════════════════════════════════════════ */

int main(void) {
    init_canvas();
    int choice;

    printf("\nWelcome to the 2D Graphics Editor!\n");
    printf("Canvas size: %d rows x %d cols\n", ROWS, COLS);
    printf("Coordinates: col = 0..%d, row = 0..%d\n\n", COLS-1, ROWS-1);

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) { choice = -1; }

        switch (choice) {
            case 1: add_line();       display_canvas(); break;
            case 2: add_rectangle();  display_canvas(); break;
            case 3: add_circle();     display_canvas(); break;
            case 4: add_triangle();   display_canvas(); break;
            case 5: delete_shape();   display_canvas(); break;
            case 6: modify_shape();   display_canvas(); break;
            case 7: list_shapes();                      break;
            case 8: display_canvas();                   break;
            case 9: clear_all();      display_canvas(); break;
            case 0: printf("Goodbye!\n");               break;
            default: printf("Invalid choice. Try again.\n"); break;
        }
    } while (choice != 0);

    return 0;
}
