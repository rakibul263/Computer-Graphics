/*
 * ============================================================
 *  Car Animation - OpenGL / GLUT
 *  Features:
 *    - Smooth car driving on a hilly mountain road
 *    - Detailed car with wheels, roof-rack, windows
 *    - Awesome mountain scenery with snow caps & trees
 *    - Rain / weather effects with puddles
 *    - Tunnel with interior lighting
 *    - Automatic day-night cycle (Dawn -> Day -> Dusk -> Night)
 *    - Oncoming traffic (red car)
 *
 *  Compile (macOS):
 *    gcc car_animation.c -framework OpenGL -framework GLUT -lm -o car_animation
 *
 *  Compile (Linux):
 *    gcc car_animation.c -lGL -lGLU -lglut -lm -o car_animation
 *
 *  Compile (Windows / MinGW):
 *    gcc car_animation.c -lfreeglut -lopengl32 -lglu32 -lm -o car_animation
 * ============================================================
 */

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ------------------------------------------------------------------ */
/*  Constants & tuneable parameters                                    */
/* ------------------------------------------------------------------ */
#define W  800
#define H  500
#define PI 3.14159265358979323846f

#define CYCLE_FRAMES  1800   /* frames per full day-night cycle        */
#define NUM_DROPS     200    /* rain drop count                        */
#define NUM_STARS     16
#define CAR_SPEED     0.003f /* player car speed (NDC units/frame)     */
#define ON_SPEED      0.0025f/* oncoming car speed                     */

/* Tunnel NDC x-range */
#define TUNNEL_X0  0.10f
#define TUNNEL_X1  0.60f

/* ------------------------------------------------------------------ */
/*  Global state                                                       */
/* ------------------------------------------------------------------ */
static int   frame      = 0;
static int   paused     = 0;
static float carX       = -1.0f;   /* player car NDC x                */
static float oncomingX  =  0.8f;   /* oncoming car NDC x              */

/* Rain drops: x, y in NDC [-1,1] */
static float dropX[NUM_DROPS];
static float dropY[NUM_DROPS];

/* Stars: x, y in NDC (sky area) */
static float starX[NUM_STARS];
static float starY[NUM_STARS];

/* ------------------------------------------------------------------ */
/*  Small helpers                                                      */
/* ------------------------------------------------------------------ */
static float clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

static float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

/* Road height at NDC x (sinusoidal hill) */
static float roadY(float x)
{
    float nx = (x + 1.0f) * 0.5f;   /* map [-1,1] -> [0,1] */
    return -0.28f
           - sinf(nx * PI * 1.1f)  * 0.18f
           + sinf(nx * PI * 2.3f + 0.4f) * 0.05f;
}

/* Road tangent angle at NDC x */
static float roadAngle(float x)
{
    float dx = 0.005f;
    return atan2f(roadY(x + dx) - roadY(x - dx), 2.0f * dx);
}

/* Tunnel darkness factor [0,1] at NDC x */
static float tunnelFade(float x)
{
    float fade = 20.0f;   /* transition width in NDC */
    if (x < TUNNEL_X0 || x > TUNNEL_X1) return 0.0f;
    float left  = clampf((x - TUNNEL_X0) * fade, 0.0f, 1.0f);
    float right = clampf((TUNNEL_X1 - x) * fade, 0.0f, 1.0f);
    return left < right ? left : right;
}

/* ------------------------------------------------------------------ */
/*  Day-night helpers                                                  */
/* ------------------------------------------------------------------ */
/* Returns phase [0,1] within current cycle */
static float dayPhase(void)
{
    return (float)(frame % CYCLE_FRAMES) / (float)CYCLE_FRAMES;
}

static int isNight(void)
{
    float p = dayPhase();
    return (p > 0.65f || p < 0.15f);
}

/* Sky top & bottom colours interpolated from keyframes */
typedef struct { float r, g, b; } RGB;

