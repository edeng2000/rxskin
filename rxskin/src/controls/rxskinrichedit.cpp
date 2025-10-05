#include "pch.h"
#include "windows.h"
#include "common/rximage.h"
#include "controls\RXSkinRichEdit.h"
#include "common/graphics.h"
#include <olectl.h>
#include "windowsx.h"
#include "commctrl.h"
#include "WinUser.h"
#include "common/RXSkinService.h"
#include "common/rxskindefines.h"
#include "fundation/rxtime.h"
#include <Windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <Richedit.h>
#include <richole.h>
#include <TextServ.h>
#include <tom.h>
#include <memory>
#include <sstream>

#if (defined(WIN32) || defined(WIN64))
#define UNICODE_CHARSET      (char*)"UCS-2LE"
#else
#define UNICODE_CHARSET      (char*)"UCS-4LE"
#endif

#define ASCII_CHARSET               (char*)"ASCII"
#define UTF8_CHARSET                (char*)"UTF-8"
#define GBK_CHARSET                 (char*)"GBK"
EXTERN_C const IID IID_ITextServices = // 8d33f740-cf58-11ce-a89d-00aa006cadc5
{ 0x8d33f740, 0xcf58, 0x11ce, { 0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 } };
EXTERN_C const IID IID_ITextHost = /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
{ 0xc5bdd8d0, 0xd26e, 0x11ce, { 0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 } };

CRXSkinRichEditBody::CRXSkinRichEditBody()
{
	m_poOwner = NULL;
	m_poTextHost = NULL;
	m_nfuSBFlags = 0;
//	m_oScrollbarHor.SetOwner(this);
	m_oScrollbarHor.SetType(SCROLLBAR_HOR);
//	m_oScrollbarVer.SetOwner(this);
	m_oScrollbarVer.SetType(SCROLLBAR_VER);
}

CRXSkinRichEditBody::~CRXSkinRichEditBody()
{

}



BOOL CRXSkinRichEditBody::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinChildWnd::OnLButtonUp(nFlags, point);
	return m_oScrollbarVer.OnClick(point);
}

BOOL CRXSkinRichEditBody::OnTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinChildWnd::OnTimer(wParam, lParam);
	return bResult;
}

BOOL CRXSkinRichEditBody::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = CRXSkinChildWnd::OnLButtonDown(nFlags, point);
	if (FALSE == bClicked)
	{
	}
	return bClicked;
}

CRXControl* CRXSkinRichEditBody::OnCreateControl(tinyxml2::XMLElement* poElement)
{
	char* pszType = (char*)poElement->Attribute("type");

	return NULL;
}

BOOL CRXSkinRichEditBody::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinChildWnd::OnCreate(lpCreate);
	if (bResult)
	{
		m_poTextHost = new CRXSkinTextHost(this);
		m_poTextHost->Create();
		m_oScrollbarVer.SetParentWnd(GetHWnd());
		m_oScrollbarHor.SetParentWnd(GetHWnd());

	}
	return bResult;
}

DWORD CALLBACK EditStreamInCallbackMem(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	CRXSkinRichEditBody* poThis = (CRXSkinRichEditBody*)dwCookie;
	uint32_t nReaded = 0;
	CRXSkinRichEditBody::MemBlock& oMem = poThis->GetMemory();
	if (oMem.pBuf && oMem.dwLen - oMem.dwCurPos>= cb)
	{
		memcpy(pbBuff, oMem.pBuf+oMem.dwCurPos, cb);
		oMem.dwCurPos += cb;
		if (pcb)
			*pcb = cb;
	}
	else
	{
		if (oMem.pBuf && oMem.dwCurPos<oMem.dwLen)
		{
			uint32_t dwRead = oMem.dwLen - oMem.dwCurPos;
			memcpy(pbBuff, oMem.pBuf + oMem.dwCurPos, dwRead);
			oMem.dwCurPos += dwRead;
			if (pcb)
				*pcb = dwRead;
		}
	}
	return 0;
}


