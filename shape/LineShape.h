// https://github.com/SFML/SFML/wiki/source:-line-segment-with-thickness
#ifndef LINESHAPE_H
#define LINESHAPE_H

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

class LineShape : public sf::Shape
{
public:
    LineShape() : LineShape{{0.f, 0.f}, {0.f, 0.f}} {}
    explicit LineShape(const sf::Vector2f &point1, const sf::Vector2f &point2, float thickness=2.f);

    // Indices are only 0 & 1
    void setPoint(int index, const sf::Vector2f& point);
    
    void setThickness(float thickness);

    float getThickness() const;

    float getLength() const;

    const sf::Vector2f& getDirection() const;
    
    virtual size_t getPointCount() const override;

    virtual sf::Vector2f getPoint(size_t index) const override;

private:
    sf::Vector2f m_direction; ///< Direction of the line
    sf::Vector2f m_point1;
    sf::Vector2f m_point2;
    float m_thickness;        ///< Thickness of the line
};

#endif // LINESHAPE_H