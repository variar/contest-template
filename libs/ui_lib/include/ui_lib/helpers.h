#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

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

class FileHelpers
{
    public:
    static std::string GetOpenFileName();
    static std::string GetSaveFileName();

};

}