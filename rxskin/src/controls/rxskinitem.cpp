#include "pch.h"
#include "controls/rxskinitem.h"

CRXSkinItem::CRXSkinItem()
{
	m_enState = ITEM_NORMAL;
	m_rcItem.SetRectEmpty();
	m_dwID = 0;
	m_bEnabled = TRUE;
	m_hParentWnd = NULL;
	memset(m_szText, 0, sizeof(TCHAR)*256);
	memset(m_szTootip, 0, sizeof(TCHAR)*512);
	m_bVisible = FALSE;
}

CRXSkinItem::~CRXSkinItem()
{

}

uint64_t CRXSkinItem::GetID()
{
	return m_dwID;
}

VOID CRXSkinItem::SetID(const uint64_t dwID)
{
	m_dwID = dwID;
}


VOID CRXSkinItem::SetRect(CRXSkinRect rcItem)
{
	m_rcItem = rcItem;
}

CRXSkinRect CRXSkinItem::GetRect()
{
	return m_rcItem;
}

HWND CRXSkinItem::GetParentWnd()
{
	return m_hParentWnd;
}

VOID CRXSkinItem::SetParentWnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

EItemState CRXSkinItem::GetState()
{
	return m_enState;
}

VOID CRXSkinItem::SetState(EItemState enState)
{
	m_enState = enState;
}

BOOL CRXSkinItem::IsEnabled()
{
	return m_bEnabled;
}

VOID CRXSkinItem::Enable(const BOOL bEnabled)
{
	m_bEnabled = bEnabled;
}

CRXSkinString CRXSkinItem::GetClassType()
{
	return _T("");
}

BOOL   CRXSkinItem::IsVisible()
{
	return m_bVisible; 
}
VOID CRXSkinItem::SetVisible(const BOOL bIsVisible) 
{
	m_bVisible = bIsVisible; 
};

VOID CRXSkinItem::Invalidate()
{
	if (m_hParentWnd)
	{
		RECT rc;
		rc.left = m_rcItem.left;
		rc.right = m_rcItem.right;
		rc.top = m_rcItem.top;
		rc.bottom = m_rcItem.bottom;
		::InvalidateRect(m_hParentWnd,&rc,FALSE);
	}
}