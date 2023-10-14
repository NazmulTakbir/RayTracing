#ifndef GENERALQUAD
#define GENERALQUAD

#include <GL/glut.h>

#include <cmath>
#include <iostream>

#include "1705103_Object.hpp"
#include "1705103_Point.hpp"
#include "1705103_Ray.hpp"
#include "1705103_Utils.hpp"
using namespace std;

class GeneralQuad : public Object {
   private:
    double A, B, C, D, E, F, G, H, I, J;
    double xd, yd, zd, xo, yo, zo, t0, t1, determinant, x, y, z, magnitude;
    double maxX = INF, maxY = INF, maxZ = INF, minX = -INF, minY = -INF, minZ = -INF;

   public:
    GeneralQuad(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J,
                double cubeX, double cubeY, double cubeZ, double length, double width, double height);

    void draw();

    Ray getNormal(Point &intersectionPoint, Ray &incidentRay);
    double intersectionParameter(Ray *ray);
};

GeneralQuad::GeneralQuad(double A, double B, double C, double D, double E, double F, double G, double H, double I,
                         double J, double cubeX, double cubeY, double cubeZ, double length, double width,
                         double height) {
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
    this->E = E;
    this->F = F;
    this->G = G;
    this->H = H;
    this->I = I;
    this->J = J;

    // length, width, height is thought of as translation vector from reference point.
    // Hence length, width, height might be negative as well. Using fabs will be redundant
    // if length, width and height are all positive.

    if (fabs(length) > EPS) {
        maxX = max(cubeX, cubeX + length);
        minX = min(cubeX, cubeX + length);
    }
    if (fabs(width) > EPS) {
        maxY = max(cubeY, cubeY + width);
        minY = min(cubeY, cubeY + width);
    }
    if (fabs(height) > EPS) {
        maxZ = max(cubeZ, cubeZ + height);
        minZ = min(cubeZ, cubeZ + height);
    }
}

void GeneralQuad::draw() {}

double GeneralQuad::intersectionParameter(Ray *ray) {
    // Reference: http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html

    Vector &direction = ray->getDirection();
    Point &start = ray->getStart();

    xd = direction[0];
    yd = direction[1];
    zd = direction[2];
    xo = start[0];
    yo = start[1];
    zo = start[2];

    double Aq = A * xd * xd + B * yd * yd + C * zd * zd + D * xd * yd + E * xd * zd + F * yd * zd;

    double Bq = 2 * A * xo * xd + 2 * B * yo * yd + 2 * C * zo * zd + D * (xo * yd + yo * xd) +
                E * (xo * zd + zo * xd) + F * (yo * zd + yd * zo) + G * xd + H * yd + I * zd;

    double Cq = A * xo * xo + B * yo * yo + C * zo * zo + D * xo * yo + E * xo * zo + F * yo * zo + G * xo + H * yo +
                I * zo + J;

    if (fabs(Aq) > EPS) {
        determinant = Bq * Bq - 4 * Aq * Cq;
        if (determinant < 0) {
            return -1;
        }
        determinant = sqrt(determinant);

        t1 = (-Bq + determinant) / (2 * Aq);
        if (t1 < 0) {
            return -1;
        }
        t0 = (-Bq - determinant) / (2 * Aq);

        if (t0 > 0) {
            x = start[0] + t0 * direction[0];
            y = start[1] + t0 * direction[1];
            z = start[2] + t0 * direction[2];

            if (x > minX && x < maxX && y > minY && y < maxY && z > minZ && z < maxZ) {
                return t0;
            }
        }

        if (t1 > 0) {
            x = start[0] + t1 * direction[0];
            y = start[1] + t1 * direction[1];
            z = start[2] + t1 * direction[2];

            if (x > minX && x < maxX && y > minY && y < maxY && z > minZ && z < maxZ) {
                return t1;
            }
        }
    } else {
        if (fabs(Bq) > EPS) {
            t0 = -Cq / Bq;
            if (t0 > 0) {
                x = start[0] + t0 * direction[0];
                y = start[1] + t0 * direction[1];
                z = start[2] + t0 * direction[2];

                if (x > minX && x < maxX && y > minY && y < maxY && z > minZ && z < maxZ) {
                    return t0;
                }
            }
        }
    }

    return -1;
}

Ray GeneralQuad::getNormal(Point &intersectionPoint, Ray &incidentRay) {
    x = 2 * A * intersectionPoint[0] + D * intersectionPoint[1] + E * intersectionPoint[2] + G;
    y = 2 * B * intersectionPoint[1] + D * intersectionPoint[0] + F * intersectionPoint[2] + H;
    z = 2 * C * intersectionPoint[2] + E * intersectionPoint[0] + F * intersectionPoint[1] + I;

    Ray normal;
    normal.setStart(intersectionPoint);

    magnitude = sqrt(x * x + y * y + z * z);

    normal.setDirection(x / magnitude, y / magnitude, z / magnitude);

    return normal;
}

#endif  // GENERALQUAD