#include <GLUT/glut.h>

typedef struct {
    GLfloat x;
    GLfloat y;
} Point;

#define MAX_TRAIL_POINTS 512

// Global variables for animation
static GLfloat boatPos = 0.8f;
static GLfloat boatScale = 1.2f;
static Point riverTrail[MAX_TRAIL_POINTS];
static int riverTrailCount = 0;
static int originalWindow = 0;
static int animationWindow = 0;

static void setup2D(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Function to draw the basic boat shape
static void drawBoat(GLfloat scale) {
    glPushMatrix();
    // This scale only affects the boat relative to its local center
    glScalef(scale, scale, 1.0f);

    // Hull (Brown)
    glColor3f(0.4f, 0.26f, 0.13f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.35f,  0.0f);
        glVertex2f( 0.35f,  0.0f);
        glVertex2f( 0.25f, -0.2f);
        glVertex2f(-0.25f, -0.2f);
    glEnd();

    // Mast (Grey)
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.4f);
    glEnd();

    // Sail (White)
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.35f);
        glVertex2f(-0.2f, 0.1f);
        glVertex2f(0.2f, 0.1f);
    glEnd();

    glPopMatrix();
}

// Window 1: Stays static at scale 1.0
static void displayOriginal(void) {
    glutSetWindow(originalWindow);
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Pass constant 1.0f so it never moves or scales
    drawBoat(1.0f);

    glutSwapBuffers();
}

// Window 2: Animates position, rotation, and scale
static void displayAnimation(void) {
    glutSetWindow(animationWindow);
    glClearColor(0.1f, 0.4f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // 1. Draw River Trail
    glColor3f(0.0f, 0.5f, 0.8f);
    glLineWidth(10.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < riverTrailCount; ++i) {
        glVertex2f(riverTrail[i].x, riverTrail[i].y);
    }
    glEnd();

    // 2. Draw Animated Boat
    glPushMatrix();
        glTranslatef(boatPos, boatPos, 0.0f); // Move diagonally
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);   // Rotate 45 degrees
        drawBoat(boatScale);                  // Pass the changing scale
    glPopMatrix();

    glutSwapBuffers();
}

static void update(int value) {
    // Update movement logic
    boatPos -= 0.005f;
    boatScale -= 0.002f;

    // Record trail
    if (boatPos > -1.0f && riverTrailCount < MAX_TRAIL_POINTS) {
        riverTrail[riverTrailCount].x = boatPos;
        riverTrail[riverTrailCount].y = boatPos;
        ++riverTrailCount;
    }

    // Reset loop
    if (boatPos < -1.3f) {
        boatPos = 0.8f;
        boatScale = 1.2f;
        riverTrailCount = 0;
    }

    // Refresh both windows
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create Original Window
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    originalWindow = glutCreateWindow("Original");
    setup2D();
    glutDisplayFunc(displayOriginal);

    // Create Animation Window
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(550, 100);
    animationWindow = glutCreateWindow("Animation");
    setup2D();
    glutDisplayFunc(displayAnimation);

    glutTimerFunc(25, update, 0);
    glutMainLoop();
    return 0;
}
