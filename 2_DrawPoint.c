#include<GLUT/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear screen

    glColor3f(1,1,1);               // Set draw color (white)
    glPointSize(8.0f);              // Point size = 8px

    glBegin(GL_POINTS);             // Start drawing points
        glVertex2f(0.0f, 0.0f);     // Point at (0,0) → center of window
    glEnd();

    glFlush();                      // Execute drawing commands
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Init GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Single buffer, RGB
    glutInitWindowSize(400, 400);                   // Window size
    glutCreateWindow("Point Test");                 // Create window
    glClearColor(0,0,0,0);                          // Background color = black

    glMatrixMode(GL_PROJECTION);                    // Projection setup
    glLoadIdentity();                               // Reset to identity

    glutDisplayFunc(display);                       // Register display callback
    glutMainLoop();                                 // Event loop
    return 0;
}
