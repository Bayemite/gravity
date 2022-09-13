#ifndef TRACE_H
#define TRACE_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>

#include <iostream>

class Trace : public sf::Drawable
{
    sf::VertexBuffer m_plot;
    sf::CircleShape m_dot;
    sf::Color m_color;
    size_t m_vertsPerCircle;
    size_t m_plotSize;
    size_t m_insertPoint;

protected:
    virtual void draw(sf::RenderTarget &target, const sf::RenderStates &states) const
    {
        target.draw(m_plot, states);
    }

public:
    Trace(size_t size, const sf::Color &color) : m_plot{sf::PrimitiveType::Triangles, sf::VertexBuffer::Stream},
                                                 m_dot{2, 16},
                                                 m_color{color},
                                                 m_insertPoint{0}
    {
        float r = m_dot.getRadius();
        m_dot.setOrigin({r, r});

        // So basically, I'm too lazy to calc my own vertices
        // so I use the vertexFan points given by the CircleShape class
        // And convert it to triangle primitives for batch drawing multiple circles
        m_vertsPerCircle = 3 * m_dot.getPointCount();
        m_plotSize = m_vertsPerCircle * 64; // 64 circles
        if (!m_plot.create(m_plotSize))
            std::cerr << "Failed to allocate tracing vertex buffer.\n";
    }

    void push(const sf::Vector2f &pos)
    {
        m_dot.setPosition(pos);

        sf::Vector2f center{m_dot.getPosition()};
        center.x += m_dot.getRadius();
        center.y += m_dot.getRadius();

        sf::VertexArray circle;

        sf::Vertex vert{center};
        circle.append(vert); // center

        int size = m_dot.getPointCount();
        for (int i = 0; i < size; i++)
        {
            vert.position = m_dot.getPosition() + m_dot.getPoint(i);
            circle.append(vert);
            vert.position = m_dot.getPosition() + m_dot.getPoint(i + 1);
            circle.append(vert);
            vert.position = center;
            circle.append(vert);
        }

        if (m_insertPoint + m_vertsPerCircle >= m_plotSize)
            m_insertPoint = 0;
        if (!m_plot.update(&circle[0], m_vertsPerCircle, m_insertPoint))
        {
            std::cerr << "Failed to update tracing vertex buffer.\n";
        }
        m_insertPoint += m_vertsPerCircle;
    }

    size_t getVertsPerCircle() const { return m_vertsPerCircle; }

    size_t getTotalVerts() const { return m_plotSize; }

    size_t getVertEndPoint() const { return m_insertPoint; }

    void setColor(const sf::Color &color)
    {
        m_color = color;
    }
};

#endif