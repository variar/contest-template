#ifndef SFML_LINESHAPE_HPP
#define SFML_LINESHAPE_HPP

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>


namespace sf
{

class SFML_GRAPHICS_API LineShape : public Shape
{
    public:

    explicit LineShape(const Vector2f& point1, const Vector2f& point2);

    void setThickness(float thickness);

    float getThickness() const;

    float getLength() const;

    std::size_t getPointCount() const override;

    Vector2f getPoint(std::size_t index) const override;

    private:

    Vector2f m_direction; ///< Direction of the line
    float m_thickness;    ///< Thickness of the line
};

} // namespace sf


#endif // SFML_LINESHAPE_HPP