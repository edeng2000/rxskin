#include "pch.h"
#include "controls/rxskincolorpicker.h"
#include <math.h>

#define CELL_WIDTH         16
#define CELL_HEIGHT        16
#define CELL_OFFSET        2
#define TOP_OFFSET_X       4
#define TOP_OFFSET_Y       4
#define PICKER_WIDTH       (CELL_WIDTH*8) + 7 * CELL_OFFSET + TOP_OFFSET_X * 2
#define PICKER_HEIGHT      (CELL_HEIGHT*5) + 4 * CELL_OFFSET + TOP_OFFSET_Y * 2


ColourTableEntry CRXSkinColorPicker::m_Colours[] =
{
	{ RGB(0x00, 0x00, 0x00),    (TCHAR*)_T("Black")             },
	{ RGB(0xA5, 0x2A, 0x00),    (TCHAR*)_T("Brown")             },
	{ RGB(0x00, 0x40, 0x40),    (TCHAR*)_T("Dark Olive Green")  },
	{ RGB(0x00, 0x55, 0x00),    (TCHAR*)_T("Dark Green")        },
	{ RGB(0x00, 0x00, 0x5E),    (TCHAR*)_T("Dark Teal")         },
	{ RGB(0x00, 0x00, 0x8B),    (TCHAR*)_T("Dark blue")         },
	{ RGB(0x4B, 0x00, 0x82),    (TCHAR*)_T("Indigo")            },
	{ RGB(0x28, 0x28, 0x28),    (TCHAR*)_T("Dark grey")         },

	{ RGB(0x8B, 0x00, 0x00),    (TCHAR*)_T("Dark red")          },
	{ RGB(0xFF, 0x68, 0x20),    (TCHAR*)_T("Orange")            },
	{ RGB(0x8B, 0x8B, 0x00),    (TCHAR*)_T("Dark yellow")       },
	{ RGB(0x00, 0x93, 0x00),    (TCHAR*)_T("Green")             },
	{ RGB(0x38, 0x8E, 0x8E),    (TCHAR*)_T("Teal")              },
	{ RGB(0x00, 0x00, 0xFF),    (TCHAR*)_T("Blue")              },
	{ RGB(0x7B, 0x7B, 0xC0),    (TCHAR*)_T("Blue-grey")         },
	{ RGB(0x66, 0x66, 0x66),    (TCHAR*)_T("Grey - 40")         },

	{ RGB(0xFF, 0x00, 0x00),    (TCHAR*)_T("Red")               },
	{ RGB(0xFF, 0xAD, 0x5B),    (TCHAR*)_T("Light orange")      },
	{ RGB(0x32, 0xCD, 0x32),    (TCHAR*)_T("Lime")              },
	{ RGB(0x3C, 0xB3, 0x71),    (TCHAR*)_T("Sea green")         },
	{ RGB(0x7F, 0xFF, 0xD4),    (TCHAR*)_T("Aqua")              },
	{ RGB(0x7D, 0x9E, 0xC0),    (TCHAR*)_T("Light blue")        },
	{ RGB(0x80, 0x00, 0x80),    (TCHAR*)_T("Violet")            },
	{ RGB(0x7F, 0x7F, 0x7F),    (TCHAR*)_T("Grey - 50")         },

	{ RGB(0xFF, 0xC0, 0xCB),    (TCHAR*)_T("Pink")              },
	{ RGB(0xFF, 0xD7, 0x00),    (TCHAR*)_T("Gold")              },
	{ RGB(0xFF, 0xFF, 0x00),    (TCHAR*)_T("Yellow")            },
	{ RGB(0x00, 0xFF, 0x00),    (TCHAR*)_T("Bright green")      },
	{ RGB(0x40, 0xE0, 0xD0),    (TCHAR*)_T("Turquoise")         },
	{ RGB(0xC0, 0xFF, 0xFF),    (TCHAR*)_T("Skyblue")           },
	{ RGB(0x48, 0x00, 0x48),    (TCHAR*)_T("Plum")              },
	{ RGB(0xC0, 0xC0, 0xC0),    (TCHAR*)_T("Light grey")        },

	{ RGB(0xFF, 0xE4, 0xE1),    (TCHAR*)_T("Rose")              },
	{ RGB(0xD2, 0xB4, 0x8C),    (TCHAR*)_T("Tan")               },
	{ RGB(0xFF, 0xFF, 0xE0),    (TCHAR*)_T("Light yellow")      },
	{ RGB(0x98, 0xFB, 0x98),    (TCHAR*)_T("Pale green ")       },
	{ RGB(0xAF, 0xEE, 0xEE),    (TCHAR*)_T("Pale turquoise")    },
	{ RGB(0x68, 0x83, 0x8B),    (TCHAR*)_T("Pale blue")         },
	{ RGB(0xE6, 0xE6, 0xFA),    (TCHAR*)_T("Lavender")          },
	{ RGB(0xFF, 0xFF, 0xFF),    (TCHAR*)_T("White")             }
};