static RGB lerpRGB(RGB a, RGB b, float t)
{
    RGB c = { lerp(a.r,b.r,t), lerp(a.g,b.g,t), lerp(a.b,b.b,t) };
    return c;
}

static void skyColors(RGB *top, RGB *bot)
{
    float p = dayPhase();
    /* Keyframes: night(0), dawn(0.15), day(0.35), dusk(0.58), night(0.65) */
    RGB nightT  = {0.02f, 0.03f, 0.17f};
    RGB nightB  = {0.10f, 0.12f, 0.37f};
    RGB dawnT   = {0.05f, 0.10f, 0.30f};
    RGB dawnB   = {0.77f, 0.47f, 0.23f};
    RGB dayT    = {0.10f, 0.42f, 0.78f};
    RGB dayB    = {0.77f, 0.91f, 0.97f};
    RGB duskT   = {0.11f, 0.16f, 0.42f};
    RGB duskB   = {0.87f, 0.44f, 0.19f};

    if (p < 0.15f) {
        *top = nightT; *bot = nightB;
    } else if (p < 0.22f) {
        float t = (p - 0.15f) / 0.07f;
        *top = lerpRGB(nightT, dawnT, t);
        *bot = lerpRGB(nightB, dawnB, t);
    } else if (p < 0.35f) {
        float t = (p - 0.22f) / 0.13f;
        *top = lerpRGB(dawnT, dayT, t);
        *bot = lerpRGB(dawnB, dayB, t);
    } else if (p < 0.50f) {
        *top = dayT; *bot = dayB;
    } else if (p < 0.58f) {
        float t = (p - 0.50f) / 0.08f;
        *top = lerpRGB(dayT, duskT, t);
        *bot = lerpRGB(dayB, duskB, t);
    } else if (p < 0.65f) {
        float t = (p - 0.58f) / 0.07f;
        *top = lerpRGB(duskT, nightT, t);
        *bot = lerpRGB(duskB, nightB, t);
    } else {
        *top = nightT; *bot = nightB;
    }
}

/* ------------------------------------------------------------------ */
/*  Drawing primitives                                                 */
/* ------------------------------------------------------------------ */

/* Filled polygon circle at (cx,cy) with radius r */
static void drawCircle(float cx, float cy, float r, int segs)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segs; i++) {
        float a = (float)i / (float)segs * 2.0f * PI;
        glVertex2f(cx + cosf(a) * r, cy + sinf(a) * r);
    }
    glEnd();
}

/* Axis-aligned filled quad */
static void drawRect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);
    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);
    glEnd();
}

/* Gradient sky quad */
static void drawSky(void)
{
    RGB t, b;
    skyColors(&t, &b);

    glBegin(GL_QUADS);
    glColor3f(b.r, b.g, b.b); glVertex2f(-1.0f, -1.0f);
    glColor3f(b.r, b.g, b.b); glVertex2f( 1.0f, -1.0f);
    glColor3f(t.r, t.g, t.b); glVertex2f( 1.0f,  1.0f);
    glColor3f(t.r, t.g, t.b); glVertex2f(-1.0f,  1.0f);
    glEnd();
}

/* ------------------------------------------------------------------ */
/*  Stars                                                              */
/* ------------------------------------------------------------------ */
static void initStars(void)
{
    srand((unsigned)42);
    for (int i = 0; i < NUM_STARS; i++) {
        starX[i] = -0.95f + (float)rand() / RAND_MAX * 1.90f;
        starY[i] =  0.10f + (float)rand() / RAND_MAX * 0.85f;
    }
}

static void drawStars(void)
{
    if (!isNight()) return;
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; i++) {
        float blink = 0.6f + 0.4f * sinf(frame * 0.04f + starX[i] * 10.0f);
        glColor4f(1.0f, 1.0f, 1.0f, blink);
        glVertex2f(starX[i], starY[i]);
    }
    glEnd();
    glPointSize(1.0f);
}

