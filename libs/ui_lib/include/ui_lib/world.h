#pragma once

#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <ui_lib/world_parameters.h>

namespace sf
{
    class RenderTarget;
}

namespace ui
{
    class World
    {
    public:
        virtual ~World() = default;

        virtual void SetFont(const std::shared_ptr<sf::Font>&) = 0;

        virtual sf::Vector2u GetSize() const = 0;

        virtual ui::WorldParameters GetParams() const = 0;
        virtual void SetParams(const ui::WorldParameters& params) = 0;

        virtual ui::WorldActions GetActions() = 0;
        
        virtual void ProcessEvent(const sf::Event& event, const sf::Vector2f& worldCoords) = 0;
        virtual void Draw(sf::RenderTarget&) = 0;

        virtual std::vector<std::string> GetLog() const = 0;
        virtual void ClearLog() = 0;
    };
}