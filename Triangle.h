#pragma once

#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
using namespace sf;

#include "Shapes.h"
#include "Message.h"
#include "constants.h"

class Triangle : public Drawable {
    public:
        Triangle();
        virtual void draw(RenderTarget&, RenderStates) const;
        void clickAt(const Vector2f&);
        void sendChar(char);
        void clear();
        void reset();
        void message(const sf::String&);
        void cancelSelection();
        void update();
        void resize(const Vector2f&);
    private:
        Message _messageBox;
        Angle _angles[3];
        Side _sides[3];
        Font _mainFont;
        unsigned int _windowWidth;
        unsigned int _windowHeight;
        
        bool _checkConstraints(double side[3], double angle[3], int &sideCount, int &angleCount);
        bool _lawOfCosines(double side[3], double angle[3], int &sideCount, int &angleCount);
        void _drawSolved(double side[3], double angle[3]);
};
