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
    sf::VertexArray m_plot;
    sf::Color m_color;

protected:
    virtual void draw(sf::RenderTarget &target, const sf::RenderStates &states) const
    {
        target.draw(m_plot, states);
    }

public:
    Trace(size_t size, const sf::Color &color) : m_plot{sf::PrimitiveType::Triangles},
                                                 m_color{color}
    {
    }

    void push(const sf::Vector2f &pos)
    {
        sf::CircleShape dot{2, 16};
        dot.setOrigin({2.f, 2.f});
        dot.setPosition(pos);

        sf::Vector2f center{dot.getPosition()};
        center.x += dot.getRadius();
        center.y += dot.getRadius();

        sf::Vertex vert{center};
        m_plot.append(vert); // center

        int size = dot.getPointCount();
        for (int i = 0; i < size; i++)
        {
            vert.position = dot.getPosition() + dot.getPoint(i);
            m_plot.append(vert);
            vert.position = dot.getPosition() + dot.getPoint(i + 1);
            m_plot.append(vert);
            vert.position = center;
            m_plot.append(vert);
        }
        vert.position = dot.getPosition()+dot.getPoint(size-1);
        m_plot.append(vert);
        vert.position = dot.getPosition()+dot.getPoint(0);
        m_plot.append(vert);
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

    sf::Shader shader;

    void addGravPoint(const sf::Vector2f &coords, const sf::Vector2f &velocity);

    void handleEvents();
    void stepSim();

public:
    App();
    void run();
};

#endif