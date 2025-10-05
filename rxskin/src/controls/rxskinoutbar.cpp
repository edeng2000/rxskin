#include "pch.h"
#include "controls\rxskinoutbar.h"
#include "common\RXSkinService.h"
#include "common\graphics.h"
#define FRIEND_CTRL_NAME   _T("RXSkinOutbar")

#define TYPE_OUTBAR_ITEM_NAME     _T("OutbarItem")
#define TYPE_OUTBAR_FOLDER_NAME   _T("OutbarFolder")


CRXSkinOutbarItem::CRXSkinOutbarItem()
{
	m_poImage = NULL;
	m_strName = m_strImageFile = _T("");
	m_bSelected = FALSE;
}

CRXSkinOutbarItem::~CRXSkinOutbarItem()
{
}

VOID CRXSkinOutbarItem::SetSelected(const BOOL bSelected)
{
	m_bSelected = bSelected;
}

BOOL CRXSkinOutbarItem::IsSelected()
{
	return m_bSelected;
}

VOID CRXSkinOutbarItem::SetImageFile(const TCHAR* pszImageFile)
{
	if (m_poImage)
	{
		CRXSkinService::Instance()->GetImageMgr().RemoveImage(m_strImageFile);
		m_poImage = NULL;
	}
	m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenImage(pszImageFile);
	if (m_poImage)
	{
		m_strImageFile = pszImageFile;
	}
}

VOID CRXSkinOutbarItem::SetName(const TCHAR* pszName)
{
	m_strName = pszName;
}

CRXSkinString CRXSkinOutbarItem::GetName()
{
	return m_strName;
}

VOID CRXSkinOutbarItem::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	COLORREF clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemNormalText();
	if (m_bSelected)
	{
		clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemSelectedText();
		if (GetState() == ITEM_MOUSEON)
		{
			CRXSkinRect rcBk = rcItem;
			rcBk.left += CRXSkinService::Instance()->GetDrawMgr().GetOutbarOffSet().cx;
			rcBk.right -= CRXSkinService::Instance()->GetDrawMgr().GetOutbarOffSet().cx;
			HRGN hRgn = CreateRoundRectRgn(rcBk.left,
				rcBk.top,
				rcBk.right,
				rcBk.bottom,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy);
			DrawGradientRgn(hDC,
				TRUE,
				hRgn,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemSelectedMouseOnFrom(),
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemSelectedMouseOnTo());
			DeleteObject(hRgn);
		}
		else
		{
			CRXSkinRect rcBk = rcItem;
			rcBk.left += CRXSkinService::Instance()->GetDrawMgr().GetOutbarOffSet().cx;
			rcBk.right -= CRXSkinService::Instance()->GetDrawMgr().GetOutbarOffSet().cx;
			HRGN hRgn = CreateRoundRectRgn(rcBk.left,
				rcBk.top,
				rcBk.right,
				rcBk.bottom,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy);
			DrawGradientRgn(hDC,
				TRUE,
				hRgn,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemSelectedNormalFrom(),
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemSelectedNormalTo());
			DeleteObject(hRgn);
		}
	}
	else
	{
		switch(GetState())
		{
		case ITEM_MOUSEON:
			{
				clrTxt = CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemMouseOnText();
			}
			break;
		default:
			break;
		}
	}

	CRXSkinRect rcImage = rcItem;
	rcImage.left += CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemIndentationX();
	INT32 nLeft = rcImage.left;
	if (m_poImage)
	{
		rcImage.right = rcImage.left + m_poImage->GetWidth();
		rcImage.top = (rcItem.Height() - m_poImage->GetHeight())/2+ rcItem.top;
		rcImage.bottom  = rcImage.top + m_poImage->GetHeight();
		nLeft += m_poImage->GetWidth();
		m_poImage->Draw(hDC,rcImage);
	}
	CRXSkinRect rcTxt = rcItem;
	rcTxt.left = nLeft + CRXSkinService::Instance()->GetDrawMgr().GetOutbarOffSet().cx;
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas, m_strName.GetData(), rcTxt, clrTxt, ID_FONT_COMMON,
			DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
}

CRXSkinItem* CRXSkinOutbarItem::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	return NULL;
}

CRXSkinString CRXSkinOutbarItem::GetClassType()
{
	return TYPE_OUTBAR_ITEM_NAME;
}