/* ------------------------------------------------------------------ */
/*  Sun / Moon                                                         */
/* ------------------------------------------------------------------ */
static void drawSunMoon(void)
{
    float p = dayPhase();
    int   nt = isNight();

    if (nt) {
        /* Moon */
        glColor3f(0.84f, 0.91f, 1.00f);
        drawCircle(0.75f, 0.72f, 0.07f, 40);
        /* Crater bite */
        glColor3f(0.72f, 0.79f, 0.90f);
        drawCircle(0.79f, 0.76f, 0.055f, 32);
    } else {
        /* Sun: x travels left to right during day half */
        float sunX = -0.8f + p * 3.2f;
        float sunY = 0.60f + sinf(p * PI) * 0.25f;

        /* Rays */
        glColor3f(1.0f, 0.90f, 0.30f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for (int i = 0; i < 12; i++) {
            float a = i * PI / 6.0f + frame * 0.007f;
            glVertex2f(sunX + cosf(a) * 0.06f, sunY + sinf(a) * 0.06f);
            glVertex2f(sunX + cosf(a) * 0.10f, sunY + sinf(a) * 0.10f);
        }
        glEnd();
        glLineWidth(1.0f);

        /* Disc */
        glColor3f(1.0f, 0.87f, 0.15f);
        drawCircle(sunX, sunY, 0.055f, 40);
    }
}

/* ------------------------------------------------------------------ */
/*  Clouds                                                             */
/* ------------------------------------------------------------------ */
static void drawCloudAt(float cx, float cy, float s, int nt)
{
    if (nt)
        glColor4f(0.25f, 0.28f, 0.38f, 0.88f);
    else
        glColor4f(1.0f, 1.0f, 1.0f, 0.88f);

    float bumps[][3] = {
        { 0.00f,  0.00f, 0.060f },
        { 0.07f, -0.02f, 0.048f },
        { 0.13f,  0.01f, 0.055f },
        { 0.20f, -0.02f, 0.042f },
        { 0.26f,  0.01f, 0.038f },
    };
    for (int b = 0; b < 5; b++)
        drawCircle(cx + bumps[b][0]*s, cy + bumps[b][1]*s, bumps[b][2]*s, 28);
}

static void drawClouds(void)
{
    int nt = isNight();
    float sp = (float)frame * 0.00012f;
    float offsets[] = { 0.0f, 0.37f, 0.71f };
    float ys[]      = { 0.72f, 0.78f, 0.65f };
    float scales[]  = { 1.0f, 0.82f, 0.68f };

    for (int i = 0; i < 3; i++) {
        float cx = fmodf(-1.0f + (sp + offsets[i]) * 1.5f, 2.2f) - 1.1f;
        drawCloudAt(cx, ys[i], scales[i], nt);
    }
}

/* ------------------------------------------------------------------ */
/*  Mountains                                                          */
/* ------------------------------------------------------------------ */
static void drawMountainRange(float peaks[][2], int n, float baseY, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-1.0f, baseY);
    glVertex2f(-1.0f, peaks[0][1]);
    for (int i = 0; i < n; i++)
        glVertex2f(peaks[i][0], peaks[i][1]);
    glVertex2f( 1.0f, peaks[n-1][1]);
    glVertex2f( 1.0f, baseY);
    glEnd();
}

