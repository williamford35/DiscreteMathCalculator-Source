#ifndef NATIVE_ELLIPTIC_POINT_PLOT_H
#define NATIVE_ELLIPTIC_POINT_PLOT_H

#include <wx/frame.h>
#include <wx/panel.h>

#include <utility>
#include <vector>

class NativeEllipticPointPlot : public wxFrame
{
public:
    NativeEllipticPointPlot(
        wxWindow* parent,
        long a,
        long b,
        long prime,
        const std::vector<std::pair<long, long>>& points);

private:
    class PlotPanel : public wxPanel
    {
    public:
        PlotPanel(
            wxWindow* parent,
            long prime,
            const std::vector<std::pair<long, long>>& points);

    private:
        void OnPaint(wxPaintEvent& event);

        long prime_;
        std::vector<std::pair<long, long>> points_;
    };
};

#endif
