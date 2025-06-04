#include "render_module/zoomable_canvas.hpp"
#include <imgui.h>
#include "render_module/nvg_wrapper.hpp"

std::unordered_map<std::string, ZoomableCanvas::State> ZoomableCanvas::states;

void ZoomableCanvas::Draw(const std::string& name, NVGcontext* vg, DrawCallback draw) {
    ImGui::Begin(name.c_str());
    State& state = states[name];

    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    if (canvasSize.x < 1.0f || canvasSize.y < 1.0f)
        canvasSize = ImVec2(300, 200);
    ImGui::InvisibleButton((name + "_canvas").c_str(), canvasSize,
                           ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

    bool hovered = ImGui::IsItemHovered();
    ImVec2 mouse = ImGui::GetIO().MousePos;
    ImVec2 mouseRel = ImVec2(mouse.x - canvasPos.x, mouse.y - canvasPos.y);

    // Zoom
    if (hovered) {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f) {
            float zoomFactor = (wheel > 0) ? 1.1f : (1.0f / 1.1f);
            float oldScale = state.scale;
            state.scale *= zoomFactor;
            state.scale = std::clamp(state.scale, 0.1f, 10.0f);


            // Adjust offset to zoom towards mouse
            state.offsetX -= (mouseRel.x / oldScale - mouseRel.x / state.scale);
            state.offsetY -= (mouseRel.y / oldScale - mouseRel.y / state.scale);
            printf("Zoom %.2f Offset (%.2f, %.2f)\n", state.scale, state.offsetX, state.offsetY);

        }
    }

    // Pan
    if (hovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        state.offsetX += delta.x / state.scale;
        state.offsetY += delta.y / state.scale;
    }

    // Draw with NanoVG
    nvg::SetContext(vg);
    nvg::Save();
    nvg::Translate(canvasPos.x, canvasPos.y);
    nvg::Scissor(canvasPos.x, canvasPos.y, canvasSize.x, canvasSize.y);
    nvg::Translate(state.offsetX * state.scale, state.offsetY * state.scale);
    nvg::Scale(state.scale, state.scale);

    draw(vg);

    nvg::ResetScissor();
    nvg::Restore();

    ImGui::End();
}
