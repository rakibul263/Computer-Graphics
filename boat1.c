#include <GLUT/glut.h>
#include <math.h>

typedef struct {
    GLfloat x, y;
} Point;

#define MAX_TRAIL_POINTS 512

// Global variables
static GLfloat boatPos = 0.8f;
// boatScale removed from here as we want it fixed at 1.0f
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void drawBackground(void) {
    // Sky Gradient
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.6f, 0.9f); glVertex2f(-1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glColor3f(0.7f, 0.9f, 1.0f); glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Distant Clouds
    glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
    for(float i = -0.8f; i < 1.0f; i += 0.6f) {
        glBegin(GL_POLYGON);
            for(int j=0; j<360; j+=30) {
                float rad = j * 3.14159f / 180.0f;
                glVertex2f(i + cos(rad)*0.15f, 0.7f + sin(rad)*0.08f);
            }
        glEnd();
    }
}

static void drawBoat(GLfloat scale) {
    glPushMatrix();
    glScalef(scale, scale, 1.0f);

    // Hull
    glColor3f(0.45f, 0.29f, 0.15f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.35f, 0.0f); glVertex2f(0.35f, 0.0f);
        glColor3f(0.3f, 0.15f, 0.05f);
        glVertex2f(0.25f, -0.2f); glVertex2f(-0.25f, -0.2f);
    glEnd();

    // Mast
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f); glVertex2f(0.0f, 0.45f);
    glEnd();

    // Sail
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.4f);
        glVertex2f(-0.2f, 0.1f);
        glVertex2f(0.2f, 0.1f);
    glEnd();
    glPopMatrix();
}

static void displayOriginal(void) {
    glutSetWindow(originalWindow);
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    drawBoat(1.0f); // Original is fixed scale
    glutSwapBuffers();
}

static void displayAnimation(void) {
    glutSetWindow(animationWindow);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    // 1. Draw Linear Trail
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < riverTrailCount; ++i) {
        glVertex2f(riverTrail[i].x, riverTrail[i].y);
    }
    glEnd();

    // 2. Draw Animated Boat
    glPushMatrix();
        glTranslatef(boatPos, boatPos, 0.0f);
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
        // CHANGED: Pass 1.0f directly to keep the boat size constant
        drawBoat(1.0f);
    glPopMatrix();

    glutSwapBuffers();
}

static void update(int value) {
    boatPos -= 0.005f;

    // Record trail positions
    if (boatPos > -1.2f && riverTrailCount < MAX_TRAIL_POINTS) {
        riverTrail[riverTrailCount].x = boatPos;
        riverTrail[riverTrailCount].y = boatPos;
        ++riverTrailCount;
    }

    // Reset loop
    if (boatPos < -1.4f) {
        boatPos = 1.0f;
        riverTrailCount = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int windowSize = 500;

    // Window 1
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(100, 100);
    originalWindow = glutCreateWindow("Original");
    setup2D();
    glutDisplayFunc(displayOriginal);

    // Window 2
    glutInitWindowSize(windowSize, windowSize);
    glutInitWindowPosition(100 + windowSize + 20, 100);
    animationWindow = glutCreateWindow("Linear Animation - Fixed Scale");
    setup2D();
    glutDisplayFunc(displayAnimation);

    glutTimerFunc(25, update, 0);
    glutMainLoop();
    return 0;
}
