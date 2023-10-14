#include <GL/glut.h>
#include <math.h>
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "1705103_Floor.hpp"
#include "1705103_GeneralQuad.hpp"
#include "1705103_Object.hpp"
#include "1705103_Point.hpp"
#include "1705103_PointLight.hpp"
#include "1705103_Ray.hpp"
#include "1705103_Sphere.hpp"
#include "1705103_SpotLight.hpp"
#include "1705103_Triangle.hpp"
#include "1705103_Utils.hpp"
#include "1705103_Vector.hpp"
#include "1705103_bitmap_image.hpp"
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std;

bool video = true;

int showAxes = 1, imgHeight, imgWidth, recursionDepth, windowHeight = 500, windowWidth = 500, outputNo = 11;
double camShiftMag = 0.25, camRotMag = 3, view_angle = 80;

Vector upVector, rightVector, lookVector;
Point camPos;

void init();
void display();
void animate();

void mouseListener(int button, int state, int x, int y);
void keyboardListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x, int y);

void drawAxes();

void loadData();
void capture();

int main(int argc, char **argv) {
    loadData();

    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("Ray Tracing");

    init();

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();

    return 0;
}

void init() {
    upVector.setComponents(0.5251074, -0.3828704, 0.7600477);
    rightVector.setComponents(-0.5921224, -0.8058419, 0.0031508);
    lookVector.setComponents(0.6112719, -0.4516958, -0.6498596);
    camPos.setCoordinates(-47.1283060, 49.1190923, 79.9720228);

    upVector = Vector::normalize(upVector);
    rightVector = Vector::normalize(rightVector);
    lookVector = Vector::normalize(lookVector);

    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view_angle, 1, 1, 1000.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camPos[0], camPos[1], camPos[2], camPos[0] + lookVector[0], camPos[1] + lookVector[1],
              camPos[2] + lookVector[2], upVector[0], upVector[1], upVector[2]);

    glMatrixMode(GL_MODELVIEW);

    drawAxes();

    for (int i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }

    for (int i = 0; i < pointLights.size(); i++) {
        pointLights[i]->draw();
    }

    for (int i = 0; i < spotLights.size(); i++) {
        spotLights[i]->draw();
    }

    glutSwapBuffers();
}

int counter = 0;
bool forward_move = true;
void animate() {
    if (counter++ > 100) {
        Sphere *sphere1 = dynamic_cast<Sphere *>(objects[0]);
        Sphere *sphere2 = dynamic_cast<Sphere *>(objects[1]);
        if (Sphere::willCollide(sphere1, sphere2)) {
            sphere1->reverseDirection();
            sphere2->reverseDirection();
            forward_move = false;
        }
        sphere1->move();
        sphere2->move();
        if (forward_move) {
            camPos = camPos.translate(camPos, lookVector, camShiftMag);
        } else {
            camPos = camPos.translate(camPos, lookVector, -camShiftMag);
        }

        counter = 0;
        if (video) capture();
    }
    glutPostRedisplay();
}

void mouseListener(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) showAxes = 1 - showAxes;
            break;
        case GLUT_RIGHT_BUTTON:
            break;

        case GLUT_MIDDLE_BUTTON:
            break;
        default:
            break;
    }
}

void keyboardListener(unsigned char key, int x, int y) {
    switch (key) {
        case '0':
            capture();
            break;
        case '1':
            lookVector = lookVector.rotate(lookVector, upVector, camRotMag);
            rightVector = rightVector.rotate(rightVector, upVector, camRotMag);
            if (video) capture();
            break;
        case '2':
            lookVector = lookVector.rotate(lookVector, upVector, -camRotMag);
            rightVector = rightVector.rotate(rightVector, upVector, -camRotMag);
            if (video) capture();
            break;
        case '3':
            lookVector = lookVector.rotate(lookVector, rightVector, camRotMag);
            upVector = upVector.rotate(upVector, rightVector, camRotMag);
            if (video) capture();
            break;
        case '4':
            lookVector = lookVector.rotate(lookVector, rightVector, -camRotMag);
            upVector = upVector.rotate(upVector, rightVector, -camRotMag);
            if (video) capture();
            break;
        case '5':
            upVector = upVector.rotate(upVector, lookVector, camRotMag);
            rightVector = rightVector.rotate(rightVector, lookVector, camRotMag);
            if (video) capture();
            break;
        case '6':
            upVector = upVector.rotate(upVector, lookVector, -camRotMag);
            rightVector = rightVector.rotate(rightVector, lookVector, -camRotMag);
            if (video) capture();
            break;
        default:
            break;
    }
}

