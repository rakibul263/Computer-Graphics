#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void drawLineDDA(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    float x = x0;
    float y = y0;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        int px = (int)round(x);
        int py = (int)round(y);

        glVertex2i(px, py);

        printf("Step %3d : X = %7.2f, Y = %7.2f  -> Plotted Pixel = (%d, %d)\n",
               i, x, y, px, py);

        x += Xinc;
        y += Yinc;
    }
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    drawLineDDA(20, 30, 180, 150);

    glFlush();
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPointSize(3.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 200, 0, 200);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
