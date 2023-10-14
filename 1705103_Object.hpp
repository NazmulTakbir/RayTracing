#ifndef OBJECT
#define OBJECT

#include <vector>

#include "1705103_Point.hpp"
#include "1705103_PointLight.hpp"
#include "1705103_Ray.hpp"
#include "1705103_SpotLight.hpp"
#include "1705103_Utils.hpp"
using namespace std;

class Object;

vector<Object *> objects = vector<Object *>(0);

class Object {
   private:
    double color[3];         // RGB
    double coefficients[4];  // ambient, diffuse, specular, recursive reflection coeff
    int shine;

   public:
    virtual void draw() = 0;
    virtual double intersectionParameter(Ray *viewRay) = 0;
    virtual Ray getNormal(Point &intersectionPoint, Ray &incidentRay) = 0;

    void intersectionColor(double t, Ray *viewRay, double *color, int level);

    virtual void setColor(double r, double g, double b);
    void setCoefficients(double ambient, double diffuse, double specular, double reflection, int shine);

    const double *getColor();
    const double *getCoefficients();
    virtual double *getColorAt(const Point &intersectionPoint);
};

void Object::setColor(double r, double g, double b) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Object::setCoefficients(double ambient, double diffuse, double specular, double reflection, int shine) {
    coefficients[0] = ambient;
    coefficients[1] = diffuse;
    coefficients[2] = specular;
    coefficients[3] = reflection;
    this->shine = shine;
}

const double *Object::getColor() { return color; }
const double *Object::getCoefficients() { return coefficients; }
double *Object::getColorAt(const Point &intersectionPoint) { return color; }

void Object::intersectionColor(double t, Ray *viewRay, double *color, int level) {
    Point &start = viewRay->getStart();
    Vector &direction = viewRay->getDirection();

    Point intersectionPoint;
    intersectionPoint.setCoordinates(start[0] + direction[0] * t, start[1] + direction[1] * t,
                                     start[2] + direction[2] * t);

    const double *colorAtIntersection = getColorAt(intersectionPoint);
    for (int i = 0; i < 3; i++) color[i] = colorAtIntersection[i] * coefficients[0];

    for (int lightNo = 0; lightNo < pointLights.size(); lightNo++) {
        PointLight *pointLight = pointLights[lightNo];

        bool shaded = false;
        Ray incidentRay(pointLight->getPosition(), pointLight->getPosition(), intersectionPoint);
        Ray normalRay = getNormal(intersectionPoint, incidentRay);
        Ray reflectedRay = Ray::getReflectedRay(intersectionPoint, incidentRay, normalRay);

        double selft = Point::getDistance(intersectionPoint, pointLight->getPosition());

        if (selft < EPS) continue;

        for (int obstacleNo = 0; obstacleNo < objects.size(); obstacleNo++) {
            double obstaclet = objects[obstacleNo]->intersectionParameter(&incidentRay);
            if (obstaclet > 0 && obstaclet + EPS < selft) {
                shaded = true;
                break;
            }
        }

        if (!shaded) {
            double phongValue = max(0.0, -Ray::dotProduct(reflectedRay, *viewRay));
            double dotProductIN = -Ray::dotProduct(incidentRay, normalRay);

            for (int i = 0; i < 3; i++) {
                color[i] +=
                    pointLight->getColor()[i] * coefficients[1] * max(0.0, dotProductIN) * colorAtIntersection[i];

                color[i] +=
                    pointLight->getColor()[i] * coefficients[2] * pow(phongValue, shine) * colorAtIntersection[i];
            }
        }
    }

    for (int i = 0; i < spotLights.size(); i++) {
        SpotLight *spotLight = spotLights[i];

        if (spotLight->insideCutoff(intersectionPoint)) {
            bool shaded = false;
            Ray incidentRay(spotLight->getPosition(), spotLight->getPosition(), intersectionPoint);
            Ray normalRay = getNormal(intersectionPoint, incidentRay);
            Ray reflectedRay = Ray::getReflectedRay(intersectionPoint, incidentRay, normalRay);

            double selft = Point::getDistance(intersectionPoint, spotLight->getPosition());

            if (selft < EPS) continue;

            for (int obstacleNo = 0; obstacleNo < objects.size(); obstacleNo++) {
                double obstaclet = objects[obstacleNo]->intersectionParameter(&incidentRay);
                if (obstaclet > 0 && obstaclet + EPS < selft) {
                    shaded = true;
                    break;
                }
            }

            if (!shaded) {
                double phongValue = max(0.0, -Ray::dotProduct(reflectedRay, *viewRay));
                double dotProductIN = -Ray::dotProduct(incidentRay, normalRay);

                for (int i = 0; i < 3; i++) {
                    color[i] +=
                        spotLight->getColor()[i] * coefficients[1] * max(0.0, dotProductIN) * colorAtIntersection[i];

                    color[i] +=
                        spotLight->getColor()[i] * coefficients[2] * pow(phongValue, shine) * colorAtIntersection[i];
                }
            }
        }
    }

    if (level >= 2) {
        Ray normalRay = getNormal(intersectionPoint, *viewRay);
        Ray reflectedRay = Ray::getReflectedRay(intersectionPoint, *viewRay, normalRay);
        reflectedRay.moveForward(EPS);

        int nearestObjNo = -1;
        double t, tMin = INF;
        Object *obj;

        for (int objNo = 0; objNo < objects.size(); objNo++) {
            obj = objects[objNo];

            t = obj->intersectionParameter(&reflectedRay);
            if (t > 0 && t < tMin) {
                tMin = t;
                nearestObjNo = objNo;
            }
        }

        if (nearestObjNo != -1) {
            double reflectedColor[3];
            for (int i = 0; i < 3; i++) reflectedColor[i] = 0;

            objects[nearestObjNo]->intersectionColor(tMin, &reflectedRay, reflectedColor, level - 1);
            for (int i = 0; i < 3; i++) {
                color[i] += reflectedColor[i] * coefficients[3];
            }
        }
    }
}

#endif  // OBJECT