void CRXSkinRichEditBody::SetText(const BYTE* pszText, const uint32_t& dwLen)
{
	if (m_poTextHost==NULL)
	{
		return;
	}
	if (m_poTextHost->GetTextServices() ==nullptr)
	{
		return;
	}

	m_oMem.pBuf = (BYTE*)malloc(dwLen);
	memcpy(m_oMem.pBuf, pszText, dwLen);
	m_oMem.dwLen = dwLen;
	m_oMem.dwCurPos = 0;
	EDITSTREAM es;
	es.dwCookie = (DWORD_PTR)this;
	es.pfnCallback = EditStreamInCallbackMem;
	LRESULT nResult = 0;
	DWORD dwRet = (DWORD)m_poTextHost->GetTextServices()->TxSendMessage(EM_STREAMIN, SF_RTF, (LPARAM)&es,&nResult);


}

void CRXSkinRichEditBody::OnClickedClose()
{

}

BOOL CRXSkinRichEditBody::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd, HICON hIcon /* = NULL */)
{
	BOOL bResult = CRXSkinChildWnd::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
	if (bResult)
	{
	}
	return bResult;
}

RECT CRXSkinRichEditBody::GetEditRect()
{
	RECT rc;
	::GetClientRect(GetHWnd(), &rc);
	if (m_oScrollbarHor.IsVisible())
	{
		rc.bottom -= m_oScrollbarHor.GetHeight();
	}
	if (m_oScrollbarVer.IsVisible())
	{
		rc.right -= m_oScrollbarVer.GetWidth();

	}
	return rc;
}

BOOL CRXSkinRichEditBody::EnableScrollbar(INT fuSBFlags)
{
	switch (fuSBFlags)
	{
	case SB_BOTH:
	{
		m_oScrollbarVer.SetVisible(TRUE);
		m_oScrollbarHor.SetVisible(TRUE);
	}
	break;
	case SB_VERT:
	{
		m_oScrollbarVer.SetVisible(TRUE);
	
	}
	break;
	case SB_HORZ:
	{
		m_oScrollbarHor.SetVisible(TRUE);
	}
	break;
	}
	m_nfuSBFlags |= fuSBFlags;
	return TRUE;
}

BOOL CRXSkinRichEditBody::OnMouseMove(UINT nFlags, POINT point)
{
	return CRXSkinChildWnd::OnMouseMove(nFlags, point);
}
void CRXSkinRichEditBody::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	DrawRect(hDC,
		CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBorder(),
		CRXSkinService::Instance()->GetDrawMgr().GetListCtrlBK(),
		rcItem.left,
		rcItem.top,
		rcItem.right,
		rcItem.bottom);
	if (NULL == m_poTextHost)
	{
		return;
	}
	if (NULL == m_poTextHost->GetTextServices())
	{
		return;
	}
	if (m_poTextHost->GetTextServices())
	{
		RECT rc = GetEditRect();
		m_poTextHost->GetTextServices()->TxDraw(
			DVASPECT_CONTENT,
			0,
			nullptr,
			nullptr,
			hDC,
			nullptr,
			reinterpret_cast<LPCRECTL>(&rc),
			nullptr,
			nullptr,
			nullptr,
			0,
			0
		);
	}
	m_oScrollbarHor.OnDraw(poSurface,hDC, rcItem);
	m_oScrollbarVer.OnDraw(poSurface,hDC, rcItem);
}

CComPtr<MyOLEObject> CRXSkinRichEditBody::GetOLEObjectAtMouseCursor() {

// 	POINT position{};
// 	GetCursorPos(&position);
// 
// 	CComPtr<IRichEditOle> rich_edit_ole;
// 	g_text_service->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&rich_edit_ole, nullptr);
// 
// 	CComPtr<ITextDocument> text_document;
// 	HRESULT hresult = rich_edit_ole->QueryInterface(IID_ITextDocument, reinterpret_cast<void**>(&text_document));
// 	if (FAILED(hresult)) {
// 		return nullptr;
// 	}
// 
// 	CComPtr<ITextRange> text_range;
// 	hresult = text_document->RangeFromPoint(position.x, position.y, &text_range);
// 	if (FAILED(hresult)) {
// 		return nullptr;
// 	}
// 
// 	CComPtr<IUnknown> ole_object;
// 	hresult = text_range->GetEmbeddedObject(&ole_object);
// 	if (FAILED(hresult)) {
// 		return nullptr;
// 	}
// 
// 	CComPtr<MyOLEObject> my_ole_object(dynamic_cast<MyOLEObject*>(ole_object.p));
	return NULL;
}
LRESULT CRXSkinRichEditBody::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RXSKIN_SCROLLBAR_POS:
	{
		if (wParam == SCROLLBAR_VER)
		{
		}
		if (wParam == SCROLLBAR_HOR)
		{
		}
	}
	break;
	default:
		break;
	}
	return CRXSkinChildWnd::OnHandleMessage(message, wParam, lParam);
}

