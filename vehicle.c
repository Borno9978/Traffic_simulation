#include "scene.h"
#include "vehicle.h"

static void drawWheel(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawUnitCylinder(0.34f, 0.22f, 10, 0.05f, 0.055f, 0.06f);
    drawUnitCylinder(0.16f, 0.25f, 10, 0.78f, 0.80f, 0.82f);
    glPopMatrix();
}

static void drawVehicleShadow(float w, float d) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.02f, 0.03f, 0.03f, 0.22f);
    glBegin(GL_QUADS);
    glVertex3f(-w * 0.5f, 0.032f, -d * 0.5f);
    glVertex3f(w * 0.5f, 0.032f, -d * 0.5f);
    glVertex3f(w * 0.5f, 0.032f, d * 0.5f);
    glVertex3f(-w * 0.5f, 0.032f, d * 0.5f);
    glEnd();
    glDisable(GL_BLEND);
}

static void drawCar(float x, float z, float rot, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    drawVehicleShadow(4.9f, 2.55f);

    glPushMatrix();
    glTranslatef(0.0f, 0.66f, 0.0f);
    drawBox(4.2f, 0.9f, 2.05f, r, g, b);
    glTranslatef(0.0f, 0.47f, 0.0f);
    drawBox(4.35f, 0.12f, 2.12f, r * 0.78f, g * 0.78f, b * 0.78f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.35f, 1.36f, 0.0f);
    drawBox(2.0f, 0.78f, 1.72f, r * 1.05f, g * 1.05f, b * 1.05f);
    glPopMatrix();

    setColor(0.10f, 0.24f, 0.34f);
    glBegin(GL_QUADS);
    glVertex3f(0.72f, 1.08f, 0.885f); glVertex3f(1.22f, 1.56f, 0.885f);
    glVertex3f(0.25f, 1.74f, 0.885f); glVertex3f(-0.55f, 1.08f, 0.885f);
    glVertex3f(-1.26f, 1.08f, 0.885f); glVertex3f(-0.68f, 1.62f, 0.885f);
    glVertex3f(-1.35f, 1.62f, 0.885f); glVertex3f(-1.56f, 1.08f, 0.885f);
    glVertex3f(0.72f, 1.08f, -0.885f); glVertex3f(1.22f, 1.56f, -0.885f);
    glVertex3f(0.25f, 1.74f, -0.885f); glVertex3f(-0.55f, 1.08f, -0.885f);
    glVertex3f(-1.26f, 1.08f, -0.885f); glVertex3f(-0.68f, 1.62f, -0.885f);
    glVertex3f(-1.35f, 1.62f, -0.885f); glVertex3f(-1.56f, 1.08f, -0.885f);
    glVertex3f(1.04f, 1.05f, -0.62f); glVertex3f(1.04f, 1.05f, 0.62f);
    glVertex3f(1.04f, 1.48f, 0.62f); glVertex3f(1.04f, 1.48f, -0.62f);
    glVertex3f(-1.48f, 1.05f, -0.62f); glVertex3f(-1.48f, 1.05f, 0.62f);
    glVertex3f(-1.48f, 1.44f, 0.62f); glVertex3f(-1.48f, 1.44f, -0.62f);
    glEnd();

    drawWheel(1.35f, 0.35f, 1.08f);
    drawWheel(-1.35f, 0.35f, 1.08f);
    glPushMatrix();
    glScalef(1.0f, 1.0f, -1.0f);
    drawWheel(1.35f, 0.35f, 1.08f);
    drawWheel(-1.35f, 0.35f, 1.08f);
    glPopMatrix();

    setColor(1.0f, 0.92f, 0.55f);
    glBegin(GL_QUADS);
    glVertex3f(2.12f, 0.75f, -0.62f); glVertex3f(2.14f, 0.75f, -0.30f);
    glVertex3f(2.14f, 0.98f, -0.30f); glVertex3f(2.12f, 0.98f, -0.62f);
    glVertex3f(2.12f, 0.75f, 0.30f); glVertex3f(2.14f, 0.75f, 0.62f);
    glVertex3f(2.14f, 0.98f, 0.62f); glVertex3f(2.12f, 0.98f, 0.30f);
    glEnd();

    glPushMatrix();
    glTranslatef(2.24f, 0.52f, 0.0f);
    drawBox(0.18f, 0.25f, 1.55f, 0.08f, 0.09f, 0.10f);
    glTranslatef(-4.48f, 0.0f, 0.0f);
    drawBox(0.18f, 0.25f, 1.55f, 0.08f, 0.09f, 0.10f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.95f, 1.08f, 1.14f);
    drawBox(0.12f, 0.18f, 0.32f, 0.05f, 0.06f, 0.07f);
    glTranslatef(0.0f, 0.0f, -2.28f);
    drawBox(0.12f, 0.18f, 0.32f, 0.05f, 0.06f, 0.07f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.18f, 0.82f, 0.0f);
    drawBox(0.08f, 0.28f, 0.64f, 0.95f, 0.06f, 0.04f);
    glTranslatef(4.36f, 0.0f, 0.0f);
    drawBox(0.08f, 0.20f, 0.55f, 1.0f, 0.96f, 0.70f);
    glPopMatrix();

    glPopMatrix();
}

