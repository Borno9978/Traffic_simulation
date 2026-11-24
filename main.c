#include "scene.h"
#include "environment.h"
#include "road.h"
#include "traffic.h"
#include "vehicle.h"
#include <stdlib.h>

static int cameraMode = 1;
static int winW = 1280;
static int winH = 720;

float gSceneTime = 0.0f;
float gDeltaTime = 0.016f;
float gDayClock = 0.0f;
float gDayFactor = 1.0f;
float gSeasonBlend = 0.0f;
int gSeason = 0;
int gTrafficPhase = 0;
int gAutoTraffic = 1;
int gAutoDayNight = 1;
int gAutoSeason = 1;
int gWeatherMode = 0;

void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

float clamp01(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

float mixf(float a, float b, float t) {
    return a + (b - a) * t;
}

static float clampColor(float v) {
    return clamp01(v);
}

void drawBox(float w, float h, float d, float r, float g, float b) {
    float hw = w * 0.5f;
    float hh = h * 0.5f;
    float hd = d * 0.5f;

    glBegin(GL_QUADS);
    setColor(clampColor(r * 1.10f), clampColor(g * 1.10f), clampColor(b * 1.10f));
    glVertex3f(-hw, hh, -hd); glVertex3f(hw, hh, -hd); glVertex3f(hw, hh, hd); glVertex3f(-hw, hh, hd);

    setColor(r, g, b);
    glVertex3f(-hw, -hh, hd); glVertex3f(hw, -hh, hd); glVertex3f(hw, hh, hd); glVertex3f(-hw, hh, hd);

    setColor(clampColor(r * 0.90f), clampColor(g * 0.90f), clampColor(b * 0.90f));
    glVertex3f(hw, -hh, hd); glVertex3f(hw, -hh, -hd); glVertex3f(hw, hh, -hd); glVertex3f(hw, hh, hd);

    setColor(clampColor(r * 0.82f), clampColor(g * 0.82f), clampColor(b * 0.82f));
    glVertex3f(-hw, -hh, -hd); glVertex3f(-hw, -hh, hd); glVertex3f(-hw, hh, hd); glVertex3f(-hw, hh, -hd);

    setColor(clampColor(r * 0.95f), clampColor(g * 0.95f), clampColor(b * 0.95f));
    glVertex3f(hw, -hh, -hd); glVertex3f(-hw, -hh, -hd); glVertex3f(-hw, hh, -hd); glVertex3f(hw, hh, -hd);

    setColor(clampColor(r * 0.76f), clampColor(g * 0.76f), clampColor(b * 0.76f));
    glVertex3f(-hw, -hh, -hd); glVertex3f(hw, -hh, -hd); glVertex3f(hw, -hh, hd); glVertex3f(-hw, -hh, hd);
    glEnd();
}

void drawUnitCylinder(float radius, float height, int sides, float r, float g, float b) {
    int i;
    float half = height * 0.5f;

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= sides; ++i) {
        float a = 2.0f * PI * (float)i / (float)sides;
        float x = cosf(a) * radius;
        float z = sinf(a) * radius;
        float shade = 0.75f + 0.25f * (sinf(a) * 0.5f + 0.5f);
        setColor(clampColor(r * shade), clampColor(g * shade), clampColor(b * shade));
        glVertex3f(x, -half, z);
        glVertex3f(x, half, z);
    }
    glEnd();

    setColor(clampColor(r * 1.1f), clampColor(g * 1.1f), clampColor(b * 1.1f));
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, half, 0.0f);
    for (i = 0; i <= sides; ++i) {
        float a = 2.0f * PI * (float)i / (float)sides;
        glVertex3f(cosf(a) * radius, half, sinf(a) * radius);
    }
    glEnd();
}