static void drawSnowCap(float px, float py, float hw)
{
    glColor3f(0.93f, 0.96f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(px - hw, py + hw * 0.8f);
    glVertex2f(px,      py);
    glVertex2f(px + hw, py + hw * 0.8f);
    glEnd();
}

static void drawMountains(void)
{
    int nt = isNight();

    /* Far range */
    float far[][2] = {
        {-1.00f, 0.20f},{-0.80f, 0.55f},{-0.60f, 0.30f},
        {-0.40f, 0.68f},{-0.20f, 0.42f},{ 0.00f, 0.72f},
        { 0.20f, 0.38f},{ 0.40f, 0.60f},{ 0.60f, 0.28f},
        { 0.80f, 0.50f},{ 1.00f, 0.22f}
    };
    if (nt)
        drawMountainRange(far, 11, -0.30f, 0.12f, 0.17f, 0.37f);
    else
        drawMountainRange(far, 11, -0.30f, 0.36f, 0.55f, 0.84f);

    /* Snow caps on tallest far peaks */
    if (!nt) {
        drawSnowCap( 0.00f, 0.72f, 0.045f);
        drawSnowCap(-0.40f, 0.68f, 0.040f);
        drawSnowCap( 0.40f, 0.60f, 0.035f);
    }

    /* Near range */
    float near_[][2] = {
        {-1.00f, 0.05f},{-0.85f, 0.40f},{-0.65f, 0.18f},
        {-0.45f, 0.52f},{-0.25f, 0.25f},{-0.05f, 0.48f},
        { 0.15f, 0.22f},{ 0.35f, 0.44f},{ 0.55f, 0.16f},
        { 0.75f, 0.38f},{ 1.00f, 0.10f}
    };
    if (nt)
        drawMountainRange(near_, 11, -0.30f, 0.14f, 0.23f, 0.40f);
    else
        drawMountainRange(near_, 11, -0.30f, 0.24f, 0.43f, 0.71f);

    if (!nt) {
        drawSnowCap(-0.45f, 0.52f, 0.038f);
        drawSnowCap(-0.05f, 0.48f, 0.035f);
        drawSnowCap( 0.35f, 0.44f, 0.032f);
    }

    /* Green foothills */
    float hill[2][2] = { {-1.0f, -0.05f},{1.0f, -0.05f} };
    if (nt) {
        glColor3f(0.10f, 0.23f, 0.13f);
    } else {
        glColor3f(0.23f, 0.55f, 0.25f);
    }
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.10f);
    glVertex2f( 1.0f, -0.10f);
    glVertex2f( 1.0f, -0.30f);
    glVertex2f(-1.0f, -0.30f);
    glEnd();

    /* Smooth grass strip using bezier-like steps */
    glBegin(GL_TRIANGLE_STRIP);
    int steps = 80;
    for (int i = 0; i <= steps; i++) {
        float x  = -1.0f + 2.0f * i / steps;
        float ry = roadY(x);
        if (nt)
            glColor3f(0.09f, 0.20f, 0.11f);
        else
            glColor3f(0.20f, 0.50f, 0.22f);
        glVertex2f(x, ry + 0.01f);
        glVertex2f(x, ry - 0.30f);
    }
    glEnd();
}

/* ------------------------------------------------------------------ */
/*  Trees                                                              */
/* ------------------------------------------------------------------ */
static void drawTree(float x, int nt)
{
    float y  = roadY(x);
    float tw = 0.030f;
    float th = 0.120f;

    /* Trunk */
    glColor3f(nt ? 0.22f : 0.45f, nt ? 0.12f : 0.28f, nt ? 0.06f : 0.12f);
    drawRect(x - 0.008f, y, 0.016f, 0.022f);

    /* Three triangle tiers */
    float tc[][3] = {
        { nt?0.06f:0.12f,  nt?0.18f:0.42f,  nt?0.08f:0.16f },
        { nt?0.09f:0.15f,  nt?0.24f:0.49f,  nt?0.10f:0.18f },
        { nt?0.11f:0.18f,  nt?0.30f:0.56f,  nt?0.12f:0.22f },
    };
    float heights[] = { th * 0.50f, th * 0.70f, th * 0.90f };
    float widths[]  = { tw,         tw * 0.76f, tw * 0.55f };

    for (int i = 0; i < 3; i++) {
        glColor3f(tc[i][0], tc[i][1], tc[i][2]);
        float base = y + 0.022f + (i == 0 ? 0 : heights[i-1] * 0.55f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x - widths[i], base);
        glVertex2f(x + widths[i], base);
        glVertex2f(x,             base + heights[i]);
        glEnd();
    }
}

