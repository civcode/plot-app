#include "PlotApp.h"
#include <math.h>

// #include <imgui.h>
// #include <implot.h>
// #include "nanovg.h"
int main() {
    PlotApp::Init(1280, 720);

    PlotApp::SetImGuiCallback([](){
        ImGui::Begin("Sine Plot");
        if (ImPlot::BeginPlot("Sine")) {
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

    PlotApp::AddPaintWindow("NanoVG Window", [](NVGcontext* vg){
        nvgBeginPath(vg);
        nvgCircle(vg, 100, 100, 50);
        nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));
        nvgFill(vg);
    });

    PlotApp::Run();
    PlotApp::Shutdown();
    return 0;
}
