#ifndef POINT
#define POINT

#include <cmath>
#include <iomanip>
#include <iostream>

#include "1705103_Vector.hpp"
using namespace std;

class Point {
   private:
    double coordinates[3];

   public:
    void setCoordinates(double x, double y, double z);

    Point operator-() const;
    double operator[](int index) const;
    double &operator[](int index);

    Point translate(const Point &source, const Vector &direction, double unit);
    void translateTwice(const Point &source, Point &destination, const Vector &direction1, double unit1,
                        const Vector &direction2, double unit2);

    static double getDistance(const Point &source, const Point &destination);
    static Vector getVector(const Point &source, const Point &destination);

    friend istream &operator>>(istream &inputStream, Point &point);
    friend ostream &operator<<(ostream &outputStream, const Point &point);
};

void Point::setCoordinates(double x, double y, double z) {
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
}

Point Point::operator-() const {
    Point negatedPoint;
    negatedPoint.setCoordinates(-coordinates[0], -coordinates[1], -coordinates[2]);
    return negatedPoint;
}

double Point::operator[](int index) const {
    if (index == 0 || index == 1 || index == 2) {
        return coordinates[index];
    } else {
        cout << "Point Index cannot be " << index << "\n";
        exit(-1);
    }
}

double &Point::operator[](int index) {
    if (index == 0 || index == 1 || index == 2) {
        return coordinates[index];
    } else {
        cout << "Point Index cannot be " << index << "\n";
        exit(-1);
    }
}

Point Point::translate(const Point &source, const Vector &direction, double unit) {
    Vector normDirection = Vector::normalize(direction);
    Point destination;
    destination[0] = source[0] + unit * normDirection[0];
    destination[1] = source[1] + unit * normDirection[1];
    destination[2] = source[2] + unit * normDirection[2];
    return destination;
}

void Point::translateTwice(const Point &source, Point &destination, const Vector &direction1, double unit1,
                           const Vector &direction2, double unit2) {
    destination[0] = source[0] + unit1 * direction1[0];
    destination[1] = source[1] + unit1 * direction1[1];
    destination[2] = source[2] + unit1 * direction1[2];

    destination[0] = destination[0] + unit2 * direction2[0];
    destination[1] = destination[1] + unit2 * direction2[1];
    destination[2] = destination[2] + unit2 * direction2[2];
}

double Point::getDistance(const Point &source, const Point &destination) {
    double a = source[0] - destination[0];
    double b = source[1] - destination[1];
    double c = source[2] - destination[2];
    return sqrt(a * a + b * b + c * c);
}

Vector Point::getVector(const Point &source, const Point &destination) {
    Vector vector;
    vector[0] = destination[0] - source[0];
    vector[1] = destination[1] - source[1];
    vector[2] = destination[2] - source[2];
    return vector;
}

istream &operator>>(istream &inputStream, Point &point) {
    inputStream >> point.coordinates[0] >> point.coordinates[1] >> point.coordinates[2];
    return inputStream;
}

ostream &operator<<(ostream &outputStream, const Point &point) {
    outputStream << setprecision(7) << fixed << point.coordinates[0] << " " << point.coordinates[1] << " "
                 << point.coordinates[2] << "\n";
    return outputStream;
}

#endif  // POINT