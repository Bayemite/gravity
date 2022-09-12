#ifndef APP_H
#define APP_H

#include "circle_buffer.h"
#include "colors.h"
#include "gravity_point.h"
#include "sf_utils.h"
#include "trace.h"

#include "shape/LineShape.h"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <vector>


class App
{
    sf::RenderWindow window;
    sf::View view;
    float viewZoom; // view doesn't track zoom... come on SFML. TODO: combine into zoom class
    sf::View plainView;

    sf::Vector2f mouseCoords;
    sf::Vector2i mousePixelCoords;
    bool leftClicked;
    sf::Vector2f lClickCoords;

    bool newGravPoint;
    LineShape m_newGravPointVec;

    std::vector<GravityPoint> gravPoints;

    std::vector<Trace> traces;
    // sf::Shader m_alphaFadeOff;


    void addGravPoint(const sf::Vector2f &coords, const sf::Vector2f &velocity);

    void handleEvents();
    void stepSim();

public:
    App();
    void run();
};

#endif