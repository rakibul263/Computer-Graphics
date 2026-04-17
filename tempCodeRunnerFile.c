#include <GLUT/glut.h>
#include <math.h>
#include <stdlib.h>

int xc = 0, yc = 0, r = 7;

// Bresenham's Line Algorithm
void drawLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (1) {
        glVertex2i(x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    glEnd();
}

// Midpoint Circle থেকে নির্দিষ্ট 4টি arc
void drawArcs() {
    int x = 0, y = r;
    int d = 1 - r;

    glBegin(GL_POINTS);
    while (x <= y) {
        // Fan blade এর 4টা curved অংশ
        glVertex2i(xc + y, yc + x); // 1st arc
        glVertex2i(xc - x, yc + y); // 2nd arc
        glVertex2i(xc - y, yc - x); // 3rd arc
        glVertex2i(xc + x, yc - y); // 4th arc

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // সাদা রং
    glColor3f(1.0, 1.0, 1.0);

    // 1. Axes
    drawLine(-12, 0, 12, 0);
    drawLine(0, -12, 0, 12);

    // 2. Diagonal lines (45 degree)
    int d = (int)(r / sqrt(2.0));

    drawLine(xc, yc, xc + d, yc + d); // Top-right
    drawLine(xc, yc, xc - d, yc + d); // Top-left
    drawLine(xc, yc, xc - d, yc - d); // Bottom-left
    drawLine(xc, yc, xc + d, yc - d); // Bottom-right

    // 3. Arcs
    drawArcs();

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // ডটগুলো মোটা ও কাছাকাছি দেখানোর জন্য
    glPointSize(6.0);

    // Smooth point enable
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Projection range ছোট করলে shape বড় দেখাবে,
    // ফলে dots গুলোও কাছাকাছি ও line-এর মতো লাগবে
    glOrtho(-14, 14, -14, 14, -1, 1);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Improved Fan Shape");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
