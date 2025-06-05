#ifndef ZOOM_VIEW_HPP_
#define ZOOM_VIEW_HPP_
 
#include <nanovg.h>
#include <imgui.h>
#include <string>
#include <functional>

namespace ZoomView {

/// Draws a zoomable and pannable canvas.
/// Internally stores zoom and pan state by label.
///
/// @param label Unique identifier for the zoom view instance.
/// @param vg    NanoVG context for rendering.
/// @param drawCallback A lambda or function that performs NanoVG drawing commands.
void Draw(const std::string& label, NVGcontext* vg, std::function<void(NVGcontext*)> drawCallback);

} // namespace ZoomView

#endif // ZOOM_VIEW_HPP_