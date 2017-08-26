#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>
using namespace sf;

#include "constants.h"

class Arc : public Shape {
    public:
        Arc();
        void setAngle(double);
        double getAngle() const;
        void setRadius(double);
        
        virtual size_t getPointCount() const;
        virtual Vector2f getPoint(size_t) const;
    private:
        double _angle;
        double _radius;
        static const int _SEGMENTS = 40;
        Vector2f _points[_SEGMENTS];
};
