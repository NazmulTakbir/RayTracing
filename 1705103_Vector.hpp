#ifndef VECTOR
#define VECTOR

#include <math.h>

#include <iomanip>
#include <iostream>

#include "1705103_Point.hpp"
using namespace std;

class Vector {
   private:
    double components[3];

   public:
    void setComponents(double x, double y, double z);

    double operator[](int index) const;
    double &operator[](int index);
    Vector operator-() const;

    static double degToRad(double deg);
    static Vector normalize(const Vector &vector);
    static double dotProduct(const Vector &a, const Vector &b);
    static Vector crossProduct(const Vector &a, const Vector &b);
    static Vector rotate(const Vector &vector, const Vector &axis, double angle);

    friend istream &operator>>(istream &inputStream, Vector &vector);
    friend ostream &operator<<(ostream &outputStream, const Vector &vector);
};

void Vector::setComponents(double x, double y, double z) {
    components[0] = x;
    components[1] = y;
    components[2] = z;
}

Vector Vector::operator-() const {
    Vector negatedVector;
    negatedVector.setComponents(-components[0], -components[1], -components[2]);
    return negatedVector;
}

double Vector::operator[](int index) const {
    if (index == 0 || index == 1 || index == 2) {
        return components[index];
    } else {
        cout << "Vector Index cannot be " << index << "\n";
        exit(-1);
    }
}

double &Vector::operator[](int index) {
    if (index == 0 || index == 1 || index == 2) {
        return components[index];
    } else {
        cout << "Vector Index cannot be " << index << "\n";
        exit(-1);
    }
}

double Vector::degToRad(double deg) { return deg * (2 * acos(0.0)) / 180.0; }

Vector Vector::normalize(const Vector &vector) {
    double m = sqrt(vector.components[0] * vector.components[0] + vector.components[1] * vector.components[1] +
                    vector.components[2] * vector.components[2]);

    Vector normalizedVector;
    normalizedVector.components[0] = vector.components[0] / m;
    normalizedVector.components[1] = vector.components[1] / m;
    normalizedVector.components[2] = vector.components[2] / m;
    return normalizedVector;
}

double Vector::dotProduct(const Vector &a, const Vector &b) {
    return a.components[0] * b.components[0] + a.components[1] * b.components[1] + a.components[2] * b.components[2];
}

Vector Vector::crossProduct(const Vector &a, const Vector &b) {
    Vector c;
    c.components[0] = a.components[1] * b.components[2] - b.components[1] * a.components[2];
    c.components[1] = -(a.components[0] * b.components[2] - b.components[0] * a.components[2]);
    c.components[2] = a.components[0] * b.components[1] - b.components[0] * a.components[1];
    return c;
}

Vector Vector::rotate(const Vector &vector, const Vector &axis, double angle) {
    Vector normalizedAxis = normalize(axis);
    angle = degToRad(angle);

    double dp = dotProduct(vector, normalizedAxis);
    Vector cp = crossProduct(normalizedAxis, vector);

    Vector vr;
    vr.components[0] = vector.components[0] * cos(angle) + cp.components[0] * sin(angle) +
                       normalizedAxis.components[0] * dp * (1 - cos(angle));

    vr.components[1] = vector.components[1] * cos(angle) + cp.components[1] * sin(angle) +
                       normalizedAxis.components[1] * dp * (1 - cos(angle));

    vr.components[2] = vector.components[2] * cos(angle) + cp.components[2] * sin(angle) +
                       normalizedAxis.components[2] * dp * (1 - cos(angle));

    return vr;
}

istream &operator>>(istream &inputStream, Vector &vector) {
    inputStream >> vector.components[0] >> vector.components[1] >> vector.components[2];
    return inputStream;
}

ostream &operator<<(ostream &outputStream, const Vector &vector) {
    outputStream << setprecision(7) << fixed << vector.components[0] << " " << vector.components[1] << " "
                 << vector.components[2] << "\n";
    return outputStream;
}

#endif  // VECTOR