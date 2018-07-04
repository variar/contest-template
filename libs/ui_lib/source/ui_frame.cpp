#include <ui_lib/ui_frame.h>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>

#include <plog/Log.h>

#include <gsl/gsl_util.h>

#include <ui_lib/font.h>
#include <ui_lib/imgui_visitor.h>

namespace ui
{

class UiFrameImpl : public UiFrame
{
public:
    UiFrameImpl(
            uint32_t windowWidth,
            uint32_t windowHeight,
            const char* windowName,
            std::unique_ptr<World> world);

    ~UiFrameImpl();
    
    void Run() override;

private:
    enum class WindowArea
    {
        World,
        Ui,
        Panel,
        Preview
    };

    bool ProcessEvents();
    void PushEventToWorld(const sf::Event& event) const;

    void PrepareUi();
    void DrawPreview();

    void AdjustViewsToWindowSize(const sf::Vector2u& windowSize, const sf::Vector2u& worldSize);
    void ZoomViews(const sf::Event::MouseWheelScrollEvent& scroll);
    void ZoomViewAt(sf::Vector2i pixel, float zoom);

    sf::Vector2i MapPixelToWorld(sf::Vector2i pixel) const;

    WindowArea GetWindowArea(const sf::Vector2i& pixel) const;

private:
    sf::RenderWindow m_appWindow;
    std::unique_ptr<World> m_world;

    std::shared_ptr<sf::Font> m_font;

    sf::Vector2u m_windowSize;

    absl::optional<sf::Vector2f> m_mouseMoveOrigin;

    sf::View m_worldView;
    sf::View m_previewView;

    ImGuiTextBuffer m_logBuffer;

private:
    static constexpr uint16_t PanelWidth = 220;
    static constexpr uint16_t WorldPreviewWidth = 200;
    static constexpr uint16_t WorldPreviewHeight = 200;
    static constexpr uint16_t PanelHeight = 100;
    static constexpr float ZoomFactor = 1.1;

}; 

std::unique_ptr<UiFrame> UiFrame::MakeUi(
            uint32_t width,
            uint32_t height,
            const char* name,
            std::unique_ptr<World> world)
{
    return std::make_unique<UiFrameImpl>(width, height, name, std::move(world));
}


UiFrameImpl::UiFrameImpl(
            uint32_t windowWidth,
            uint32_t windowHeight,
            const char* windowName,
            std::unique_ptr<World> world
)
    : m_appWindow{sf::VideoMode{windowWidth, windowHeight}, windowName}
    , m_world{std::move(world)}
    , m_font{std::make_shared<sf::Font>()}
{
    ImGui::SFML::Init(m_appWindow);

    if (!m_font->loadFromMemory(font::Data, font::Lenght))
    {
        LOG_ERROR << "Failed to load font";
    }
    else
    {
        m_world->SetFont(m_font);
    }

    m_appWindow.setVerticalSyncEnabled(true);
}

UiFrameImpl::~UiFrameImpl()
{
    ImGui::SFML::Shutdown();
}

void UiFrameImpl::Run()
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

bool UiFrameImpl::ProcessEvents()
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

void UiFrameImpl::PushEventToWorld(const sf::Event& event) const
{
    absl::optional<sf::Vector2i> position;
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

void UiFrameImpl::PrepareUi()
{
    ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_AlwaysAutoResize;

    const auto windowSize = m_appWindow.getSize();
    const auto lPanelSize = sf::Vector2u{WorldPreviewWidth, windowSize.y - WorldPreviewHeight};


    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSizeConstraints(lPanelSize, lPanelSize);
    ImGui::Begin("Parameters", nullptr, flags);

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


    m_logBuffer.clear();
    const auto logStrings = m_world->GetLog();
    for (const auto& s: logStrings) 
    {
        m_logBuffer.appendf("%s\n", s.c_str());
    }

    const auto panelPosition = sf::Vector2u{WorldPreviewWidth, windowSize.y - PanelHeight};
    const auto dPanelSize = sf::Vector2u{windowSize.x - WorldPreviewWidth, PanelHeight};
    ImGui::SetNextWindowPos(panelPosition);
    ImGui::SetNextWindowSizeConstraints(dPanelSize, dPanelSize);
    ImGui::Begin("Log", nullptr, flags);

    if (ImGui::Button("Clear"))
    {
        m_world->ClearLog();
        m_logBuffer.clear();


    }
    ImGui::Separator();

    ImGui::BeginChild("scrolling");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,1));
    ImGui::TextUnformatted(m_logBuffer.begin());
    ImGui::SetScrollHere(1.0f);
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::End();
}

