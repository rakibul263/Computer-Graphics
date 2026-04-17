#include <GLUT/glut.h>

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.60f, 0.85f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(1.0f, 0.25f);
    glVertex2f(0.0f, 0.25f);
    glEnd();

    glColor3f(0.20f, 0.65f, 0.20f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.25f);
    glVertex2f(1.0f, 0.25f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    glColor3f(1.0f, 0.9f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(0.78f, 0.82f);
    glVertex2f(0.82f, 0.82f);
    glVertex2f(0.82f, 0.88f);
    glVertex2f(0.78f, 0.88f);
    glEnd();

    glColor3f(0.95f, 0.80f, 0.72f);
    glBegin(GL_POLYGON);
    glVertex2f(0.28f, 0.25f);
    glVertex2f(0.72f, 0.25f);
    glVertex2f(0.72f, 0.62f);
    glVertex2f(0.28f, 0.62f);
    glEnd();

    glColor3f(0.75f, 0.15f, 0.15f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.25f, 0.62f);
    glVertex2f(0.75f, 0.62f);
    glVertex2f(0.50f, 0.88f);
    glEnd();

    glColor3f(0.45f, 0.25f, 0.10f);
    glBegin(GL_POLYGON);
    glVertex2f(0.46f, 0.25f);
    glVertex2f(0.54f, 0.25f);
    glVertex2f(0.54f, 0.45f);
    glVertex2f(0.46f, 0.45f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.525f, 0.35f);
    glEnd();

    glColor3f(0.55f, 0.82f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.34f, 0.38f);
    glVertex2f(0.42f, 0.38f);
    glVertex2f(0.42f, 0.48f);
    glVertex2f(0.34f, 0.48f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.58f, 0.38f);
    glVertex2f(0.66f, 0.38f);
    glVertex2f(0.66f, 0.48f);
    glVertex2f(0.58f, 0.48f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(0.38f, 0.38f);
    glVertex2f(0.38f, 0.48f);
    glVertex2f(0.34f, 0.43f);
    glVertex2f(0.42f, 0.43f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0.62f, 0.38f);
    glVertex2f(0.62f, 0.48f);
    glVertex2f(0.58f, 0.43f);
    glVertex2f(0.66f, 0.43f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.12f, 0.25f);
    glVertex2f(0.12f, 0.75f);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.12f, 0.73f);
    glVertex2f(0.34f, 0.73f);
    glVertex2f(0.34f, 0.53f);
    glVertex2f(0.12f, 0.53f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.20f, 0.58f);
    glVertex2f(0.26f, 0.58f);
    glVertex2f(0.26f, 0.66f);
    glVertex2f(0.20f, 0.66f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.10f, 0.25f);
    glVertex2f(0.14f, 0.25f);
    glVertex2f(0.14f, 0.19f);
    glVertex2f(0.10f, 0.19f);
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(0.60f, 0.85f, 0.95f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("House with Flag");
    init();
    glutDisplayFunc(drawScene);
    glutMainLoop();
    return 0;
}