CRXSkinOutbarFolder::CRXSkinOutbarFolder()
{
	m_bOpen = FALSE;
	m_strGroupName = _T("");
	m_nTotalHeight = 0;
}

CRXSkinOutbarFolder::~CRXSkinOutbarFolder()
{
	_DeleteAll();
}

VOID CRXSkinOutbarFolder::_DeleteAll()
{
	COutbarItemMapItr itr = m_oOutbarItemMap.begin();
	for (;itr!=m_oOutbarItemMap.end();itr++)
	{
		delete itr->second;
	}
	m_oOutbarItemMap.clear();
}


CRXSkinOutbarFolder::COutbarItemMap& CRXSkinOutbarFolder::GetItemMap()
{
	return m_oOutbarItemMap;
}

CRXSkinString CRXSkinOutbarFolder::GetGroupName()
{
	return m_strGroupName;
}

VOID CRXSkinOutbarFolder::SetGroupName(const TCHAR* pszGroupName)
{
	m_strGroupName = pszGroupName;
}

CRXSkinOutbarItem* CRXSkinOutbarFolder::FindItem(const UINT32 dwItemID)
{
	COutbarItemMapItr itr = m_oOutbarItemMap.find(dwItemID);
	if (itr!=m_oOutbarItemMap.end())
	{
		return itr->second;
	}
	return NULL;
}

BOOL CRXSkinOutbarFolder::AddItem(CRXSkinOutbarItem* poItem)
{
	if (NULL == FindItem((uint32_t)poItem->GetID()))
	{
		m_oOutbarItemMap.insert(make_pair((uint32_t)poItem->GetID(),poItem));
		return TRUE;
	}
	return FALSE;
}

CRXSkinOutbarItem* CRXSkinOutbarFolder::RemoveItem(const UINT32 dwItemID)
{
	COutbarItemMapItr itr = m_oOutbarItemMap.find(dwItemID);
	if (itr!=m_oOutbarItemMap.end())
	{
		CRXSkinOutbarItem* poItem = itr->second;
		m_oOutbarItemMap.erase(itr);
		return poItem;
	}
	return NULL;
}

BOOL CRXSkinOutbarFolder::GetOpened()
{
	return m_bOpen;
}

VOID CRXSkinOutbarFolder::SetOpened(const BOOL bOpened)
{
	m_bOpen = bOpened;
}

VOID CRXSkinOutbarFolder::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	COLORREF clrOldTxt = ::SetTextColor(hDC,CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderNormalText());
	m_nTotalHeight= CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderHeight();
	rcItem.bottom = rcItem.top + m_nTotalHeight;		 
	CRXSkinRect rcBk = rcItem;
	rcBk.bottom = m_nTotalHeight+rcBk.top;
	switch(GetState())
	{
	case ITEM_NORMAL:
		{
			clrOldTxt = CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderNormalText();
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderNormalBorder(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
				rcBk,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy);
			HRGN hRgn = CreateRoundRectRgn(rcItem.left+2,
				rcBk.top+2,
				rcBk.right - 1,
				rcBk.bottom - 1,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy
				);
			DrawGradientRgn(hDC,TRUE,hRgn,CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderNormalFrom(),
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderNormalTo());
			::DeleteObject(hRgn);
		}
		break;
	case ITEM_MOUSEON:
		{
			clrOldTxt = CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderMouseOnText();
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderMouseOnFirstBorder(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
				rcItem,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy);

			rcBk.left += 1;
			rcBk.top += 1;
			rcBk.right -= 1;
			rcBk.bottom -= 1;
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderMouseOnSecondBorder(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
				rcBk,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy);
			rcBk.left += 1;
			rcBk.top += 1;
			rcBk.right -= 1;
			rcBk.bottom -= 1;
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderMouseOnThirdBorder(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
				rcBk,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy);
			HRGN hRgn = CreateRoundRectRgn(rcItem.left+2,
				rcBk.top+2,
				rcBk.right,
				rcBk.bottom,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy
				);
			DrawGradientRgn(hDC,TRUE,hRgn,CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderMouseOnFrom(),
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderMouseOnTo());
			::DeleteObject(hRgn);
		}
		break;
	case ITEM_PRESSED:
		{
			clrOldTxt = CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderPressedText();
			DrawRoundRect(hDC,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderPressedBorder(),
				CRXSkinService::Instance()->GetDrawMgr().GetCommonBk(),
				rcBk,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy);
			HRGN hRgn = CreateRoundRectRgn(rcItem.left+2,
				rcBk.top+2,
				rcBk.right - 2,
				rcBk.bottom - 2,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cx,
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarRound().cy
				);
			DrawGradientRgn(hDC,TRUE,hRgn,CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderPressedFrom(),
				CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderPressedTo());
			::DeleteObject(hRgn);
		}
		break;
	default:
		break;
	}
	INT32 nImageWidth = 0;
	CRXSkinRect rcText = rcItem;
	rcText.left += CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderOffSet().cx;
	rcText.left += nImageWidth;
	SkCanvas* poCanvas = poSurface->getCanvas();
	if (poCanvas)
	{
		CRXSkinFontMgr::Instance()->DrawText(poCanvas,
			m_strGroupName.GetData(), rcText, clrOldTxt, ID_FONT_COMMON_BOLD,
			DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	}
	if (m_bOpen)
	{
		CRXSkinRect rcFriendItem = rcItem;
		rcFriendItem.bottom = CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderHeight()+rcItem.top;

		rcFriendItem.top = rcFriendItem.bottom;
		rcFriendItem.top += CRXSkinService::Instance()->GetDrawMgr().GetOutbarOffSet().cy;
		COutbarItemMapItr itr = m_oOutbarItemMap.begin();
		for (;itr!=m_oOutbarItemMap.end();itr++)
		{
			rcFriendItem.bottom = rcFriendItem.top + CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemHeight();
			itr->second->OnDraw(poSurface,hDC,rcFriendItem);
			rcFriendItem.top = rcFriendItem.bottom;
			m_nTotalHeight += CRXSkinService::Instance()->GetDrawMgr().GetOutbarItemHeight();
		}
	}
	else
	{
		COutbarItemMapItr itr = m_oOutbarItemMap.begin();
		for (;itr!=m_oOutbarItemMap.end();itr++)
		{
			itr->second->SetRect(CRXSkinRect(0,0,0,0));	
		}
	}

	SetRect(rcItem);
}

