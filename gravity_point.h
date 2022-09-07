#ifndef GRAVITY_POINT_H
#define GRAVITY_POINT_H

#include <SFML/Graphics.hpp>

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
    GravityPoint(const sf::Vector2f &pos) : m_circle{16.f, 256},
                                            m_mass{10.f},
                                            m_velocity{}
    {
        m_circle.setPosition(pos);
        setRadius(64.f);
    }

    void setMass(float mass)
    {
        m_mass = mass;
    }

    void setInitialVelocity(const sf::Vector2f &velocity)
    {
        m_velocity = velocity;
    }

    void updateVelocity(const GravityPoint &affector)
    {
        // https://en.wikipedia.org/wiki/Gravity
        sf::Vector2f pos = m_circle.getPosition();
        sf::Vector2f affectorPos = affector.m_circle.getPosition();
        sf::Vector2f diff = affectorPos - pos;
        float distance = sqrt(pow(diff.y, 2) + pow(diff.x, 2));

        // TODO: Units are magic, not determined yet
        float gConstant = 0.1; // 6.674×10^−11 m^3⋅kg^−1⋅s^−2
        // https://physics.stackexchange.com/questions/17285/split-gravitational-force-into-x-y-and-z-componenets
        float a = gConstant * m_mass * affector.m_mass;
        float b = pow(distance, 2);
        float forceX = diff.x * a / b;
        float forceY = diff.y * a / b;
        m_velocity.x += forceX / m_mass;
        m_velocity.y += forceY / m_mass;
    }

    void updatePosition()
    {
        m_circle.move(m_velocity);
    }

    const sf::Vector2f &getPosition()
    {
        return m_circle.getPosition();
    }

    // Appearance API
    void setFillColor(const sf::Color &color)
    {
        m_circle.setFillColor(color);
    }
    
    void setRadius(float radius)
    {
        m_circle.setRadius(radius);
        m_circle.setOrigin({radius, radius});
    }

    const sf::Color& getFillColor()
    {
        return m_circle.getFillColor();
    }
};

#endif