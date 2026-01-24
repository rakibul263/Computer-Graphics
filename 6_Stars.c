#include<GLUT/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);       // Clear screen

    // White polygon
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.53, 0.42, 0.0);
        glVertex3f(0.56, 0.34, 0.0);
        glVertex3f(0.59, 0.42, 0.0);
        glVertex3f(0.67, 0.45, 0.0);
        glVertex3f(0.59, 0.48, 0.0);
        glVertex3f(0.56, 0.56, 0.0);
        glVertex3f(0.53, 0.48, 0.0);
        glVertex3f(0.45, 0.45, 0.0);
    glEnd();

    // Red polygon
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.31, 0.42, 0.0);
        glVertex3f(0.34, 0.34, 0.0);
        glVertex3f(0.37, 0.42, 0.0);
        glVertex3f(0.45, 0.45, 0.0);
        glVertex3f(0.37, 0.48, 0.0);
        glVertex3f(0.34, 0.56, 0.0);
        glVertex3f(0.31, 0.48, 0.0);
        glVertex3f(0.23, 0.45, 0.0);
    glEnd();

    // Green polygon
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.42, 0.53, 0.0);
        glVertex3f(0.45, 0.45, 0.0);
        glVertex3f(0.48, 0.53, 0.0);
        glVertex3f(0.56, 0.56, 0.0);
        glVertex3f(0.48, 0.59, 0.0);
        glVertex3f(0.45, 0.67, 0.0);
        glVertex3f(0.42, 0.59, 0.0);
        glVertex3f(0.34, 0.56, 0.0);
    glEnd();

    // Blue polygon
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.42, 0.31, 0.0);
        glVertex3f(0.45, 0.23, 0.0);
        glVertex3f(0.48, 0.31, 0.0);
        glVertex3f(0.56, 0.34, 0.0);
        glVertex3f(0.48, 0.37, 0.0);
        glVertex3f(0.45, 0.45, 0.0);
        glVertex3f(0.42, 0.37, 0.0);
        glVertex3f(0.34, 0.34, 0.0);
    glEnd();

    glFlush();                          // Execute drawing
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);   // Background = black
    glMatrixMode(GL_PROJECTION);        // Setup projection
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); // Orthographic projection
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Init GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Single buffer, RGB
    glutInitWindowSize(500, 500);                   // Window size
    glutInitWindowPosition(100, 100);               // Window position
    glutCreateWindow("Star Designing");             // Create window
    init();                                         // Init settings
    glutDisplayFunc(display);                       // Register display callback
    glutMainLoop();                                 // Start event loop
    return 0;
}
