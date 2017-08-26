#include "Arc.h"

using namespace std;

Arc::Arc() {
    setFillColor(Color::Transparent);
    setOutlineColor(Color::White);
    setOutlineThickness(1);
    _radius = 30;
    setAngle(180);
    _points[_SEGMENTS - 1] = Vector2f(0,0); //center point for nicer drawing
}

void Arc::setRadius(double radius) {
    if (radius == _radius) return; //same radius, don't recalculate
    for (int i = 0; i < _SEGMENTS - 1; i++) {
        _points[i] *= static_cast<float>(radius / _radius); //scale all points to match new radius
    }
    _radius = radius;
    update(); //let SFML know the shape has changed
}
void Arc::setAngle(double angle) { //updates all points based on new angle
    angle *= TO_RADS;
    if (angle == _angle) return; //same angle, don't recalculate
    _angle = angle;
    double angStart = -_angle / 2.0;
    double angStep = _angle / static_cast<double>(_SEGMENTS - 1);
    for (int i = 0; i < _SEGMENTS - 1; i++) {
        _points[i] = Vector2f(_radius*cos(angStart + angStep*i), _radius*sin(angStart + angStep*i));
    }
    update(); //let SFML know the shape has changed
}
double Arc::getAngle() const { return _angle * TO_DEGS; }

size_t Arc::getPointCount() const { return _SEGMENTS; } //Shape interface
Vector2f Arc::getPoint(size_t i) const { return _points[i]; } //Shape interface
