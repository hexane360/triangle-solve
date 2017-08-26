#include "Message.h"
using namespace std;

#include "constants.h"

Message::Message() {
    //set positions and sizes to fit in box/on screen
    //Vector2f midPoint = winSize / 2.0;
    //_box.setPosition(midPoint);
    _text.setPosition(Vector2f(0, -BOX_HEIGHT/2 + 10));
    _defaultText.setPosition(Vector2f(0, BOX_HEIGHT/2 - 15));
    _box.setSize(Vector2f(BOX_WIDTH, BOX_HEIGHT));
    _box.setOrigin(Vector2f(BOX_WIDTH/2, BOX_HEIGHT/2));
    _box.setFillColor(Color::Black);
    _box.setOutlineColor(Color::White);
    _box.setOutlineThickness(2);
    _text.setFillColor(Color::White);
    _text.setCharacterSize(18);
    _defaultText.setCharacterSize(16);
    _defaultText.setFillColor(Color::White);
    _defaultText.setString("Press R to reset triangle\nPress C to clear all dimensions\nPress ESC when selected to clear one\n\nPress enter to continue");
}

void Message::display(const String &newMsg) { //enable box and set message
    displayed = true;
    _text.setString(newMsg); //eventually add more here
    _text.setOrigin(_text.getLocalBounds().width / 2.0, 0); //centers text
}

void Message::setFont(const Font &f) {
    _text.setFont(f);
    _defaultText.setFont(f);
    _defaultText.setOrigin(_defaultText.getLocalBounds().width / 2.0, _defaultText.getLocalBounds().height); //centers text
}
void Message::draw(RenderTarget &target, RenderStates states) const { //fast draw loop
    if (displayed) {
        target.draw(_box, states);
        target.draw(_text, states);
        target.draw(_defaultText, states);
    }
}
bool Message::contains(const Vector2f &pos) const { //whether given point is inside error box
    return _box.getGlobalBounds().contains(pos);
}
