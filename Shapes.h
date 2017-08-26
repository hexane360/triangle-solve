#pragma once

#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <SFML/Graphics.hpp>
using namespace sf;

#include "constants.h"
#include "Arc.h"

class Dimension : public Drawable { //abstract class
    public:
        virtual void setDrawPos(const Vector2f &pos, double angle) = 0; //update shapes, one corner of line
        virtual void draw(RenderTarget&, RenderStates) const = 0; //called each frame
        virtual void setValue(double val) = 0; //called by Triangle to update with solved values; updates shapes
        virtual double getValue() const = 0; //called by Triangle on d
        virtual void flipText(bool) = 0; //draw text upside down?
        void select(); //show cursor and wait for entry
        void cancelSelection();
        //double distFrom(const Vector2f&) const;
        bool contains(const Vector2f&) const;
        bool selected() const;
        void sendChar(char c); //type char to entry window
        bool isDriving() const; //whether value has been set by hand
        void makeDriven();      //reset value
        void setFont(const Font&); //called to initialize text's font
    protected:
        virtual void _update(const std::wstring& = L"") = 0; //called whenever dimension text needs to be updated
        //double _euclidDist(const Vector2f&, const Vector2f&) const;
        Text _text;
        std::wstring _partial;
        bool _driving;
        bool _selected;
};

class Side : public Dimension {
    public:
        Side();
        virtual void setDrawPos(const Vector2f &pos, double angle); //update shapes, one corner of line
        virtual void draw(RenderTarget&, RenderStates) const; //called each frame
        virtual void setValue(double val); //called by Triangle to update with solved values; updates shapes
        virtual double getValue() const; //called by Triangle on d
        virtual void flipText(bool);
        void setScale(double); //px/units
    private:
        static const int HEIGHT_FUDGE = 5;
        virtual void _update(const std::wstring& = L"");
        bool _flipText = false;
        Vector2f _midpoint() const;
        double _length; //units
        double _scale; //px/unit
        RectangleShape _textBG;
        RectangleShape _line;
};

class Angle : public Dimension {
    public:
        Angle();
        virtual void setDrawPos(const Vector2f &pos, double angle); //update shapes, one corner of line
        virtual void draw(RenderTarget&, RenderStates) const; //called each frame
        virtual void setValue(double val); //called by Triangle to update with solved values; updates shapes
        virtual double getValue() const; //called by Triangle on d
        virtual void flipText(bool);
    private:
        static const int HEIGHT_FUDGE = 5;
        virtual void _update(const std::wstring& = L"");
        bool _flipText = false;
        double _angle; //angle
        Arc _arc;
};
