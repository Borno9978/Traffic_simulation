#include "scene.h"
#include "environment.h"

static void drawFlatQuad(float x1, float z1, float x2, float z2, float y, float r, float g, float b) {
    setColor(r, g, b);
    glBegin(GL_QUADS);
    glVertex3f(x1, y, z1);
    glVertex3f(x2, y, z1);
    glVertex3f(x2, y, z2);
    glVertex3f(x1, y, z2);
    glEnd();
}

static void drawWindowGrid(float w, float h, float d) {
    float wx, wy;
    int row = 0;
    float night = 1.0f - gDayFactor;
    setColor(0.13f, 0.28f, 0.38f);

    glBegin(GL_QUADS);
    for (wy = 1.6f; wy < h - 1.0f; wy += 2.15f) {
        int col = 0;
        for (wx = -w * 0.5f + 0.85f; wx < w * 0.5f - 0.7f; wx += 1.65f) {
            int on = ((row * 3 + col * 5 + (int)(w * 10.0f)) % 4) != 0;
            if (night > 0.35f && on) {
                setColor(0.95f, 0.72f, 0.32f);
            } else {
                setColor(0.12f, 0.25f, 0.32f);
            }
            glVertex3f(wx, wy, d * 0.5f + 0.025f);
            glVertex3f(wx + 0.74f, wy, d * 0.5f + 0.025f);
            glVertex3f(wx + 0.74f, wy + 0.9f, d * 0.5f + 0.025f);
            glVertex3f(wx, wy + 0.9f, d * 0.5f + 0.025f);
            col++;
        }
        row++;
    }
    glEnd();

    row = 0;
    glBegin(GL_QUADS);
    for (wy = 2.0f; wy < h - 1.2f; wy += 2.4f) {
        if (night > 0.45f && (row % 2 == 0)) {
            setColor(0.95f, 0.70f, 0.30f);
        } else {
            setColor(0.16f, 0.30f, 0.38f);
        }
        glVertex3f(w * 0.5f + 0.025f, wy, -d * 0.5f + 0.7f);
        glVertex3f(w * 0.5f + 0.025f, wy, -d * 0.5f + 1.35f);
        glVertex3f(w * 0.5f + 0.025f, wy + 0.75f, -d * 0.5f + 1.35f);
        glVertex3f(w * 0.5f + 0.025f, wy + 0.75f, -d * 0.5f + 0.7f);
        row++;
    }
    glEnd();
}

static void drawDoor(float w, float d) {
    setColor(0.18f, 0.20f, 0.22f);
    glBegin(GL_QUADS);
    glVertex3f(-0.55f, 0.0f, d * 0.5f + 0.035f);
    glVertex3f(0.55f, 0.0f, d * 0.5f + 0.035f);
    glVertex3f(0.55f, 1.55f, d * 0.5f + 0.035f);
    glVertex3f(-0.55f, 1.55f, d * 0.5f + 0.035f);
    glEnd();

    setColor(0.82f, 0.88f, 0.90f);
    glBegin(GL_QUADS);
    glVertex3f(-w * 0.5f + 0.5f, 0.15f, d * 0.5f + 0.035f);
    glVertex3f(-w * 0.5f + 1.2f, 0.15f, d * 0.5f + 0.035f);
    glVertex3f(-w * 0.5f + 1.2f, 1.1f, d * 0.5f + 0.035f);
    glVertex3f(-w * 0.5f + 0.5f, 1.1f, d * 0.5f + 0.035f);
    glEnd();
}

static void drawBuilding(float x, float z, float w, float h, float d, float r, float g, float b, int style) {
    glPushMatrix();
    glTranslatef(x, h * 0.5f, z);
    drawBox(w, h, d, r, g, b);

    glPushMatrix();
    glTranslatef(0.0f, -h * 0.5f, 0.0f);
    drawWindowGrid(w, h, d);
    drawDoor(w, d);
    glPopMatrix();

    glTranslatef(0.0f, h * 0.5f + 0.12f, 0.0f);
    if (style == 0) {
        drawBox(w * 0.96f, 0.28f, d * 0.96f, 0.28f, 0.30f, 0.32f);
    } else if (style == 1) {
        drawBox(w * 0.78f, 0.55f, d * 0.78f, r * 0.70f, g * 0.70f, b * 0.70f);
        glTranslatef(0.0f, 0.42f, 0.0f);
        drawBox(w * 0.45f, 0.35f, d * 0.45f, r * 0.62f, g * 0.62f, b * 0.62f);
    } else {
        drawBox(w * 1.08f, 0.32f, d * 1.08f, 0.78f, 0.28f, 0.20f);
        glTranslatef(0.0f, -0.55f, d * 0.52f);
        drawBox(w * 0.95f, 0.45f, 0.28f, 0.95f, 0.64f, 0.22f);
    }
    glPopMatrix();
}

