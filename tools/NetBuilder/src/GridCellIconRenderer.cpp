#include "Precompiled.h"
#include "GridCellIconRenderer.h"
//#include "pc.xpm"
//#include "Nomad.xpm"

wxSize wxGridCellIconRenderer::GetBestSize(wxGrid& grid,
                                             wxGridCellAttr& attr,
                                             wxDC& dc,
                                             int row, int col)
{
    return wxSize(16, 16);
}

void wxGridCellIconRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc,
	const wxRect& rectCell, int row, int col, bool isSelected)
{
	wxGridCellRenderer::Draw(grid, attr, dc, rectCell, row, col, isSelected);
	const char** psz=NULL;
// 	if (grid.GetCellValue(row, col) == wxT("IDI_PC"))
// 		psz = szIconPC;
// 	else psz = szIconNomad;
	wxIcon icon(psz);
	icon.SetHeight(16);
	icon.SetWidth(16);
//	wxColour colourBackGround = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_BTNFACE);
//	dc.SetBrush(wxBrush(colourBackGround, wxSOLID));
//	dc.SetPen(wxPen(colourBackGround, 1, wxSOLID));
//	dc.DrawRectangle(rectCell);
	int nMargin = (rectCell.GetHeight() - icon.GetHeight()) /2;
	if (nMargin < 0) nMargin = 0;
	dc.DrawIcon(icon, rectCell.x, rectCell.y+nMargin);
	if (m_border == wxLAYOUT_TOP)
	{
		dc.SetPen(wxPen(*wxBLACK, 1, wxDOT));
		dc.DrawLine(rectCell.GetRight(), rectCell.GetTop(), 
			rectCell.GetLeft(), rectCell.GetTop());
	}
//	dc.DrawIcon(icon, 16, 0);
}