static void drawBus(float x, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    drawVehicleShadow(7.8f, 2.95f);

    glPushMatrix();
    glTranslatef(0.0f, 1.15f, 0.0f);
    drawBox(7.2f, 2.0f, 2.45f, 0.95f, 0.72f, 0.18f);
    glTranslatef(0.0f, 1.05f, 0.0f);
    drawBox(7.35f, 0.18f, 2.55f, 0.78f, 0.52f, 0.12f);
    glPopMatrix();

    setColor(0.11f, 0.25f, 0.34f);
    glBegin(GL_QUADS);
    for (float xw = -2.8f; xw <= 2.0f; xw += 1.2f) {
        glVertex3f(xw, 1.35f, 1.245f);
        glVertex3f(xw + 0.82f, 1.35f, 1.245f);
        glVertex3f(xw + 0.82f, 2.05f, 1.245f);
        glVertex3f(xw, 2.05f, 1.245f);
    }
    glVertex3f(3.62f, 1.18f, -0.75f); glVertex3f(3.62f, 1.18f, 0.75f);
    glVertex3f(3.62f, 2.0f, 0.75f); glVertex3f(3.62f, 2.0f, -0.75f);
    glEnd();

    glPushMatrix();
    glTranslatef(-3.0f, 0.92f, 1.27f);
    drawBox(0.65f, 1.35f, 0.08f, 0.12f, 0.18f, 0.20f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.65f, 0.46f, 1.28f);
    drawBox(0.82f, 0.82f, 0.35f, 0.05f, 0.055f, 0.06f);
    glTranslatef(-4.45f, 0.0f, 0.0f);
    drawBox(0.82f, 0.82f, 0.35f, 0.05f, 0.055f, 0.06f);
    glPopMatrix();

    glPushMatrix();
    glScalef(1.0f, 1.0f, -1.0f);
    glTranslatef(2.65f, 0.46f, 1.28f);
    drawBox(0.82f, 0.82f, 0.35f, 0.05f, 0.055f, 0.06f);
    glTranslatef(-4.45f, 0.0f, 0.0f);
    drawBox(0.82f, 0.82f, 0.35f, 0.05f, 0.055f, 0.06f);
    glPopMatrix();

    glPopMatrix();
}