static void drawStepBuilding(float x, float z, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    drawBuilding(0.0f, 0.0f, 9.5f, 7.0f, 8.0f, r, g, b, 0);
    glTranslatef(2.1f, 7.2f, -0.4f);
    drawBox(4.2f, 3.5f, 6.4f, r * 0.92f, g * 0.92f, b * 0.92f);
    glPopMatrix();
}

static void drawTree(float x, float z, float scale) {
    /* Per-tree sway driven by world position so neighbors don't move in lockstep.
     * Wind strength is amplified during rain/storm weather. */
    float windBase = (gWeatherMode == 1) ? 4.5f : (gWeatherMode == 3 ? 2.2f : 1.8f);
    float swayX = sinf(gSceneTime * 1.3f + x * 0.13f + z * 0.07f) * windBase;
    float swayZ = cosf(gSceneTime * 1.0f + x * 0.09f - z * 0.11f) * windBase * 0.6f;
    /* Seasonal canopy color: spring/summer green, autumn orange, winter pale */
    float canopy[4][3] = {
        {0.22f, 0.66f, 0.26f},
        {0.20f, 0.62f, 0.25f},
        {0.86f, 0.48f, 0.16f},
        {0.78f, 0.82f, 0.80f}
    };
    int next = (gSeason + 1) % 4;
    float cr = mixf(canopy[gSeason][0], canopy[next][0], gSeasonBlend);
    float cg = mixf(canopy[gSeason][1], canopy[next][1], gSeasonBlend);
    float cb = mixf(canopy[gSeason][2], canopy[next][2], gSeasonBlend);
    cr = mixf(0.05f, cr, gDayFactor);
    cg = mixf(0.10f, cg, gDayFactor);
    cb = mixf(0.08f, cb, gDayFactor);

    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);

    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    drawUnitCylinder(0.22f, 2.0f, 7, 0.48f, 0.30f, 0.16f);
    glPopMatrix();

    /* Canopy pivots at the trunk top, sways in wind */
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(swayZ, 1.0f, 0.0f, 0.0f);
    glRotatef(swayX, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.75f, 0.0f);
    drawLowPolySphere(1.25f, cr, cg, cb);

    glPushMatrix();
    glTranslatef(-0.48f, -0.55f, 0.25f);
    drawLowPolySphere(0.82f, cr * 0.85f, cg * 0.88f, cb * 0.85f);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

static void drawLamp(float x, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 2.4f, 0.0f);
    drawUnitCylinder(0.09f, 4.8f, 8, 0.36f, 0.39f, 0.42f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75f, 4.65f, 0.0f);
    drawBox(1.5f, 0.12f, 0.12f, 0.36f, 0.39f, 0.42f);
    glTranslatef(0.75f, -0.12f, 0.0f);
    drawLowPolySphere(0.28f, 1.0f, 0.92f, 0.48f);
    if (gDayFactor < 0.45f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.82f, 0.30f, 0.28f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, 0.0f);
        for (int i = 0; i <= 24; ++i) {
            float a = 2.0f * PI * (float)i / 24.0f;
            glVertex3f(cosf(a) * 1.0f, -0.05f, sinf(a) * 1.0f);
        }
        glEnd();
        glDisable(GL_BLEND);
    }
    glPopMatrix();
    glPopMatrix();
}

static void drawBench(float x, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, 0.45f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);

    drawBox(2.6f, 0.24f, 0.65f, 0.64f, 0.36f, 0.20f);
    glPushMatrix();
    glTranslatef(0.0f, 0.65f, -0.28f);
    drawBox(2.7f, 0.85f, 0.18f, 0.55f, 0.29f, 0.16f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.95f, -0.28f, 0.0f);
    drawBox(0.14f, 0.56f, 0.55f, 0.18f, 0.18f, 0.18f);
    glTranslatef(1.9f, 0.0f, 0.0f);
    drawBox(0.14f, 0.56f, 0.55f, 0.18f, 0.18f, 0.18f);
    glPopMatrix();
    glPopMatrix();
}

