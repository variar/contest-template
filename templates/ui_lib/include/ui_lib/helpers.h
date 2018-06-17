#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include <future>

namespace ui
{

template<typename World>
std::future<void> RenderToFile(const std::string& filename, World& w)
{
    const auto worldSize = w.GetSize();

    sf::RenderTexture texture;
    texture.create(worldSize.x, worldSize.y);

    w.Draw(texture);
    texture.display();

    return std::async(std::launch::async, [texture = texture.getTexture(), filename]
    {
        sf::Image screeshot = texture.copyToImage();
        screeshot.saveToFile(filename);
    });
}


}