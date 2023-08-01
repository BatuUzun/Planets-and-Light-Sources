/*********
   CTIS164 - Template Source Program
----------
STUDENT : Batu Uzun
SECTION : 001
HOMEWORK: 3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
F1, F2 and F3 work seperately in every planet(pressing F1 changes only chosen planet's light source).
F7 starts or stops only chosen planet's animation.
Right and Left arrow keys change chosen planet.
F6 restarts only chosen planet.
Planet's light sources status is shown for each planet.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "vec.h"
#define WINDOW_WIDTH  1300
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  60 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define HOWMANYPLANETS 4
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int whichplanet = 1, activeTimer[HOWMANYPLANETS] = { 1,1,1,1 }, random = 0, redstatus[4] = { 1,1,1,1 }, duration = 0, checktimer = 1;
int greenstatus[4] = { 1,1,1,1 }, bluestatus[4] = { 1,1,1,1 };
/*whichplanet: shows chosen planet.
redstatus, greenstatus and bluestatus: light sources status are saved in these arrays
checktimer: checks timer if it is active.
*/
typedef struct {
    int r, x, y;
}planet_t;
planet_t planet[HOWMANYPLANETS];//planet's locations
//**************************************************************************************************
typedef struct {
    float r, g, b;
} color_t;

typedef struct {
    vec_t pos;
    vec_t N;
} vertex_t;

typedef struct {
    vec_t   pos;
    color_t color;
    int speed;
    int location;
    int radius;
    float angle;
} light_t;
//light's information for first planet.
light_t lightsources1[3] =
{
   { {    -450 + 45 , 180}, { 255, 0, 0 }, {0},{0},{10},{0} },
   { { -450 + 80, 180 }, { 0, 255, 0 }, {0},{0},{10},{0} },
   { { -450 + 115 , 180}, { 0, 0, 255 }, {0},{0},{10} ,{0}}
};
//light's information for second planet.

light_t lightsources2[3] =
{
   { {    -450 + 45 , -150}, { 255, 0, 0 }, {0},{0},{10},{0}},
   { { -450 + 80, -150 }, { 0, 255, 0 } , {0},{0},{10},{0}},
   { { -450 + 115 , -150}, { 0, 0, 255 } , {0},{0},{10},{0}}
};
//light's information for third planet.

light_t lightsources3[3] =
{
   { { 450 + 45 , 180}, { 255, 0, 0 }, {0},{0} ,{10},{0}},
   { { 450 + 80, 180 }, { 0, 255, 0 } , {0},{0},{10},{0}},
   { { 450 + 115 , 180}, { 0, 0, 255 },  {0},{0},{10} ,{0}}
};
//light's information for fourth planet.

light_t lightsources4[3] =
{
   { { 450 + 45 , -150}, { 255, 0, 0 },  {0},{0},{10} ,{0}},
   { { 450 + 80, -150 }, { 0, 255, 0 } , {0},{0} ,{10},{0}},
   { { 450 + 115 , -150}, { 0, 0, 255 } , {0},{0} ,{10},{0}}
};




//****************************************************************************************************
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

//calculate color
color_t mulColor(float k, color_t c) {
    color_t tmp = { k * c.r, k * c.g, k * c.b };
    return tmp;
}

color_t addColor(color_t c1, color_t c2) {
    color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
    return tmp;
}
//calculate distance impact of light sources
double distanceImpact(double d) {
    return (-1.0 / 200.0) * d + 1.0;
}

color_t calculateColor(light_t source, vertex_t v) {
    vec_t L = subV(source.pos, v.pos);
    vec_t uL = unitV(L);
    float factor = dotP(uL, v.N) * distanceImpact(magV(L));
    return mulColor(factor, source.color);
}
//display colors for first planet
void displaycolors1() {
    for (int degree = 0; degree < 360; degree++)
    {


        vec_t surface = { (cos(degree) * planet[0].r), sin(degree) * planet[0].r };
        vertex_t point = { { planet[0].x + cos(degree) * planet[0].r, planet[0].y + sin(degree) * planet[0].r  }, {unitV(surface).x,unitV(surface).y } };

        color_t res = { 0, 0, 0 };

        if (redstatus[0] == 1)
        {
            res = addColor(res, calculateColor(lightsources1[0], point));
            glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(planet[0].x + cos(degree) * planet[0].r, planet[0].y + sin(degree) * planet[0].r);
            glColor3f(res.r * planet[0].r, res.g * planet[0].r, res.b * planet[0].r);
            glVertex2f(planet[0].x, planet[0].y);
            glEnd();
        }
        if (greenstatus[0] == 1)
        {
            res = addColor(res, calculateColor(lightsources1[1], point));
            glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(planet[0].x + cos(degree) * planet[0].r, planet[0].y + sin(degree) * planet[0].r);
            glColor3f(res.r * planet[0].r, res.g * planet[0].r, res.b * planet[0].r);
            glVertex2f(planet[0].x, planet[0].y);
            glEnd();
        }
        if (bluestatus[0] == 1)
        {
            res = addColor(res, calculateColor(lightsources1[2], point));
            glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(planet[0].x + cos(degree) * planet[0].r, planet[0].y + sin(degree) * planet[0].r);
            glColor3f(res.r * planet[0].r, res.g * planet[0].r, res.b * planet[0].r);
            glVertex2f(planet[0].x, planet[0].y);
            glEnd();
        }

    }
}
//display colors for second planet

