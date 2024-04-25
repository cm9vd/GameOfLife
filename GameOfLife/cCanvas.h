#pragma once

#include "wx/wx.h"
#include "wx/vscroll.h"

class cCanvas : public wxHVScrolledWindow
{
public:
	cCanvas(wxWindow* parent);
	~cCanvas();

private:
	int* m_pHabitat = nullptr;
	int m_nPixelSize = 16;
	wxColour palette[9];
	int m_nColour = 1;

	bool lightMode = true;
	bool gridOn = true;

public:
	void SetPixelSize(int n);
	void SetColour(int c);
	void SetHabitatData(int rows, int columns, int* pHabitat);

	void SetMode(bool light);
	void SetGrid();
	
private:
	virtual wxCoord OnGetRowHeight(size_t row) const;
	virtual wxCoord OnGetColumnWidth(size_t col) const;

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);

	void OnMouseLeftDown(wxMouseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

