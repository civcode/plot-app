#include "render_module/zoom_view.hpp"
#include <unordered_map>

namespace ZoomView {

struct State {
    float zoom = 1.0f;
    ImVec2 offset = {0.0f, 0.0f};
};

static std::unordered_map<std::string, State> stateMap;

void Draw(const std::string& label, NVGcontext* vg, std::function<void(NVGcontext*)> drawCallback) {
    State& state = stateMap[label];

    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    ImGui::InvisibleButton(("##" + label).c_str(), canvasSize,
        ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    bool hovering = ImGui::IsItemHovered();
    ImVec2 mouse = ImGui::GetIO().MousePos;
    ImVec2 delta = ImGui::GetIO().MouseDelta;

    if (hovering) {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f) {
            float zoomFactor = 1.1f;
            float prevZoom = state.zoom;
            state.zoom *= (wheel > 0) ? zoomFactor : (1.0f / zoomFactor);
            ImVec2 mouseInCanvas = ImVec2(mouse.x - canvasPos.x, mouse.y - canvasPos.y);
            state.offset.x -= mouseInCanvas.x * (state.zoom - prevZoom) / state.zoom;
            state.offset.y -= mouseInCanvas.y * (state.zoom - prevZoom) / state.zoom;
        }
    }

    if (hovering && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        state.offset.x += delta.x;
        state.offset.y += delta.y;
    }

    // Actual rendering with zoom and pan
    nvgBeginFrame(vg, canvasSize.x, canvasSize.y, 1.0f);
    nvgSave(vg);
    nvgTranslate(vg, state.offset.x, state.offset.y);
    nvgScale(vg, state.zoom, state.zoom);
    drawCallback(vg);
    nvgRestore(vg);
    nvgEndFrame(vg);
}

} // namespace ZoomView