void specialKeyListener(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            camPos = camPos.translate(camPos, lookVector, -camShiftMag);
            if (video) capture();
            break;
        case GLUT_KEY_UP:
            camPos = camPos.translate(camPos, lookVector, camShiftMag);
            if (video) capture();
            break;
        case GLUT_KEY_RIGHT:
            camPos = camPos.translate(camPos, rightVector, camShiftMag);
            if (video) capture();
            break;
        case GLUT_KEY_LEFT:
            camPos = camPos.translate(camPos, rightVector, -camShiftMag);
            if (video) capture();
            break;
        case GLUT_KEY_PAGE_UP:
            camPos = camPos.translate(camPos, upVector, camShiftMag);
            if (video) capture();
            break;
        case GLUT_KEY_PAGE_DOWN:
            camPos = camPos.translate(camPos, upVector, -camShiftMag);
            if (video) capture();
            break;
        default:
            break;
    }
}

void drawAxes() {
    if (showAxes == 1) {
        glBegin(GL_LINES);

        // X axis Red
        glColor3f(1, 0, 0);
        glVertex3f(100, 0, 0);
        glVertex3f(0, 0, 0);
        glColor3f(1, 0.75, 0.75);
        glVertex3f(0, 0, 0);
        glVertex3f(-100, 0, 0);

        // Y axis Green
        glColor3f(0, 1, 0);
        glVertex3f(0, 100, 0);
        glVertex3f(0, 0, 0);
        glColor3f(0.75, 1, 0.75);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -100, 0);

        // Z axis Blue
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, 0);
        glColor3f(0.75, 0.75, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -100);

        glEnd();
    }
}

void loadData() {
    ifstream sceneIn;
    sceneIn.open("scene.txt");

    sceneIn >> recursionDepth >> imgWidth;
    imgHeight = imgWidth;

    int numObjects;
    sceneIn >> numObjects;

    string objectType;
    for (int i = 0; i < numObjects; i++) {
        sceneIn >> objectType;

        Object *object;
        double r, g, b, ambient, diffuse, specular, recReflectionCoeff, shine;

        if (objectType == "sphere") {
            Point center;
            sceneIn >> center;

            double radius, dx, dy, dz;
            sceneIn >> radius >> r >> g >> b >> dx >> dy >> dz >> ambient >> diffuse >> specular >>
                recReflectionCoeff >> shine;

            object = new Sphere(center, radius, dx, dy, dz);
            object->setColor(r, g, b);
            object->setCoefficients(ambient, diffuse, specular, recReflectionCoeff, shine);
            objects.push_back(object);

        } else if (objectType == "triangle") {
            Point v1, v2, v3;
            sceneIn >> v1 >> v2 >> v3;
            sceneIn >> r >> g >> b >> ambient >> diffuse >> specular >> recReflectionCoeff >> shine;

            object = new Triangle(v1, v2, v3);
            object->setColor(r, g, b);
            object->setCoefficients(ambient, diffuse, specular, recReflectionCoeff, shine);
            objects.push_back(object);

        } else if (objectType == "general") {
            double A, B, C, D, E, F, G, H, I, J;
            double cubeX, cubeY, cubeZ, length, width, height;
            double r, g, b, ambient, diffuse, specular, recReflectionCoeff, shine;

            sceneIn >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            sceneIn >> cubeX >> cubeY >> cubeZ >> length >> width >> height;
            sceneIn >> r >> g >> b >> ambient >> diffuse >> specular >> recReflectionCoeff >> shine;

            object = new GeneralQuad(A, B, C, D, E, F, G, H, I, J, cubeX, cubeY, cubeZ, length, width, height);
            object->setColor(r, g, b);
            object->setCoefficients(ambient, diffuse, specular, recReflectionCoeff, shine);
            objects.push_back(object);
        }
    }

    int pointLightSources;
    sceneIn >> pointLightSources;

    Point pos;
    double r, g, b;
    for (int i = 0; i < pointLightSources; i++) {
        sceneIn >> pos >> r >> g >> b;

        PointLight *pointLight = new PointLight(pos);
        pointLight->setColor(r, g, b);
        pointLights.push_back(pointLight);
    }

    int spotLightSources;
    sceneIn >> spotLightSources;

    Vector direction;
    double cutOff;
    for (int i = 0; i < spotLightSources; i++) {
        sceneIn >> pos >> r >> g >> b >> direction >> cutOff;

        SpotLight *spotLight = new SpotLight(pos, direction, cutOff);
        spotLight->setColor(r, g, b);
        spotLights.push_back(spotLight);
    }

    Object *object = new Floor(300, 300, 20, 20);
    object->setColor(0, 0, 0);
    object->setCoefficients(0.2, 0.2, 0.2, 0.2, 1);
    objects.push_back(object);

    sceneIn.close();
}

