#include <GLUT/glut.h>
#include <math.h>

// -------- GLOBAL VARIABLES --------
float angle = 30.0;
float rad;

float carX = -0.8;
float carY = -0.5;
float speed = 0.01;

// -------- DRAW CAR --------
void drawCar()
{
    // -------- BACK (SQUARE BODY) --------
    glColor3f(0.2, 0.6, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4, -0.1);
    glVertex2f(-0.4, 0.2);
    glVertex2f(-0.1, 0.2);
    glVertex2f(-0.1, -0.1);
    glEnd();

    // -------- FRONT (RECTANGLE) --------
    glColor3f(0.1, 0.4, 0.8);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1, -0.1);
    glVertex2f(-0.1, 0.1);
    glVertex2f(0.35, 0.1);
    glVertex2f(0.35, -0.1);
    glEnd();

    // -------- ROOF --------
    glColor3f(1.0, 0.3, 0.3);
    glBegin(GL_POLYGON);
    glVertex2f(-0.35, 0.2);
    glVertex2f(-0.15, 0.2);
    glVertex2f(-0.15, 0.35);
    glVertex2f(-0.35, 0.35);
    glEnd();

    // ================= WHEELS =================
    float cx1 = -0.25, cy = -0.1;
    float cx2 = 0.2;

    // -------- BACK WHEEL --------
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for(int i=0;i<360;i++){
        float t = i * 3.1416 / 180;
        glVertex2f(cx1 + 0.08*cos(t), cy + 0.08*sin(t));
    }
    glEnd();

    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
    for(int i=0;i<360;i++){
        float t = i * 3.1416 / 180;
        glVertex2f(cx1 + 0.04*cos(t), cy + 0.04*sin(t));
    }
    glEnd();

    // Cross (+)
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(cx1 - 0.04, cy);
    glVertex2f(cx1 + 0.04, cy);
    glVertex2f(cx1, cy - 0.04);
    glVertex2f(cx1, cy + 0.04);
    glEnd();

    // -------- FRONT WHEEL --------
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for(int i=0;i<360;i++){
        float t = i * 3.1416 / 180;
        glVertex2f(cx2 + 0.08*cos(t), cy + 0.08*sin(t));
    }
    glEnd();

    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
    for(int i=0;i<360;i++){
        float t = i * 3.1416 / 180;
        glVertex2f(cx2 + 0.04*cos(t), cy + 0.04*sin(t));
    }
    glEnd();

    // Cross (+)
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(cx2 - 0.04, cy);
    glVertex2f(cx2 + 0.04, cy);
    glVertex2f(cx2, cy - 0.04);
    glVertex2f(cx2, cy + 0.04);
    glEnd();
}

// -------- WINDOW 1 --------
void display1()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawCar();

    glFlush();
}

// -------- WINDOW 2 --------
void display2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // -------- SLOPE --------
    glColor3f(0.3, 0.9, 0.3);
    glBegin(GL_POLYGON);
    glVertex2f(-1, -0.5);
    glVertex2f(1, 0.5);
    glVertex2f(1, -1);
    glVertex2f(-1, -1);
    glEnd();

    // -------- TRANSFORM --------
    glTranslatef(carX, carY, 0);
    glRotatef(angle, 0, 0, 1);

    // Optional: make bigger in 2nd window
    glScalef(1.3, 1.3, 1);

    drawCar();

    glFlush();
}

// -------- ANIMATION --------
void update(int value)
{
    carX += 0.01 * cos(rad);
    carY += 0.01 * sin(rad);

    if (carX > 1.0 || carY > 1.0)
    {
        carX = -0.8;
        carY = -0.5;
    }

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

// -------- INIT --------
void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1, 1, -1, 1);
}

// -------- MAIN --------
int main(int argc, char** argv)
{
    rad = angle * 3.1416 / 180.0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Window 1
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Window 1: Jeep Design");
    init();
    glutDisplayFunc(display1);

    // Window 2
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 100);
    glutCreateWindow("Window 2: Car Moving on Slope");
    init();
    glutDisplayFunc(display2);

    glutTimerFunc(30, update, 0);

    glutMainLoop();
}
