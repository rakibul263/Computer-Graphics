#include <GLUT/glut.h>
#include <math.h>
#include <stdlib.h>

#define WIN_W 600
#define WIN_H 600
#define RADIUS 6
#define AXIS_LEN 6

void bresenhamLine(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int e2;

    glBegin(GL_POINTS);
    while (1)
    {
        glVertex2i(x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
    glEnd();
}

void midpointCircleSector(int cx, int cy, int r, float startDeg, float endDeg)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int i;

    float angle;
    float pi = 3.14159265f;
    float a0 = startDeg * pi / 180.0f;
    float a1 = endDeg * pi / 180.0f;

    int ox[8], oy[8];

    glBegin(GL_POINTS);
    while (x <= y)
    {
        ox[0] =  x;  oy[0] =  y;
        ox[1] =  y;  oy[1] =  x;
        ox[2] = -y;  oy[2] =  x;
        ox[3] = -x;  oy[3] =  y;
        ox[4] = -x;  oy[4] = -y;
        ox[5] = -y;  oy[5] = -x;
        ox[6] =  y;  oy[6] = -x;
        ox[7] =  x;  oy[7] = -y;

        for (i = 0; i < 8; i++)
        {
            angle = atan2(oy[i], ox[i]);

            while (angle < a0)
                angle += 2 * pi;

            if (angle <= a1 + 0.02f)
                glVertex2i(cx + ox[i], cy + oy[i]);
        }

        if (d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}

void drawSector(float startDeg, float endDeg)
{
    float pi = 3.14159265f;

    float a0 = startDeg * pi / 180.0f;
    float a1 = endDeg * pi / 180.0f;

    int ex0 = (int)(RADIUS * cos(a0));
    int ey0 = (int)(RADIUS * sin(a0));
    int ex1 = (int)(RADIUS * cos(a1));
    int ey1 = (int)(RADIUS * sin(a1));

    midpointCircleSector(0, 0, RADIUS, startDeg, endDeg);
    bresenhamLine(0, 0, ex0, ey0);
    bresenhamLine(0, 0, ex1, ey1);
}

void drawAxes()
{
    int al = AXIS_LEN;
    bresenhamLine(-al, 0, al, 0);
    bresenhamLine(0, -al, 0, al);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);

    drawAxes();
    drawSector(0, 45);
    drawSector(90, 135);
    drawSector(180, 225);
    drawSector(270, 315);

    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -1, 1);
    glPointSize(5.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line and Midpoint Circle Sector");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