static void drawTrees(void)
{
    int nt = isNight();
    float positions[] = { -0.85f, -0.65f, -0.42f, 0.72f, 0.84f, 0.93f };
    for (int i = 0; i < 6; i++) {
        float tx = positions[i];
        /* Skip trees inside tunnel region */
        if (tx > TUNNEL_X0 - 0.05f && tx < TUNNEL_X1 + 0.05f) continue;
        drawTree(tx, nt);
    }
}

/* ------------------------------------------------------------------ */
/*  Road                                                               */
/* ------------------------------------------------------------------ */
static void drawRoad(void)
{
    int   nt     = isNight();
    int   steps  = 120;
    float thick  = 0.045f;

    /* Road surface */
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= steps; i++) {
        float x  = -1.0f + 2.0f * i / steps;
        float ry = roadY(x);
        glColor3f(nt ? 0.14f : 0.33f, nt ? 0.14f : 0.33f, nt ? 0.14f : 0.33f);
        glVertex2f(x, ry);
        glVertex2f(x, ry - thick);
    }
    glEnd();

    /* Dashed centre line */
    glColor3f(1.0f, 0.90f, 0.27f);
    glLineWidth(1.5f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x3333);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= steps; i++) {
        float x  = -1.0f + 2.0f * i / steps;
        glVertex2f(x, roadY(x) - thick * 0.5f);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);

    /* Puddles if raining (always on for this build; toggle via #define) */
    for (int i = 0; i < 7; i++) {
        float px  = -0.90f + i * 0.28f;
        float py  = roadY(px) - 0.012f;
        float a   = 0.06f + (i % 3) * 0.02f;
        glColor4f(0.48f, 0.71f, 0.89f, 0.45f);
        drawCircle(px, py, a * 0.5f, 20);
    }
}

/* ------------------------------------------------------------------ */
/*  Tunnel                                                             */
/* ------------------------------------------------------------------ */
static void drawTunnel(void)
{
    int  nt    = isNight();
    float entX = TUNNEL_X0;
    float extX = TUNNEL_X1;
    float entY = roadY(entX);
    float extY = roadY(extX);
    float ch   = 0.28f;   /* ceiling height above road */

    /* Ceiling fill */
    glColor3f(nt ? 0.07f : 0.22f, nt ? 0.07f : 0.22f, nt ? 0.10f : 0.28f);
    glBegin(GL_QUADS);
    glVertex2f(entX, entY);
    glVertex2f(extX, extY);
    glVertex2f(extX, extY + ch);
    glVertex2f(entX, entY + ch);
    glEnd();

    /* Side walls */
    glColor3f(0.30f, 0.30f, 0.35f);
    /* Left wall (entry) */
    drawRect(entX - 0.06f, entY - 0.05f, 0.06f, ch + 0.10f);
    /* Right wall (exit) */
    drawRect(extX,         extY - 0.05f, 0.06f, ch + 0.10f);

    /* Arch outlines */
    glColor3f(0.20f, 0.20f, 0.25f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    int asegs = 20;
    float acx = entX, acy = entY + ch * 0.5f, ar = ch * 0.52f;
    for (int i = 0; i <= asegs; i++) {
        float a = PI + (float)i / asegs * PI;
        glVertex2f(acx + cosf(a) * ar * 0.55f, acy + sinf(a) * ar);
    }
    glEnd();
    acx = extX; acy = extY + ch * 0.5f;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= asegs; i++) {
        float a = PI + (float)i / asegs * PI;
        glVertex2f(acx + cosf(a) * ar * 0.55f, acy + sinf(a) * ar);
    }
    glEnd();
    glLineWidth(1.0f);

    /* Interior tunnel lights */
    int numLights = 5;
    for (int i = 0; i < numLights; i++) {
        float lx = entX + (extX - entX) * (i + 0.5f) / numLights;
        float ly = lerp(entY, extY, (i + 0.5f) / numLights) + ch * 0.88f;
        glColor3f(1.0f, 0.95f, 0.70f);
        drawCircle(lx, ly, 0.012f, 12);
        /* Soft glow cone downward */
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 0.95f, 0.70f, 0.18f);
        glVertex2f(lx, ly - 0.005f);
        glColor4f(1.0f, 0.95f, 0.70f, 0.0f);
        for (int s = 0; s <= 20; s++) {
            float a = -PI * 0.25f + s * PI * 0.5f / 20.0f;
            glVertex2f(lx + cosf(a) * 0.07f, ly - 0.12f + sinf(a) * 0.02f);
        }
        glEnd();
    }

    /* Tunnel darkness overlay based on car position */
    float tf = tunnelFade(carX);
    if (tf > 0.0f) {
        glColor4f(0.0f, 0.0f, 0.0f, tf * 0.55f);
        drawRect(-1.0f, -1.0f, 2.0f, 2.0f);
    }
}

