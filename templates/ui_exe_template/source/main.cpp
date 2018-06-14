#include <dll_template/dummy_dll.h>
#include <dll_template/log.h>

#include <plog/Appenders/ConsoleAppender.h>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

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

int main(int argc, char *argv[])
{
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    dll_template_log::initializePlog(plog::debug, plog::get());

    DummyDll dll;
    
    sf::RenderWindow app_window(sf::VideoMode(800, 600), "App window");
    app_window.setFramerateLimit(25);

    ImGui::SFML::Init(app_window);

    sf::Event event;
    sf::Clock clock;

    auto miniViewSize = 150.0f;
    auto zoomFactor = 1.1f;


    sf::View worldView(sf::FloatRect(0, 0, app_window.getSize().x-miniViewSize, app_window.getSize().y));
    worldView.setViewport(sf::FloatRect(0,0, 1-miniViewSize/event.size.width, 1.0f));

    sf::View miniView(sf::Vector2f(500, 500), sf::Vector2f(1000, 1000));
    
    miniView.setViewport(sf::FloatRect(1-miniViewSize/app_window.getSize().x, 0, 
    miniViewSize/app_window.getSize().x, miniViewSize/app_window.getSize().y));

    std::vector<sf::CircleShape> circles;


    auto drawWorld = [&]()
    {
        bool isBlack = true;

         for (auto x=0; x<500; x+=25) {
            for (auto y=0; y<500; y+=25) {
                sf::RectangleShape rectangle(sf::Vector2f(25, 25));
                rectangle.setFillColor(isBlack ?  sf::Color::Black : sf::Color::Blue);
                isBlack = !isBlack;
                rectangle.setPosition(x, y);
                app_window.draw(rectangle);
            }
            isBlack = !isBlack;
        }

        for (auto x=500; x<1000; x+=25) {
            for (auto y=500; y<1000; y+=25) {
                sf::RectangleShape rectangle(sf::Vector2f(25, 25));
                rectangle.setFillColor(isBlack ?  sf::Color::Green : sf::Color::Red);
                isBlack = !isBlack;
                rectangle.setPosition(x, y);
                app_window.draw(rectangle);
            }
            isBlack = !isBlack;
        }

        for (auto& c : circles) {
            app_window.draw(c);
        }
    };

    bool moving = false;
    sf::Vector2f dragStart;


    while( app_window.isOpen() ) {
        // Event processing.
        while( app_window.pollEvent( event ) ) {
            ImGui::SFML::ProcessEvent(event);

            // If window is about to be closed, leave program.
            if( event.type == sf::Event::Closed ) {
               app_window.close();
            }

            switch (event.type) {
                case sf::Event::Resized:
                {
                    // update the view to the new size of the window
                    sf::FloatRect visibleArea(0, 0, event.size.width-miniViewSize, event.size.height);
                    worldView = sf::View(visibleArea);
                    worldView.setViewport(sf::FloatRect(0,0, 1-miniViewSize/event.size.width, 1.0f));
                    
                    miniView.setViewport(sf::FloatRect(1-miniViewSize/event.size.width, 0, 
                        miniViewSize/event.size.width, miniViewSize/event.size.height));
                    break;
                }

                case sf::Event::MouseWheelScrolled:
                {
                    auto factor = zoomFactor;
                    
                    if (event.mouseWheelScroll.delta > 0)
                        factor = (1.f / zoomFactor);

                    auto zoomPos = sf::Vector2i{event.mouseWheelScroll.x, event.mouseWheelScroll.y};
                     if (event.mouseWheelScroll.x > app_window.getSize().x - miniViewSize && 
                        event.mouseWheelScroll.y < miniViewSize) {
                            auto worldCoords = app_window.mapPixelToCoords(zoomPos, miniView);
                            zoomPos = app_window.mapCoordsToPixel(worldCoords, worldView);
                        }
                    
                        worldView = zoomViewAt(zoomPos, app_window, worldView, factor);
                }
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == 1) {
                        auto worldCoords = app_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), worldView);
                        auto circle = sf::CircleShape(10);
                        circle.setFillColor(sf::Color::Red);
                        circle.setPosition(worldCoords - sf::Vector2f{10.f, 10.f});
                        circles.push_back(circle);
                        break;
                    }
                    if (event.mouseButton.x > app_window.getSize().x - miniViewSize && 
                        event.mouseButton.y < miniViewSize) {
                            auto worldCoords = app_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), miniView);
                            worldView.setCenter(worldCoords);
                            moving = true;
                        }
                    else if (event.mouseMove.x < app_window.getSize().x - miniViewSize)
                    {
                        dragStart = app_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), worldView);
                        moving = true;

                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    moving = false;
                    break;

                case sf::Event::MouseMoved:
                {
                    if (!moving)
                        break;

                    sf::Vector2f worldCoords;
                    if (event.mouseMove.x > app_window.getSize().x - miniViewSize && 
                        event.mouseMove.y < miniViewSize) {
                            worldCoords = app_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), miniView);
                            worldView.setCenter(worldCoords);

                    }
                    else if (event.mouseMove.x < app_window.getSize().x - miniViewSize)
                    {
                        auto newPos = app_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), worldView);
                        auto delta = dragStart - newPos;
                        worldCoords = worldView.getCenter() + delta;
                        worldView.setCenter(worldCoords);
                        dragStart = app_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), worldView);
                    }

                    break;
                    
                }
                    

                default:
                break;
            }
        }

        ImGui::SFML::Update(app_window, clock.restart());

        ImGuiWindowFlags corner =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("Hello, world!", nullptr, corner);
        ImGui::Button("Look at this pretty button");
        int myInt;
        ImGui::InputInt("Some int", &myInt);
        ImGui::SetWindowPos(ImVec2(app_window.getSize().x - miniViewSize, miniViewSize));
        ImGui::End();

        ImGui::ShowTestWindow();

        app_window.clear(sf::Color::White);

        app_window.setView(worldView);
        drawWorld();

        app_window.setView(miniView);
        drawWorld();
        sf::RectangleShape visibleWorld(worldView.getSize());
        visibleWorld.setPosition(worldView.getCenter().x - worldView.getSize().x/2,
         worldView.getCenter().y - worldView.getSize().y/2);
        visibleWorld.setOutlineColor(sf::Color::Magenta);
        visibleWorld.setOutlineThickness(10);
        visibleWorld.setFillColor(sf::Color::Transparent);
        app_window.draw(visibleWorld);


        ImGui::SFML::Render(app_window);

        app_window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