void capture() {
    auto t1 = high_resolution_clock::now();

    bitmap_image image(imgWidth, imgHeight);

    for (int x = 0; x < imgWidth; x++) {
        for (int y = 0; y < imgHeight; y++) {
            image.set_pixel(x, y, 0, 0, 0);
        }
    }

    double planeDistance = (windowHeight / 2.0) / tan(Vector::degToRad(view_angle) / 2.0);

    Point temp, topLeft;
    temp = camPos.translate(camPos, lookVector, planeDistance);
    temp = temp.translate(temp, rightVector, -windowWidth / 2.0);
    topLeft = temp.translate(temp, upVector, windowHeight / 2.0);

    double dx = ((double)windowWidth) / imgWidth;
    double dy = ((double)windowHeight) / imgHeight;

    temp = topLeft.translate(topLeft, rightVector, dx / 2);
    topLeft = temp.translate(temp, upVector, -dy / 2);

    Point currentPixel;
    Object *obj;
    double *color = new double[3];

    Ray ray;
    ray.setStart(camPos);

    for (int x = 0; x < imgWidth; x++) {
        for (int y = 0; y < imgHeight; y++) {
            int nearestObjNo = -1;
            double t, tMin = INF;

            topLeft.translateTwice(topLeft, currentPixel, rightVector, x * dx, upVector, -y * dy);

            // Compute ray direction and normalize. Implemented in place instead of using
            // existing functions to avoid overhead of function call and increase speed

            Vector &rayDirection = ray.getDirection();
            rayDirection[0] = currentPixel[0] - camPos[0];
            rayDirection[1] = currentPixel[1] - camPos[1];
            rayDirection[2] = currentPixel[2] - camPos[2];
            double magnitude = sqrt(rayDirection[0] * rayDirection[0] + rayDirection[1] * rayDirection[1] +
                                    rayDirection[2] * rayDirection[2]);
            rayDirection[0] /= magnitude;
            rayDirection[1] /= magnitude;
            rayDirection[2] /= magnitude;

            for (int objNo = 0; objNo < objects.size(); objNo++) {
                obj = objects[objNo];

                t = obj->intersectionParameter(&ray);
                if (t > 0 && t < tMin) {
                    tMin = t;
                    nearestObjNo = objNo;
                }
            }

            if (nearestObjNo != -1) {
                for (int i = 0; i < 3; i++) color[i] = 0;

                objects[nearestObjNo]->intersectionColor(tMin, &ray, color, recursionDepth);
                for (int i = 0; i < 3; i++) {
                    color[i] = max(min(color[i], 1.0), 0.0);
                }
                image.set_pixel(x, y, round(color[0] * 255), round(color[1] * 255), round(color[2] * 255));
            }
        }
    }
    delete[] color;

    if (video) {
        string originalNo, newNo;
        newNo = originalNo = to_string(outputNo);
        for (int i = 1; i <= 6 - originalNo.length(); i++) newNo = "0" + newNo;
        image.save_image("images/output" + newNo + ".bmp");
    } else {
        image.save_image("output" + to_string(outputNo) + ".bmp");
    }
    outputNo++;

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    cout << "\nTime to Generate Output: " << ms_double.count() / 1000 << "seconds\n";
}