void displaycolors2() {
    for (int degree = 0; degree < 360; degree++) {
        vec_t surface = { (cos(degree) * planet[1].r),sin(degree) * planet[1].r };
        vertex_t point = { {planet[1].x + cos(degree) * planet[1].r,sin(degree) * planet[1].r + planet[1].y},{unitV(surface).x,unitV(surface).y} };

        color_t color = { 0, 0, 0 };
        if (redstatus[1] == 1)
        {
            color = addColor(color, calculateColor(lightsources2[0], point));
            glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(planet[1].x + cos(degree) * 30, planet[1].y + sin(degree) * 30);
            glColor3f(color.r * planet[1].r, color.g * planet[1].r, color.b * planet[1].r);
            glVertex2f(planet[1].x, planet[1].y);
            glEnd();
        }
        if (greenstatus[1] == 1) {
            color = addColor(color, calculateColor(lightsources2[1], point));
            glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(planet[1].x + cos(degree) * planet[1].r, planet[1].y + sin(degree) * planet[1].r);
            glColor3f(color.r * planet[1].r, color.g * planet[1].r, color.b * planet[1].r);
            glVertex2f(planet[1].x, planet[1].y);
            glEnd();
        }
        if (bluestatus[1] == 1)
        {
            color = addColor(color, calculateColor(lightsources2[2], point));

            glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(planet[1].x + cos(degree) * planet[1].r, planet[1].y + sin(degree) * planet[1].r);
            glColor3f(color.r * planet[1].r, color.g * planet[1].r, color.b * planet[1].r);
            glVertex2f(planet[1].x, planet[1].y);
            glEnd();

        }

    }
}
//display colors for third planet

void  displaycolors3() {
    for (int degree = 0; degree < 360; degree++) {
        vec_t surface = { (cos(degree) * planet[2].r), sin(degree) * planet[2].r };
        vertex_t point = { { planet[2].x + cos(degree) * planet[2].r, planet[2].y + sin(degree) * planet[2].r  }, {unitV(surface).x,unitV(surface).y } };

        color_t res = { 0, 0, 0 };

        if (redstatus[2] == 1) {
            res = addColor(res, calculateColor(lightsources3[0], point));
            glBegin(GL_LINES);

            glColor3f(res.r * planet[2].r, res.g * planet[2].r, res.b * planet[2].r);
            glVertex2f(planet[2].x, planet[2].y);

            glColor3f(0, 0, 0);
            glVertex2f(planet[2].x + cos(degree) * planet[2].r, planet[2].y + sin(degree) * planet[2].r);
            glEnd();
        }
        if (greenstatus[2] == 1) {
            res = addColor(res, calculateColor(lightsources3[1], point));
            glBegin(GL_LINES);

            glColor3f(res.r * planet[2].r, res.g * planet[2].r, res.b * planet[2].r);
            glVertex2f(planet[2].x, planet[2].y);

            glColor3f(0, 0, 0);
            glVertex2f(planet[2].x + cos(degree) * planet[2].r, planet[2].y + sin(degree) * planet[2].r);
            glEnd();
        }
        if (bluestatus[2] == 1) {
            res = addColor(res, calculateColor(lightsources3[2], point));
            glBegin(GL_LINES);

            glColor3f(res.r * planet[2].r, res.g * planet[2].r, res.b * planet[2].r);
            glVertex2f(planet[2].x, planet[2].y);

            glColor3f(0, 0, 0);
            glVertex2f(planet[2].x + cos(degree) * planet[2].r, planet[2].y + sin(degree) * planet[2].r);
            glEnd();
        }





    }

}
//display colors for fourth planet

void  displaycolors4() {
    for (int degree = 0; degree < 360; degree++) {
        vec_t surface = { (cos(degree) * planet[3].r), sin(degree) * planet[3].r };

        vertex_t point = { { planet[3].x + cos(degree) * planet[3].r, planet[3].y + sin(degree) * planet[3].r  }, {unitV(surface).x,unitV(surface).y } };

        color_t res = { 0, 0, 0 };

        if (redstatus[3] == 1) {
            res = addColor(res, calculateColor(lightsources4[0], point));
            glBegin(GL_LINES);

            glColor3f(res.r * planet[3].r, res.g * planet[3].r, res.b * planet[3].r);
            glVertex2f(planet[3].x, planet[3].y);

            glColor3f(0, 0, 0);
            glVertex2f(planet[3].x + cos(degree) * planet[3].r, planet[3].y + sin(degree) * planet[3].r);
            glEnd();
        }
        if (greenstatus[3] == 1)
        {
            res = addColor(res, calculateColor(lightsources4[1], point));
            glBegin(GL_LINES);

            glColor3f(res.r * planet[3].r, res.g * planet[3].r, res.b * planet[3].r);
            glVertex2f(planet[3].x, planet[3].y);

            glColor3f(0, 0, 0);
            glVertex2f(planet[3].x + cos(degree) * planet[3].r, planet[3].y + sin(degree) * planet[3].r);
            glEnd();
        }
        if (bluestatus[3] == 1)
        {
            res = addColor(res, calculateColor(lightsources4[2], point));
            glBegin(GL_LINES);

            glColor3f(res.r * planet[3].r, res.g * planet[3].r, res.b * planet[3].r);
            glVertex2f(planet[3].x, planet[3].y);

            glColor3f(0, 0, 0);
            glVertex2f(planet[3].x + cos(degree) * planet[3].r, planet[3].y + sin(degree) * planet[3].r);
            glEnd();
        }

    }

}
void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}