LRESULT CRXSkinRichEditBody::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	m_poTextHost->GetTextServices()->TxSendMessage(WM_KEYDOWN, wParam, lParam, &result);

	return result;
}
LRESULT CRXSkinRichEditBody::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	m_poTextHost->GetTextServices()->TxSendMessage(WM_KEYUP, wParam, lParam, &result);

	return result;
}
LRESULT CRXSkinRichEditBody::OnSysChar(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	m_poTextHost->GetTextServices()->TxSendMessage(WM_SYSCHAR, wParam, lParam, &result);

	return result;
}
LRESULT CRXSkinRichEditBody::OnSysKeydown(WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	m_poTextHost->GetTextServices()->TxSendMessage(WM_SYSKEYDOWN, wParam, lParam, &result);

	return result;
}

BOOL CRXSkinRichEditBody::OnMessage(UINT32 dwMessageID, WPARAM wParam, LPARAM lParam)
{
	return CRXSkinChildWnd::OnMessage(dwMessageID,
		wParam,
		lParam);
}

BOOL CRXSkinRichEditBody::OnRButtonUp(UINT nFlags, POINT point)
{

	return m_oControlMgr.OnRButtonUp(point) == NULL ? FALSE : TRUE;
}

BOOL CRXSkinRichEditBody::OnSize(WPARAM wParam, LPARAM lParam)
{
	return CRXSkinChildWnd::OnSize(wParam, lParam);

}

BOOL CRXSkinRichEditBody::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CRXSkinChildWnd::OnCommand(wParam, lParam);
	return TRUE;
}

LRESULT CRXSkinRichEditBody::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	if (nullptr == m_poTextHost->GetTextServices())
	{
		return 0;
	}
	m_poTextHost->GetTextServices()->OnTxInPlaceDeactivate();

	LRESULT result = 0;
	m_poTextHost->GetTextServices()->TxSendMessage(WM_KILLFOCUS, wParam, lParam, &result);

	return 0;
}
LRESULT CRXSkinRichEditBody::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	if (nullptr == m_poTextHost->GetTextServices())
	{
		return 0;
	}
	m_poTextHost->GetTextServices()->OnTxInPlaceActivate(nullptr);

	LRESULT result = 0;
	m_poTextHost->GetTextServices()->TxSendMessage(WM_SETFOCUS, wParam, lParam, &result);
	return 0;
}



LRESULT CRXSkinRichEditBody::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;

	return 0;
}

VOID CRXSkinRichEditBody::OnScrollBarChanged(EScrollType enType, INT32 nPercent)
{
	
}

BOOL CRXSkinRichEditBody::OnLButtonDblClk(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinChildWnd::OnLButtonDblClk(nFlags, point);
	return bResult;
}

BOOL CRXSkinRichEditBody::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	BOOL bResult = CRXSkinChildWnd::OnMouseWheel(nFlags, zDelta, pt);
	return bResult;
}


CRXSkinRichEdit::CRXSkinRichEdit(void)
{
	m_strTipValue = _T("");
	m_clrTipText = RGB(0, 0, 0);
	m_clrTipBG = RGB(255, 255, 255);
	m_bIsPassword = false;
	m_bIsReadOnly = false;
	m_bIsMultLine = false;
	m_hBkBrush = NULL;
	m_clrEditTextColor = RGB(50, 250, 50);
	m_clrEditBKColor = RGB(201, 212, 215);
	m_clrBorder = RGB(201, 212, 215);

}

CRXSkinRichEdit::~CRXSkinRichEdit(void)
{
	_DeleteEdit();

}

HWND CRXSkinRichEdit::GetEditWnd()
{
	return m_wndBody.GetHWnd();
}


