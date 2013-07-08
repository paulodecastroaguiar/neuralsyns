#ifndef _GRIDCELLCHOICERENDERER_H_INCLUDED_
#define _GRIDCELLCHOICERENDERER_H_INCLUDED_

#include "wx/grid.h"
#include "wx/laywin.h"
#define COLORBASE wxColour(208,208,208)

class wxGridCellChoiceRenderer : public wxGridCellStringRenderer
{
public:
	wxGridCellChoiceRenderer(wxLayoutAlignment border = wxLAYOUT_NONE) : 
		m_border(border) {}
	virtual void Draw(wxGrid& grid,
		wxGridCellAttr& attr,
		wxDC& dc,
		const wxRect& rect,
		int row, int col,
		bool isSelected);
	virtual wxGridCellRenderer *Clone() const
		{ return new wxGridCellChoiceRenderer; }
private:
	wxLayoutAlignment m_border;
};


#endif // _GRIDCELLCHOICERENDERER_H_INCLUDED_