/* ------------------------------------------------------------------ */
/*  Rain                                                               */
/* ------------------------------------------------------------------ */
static void initRain(void)
{
    for (int i = 0; i < NUM_DROPS; i++) {
        dropX[i] = -1.0f + (float)rand() / RAND_MAX * 2.0f;
        dropY[i] = -1.0f + (float)rand() / RAND_MAX * 2.0f;
    }
}

static void updateRain(void)
{
    if (paused) return;
    for (int i = 0; i < NUM_DROPS; i++) {
        dropY[i] -= 0.022f + dropX[i] * 0.002f;
        dropX[i] += 0.005f;
        if (dropY[i] < -1.0f) {
            dropY[i] = 1.0f + (float)rand() / RAND_MAX * 0.5f;
            dropX[i] = -1.0f + (float)rand() / RAND_MAX * 2.0f;
        }
    }
}

static void drawRain(void)
{
    float tf = tunnelFade(carX);
    float alpha = 0.35f * (1.0f - tf * 0.9f);
    glColor4f(0.67f, 0.82f, 0.97f, alpha);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < NUM_DROPS; i++) {
        glVertex2f(dropX[i],          dropY[i]);
        glVertex2f(dropX[i] + 0.006f, dropY[i] - 0.028f);
    }
    glEnd();
}

