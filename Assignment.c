#include <GLUT/glut.h>
#include <math.h>
#include <stdlib.h>

int xc = 0, yc = 0, r = 7;

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

void drawArcs() {
    int x = 0, y = r;
    int d = 1 - r;

    glBegin(GL_POINTS);
    while (x <= y) {
        // 4 arc points
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc - y, yc - x);
        glVertex2i(xc + x, yc - y);

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
    glColor3f(1.0, 1.0, 1.0);

    // X and Y axes
    drawLine(-12, 0, 12, 0);
    drawLine(0, -12, 0, 12);

    // Diagonal lines to arc boundary
    int d = (int)round(r / sqrt(2.0));

    drawLine(xc, yc, xc + d, yc + d);
    drawLine(xc, yc, xc - d, yc + d);
    drawLine(xc, yc, xc - d, yc - d);
    drawLine(xc, yc, xc + d, yc - d);

    drawArcs();

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);

    glPointSize(6.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-14, 14, -14, 14, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Fan Shape");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
