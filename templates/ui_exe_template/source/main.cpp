#include <dll_template/dummy_dll.h>
#include <dll_template/log.h>

#include <plog/Appenders/ConsoleAppender.h>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <ui_lib/ui_container.h>
#include <ui_lib/helpers.h>


#include <absl/debugging/failure_signal_handler.h>
#include <absl/debugging/symbolize.h>

sf::View zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window,  sf::View view, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel, view) };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
    return  view;
}

class World 
{
public:
    World() : m_circleRadius{10.f}
    {
    }

    sf::Vector2u GetSize()
    {
        return {1000, 1000};
    }

    void Draw(sf::RenderTarget& renderTarget)
    {
        bool isBlack = true;

        sf::RectangleShape background{sf::Vector2f{GetSize()}};
        background.setPosition(0,0);
        background.setFillColor(sf::Color::White);
        renderTarget.draw(background);

        for (auto x=0; x<500; x+=25) {
            for (auto y=0; y<500; y+=25) {
                sf::RectangleShape rectangle(sf::Vector2f(25, 25));
                rectangle.setFillColor(isBlack ?  sf::Color::Black : sf::Color::Blue);
                isBlack = !isBlack;
                rectangle.setPosition(x, y);
                renderTarget.draw(rectangle);
            }
            isBlack = !isBlack;
        }

        for (auto x=500; x<1000; x+=25) {
            for (auto y=500; y<1000; y+=25) {
                sf::RectangleShape rectangle(sf::Vector2f(25, 25));
                rectangle.setFillColor(isBlack ?  sf::Color::Green : sf::Color::Red);
                isBlack = !isBlack;
                rectangle.setPosition(x, y);
                renderTarget.draw(rectangle);
            }
            isBlack = !isBlack;
        }

        for (const auto& c : m_circles) {
            renderTarget.draw(c);
        }
    }

    void ProcessEvent(const sf::Event& event, sf::Vector2f worldCoords)
    {
        const auto isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) 
        {
            if (isShiftPressed)
            {
                auto circle = std::find_if(m_circles.begin(), m_circles.end(),
                [&worldCoords](const auto& c)
                {
                    return c.getGlobalBounds().contains(worldCoords);
                });
                if (circle != m_circles.end())
                {
                    m_selectedCircle = circle;
                }
            }
            else
            {
                auto circle = sf::CircleShape(m_circleRadius);
                circle.setFillColor(sf::Color::Red);
                circle.setPosition(worldCoords - sf::Vector2f{m_circleRadius, m_circleRadius});
                m_circles.push_back(circle);
                m_selectedCircle = m_circles.end();
            }
        }

        if (event.type == sf::Event::MouseMoved)
        {

            if (m_selectedCircle != m_circles.end())
            {
                const auto radius = m_selectedCircle->getRadius();
                m_selectedCircle->setPosition(worldCoords - sf::Vector2f{radius, radius});
            }

            if (!isShiftPressed)
            {
                m_selectedCircle = m_circles.end();
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (m_selectedCircle != m_circles.end())
            {
                const auto radius = m_selectedCircle->getRadius();
                m_selectedCircle->setPosition(worldCoords - sf::Vector2f{radius, radius});
            }
            m_selectedCircle = m_circles.end();
        }
    }

    ui::WorldParameters GetParams()
    {
        ui::WorldParameters params;
        params.emplace_back("Circle radius", m_circleRadius);

        return params;
    }

    void SetParams(ui::WorldParameters&& params)
    {
        m_circleRadius = ui::GetParamByName<float>(params, "Circle radius");
    }

    ui::WorldActions GetActions()
    {
        return { 
                    {"Clear", [this]{ m_circles.clear(); } },
                    {"Sceenshot", [this]
                    {
                        m_screenshotFutures.emplace_back(ui::RenderToFile("test.png", *this));
                    }}
                };

    }

private:
    std::vector<sf::CircleShape> m_circles;
    std::vector<sf::CircleShape>::iterator m_selectedCircle;

    float m_circleRadius;

    std::vector<std::future<void>> m_screenshotFutures;

};

int main(int argc, char *argv[])
{
    absl::InitializeSymbolizer(argv[0]);
    absl::FailureSignalHandlerOptions options;
    absl::InstallFailureSignalHandler(options);

    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    ui::UiContainer<World> container{1280, 720, "Ui template", std::make_unique<World>()};
    container.Run();

    return 0;
}
