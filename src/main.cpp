#include "PlotApp.hpp"

#include <math.h>
#include <imgui.h>
#include <implot.h>
#include "nanovg.h"

// shared state
float g_speed = 1.0f;

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

    // Combined ImGui callback that creates multiple windows
    PlotApp::SetImGuiCallback([]() {
        // First window: Sine plot
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

        // Second window: Control panel
        ImGui::Begin("Control Panel");
        ImGui::SliderFloat("Speed", &g_speed, 0.1f, 10.0f);
        ImGui::End();
    });

    // Paint window: animated NanoVG circle
    PlotApp::AddPaintWindow("NanoVG Canvas", [](NVGcontext* vg) {
        static float x = 0.0f;
        x += g_speed;
        if (x > 300.0f) x = 0.0f;

        nvgBeginPath(vg);
        nvgCircle(vg, x, 100, 40);
        nvgFillColor(vg, nvgRGBA(255, 100, 0, 255));
        nvgFill(vg);
    });


    // NanoVG paint window with persistent state
    static PaintState paint;

        // Register the paint window using state
    PlotApp::AddPaintWindow("NanoVG Canvas 2", [](NVGcontext* vg) {
        paint.draw(vg);
    });
    
    PlotApp::AddPaintWindow("NanoVG Canvas 3", [](NVGcontext* vg) {
        // nvgBeginPath(vg);
        // for (int i = 0; i < 2; ++i) {
        //     float angle = i * 2.0f * M_PI / 100.0f;
        //     float x = 40 + cosf(angle) * 100;
        //     float y = 30 + sinf(angle) * 100;
        //     nvgCircle(vg, x, y, 10);
        // }
        // nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 255));
        // nvgStroke(vg);
        // // nvgFillColor(vg, nvgRGBA(0, 255, 0, 255));
        // // nvgFill(vg);

        static int cnt;
        for (int i = 0; i < 10; ++i) {
            float angle = (i+(cnt++)/100.0f) * 2.0f * M_PI / 100.0f;
            float x = 100 + cosf(angle) * 100;
            float y = 100 + sinf(angle) * 100;

            nvgBeginPath(vg);
            nvgCircle(vg, x, y, 3);
            nvgFillColor(vg, nvgRGBA(0, 255, 0, 255));
            nvgFill(vg);
        }
    });
        

    PlotApp::Run();
    PlotApp::Shutdown();
    return 0;
}