CRXSkinColorCell::CRXSkinColorCell()
{
	m_crlSelect = RGB(0,0,0);
	m_poOwner = NULL;
}

CRXSkinColorCell::~CRXSkinColorCell()
{
}


void CRXSkinColorCell::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	switch (GetState())
	{
	case ITEM_NORMAL:
	{
		DrawRect(hDC, m_crlSelect, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
	}
	break;
	case ITEM_MOUSEON:
	case ITEM_PRESSED:
	case ITEM_SELECTED:
	{
		DrawRect(hDC,RGB(50,60,70), m_crlSelect, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
		break;
	}
	default:
		break;
	}
}

CRXSkinItem* CRXSkinColorCell::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	return NULL;
}

CRXSkinColorPicker::CRXSkinColorPicker()
{
	m_dwParentID = 0;
	m_poMouseOnCell = NULL;
}

CRXSkinColorPicker::~CRXSkinColorPicker()
{
	for (int m=0;m < (int)m_vecCells.size();m ++)
	{
		delete m_vecCells[m];
	}
	m_vecCells.clear();
}

BOOL CRXSkinColorPicker::Create(const TCHAR* pszWindowClassName, const TCHAR* pszWindowName, const UINT32 dwStyle, const HWND hParentWnd, HICON hIcon /* = NULL */)
{
	return CRXSkinWnd::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
}


BOOL CRXSkinColorPicker::OnMouseMove(UINT nFlags, POINT point)
{
	if (GetCapture() != GetHWnd())
	{
		return FALSE;
	}
	CRXSkinColorCell* poCell = _HitTest(point);
	if (NULL == poCell)
	{
		if (m_poMouseOnCell)
		{
			m_poMouseOnCell->SetState(ITEM_NORMAL);
			RECT rc = m_poMouseOnCell->GetRect();
			InvalidateRect(GetHWnd(),&rc,FALSE);
			m_poMouseOnCell = NULL;
		}
	}
	else
	{
		if (poCell!=m_poMouseOnCell)
		{
			if (m_poMouseOnCell)
			{
				m_poMouseOnCell->SetState(ITEM_NORMAL);
				RECT rc = m_poMouseOnCell->GetRect();
				InvalidateRect(GetHWnd(), &rc, FALSE);
			}
			m_poMouseOnCell = poCell;
			m_poMouseOnCell->SetState(ITEM_MOUSEON);
			RECT rc = m_poMouseOnCell->GetRect();
			InvalidateRect(GetHWnd(), &rc, FALSE);
		}
	}
	return CRXSkinWnd::OnMouseMove(nFlags, point);
}

void CRXSkinColorPicker::Show(CRXControl* poControl)
{
	POINT pos = poControl->GetWindowPos();
	UINT32 dwScreenX = GetSystemMetrics(SM_CXSCREEN);
	UINT32 dwScreenY = GetSystemMetrics(SM_CYSCREEN);
	CRXSkinRect rcWin;
	rcWin.left = pos.x;
	if (rcWin.left + PICKER_WIDTH >(int) dwScreenX)
	{
		rcWin.left = dwScreenX - PICKER_WIDTH;
	}
	rcWin.right = rcWin.left + PICKER_WIDTH;
	rcWin.top = pos.y - PICKER_HEIGHT;
	if (rcWin.top<0)
	{
		rcWin.top = 0;
	}
	rcWin.bottom = rcWin.top + PICKER_HEIGHT;
	m_dwParentID = poControl->GetID();
	for (int m=0; m < 40;m ++)
	{
		CRXSkinColorCell* poCell = new CRXSkinColorCell;
		poCell->SetColor(m_Colours[m].crColour);
		poCell->SetOwner(this);
		m_vecCells.push_back(poCell);
	}
	if (!CRXSkinWnd::Create(RXSKIN_COLOR_PICKER,
		_T(""),
		WS_POPUP | WS_VISIBLE,
		poControl->GetParentWnd(),
		LoadIcon(CRXSkinService::Instance()->GetInstance(), MAKEINTRESOURCE(107))))
	{
		return;
	}
	MoveWindow(m_hWnd, rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(), TRUE);
	ShowWindow(m_hWnd, SW_SHOW);
	::SetCapture(m_hWnd);
}

void CRXSkinColorPicker::OnCancelMode()
{

	CRXSkinWnd::OnCancelMode();
}

