#include <GLUT/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);       // Clear screen

    glBegin(GL_TRIANGLES);              // Draw triangle
        glColor3f(1,0,0);               // Red
        glVertex2f(-0.8f, -0.8f);       // Bottom-left

        glColor3f(0,1,0);               // Green
        glVertex2f(0.8f, -0.8f);        // Bottom-right

        glColor3f(0,0,1);               // Blue
        glVertex2f(0.0f, 0.8f);         // Top-center
    glEnd();

    glFlush();                          // Execute drawing
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Init GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Single buffer, RGB
    glutInitWindowSize(600, 600);                   // Window size
    glutCreateWindow("Vertex Color Interpolation"); // Create window
    glClearColor(0,0,0,0);                          // Background = black
    glutDisplayFunc(display);                       // Register display callback
    glutMainLoop();                                 // Start event loop
    return 0;
}
