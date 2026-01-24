#include<GLUT/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear screen buffer
    glFlush();                      // Force execution of OpenGL commands
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Single buffer, RGB mode
    glutInitWindowSize(400, 400);                   // Window size
    glutCreateWindow("Minimal Window");             // Create window with title
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);           // Set background color
    glutDisplayFunc(display);                       // Register display callback
    glutMainLoop();                                 // Start event loop
    return 0;
}
