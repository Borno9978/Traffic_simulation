#ifndef SCENE_H
#define SCENE_H

#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265358979323846f

extern float gSceneTime;
extern float gDeltaTime;
extern float gDayClock;
extern float gDayFactor;
extern float gSeasonBlend;
extern int gSeason;
extern int gTrafficPhase;
extern int gAutoTraffic;
extern int gAutoDayNight;
extern int gAutoSeason;
extern int gWeatherMode;

float clamp01(float v);
float mixf(float a, float b, float t);

void setColor(float r, float g, float b);
void drawBox(float w, float h, float d, float r, float g, float b);
void drawUnitCylinder(float radius, float height, int sides, float r, float g, float b);
void drawLowPolySphere(float radius, float r, float g, float b);

#endif
