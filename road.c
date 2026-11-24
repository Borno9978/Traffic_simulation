#include "scene.h"
#include "road.h"

static void quad(float x1, float z1, float x2, float z2, float y) {
    glVertex3f(x1, y, z1);
    glVertex3f(x2, y, z1);
    glVertex3f(x2, y, z2);
    glVertex3f(x1, y, z2);
}

static void drawRect(float x1, float z1, float x2, float z2, float y, float r, float g, float b) {
    setColor(r, g, b);
    glBegin(GL_QUADS);
    quad(x1, z1, x2, z2, y);
    glEnd();
}

static void drawZebraNorthSouth(float z) {
    float x;
    setColor(0.98f, 0.98f, 0.94f);
    glBegin(GL_QUADS);
    for (x = -9.1f; x <= 8.1f; x += 2.05f) {
        quad(x, z - 1.55f, x + 0.85f, z + 1.55f, 0.092f);
    }
    glEnd();
}

static void drawZebraEastWest(float x) {
    float z;
    setColor(0.98f, 0.98f, 0.94f);
    glBegin(GL_QUADS);
    for (z = -9.1f; z <= 8.1f; z += 2.05f) {
        quad(x - 1.55f, z, x + 1.55f, z + 0.85f, 0.092f);
    }
    glEnd();
}

static void drawDashedLineZ(float x, float zStart, float zEnd) {
    float z;
    setColor(0.98f, 0.78f, 0.18f);
    glBegin(GL_QUADS);
    for (z = zStart; z < zEnd; z += 6.5f) {
        quad(x - 0.16f, z, x + 0.16f, z + 3.1f, 0.096f);
    }
    glEnd();
}

static void drawDashedLineX(float z, float xStart, float xEnd) {
    float x;
    setColor(0.98f, 0.78f, 0.18f);
    glBegin(GL_QUADS);
    for (x = xStart; x < xEnd; x += 6.5f) {
        quad(x, z - 0.16f, x + 3.1f, z + 0.16f, 0.096f);
    }
    glEnd();
}

static void drawWhiteDashedLineZ(float x, float zStart, float zEnd) {
    float z;
    setColor(0.88f, 0.89f, 0.86f);
    glBegin(GL_QUADS);
    for (z = zStart; z < zEnd; z += 7.0f) {
        quad(x - 0.12f, z, x + 0.12f, z + 3.2f, 0.098f);
    }
    glEnd();
}

static void drawWhiteDashedLineX(float z, float xStart, float xEnd) {
    float x;
    setColor(0.88f, 0.89f, 0.86f);
    glBegin(GL_QUADS);
    for (x = xStart; x < xEnd; x += 7.0f) {
        quad(x, z - 0.12f, x + 3.2f, z + 0.12f, 0.098f);
    }
    glEnd();
}

static void seasonGrass(float *r, float *g, float *b) {
    float colors[4][3] = {
        {0.56f, 0.78f, 0.50f},
        {0.48f, 0.74f, 0.42f},
        {0.68f, 0.62f, 0.34f},
        {0.82f, 0.86f, 0.82f}
    };
    int next = (gSeason + 1) % 4;
    *r = mixf(colors[gSeason][0], colors[next][0], gSeasonBlend);
    *g = mixf(colors[gSeason][1], colors[next][1], gSeasonBlend);
    *b = mixf(colors[gSeason][2], colors[next][2], gSeasonBlend);
    *r = mixf(0.04f, *r, gDayFactor);
    *g = mixf(0.08f, *g, gDayFactor);
    *b = mixf(0.12f, *b, gDayFactor);

    if (gWeatherMode == 1) {
        *r *= 0.78f;
        *g *= 0.82f;
        *b *= 0.82f;
    } else if (gWeatherMode == 2) {
        *r = mixf(*r, 0.72f, 0.35f);
        *g = mixf(*g, 0.76f, 0.35f);
        *b = mixf(*b, 0.74f, 0.35f);
    } else if (gWeatherMode == 3) {
        *r = mixf(*r, 0.86f, 0.72f);
        *g = mixf(*g, 0.88f, 0.72f);
        *b = mixf(*b, 0.86f, 0.72f);
    }
}

void drawGround(void) {
    float gr, gg, gb;
    seasonGrass(&gr, &gg, &gb);
    drawRect(-150.0f, -150.0f, 150.0f, 150.0f, 0.0f, gr, gg, gb);

    setColor(gr * 0.90f, gg * 0.92f, gb * 0.90f);
    glBegin(GL_QUADS);
    quad(-150.0f, -150.0f, 150.0f, -50.0f, 0.012f);
    quad(-150.0f, 50.0f, 150.0f, 150.0f, 0.012f);
    quad(-150.0f, -150.0f, -50.0f, 150.0f, 0.012f);
    quad(50.0f, -150.0f, 150.0f, 150.0f, 0.012f);
    glEnd();
}

