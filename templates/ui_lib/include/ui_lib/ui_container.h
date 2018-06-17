#pragma once
#include <plog/Log.h>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <SFML/Graphics/View.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>
#include <absl/types/optional.h>

#include <ui_lib/ui_parameters_wrappers.h>

#include <gsl/gsl_util.h>

namespace ui
{

template<typename WorldType>
class UiContainer
{
public:
    UiContainer(uint32_t windowWidth, uint32_t windowHeight, const char* windowName,
                std::unique_ptr<WorldType> world)
        : m_appWindow{sf::VideoMode{windowWidth, windowHeight}, windowName}
        , m_world{std::move(world)}
    {
        ImGui::SFML::Init(m_appWindow);
    }

    ~UiContainer()
    {
        ImGui::SFML::Shutdown();
    }

    UiContainer(const UiContainer&) = delete;
    UiContainer& operator=(const UiContainer&) = delete;
    UiContainer(UiContainer&&) = delete;
    UiContainer& operator=(UiContainer&&) = delete;

    void Run()
    {
        auto worldSize = m_world->GetSize();

        m_windowSize = m_appWindow.getSize();

        AdjustViewsToWindowSize(m_windowSize, worldSize);

        sf::Clock clock;

        while(m_appWindow.isOpen())
        {
            ProcessEvents();

            auto newWorldSize = m_world->GetSize();
            if (worldSize != newWorldSize)
            {
                worldSize = newWorldSize;
                AdjustViewsToWindowSize(m_appWindow.getSize(), worldSize);
            }

            ImGui::SFML::Update(m_appWindow, clock.restart());

            PrepareUi();

            m_appWindow.clear(sf::Color::Black);

            m_appWindow.setView(m_worldView);
            m_world->Draw(m_appWindow);

            DrawPreview();

            ImGui::SFML::Render(m_appWindow);

            m_appWindow.display();
        }

    }

private:
    enum class WindowArea
    {
        World,
        Ui,
        Panel,
        Preview
    };

