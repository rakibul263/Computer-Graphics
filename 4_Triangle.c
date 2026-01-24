#include <GLUT/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);       // Clear screen
    glColor3f(0.2f, 0.7f, 1.0f);        // Set color (light blue)

    glBegin(GL_POLYGON);                // Start polygon (triangle here)
        glVertex2f(-0.5f, -0.3f);       // Bottom-left
        glVertex2f(0.5f, -0.3f);        // Bottom-right
        glVertex2f(0.0f, 0.6f);         // Top-center
    glEnd();

    glFlush();                          // Execute drawing
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Init GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Single buffer, RGB
    glutInitWindowSize(500, 500);                   // Window size
    glutCreateWindow("Triangle");                   // Create window
    glClearColor(0,0,0,0);                          // Background = black
    glutDisplayFunc(display);                       // Register display callback
    glutMainLoop();                                 // Start event loop
    return 0;
}
