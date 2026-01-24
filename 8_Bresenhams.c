#include<GLUT/glut.h>
#include <math.h>
#include<stdlib.h>

// Function to draw a line using Bresenham's Algorithm
void drawLineBresenham(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (1)
    {
        glVertex2i(x0, y0); // Plot pixel

        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;

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

// Display callback
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0); // Green line
    drawLineBresenham(20, 30, 180, 150);

    glFlush();
}

// Initialization
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glPointSize(3.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 200, 0, 200);
}

// Main
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Bresenham Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
