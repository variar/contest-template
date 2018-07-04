#pragma once

#include <memory>
#include <absl/types/optional.h>

#include <ui_lib/world.h>

namespace sf
{
    class Font;
    class RenderWindow;
    class View;
    class Event;
}

namespace ui
{

class UiFrame
{
public:
    UiFrame() = default;
    virtual ~UiFrame() = default;

    UiFrame(const UiFrame&) = delete;
    UiFrame(UiFrame&&) = delete;
    UiFrame& operator=(const UiFrame&) = delete;
    UiFrame& operator=(UiFrame&&) = delete;

    virtual void Run() = 0;

    static std::unique_ptr<UiFrame> MakeUi(
            uint32_t width,
            uint32_t height,
            const char* name,
            std::unique_ptr<World> world);
};




}
