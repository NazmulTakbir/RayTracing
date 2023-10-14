#ifndef TRIANGLE
#define TRIANGLE

#include <GL/glut.h>

#include <iostream>
#include <vector>

#include "1705103_Object.hpp"
#include "1705103_Point.hpp"
#include "1705103_Ray.hpp"
#include "1705103_Utils.hpp"
#include "1705103_Vector.hpp"
using namespace std;

class Triangle : public Object {
   private:
    Point vertices[3];
    Vector normal;

    double matrix[3][3], a, b, c, detA, beta, gamma, t, x, y, z, dotProduct;
    double matrixDeterminant();

   public:
    Triangle(Point v1, Point v2, Point v3);

    void draw();
    Ray getNormal(Point &intersectionPoint, Ray &incidentRay);

    double intersectionParameter(Ray *ray);
};

Triangle::Triangle(Point v1, Point v2, Point v3) {
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;

    Vector vector1 = Point::getVector(vertices[0], vertices[1]);
    Vector vector2 = Point::getVector(vertices[0], vertices[2]);

    normal = Vector::crossProduct(vector1, vector2);
    normal = Vector::normalize(normal);
}

void Triangle::draw() {
    const double *color = getColor();
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
        glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
        glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);
    }
    glEnd();
}

double Triangle::matrixDeterminant() {
    a = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2]);
    b = matrix[0][1] * (matrix[2][0] * matrix[1][2] - matrix[1][0] * matrix[2][2]);
    c = matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1]);

    return a + b + c;
}

double Triangle::intersectionParameter(Ray *ray) {
    Vector &rayDirection = ray->getDirection();
    Point &rayStart = ray->getStart();

    for (int i = 0; i < 3; i++) matrix[i][0] = vertices[0][i] - vertices[1][i];
    for (int i = 0; i < 3; i++) matrix[i][1] = vertices[0][i] - vertices[2][i];
    for (int i = 0; i < 3; i++) matrix[i][2] = rayDirection[i];
    detA = matrixDeterminant();

    if (fabs(detA) < EPS) return -1;

    x = vertices[0][0] - rayStart[0];
    y = vertices[0][1] - rayStart[1];
    z = vertices[0][2] - rayStart[2];

    matrix[0][0] = x;
    matrix[1][0] = y;
    matrix[2][0] = z;
    beta = matrixDeterminant() / detA;
    if (beta <= 0) return -1;

    for (int i = 0; i < 3; i++) matrix[i][0] = vertices[0][i] - vertices[1][i];
    matrix[0][1] = x;
    matrix[1][1] = y;
    matrix[2][1] = z;
    gamma = matrixDeterminant() / detA;
    if (gamma <= 0 || beta + gamma >= 1) return -1;

    for (int i = 0; i < 3; i++) matrix[i][1] = vertices[0][i] - vertices[2][i];
    matrix[0][2] = x;
    matrix[1][2] = y;
    matrix[2][2] = z;
    t = matrixDeterminant() / detA;
    if (t <= 0) return -1;

    return t;
}

Ray Triangle::getNormal(Point &intersectionPoint, Ray &incidentRay) {
    Ray normalRay;
    normalRay.setStart(intersectionPoint);

    if (Vector::dotProduct(normal, incidentRay.getDirection()) > 0) {
        normalRay.setDirection(-normal);
    } else {
        normalRay.setDirection(normal);
    }

    return normalRay;
}

#endif  // TRIANGLE