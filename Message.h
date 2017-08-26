#pragma once

#include <string>
#include <SFML/Graphics.hpp>
using namespace sf;

#include "constants.h"

class Message : public Drawable {
    public:
        Message();
        void display(const String&);
        void draw(RenderTarget&, RenderStates) const;
        void setFont(const Font&);
        bool contains(const Vector2f&) const;
        bool displayed = false;
    private:
        RectangleShape _box;
        Text _text;
        Text _defaultText;
};
