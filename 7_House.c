#include <GLUT/glut.h>

void drawHouse(void) {
    // House body (beige)
    glColor3f(0.96f, 0.87f, 0.70f);
    glBegin(GL_POLYGON);
        glVertex2f(0.2f, 0.2f);
        glVertex2f(0.8f, 0.2f);
        glVertex2f(0.8f, 0.6f);
        glVertex2f(0.2f, 0.6f);
    glEnd();

    // Roof (brick red)
    glColor3f(0.75f, 0.13f, 0.13f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.2f, 0.6f);
        glVertex2f(0.8f, 0.6f);
        glVertex2f(0.5f, 0.9f);
    glEnd();

    // Door (wood brown)
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_POLYGON);
        glVertex2f(0.45f, 0.2f);
        glVertex2f(0.55f, 0.2f);
        glVertex2f(0.55f, 0.4f);
        glVertex2f(0.45f, 0.4f);
    glEnd();

    // Left window (blue glass)
    glColor3f(0.53f, 0.81f, 0.98f);
    glBegin(GL_POLYGON);
        glVertex2f(0.28f, 0.35f);
        glVertex2f(0.38f, 0.35f);
        glVertex2f(0.38f, 0.45f);
        glVertex2f(0.28f, 0.45f);
    glEnd();

    // Right window (blue glass)
    glBegin(GL_POLYGON);
        glVertex2f(0.62f, 0.35f);
        glVertex2f(0.72f, 0.35f);
        glVertex2f(0.72f, 0.45f);
        glVertex2f(0.62f, 0.45f);
    glEnd();

    // Window frames (black outlines)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.28f, 0.35f);
        glVertex2f(0.38f, 0.35f);
        glVertex2f(0.38f, 0.45f);
        glVertex2f(0.28f, 0.45f);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex2f(0.62f, 0.35f);
        glVertex2f(0.72f, 0.35f);
        glVertex2f(0.72f, 0.45f);
        glVertex2f(0.62f, 0.45f);
    glEnd();

    // Ground (fresh green grass)
    glColor3f(0.13f, 0.55f, 0.13f);
    glBegin(GL_POLYGON);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(1.0f, 0.2f);
        glVertex2f(0.0f, 0.2f);
    glEnd();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear screen
    drawHouse();                   // Call house function
    glFlush();                     // Render
}

void init(void) {
    glClearColor(0.68f, 0.85f, 0.90f, 0.0f); // Light sky blue background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);  // Orthographic projection
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Aesthetic House Drawing");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
