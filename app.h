#ifndef APP_H
#define APP_H

#include "sf_utils.h"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <vector>

class GravityPoint : public sf::Drawable
{
    sf::CircleShape m_circle;
    sf::Vector2f m_velocity;
    float m_mass;

protected:
    virtual void draw(sf::RenderTarget &target, const sf::RenderStates &states) const
    {
        target.draw(m_circle, states);
    }

public:
    GravityPoint(const sf::Vector2f &pos) : m_circle{50.f, 64},
                                            m_mass{10.f},
                                            m_velocity{}
    {
        m_circle.setPosition(pos);
    }

    void setMass(float mass)
    {
        m_mass=mass;
    }

    void setInitialVelocity(const sf::Vector2f &velocity)
    {
        m_velocity = velocity;
    }

    void updateVelocity(const GravityPoint &affector)
    {
        // https://en.wikipedia.org/wiki/Gravity
        sf::Vector2f pos = center(m_circle.getGlobalBounds());
        sf::Vector2f affectorPos = center(affector.m_circle.getGlobalBounds());
        sf::Vector2f diff = pos - affectorPos;
        float distance = sqrt(pow(diff.y, 2) + pow(diff.x, 2));

        // TODO: Units are magic, not determined yet
        int gConstant = 6; // 6.674×10^−11 m^3⋅kg^−1⋅s^−2
        // https://physics.stackexchange.com/questions/17285/split-gravitational-force-into-x-y-and-z-componenets
        float forceX = diff.x * gConstant * m_mass * affector.m_mass / pow(distance, 2);
        float forceY = diff.y * gConstant * m_mass * affector.m_mass / pow(distance, 2);
        m_velocity.x += forceX / m_mass;
        m_velocity.y += forceY / m_mass;
    }

    void updatePosition()
    {
        m_circle.move(m_velocity);
    }

    // Appearance API
    void setFillColor(const sf::Color &color)
    {
        m_circle.setFillColor(color);
    }
    void setRadius(float radius)
    {
        m_circle.setRadius(radius);
    }
};

class App
{
    sf::RenderWindow window;
    sf::View view;
    float viewZoom; // view doesn't track zoom... why??

    sf::Vector2f mouseCoords;
    sf::Vector2i mousePixelCoords;
    bool leftClicked;

    std::vector<GravityPoint> gravPoints;

    void handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::KeyPressed:
            {
                static sf::Vector2i cart_coords;
                auto &data{event.key};
                switch (data.code)
                {
                default:
                    break;
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                auto &data{event.mouseButton};
                mousePixelCoords = sf::Vector2i(data.x, data.y);
                mouseCoords = window.mapPixelToCoords(sf::Vector2i(data.x, data.y), view);
                if (data.button == sf::Mouse::Left)
                    leftClicked = true;
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                auto &data{event.mouseButton};
                if (data.button == sf::Mouse::Left)
                    leftClicked = false;
                break;
            }
            case sf::Event::MouseMoved:
            {
                auto &data{event.mouseMove};
                sf::Vector2f coords{window.mapPixelToCoords(sf::Vector2i(data.x, data.y), view)};
                if (leftClicked)
                {
                    sf::Vector2f offset{mousePixelCoords - sf::Vector2i(data.x, data.y)};
                    view.move(offset * viewZoom);
                    window.setView(view);
                }
                mousePixelCoords = sf::Vector2i(data.x, data.y);
                mouseCoords = coords;
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                auto &data{event.mouseWheelScroll};
                float factor = 1 + (-data.delta / 3);
                viewZoom *= factor;
                zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, window, factor, view);
                break;
            }
            case sf::Event::Resized:
            {
                auto &data{event.size};
                view.setSize(sf::Vector2f(data.width, data.height));
                view.reset({{0.f, 0.f}, sf::Vector2f(data.width, data.height)});
                window.setView(view);
                break;
            }
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }

    void stepSim()
    {
        std::size_t size = gravPoints.size();
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (i != j)
                    gravPoints[i].updateVelocity(gravPoints[j]);

        for (GravityPoint &p : gravPoints)
            p.updatePosition();
    }

public:
    App() : window{sf::VideoMode({800, 600}), "Gravity", sf::Style::Default, sf::ContextSettings(0, 0, 4)},
            view{sf::FloatRect(sf::Vector2f(0, 0), window.getView().getSize())},
            leftClicked{false},
            viewZoom{1.f}
    {
        window.setFramerateLimit(60);
        gravPoints.push_back(GravityPoint({0.f, 0.f}));
        gravPoints.push_back(GravityPoint(center(window)));
        
        gravPoints[0].setFillColor(sf::Color::Red);
        gravPoints[0].setInitialVelocity({10.f, 0.f});
        gravPoints[0].setMass(1.f);
        gravPoints[0].setRadius(10.f);

        gravPoints[1].setFillColor(sf::Color::Blue);
        gravPoints[1].setMass(100.f);
    }

    void run()
    {
        while (window.isOpen())
        {
            handleEvents();
            stepSim();
            window.clear(sf::Color::Black);

            for (int i = 0; i < gravPoints.size(); i++)
                window.draw(gravPoints[i]);
            window.display();
        }
    }
};

#endif