#include "PlotApp.hpp"

#include <math.h>
#include <imgui.h>
#include <implot.h>
#include "nanovg.h"

struct PaintState {
    float x = 0.0f;
    float speed = 1.0f;

    void update() {
        x += speed;
        if (x > 300.0f) x = 0.0f;
    }

    void draw(NVGcontext* vg) {
        update();

        nvgBeginPath(vg);
        nvgCircle(vg, x, 100, 40);
        nvgFillColor(vg, nvgRGBA(255, 100, 0, 255));
        nvgFill(vg);
    }
};

int main() {
    PlotApp::Init(1280, 720);

    // Basic ImPlot window
    PlotApp::SetImGuiCallback([]() {
        ImGui::Begin("Sine Plot");
        if (ImPlot::BeginPlot("Sine Wave")) {
            static float x[100], y[100];
            for (int i = 0; i < 100; ++i) {
                x[i] = i * 0.1f;
                y[i] = sinf(x[i]);
            }
            ImPlot::PlotLine("sin(x)", x, y, 100);
            ImPlot::EndPlot();
        }
        ImGui::End();
    });

    // NanoVG paint window with persistent state
    static PaintState paint;

    // Add an ImGui UI to control paint state
    PlotApp::SetImGuiCallback([]() {
        ImGui::Begin("Control Panel");
        ImGui::SliderFloat("Speed", &paint.speed, 0.1f, 10.0f);
        ImGui::End();
    });

    // Register the paint window using state
    PlotApp::AddPaintWindow("NanoVG Canvas", [](NVGcontext* vg) {
        paint.draw(vg);
    });

    PlotApp::Run();
    PlotApp::Shutdown();
    return 0;
}
