#ifndef RAY
#define RAY

#include <iostream>

#include "1705103_Object.hpp"
#include "1705103_Point.hpp"
#include "1705103_Vector.hpp"
using namespace std;

class Ray {
   private:
    Point start;
    Vector direction;

   public:
    Ray(){};
    Ray(const Point &start, const Point &source, const Point &destination);

    void setStart(const Point &start);
    void setDirection(const Vector &direction);
    void setDirection(double x, double y, double z);

    Vector &getDirection();
    Point &getStart();
    void moveForward(double distance);

    static Ray getReflectedRay(const Point &intersectionPoint, const Ray &incidentRay, const Ray &normal);
    static double dotProduct(const Ray &ray1, const Ray &ray2);
};

Ray::Ray(const Point &start, const Point &source, const Point &destination) {
    this->start = start;
    direction[0] = destination[0] - source[0];
    direction[1] = destination[1] - source[1];
    direction[2] = destination[2] - source[2];

    double magnitude = sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);

    direction[0] /= magnitude;
    direction[1] /= magnitude;
    direction[2] /= magnitude;
}

void Ray::setStart(const Point &start) { this->start = start; }
void Ray::setDirection(const Vector &direction) { this->direction = direction; }

void Ray::setDirection(double x, double y, double z) {
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
}

Vector &Ray::getDirection() { return direction; }
Point &Ray::getStart() { return start; }

Ray Ray::getReflectedRay(const Point &intersectionPoint, const Ray &incidentRay, const Ray &normal) {
    Ray reflectedRay;
    reflectedRay.setStart(intersectionPoint);

    double dotProduct = normal.direction[0] * incidentRay.direction[0] +
                        normal.direction[1] * incidentRay.direction[1] + normal.direction[2] * incidentRay.direction[2];

    reflectedRay.direction[0] = incidentRay.direction[0] - 2 * dotProduct * normal.direction[0];
    reflectedRay.direction[1] = incidentRay.direction[1] - 2 * dotProduct * normal.direction[1];
    reflectedRay.direction[2] = incidentRay.direction[2] - 2 * dotProduct * normal.direction[2];

    double magnitude = sqrt(reflectedRay.direction[0] * reflectedRay.direction[0] +
                            reflectedRay.direction[1] * reflectedRay.direction[1] +
                            reflectedRay.direction[2] * reflectedRay.direction[2]);

    reflectedRay.direction[0] /= magnitude;
    reflectedRay.direction[1] /= magnitude;
    reflectedRay.direction[2] /= magnitude;

    return reflectedRay;
}

double Ray::dotProduct(const Ray &ray1, const Ray &ray2) {
    return ray1.direction[0] * ray2.direction[0] + ray1.direction[1] * ray2.direction[1] +
           ray1.direction[2] * ray2.direction[2];
}

void Ray::moveForward(double distance) {
    start[0] += direction[0] * distance;
    start[1] += direction[1] * distance;
    start[2] += direction[2] * distance;
}

#endif  // RAY