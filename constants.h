#pragma once
#include <SFML/Graphics.hpp>

static const int BOX_WIDTH = 360; //Message box dimensions
static const int BOX_HEIGHT = 240;

static const double PI = 3.1415926;
static const double TO_RADS = PI / 180.0;
static const double TO_DEGS = 180.0 / PI;

static const sf::Color DRIVEN_COLOR(0, 153, 255);    //blue, for unset dimensions
static const sf::Color DRIVING_COLOR(255, 51, 51);   //red, for set dimension
static const sf::Color SELECTED_COLOR(255, 255, 51); //yellow, for dimensions mid-entry
