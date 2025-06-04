#ifndef ZOOMABLE_CANVAS_HPP_
#define ZOOMABLE_CANVAS_HPP_

#include <string>
#include <functional>
#include <unordered_map>
#include "nanovg.h"
#include <imgui.h>

class ZoomableCanvas {
public:
    using DrawCallback = std::function<void(NVGcontext*)>;

    static void Draw(const std::string& name, NVGcontext* vg, DrawCallback draw);

private:
    struct State {
        float scale = 1.0f;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        bool dragging = false;
        ImVec2 lastMouse;
    };

    static std::unordered_map<std::string, State> states;
};

#endif // ZOOMABLE_CANVAS_HPP_