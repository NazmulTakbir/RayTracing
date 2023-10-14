#ifndef UTILS
#define UTILS

#include <cmath>

using namespace std;

const double PI = acos(-1);
const double INF = 1e16;
const double EPS = 1e-5;

double solveQuadratic(const double &a, const double &b, const double &c) {
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return -1;

    discriminant = sqrt(discriminant);
    double t1 = (-b - discriminant) / (2 * a);
    double t2 = (-b + discriminant) / (2 * a);

    if (t1 > 0) return t1;
    if (t2 > 0) return t2;

    return -1;
}

#endif  // UTILS