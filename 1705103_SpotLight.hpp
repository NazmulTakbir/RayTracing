#ifndef SPOTLIGHT
#define SPOTLIGHT

#include <GL/glut.h>

#include <cmath>
#include <vector>

#include "1705103_Point.hpp"
#include "1705103_Utils.hpp"
#include "1705103_Vector.hpp"
using namespace std;

class SpotLight {
   private:
    Point position;
    Vector direction;
    double cutoff_angle;
    double color[3];  // RGB

    // variables below are declared here to reduce stack size of repeated function calls
    double a, b, c, dotProduct, angle, magnitude;
    // done

   public:
    SpotLight(Point position, Vector direction, double cutoff_angle);
    void setColor(double r, double g, double b);
    void draw();

    Point &getPosition();
    const double *getColor();

    bool insideCutoff(Point &viewPoint);
};

SpotLight::SpotLight(Point position, Vector direction, double cutoff_angle) {
    direction = Vector::normalize(direction);
    this->position = position;
    this->direction = direction;
    this->cutoff_angle = cutoff_angle * PI / 180;
}

void SpotLight::setColor(double r, double g, double b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void SpotLight::draw() {
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(position[0], position[1], position[2]);
    glEnd();
    glPointSize(1);
}

Point &SpotLight::getPosition() { return position; }
const double *SpotLight::getColor() { return color; }

bool SpotLight::insideCutoff(Point &viewPoint) {
    a = -position[0] + viewPoint[0];
    b = -position[1] + viewPoint[1];
    c = -position[2] + viewPoint[2];

    magnitude = sqrt(a * a + b * b + c * c);

    a /= magnitude;
    b /= magnitude;
    c /= magnitude;

    dotProduct = a * direction[0] + b * direction[1] + c * direction[2];

    return dotProduct > 0 && acos(dotProduct) < cutoff_angle;
}

vector<SpotLight *> spotLights = vector<SpotLight *>(0);

#endif  // SPOTLIGHT