VOID CRXSkinColorPicker::OnPaint()
{
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	PAINTSTRUCT ps = { 0 };
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	CRXSkinMemDC memDC(hDC, rcClient);
	DrawRect(memDC.GetHDC(),
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor(),
		CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
		rcClient.left, rcClient.top, rcClient.right, rcClient.bottom
	);
	for (int x=0;x <8; x ++ )
	{
		for (int y = 0; y < 5;y ++)
		{
			CRXSkinRect rcCell = rcClient;
			rcCell.left = TOP_OFFSET_X + x * (CELL_WIDTH + CELL_OFFSET);
			rcCell.right = rcCell.left + CELL_WIDTH;
			rcCell.top = TOP_OFFSET_Y + y * (CELL_HEIGHT + CELL_OFFSET);
			rcCell.bottom = rcCell.top + CELL_HEIGHT;
			m_vecCells[8 * y + x]->OnDraw(memDC.GetSurface(),memDC.GetHDC(), rcCell);
		}
	}
	memDC.TakeSnapshot();
	EndPaint(m_hWnd, &ps);
}

BOOL CRXSkinColorPicker::OnMouseHover(UINT nFlags, POINT point)
{
	return CRXSkinWnd::OnMouseHover(nFlags, point);
}

LRESULT CRXSkinColorPicker::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_poMouseOnCell)
	{
		m_poMouseOnCell->SetState(ITEM_NORMAL);
		RECT rc = m_poMouseOnCell->GetRect();
		InvalidateRect(m_poMouseOnCell->GetParentWnd(), &rc, FALSE);
		m_poMouseOnCell = NULL;
	}
	return 0;
}

VOID CRXSkinColorPicker::OnActive(WPARAM nState, HWND hWnd)
{
	if (nState == WA_INACTIVE)
	{
		DestroyWindow(m_hWnd);
	}
}

BOOL CRXSkinColorPicker::OnLButtonDown(UINT nFlags, POINT point)
{
	::SetCapture(GetHWnd());
	SetFocus(GetHWnd());
	return CRXSkinWnd::OnLButtonDown(nFlags, point);
}
BOOL CRXSkinColorPicker::OnLButtonUp(UINT nFlags, POINT point)
{
	if (GetCapture() == GetHWnd())
		::ReleaseCapture();
	
	if (point.x < 0 || point.y < 0)
	{
		DestroyWindow(GetHWnd());
		return TRUE;
	}
	CRXSkinColorCell* poCell = _HitTest(point);
	if (poCell)
	{
		::PostMessage(GetParentWnd(), WM_RXSKIN_COLOR_PICKER_OK, m_dwParentID, (LPARAM)poCell->GetColor());
		DestroyWindow(GetHWnd());
		return TRUE;
	}

	return CRXSkinWnd::OnLButtonUp(nFlags, point);
}
BOOL CRXSkinColorPicker::OnLButtonDblClk(UINT nFlags, POINT point)
{
	SetFocus(GetHWnd());
	if (point.x < 0 || point.y < 0)
	{
		DestroyWindow(GetHWnd());
		return TRUE;
	}
	return CRXSkinWnd::OnLButtonDblClk(nFlags, point);
}

BOOL CRXSkinColorPicker::OnMButtonDown(UINT nFlags, POINT point)
{
	SetFocus(GetHWnd());
	return CRXSkinWnd::OnLButtonDown(nFlags, point);
}
BOOL CRXSkinColorPicker::OnMButtonUp(UINT nFlags, POINT point)
{
	SetFocus(GetHWnd());
	if (point.x < 0 || point.y < 0)
	{
		DestroyWindow(GetHWnd());
		return TRUE;
	}
	return CRXSkinWnd::OnMButtonUp(nFlags, point);
}
BOOL CRXSkinColorPicker::OnMButtonDblClk(UINT nFlags, POINT point)
{
	SetFocus(GetHWnd());
	if (point.x < 0 || point.y < 0)
	{
		DestroyWindow(GetHWnd());
		return TRUE;
	}
	return CRXSkinWnd::OnMButtonDblClk(nFlags, point);
}


BOOL CRXSkinColorPicker::OnRButtonDown(UINT nFlags, POINT point)
{
	if (point.x < 0 || point.y < 0)
	{
		DestroyWindow(GetHWnd());
		return TRUE;
	}
	return CRXSkinWnd::OnRButtonDown(nFlags, point);
}
BOOL CRXSkinColorPicker::OnRButtonUp(UINT nFlags, POINT point)
{
	if (point.x < 0 || point.y < 0)
	{
		DestroyWindow(GetHWnd());
		return TRUE;
	}
	return CRXSkinWnd::OnRButtonUp(nFlags, point);
}
BOOL CRXSkinColorPicker::OnRButtonDblClk(UINT nFlags, POINT point)
{
	if (point.x < 0 || point.y < 0)
	{
		DestroyWindow(GetHWnd());
		return TRUE;
	}
	return CRXSkinWnd::OnRButtonDblClk(nFlags, point);
}


void CRXSkinColorPicker::OnFinalMessage(HWND hWnd)
{
	delete this;
}

CRXSkinColorCell* CRXSkinColorPicker::_HitTest(POINT pt)
{
	for (int m=0;m < (int)m_vecCells.size(); m ++)
	{
		if (m_vecCells[m]->HitTest(pt))
		{
			return m_vecCells[m];
		}
	}
	return NULL;
}