static void drawShrub(float x, float z, float s) {
    glPushMatrix();
    glTranslatef(x, 0.45f, z);
    glScalef(s, s, s);
    drawLowPolySphere(0.7f, 0.42f, 0.78f, 0.22f);
    glTranslatef(0.55f, 0.02f, 0.25f);
    drawLowPolySphere(0.55f, 0.34f, 0.70f, 0.18f);
    glTranslatef(-1.0f, 0.03f, -0.18f);
    drawLowPolySphere(0.48f, 0.48f, 0.82f, 0.26f);
    glPopMatrix();
}

static void drawPerson(float x, float z, float rot, float shirtR, float shirtG, float shirtB) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 1.35f, 0.0f);
    drawBox(0.34f, 0.9f, 0.22f, shirtR, shirtG, shirtB);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 2.05f, 0.0f);
    drawLowPolySphere(0.22f, 0.92f, 0.70f, 0.50f);
    glTranslatef(0.0f, 0.10f, -0.02f);
    drawBox(0.28f, 0.12f, 0.24f, 0.10f, 0.07f, 0.05f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.09f, 0.62f, 0.0f);
    drawBox(0.12f, 0.7f, 0.12f, 0.15f, 0.22f, 0.30f);
    glTranslatef(0.18f, 0.0f, 0.0f);
    drawBox(0.12f, 0.7f, 0.12f, 0.15f, 0.22f, 0.30f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.28f, 1.35f, 0.02f);
    glRotatef(-18.0f, 0.0f, 0.0f, 1.0f);
    drawBox(0.10f, 0.65f, 0.10f, 0.90f, 0.67f, 0.48f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.28f, 1.35f, 0.02f);
    glRotatef(18.0f, 0.0f, 0.0f, 1.0f);
    drawBox(0.10f, 0.65f, 0.10f, 0.90f, 0.67f, 0.48f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.12f, 0.16f, 0.04f);
    drawBox(0.20f, 0.09f, 0.30f, 0.05f, 0.06f, 0.07f);
    glTranslatef(0.24f, 0.0f, 0.0f);
    drawBox(0.20f, 0.09f, 0.30f, 0.05f, 0.06f, 0.07f);
    glPopMatrix();
    glPopMatrix();
}

static void drawBike(float x, float z, float rot, float riderR, float riderG, float riderB) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(-0.55f, 0.43f, 0.0f);
    drawUnitCylinder(0.28f, 0.08f, 10, 0.05f, 0.05f, 0.05f);
    glTranslatef(1.1f, 0.0f, 0.0f);
    drawUnitCylinder(0.28f, 0.08f, 10, 0.05f, 0.05f, 0.05f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.75f, 0.0f);
    drawBox(1.15f, 0.07f, 0.08f, 0.10f, 0.12f, 0.13f);
    glTranslatef(0.25f, 0.38f, 0.0f);
    drawBox(0.12f, 0.62f, 0.12f, riderR, riderG, riderB);
    glTranslatef(0.0f, 0.47f, 0.0f);
    drawLowPolySphere(0.16f, 0.90f, 0.68f, 0.48f);
    glPopMatrix();
    glPopMatrix();
}

static void drawCloud(float x, float y, float z, float s) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(s, s, s);
    drawLowPolySphere(0.8f, 0.96f, 0.98f, 1.0f);
    glTranslatef(0.75f, 0.1f, 0.05f);
    drawLowPolySphere(0.62f, 0.96f, 0.98f, 1.0f);
    glTranslatef(-1.45f, -0.03f, 0.02f);
    drawLowPolySphere(0.55f, 0.96f, 0.98f, 1.0f);
    glPopMatrix();
}

static void drawSkyBlob(float cx, float cy, float rx, float ry, float alpha) {
    glColor4f(1.0f, 1.0f, 0.96f, alpha);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 28; ++i) {
        float a = 2.0f * PI * (float)i / 28.0f;
        glVertex2f(cx + cosf(a) * rx, cy + sinf(a) * ry);
    }
    glEnd();
}

static void drawScreenCloud(float x, float y, float s, float alpha) {
    drawSkyBlob(x - 0.050f * s, y, 0.040f * s, 0.030f * s, alpha);
    drawSkyBlob(x - 0.015f * s, y + 0.015f * s, 0.052f * s, 0.040f * s, alpha);
    drawSkyBlob(x + 0.035f * s, y + 0.004f * s, 0.047f * s, 0.032f * s, alpha);
    drawSkyBlob(x + 0.078f * s, y - 0.006f * s, 0.038f * s, 0.026f * s, alpha);
}

