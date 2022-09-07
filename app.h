#ifndef APP_H
#define APP_H

#include "colors.h"
#include "gravity_point.h"
#include "sf_utils.h"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <vector>

class App
{
    sf::RenderWindow window;
    sf::View view;
    float viewZoom; // view doesn't track zoom... why?? TODO: combine into zoom class

    sf::Vector2f mouseCoords;
    sf::Vector2i mousePixelCoords;
    bool leftClicked;

    std::vector<GravityPoint> gravPoints;

    void handleEvents();
    void stepSim();

public:
    App();
    void run();
};

#endif