CRXSkinString CRXSkinRichEdit::GetWindowText()
{
	CRXSkinString strContent;
	return strContent;
}

void CRXSkinRichEdit::Destroy()
{
	
}

VOID CRXSkinRichEdit::_ShowEdit()
{
	if (IsVisible() == FALSE || IsEnabled() == FALSE)
	{
		return;
	}
	CRXSkinRect rc = m_rcItem;
	rc.left += 2;
	rc.top += 4;
	rc.bottom -= 2;
	rc.right -= 2;
	m_wndBody.SetOwner(this);
	m_wndBody.Create(_T("RXSkinRichEdit"),
		_T("RXSkinRichEditWnd"),WS_CHILD|WS_VISIBLE,GetParentWnd(),NULL);
	m_wndBody.MoveWindow(rc);
	

}


void CRXSkinRichEdit::SetDefaultColorFont(int nSize, const TCHAR* face)
{
}


BOOL CRXSkinRichEdit::Create(const UINT32 dwID, CRXSkinRect rcClient, const BOOL bIsPassword)
{
	m_bIsPassword = bIsPassword;
	SetRect(rcClient);
	if (m_strTipValue.GetLength() <= 0)
	{
		_ShowEdit();
	}
	return TRUE;
}

VOID CRXSkinRichEdit::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	if (bIsVisible)
	{
		m_wndBody.Show();
	}
	else
	{
		m_wndBody.Hide();
	}
}

VOID CRXSkinRichEdit::Enable(const BOOL bEnabled)
{
	CRXControl::Enable(bEnabled);
	
}


VOID CRXSkinRichEdit::_DeleteEdit()
{
	m_wndBody.Destroy();
}

BOOL CRXSkinRichEdit::OnLButtonUp(POINT pt)
{
	if (m_rcItem.PtInRect(pt))
	{
		_DeleteEdit();
		_ShowEdit();
		return TRUE;
	}
	return FALSE;
}

VOID CRXSkinRichEdit::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	if (IsVisible() == FALSE)
	{
		return;
	}
	SetRect(rcItem);

	if (IsBorder())
	{
		DrawRectEdge(hDC, rcItem, 1, CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor());
	}



}

BOOL CRXSkinRichEdit::OnClick(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{

			return TRUE;
		}
		else
		{
			CRXSkinString strContent = GetWindowText();
			if (strContent.GetLength() <= 0 && m_strTipValue.GetLength() > 0)
			{
				_DeleteEdit();
			}
		}
	}
	return FALSE;
}

BOOL CRXSkinRichEdit::OnLButtonDown(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXSkinRichEdit::PtInRect(POINT pt)
{
	if (GetRect().PtInRect(pt))
	{
		return TRUE;
	}
	return FALSE;
}


BOOL CRXSkinRichEdit::OnMouseMove(POINT pt)
{
	if (IsEnabled() && IsVisible())
	{
		if (PtInRect(pt))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRXSkinRichEdit::OnSize(const UINT32 dwX, const UINT32 dwY)
{
	CRXControl::OnSize(dwX, dwY);
	return TRUE;
}

VOID CRXSkinRichEdit::ReleaseControl()
{
	delete this;
}

void  CRXSkinRichEdit::MoveWindow(const CRXSkinRect& rcItem)
{

}


HBRUSH CRXSkinRichEdit::OnCtlColorControl(HWND hWndChild, HDC hDC)
{
	CRXSkinWnd* poWnd = CRXSkinService::Instance()->FindWnd(GetParentWnd());


	return (HBRUSH)m_hBkBrush;
}


void CRXSkinRichEdit::SetText(LPCTSTR pstrText)
{
	m_strContent = pstrText;
	m_wndBody.SetText((const BYTE*)pstrText,_RXTStrlen(pstrText));
}

void CRXSkinRichEdit::AppendText(const TCHAR* pszText)
{

}

void CRXSkinRichEdit::SetData(const BYTE* pszData ,const uint32_t& dwLen)
{
	m_wndBody.SetText(pszData, dwLen);
}

void CRXSkinRichEdit::ClearAll()
{

}

LRESULT CRXSkinRichEdit::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CRXSkinRichEdit::OnSetFocus(WPARAM wParam, LPARAM lParam)
{

	return 0;
}