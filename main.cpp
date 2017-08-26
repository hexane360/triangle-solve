/* CSCI 261 Final:
 *
 * Authors: Colin Gilgenbach    cgilgenbach     cgilgenbach@mines.edu
 *          Anli Ni             anlini          anlini@mines.edu
 * Description: Graphical triangle solver to find triangles given side lengths and angles
 *
 * Objective:
 * -Display the triangle
 * -Use classes to seperate code and represent data
 * -Use arrays to store data efficienty
 * -Develop large scale code incrementally
 * -Make use of SFML to create a complete graphical program
 */
#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Triangle.h"
#include "constants.h"

int main() {
    RenderWindow window(VideoMode::getFullscreenModes().at(0), "Triangle solver");
    window.setFramerateLimit(60);
    
    Triangle triangle;
    
    Event event;
    triangle.message("Instructions:\nClick dimensions to select them.\nType to set values."); //displays intro screen
    
    while (window.isOpen()) { //main draw loop
        window.clear();
        window.draw(triangle);
        window.display();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) { //window closed
                window.close();
            } else if (event.type == Event::Resized) {
            	Vector2f newSize(event.size.width, event.size.height);
            	triangle.resize(newSize); //tell triangle to redraw itself in new bounds
        		window.setView(View(Vector2f(0, 0), newSize)); //create correctly sized view centered at 0,0
            } else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Button::Left) { //LMB released somewhere in SFML window
				Vector2f point(event.mouseButton.x, event.mouseButton.y);
				Vector2f viewPoint = point - window.getView().getSize() / static_cast<float>(2.0); //transforms window points to view/triangle points
                triangle.clickAt(viewPoint); //send to triangle for further processing
            } else if (event.type == Event::KeyReleased) { //key released in SFML window
                switch (event.key.code) { //convert keyboard code into character for parsing
                    case Keyboard::Key::Num0: triangle.sendChar('0'); break;
                    case Keyboard::Key::Num1: triangle.sendChar('1'); break;
                    case Keyboard::Key::Num2: triangle.sendChar('2'); break;
                    case Keyboard::Key::Num3: triangle.sendChar('3'); break;
                    case Keyboard::Key::Num4: triangle.sendChar('4'); break;
                    case Keyboard::Key::Num5: triangle.sendChar('5'); break;
                    case Keyboard::Key::Num6: triangle.sendChar('6'); break;
                    case Keyboard::Key::Num7: triangle.sendChar('7'); break;
                    case Keyboard::Key::Num8: triangle.sendChar('8'); break;
                    case Keyboard::Key::Num9: triangle.sendChar('9'); break;
                    case Keyboard::Key::Period: triangle.sendChar('.'); break;
                    case Keyboard::Key::Return: triangle.sendChar('\n'); break;
                    case Keyboard::Key::BackSpace: triangle.sendChar('\b'); break;
                    case Keyboard::Key::Escape: triangle.cancelSelection(); break;
                    case Keyboard::Key::R: triangle.reset(); break; //resets triangle
                    case Keyboard::Key::C: triangle.clear(); break; //clears set dimensions
                    default: break;
                }
            }
        }
    }
    return 0;
}
