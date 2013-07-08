#ifndef _GRIDCELLICONRENDERER_H_INCLUDED_
#define _GRIDCELLICONRENDERER_H_INCLUDED_

#include "wx/grid.h"
#include "wx/laywin.h"

class wxGridCellIconRenderer	: public wxGridCellRenderer
{
public:
	wxGridCellIconRenderer(wxLayoutAlignment border = wxLAYOUT_NONE) : 
		m_border(border) {}
	virtual void Draw(wxGrid& grid,
		wxGridCellAttr& attr,
		wxDC& dc,
		const wxRect& rect,
		int row, int col,
		bool isSelected);
	virtual wxSize GetBestSize(wxGrid& grid,
		wxGridCellAttr& attr,
		wxDC& dc,
		int row, int col);
	virtual wxGridCellRenderer *Clone() const
		{ return new wxGridCellIconRenderer; }
private:
	wxLayoutAlignment m_border;
};





#endif // _GRIDCELLICONRENDERER_H_INCLUDED_