CRXSkinString CRXSkinOutbarFolder::GetClassType()
{
	return TYPE_OUTBAR_FOLDER_NAME;
}

CRXSkinItem* CRXSkinOutbarFolder::HitTest(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return this;
	}
	if (m_bOpen)
	{
		COutbarItemMapItr itr = m_oOutbarItemMap.begin();
		for (;itr!=m_oOutbarItemMap.end();itr++)
		{
			CRXSkinItem* poItem = itr->second->HitTest(pt);
			if (poItem)
			{
				return poItem;
			}
		}
	}
	return NULL;
}

UINT32 CRXSkinOutbarFolder::GetTotalHeight()
{
	return m_nTotalHeight;
}


CRXSkinOutbar::CRXSkinOutbar(void)
{
	m_poMouseOn = NULL;
	m_poPressed = NULL;
	m_poSelectFolder = NULL;
	m_poOutbarListener = NULL;
}

CRXSkinOutbar::~CRXSkinOutbar(void)
{
}

VOID CRXSkinOutbar::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	DrawGradientRect(hDC,
		TRUE,
		rcItem,
		CRXSkinService::Instance()->GetDrawMgr().GetOutbarBKFrom(),
		CRXSkinService::Instance()->GetDrawMgr().GetOutbarBKTo());
	CRXSkinOutbarFolderMapItr itr = m_oOutbarFolderMap.begin();
	CRXSkinRect rcFolder = rcItem;
	rcFolder.top = rcFolder.top + CRXSkinService::Instance()->GetDrawMgr().GetOutbarFolderOffSet().cy;
	INT32 nCount = (INT32)m_oOutbarFolderMap.size();
	INT32 nCur = 0;
	for (;itr!=m_oOutbarFolderMap.end();itr++)
	{
		nCur += 1;
		CRXSkinOutbarFolder* poFolder = itr->second;
		poFolder->OnDraw(poSurface,hDC,rcFolder);
		rcFolder = poFolder->GetRect();
		rcFolder.top = rcFolder.bottom;
		rcFolder.top += (poFolder->GetTotalHeight() - poFolder->GetRect().Height());
		if (poFolder->GetOpened())
		{
			rcFolder.top = rcItem.bottom - (nCount - nCur)*poFolder->GetRect().Height();
		}
	}
}