static void drawStars(float alpha) {
    float stars[][2] = {
        {0.08f, 0.88f}, {0.16f, 0.72f}, {0.25f, 0.91f}, {0.34f, 0.78f},
        {0.43f, 0.86f}, {0.53f, 0.74f}, {0.62f, 0.93f}, {0.71f, 0.70f},
        {0.83f, 0.87f}, {0.93f, 0.76f}, {0.12f, 0.58f}, {0.48f, 0.61f},
        {0.77f, 0.55f}, {0.90f, 0.62f}
    };
    glColor4f(1.0f, 1.0f, 0.86f, alpha);
    glBegin(GL_QUADS);
    for (int i = 0; i < 14; ++i) {
        float x = stars[i][0];
        float y = stars[i][1];
        float s = 0.0035f + 0.0015f * (float)(i % 3);
        glVertex2f(x - s, y);
        glVertex2f(x, y + s);
        glVertex2f(x + s, y);
        glVertex2f(x, y - s);
    }
    glEnd();
}

static void drawBirdsAndPlane(void) {
    int i;
    float baseX = fmodf(gSceneTime * 4.5f, 120.0f) - 60.0f;
    float planeX = fmodf(gSceneTime * 2.0f, 150.0f) - 75.0f;

    glLineWidth(2.0f);
    setColor(0.06f, 0.07f, 0.08f);
    glBegin(GL_LINES);
    for (i = 0; i < 9; ++i) {
        float bx = baseX + (float)i * 5.0f;
        float by = 18.0f + sinf(gSceneTime * 2.0f + (float)i) * 1.2f;
        float bz = -38.0f - (float)(i % 3) * 4.0f;
        glVertex3f(bx - 0.55f, by, bz);
        glVertex3f(bx, by + 0.32f, bz);
        glVertex3f(bx, by + 0.32f, bz);
        glVertex3f(bx + 0.55f, by, bz);
    }
    glEnd();
    glLineWidth(1.0f);

    glPushMatrix();
    glTranslatef(planeX, 25.0f + sinf(gSceneTime * 0.7f) * 1.0f, -55.0f);
    glRotatef(8.0f, 0.0f, 1.0f, 0.0f);
    drawBox(3.5f, 0.45f, 0.55f, 0.88f, 0.92f, 0.94f);
    drawBox(1.6f, 0.10f, 4.0f, 0.78f, 0.84f, 0.90f);
    glTranslatef(-1.55f, 0.18f, 0.0f);
    drawBox(0.7f, 0.08f, 1.4f, 0.78f, 0.84f, 0.90f);
    glPopMatrix();
}

