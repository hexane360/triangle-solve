#include "Shapes.h"
using namespace std;

void Dimension::select() {
    //_text.setOutlineColor(Color::White);
    _partial = L"";
    _update(L"_");
    _text.setFillColor(SELECTED_COLOR);
    _selected = true;
}

bool Dimension::selected() const { return _selected; } //whether or not dimension is selected

void Dimension::cancelSelection() { //deselect dimension
    if (_selected) {
        _selected = false;
        if (_driving)
            _text.setFillColor(DRIVING_COLOR);
        else
            _text.setFillColor(DRIVEN_COLOR);
        _update();
    }
}

void Dimension::makeDriven() { //if dimension is set, reset it
    _driving = false;
    _text.setFillColor(DRIVEN_COLOR);
}

bool Dimension::isDriving() const { return _driving; } //getter for _driving

void Dimension::setFont(const Font &f) { //called with main font object
    _text.setFont(f);
}

bool Dimension::contains(const Vector2f &pos) const { //whether click is within dimension text
    return _text.getGlobalBounds().contains(pos);
}

void Dimension::sendChar(char c) {
    if (c == '\n') { //parse and set value
        double newVal = getValue();
        if (_partial != L"") { //can't parse an empty string, but we still want to set the dimension
            try {
                newVal = stod(_partial);
            } catch (invalid_argument &) {wcout << "error: \"" << _partial << "\" couldn't be parsed\n";}
              catch(out_of_range &) {cout << "out_of_range in parsing\n";}
        }
        if (newVal < 0.0001) { //0 breaks solving
            cancelSelection();
        } else {
            _text.setFillColor(DRIVING_COLOR);
            _driving = false; //to allow modifying value
            setValue(newVal);
            _selected = false;
            _driving = true;
        }
        return;
    }
    if (c == '\b') { //backspace
        _partial.pop_back(); //delete last character
    } else {
        _partial.push_back(c); //push char to partial entry
    }
    if (_partial == L"") //update text shape
        _update(L"_");
    else
        _update(_partial);
}

Side::Side() {
    _scale = 1;
    _length = 1;
    _line.setOutlineColor(Color::White);
    _line.setOutlineThickness(1.125);
    _line.setSize(Vector2f(1, 0));
    _text.setCharacterSize(22);
    _textBG.setFillColor(Color::Black);
    _selected = false;
}
void Side::_update(const wstring &override) {
    wstringstream ss;
    if (override == "") {
        ss << setprecision(6) << _length;
    } else {
        ss << override;
    }
    ss << L"\"";
    _text.setString(ss.str());
    FloatRect bounds = _text.getLocalBounds(); //get bounds before '"'
    _text.setOrigin(bounds.width / 2.0, bounds.height / 2.0 + HEIGHT_FUDGE); //center text
    _textBG.setOrigin((bounds.width + 5) / 2.0, bounds.height / 2.0 + HEIGHT_FUDGE); //center text background
    _textBG.setSize(Vector2f(bounds.width + 5, bounds.height)); //size text background
}

Vector2f Side::_midpoint() const { //returns midpoint of line
    return _line.getTransform().transformPoint(Vector2f(_length*_scale/2.0, 0));
}

void Side::setDrawPos(const Vector2f& newPos, double newAng) { //where to draw side on screen
    _line.setRotation(newAng);
    _text.setRotation(newAng + (_flipText ? 180 : 0)); //flip if necessary
    _textBG.setRotation(newAng);
    
    _line.setSize(Vector2f(_length*_scale, 1));
    _line.setPosition(newPos);
    Vector2f midpoint = _midpoint();
    _text.setPosition(midpoint); //gets halfway point of line
    _textBG.setPosition(midpoint);
}
void Side::flipText(bool flip) {
    _flipText = flip;
    _text.setRotation(_line.getRotation() + (flip ? 180 : 0)); //flip so text appears right side up
}
void Side::setValue(double newLength) { //set driven dimension to solved value
    if (!_driving) {
        _length = newLength;
        _update();
    }
}
double Side::getValue() const { //gets current value (set or solved)
    return _length;
}
void Side::setScale(double newScale) { //sets scale of pixels/unit
    _line.setSize(Vector2f(_length * newScale, 1)); //scales line
    _scale = newScale;
}
void Side::draw(RenderTarget &target, RenderStates states) const { //draw all shapes
    target.draw(_line, states);
    target.draw(_textBG, states);
    target.draw(_text, states);
}


Angle::Angle() {
    _angle = 60;
    _text.setCharacterSize(22);
    _selected = false;
}
void Angle::_update(const wstring &override) {
    wstringstream ss;
    if (override == L"") {
        ss << setprecision(5) << _angle;
    } else {
        ss << override;
    }
    ss << L"°";
    _text.setString(ss.str());
    FloatRect bounds = _text.getLocalBounds(); //get bounds without degree symbol
    
    bool drawOut = _arc.getAngle() < 33 || _arc.getAngle() > 156; //angle too small or big to draw inside?
    double offset = drawOut ? 10 : 40;
    
    double xOrigin = (_flipText == drawOut) ? -offset : bounds.width + offset; //offset to left or right of text? --> | offset <texttexttext> offset |
    _text.setOrigin(xOrigin, bounds.height / 2.0 + HEIGHT_FUDGE); //fudge factor accounts for degree symbol
        
}
void Angle::flipText(bool flip) {
    if (_flipText == flip) return; //value not changed
    _flipText = flip;
    //_text.setRotation(_arc.getRotation() + (_flipText ? 180 : 0)); //flip so text appears right side up
    //_update();
}
void Angle::setDrawPos(const Vector2f& newPos, double newAng) { //pos to draw angle on screen
    _arc.setAngle(_angle); //update arc with current value
    _arc.setPosition(newPos);
    _text.setPosition(newPos);
    
    _arc.setRotation(newAng);
    _text.setRotation(newAng + (_flipText ? 180 : 0));
    _update();
}
void Angle::setValue(double newAng) { //update angle with solved value
    if (!_driving) { //don't override set value
        _angle = newAng;
        _update(); //recalculate text with new value
    }
}
double Angle::getValue() const { //get current value, solved or set
    return _angle;
}
void Angle::draw(RenderTarget &target, RenderStates states) const { //draw all subshapes
    target.draw(_text, states);
    target.draw(_arc, states);
}
