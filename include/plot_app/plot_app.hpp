#ifndef PLOTAPP_HPP_
#define PLOTAPP_HPP_

#include <functional>
#include <vector>
#include <string>

#include <imgui.h>
#include <implot.h>
#include "nanovg.h"
#include "nvg_wrapper.hpp"

struct NVGcontext;

class PlotApp {
public:
    static void Init(int width, int height);
    static void SetImGuiCallback(std::function<void()> callback);
    static void AddPaintWindow(const std::string& name, std::function<void(NVGcontext*)> callback);
    static void Run();
    static void Shutdown();
};

#endif // PLOTAPP_HPP_