/* ------------------------------------------------------------------ */
/*  Car drawing (shared by player & oncoming)                         */
/* ------------------------------------------------------------------ */
static void drawCarShape(int oncoming, int nt, float tf)
{
    /*
     * Car is drawn centred at origin, scaled to roughly 0.20 wide.
     * Caller does glTranslatef / glRotatef before calling.
     */
    float s  = 0.10f;   /* half-width scale */
    float bh = s * 0.70f;
    float fh = s * 0.55f;

    /* Body colour */
    float br = oncoming ? 0.80f : 0.13f;
    float bg = oncoming ? 0.13f : 0.40f;
    float bb = oncoming ? 0.13f : 0.80f;

    /* Back body block */
    glColor3f(br, bg, bb);
    glBegin(GL_QUADS);
    glVertex2f(-s,        -bh * 0.15f);
    glVertex2f(-s * 0.05f,-bh * 0.15f);
    glVertex2f(-s * 0.05f, bh * 0.85f);
    glVertex2f(-s,         bh * 0.85f);
    glEnd();

    /* Front body block */
    glColor3f(br * 0.80f, bg * 0.80f, bb * 0.80f);
    glBegin(GL_QUADS);
    glVertex2f(-s * 0.05f,-fh * 0.15f);
    glVertex2f( s,        -fh * 0.15f);
    glVertex2f( s,         fh * 0.85f);
    glVertex2f(-s * 0.05f, fh * 0.85f);
    glEnd();

    /* Roof */
    glColor3f(oncoming ? 0.60f : 0.90f,
              oncoming ? 0.10f : 0.18f,
              oncoming ? 0.10f : 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(-s * 0.90f, bh * 0.85f);
    glVertex2f(-s * 0.25f, bh * 0.85f);
    glVertex2f(-s * 0.25f, bh * 0.85f + s * 0.55f);
    glVertex2f(-s * 0.90f, bh * 0.85f + s * 0.55f);
    glEnd();

    /* Windshield */
    glColor4f(0.70f, 0.90f, 1.0f, 0.65f);
    glBegin(GL_QUADS);
    glVertex2f(-s * 0.25f, fh * 0.85f);
    glVertex2f(-s * 0.05f, fh * 0.85f);
    glVertex2f(-s * 0.25f, bh * 0.85f + s * 0.50f);
    glVertex2f(-s * 0.45f, bh * 0.85f + s * 0.50f);
    glEnd();

    /* Side window */
    glBegin(GL_QUADS);
    glVertex2f(-s * 0.88f, bh * 0.85f + s * 0.06f);
    glVertex2f(-s * 0.26f, bh * 0.85f + s * 0.06f);
    glVertex2f(-s * 0.26f, bh * 0.85f + s * 0.48f);
    glVertex2f(-s * 0.88f, bh * 0.85f + s * 0.48f);
    glEnd();

    /* Headlight */
    int needLight = nt || tf > 0.3f;
    glColor3f(1.0f, 0.97f, 0.68f);
    drawCircle(s * 0.95f, fh * 0.40f, s * 0.10f, 16);
    if (needLight) {
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 0.97f, 0.68f, 0.5f);
        glVertex2f(s * 0.95f, fh * 0.40f);
        glColor4f(1.0f, 0.97f, 0.68f, 0.0f);
        for (int i = 0; i <= 20; i++) {
            float a = -PI * 0.15f + i * PI * 0.30f / 20.0f;
            glVertex2f(s * 0.95f + cosf(a) * s * 2.2f,
                       fh * 0.40f + sinf(a) * s * 0.55f);
        }
        glEnd();
    }

    /* Tail light */
    glColor3f(1.0f, 0.20f, 0.20f);
    drawCircle(-s * 0.95f, fh * 0.40f, s * 0.08f, 12);

    /* Bumpers */
    glColor3f(0.55f, 0.55f, 0.55f);
    drawRect( s * 0.90f, -fh * 0.15f, s * 0.12f, fh * 0.35f);
    drawRect(-s * 1.02f, -fh * 0.15f, s * 0.12f, fh * 0.35f);

    /* Roof rack */
    glColor3f(0.35f, 0.35f, 0.35f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(-s * 0.90f, bh * 0.85f + s * 0.56f);
    glVertex2f(-s * 0.25f, bh * 0.85f + s * 0.56f);
    glEnd();
    glColor3f(0.75f, 0.22f, 0.18f);
    drawRect(-s * 0.88f, bh * 0.85f + s * 0.52f, s * 0.60f, s * 0.09f);
    glLineWidth(1.0f);

    /* Wheels */
    float wheelY = -fh * 0.15f;
    float wx[2]  = { -s * 0.60f, s * 0.65f };
    float wr     = s * 0.36f;
    for (int i = 0; i < 2; i++) {
        /* Tyre */
        glColor3f(0.15f, 0.15f, 0.15f);
        drawCircle(wx[i], wheelY, wr, 32);
        /* Rim */
        glColor3f(0.73f, 0.73f, 0.73f);
        drawCircle(wx[i], wheelY, wr * 0.60f, 24);
        /* Spokes */
        glColor3f(0.52f, 0.52f, 0.52f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for (int sp = 0; sp < 5; sp++) {
            float a = sp * 2.0f * PI / 5.0f + frame * 0.07f;
            glVertex2f(wx[i], wheelY);
            glVertex2f(wx[i] + cosf(a) * wr * 0.57f,
                       wheelY + sinf(a) * wr * 0.57f);
        }
        glEnd();
        glLineWidth(1.0f);
        /* Hub */
        glColor3f(0.88f, 0.88f, 0.88f);
        drawCircle(wx[i], wheelY, wr * 0.18f, 12);
    }
}

/* ------------------------------------------------------------------ */
/*  Exhaust puffs                                                      */
/* ------------------------------------------------------------------ */
static void drawExhaust(void)
{
    if (paused) return;
    float s = 0.10f;
    for (int i = 0; i < 4; i++) {
        float age = fmodf(frame * 0.08f + i * 0.5f, 2.0f);
        float px  = -s - age * 0.06f;
        float py  = -0.04f - age * 0.04f;
        float rad = s * 0.25f + age * s * 0.40f;
        glColor4f(0.60f, 0.60f, 0.60f, (1.0f - age * 0.5f) * 0.28f);
        drawCircle(px, py, rad, 16);
    }
}

/* ------------------------------------------------------------------ */
/*  Main display callback                                              */
/* ------------------------------------------------------------------ */
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    /* --- Background --- */
    drawSky();
    drawStars();
    drawSunMoon();
    drawClouds();
    drawMountains();
    drawTrees();
    drawRoad();

    /* --- Tunnel structure (drawn before cars so cars go inside) --- */
    drawTunnel();

    /* --- Oncoming car --- */
    {
        float ox  = oncomingX;
        float oy  = roadY(ox);
        float oa  = roadAngle(ox);
        glPushMatrix();
            glTranslatef(ox, oy, 0.0f);
            glRotatef((oa + PI) * 180.0f / PI, 0, 0, 1);
            drawCarShape(1, isNight(), tunnelFade(ox));
        glPopMatrix();
    }

    /* --- Player car exhaust + car --- */
    {
        float cy  = roadY(carX);
        float ca  = roadAngle(carX);
        float tf  = tunnelFade(carX);
        glPushMatrix();
            glTranslatef(carX, cy, 0.0f);
            glRotatef(ca * 180.0f / PI, 0, 0, 1);
            drawExhaust();
            drawCarShape(0, isNight(), tf);
        glPopMatrix();
    }

    /* --- Rain (on top of everything, reduced inside tunnel) --- */
    drawRain();

    /* --- Overlay label: time of day --- */
    {
        float p = dayPhase();
        const char *label = "Night";
        if      (p > 0.22f && p < 0.35f) label = "Dawn";
        else if (p >= 0.35f && p < 0.50f) label = "Day";
        else if (p >= 0.50f && p < 0.65f) label = "Dusk";

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.97f, 0.90f);
        for (const char *c = label; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glutSwapBuffers();
}

/* ------------------------------------------------------------------ */
/*  Timer callback (animation)                                         */
/* ------------------------------------------------------------------ */
static void timer(int value)
{
    (void)value;
    if (!paused) {
        frame++;

        /* Advance player car */
        carX += CAR_SPEED;
        if (carX > 1.05f) carX = -1.05f;

        /* Advance oncoming car (opposite direction) */
        oncomingX -= ON_SPEED;
        if (oncomingX < -1.05f) oncomingX = 1.05f;

        updateRain();
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);   /* ~60 fps */
}

/* ------------------------------------------------------------------ */
/*  Keyboard callback                                                  */
/* ------------------------------------------------------------------ */
static void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    if (key == ' ') paused = !paused;
    if (key == 27)  exit(0);   /* ESC */
}

/* ------------------------------------------------------------------ */
/*  Reshape callback                                                   */
/* ------------------------------------------------------------------ */
static void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f)
        gluOrtho2D(-aspect, aspect, -1.0f, 1.0f);
    else
        gluOrtho2D(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);
    glMatrixMode(GL_MODELVIEW);
}

/* ------------------------------------------------------------------ */
/*  Init                                                               */
/* ------------------------------------------------------------------ */
static void initGL(void)
{
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
}

/* ------------------------------------------------------------------ */
/*  Main                                                               */
/* ------------------------------------------------------------------ */
int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));
    initStars();
    initRain();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(100, 80);
    glutCreateWindow("Mountain Car Drive  |  SPACE = pause  |  ESC = quit");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