static void drawFireflies(void) {
    /* Tiny glowing dots near trees that pulse in and out — only visible at night.
     * Hand-placed clusters so they sit next to actual tree positions. */
    static const float spots[][2] = {
        {-18.0f, -17.0f}, {-27.0f, -15.8f}, {-43.0f, -34.0f},
        { 18.8f, -17.0f}, { 31.0f, -16.2f}, { 46.0f, -33.0f},
        {-18.0f,  17.2f}, {-36.0f,  33.0f},
        { 18.2f,  17.2f}, { 31.0f,  16.5f}, { 45.0f,  26.0f}
    };
    int nClusters = 11;
    int i, k;
    float night = clamp01((0.55f - gDayFactor) / 0.40f);
    if (night < 0.05f) return;

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (i = 0; i < nClusters; ++i) {
        for (k = 0; k < 4; ++k) {
            float seed = (float)(i * 7 + k * 13);
            float t = gSceneTime * 0.9f + seed;
            float fx = spots[i][0] + sinf(t * 1.7f + seed) * 1.6f + cosf(seed * 0.9f) * 1.2f;
            float fz = spots[i][1] + cosf(t * 1.3f + seed * 1.1f) * 1.4f + sinf(seed * 1.3f) * 1.1f;
            float fy = 1.6f + sinf(t * 2.1f + seed) * 0.8f + (float)(k % 2) * 0.4f;
            float pulse = 0.5f + 0.5f * sinf(t * 4.0f + seed * 2.7f);
            float a = night * (0.25f + 0.55f * pulse);
            glColor4f(1.0f, 0.96f, 0.42f, a);
            glVertex3f(fx, fy, fz);
        }
    }
    glEnd();
    glPointSize(1.0f);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

static void drawRoadPuddles(void) {
    /* Semi-transparent bright patches on the road during rain — fakes
     * reflective wet asphalt without a stencil/reflection pass. */
    static const float puddles[][2] = {
        { -3.5f, -42.0f}, {  6.2f, -28.0f}, { -7.0f, -12.0f},
        {  4.0f,   6.0f}, { -2.0f,  22.0f}, {  7.0f,  38.0f},
        {-38.0f,   3.6f}, {-22.0f,  -6.0f}, { -8.0f,   2.0f},
        { 20.0f,  -3.0f}, { 36.0f,   5.5f}, { 52.0f,  -2.0f},
        {-24.0f,   2.4f}, {  2.0f,  60.0f}, {-30.0f, -55.0f}
    };
    int n = 15;
    int i, k;

    if (gWeatherMode != 1) return;

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (i = 0; i < n; ++i) {
        float cx = puddles[i][0];
        float cz = puddles[i][1];
        float ripple = 1.0f + 0.06f * sinf(gSceneTime * 3.0f + (float)i);
        float r = (1.4f + 0.4f * ((float)(i % 3))) * ripple;
        glColor4f(0.62f, 0.74f, 0.86f, 0.42f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cx, 0.115f, cz);
        for (k = 0; k <= 16; ++k) {
            float a = 2.0f * PI * (float)k / 16.0f;
            glVertex3f(cx + cosf(a) * r, 0.115f, cz + sinf(a) * r * 0.55f);
        }
        glEnd();

        glColor4f(0.92f, 0.96f, 1.00f, 0.18f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cx, 0.116f, cz);
        for (k = 0; k <= 16; ++k) {
            float a = 2.0f * PI * (float)k / 16.0f;
            glVertex3f(cx + cosf(a) * r * 0.55f, 0.116f, cz + sinf(a) * r * 0.30f);
        }
        glEnd();
    }
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

static void drawWeather(void) {
    int i;

    if (gWeatherMode == 0) return;

    if (gWeatherMode == 1) {
        glLineWidth(1.2f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.55f, 0.75f, 0.95f, 0.55f);
        glBegin(GL_LINES);
        for (i = 0; i < 90; ++i) {
            float x = -60.0f + (float)((i * 17) % 120);
            float z = -58.0f + (float)((i * 31) % 116);
            float fall = fmodf(gSceneTime * 18.0f + (float)(i * 3), 24.0f);
            float y = 24.0f - fall;
            glVertex3f(x, y, z);
            glVertex3f(x - 0.35f, y - 2.4f, z + 0.15f);
        }
        glEnd();
        glDisable(GL_BLEND);
        glLineWidth(1.0f);
    } else if (gWeatherMode == 2) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.82f, 0.88f, 0.90f, 0.28f);
        glBegin(GL_QUADS);
        glVertex3f(-85.0f, 0.4f, -60.0f);
        glVertex3f(85.0f, 0.4f, -60.0f);
        glVertex3f(85.0f, 16.0f, -60.0f);
        glVertex3f(-85.0f, 16.0f, -60.0f);

        glColor4f(0.90f, 0.94f, 0.95f, 0.18f);
        glVertex3f(-85.0f, 0.2f, 12.0f);
        glVertex3f(85.0f, 0.2f, 12.0f);
        glVertex3f(85.0f, 13.0f, 12.0f);
        glVertex3f(-85.0f, 13.0f, 12.0f);
        glEnd();
        glDisable(GL_BLEND);
    } else if (gWeatherMode == 3) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 0.78f);
        glBegin(GL_QUADS);
        for (i = 0; i < 80; ++i) {
            float x = -62.0f + (float)((i * 19) % 124);
            float z = -58.0f + (float)((i * 29) % 116);
            float fall = fmodf(gSceneTime * 3.6f + (float)(i * 5), 24.0f);
            float y = 24.0f - fall;
            float s = 0.10f + 0.05f * (float)(i % 3);
            glVertex3f(x - s, y, z);
            glVertex3f(x, y + s, z);
            glVertex3f(x + s, y, z);
            glVertex3f(x, y - s, z);
        }
        glEnd();
        glDisable(GL_BLEND);
    }
}

static float crossingProgress(void) {
    return fmodf(gSceneTime * 0.18f, 1.0f);
}

