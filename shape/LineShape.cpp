#include "LineShape.h"

#include <cassert>
#include <cmath>

using namespace sf;

LineShape::LineShape(const Vector2f &point1, const Vector2f &point2, float thickness) : m_direction(point2 - point1),
                                                                                        m_point1{point1},
                                                                                        m_point2{point2}
{
    setPosition(point1);
    setThickness(thickness);
}

void LineShape::setPoint(int index, const sf::Vector2f &point)
{
    assert(index == 0 || index == 1);
    if (index == 0)
    {
        m_point1 = point;
        m_direction = m_point2 - m_point1;
        setPosition(m_point1);
    }
    else
    {
        m_point2 = point;
        m_direction = m_point2 - m_point1;
    }
    update();
}

void LineShape::setThickness(float thickness)
{
    m_thickness = thickness;
    update();
}

float LineShape::getThickness() const
{
    return m_thickness;
}

float LineShape::getLength() const
{
    return std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
}

const sf::Vector2f& LineShape::getDirection() const
{
    return m_direction;
}

size_t LineShape::getPointCount() const
{
    return 4;
}

Vector2f LineShape::getPoint(size_t index) const
{
    Vector2f unitDirection = m_direction / getLength();
    Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

    Vector2f offset = (m_thickness / 2.f) * unitPerpendicular;

    switch (index)
    {
    default:
    case 0:
        return offset;
    case 1:
        return (m_direction + offset);
    case 2:
        return (m_direction - offset);
    case 3:
        return (-offset);
    }
}
