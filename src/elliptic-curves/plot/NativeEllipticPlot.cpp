#include "elliptic-curves/plot/NativeEllipticPlot.h"

#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/sizer.h>

#include <algorithm>
#include <cmath>
#include <memory>

NativeEllipticPlot::NativeEllipticPlot(
    wxWindow* parent,
    double a, double b,
    double xmin, double xmax,
    double ymin, double ymax)
    : wxFrame(parent, wxID_ANY,
              "Elliptic Curve Plot",
              wxDefaultPosition, wxSize(850, 650),
              wxDEFAULT_FRAME_STYLE)
{
    auto* panel = new PlotPanel(
        this, a, b, xmin, xmax, ymin, ymax);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel, 1, wxEXPAND);
    SetSizer(sizer);

    Centre();
}

NativeEllipticPlot::PlotPanel::PlotPanel(
    wxWindow* parent,
    double a, double b,
    double xmin, double xmax,
    double ymin, double ymax)
    : wxPanel(parent),
      a_(a), b_(b),
      xmin_(xmin), xmax_(xmax),
      ymin_(ymin), ymax_(ymax)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetMinSize(wxSize(300, 250));

    Bind(wxEVT_PAINT,
         &NativeEllipticPlot::PlotPanel::OnPaint, this);
}

void NativeEllipticPlot::PlotPanel::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    auto gc = std::unique_ptr<wxGraphicsContext>(
        wxGraphicsContext::Create(dc));

    if (!gc)
        return;

    const wxSize size = GetClientSize();

    const double left = 65;
    const double right = 25;
    const double top = 30;
    const double bottom = 55;

    const double width = size.GetWidth() - left - right;
    const double height = size.GetHeight() - top - bottom;

    if (width <= 0 || height <= 0 ||
        !(xmin_ < xmax_) || !(ymin_ < ymax_))
        return;

    auto screenX = [&](double x) {
        return left + (x - xmin_) *
               width / (xmax_ - xmin_);
    };

    auto screenY = [&](double y) {
        return top + (ymax_ - y) *
               height / (ymax_ - ymin_);
    };

    gc->SetBrush(*wxWHITE_BRUSH);
    gc->SetPen(*wxTRANSPARENT_PEN);
    gc->DrawRectangle(left, top, width, height);

    // Grid lines
    gc->SetPen(wxPen(wxColour(220, 220, 220), 1));

    for (int i = 0; i <= 10; ++i)
    {
        double x = left + i * width / 10.0;
        double y = top + i * height / 10.0;

        gc->StrokeLine(x, top, x, top + height);
        gc->StrokeLine(left, y, left + width, y);
    }

    // Coordinate axes
    gc->SetPen(wxPen(wxColour(70, 70, 70), 1.5));

    if (xmin_ <= 0 && xmax_ >= 0)
    {
        double x = screenX(0);
        gc->StrokeLine(x, top, x, top + height);
    }

    if (ymin_ <= 0 && ymax_ >= 0)
    {
        double y = screenY(0);
        gc->StrokeLine(left, y, left + width, y);
    }

    // Draw the upper and lower branches separately.
    gc->SetPen(wxPen(wxColour(30, 90, 200), 2));

    const int samples =
        std::max(1000, static_cast<int>(width * 3));

    for (int branch = -1; branch <= 1; branch += 2)
    {
        wxGraphicsPath path = gc->CreatePath();
        bool started = false;
        bool hasSegments = false;

        for (int i = 0; i <= samples; ++i)
        {
            double x = xmin_ +
                (xmax_ - xmin_) * i / samples;

            double value = x*x*x + a_*x + b_;

            if (!std::isfinite(value) || value < 0)
            {
                started = false;
                continue;
            }

            double y = branch * std::sqrt(value);

            if (!std::isfinite(y) ||
                y < ymin_ || y > ymax_)
            {
                started = false;
                continue;
            }

            double px = screenX(x);
            double py = screenY(y);

            if (!started)
            {
                path.MoveToPoint(px, py);
                started = true;
            }
            else
            {
                path.AddLineToPoint(px, py);
                hasSegments = true;
            }
        }

        if (hasSegments)
            gc->StrokePath(path);
    }

    // Plot border
    gc->SetPen(wxPen(wxColour(90, 90, 90), 1));
    gc->SetBrush(*wxTRANSPARENT_BRUSH);
    gc->DrawRectangle(left, top, width, height);

    // Axis labels
    gc->SetFont(wxFontInfo(11), wxColour(40, 40, 40));
    gc->DrawText("x", left + width - 5, top + height + 25);
    gc->DrawText("y", left - 25, top - 5);

    // Range labels
    gc->DrawText(wxString::Format("%g", xmin_),
                 left, top + height + 8);

    gc->DrawText(wxString::Format("%g", xmax_),
                 left + width - 40, top + height + 8);

    gc->DrawText(wxString::Format("%g", ymax_),
                 5, top);

    gc->DrawText(wxString::Format("%g", ymin_),
                 5, top + height - 15);
}
