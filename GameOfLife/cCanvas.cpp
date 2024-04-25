#include "cCanvas.h"

#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(cCanvas, wxHVScrolledWindow)
EVT_PAINT(cCanvas::OnPaint)
EVT_LEFT_DOWN(cCanvas::OnMouseLeftDown)
wxEND_EVENT_TABLE()

cCanvas::cCanvas(wxWindow* parent) : wxHVScrolledWindow(parent, wxID_ANY)
{
	SetRowColumnCount(30, 47);  // Habitat Size
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	palette[0] = wxColour(175, 175, 175);  // light_grey / default
	palette[1] = wxColour(0, 0, 0);  // black
	palette[2] = wxColour(255, 255, 255);  // white
	palette[3] = wxColour(220, 0, 0);  // red
	palette[4] = wxColour(0, 128, 0);  // green
	palette[5] = wxColour(0, 0, 220);  // blue
	palette[6] = wxColour(140, 60, 240);  // purple
	palette[7] = wxColour(255, 128, 0);  // orange
	palette[8] = wxColour(230, 230, 0);  // yellow

}

cCanvas::~cCanvas()
{
}

void cCanvas::SetPixelSize(int n)
{
	m_nPixelSize = n;
	wxVarHScrollHelper::RefreshAll();
	wxVarVScrollHelper::RefreshAll();
	Refresh();
}

void cCanvas::SetColour(int c)
{
	m_nColour = c;
}

void cCanvas::SetHabitatData(int rows, int columns, int* pHabitat)
{
	m_pHabitat = pHabitat;
	this->SetRowColumnCount(rows, columns);
	Refresh();
}

void cCanvas::SetMode(bool light)
{
	lightMode = light;
	Refresh();
}

void cCanvas::SetGrid()
{
	gridOn = !gridOn;
	Refresh();
}

wxCoord cCanvas::OnGetRowHeight(size_t row) const
{
	return wxCoord(m_nPixelSize);
}

wxCoord cCanvas::OnGetColumnWidth(size_t col) const
{
	return wxCoord(m_nPixelSize);
}

void cCanvas::OnDraw(wxDC& dc)
{
	dc.Clear();

	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();

	wxPosition s = GetVisibleBegin();
	wxPosition e = GetVisibleEnd();

	pen.SetStyle(wxPENSTYLE_SOLID);

	if (lightMode) {
		pen.SetColour(wxColour(20, 20, 20));
	}
	else {
		pen.SetColour(wxColour(50, 50, 50));
	}

	
	dc.SetPen(pen);

	if (m_nPixelSize <= 7 or !gridOn) dc.SetPen(*wxTRANSPARENT_PEN);

	for (int y = s.GetRow(); y < e.GetRow(); y++)
		for (int x = s.GetCol(); x < e.GetCol(); x++) {

			int color = m_pHabitat[y * this->GetColumnCount() + x];

			if (!lightMode) {
				if (color == 0) {
					color = 1;
				}
				else if (color == 1) {
					color = 0;
				}
			}

			brush.SetColour(palette[color]);
			brush.SetStyle(wxBRUSHSTYLE_SOLID);

			dc.SetBrush(brush);
			dc.DrawRectangle(x * m_nPixelSize, y * m_nPixelSize, m_nPixelSize, m_nPixelSize);
		}
}

void cCanvas::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}

void cCanvas::OnMouseLeftDown(wxMouseEvent& evt)
{
	wxPosition s = GetVisibleBegin();
	// save the change to the habitat
	int color = m_pHabitat[(evt.GetY() / m_nPixelSize + s.GetRow()) * this->GetColumnCount() + (evt.GetX() / m_nPixelSize + s.GetCol())];
	if (color == m_nColour) {
		m_pHabitat[(evt.GetY() / m_nPixelSize + s.GetRow()) * this->GetColumnCount() + (evt.GetX() / m_nPixelSize + s.GetCol())] = 0;
	}
	else {
		m_pHabitat[(evt.GetY() / m_nPixelSize + s.GetRow()) * this->GetColumnCount() + (evt.GetX() / m_nPixelSize + s.GetCol())] = m_nColour;
	}
	this->Refresh(false);
	evt.Skip();
}
