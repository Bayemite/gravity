#ifndef APP_H
#define APP_H

#include "circle_buffer.h"
#include "colors.h"
#include "gravity_point.h"
#include "sf_utils.h"

#include "shape/LineShape.h"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <vector>

class Trace : public sf::Drawable
{
    CircleBuffer<sf::CircleShape> m_plot;
    sf::Color m_color;

protected:
    virtual void draw(sf::RenderTarget &target, const sf::RenderStates &states) const
    {
        for (int i = 0; i < m_plot.size(); i++)
            target.draw(m_plot[i], states);
    }

public:
    Trace(size_t size, const sf::Color &color) : m_plot{size},
                                                 m_color{color}
    {
    }

    void push(const sf::Vector2f &pos)
    {
        sf::CircleShape dot{2, 16};
        dot.setOrigin({2.f, 2.f});
        dot.setPosition(pos);
        dot.setFillColor(m_color);
        m_plot.push(dot);

        for (int i = 0; i < m_plot.size(); i++)
        {
            sf::Color color = m_plot[i].getFillColor();
            color.a = 256 / m_plot.size() * i; // fade off, linear
            m_plot[i].setFillColor(color);
        }
    }

    void setColor(const sf::Color &color)
    {
        m_color = color;
    }
};


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

    void addGravPoint(const sf::Vector2f &coords, const sf::Vector2f &velocity);

    void handleEvents();
    void stepSim();

public:
    App();
    void run();
};

#endif