    bool ProcessEvents()
    {
        sf::Event event;
        while (m_appWindow.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            PushEventToWorld(event);

            if( event.type == sf::Event::Closed ) {
               m_appWindow.close();
               return false;
            }

            switch (event.type)
            {
                case sf::Event::Resized:
                {
                    AdjustViewsToWindowSize(sf::Vector2u{event.size.width, event.size.height}, m_world->GetSize());
                    break;
                }

                case sf::Event::MouseWheelScrolled:
                {
                    ZoomViews(event.mouseWheelScroll);
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    m_mouseMoveOrigin.reset();
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    const auto position = sf::Vector2i{event.mouseButton.x, event.mouseButton.y};
                    const auto area = GetWindowArea(position);

                    if (area == WindowArea::Panel || area == WindowArea::Ui)
                    {
                        break;
                    }

                    if (event.mouseButton.button == sf::Mouse::Right && area == WindowArea::World)
                    {
                        m_mouseMoveOrigin = m_appWindow.mapPixelToCoords(position, m_worldView);
                    }
                    else if (area == WindowArea::Preview)
                    {
                        m_mouseMoveOrigin = m_appWindow.mapPixelToCoords(position, m_previewView);
                        m_worldView.setCenter(*m_mouseMoveOrigin);
                    }
                        
                    break;
                }

                case sf::Event::MouseMoved:
                {
                    if (!m_mouseMoveOrigin)
                        break;
                    
                    const auto position = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};
                    const auto area = GetWindowArea(position);

                    switch(area)
                    {
                        case WindowArea::Preview:
                        {
                            m_mouseMoveOrigin = m_appWindow.mapPixelToCoords(position, m_previewView);
                            m_worldView.setCenter(*m_mouseMoveOrigin);
                            break;
                        }
                        case WindowArea::World:
                        {
                            const auto worldCoords = m_appWindow.mapPixelToCoords(position, m_worldView);
                            const auto delta = *m_mouseMoveOrigin - worldCoords;
                            m_worldView.setCenter(m_worldView.getCenter() + delta);
                            m_mouseMoveOrigin = m_appWindow.mapPixelToCoords(position, m_worldView);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
        }

        return true;
    }

    void PushEventToWorld(const sf::Event& event) const
    {
        std::optional<sf::Vector2i> position;
        switch(event.type)
        {
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
            {
                position = sf::Vector2i{event.mouseButton.x, event.mouseButton.y};
                break;
            }

            case sf::Event::MouseMoved:
            {
                position = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};
                break;
            }

            default:
            break;
        }

        if (position)
        {
            const auto area = GetWindowArea(*position);
            if (area == WindowArea::World)
            {
                m_world->ProcessEvent(event, m_appWindow.mapPixelToCoords(*position, m_worldView));
            }
        }
    }

    void PrepareUi()
    {
        ImGuiWindowFlags flags =
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoTitleBar;

        const auto windowSize = m_appWindow.getSize();
        const auto lPanelSize = sf::Vector2u{WorldPreviewWidth, windowSize.y - WorldPreviewHeight};


        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSizeConstraints(lPanelSize, lPanelSize);
        ImGui::Begin("LPanel", nullptr, flags);
   
        ImGui::Text("Left panel");

        ImGui::PushItemWidth(lPanelSize.x*0.8);
        auto worldParams = m_world->GetParams();
        for (auto& param : worldParams)
        {
            absl::visit(ImGuiParamsVisitor(param.name, param.isReadOnly), param.value);
        }
        ImGui::PopItemWidth();

        m_world->SetParams(std::move(worldParams));

        const auto worldActions = m_world->GetActions();
        for (const auto& action : worldActions)
        {
            if (ImGui::Button(action.name.c_str()))
            {
                action.execute();
            }

        }

        ImGui::End();

        const auto panelPosition = sf::Vector2u{WorldPreviewWidth, windowSize.y - PanelHeight};
        const auto dPanelSize = sf::Vector2u{windowSize.x - WorldPreviewWidth, PanelHeight};
        ImGui::SetNextWindowPos(panelPosition);
        ImGui::SetNextWindowSizeConstraints(dPanelSize, dPanelSize);
        ImGui::Begin("DPanel", nullptr, flags);
        ImGui::Text("Bottom panel");
        ImGui::End();

    }

    void DrawPreview()
    {
        auto& originalView = m_appWindow.getView();

        m_appWindow.setView(m_previewView); 

        m_world->Draw(m_appWindow);

        const auto visibleWorldSize = m_worldView.getSize();
        const auto visibleWorldCenter = m_worldView.getCenter();
        auto visibleWorldFrame = sf::RectangleShape{visibleWorldSize};
        visibleWorldFrame.setPosition(visibleWorldCenter - (visibleWorldSize / 2.f));
        visibleWorldFrame.setOutlineColor(sf::Color::Magenta);
        visibleWorldFrame.setOutlineThickness(10);
        visibleWorldFrame.setFillColor(sf::Color::Transparent);

        m_appWindow.draw(visibleWorldFrame);

        m_appWindow.setView(originalView);
    }

private:
    void AdjustViewsToWindowSize(const sf::Vector2u& windowSize, const sf::Vector2u& worldSize)
    {
        static const sf::Vector2f previewSize {gsl::narrow_cast<float>(WorldPreviewWidth), gsl::narrow_cast<float>(WorldPreviewHeight)};
        const auto panelSize = sf::Vector2f{windowSize.x - previewSize.x, gsl::narrow_cast<float>(PanelHeight)};

        LOG_INFO << "old window size " << m_windowSize.x << " " << m_windowSize.y;
        LOG_INFO << "window size " << windowSize.x << " " << windowSize.y;


        const sf::Vector2i windowSizeDelta = {gsl::narrow_cast<int>(windowSize.x - m_windowSize.x), 
                                                gsl::narrow_cast<int>(windowSize.y - m_windowSize.y)};
        if (windowSizeDelta.x == 0 && windowSizeDelta.y == 0) {
            m_worldView.reset(sf::FloatRect(0, 0, windowSize.x - previewSize.x, windowSize.y - panelSize.y));
        }
        else 
        {
            const auto center = m_worldView.getCenter();
            auto worldViewSize = m_worldView.getSize();
            LOG_INFO << "old view size " << worldViewSize.x << " " << worldViewSize.y;

            worldViewSize.x += windowSizeDelta.x;
            worldViewSize.y += windowSizeDelta.y;

            LOG_INFO << "view size " << worldViewSize.x << " " << worldViewSize.y;

            m_worldView.setSize(worldViewSize);
            m_worldView.setCenter(center);
        }


        m_worldView.setViewport({previewSize.x/windowSize.x, 0,
                                1.0f - previewSize.x/windowSize.x, 1.0f - panelSize.y/windowSize.y});

        m_previewView.setSize(sf::Vector2f{gsl::narrow_cast<float>(worldSize.x), gsl::narrow_cast<float>(worldSize.y)});
        m_previewView.setCenter(m_previewView.getSize() / 2.f);

        m_previewView.setViewport({0, 1.0f-previewSize.y/windowSize.y,
                                   previewSize.x/windowSize.x, previewSize.y/windowSize.y});

        m_windowSize = windowSize;
    }

    void ZoomViews(const sf::Event::MouseWheelScrollEvent& scroll)
    {
        const auto factor = scroll.delta < 0 ? ZoomFactor : 1.f / ZoomFactor;
        const auto scrollOrigin = MapPixelToWorld({scroll.x, scroll.y});

        switch(GetWindowArea(scrollOrigin))
        {
            case WindowArea::Preview:
            case WindowArea::World:
                ZoomViewAt(scrollOrigin, factor);
                break;
            default:
                break;
        }
    }

    void ZoomViewAt(sf::Vector2i pixel, float zoom)
    {
        const auto beforeCoords = m_appWindow.mapPixelToCoords(pixel, m_worldView);
        m_worldView.zoom(zoom);

        const auto afterCoords =  m_appWindow.mapPixelToCoords(pixel, m_worldView);
        const auto offset = beforeCoords - afterCoords;
        m_worldView.move(offset);
    }

    sf::Vector2i MapPixelToWorld(sf::Vector2i pixel) const
    {
        if (GetWindowArea(pixel) == WindowArea::Preview)
        {
            const auto worldCoords = m_appWindow.mapPixelToCoords(pixel, m_previewView);
            pixel = m_appWindow.mapCoordsToPixel(worldCoords, m_worldView);
        }

        return pixel;
    }

    WindowArea GetWindowArea(const sf::Vector2i& pixel) const
    {
        const auto y = gsl::narrow_cast<unsigned>(pixel.y);
        if (pixel.x < WorldPreviewWidth)
        {
            return  (y > m_appWindow.getSize().y - WorldPreviewHeight) ?
                        WindowArea::Preview : WindowArea::Ui;
        }

        if (y > m_appWindow.getSize().y - PanelHeight) 
        {
            return WindowArea::Panel;
        }

        return WindowArea::World;
    }

private:
    sf::RenderWindow m_appWindow;
    absl::optional<sf::Vector2f> m_mouseMoveOrigin;
    sf::Vector2u m_windowSize;

    std::unique_ptr<WorldType> m_world;

    sf::View m_worldView;
    sf::View m_previewView;

private:
    static constexpr uint16_t PanelWidth = 220;
    static constexpr uint16_t WorldPreviewWidth = 200;
    static constexpr uint16_t WorldPreviewHeight = 200;
    static constexpr uint16_t PanelHeight = 100;
    static constexpr float ZoomFactor = 1.1;
};

}