static void drawCng(float x, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    drawVehicleShadow(3.0f, 1.9f);

    glPushMatrix();
    glTranslatef(0.0f, 0.65f, 0.0f);
    drawBox(2.4f, 0.85f, 1.35f, 0.12f, 0.62f, 0.36f);
    glTranslatef(-0.35f, 0.78f, 0.0f);
    drawBox(1.45f, 0.68f, 1.22f, 0.10f, 0.52f, 0.32f);
    glTranslatef(0.08f, 0.48f, 0.0f);
    drawBox(1.72f, 0.14f, 1.45f, 0.05f, 0.22f, 0.16f);
    glPopMatrix();

    setColor(0.10f, 0.24f, 0.34f);
    glBegin(GL_QUADS);
    glVertex3f(0.40f, 1.05f, -0.62f); glVertex3f(0.95f, 1.05f, -0.62f);
    glVertex3f(0.95f, 1.42f, -0.62f); glVertex3f(0.40f, 1.42f, -0.62f);
    glVertex3f(0.40f, 1.05f, 0.62f); glVertex3f(0.95f, 1.05f, 0.62f);
    glVertex3f(0.95f, 1.42f, 0.62f); glVertex3f(0.40f, 1.42f, 0.62f);
    glEnd();

    drawWheel(0.78f, 0.3f, 0.74f);
    drawWheel(-0.78f, 0.3f, 0.74f);
    glPushMatrix();
    glScalef(1.0f, 1.0f, -1.0f);
    drawWheel(0.78f, 0.3f, 0.74f);
    drawWheel(-0.78f, 0.3f, 0.74f);
    glPopMatrix();

    glPopMatrix();
}

static void drawAmbulance(float x, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    drawVehicleShadow(6.1f, 2.85f);

    glPushMatrix();
    glTranslatef(0.0f, 0.95f, 0.0f);
    drawBox(5.4f, 1.55f, 2.35f, 0.96f, 0.96f, 0.90f);
    glTranslatef(1.45f, 0.62f, 0.0f);
    drawBox(2.1f, 0.95f, 2.15f, 0.88f, 0.94f, 0.96f);
    glPopMatrix();

    setColor(0.82f, 0.05f, 0.05f);
    glBegin(GL_QUADS);
    glVertex3f(-0.35f, 1.28f, 1.205f); glVertex3f(0.35f, 1.28f, 1.205f);
    glVertex3f(0.35f, 1.88f, 1.205f); glVertex3f(-0.35f, 1.88f, 1.205f);
    glVertex3f(-0.75f, 1.47f, 1.208f); glVertex3f(0.75f, 1.47f, 1.208f);
    glVertex3f(0.75f, 1.70f, 1.208f); glVertex3f(-0.75f, 1.70f, 1.208f);
    glEnd();

    setColor(0.10f, 0.24f, 0.34f);
    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.42f, 1.205f); glVertex3f(2.08f, 1.42f, 1.205f);
    glVertex3f(2.08f, 2.0f, 1.205f); glVertex3f(1.0f, 2.0f, 1.205f);
    glVertex3f(2.55f, 1.28f, -0.75f); glVertex3f(2.55f, 1.28f, 0.75f);
    glVertex3f(2.55f, 1.78f, 0.75f); glVertex3f(2.55f, 1.78f, -0.75f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.0f, 2.05f, 0.0f);
    drawBox(1.35f, 0.20f, 0.42f, 0.85f, 0.06f, 0.05f);
    glTranslatef(0.38f, 0.12f, 0.0f);
    drawBox(0.36f, 0.12f, 0.46f, 0.08f, 0.36f, 0.92f);
    glPopMatrix();

    drawWheel(1.65f, 0.38f, 1.24f);
    drawWheel(-1.65f, 0.38f, 1.24f);
    glPushMatrix();
    glScalef(1.0f, 1.0f, -1.0f);
    drawWheel(1.65f, 0.38f, 1.24f);
    drawWheel(-1.65f, 0.38f, 1.24f);
    glPopMatrix();

    glPopMatrix();
}

