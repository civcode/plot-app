#include "render_module/render_module.hpp"

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
    RenderModule::Init(1280, 720);

    // Combined ImGui callback that creates multiple windows
    RenderModule::SetImGuiCallback([]() {
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
    RenderModule::AddPaintWindow("NanoVG Canvas", [](NVGcontext* vg) {
        static float x = 0.0f;
        x += g_speed;
        if (x > 300.0f) x = 0.0f;

        nvg::SetContext(vg);
        nvg::BeginPath();
        nvg::Circle(x, 100, 40);
        nvg::FillColor(nvgRGBA(255, 100, 0, 255));
        nvg::Fill();
        // nvgBeginPath(vg);
        // nvgCircle(vg, x, 100, 40);
        // nvgFillColor(vg, nvgRGBA(255, 100, 0, 255));
        // nvgFill(vg);
    });


    // NanoVG paint window with persistent state
    static PaintState paint;

        // Register the paint window using state
    RenderModule::AddPaintWindow("NanoVG Canvas 2", [](NVGcontext* vg) {
        paint.draw(vg);
    });
    
    RenderModule::AddPaintWindow("NanoVG Canvas 3", [](NVGcontext* vg) {

        nvg::SetContext(vg);
        // nvg::Scale(1.5, 1.5);
        static int cnt;
        for (int i = 0; i < 10; ++i) {
            float angle = (i+(cnt++)/100.0f) * 2.0f * M_PI / 100.0f;
            float x = 100 + cosf(angle) * 100;
            float y = 100 + sinf(angle) * 100;

            nvg::BeginPath();
            nvg::SetShapeAntiAlias(true);
            nvg::Circle(x, y, 3);
            nvg::StrokeColor(nvg::RGBA(255, 0, 0, 255));
            nvgStrokeWidth(vg, 0.5);
            nvg::Stroke();
            // nvg::FillColor(nvgRGBA(0, 255, 0, 255));
            // nvg::Fill();
            nvg::ClosePath();

            nvg::BeginPath();
            // nvg::SetShapeAntiAlias(false);
            nvg::MoveTo(200, 200);
            nvg::LineTo(x, y);
            nvg::StrokeColor(nvg::RGBA(0, 255, 0, 255));
            nvg::Stroke();
            nvg::ClosePath();
        }
    });
        

    RenderModule::Run();
    RenderModule::Shutdown();
    return 0;
}
