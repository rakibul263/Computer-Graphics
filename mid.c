#include <GLUT/glut.h>
#include <math.h>

void axes()
{
    glBegin(GL_LINES);

    // X axis
    glVertex2f(-1,0);
    glVertex2f(1,0);

    // Y axis
    glVertex2f(0,-1);
    glVertex2f(0,1);

    glEnd();
}

void arc(float start, float end)
{
    float r = 0.5;

    glBegin(GL_LINE_STRIP);

    for(float angle=start; angle<=end; angle+=1)
    {
        float rad = angle * 3.1416 / 180;

        float x = r * cos(rad);
        float y = r * sin(rad);

        glVertex2f(x,y);
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0,0,0);

    axes();

    // 4 arcs (quadrant অনুযায়ী)
    arc(20,80);     // top right
    arc(110,170);   // top left
    arc(200,260);   // bottom left
    arc(290,350);   // bottom right

    glFlush();
}

void init()
{
    glClearColor(1,1,1,1);
    gluOrtho2D(-1,1,-1,1);
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutCreateWindow("Computer Graphics");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
}