static void drawSignalAwarePedestrians(void) {
    float t = crossingProgress();
    float walkBob = sinf(gSceneTime * 12.0f) * 3.0f;

    if (gTrafficPhase == 2) {
        drawPerson(-11.5f + 23.0f * t, 12.45f, 90.0f + walkBob, 0.20f, 0.48f, 0.86f);
        drawPerson(11.5f - 23.0f * t, -12.45f, -90.0f - walkBob, 0.94f, 0.32f, 0.22f);
    } else {
        drawPerson(-16.0f, 12.45f, 90.0f, 0.20f, 0.48f, 0.86f);
        drawPerson(16.0f, -12.45f, -90.0f, 0.94f, 0.32f, 0.22f);
    }

    if (gTrafficPhase == 0) {
        drawPerson(-12.45f, -11.5f + 23.0f * t, 0.0f + walkBob, 0.98f, 0.72f, 0.20f);
        drawPerson(12.45f, 11.5f - 23.0f * t, 180.0f - walkBob, 0.92f, 0.82f, 0.18f);
    } else {
        drawPerson(-12.45f, -16.0f, 0.0f, 0.98f, 0.72f, 0.20f);
        drawPerson(12.45f, 16.0f, 180.0f, 0.92f, 0.82f, 0.18f);
    }
}

