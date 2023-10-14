#ifndef FLOOR
#define FLOOR

#include <GL/glut.h>

#include <iostream>

#include "1705103_Object.hpp"
#include "1705103_Point.hpp"
#include "1705103_Ray.hpp"
#include "1705103_Utils.hpp"
#include "1705103_Vector.hpp"
using namespace std;

class Floor : public Object {
   private:
    double floorLength, floorWidth, boxLength, boxWidth;
    double mainColor[3], invertedColor[3];
    double t, x, y;
    int boxNoX, boxNoY;

   public:
    Floor(double floorLength, double floorWidth, double boxLength, double boxWidth);

    void setColor(double r, double g, double b);

    void draw();
    Ray getNormal(Point &intersectionPoint, Ray &incidentRay);
    double *getColorAt(const Point &intersectionPoint);

    double intersectionParameter(Ray *ray);
};

Floor::Floor(double floorLength, double floorWidth, double boxLength, double boxWidth) {
    this->floorLength = floorLength;
    this->floorWidth = floorWidth;
    this->boxLength = boxLength;
    this->boxWidth = boxWidth;
}

void Floor::setColor(double r, double g, double b) {
    mainColor[0] = r;
    mainColor[1] = g;
    mainColor[2] = b;
    invertedColor[0] = 1 - r;
    invertedColor[1] = 1 - g;
    invertedColor[2] = 1 - b;
}

void Floor::draw() {
    int num_x = (int)round(floorLength / boxLength);
    int num_y = (int)round(floorWidth / boxWidth);

    double x = -floorLength / 2;
    for (int row = 0; row < num_x; row++) {
        double y = -floorWidth / 2;
        for (int column = 0; column < num_y; column++) {
            if (row % 2 == 0 && column % 2 == 0) {
                glColor3f(mainColor[0], mainColor[1], mainColor[2]);
            } else if (row % 2 == 0 && column % 2 != 0) {
                glColor3f(invertedColor[0], invertedColor[1], invertedColor[2]);
            } else if (row % 2 != 0 && column % 2 == 0) {
                glColor3f(invertedColor[0], invertedColor[1], invertedColor[2]);
            } else if (row % 2 != 0 && column % 2 != 0) {
                glColor3f(mainColor[0], mainColor[1], mainColor[2]);
            }

            glBegin(GL_QUADS);
            {
                glVertex3f(x, y, 0);
                glVertex3f(x + boxLength, y, 0);
                glVertex3f(x + boxLength, y + boxWidth, 0);
                glVertex3f(x, y + boxWidth, 0);
            }
            glEnd();

            y += boxWidth;
        }
        x += boxLength;
    }
}

double Floor::intersectionParameter(Ray *ray) {
    Point &start = ray->getStart();
    Vector &direction = ray->getDirection();

    if (start[2] > -EPS && start[2] < EPS) return -1;
    if (direction[2] > -EPS && direction[2] < EPS) return -1;

    t = -start[2] / direction[2];

    if (t <= 0) return -1;

    x = start[0] + t * direction[0];
    y = start[1] + t * direction[1];

    if (x < -floorLength / 2 || x > floorLength / 2 || y < -floorWidth / 2 || y > floorWidth / 2) return -1;

    return t;
}

Ray Floor::getNormal(Point &intersectionPoint, Ray &incidentRay) {
    Ray normal;
    normal.setStart(intersectionPoint);

    Point &start = incidentRay.getStart();
    if (start[2] > 0) {
        normal.setDirection(0, 0, 1);
    } else {
        normal.setDirection(0, 0, -1);
    }

    return normal;
}

double *Floor::getColorAt(const Point &intersectionPoint) {
    x = intersectionPoint[0];
    y = intersectionPoint[1];

    boxNoX = floor((x - floorLength / 2) / boxLength);
    boxNoY = floor((y - floorWidth / 2) / boxWidth);

    if (boxNoX % 2 == 0 && boxNoY % 2 == 0) {
        return mainColor;
    } else if (boxNoX % 2 == 0 && boxNoY % 2 != 0) {
        return invertedColor;
    } else if (boxNoX % 2 != 0 && boxNoY % 2 == 0) {
        return invertedColor;
    } else {
        return mainColor;
    }
}

#endif  // FLOOR