static void drawPickup(float x, float z, float rot, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    drawVehicleShadow(5.4f, 2.55f);

    glPushMatrix();
    glTranslatef(0.0f, 0.68f, 0.0f);
    drawBox(4.8f, 0.9f, 2.05f, r, g, b);
    glTranslatef(-1.15f, 0.55f, 0.0f);
    drawBox(1.75f, 0.75f, 1.85f, r * 1.04f, g * 1.04f, b * 1.04f);
    glTranslatef(2.15f, -0.20f, 0.0f);
    drawBox(1.9f, 0.25f, 1.82f, r * 0.62f, g * 0.62f, b * 0.62f);
    glPopMatrix();

    setColor(0.10f, 0.24f, 0.34f);
    glBegin(GL_QUADS);
    glVertex3f(-1.95f, 1.05f, 0.95f); glVertex3f(-0.85f, 1.05f, 0.95f);
    glVertex3f(-0.85f, 1.55f, 0.95f); glVertex3f(-1.95f, 1.55f, 0.95f);
    glVertex3f(-2.22f, 1.0f, -0.62f); glVertex3f(-2.22f, 1.0f, 0.62f);
    glVertex3f(-2.22f, 1.44f, 0.62f); glVertex3f(-2.22f, 1.44f, -0.62f);
    glEnd();

    drawWheel(1.45f, 0.36f, 1.08f);
    drawWheel(-1.55f, 0.36f, 1.08f);
    glPushMatrix();
    glScalef(1.0f, 1.0f, -1.0f);
    drawWheel(1.45f, 0.36f, 1.08f);
    drawWheel(-1.55f, 0.36f, 1.08f);
    glPopMatrix();

    glPopMatrix();
}

static void drawRider(float x, float y, float z, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    drawBox(0.24f, 0.62f, 0.18f, r, g, b);
    glTranslatef(0.0f, 0.46f, 0.0f);
    drawLowPolySphere(0.15f, 0.88f, 0.66f, 0.46f);
    glTranslatef(0.0f, 0.08f, 0.0f);
    drawLowPolySphere(0.16f, 0.08f, 0.09f, 0.10f);
    glPopMatrix();
}

