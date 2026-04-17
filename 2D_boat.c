#include <GLUT/glut.h>
#include <math.h>

float x2 = -0.9f, y2 = -0.7f;
float angle = 0.0f;

void drawCircle(float r) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float t = 2.0f * 3.1416f * i / 100;
        glVertex2f(r * cos(t), r * sin(t));
    }
    glEnd();
}

void drawPropeller(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);

    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-0.07f, -0.015f);
    glVertex2f( 0.07f, -0.015f);
    glVertex2f( 0.07f,  0.015f);
    glVertex2f(-0.07f,  0.015f);

    glVertex2f(-0.015f, -0.07f);
    glVertex2f( 0.015f, -0.07f);
    glVertex2f( 0.015f,  0.07f);
    glVertex2f(-0.015f,  0.07f);
    glEnd();

    glColor3f(0.7f, 0.7f, 0.7f);
    drawCircle(0.02f);
    glPopMatrix();
}

void drawBoat() {
    glColor3f(0.0f, 0.2f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.32f,  0.00f);
    glVertex2f(-0.40f, -0.05f);
    glVertex2f( 0.28f, -0.05f);
    glVertex2f( 0.42f,  0.08f);
    glVertex2f( 0.18f,  0.12f);
    glVertex2f(-0.32f,  0.12f);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.40f, -0.05f);
    glVertex2f( 0.28f, -0.05f);
    glVertex2f( 0.20f, -0.16f);
    glVertex2f(-0.30f, -0.16f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.36f, 0.0f);
    glVertex2f( 0.32f, 0.0f);
    glVertex2f( 0.32f, 0.02f);
    glVertex2f(-0.36f, 0.02f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(-0.12f, 0.12f);
    glVertex2f( 0.14f, 0.12f);
    glVertex2f( 0.14f, 0.26f);
    glVertex2f(-0.12f, 0.26f);
    glEnd();

    glColor3f(0.2f, 0.6f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-0.13f, 0.26f);
    glVertex2f( 0.15f, 0.26f);
    glVertex2f( 0.15f, 0.30f);
    glVertex2f(-0.13f, 0.30f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.02f, 0.18f, 0);
    glColor3f(0.5f, 0.8f, 1.0f);
    drawCircle(0.04f);
    glPopMatrix();

    glColor3f(0.4f, 0.2f, 0.1f);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.30f);
    glVertex2f(0.0f, 0.65f);
    glEnd();
    glLineWidth(1);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.65f);
    glVertex2f(0.18f, 0.55f);
    glVertex2f(0.0f, 0.45f);
    glEnd();

    drawPropeller(-0.38f, -0.10f);
}

void display1() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBoat();
    glutSwapBuffers();
}

void display2() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(x2, y2, 0);
    glRotatef(30, 0, 0, 1);
    glScalef(1.6f, 1.6f, 1);
    drawBoat();
    glPopMatrix();
    glutSwapBuffers();
}

void update(int v) {
    x2 += 0.008f;
    y2 += 0.008f;

    if (x2 > 1.3f) {
        x2 = -1.3f;
        y2 = -1.0f;
    }

    angle += 4.0f;
    if (angle > 360) angle -= 360;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void init() {
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Static Boat");
    init();
    glutDisplayFunc(display1);

    glutInitWindowPosition(600, 100);
    glutCreateWindow("Animated Boat");
    init();
    glutDisplayFunc(display2);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
