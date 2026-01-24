#include <GLUT/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear screen
    glColor3f(1,1,0);               // Set color = yellow

    glBegin(GL_LINES);              // Draw lines
        glVertex2f(-0.8f, -0.8f);   // Line start
        glVertex2f(0.8f, 0.8f);     // Line end
    glEnd();

    glFlush();                      // Execute drawing
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Init GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Single buffer, RGB
    glutInitWindowSize(400, 400);                   // Window size
    glutCreateWindow("Line Test");                  // Create window
    glClearColor(0,0,0,0);                          // Background = black
    glutDisplayFunc(display);                       // Register display callback
    glutMainLoop();                                 // Start event loop
    return 0;
}