static void drawMotorbike(float x, float z, float rot, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    drawVehicleShadow(2.3f, 1.0f);

    glPushMatrix();
    glTranslatef(-0.75f, 0.38f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawUnitCylinder(0.34f, 0.12f, 12, 0.04f, 0.04f, 0.04f);
    glTranslatef(1.5f, 0.0f, 0.0f);
    drawUnitCylinder(0.34f, 0.12f, 12, 0.04f, 0.04f, 0.04f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.72f, 0.0f);
    drawBox(1.4f, 0.22f, 0.42f, r, g, b);
    glTranslatef(0.45f, 0.2f, 0.0f);
    drawBox(0.52f, 0.32f, 0.36f, r * 1.1f, g * 1.1f, b * 1.1f);
    glTranslatef(-0.75f, 0.03f, 0.0f);
    drawBox(0.5f, 0.14f, 0.45f, 0.06f, 0.06f, 0.06f);
    glPopMatrix();

    drawRider(-0.15f, 1.0f, 0.0f, 0.18f, 0.38f, 0.90f);
    glPopMatrix();
}

static void drawBicycle(float x, float z, float rot, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    drawVehicleShadow(2.1f, 0.8f);

    glPushMatrix();
    glTranslatef(-0.72f, 0.40f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawUnitCylinder(0.36f, 0.06f, 12, 0.04f, 0.04f, 0.04f);
    glTranslatef(1.44f, 0.0f, 0.0f);
    drawUnitCylinder(0.36f, 0.06f, 12, 0.04f, 0.04f, 0.04f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.72f, 0.0f);
    drawBox(1.15f, 0.07f, 0.07f, r, g, b);
    glRotatef(24.0f, 0.0f, 0.0f, 1.0f);
    drawBox(0.82f, 0.06f, 0.06f, r, g, b);
    glRotatef(-48.0f, 0.0f, 0.0f, 1.0f);
    drawBox(0.82f, 0.06f, 0.06f, r, g, b);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.68f, 1.03f, 0.0f);
    drawBox(0.36f, 0.06f, 0.56f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();

    drawRider(-0.05f, 1.05f, 0.0f, 0.95f, 0.40f, 0.18f);
    glPopMatrix();
}

/* ---------- Collision-aware vehicle simulation ----------
 * Each vehicle knows its lane (0..3), direction, length, and cruise speed.
 * Each frame, every vehicle scans the same lane for the nearest vehicle
 * ahead and matches its speed to maintain a safe gap. The result is true
 * car-following behavior: queues form behind slow vehicles and at red
 * lights, instead of vehicles ghosting through one another. */

typedef struct {
    float startPos;
    float cruiseSpeed;
    float length;
    int dir;
    int lane;
} VehicleDef;

static VehicleDef gDefs[13] = {
    { -82.0f, 7.4f, 4.9f,  1, 0 },
    {-100.0f, 7.4f, 4.9f,  1, 0 },
    { -54.0f, 8.6f, 6.1f,  1, 0 },
    { -24.0f, 6.6f, 3.0f,  1, 0 },
    {  80.0f, 7.0f, 4.9f, -1, 1 },
    {  98.0f, 7.1f, 5.4f, -1, 1 },
    {  58.0f, 6.4f, 3.0f, -1, 1 },
    { -72.0f, 7.8f, 4.9f,  1, 2 },
    { -90.0f, 6.2f, 7.8f,  1, 2 },
    { -42.0f, 5.6f, 2.1f,  1, 2 },
    {  82.0f, 8.0f, 4.9f, -1, 3 },
    { 100.0f, 7.7f, 4.9f, -1, 3 },
    {  54.0f, 9.4f, 2.3f, -1, 3 }
};

static float gPos[13];
static float gSpeed[13];
static int   gBraking[13];
static int   gSimInit = 0;

static void initVehicleSim(void) {
    int i;
    for (i = 0; i < 13; ++i) {
        gPos[i] = gDefs[i].startPos;
        gSpeed[i] = gDefs[i].cruiseSpeed;
        gBraking[i] = 0;
    }
    gSimInit = 1;
}

static void updateVehicleSim(void) {
    int i, j;
    if (!gSimInit) initVehicleSim();

    for (i = 0; i < 13; ++i) {
        VehicleDef *v = &gDefs[i];
        int ns = (v->lane == 0 || v->lane == 1);
        int go = ns ? (gTrafficPhase == 0) : (gTrafficPhase == 2);
        float stopLine = (v->dir > 0) ? -17.5f : 17.5f;
        float distToStop = (float)v->dir * (stopLine - gPos[i]);
        float minDist = 1.0e9f;
        float aheadLen = 0.0f;
        float cruise = v->cruiseSpeed;
        float safeGap;
        float slowRange = 9.0f;
        float target = cruise;
        float accel;
        float ease;

        for (j = 0; j < 13; ++j) {
            float d;
            if (j == i || gDefs[j].lane != v->lane) continue;
            d = (gPos[j] - gPos[i]) * (float)v->dir;
            /* Wrap so vehicles just past +/-132 are not seen as 264m ahead */
            if (d < -132.0f) d += 264.0f;
            if (d >  132.0f) d -= 264.0f;
            if (d > 0.0f && d < minDist) {
                minDist = d;
                aheadLen = gDefs[j].length;
            }
        }

        safeGap = v->length * 0.5f + aheadLen * 0.5f + 2.0f;
        if (minDist < safeGap) {
            target = 0.0f;
        } else if (minDist < safeGap + slowRange) {
            target = cruise * (minDist - safeGap) / slowRange;
        }

        if (!go && distToStop > 0.0f && distToStop < 14.0f) {
            float redTarget = cruise * (distToStop / 14.0f) * 0.45f;
            if (distToStop < 1.2f) redTarget = 0.0f;
            if (redTarget < target) target = redTarget;
        }

        gBraking[i] = (target < gSpeed[i] - 0.6f) ? 1 : 0;
        accel = (target > gSpeed[i]) ? 2.4f : 5.2f;
        ease = clamp01(accel * gDeltaTime);
        gSpeed[i] += (target - gSpeed[i]) * ease;
        if (gSpeed[i] < 0.0f) gSpeed[i] = 0.0f;

        gPos[i] += (float)v->dir * gSpeed[i] * gDeltaTime;
        if (gPos[i] >  132.0f) gPos[i] -= 264.0f;
        if (gPos[i] < -132.0f) gPos[i] += 264.0f;
    }
}

/* ---------- Headlights / taillights / brake glow / exhaust ----------
 * All effects use additive blending and live in the vehicle's local frame
 * (front = local +X). They turn on at night or when braking. */

static void drawVehicleLights(float x, float z, float rot, float halfLen, int braking, float night) {
    int wantHead = (night > 0.18f);
    int wantBrake = braking;
    int wantTail = (night > 0.20f) || braking;
    int i;

    if (!wantHead && !wantBrake && !wantTail) return;

    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    if (wantHead) {
        float a = 0.30f * night;
        glColor4f(1.0f, 0.96f, 0.74f, a);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(halfLen + 0.05f, 0.06f, 0.0f);
        for (i = 0; i <= 22; ++i) {
            float ang = -1.05f + 2.10f * (float)i / 22.0f;
            glVertex3f(halfLen + 6.5f, 0.06f, sinf(ang) * 2.6f);
        }
        glEnd();

        glColor4f(1.0f, 0.98f, 0.78f, 0.85f);
        glBegin(GL_QUADS);
        glVertex3f(halfLen + 0.01f, 0.55f, -0.62f);
        glVertex3f(halfLen + 0.10f, 0.55f, -0.62f);
        glVertex3f(halfLen + 0.10f, 0.78f, -0.62f);
        glVertex3f(halfLen + 0.01f, 0.78f, -0.62f);
        glVertex3f(halfLen + 0.01f, 0.55f, 0.62f);
        glVertex3f(halfLen + 0.10f, 0.55f, 0.62f);
        glVertex3f(halfLen + 0.10f, 0.78f, 0.62f);
        glVertex3f(halfLen + 0.01f, 0.78f, 0.62f);
        glEnd();
    }

    if (wantTail) {
        float r = wantBrake ? 1.0f : 0.78f;
        float a = wantBrake ? 0.95f : 0.50f * night;
        glColor4f(r, 0.10f, 0.06f, a);
        glBegin(GL_QUADS);
        glVertex3f(-halfLen - 0.01f, 0.55f, -0.62f);
        glVertex3f(-halfLen - 0.10f, 0.55f, -0.62f);
        glVertex3f(-halfLen - 0.10f, 0.74f, -0.62f);
        glVertex3f(-halfLen - 0.01f, 0.74f, -0.62f);
        glVertex3f(-halfLen - 0.01f, 0.55f, 0.62f);
        glVertex3f(-halfLen - 0.10f, 0.55f, 0.62f);
        glVertex3f(-halfLen - 0.10f, 0.74f, 0.62f);
        glVertex3f(-halfLen - 0.01f, 0.74f, 0.62f);
        glEnd();

        if (wantBrake) {
            glColor4f(1.0f, 0.14f, 0.10f, 0.32f);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(-halfLen - 0.35f, 0.62f, 0.0f);
            for (i = 0; i <= 18; ++i) {
                float a2 = 2.0f * PI * (float)i / 18.0f;
                glVertex3f(-halfLen - 0.35f + cosf(a2) * 0.95f, 0.62f + sinf(a2) * 0.55f, 0.0f);
            }
            glEnd();
        }
    }

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glPopMatrix();
}

static void drawExhaustPuff(float x, float z, float rot, float halfLen, float speed, int seed) {
    int i;

    if (speed < 0.4f) return;

    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rot, 0.0f, 1.0f, 0.0f);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (i = 0; i < 3; ++i) {
        float phase = fmodf(gSceneTime * 1.6f + (float)(i * 17 + seed * 13) * 0.31f, 1.0f);
        float ax = -halfLen - 0.25f - phase * 1.6f;
        float ay = 0.55f + phase * 0.45f;
        float az = 0.18f * sinf(phase * 6.0f + (float)i);
        float r = 0.10f + phase * 0.30f;
        float alpha = (1.0f - phase) * 0.18f;
        glColor4f(0.78f, 0.78f, 0.80f, alpha);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(ax, ay, az);
        for (int k = 0; k <= 10; ++k) {
            float a = 2.0f * PI * (float)k / 10.0f;
            glVertex3f(ax + cosf(a) * r, ay + sinf(a) * r, az);
        }
        glEnd();
    }

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glPopMatrix();
}

void drawVehicles(void) {
    /* Day/night factor: gDayFactor is 0.16 (deepest night) to 1.0 (full day).
     * Headlights ramp up below ~0.6 and peak below ~0.4. */
    float night = clamp01((0.65f - gDayFactor) / 0.45f);
    int i;
    float halfLens[13] = {
        2.45f, 2.45f, 3.05f, 1.50f,
        2.45f, 2.70f, 1.50f,
        2.45f, 3.90f, 1.05f,
        2.45f, 2.45f, 1.15f
    };
    float xOff[13] = {
        -4.8f, -4.8f, -4.8f, -4.8f,
         4.8f,  4.8f,  4.8f,
        -4.8f, -4.8f, -4.8f,
         4.8f,  4.8f,  4.8f
    };
    int   ns[13] = { 1,1,1,1,  1,1,1,  0,0,0,  0,0,0 };
    float rots[13] = {
        -90.0f, -90.0f, -90.0f, -90.0f,
         90.0f,  90.0f,  90.0f,
          0.0f,   0.0f,   0.0f,
        180.0f, 180.0f, 180.0f
    };

    updateVehicleSim();

    /* Lane 0 (north-bound, x = -4.8) */
    drawCar(xOff[0], gPos[0], rots[0], 0.88f, 0.18f, 0.16f);
    drawCar(xOff[1], gPos[1], rots[1], 0.16f, 0.44f, 0.86f);
    drawAmbulance(xOff[2], gPos[2], rots[2]);
    drawCng(xOff[3], gPos[3], rots[3]);

    /* Lane 1 (south-bound, x = +4.8) */
    drawCar(xOff[4], gPos[4], rots[4], 0.95f, 0.95f, 0.90f);
    drawPickup(xOff[5], gPos[5], rots[5], 0.55f, 0.70f, 0.78f);
    drawCng(xOff[6], gPos[6], rots[6]);

    /* Lane 2 (east-bound, z = -4.8) */
    drawCar(gPos[7], xOff[7], rots[7], 0.92f, 0.90f, 0.28f);
    drawBus(gPos[8], xOff[8], rots[8]);
    drawBicycle(gPos[9], xOff[9], rots[9], 0.12f, 0.32f, 0.78f);

    /* Lane 3 (west-bound, z = +4.8) */
    drawCar(gPos[10], xOff[10], rots[10], 0.20f, 0.70f, 0.34f);
    drawCar(gPos[11], xOff[11], rots[11], 0.95f, 0.28f, 0.20f);
    drawMotorbike(gPos[12], xOff[12], rots[12], 0.90f, 0.18f, 0.12f);

    /* Lights and exhaust */
    for (i = 0; i < 13; ++i) {
        float vx = ns[i] ? xOff[i] : gPos[i];
        float vz = ns[i] ? gPos[i] : xOff[i];
        /* Skip lights/exhaust for bicycle (id 9) */
        if (i == 9) continue;
        drawVehicleLights(vx, vz, rots[i], halfLens[i], gBraking[i], night);
        /* Exhaust only for motorized full-size vehicles */
        if (i != 12) {
            drawExhaustPuff(vx, vz, rots[i], halfLens[i], gSpeed[i], i);
        }
    }
}
