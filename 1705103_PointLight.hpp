#ifndef POINTLIGHT
#define POINTLIGHT

#include <GL/glut.h>

#include <vector>

#include "1705103_Point.hpp"
using namespace std;

class PointLight {
   private:
    Point position;
    double color[3];  // RGB

   public:
    PointLight(Point position);
    void setColor(double r, double g, double b);

    Point &getPosition();
    const double *getColor();
    void draw();
};

PointLight::PointLight(Point position) { this->position = position; }

Point &PointLight::getPosition() { return position; }

void PointLight::setColor(double r, double g, double b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

const double *PointLight::getColor() {
    return color;
}

void PointLight::draw() {
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(position[0], position[1], position[2]);
    glEnd();
    glPointSize(1);
}

vector<PointLight *> pointLights = vector<PointLight *>(0);

#endif  // POINTLIGHT