void drawSky(void) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    float dayT = fmodf(gDayClock / 55.0f, 1.0f);
    float sx = 0.10f + 0.82f * dayT;
    float sy = 0.18f + 0.64f * sinf(dayT * PI);
    float mx = 1.02f - sx;
    float my = 0.20f + 0.55f * sinf(fmodf(dayT + 0.5f, 1.0f) * PI);
    float d = gDayFactor;
    float topR = mixf(0.03f, 0.30f, d);
    float topG = mixf(0.06f, 0.66f, d);
    float topB = mixf(0.18f, 0.96f, d);
    float botR = mixf(0.07f, 0.76f, d);
    float botG = mixf(0.10f, 0.92f, d);
    float botB = mixf(0.24f, 0.99f, d);

    if (gWeatherMode == 1) {
        topR = mixf(topR, 0.28f, 0.55f);
        topG = mixf(topG, 0.38f, 0.55f);
        topB = mixf(topB, 0.48f, 0.55f);
        botR = mixf(botR, 0.45f, 0.45f);
        botG = mixf(botG, 0.56f, 0.45f);
        botB = mixf(botB, 0.62f, 0.45f);
    } else if (gWeatherMode == 2) {
        topR = mixf(topR, 0.66f, 0.55f);
        topG = mixf(topG, 0.72f, 0.55f);
        topB = mixf(topB, 0.76f, 0.55f);
        botR = mixf(botR, 0.84f, 0.50f);
        botG = mixf(botG, 0.88f, 0.50f);
        botB = mixf(botB, 0.90f, 0.50f);
    } else if (gWeatherMode == 3) {
        topR = mixf(topR, 0.62f, 0.38f);
        topG = mixf(topG, 0.74f, 0.38f);
        topB = mixf(topB, 0.86f, 0.38f);
        botR = mixf(botR, 0.90f, 0.45f);
        botG = mixf(botG, 0.94f, 0.45f);
        botB = mixf(botB, 0.96f, 0.45f);
    }

    glBegin(GL_QUADS);
    setColor(topR, topG, topB);
    glVertex2f(0.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    setColor(botR, botG, botB);
    glVertex2f(1.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawStars((1.0f - d) * 0.85f);

    glColor4f(1.0f, 0.92f, 0.42f, 0.18f * d);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(sx, sy);
    for (int i = 0; i <= 48; ++i) {
        float a = 2.0f * PI * (float)i / 48.0f;
        glVertex2f(sx + cosf(a) * 0.115f, sy + sinf(a) * 0.155f);
    }
    glEnd();

    glColor4f(1.0f, 0.96f, 0.62f, 0.26f * d);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(sx, sy);
    for (int i = 0; i <= 48; ++i) {
        float a = 2.0f * PI * (float)i / 48.0f;
        glVertex2f(sx + cosf(a) * 0.082f, sy + sinf(a) * 0.110f);
    }
    glEnd();

    glColor4f(1.0f, 0.98f, 0.76f, 0.22f * d);
    glBegin(GL_LINES);
    for (int i = 0; i < 18; ++i) {
        float a = 2.0f * PI * (float)i / 18.0f;
        glVertex2f(sx + cosf(a) * 0.070f, sy + sinf(a) * 0.094f);
        glVertex2f(sx + cosf(a) * 0.135f, sy + sinf(a) * 0.178f);
    }
    glEnd();

    glColor4f(1.0f, 0.90f, 0.32f, d);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(sx, sy);
    for (int i = 0; i <= 32; ++i) {
        float a = 2.0f * PI * (float)i / 32.0f;
        glVertex2f(sx + cosf(a) * 0.055f, sy + sinf(a) * 0.075f);
    }
    glEnd();

    glColor4f(0.88f, 0.92f, 1.0f, 1.0f - d);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(mx, my);
    for (int i = 0; i <= 28; ++i) {
        float a = 2.0f * PI * (float)i / 28.0f;
        glVertex2f(mx + cosf(a) * 0.040f, my + sinf(a) * 0.055f);
    }
    glEnd();

    {
        float cloudA = fmodf(gSceneTime * 0.012f, 1.35f) - 0.18f;
        float cloudB = fmodf(gSceneTime * 0.008f + 0.35f, 1.35f) - 0.18f;
        float cloudC = fmodf(gSceneTime * 0.015f + 0.70f, 1.35f) - 0.18f;
        drawScreenCloud(cloudA, 0.80f, 1.35f, mixf(0.50f, 0.90f, d));
        drawScreenCloud(cloudB, 0.72f, 1.05f, mixf(0.42f, 0.82f, d));
        drawScreenCloud(cloudC, 0.90f, 0.72f, mixf(0.38f, 0.68f, d));
        drawScreenCloud(0.18f, 0.84f, 0.95f, 0.60f);
        drawScreenCloud(0.91f, 0.91f, 0.70f, 0.58f);
    }

    glDisable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawEnvironment(void) {
    if (gWeatherMode == 3) {
        drawFlatQuad(-70.0f, -70.0f, 70.0f, 70.0f, -0.02f, 0.78f, 0.82f, 0.78f);
    } else if (gWeatherMode == 1) {
        drawFlatQuad(-70.0f, -70.0f, 70.0f, 70.0f, -0.02f, 0.42f, 0.60f, 0.42f);
    } else {
        drawFlatQuad(-70.0f, -70.0f, 70.0f, 70.0f, -0.02f, 0.54f, 0.78f, 0.48f);
    }

    drawBuilding(-37.0f, -31.0f, 11.0f, 8.0f, 9.0f, 0.38f, 0.55f, 0.68f, 0);
    drawBuilding(-22.0f, -32.0f, 10.0f, 9.0f, 8.0f, 0.86f, 0.78f, 0.64f, 1);
    drawBuilding(-35.0f, -16.5f, 8.0f, 7.0f, 7.0f, 0.82f, 0.44f, 0.33f, 2);
    drawStepBuilding(-51.0f, -25.0f, 0.78f, 0.70f, 0.58f);

    drawBuilding(22.0f, -32.0f, 12.0f, 8.5f, 9.0f, 0.88f, 0.78f, 0.61f, 1);
    drawBuilding(38.0f, -26.0f, 10.0f, 10.0f, 9.0f, 0.88f, 0.52f, 0.26f, 2);
    drawBuilding(25.0f, -17.0f, 9.0f, 7.5f, 7.0f, 0.54f, 0.70f, 0.83f, 0);
    drawStepBuilding(52.0f, -18.0f, 0.64f, 0.76f, 0.82f);

    drawBuilding(-50.0f, 27.0f, 9.0f, 8.0f, 7.0f, 0.58f, 0.66f, 0.70f, 1);
    drawBuilding(-59.0f, 22.0f, 8.0f, 7.0f, 8.0f, 0.78f, 0.48f, 0.34f, 2);
    drawBuilding(55.0f, 24.0f, 9.5f, 8.5f, 8.0f, 0.86f, 0.76f, 0.58f, 1);
    drawBuilding(61.0f, 34.0f, 8.0f, 7.0f, 7.0f, 0.75f, 0.43f, 0.32f, 2);

    float skylineX[] = {-58.0f, -43.5f, -29.0f, -15.0f, 15.0f, 29.0f, 43.5f, 58.0f};
    for (int i = 0; i < 8; ++i) {
        float x = skylineX[i];
        drawBuilding(x, -56.0f, 8.0f, 6.0f + (float)(i % 3) * 2.0f, 7.0f, 0.70f, 0.80f, 0.86f, i % 3);
    }

    float trees[][3] = {
        {-18.0f, -17.0f, 0.9f}, {-27.0f, -15.8f, 0.8f}, {-43.0f, -34.0f, 1.0f},
        {18.8f, -17.0f, 0.9f}, {31.0f, -16.2f, 0.8f}, {46.0f, -33.0f, 1.05f},
        {-18.0f, 17.2f, 0.9f}, {-36.0f, 33.0f, 0.95f}, {-22.0f, 30.0f, 0.75f},
        {18.2f, 17.2f, 0.9f}, {31.0f, 16.5f, 0.82f}, {45.0f, 26.0f, 1.05f}
    };
    for (int i = 0; i < 12; ++i) {
        drawTree(trees[i][0], trees[i][1], trees[i][2]);
    }

    float backTrees[][2] = {
        {-58.0f, -43.0f}, {-44.0f, -43.0f}, {-30.0f, -43.0f}, {-16.0f, -43.0f},
        {16.0f, -43.0f}, {30.0f, -43.0f}, {44.0f, -43.0f}, {58.0f, -43.0f},
        {-58.0f, 47.0f}, {-42.0f, 47.0f}, {-26.0f, 47.0f}, {-16.0f, 47.0f},
        {16.0f, 47.0f}, {28.0f, 47.0f}, {42.0f, 47.0f}, {58.0f, 47.0f},
        {-48.0f, 31.0f}, {-33.0f, 31.0f}, {-18.0f, 31.0f},
        {18.0f, 31.0f}, {33.0f, 31.0f}, {48.0f, 31.0f}
    };
    for (int i = 0; i < 22; ++i) {
        drawTree(backTrees[i][0], backTrees[i][1], 0.82f);
    }

    float shrubLine[][2] = {
        {-43.0f, 24.5f}, {-29.0f, 24.5f}, {-16.0f, 24.5f},
        {16.0f, 24.5f}, {30.0f, 24.5f}, {44.0f, 24.5f}
    };
    for (int i = 0; i < 6; ++i) {
        drawShrub(shrubLine[i][0], shrubLine[i][1], 0.72f);
    }

    for (int i = 0; i < 5; ++i) {
        float z = -32.0f + i * 16.0f;
        if (z < -15.5f || z > 15.5f) {
            drawTree(-57.0f, z, 0.76f);
            drawTree(57.0f, z, 0.76f);
        }
    }

    drawLamp(-14.0f, -14.0f, 45.0f);
    drawLamp(14.0f, -14.0f, 135.0f);
    drawLamp(-14.0f, 14.0f, -45.0f);
    drawLamp(14.0f, 14.0f, -135.0f);
    drawLamp(-33.0f, 14.2f, -90.0f);
    drawLamp(33.0f, -14.2f, 90.0f);
    drawLamp(-46.0f, -14.2f, 90.0f);
    drawLamp(47.0f, 14.2f, -90.0f);

    drawBench(-24.0f, 14.4f, 0.0f);
    drawBench(23.0f, -14.4f, 180.0f);
    drawBench(-48.0f, 31.5f, 90.0f);

    drawShrub(-38.0f, 13.8f, 1.0f);
    drawShrub(33.0f, 14.4f, 0.9f);
    drawShrub(-24.0f, -15.0f, 0.85f);
    drawShrub(47.0f, -17.0f, 1.0f);
    drawShrub(16.0f, 31.0f, 0.95f);
    drawShrub(28.0f, 29.0f, 0.82f);
    drawShrub(-16.0f, 29.5f, 0.9f);
    drawShrub(-28.0f, 33.0f, 0.75f);

    drawSignalAwarePedestrians();
    drawPerson(-45.0f, -14.0f, 90.0f, 0.92f, 0.82f, 0.18f);
    drawBike(-18.5f, 14.6f, 20.0f, 0.95f, 0.40f, 0.20f);
    drawBike(29.0f, 13.8f, -20.0f, 0.25f, 0.62f, 0.88f);

    drawCloud(-28.0f, 23.0f, -46.0f, 1.6f);
    drawCloud(7.0f, 25.0f, -54.0f, 1.9f);
    drawCloud(34.0f, 22.0f, -44.0f, 1.3f);
    drawBirdsAndPlane();
    drawRoadPuddles();
    drawWeather();
    drawFireflies();
}
