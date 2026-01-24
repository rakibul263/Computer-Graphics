#include <GLUT/glut.h>
#include <math.h>
#include<stdlib.h>

// Function to draw a line using DDA Algorithm
void drawLineDDA(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    float x = x0;
    float y = y0;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2i(round(x), round(y)); // Plot pixel
        x += Xinc;
        y += Yinc;
    }
    glEnd();
}

// Display callback
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0); // White line
    drawLineDDA(20, 30, 180, 150);

    glFlush(); // Force execution of GL commands
}

// Initialization
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // Background = black
    glColor3f(1.0, 1.0, 1.0);         // Drawing color = white
    glPointSize(3.0);                 // Pixel size

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 200, 0, 200); // 2D viewing area
}

// Main
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("DDA Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