void UiFrameImpl::DrawPreview()
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

void UiFrameImpl::AdjustViewsToWindowSize(const sf::Vector2u& windowSize, const sf::Vector2u& worldSize)
{
    static const sf::Vector2f previewSize {gsl::narrow_cast<float>(WorldPreviewWidth), gsl::narrow_cast<float>(WorldPreviewHeight)};
    const auto panelSize = sf::Vector2f{windowSize.x - previewSize.x, gsl::narrow_cast<float>(PanelHeight)};

    auto worldViewport = sf::FloatRect{previewSize.x/windowSize.x, 0,
                            1.0f - previewSize.x/windowSize.x, 1.0f - panelSize.y/windowSize.y};

    auto worldViewSize = sf::Vector2f{windowSize.x - previewSize.x, windowSize.y - panelSize.y};

    if (windowSize == m_windowSize)
    {
        m_worldView.reset({0, 0, worldViewSize.x, worldViewSize.y});
    }
    else 
    {
        auto oldViewSize = m_worldView.getSize();

        auto aspect = worldViewSize.y/worldViewSize.x;
        if(windowSize.x > windowSize.y)
        {
            worldViewSize.y = oldViewSize.y;
            worldViewSize.x = oldViewSize.y / aspect;
        }
        else 
        {
            worldViewSize.x = oldViewSize.x;
            worldViewSize.y = oldViewSize.x * aspect;
        }

        auto oldCenter = m_worldView.getCenter();
        auto centerPixel = m_appWindow.mapCoordsToPixel(oldCenter, m_worldView);

        m_worldView.setSize(worldViewSize);

        auto newCenter = m_appWindow.mapPixelToCoords(centerPixel, m_worldView);

        m_worldView.move(newCenter - oldCenter);
    }

    m_worldView.setViewport(worldViewport);

    m_previewView.setSize(sf::Vector2f{gsl::narrow_cast<float>(worldSize.x), gsl::narrow_cast<float>(worldSize.y)});
    m_previewView.setCenter(m_previewView.getSize() / 2.f);

    m_previewView.setViewport({0, 1.0f-previewSize.y/windowSize.y,
                                previewSize.x/windowSize.x, previewSize.y/windowSize.y});

    m_windowSize = windowSize;
}

void UiFrameImpl::ZoomViews(const sf::Event::MouseWheelScrollEvent& scroll)
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

void UiFrameImpl::ZoomViewAt(sf::Vector2i pixel, float zoom)
{
    const auto beforeCoords = m_appWindow.mapPixelToCoords(pixel, m_worldView);
    m_worldView.zoom(zoom);

    const auto afterCoords =  m_appWindow.mapPixelToCoords(pixel, m_worldView);
    const auto offset = beforeCoords - afterCoords;
    m_worldView.move(offset);
}

sf::Vector2i UiFrameImpl::MapPixelToWorld(sf::Vector2i pixel) const
{
    if (GetWindowArea(pixel) == WindowArea::Preview)
    {
        const auto worldCoords = m_appWindow.mapPixelToCoords(pixel, m_previewView);
        pixel = m_appWindow.mapCoordsToPixel(worldCoords, m_worldView);
    }

    return pixel;
}

UiFrameImpl::WindowArea UiFrameImpl::GetWindowArea(const sf::Vector2i& pixel) const
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

}