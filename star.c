#include<GLUT/glut.h>

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(0.5, 0.8); // Top point
        glVertex2f(0.2, 0.2); // Left bottom
        glVertex2f(0.8, 0.2);
    glEnd();

    glFlush();
}

void init(void){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,1,0,1,-1,1);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Star Designing");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