//my name, surname and homework information
void label() {
    glColor3ub(255, 255, 255);
    vprint(-55, 330, GLUT_BITMAP_8_BY_13, "Batu Uzun");
    vprint(55, 330, GLUT_BITMAP_8_BY_13, "Homework #3");
}
//inform the user
void informativeMessages() {
    glColor3ub(0, 255, 255);

    vprint(-100, 0, GLUT_BITMAP_8_BY_13, "%d. planet is chosen(Right/Left arrow keys)", whichplanet);

    glColor3ub(255, 255, 255);
    vprint(-20, -50, GLUT_BITMAP_8_BY_13, "F5(Restart)");
    vprint(-20, -75, GLUT_BITMAP_8_BY_13, "F6(Restart only chosen planet)");
    if (checktimer % 2 == 0)
        vprint(-20, -25, GLUT_BITMAP_8_BY_13, "F4(Start animation)");
    else
        vprint(-20, -25, GLUT_BITMAP_8_BY_13, "F4(Stop animation)");


    if (redstatus[0] == 1)
        vprint(-280, 270, GLUT_BITMAP_8_BY_13, "F1(red): ON");
    else
        vprint(-280, 270, GLUT_BITMAP_8_BY_13, "F1(red): OFF");
    if (bluestatus[0] == 1)
        vprint(-280, 250, GLUT_BITMAP_8_BY_13, "F2(blue): ON");
    else
        vprint(-280, 250, GLUT_BITMAP_8_BY_13, "F2(blue): OFF");
    if (greenstatus[0] == 1)
        vprint(-280, 230, GLUT_BITMAP_8_BY_13, "F3(green): ON");
    else
        vprint(-280, 230, GLUT_BITMAP_8_BY_13, "F3(green): OFF");
    if (activeTimer[0] == 1)
        vprint(-280, 210, GLUT_BITMAP_8_BY_13, "F7(Animation): ON");
    else
        vprint(-280, 210, GLUT_BITMAP_8_BY_13, "F7(Animation): OFF");


    if (redstatus[1] == 1)
        vprint(-280, -230, GLUT_BITMAP_8_BY_13, "F1(red): ON");
    else
        vprint(-280, -230, GLUT_BITMAP_8_BY_13, "F1(red): OFF");
    if (bluestatus[1] == 1)
        vprint(-280, -250, GLUT_BITMAP_8_BY_13, "F2(blue): ON");
    else
        vprint(-280, -250, GLUT_BITMAP_8_BY_13, "F2(blue): OFF");
    if (greenstatus[1] == 1)
        vprint(-280, -270, GLUT_BITMAP_8_BY_13, "F3(green): ON");
    else
        vprint(-280, -270, GLUT_BITMAP_8_BY_13, "F3(green): OFF");
    if (activeTimer[1] == 1)
        vprint(-280, -290, GLUT_BITMAP_8_BY_13, "F7(Animation): ON");
    else
        vprint(-280, -290, GLUT_BITMAP_8_BY_13, "F7(Animation): OFF");



    if (redstatus[2] == 1)
        vprint(140, 270, GLUT_BITMAP_8_BY_13, "F1(red): ON");
    else
        vprint(140, 270, GLUT_BITMAP_8_BY_13, "F1(red): OFF");
    if (bluestatus[2] == 1)
        vprint(140, 250, GLUT_BITMAP_8_BY_13, "F2(blue): ON");
    else
        vprint(140, 250, GLUT_BITMAP_8_BY_13, "F2(blue): OFF");
    if (greenstatus[2] == 1)
        vprint(140, 230, GLUT_BITMAP_8_BY_13, "F3(green): ON");
    else
        vprint(140, 230, GLUT_BITMAP_8_BY_13, "F3(green): OFF");
    if (activeTimer[2] == 1)
        vprint(140, 210, GLUT_BITMAP_8_BY_13, "F7(Animation): ON");
    else
        vprint(140, 210, GLUT_BITMAP_8_BY_13, "F7(Animation): OFF");



    if (redstatus[3] == 1)
        vprint(140, -230, GLUT_BITMAP_8_BY_13, "F1(red): ON");
    else
        vprint(140, -230, GLUT_BITMAP_8_BY_13, "F1(red): OFF");
    if (bluestatus[3] == 1)
        vprint(140, -250, GLUT_BITMAP_8_BY_13, "F2(blue): ON");
    else
        vprint(140, -250, GLUT_BITMAP_8_BY_13, "F2(blue): OFF");

    if (greenstatus[3] == 1)
        vprint(140, -270, GLUT_BITMAP_8_BY_13, "F3(green): ON");
    else
        vprint(140, -270, GLUT_BITMAP_8_BY_13, "F3(green): OFF");
    if (activeTimer[3] == 1)
        vprint(140, -290, GLUT_BITMAP_8_BY_13, "F7(Animation): ON");
    else
        vprint(140, -290, GLUT_BITMAP_8_BY_13, "F7(Animation): OFF");

    glColor3ub(255, 255, 0);
    for (int p = 0; p < 4; p++)
        vprint(planet[p].x - 190, planet[p].y + 140, GLUT_BITMAP_8_BY_13, "planet %d", p + 1);
}
//draw black planets
void blackplanet() {

    glColor3ub(0, 0, 0);
    for (int p = 0; p < 4; p++)
        planet[p].r = 30;

    planet[0].x = planet[1].x = -450;
    planet[2].x = planet[3].x = 450;
    planet[0].y = planet[2].y = 180;
    planet[1].y = planet[3].y = -150;
    //planet1
    for (int p = 0; p < 4; p++)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(planet[p].x, planet[p].y); // center of circle
        for (int i = 0; i <= 20; i++) {
            glVertex2f(planet[p].x + (planet[p].r * cos(i * 2 * PI / 20)), planet[p].y + (planet[p].r * sin(i * 2 * PI / 20)));
        }
        glEnd();
    }
}
//draw orbits
void orbits() {
    //orbits for planet 1
    for (int p = 0; p < 4; p++)
    {
        circle_wire(planet[p].x, planet[p].y, planet[p].r + 45);
        circle_wire(planet[p].x, planet[p].y, planet[p].r + 80);
        circle_wire(planet[p].x, planet[p].y, planet[p].r + 115);
    }
}
//draw light sources
void lights() {
    //light source 1
    glColor3ub(lightsources1[0].color.r, lightsources1[0].color.g, lightsources1[0].color.b);
    if (redstatus[0] == 1) {

        circle((planet[0].r + 45) * cos(lightsources1[0].angle * D2R) + planet[0].x, (planet[0].r + 45) * sin(lightsources1[0].angle * D2R) + planet[0].y, lightsources1[0].radius);

    }


    glColor3ub(lightsources1[1].color.r, lightsources1[1].color.g, lightsources1[1].color.b);
    if (greenstatus[0] == 1) {

        circle((planet[0].r + 80) * cos(lightsources1[1].angle * D2R) + planet[0].x, (planet[0].r + 80) * sin(lightsources1[1].angle * D2R) + planet[0].y, lightsources1[1].radius);


    }

    glColor3ub(lightsources1[2].color.r, lightsources1[2].color.g, lightsources1[2].color.b);
    if (bluestatus[0] == 1) {

        circle((planet[0].r + 115) * cos(lightsources1[2].angle * D2R) + planet[0].x, (planet[0].r + 115) * sin(lightsources1[2].angle * D2R) + planet[0].y, lightsources1[2].radius);


    }

    //*******************************
    //light source 2

    glColor3ub(lightsources2[0].color.r, lightsources2[0].color.g, lightsources2[0].color.b);
    if (redstatus[1] == 1) {

        circle((planet[1].r + 45) * cos(lightsources2[0].angle * D2R) + planet[1].x, (planet[1].r + 45) * sin(lightsources2[0].angle * D2R) + planet[1].y, lightsources2[0].radius);


    }

    glColor3ub(lightsources2[1].color.r, lightsources2[1].color.g, lightsources2[1].color.b);
    if (greenstatus[1] == 1) {

        circle((planet[1].r + 80) * cos(lightsources2[1].angle * D2R) + planet[1].x, (planet[1].r + 80) * sin(lightsources2[1].angle * D2R) + planet[1].y, lightsources2[1].radius);


    }

    glColor3ub(lightsources2[2].color.r, lightsources2[2].color.g, lightsources2[2].color.b);
    if (bluestatus[1] == 1) {

        circle((planet[1].r + 115) * cos(lightsources2[2].angle * D2R) + planet[1].x, (planet[1].r + 115) * sin(lightsources2[2].angle * D2R) + planet[1].y, lightsources2[2].radius);


    }

    //********************************
    //light source 3

    glColor3ub(lightsources3[0].color.r, lightsources3[0].color.g, lightsources3[0].color.b);
    if (redstatus[2] == 1) {

        circle((planet[2].r + 45) * cos(lightsources3[0].angle * D2R) + planet[2].x, (planet[2].r + 45) * sin(lightsources3[0].angle * D2R) + planet[2].y, lightsources3[0].radius);


    }

    glColor3ub(lightsources3[1].color.r, lightsources3[1].color.g, lightsources3[1].color.b);
    if (greenstatus[2] == 1) {

        circle((planet[2].r + 80) * cos(lightsources3[1].angle * D2R) + planet[2].x, (planet[2].r + 80) * sin(lightsources3[1].angle * D2R) + planet[2].y, lightsources3[1].radius);

    }

    glColor3ub(lightsources3[2].color.r, lightsources3[2].color.g, lightsources3[2].color.b);
    if (bluestatus[2] == 1) {

        circle((planet[2].r + 115) * cos(lightsources3[2].angle * D2R) + planet[2].x, (planet[2].r + 115) * sin(lightsources3[2].angle * D2R) + planet[2].y, lightsources3[2].radius);

    }

    //*********************
    //light source 4

    glColor3ub(lightsources4[0].color.r, lightsources4[0].color.g, lightsources4[0].color.b);
    if (redstatus[3] == 1) {

        circle((planet[3].r + 45) * cos(lightsources4[0].angle * D2R) + planet[3].x, (planet[2].r + 45) * sin(lightsources4[0].angle * D2R) + planet[3].y, lightsources4[0].radius);

    }

    glColor3ub(lightsources4[1].color.r, lightsources4[1].color.g, lightsources4[1].color.b);
    if (greenstatus[3] == 1) {

        circle((planet[3].r + 80) * cos(lightsources4[1].angle * D2R) + planet[3].x, (planet[2].r + 80) * sin(lightsources4[1].angle * D2R) + planet[3].y, lightsources4[1].radius);


    }

    glColor3ub(lightsources4[2].color.r, lightsources4[2].color.g, lightsources4[2].color.b);
    if (bluestatus[3] == 1) {

        circle((planet[3].r + 115) * cos(lightsources4[2].angle * D2R) + planet[3].x, (planet[2].r + 115) * sin(lightsources4[2].angle * D2R) + planet[3].y, lightsources4[2].radius);


    }
}
//
// To display onto window using OpenGL commands
//


