#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Dimension {
    public:
        void drawAt(Window&, double x, double y, double angle);
    private:
        Text text;
};