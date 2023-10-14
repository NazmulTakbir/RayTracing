#ifndef SPHERE
#define SPHERE

#include <GL/glut.h>

#include <cmath>

#include "1705103_Object.hpp"
#include "1705103_Point.hpp"
#include "1705103_Ray.hpp"
#include "1705103_Utils.hpp"
#include "1705103_Vector.hpp"
using namespace std;

#define pi (2 * acos(0.0))

class Sphere : public Object {
   private:
    Point center, viewStart;
    Vector moveDirection;
    double radius, radiusSquared;

   public:
    Sphere(Point center, double radius, double dx, double dy, double dz);
    void move();

    void draw();
    Ray getNormal(Point &intersectionPoint, Ray &incidentRay);
    void reverseDirection();

    static bool willCollide(Sphere *sphere1, Sphere *sphere2);

    double intersectionParameter(Ray *ray);
};

Sphere::Sphere(Point center, double radius, double dx, double dy, double dz) {
    this->center = center;
    this->radius = radius;
    this->radiusSquared = radius * radius;
    this->moveDirection.setComponents(dx, dy, dz);
}

void Sphere::reverseDirection() {
    moveDirection.setComponents(-moveDirection[0], -moveDirection[1], -moveDirection[2]);
}

bool Sphere::willCollide(Sphere *sphere1, Sphere *sphere2) {
    double x, y, z;
    x = sphere1->center[0];
    y = sphere1->center[1];
    z = sphere1->center[2];

    double x2, y2, z2;
    x2 = sphere2->center[0];
    y2 = sphere2->center[1];
    z2 = sphere2->center[2];

    double minDistance = sphere1->radius + sphere2->radius;
    if (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2) + (z - z2) * (z - z2)) < minDistance) {
        return true;
    } else {
        return false;
    }
}

void Sphere::move() {
    center[0] += moveDirection[0] * 0.25;
    center[1] += moveDirection[1] * 0.25;
    center[2] += moveDirection[2] * 0.25;
}

void Sphere::draw() {
    int slices = 24, stacks = 30;
    Point points[100][100];
    int i, j;
    double h, r;

    for (i = 0; i <= stacks; i++) {
        h = radius * sin(((double)i / (double)stacks) * (pi / 2));
        r = radius * cos(((double)i / (double)stacks) * (pi / 2));
        for (j = 0; j <= slices; j++) {
            points[i][j][0] = r * cos(((double)j / (double)slices) * 2 * pi);
            points[i][j][1] = r * sin(((double)j / (double)slices) * 2 * pi);
            points[i][j][2] = h;
        }
    }

    const double *color = getColor();
    glColor3f(color[0], color[1], color[2]);

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    for (i = 0; i < stacks; i++) {
        for (j = 0; j < slices; j++) {
            glBegin(GL_QUADS);
            {
                // upper hemisphere
                glVertex3f(points[i][j][0], points[i][j][1], points[i][j][2]);
                glVertex3f(points[i][j + 1][0], points[i][j + 1][1], points[i][j + 1][2]);
                glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1], points[i + 1][j + 1][2]);
                glVertex3f(points[i + 1][j][0], points[i + 1][j][1], points[i + 1][j][2]);
                // lower hemisphere
                glVertex3f(points[i][j][0], points[i][j][1], -points[i][j][2]);
                glVertex3f(points[i][j + 1][0], points[i][j + 1][1], -points[i][j + 1][2]);
                glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1], -points[i + 1][j + 1][2]);
                glVertex3f(points[i + 1][j][0], points[i + 1][j][1], -points[i + 1][j][2]);
            }
            glEnd();
        }
    }
    glPopMatrix();
}

double Sphere::intersectionParameter(Ray *ray) {
    Point &rayStart = ray->getStart();
    viewStart[0] = rayStart[0] - center[0];
    viewStart[1] = rayStart[1] - center[1];
    viewStart[2] = rayStart[2] - center[2];

    Vector &direction = ray->getDirection();

    double a = 1;
    double b = 2 * (viewStart[0] * direction[0] + viewStart[1] * direction[1] + viewStart[2] * direction[2]);
    double c =
        (viewStart[0] * viewStart[0] + viewStart[1] * viewStart[1] + viewStart[2] * viewStart[2]) - radiusSquared;

    return solveQuadratic(a, b, c);
}

Ray Sphere::getNormal(Point &intersectionPoint, Ray &incidentRay) {
    return Ray(intersectionPoint, center, intersectionPoint);
}

#endif  // SPHERE