void drawLowPolySphere(float radius, float r, float g, float b) {
    int stacks = 7;
    int slices = 12;
    int i, j;

    for (i = 0; i < stacks; ++i) {
        float lat0 = PI * (-0.5f + (float)i / (float)stacks);
        float lat1 = PI * (-0.5f + (float)(i + 1) / (float)stacks);
        float y0 = sinf(lat0) * radius;
        float y1 = sinf(lat1) * radius;
        float rr0 = cosf(lat0) * radius;
        float rr1 = cosf(lat1) * radius;

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= slices; ++j) {
            float lng = 2.0f * PI * (float)j / (float)slices;
            float shade = 0.82f + 0.22f * (cosf(lng - 0.8f) * 0.5f + 0.5f);
            setColor(clampColor(r * shade), clampColor(g * shade), clampColor(b * shade));
            glVertex3f(cosf(lng) * rr0, y0, sinf(lng) * rr0);
            glVertex3f(cosf(lng) * rr1, y1, sinf(lng) * rr1);
        }
        glEnd();
    }
}

static void setCamera(void) {
    if (cameraMode == 1) {
        gluLookAt(54.0, 30.0, 58.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
    } else if (cameraMode == 2) {
        gluLookAt(70.0, 18.0, 6.0, 0.0, 1.2, 0.0, 0.0, 1.0, 0.0);
    } else {
        gluLookAt(-6.5, 3.0, 21.0, -2.0, 1.2, 5.0, 0.0, 1.0, 0.0);
    }
}

static void drawScene(void) {
    drawSky();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setCamera();

    drawGround();
    drawRoads();
    drawEnvironment();
    drawTraffic();
    drawVehicles();
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glutSwapBuffers();
}

static void reshape(int w, int h) {
    if (h <= 0) h = 1;
    winW = w;
    winH = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(38.0, (double)w / (double)h, 0.1, 300.0);
    glMatrixMode(GL_MODELVIEW);
}

static void keyboard(unsigned char key, int x, int y) {
    (void)x;
    (void)y;

    if (key == '1') cameraMode = 1;
    if (key == '2') cameraMode = 2;
    if (key == '3') cameraMode = 3;
    if (key == 't' || key == 'T') gAutoTraffic = !gAutoTraffic;
    if (key == 'n' || key == 'N') gAutoDayNight = !gAutoDayNight;
    if (key == 'c' || key == 'C') gAutoSeason = !gAutoSeason;
    if (key == 'r' || key == 'R') gTrafficPhase = (gTrafficPhase + 1) % 4;
    if (key == 'y' || key == 'Y') {
        gDayClock += 6.0f;
        gAutoDayNight = 0;
    }
    if (key == 's' || key == 'S') {
        gSeason = (gSeason + 1) % 4;
        gSeasonBlend = 0.0f;
        gAutoSeason = 0;
    }
    if (key == 'w' || key == 'W') gWeatherMode = (gWeatherMode + 1) % 4;
    if (key == 27 || key == 'q' || key == 'Q') exit(0);

    glutPostRedisplay();
}

static void updateSimulation(int value) {
    float dayT;
    float trafficT;
    float seasonT;
    (void)value;

    gDeltaTime = 0.016f;
    gSceneTime += gDeltaTime;

    if (gAutoDayNight) {
        gDayClock += gDeltaTime;
    }

    dayT = fmodf(gDayClock / 55.0f, 1.0f);
    if (gAutoDayNight) {
        gDayFactor = 0.16f + 0.84f * clamp01(sinf(dayT * PI));
    } else {
        gDayFactor = 0.16f + 0.84f * clamp01(sinf(dayT * PI));
    }

    if (gAutoTraffic) {
        trafficT = fmodf(gSceneTime, 28.0f);
        if (trafficT < 11.0f) {
            gTrafficPhase = 0;       /* north-south green */
        } else if (trafficT < 14.0f) {
            gTrafficPhase = 1;       /* all-red/yellow safety gap */
        } else if (trafficT < 25.0f) {
            gTrafficPhase = 2;       /* east-west green */
        } else {
            gTrafficPhase = 3;       /* all-red/yellow safety gap */
        }
    }

    if (gAutoSeason) {
        seasonT = fmodf(gSceneTime / 120.0f, 4.0f);
        gSeason = (int)seasonT;
        gSeasonBlend = seasonT - (float)gSeason;
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateSimulation, 0);
}

static void initGL(void) {
    glClearColor(0.58f, 0.82f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Low-Poly 3D Traffic Intersection - 1/2/3 Camera Modes");

    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, updateSimulation, 0);
    glutMainLoop();
    return 0;
}
