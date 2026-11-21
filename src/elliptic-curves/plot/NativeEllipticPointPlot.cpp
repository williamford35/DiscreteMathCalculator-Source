/*
 * Native wxWidgets plot of elliptic-curve points
 * over a finite field.
 *
 * MIT License - see the application LICENSE file.
 */

#include "elliptic-curves/plot/NativeEllipticPointPlot.h"

#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/sizer.h>

#include <memory>

NativeEllipticPointPlot::NativeEllipticPointPlot(
    wxWindow* parent,
    long a,
    long b,
    long prime,
    const std::vector<std::pair<long, long>>& points)
    : wxFrame(
          parent,
          wxID_ANY,
          wxString::Format(
              "y^2 = x^3 + (%ld)x + (%ld) (mod %ld)",
              a, b, prime),
          wxDefaultPosition,
          wxSize(850, 650))
{
    PlotPanel* panel = new PlotPanel(this, prime, points);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel, 1, wxEXPAND);

    SetSizer(sizer);
    Centre();
}

NativeEllipticPointPlot::PlotPanel::PlotPanel(
    wxWindow* parent,
    long prime,
    const std::vector<std::pair<long, long>>& points)
    : wxPanel(parent),
      prime_(prime),
      points_(points)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetMinSize(wxSize(300, 250));

    Bind(
        wxEVT_PAINT,
        &NativeEllipticPointPlot::PlotPanel::OnPaint,
        this);
}

void NativeEllipticPointPlot::PlotPanel::OnPaint(
    wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    std::unique_ptr<wxGraphicsContext> gc(
        wxGraphicsContext::Create(dc));

    if (!gc || prime_ < 2)
        return;

    wxSize size = GetClientSize();

    const double left = 65.0;
    const double right = 30.0;
    const double top = 30.0;
    const double bottom = 60.0;

    const double width =
        size.GetWidth() - left - right;

    const double height =
        size.GetHeight() - top - bottom;

    if (width <= 0 || height <= 0)
        return;

    // Display the finite-field coordinates 0 through p-1.

    const double minCoord = -0.5;
    const double maxCoord =
        static_cast<double>(prime_) - 0.5;

    auto screenX = [&](double x)
    {
        return left +
            (x - minCoord) * width /
            (maxCoord - minCoord);
    };

    auto screenY = [&](double y)
    {
        return top +
            (maxCoord - y) * height /
            (maxCoord - minCoord);
    };

    // White plotting background.

    gc->SetBrush(*wxWHITE_BRUSH);
    gc->SetPen(*wxTRANSPARENT_PEN);

    gc->DrawRectangle(
        left, top, width, height);

    // Draw grid lines.

    gc->SetPen(
        wxPen(wxColour(225, 225, 225), 1));

    for (int i = 0; i <= 10; ++i)
    {
        double x =
            left + i * width / 10.0;

        double y =
            top + i * height / 10.0;

        gc->StrokeLine(
            x, top,
            x, top + height);

        gc->StrokeLine(
            left, y,
            left + width, y);
    }

    // Draw coordinate axes.

    gc->SetPen(
        wxPen(wxColour(70, 70, 70), 1.5));

    gc->StrokeLine(
        screenX(0), top,
        screenX(0), top + height);

    gc->StrokeLine(
        left, screenY(0),
        left + width, screenY(0));

    // Draw the elliptic-curve points.

    gc->SetPen(*wxTRANSPARENT_PEN);

    gc->SetBrush(
        wxBrush(wxColour(20, 65, 160)));

    double radius = 3.0;

    if (prime_ > 100)
        radius = 2.5;

    if (prime_ > 500)
        radius = 1.5;

    if (prime_ > 2000)
        radius = 1.0;

    for (const auto& pt : points_)
    {
        double x = screenX(pt.first);
        double y = screenY(pt.second);

        gc->DrawEllipse(
            x - radius,
            y - radius,
            radius * 2.0,
            radius * 2.0);
    }

    // Draw border around the plotting area.

    gc->SetPen(
        wxPen(wxColour(80, 80, 80), 1));

    gc->SetBrush(*wxTRANSPARENT_BRUSH);

    gc->DrawRectangle(
        left, top, width, height);

    // Draw coordinate labels.

    gc->SetFont(
        wxFontInfo(11),
        wxColour(40, 40, 40));

    gc->DrawText(
        "x",
        left + width - 5,
        top + height + 30);

    gc->DrawText(
        "y",
        left - 25,
        top - 5);

    gc->DrawText(
        "0",
        left - 12,
        top + height + 8);

    gc->DrawText(
        wxString::Format("%ld", prime_ - 1),
        left + width - 35,
        top + height + 8);

    gc->DrawText(
        wxString::Format("%ld", prime_ - 1),
        5,
        top);
}