void display() {
    //
    // clear window to black
    //
    glClearColor(40. / 255., 40. / 255, 40. / 255, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    label();
    informativeMessages();
    blackplanet();
    orbits();
    if (duration != 0)
        lights();


    displaycolors1();
    displaycolors2();
    displaycolors3();
    displaycolors4();



    glutSwapBuffers();
}
//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }
    if (key == GLUT_KEY_RIGHT)
    {
        if (whichplanet == 4)
            whichplanet = 1;
        else
            whichplanet++;
    }
    if (key == GLUT_KEY_LEFT)
    {
        if (whichplanet == 1)
            whichplanet = 4;
        else
            whichplanet--;

    }
    //change red status
    if (key == GLUT_KEY_F1)
    {
        redstatus[whichplanet - 1] = !redstatus[whichplanet - 1];
    }
    //change blue status

    if (key == GLUT_KEY_F2)
    {
        bluestatus[whichplanet - 1] = !bluestatus[whichplanet - 1];
    }
    //change green status

    if (key == GLUT_KEY_F3)
    {
        greenstatus[whichplanet - 1] = !greenstatus[whichplanet - 1];
    }
    if (key == GLUT_KEY_F4)
    {
        //start and stop animation

        if (checktimer % 2 == 1)
            for (int i = 0; i <= 3; i++)
                activeTimer[i] = 0;
        else
            for (int i = 0; i <= 3; i++)
                activeTimer[i] = 1;
        checktimer++;
    }
    //restart
    if (key == GLUT_KEY_F5)
    {
        duration = 0;
        for (int i = 0; i <= 3; i++)
        {
            redstatus[i] = 1;
            greenstatus[i] = 1;
            bluestatus[i] = 1;
            activeTimer[i] = 1;
            lightsources1[i].angle = 0;
            lightsources2[i].angle = 0;
            lightsources3[i].angle = 0;
            lightsources4[i].angle = 0;
        }
    }
    //restart chosen planet
    if (key == GLUT_KEY_F6)
    {

        redstatus[whichplanet - 1] = 1;
        bluestatus[whichplanet - 1] = 1;
        greenstatus[whichplanet - 1] = 1;
        activeTimer[whichplanet - 1] = 1;

        if (whichplanet == 1)
        {
            for (int i = 0; i <= 2; i++)
            {
                random = rand() % 7;
                lightsources1[i].speed = random + 1;
                random = rand() % 7;
                lightsources1[i].speed = random + 1;
                random = rand() % 7;
                lightsources1[whichplanet - 1].speed = random + 1;

                random = rand() % 100;
                lightsources1[i].location = random;
                random = rand() % 100;
                lightsources1[i].location = random;
                random = rand() % 100;
                lightsources1[i].location = random;

                random = rand() % 10;
                random += 8;
                lightsources1[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources1[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources1[i].radius = random;

                lightsources1[i].angle = 0;
                lightsources1[i].angle = 0;
                lightsources1[i].angle = 0;
            }

        }

        if (whichplanet == 2)
        {

            for (int i = 0; i <= 2; i++)
            {
                random = rand() % 4;
                lightsources2[i].speed = random + 1;
                random = rand() % 4;
                lightsources2[i].speed = random + 1;
                random = rand() % 4;
                lightsources2[whichplanet - 1].speed = random + 1;

                random = rand() % 100;
                lightsources2[i].location = random;
                random = rand() % 100;
                lightsources2[i].location = random;
                random = rand() % 100;
                lightsources2[i].location = random;

                random = rand() % 10;
                random += 8;
                lightsources2[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources2[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources2[i].radius = random;

                lightsources2[i].angle = 0;
                lightsources2[i].angle = 0;
                lightsources2[i].angle = 0;
            }
        }
        if (whichplanet == 3)
        {

            for (int i = 0; i <= 2; i++)
            {
                random = rand() % 4;
                lightsources3[i].speed = random + 1;
                random = rand() % 4;
                lightsources3[i].speed = random + 1;
                random = rand() % 4;
                lightsources3[whichplanet - 1].speed = random + 1;

                random = rand() % 100;
                lightsources3[i].location = random;
                random = rand() % 100;
                lightsources3[i].location = random;
                random = rand() % 100;
                lightsources3[i].location = random;

                random = rand() % 10;
                random += 8;
                lightsources3[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources3[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources3[i].radius = random;

                lightsources3[i].angle = 0;
                lightsources3[i].angle = 0;
                lightsources3[i].angle = 0;
            }
        }
        if (whichplanet == 4)
        {

            for (int i = 0; i <= 2; i++)
            {
                random = rand() % 4;
                lightsources4[i].speed = random + 1;
                random = rand() % 4;
                lightsources4[i].speed = random + 1;
                random = rand() % 4;
                lightsources4[whichplanet - 1].speed = random + 1;

                random = rand() % 100;
                lightsources4[i].location = random;
                random = rand() % 100;
                lightsources4[i].location = random;
                random = rand() % 100;
                lightsources4[i].location = random;

                random = rand() % 10;
                random += 8;
                lightsources4[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources4[i].radius = random;
                random = rand() % 10;
                random += 8;
                lightsources4[i].radius = random;

                lightsources4[i].angle = 0;
                lightsources4[i].angle = 0;
                lightsources4[i].angle = 0;
            }
        }
    }
    //start or stop chosen planet's animation
    if (key == GLUT_KEY_F7)
    {
        activeTimer[whichplanet - 1] = !activeTimer[whichplanet - 1];

    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (duration == 0)
    {
        //give random values for light sources information
        for (int i = 0; i <= 2; i++)
        {
            random = rand() % 7;
            lightsources1[i].speed = random + 1;
            random = rand() % 7;
            lightsources2[i].speed = random + 1;
            random = rand() % 7;
            lightsources3[i].speed = random + 1;
            random = rand() % 7;
            lightsources4[i].speed = random + 1;


            random = rand() % 100;
            lightsources1[i].location = random;
            random = rand() % 100;
            lightsources2[i].location = random;
            random = rand() % 100;
            lightsources3[i].location = random;
            random = rand() % 100;
            lightsources4[i].location = random;


            random = rand() % 7;
            random += 9;
            lightsources1[i].radius = random;
            random = rand() % 7;
            random += 9;
            lightsources2[i].radius = random;
            random = rand() % 7;
            random += 9;
            lightsources3[i].radius = random;
            random = rand() % 7;
            random += 9;
            lightsources4[i].radius = random;

        }
    }
    duration++;

    //planet 1

    if (activeTimer[0] == 1)
    {


        for (int i = 0; i <= 2; i++)
        {
            //find rotation direction
            if (lightsources1[i].location < 50)
            {

                lightsources1[i].angle -= lightsources1[i].speed;
                if (lightsources1[i].angle < 0)
                    lightsources1[i].angle += 360;
                //calculate colors

                if (i == 0)
                    lightsources1[i].pos = addV({ (planet[0].r + 45) * cos(lightsources1[i].angle * D2R) + planet[0].x, (planet[0].r + 45) * sin(lightsources1[i].angle * D2R) + planet[0].y }, { lightsources1[i].speed * cos(lightsources1[i].angle),lightsources1[i].speed * sin(lightsources1[i].angle) });
                if (i == 1)
                    lightsources1[i].pos = addV({ (planet[0].r + 80) * cos(lightsources1[i].angle * D2R) + planet[0].x, (planet[0].r + 80) * sin(lightsources1[i].angle * D2R) + planet[0].y }, { lightsources1[i].speed * cos(lightsources1[i].angle),lightsources1[i].speed * sin(lightsources1[i].angle) });

                if (i == 2)
                    lightsources1[i].pos = addV({ (planet[0].r + 115) * cos(lightsources1[i].angle * D2R) + planet[0].x, (planet[0].r + 115) * sin(lightsources1[i].angle * D2R) + planet[0].y }, { lightsources1[i].speed * cos(lightsources1[i].angle),lightsources1[i].speed * sin(lightsources1[i].angle) });



            }
            //x     (planet[0].r + 45) * cos(lightsources1[0].angle * D2R) + planet[0].x
            //y      (planet[0].r+45) * sin(lightsources1[0].angle * D2R) + planet[0].y
            else
            {
                lightsources1[i].angle += lightsources1[i].speed;
                if (lightsources1[i].angle > 360)
                    lightsources1[i].angle -= 360;
                //calculate colors
                if (i == 0)
                    lightsources1[i].pos = addV({ (planet[0].r + 45) * cos(lightsources1[i].angle * D2R) + planet[0].x, (planet[0].r + 45) * sin(lightsources1[i].angle * D2R) + planet[0].y }, { lightsources1[i].speed * cos(lightsources1[i].angle),lightsources1[i].speed * sin(lightsources1[i].angle) });
                if (i == 1)
                    lightsources1[i].pos = addV({ (planet[0].r + 80) * cos(lightsources1[i].angle * D2R) + planet[0].x, (planet[0].r + 80) * sin(lightsources1[i].angle * D2R) + planet[0].y }, { lightsources1[i].speed * cos(lightsources1[i].angle),lightsources1[i].speed * sin(lightsources1[i].angle) });

                if (i == 2)
                    lightsources1[i].pos = addV({ (planet[0].r + 115) * cos(lightsources1[i].angle * D2R) + planet[0].x, (planet[0].r + 115) * sin(lightsources1[i].angle * D2R) + planet[0].y }, { lightsources1[i].speed * cos(lightsources1[i].angle),lightsources1[i].speed * sin(lightsources1[i].angle) });




            }
        }
    }
    if (activeTimer[1] == 1)
    {
        for (int i = 0; i <= 2; i++)
        {


            if (lightsources2[i].location < 50)
            {
                lightsources2[i].angle -= lightsources2[i].speed;


                if (lightsources2[i].angle < 0)
                    lightsources2[i].angle += 360;
                if (i == 0)
                    lightsources2[i].pos = addV({ (planet[0].r + 45) * cos(lightsources2[i].angle * D2R) + planet[1].x, (planet[1].r + 45) * sin(lightsources2[i].angle * D2R) + planet[1].y }, { lightsources2[i].speed * cos(lightsources2[i].angle),lightsources2[i].speed * sin(lightsources2[i].angle) });
                if (i == 1)
                    lightsources2[i].pos = addV({ (planet[0].r + 80) * cos(lightsources2[i].angle * D2R) + planet[1].x, (planet[1].r + 80) * sin(lightsources2[i].angle * D2R) + planet[1].y }, { lightsources2[i].speed * cos(lightsources2[i].angle),lightsources2[i].speed * sin(lightsources2[i].angle) });

                if (i == 2)
                    lightsources2[i].pos = addV({ (planet[0].r + 115) * cos(lightsources2[i].angle * D2R) + planet[1].x, (planet[1].r + 115) * sin(lightsources2[i].angle * D2R) + planet[1].y }, { lightsources2[i].speed * cos(lightsources2[i].angle),lightsources2[i].speed * sin(lightsources2[i].angle) });
            }
            else
            {
                lightsources2[i].angle += lightsources2[i].speed;

                if (lightsources2[i].angle > 360)
                    lightsources2[i].angle -= 360;
                if (i == 0)
                    lightsources2[i].pos = addV({ (planet[0].r + 45) * cos(lightsources2[i].angle * D2R) + planet[1].x, (planet[1].r + 45) * sin(lightsources2[i].angle * D2R) + planet[1].y }, { lightsources2[i].speed * cos(lightsources2[i].angle),lightsources2[i].speed * sin(lightsources2[i].angle) });
                if (i == 1)
                    lightsources2[i].pos = addV({ (planet[0].r + 80) * cos(lightsources2[i].angle * D2R) + planet[1].x, (planet[1].r + 80) * sin(lightsources2[i].angle * D2R) + planet[1].y }, { lightsources2[i].speed * cos(lightsources2[i].angle),lightsources2[i].speed * sin(lightsources2[i].angle) });

                if (i == 2)
                    lightsources2[i].pos = addV({ (planet[0].r + 115) * cos(lightsources2[i].angle * D2R) + planet[1].x, (planet[1].r + 115) * sin(lightsources2[i].angle * D2R) + planet[1].y }, { lightsources2[i].speed * cos(lightsources2[i].angle),lightsources2[i].speed * sin(lightsources2[i].angle) });
            }
        }
    }
    if (activeTimer[2] == 1)
    {
        for (int i = 0; i <= 2; i++)
        {

            if (lightsources3[i].location < 50)
            {
                lightsources3[i].angle -= lightsources3[i].speed;
                if (lightsources3[i].angle < 0)
                    lightsources3[i].angle += 360;
                if (i == 0)
                    lightsources3[i].pos = addV({ (planet[2].r + 45) * cos(lightsources3[i].angle * D2R) + planet[2].x, (planet[2].r + 45) * sin(lightsources3[i].angle * D2R) + planet[2].y }, { lightsources3[i].speed * cos(lightsources3[i].angle),lightsources3[i].speed * sin(lightsources3[i].angle) });
                if (i == 1)
                    lightsources3[i].pos = addV({ (planet[2].r + 80) * cos(lightsources3[i].angle * D2R) + planet[2].x, (planet[2].r + 80) * sin(lightsources3[i].angle * D2R) + planet[2].y }, { lightsources3[i].speed * cos(lightsources3[i].angle),lightsources3[i].speed * sin(lightsources3[i].angle) });

                if (i == 2)
                    lightsources3[i].pos = addV({ (planet[2].r + 115) * cos(lightsources3[i].angle * D2R) + planet[2].x, (planet[2].r + 115) * sin(lightsources3[i].angle * D2R) + planet[2].y }, { lightsources3[i].speed * cos(lightsources3[i].angle),lightsources3[i].speed * sin(lightsources3[i].angle) });
            }
            else
            {
                lightsources3[i].angle += lightsources3[i].speed;
                if (lightsources3[i].angle > 360)
                    lightsources3[i].angle -= 360;
                if (i == 0)
                    lightsources3[i].pos = addV({ (planet[2].r + 45) * cos(lightsources3[i].angle * D2R) + planet[2].x, (planet[2].r + 45) * sin(lightsources3[i].angle * D2R) + planet[2].y }, { lightsources3[i].speed * cos(lightsources3[i].angle),lightsources3[i].speed * sin(lightsources3[i].angle) });
                if (i == 1)
                    lightsources3[i].pos = addV({ (planet[2].r + 80) * cos(lightsources3[i].angle * D2R) + planet[2].x, (planet[2].r + 80) * sin(lightsources3[i].angle * D2R) + planet[2].y }, { lightsources3[i].speed * cos(lightsources3[i].angle),lightsources3[i].speed * sin(lightsources3[i].angle) });

                if (i == 2)
                    lightsources3[i].pos = addV({ (planet[2].r + 115) * cos(lightsources3[i].angle * D2R) + planet[2].x, (planet[2].r + 115) * sin(lightsources3[i].angle * D2R) + planet[2].y }, { lightsources3[i].speed * cos(lightsources3[i].angle),lightsources3[i].speed * sin(lightsources3[i].angle) });
            }

        }
    }
    if (activeTimer[3] == 1)
    {
        for (int i = 0; i <= 2; i++)
        {

            if (lightsources4[i].location < 50)
            {
                lightsources4[i].angle -= lightsources3[i].speed;
                if (lightsources4[i].angle < 0)
                    lightsources4[i].angle += 360;
                if (i == 0)
                    lightsources4[i].pos = addV({ (planet[3].r + 45) * cos(lightsources4[i].angle * D2R) + planet[3].x, (planet[3].r + 45) * sin(lightsources4[i].angle * D2R) + planet[3].y }, { lightsources4[i].speed * cos(lightsources4[i].angle),lightsources4[i].speed * sin(lightsources4[i].angle) });
                if (i == 1)
                    lightsources4[i].pos = addV({ (planet[3].r + 80) * cos(lightsources4[i].angle * D2R) + planet[3].x, (planet[3].r + 80) * sin(lightsources4[i].angle * D2R) + planet[3].y }, { lightsources4[i].speed * cos(lightsources4[i].angle),lightsources4[i].speed * sin(lightsources4[i].angle) });

                if (i == 2)
                    lightsources4[i].pos = addV({ (planet[3].r + 115) * cos(lightsources4[i].angle * D2R) + planet[3].x, (planet[3].r + 115) * sin(lightsources4[i].angle * D2R) + planet[3].y }, { lightsources4[i].speed * cos(lightsources4[i].angle),lightsources4[i].speed * sin(lightsources4[i].angle) });
            }
            else
            {
                lightsources4[i].angle += lightsources3[i].speed;
                if (lightsources4[i].angle > 360)
                    lightsources4[i].angle -= 360;
                if (i == 0)
                    lightsources4[i].pos = addV({ (planet[3].r + 45) * cos(lightsources4[i].angle * D2R) + planet[3].x, (planet[3].r + 45) * sin(lightsources4[i].angle * D2R) + planet[3].y }, { lightsources4[i].speed * cos(lightsources4[i].angle),lightsources4[i].speed * sin(lightsources4[i].angle) });
                if (i == 1)
                    lightsources4[i].pos = addV({ (planet[3].r + 80) * cos(lightsources4[i].angle * D2R) + planet[3].x, (planet[3].r + 80) * sin(lightsources4[i].angle * D2R) + planet[3].y }, { lightsources4[i].speed * cos(lightsources4[i].angle),lightsources4[i].speed * sin(lightsources4[i].angle) });

                if (i == 2)
                    lightsources4[i].pos = addV({ (planet[3].r + 115) * cos(lightsources4[i].angle * D2R) + planet[3].x, (planet[3].r + 115) * sin(lightsources4[i].angle * D2R) + planet[3].y }, { lightsources4[i].speed * cos(lightsources4[i].angle),lightsources4[i].speed * sin(lightsources4[i].angle) });
            }

        }
    }


    // to refresh the window it calls display() function
    // display()
    glutPostRedisplay();
}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Homework-3");
    srand(time(NULL));
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}