VOID CRXSkinOutbar::ReleaseControl()
{
	delete this;
}
BOOL CRXSkinOutbar::PtInRect(POINT pt)
{
	if (IsVisible() && IsVisible())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinOutbar::OnClick(POINT pt)
{
	CRXSkinItem* poItem = _HitTest(pt);
	if (poItem)
	{
		if (poItem->GetClassType() == TYPE_OUTBAR_FOLDER_NAME)
		{
			CRXSkinOutbarFolder* poFolder = (CRXSkinOutbarFolder*)poItem;
			if (FALSE == poFolder->GetOpened())
			{
				poFolder->SetOpened(!poFolder->GetOpened());
				if (m_poSelectFolder != poFolder)
				{
					if (NULL == m_poSelectFolder)
					{
						m_poSelectFolder = poFolder;						
					}
					else
					{
						m_poSelectFolder->SetOpened(FALSE);
						m_poSelectFolder = poFolder;
					}
					SelectFolder((uint32_t)poFolder->GetID());
				}
				Invalidate();
			}
		}
		else
		{
			if (NULL == m_poPressed)
			{
				m_poPressed = poItem;
				m_poPressed->SetState(ITEM_SELECTED);
				((CRXSkinOutbarItem*)m_poPressed)->SetSelected(TRUE);
				m_poPressed->Invalidate();				
			}
			else
			{
				if (m_poPressed!=poItem)
				{
					((CRXSkinOutbarItem*)m_poPressed)->SetSelected(FALSE);
					m_poPressed->Invalidate();
					m_poPressed = poItem;
					((CRXSkinOutbarItem*)m_poPressed)->SetSelected(TRUE);
					m_poPressed->Invalidate();
				}
			}
			if (m_poOutbarListener)
			{
				m_poOutbarListener->OnItemSelected((uint32_t)poItem->GetID());
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRXSkinOutbar::OnMouseMove(POINT pt)
{
	CRXSkinItem* poItem = _HitTest(pt);
	if (poItem)
	{
		if (NULL == m_poMouseOn)
		{
			m_poMouseOn = poItem;
			m_poMouseOn->SetState(ITEM_MOUSEON);
			m_poMouseOn->Invalidate();
			
		}
		else
		{
			if (poItem != m_poMouseOn)
			{
				m_poMouseOn->SetState(ITEM_NORMAL);
				m_poMouseOn->Invalidate();
				m_poMouseOn = poItem;
				m_poMouseOn->SetState(ITEM_MOUSEON);
				m_poMouseOn->Invalidate();
			}
		}
		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_HAND);
	}
	else
	{
		if (m_poMouseOn)
		{
			m_poMouseOn->SetState(ITEM_NORMAL);
			m_poMouseOn->Invalidate();
			m_poMouseOn = NULL;
		}

		CRXSkinService::Instance()->GetDrawMgr().SetCursor(CDrawMgr::CURSOR_ARROW);
	}
	return FALSE;
}

BOOL CRXSkinOutbar::OnLButtonDown(POINT pt)
{
	if (IsVisible() && IsVisible())
	{
		return PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinOutbar::OnSize(const UINT32 dwX,const UINT32 dwY)
{
	return CRXControl::OnSize(dwX,dwY);
}

CRXSkinRect CRXSkinOutbar::GetInvalidateRect()
{
	return GetRect();
}

VOID CRXSkinOutbar::OnMouseLeave()
{

}

CRXSkinItem* CRXSkinOutbar::_HitTest(POINT pt)
{
	CRXSkinOutbarFolderMapItr itr = m_oOutbarFolderMap.begin();
	for (;itr!=m_oOutbarFolderMap.end();itr++)
	{
		CRXSkinOutbarFolder* poFolder = itr->second;
		if (poFolder)
		{
			CRXSkinItem* poItem = poFolder->HitTest(pt);
			if (poItem)
			{
				return poItem;
			}
		}
	}
	return NULL;
}

VOID CRXSkinOutbar::_DeleteAll()
{
	CRXSkinOutbarFolderMapItr itr = m_oOutbarFolderMap.begin();
	for (;itr!=m_oOutbarFolderMap.end();itr++)
	{
		delete itr->second;
	}
	m_oOutbarFolderMap.clear();
}

VOID CRXSkinOutbar::SetListen(IRXSkinOutbarListener* poOutbarListener)
{
	m_poOutbarListener = poOutbarListener;
}

VOID CRXSkinOutbar::SelectFolder(const UINT32 dwFolderID)
{
	CRXSkinOutbarFolderMapItr itr =m_oOutbarFolderMap.begin();
	for (;itr!=m_oOutbarFolderMap.end();itr++)
	{
		if (itr->first == dwFolderID)
		{
			itr->second->SetOpened(TRUE);
			CRXSkinOutbarFolder* poFolder = itr->second;
			if (poFolder)
			{
				CRXSkinOutbarFolder::COutbarItemMap& oMap = poFolder->GetItemMap();
				CRXSkinOutbarFolder::COutbarItemMapItr itrItem = oMap.begin();
				if (itrItem!=oMap.end())
				{
					CRXSkinOutbarItem* poItem = itrItem->second;
					SelectItem((uint32_t)poItem->GetID());
				}
			}
			m_poSelectFolder = itr->second;
			if (m_poOutbarListener)
			{
				m_poOutbarListener->OnFolderSelected(dwFolderID);				
			}
		}
		else
		{
			itr->second->SetOpened(FALSE);
		}
	}
	Invalidate();
}

VOID CRXSkinOutbar::SelectItem(const UINT32 dwUserID)
{
	CRXSkinOutbarItem* poItem = FindItem(dwUserID);
	if (poItem)
	{
		poItem->SetSelected(TRUE);
		if (m_poPressed && m_poPressed!=poItem)
		{
			((CRXSkinOutbarItem*)m_poPressed)->SetSelected(FALSE);
			m_poPressed->Invalidate();
		}
		m_poPressed = poItem;
		poItem->Invalidate();
		if (m_poOutbarListener)
		{
			m_poOutbarListener->OnItemSelected((uint32_t)poItem->GetID());
		}
	}
}

BOOL CRXSkinOutbar::AddItem(const UINT32 dwFolderID,
							const UINT32 dwUserID,
							const TCHAR* pszName,								    
							const TCHAR* pszImagePath)
{
	CRXSkinOutbarFolder* poFolder = FindFolder(dwFolderID);
	if (NULL == poFolder)
	{
		return FALSE;
	}
	CRXSkinOutbarItem* poItem = poFolder->FindItem(dwUserID);
	if (poItem)
	{
		return FALSE;
	}
	poItem = new CRXSkinOutbarItem;
	poItem->SetImageFile(pszImagePath);
	poItem->SetName(pszName);
	poItem->SetID(dwUserID);
	poItem->SetParentWnd(GetParentWnd());
	if (FALSE == poFolder->AddItem(poItem))
	{
		delete poItem;
		return FALSE;
	}
	return TRUE;
}

BOOL CRXSkinOutbar::AddFolder(const UINT32 dwFolderID,const TCHAR* pszFoldername)
{
	if (NULL == FindFolder(dwFolderID))
	{
		CRXSkinOutbarFolder* poFolder = new CRXSkinOutbarFolder;
		poFolder->SetID(dwFolderID);
		poFolder->SetGroupName(pszFoldername);
		poFolder->SetParentWnd(GetParentWnd());
		m_oOutbarFolderMap.insert(make_pair(dwFolderID,poFolder));
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

CRXSkinOutbarFolder* CRXSkinOutbar::FindFolder(const UINT32 dwFolderID)
{
	CRXSkinOutbarFolderMapItr itr = m_oOutbarFolderMap.find(dwFolderID);
	if (itr!=m_oOutbarFolderMap.end())
	{
		return itr->second;
	}
	return NULL;
}

CRXSkinOutbarItem* CRXSkinOutbar::FindItem(const UINT32 dwUserID)
{
	CRXSkinOutbarItem* poResult = NULL;
	CRXSkinOutbarFolderMapItr itr = m_oOutbarFolderMap.begin();
	for (;itr!=m_oOutbarFolderMap.end();itr++)
	{
		poResult = itr->second->FindItem(dwUserID);
		if (poResult)
		{
			break;
		}
	}
	return poResult;
}

CRXSkinOutbarItem* CRXSkinOutbar::RemoveItem(const UINT32 dwUserID)
{
	CRXSkinOutbarItem* poResult = NULL;
	CRXSkinOutbarFolderMapItr itr = m_oOutbarFolderMap.begin();
	for (;itr!=m_oOutbarFolderMap.end();itr++)
	{
		poResult = itr->second->RemoveItem(dwUserID);
		if (poResult)
		{
			break;
		}
	}
	return poResult;
}
