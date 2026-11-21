#ifndef NATIVE_ELLIPTIC_PLOT_H
#define NATIVE_ELLIPTIC_PLOT_H

#include <wx/frame.h>
#include <wx/panel.h>

class NativeEllipticPlot : public wxFrame
{
public:
    NativeEllipticPlot(wxWindow* parent,
                       double a, double b,
                       double xmin, double xmax,
                       double ymin, double ymax);

private:
    class PlotPanel : public wxPanel
    {
    public:
        PlotPanel(wxWindow* parent,
                  double a, double b,
                  double xmin, double xmax,
                  double ymin, double ymax);

    private:
        void OnPaint(wxPaintEvent& event);

        double a_, b_;
        double xmin_, xmax_, ymin_, ymax_;
    };
};

#endif
