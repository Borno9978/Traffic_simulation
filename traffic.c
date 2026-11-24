#include "scene.h"
#include "traffic.h"

static void drawSignalLens(float y, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(0.0f, y, 0.43f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, 0.22f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.01f);
    for (int i = 0; i <= 20; ++i) {
        float a = 2.0f * PI * (float)i / 20.0f;
        glVertex3f(cosf(a) * 0.31f, sinf(a) * 0.31f, 0.01f);
    }
    glEnd();
    glDisable(GL_BLEND);
    drawLowPolySphere(0.19f, r, g, b);
    glPopMatrix();
}

static void drawTrafficLight(float x, float z, float rot, int mode) {
    int redOn = mode == 0;
    int yellowOn = mode == 1;
    int greenOn = mode == 2;

    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 2.7f, 0.0f);
    drawUnitCylinder(0.12f, 5.4f, 10, 0.33f, 0.36f, 0.37f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 5.45f, 0.56f);
    drawBox(1.05f, 2.45f, 0.78f, 0.10f, 0.11f, 0.12f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.43f);
    drawBox(1.22f, 2.62f, 0.12f, 0.06f, 0.07f, 0.08f);
    glPopMatrix();
    drawSignalLens(0.62f, redOn ? 0.95f : 0.22f, redOn ? 0.08f : 0.04f, redOn ? 0.07f : 0.04f);
    drawSignalLens(0.0f, yellowOn ? 0.95f : 0.28f, yellowOn ? 0.78f : 0.22f, yellowOn ? 0.10f : 0.04f);
    drawSignalLens(-0.62f, greenOn ? 0.08f : 0.05f, greenOn ? 0.88f : 0.25f, greenOn ? 0.18f : 0.07f);

    glPushMatrix();
    glTranslatef(0.0f, 0.62f, 0.66f);
    drawBox(0.62f, 0.08f, 0.42f, 0.05f, 0.055f, 0.06f);
    glTranslatef(0.0f, -0.62f, 0.0f);
    drawBox(0.62f, 0.08f, 0.42f, 0.05f, 0.055f, 0.06f);
    glTranslatef(0.0f, -0.62f, 0.0f);
    drawBox(0.62f, 0.08f, 0.42f, 0.05f, 0.055f, 0.06f);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 5.55f, 1.15f);
    drawBox(2.5f, 0.16f, 0.16f, 0.33f, 0.36f, 0.37f);
    glTranslatef(1.0f, -0.1f, 0.0f);
    drawBox(0.72f, 0.50f, 0.34f, 0.10f, 0.11f, 0.12f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.42f, 2.0f, 0.48f);
    drawBox(0.64f, 0.72f, 0.24f, 0.08f, 0.09f, 0.10f);
    setColor(redOn ? 0.96f : 0.06f, redOn ? 0.10f : 0.80f, redOn ? 0.08f : 0.18f);
    glBegin(GL_QUADS);
    glVertex3f(-0.18f, -0.18f, 0.13f); glVertex3f(0.18f, -0.18f, 0.13f);
    glVertex3f(0.18f, 0.18f, 0.13f); glVertex3f(-0.18f, 0.18f, 0.13f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

static void drawStopLine(float x1, float z1, float x2, float z2) {
    setColor(0.98f, 0.98f, 0.94f);
    glBegin(GL_QUADS);
    glVertex3f(x1, 0.11f, z1);
    glVertex3f(x2, 0.11f, z1);
    glVertex3f(x2, 0.11f, z2);
    glVertex3f(x1, 0.11f, z2);
    glEnd();
}

void drawTraffic(void) {
    int nsMode;
    int ewMode;

    if (gTrafficPhase == 0) {
        nsMode = 2;
        ewMode = 0;
    } else if (gTrafficPhase == 1) {
        nsMode = 1;
        ewMode = 0;
    } else if (gTrafficPhase == 2) {
        nsMode = 0;
        ewMode = 2;
    } else {
        nsMode = 0;
        ewMode = 1;
    }

    drawTrafficLight(-13.3f, -13.3f, 45.0f, nsMode);
    drawTrafficLight(13.3f, 13.3f, 225.0f, nsMode);
    drawTrafficLight(13.3f, -13.3f, 135.0f, ewMode);
    drawTrafficLight(-13.3f, 13.3f, -45.0f, ewMode);

    drawStopLine(-9.5f, -15.0f, 9.5f, -14.5f);
    drawStopLine(-9.5f, 14.5f, 9.5f, 15.0f);
    drawStopLine(-15.0f, -9.5f, -14.5f, 9.5f);
    drawStopLine(14.5f, -9.5f, 15.0f, 9.5f);
}