void drawRoads(void) {
    const float road = 10.0f;
    const float walkOuter = 15.0f;
    float sr = 0.64f, sg = 0.47f, sb = 0.36f;
    float cr = 0.58f, cg = 0.40f, cb = 0.30f;

    if (gWeatherMode == 1) {
        drawRect(-road, -140.0f, road, 140.0f, 0.045f, 0.16f, 0.18f, 0.19f);
        drawRect(-140.0f, -road, 140.0f, road, 0.046f, 0.17f, 0.19f, 0.20f);
    } else {
        drawRect(-road, -140.0f, road, 140.0f, 0.045f, 0.24f, 0.25f, 0.26f);
        drawRect(-140.0f, -road, 140.0f, road, 0.046f, 0.25f, 0.26f, 0.27f);
    }

    drawRect(-road, -road, road, road, 0.052f, 0.27f, 0.28f, 0.29f);

    drawRect(-walkOuter, -140.0f, -road, -walkOuter, 0.07f, sr, sg, sb);
    drawRect(road, -140.0f, walkOuter, -walkOuter, 0.07f, sr, sg, sb);
    drawRect(-walkOuter, walkOuter, -road, 140.0f, 0.07f, sr, sg, sb);
    drawRect(road, walkOuter, walkOuter, 140.0f, 0.07f, sr, sg, sb);

    drawRect(-140.0f, -walkOuter, -walkOuter, -road, 0.071f, sr, sg, sb);
    drawRect(walkOuter, -walkOuter, 140.0f, -road, 0.071f, sr, sg, sb);
    drawRect(-140.0f, road, -walkOuter, walkOuter, 0.071f, sr, sg, sb);
    drawRect(walkOuter, road, 140.0f, walkOuter, 0.071f, sr, sg, sb);

    drawRect(-walkOuter, -walkOuter, -road, -road, 0.072f, sr * 1.05f, sg * 1.05f, sb * 1.05f);
    drawRect(road, -walkOuter, walkOuter, -road, 0.072f, sr * 1.05f, sg * 1.05f, sb * 1.05f);
    drawRect(-walkOuter, road, -road, walkOuter, 0.072f, sr * 1.05f, sg * 1.05f, sb * 1.05f);
    drawRect(road, road, walkOuter, walkOuter, 0.072f, sr * 1.05f, sg * 1.05f, sb * 1.05f);

    setColor(cr, cg, cb);
    glBegin(GL_LINES);
    for (float p = -136.0f; p <= 136.0f; p += 8.0f) {
        glVertex3f(-walkOuter, 0.083f, p); glVertex3f(-road, 0.083f, p);
        glVertex3f(road, 0.083f, p); glVertex3f(walkOuter, 0.083f, p);
        glVertex3f(p, 0.084f, -walkOuter); glVertex3f(p, 0.084f, -road);
        glVertex3f(p, 0.084f, road); glVertex3f(p, 0.084f, walkOuter);
    }
    glEnd();

    drawDashedLineZ(0.0f, -138.0f, -15.0f);
    drawDashedLineZ(0.0f, 15.0f, 138.0f);
    drawDashedLineX(0.0f, -138.0f, -15.0f);
    drawDashedLineX(0.0f, 15.0f, 138.0f);
    drawWhiteDashedLineZ(-5.0f, -138.0f, -15.0f);
    drawWhiteDashedLineZ(5.0f, 15.0f, 138.0f);
    drawWhiteDashedLineX(-5.0f, -138.0f, -15.0f);
    drawWhiteDashedLineX(5.0f, 15.0f, 138.0f);

    drawRect(-9.4f, -14.1f, 9.4f, -13.62f, 0.094f, 0.93f, 0.93f, 0.88f);
    drawRect(-9.4f, 13.62f, 9.4f, 14.1f, 0.094f, 0.93f, 0.93f, 0.88f);
    drawRect(-14.1f, -9.4f, -13.62f, 9.4f, 0.094f, 0.93f, 0.93f, 0.88f);
    drawRect(13.62f, -9.4f, 14.1f, 9.4f, 0.094f, 0.93f, 0.93f, 0.88f);

    drawZebraNorthSouth(-12.3f);
    drawZebraNorthSouth(12.3f);
    drawZebraEastWest(-12.3f);
    drawZebraEastWest(12.3f);

    setColor(0.16f, 0.17f, 0.18f);
    glBegin(GL_LINES);
    glVertex3f(-road, 0.105f, -140.0f); glVertex3f(-road, 0.105f, 140.0f);
    glVertex3f(road, 0.105f, -140.0f); glVertex3f(road, 0.105f, 140.0f);
    glVertex3f(-140.0f, 0.105f, -road); glVertex3f(140.0f, 0.105f, -road);
    glVertex3f(-140.0f, 0.105f, road); glVertex3f(140.0f, 0.105f, road);
    glEnd();
}
