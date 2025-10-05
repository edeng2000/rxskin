#include "pch.h"
#include "main/rxskinwnd.h"
#include "common/rxskinutil.h"
#include "common/rxskinservice.h"
#include <windowsx.h>
CRXSkinWnd::CRXSkinWnd()
{
	m_hWnd = NULL;
	m_OldWndProc = ::DefWindowProc;
	m_bSizing = FALSE;
	m_bDCReady = false;
	m_bAutoDelete = false;
}

CRXSkinWnd::~CRXSkinWnd()
{
	Destroy();
}

bool CRXSkinWnd::IsAutoDelete()
{
	return m_bAutoDelete;
}

void CRXSkinWnd::SetAutoDelete(const bool& bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
}

CRXControl* CRXSkinWnd::OnCreateControl(tinyxml2::XMLElement* poElement)
{
	return NULL;
}

CRXControlMgr& CRXSkinWnd::GetControlMgr()
{
	return m_oControlMgr;
}

BOOL CRXSkinWnd::Create(const TCHAR* pszWindowClassName,
						const TCHAR* pszWindowName,
						const UINT32 dwStyle,
						const HWND hParentWnd,
						HICON hIcon)
{
	if (FALSE == RegisterWindowClass(pszWindowClassName,CRXSkinWnd::_WndProc,hIcon))
	{
		int nError = GetLastError();
		if (nError!=1410)
			return FALSE;
	}
	m_hWnd = CreateWindowEx(WS_EX_COMPOSITED,pszWindowClassName,
		pszWindowName,
		dwStyle|WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hParentWnd,
		NULL,
		CRXSkinService::Instance()->GetInstance(),
		(LPVOID)this);
	if (m_hWnd)
	{
		m_hParentWnd = hParentWnd;
		CRXSkinService::Instance()->AddWnd(this);
	}
	else
	{
		DWORD dwError = GetLastError();
		printf("%d\n", dwError);
	}
	return m_hWnd?TRUE:FALSE;
}

HWND CRXSkinWnd::GetHWnd()
{
	return m_hWnd;
}

HWND CRXSkinWnd::GetParentWnd()
{
	return m_hParentWnd;
}

VOID CRXSkinWnd::Destroy()
{
	m_mapTimer.clear();
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
	}
	if (m_hDcPaint)
	{
		ReleaseDC(m_hWnd, m_hDcPaint);
		m_hDcPaint = NULL;
	}
}

bool CRXSkinWnd::IsValid()
{
	return m_hWnd != NULL && m_hDcPaint != NULL;
}

CRXSkinRect CRXSkinWnd::GetRect()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	return rc;
}

CRXSkinRect CRXSkinWnd::GetClientRectEx()
{
	CRXSkinWnd* poParent = CRXSkinService::Instance()->FindWnd(m_hParentWnd);
	if (poParent)
	{
		CRXSkinRect rcParent = poParent->GetWindowRectEx();
		CRXSkinRect rc = GetWindowRectEx();
		CRXSkinRect rcSelf;
		rcSelf.left = rc.left - rcParent.left;
		rcSelf.right = rcSelf.left + rc.Width();
		rcSelf.top = rc.top - rcParent.top;
		rcSelf.bottom = rcSelf.top + rc.Height();
		return rcSelf;
	}
	return GetRect();
}

CRXSkinRect CRXSkinWnd::GetWindowRectEx()
{
	RECT rc;
	::GetWindowRect(m_hWnd, &rc);
	return rc;
}
bool CRXSkinWnd::RegisterSuperclass(const TCHAR* pszClassName)
{
	LPWSTR lpClassName = (LPWSTR)pszClassName;

	// Get the class information from an existing
	// window so we can subclass it later on...
	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	if (!::GetClassInfoExW(NULL, pszClassName, &wc)) {
		if (!::GetClassInfoExW(CRXSkinService::Instance()->GetInstance(), pszClassName, &wc)) {
			ASSERT(!"Unable to locate window class");
			return NULL;
		}
	}
	m_OldWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = CRXSkinWnd::_WndProc;
	wc.hInstance = CRXSkinService::Instance()->GetInstance();
	wc.lpszClassName = lpClassName;
	ATOM ret = ::RegisterClassExW(&wc);
	ASSERT(ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

void CRXSkinWnd::Invalidate()
{
	CRXSkinRect rc = GetRect();
	InvalidateRect(m_hWnd, &rc,TRUE);
}

LRESULT CALLBACK CRXSkinWnd::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CRXSkinWnd* pThis = NULL;
	if( message == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CRXSkinWnd*>(lpcs->lpCreateParams);
		pThis->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
		CRXSkinService::Instance()->AddWnd(pThis);
	}  
	else {
		pThis = CRXSkinService::Instance()->FindWnd(hWnd);
		if( message == WM_NCDESTROY && pThis != NULL ) {
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, message, wParam, lParam);
			::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
			pThis->m_hWnd = NULL;
		
			CRXSkinService::Instance()->RemoveWnd(hWnd);
			if(false == pThis->IsAutoDelete())
				CRXSkinService::Instance()->AddRecycleWnd(pThis);
			return lRes;
		}
	}
	if( pThis != nullptr) {
  		return pThis->OnHandleMessage(message, wParam, lParam);
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void CRXSkinWnd::SetForgeWindow()
{
	SetWindowPos(GetHWnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CRXSkinWnd::SetTopmostWindow()
{
	SetWindowPos(GetHWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

VOID CRXSkinWnd::OnDraw(SkSurface* poSurface, HDC hDC, RECT rcClient)
{
	
}

BOOL CRXSkinWnd::OnCreate(LPCREATESTRUCT lpCreate)
 {
	m_hDcPaint = ::GetDC(m_hWnd);
	CRXSkinService::Instance()->AddWnd(this);
	return TRUE;
}

BOOL CRXSkinWnd::OnSize(WPARAM wParam, LPARAM lParam)
{
	m_bSizing = FALSE;
	return FALSE;
}

BOOL CRXSkinWnd::OnSizing(WPARAM wParam, LPARAM lParam)
{
	m_bSizing = TRUE;
	return FALSE;
}
void CRXSkinWnd::OnFinalMessage(HWND hWnd)
{
	
}

BOOL CRXSkinWnd::OnMouseMove(UINT nFlags, POINT point)
{
	return TRUE;
}

BOOL  CRXSkinWnd::OnMouseHover(UINT nFlags, POINT point)
{
	return FALSE;
}


BOOL CRXSkinWnd::OnLButtonDown(UINT nFlags, POINT point)
{
	return FALSE;
}

BOOL CRXSkinWnd::OnLButtonUp(UINT nFlags, POINT point)
{
	return FALSE;
}

BOOL CRXSkinWnd::OnLButtonDblClk(UINT nFlags, POINT point)
{
	return FALSE;
}

BOOL CRXSkinWnd::OnMButtonDown(UINT nFlags, POINT point)
{
	return FALSE;
}

BOOL CRXSkinWnd::OnMButtonUp(UINT nFlags, POINT point)
{
	return FALSE;
}

BOOL CRXSkinWnd::OnMButtonDblClk(UINT nFlags, POINT point)
{
	return FALSE;
}

BOOL CRXSkinWnd::OnRButtonDown(UINT nFlags, POINT point)
{
	return FALSE;
}
BOOL CRXSkinWnd::OnRButtonUp(UINT nFlags, POINT point)
{
	return FALSE;
}


BOOL CRXSkinWnd::OnRButtonDblClk(UINT nFlags, POINT point)
{
	return FALSE;
}


LRESULT CRXSkinWnd::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
BOOL CRXSkinWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}
BOOL CRXSkinWnd::OnMove(int x, int y)
{
	return FALSE;
}

LRESULT CRXSkinWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CRXSkinWnd::OnEditChanged(WPARAM wParam, LPARAM lParam)
{
	return m_oControlMgr.OnEditChanged(wParam,lParam);
}

LRESULT CRXSkinWnd::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT32 msgID = GET_WM_COMMAND_CMD(wParam, lParam);
	if (msgID == EN_CHANGE)
	{
		return OnEditChanged(wParam, lParam);
	}
//	return ::DefWindowProc(m_hWnd, message, wParam, lParam);fff
	switch(message)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			return OnCreate(lpcs);
		}
		break;
	case WM_PAINT:
		{
			OnPaint();
			return TRUE;
		}
		break;
	case WM_SIZE:
		{
			return OnSize(wParam,lParam);
		}
		break;
	case WM_KILLFOCUS:
	{	
		return OnKillFocus(wParam, lParam);
	}
	break;
	case WM_SETFOCUS:
	{
		return OnSetFocus(wParam, lParam);
	}
	case WM_SIZING:
		{
			return OnSizing(wParam,lParam);
		}
		break;
	case OCM_COMMAND:
	{
		if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
		{
			printf("");
		}
		else if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE) {
			printf("");
		}
	}
	break;
	case WM_CTLCOLORSTATIC:
	case  OCM__BASE + WM_CTLCOLOREDIT :
	case OCM__BASE + WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
		{
			return (LRESULT) OnCtlColorControl(message,(HWND) lParam,(HDC)wParam);
		}
		break;
	case WM_ERASEBKGND:
		{
			return TRUE;
		}
	case WM_SETCURSOR:
		{
			return TRUE;
		}

	case WM_MOVE:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			return OnMove(pt.x,pt.y);
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			
			return OnMouseMove((UINT)wParam, pt);
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			return OnLButtonUp((UINT)wParam,pt);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			return OnLButtonDown((UINT)wParam,pt);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			return OnLButtonDblClk((UINT)wParam,pt);
		}
		break;

	case WM_MBUTTONUP:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		return OnMButtonUp((UINT)wParam, pt);
	}
	break;
	case WM_MBUTTONDOWN:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		return OnMButtonDown((UINT)wParam, pt);
	}
	break;
	case WM_MBUTTONDBLCLK:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		return OnMButtonDblClk((UINT)wParam, pt);
	}
	break;

	case WM_RBUTTONUP:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		return OnRButtonUp((UINT)wParam, pt);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		return OnRButtonDown((UINT)wParam, pt);
	}
	break;
	case WM_RBUTTONDBLCLK:
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		return OnRButtonDblClk((UINT)wParam, pt);
	}
	break;

	case WM_MOUSEHOVER:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			return OnMouseHover((UINT)wParam,pt);
		}
		break;
	case WM_MOUSELEAVE:
		{
			OnMouseLeave(wParam,lParam);
			return TRUE;
		}
		break;
	case WM_TIMER:
		{
			return OnTimer(wParam,lParam);
		}
		break;
	case WM_RXSKIN_SCROLLBAR_POS:
		{
			OnScrollBarChanged((EScrollType)wParam,(INT32)lParam);
			return TRUE;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			OnMouseWheel((UINT)LOWORD(wParam),(short)HIWORD(wParam),pt);
			return TRUE;
		}
		break;
	case WM_CANCELMODE:
		{
			OnCancelMode();
			return TRUE;
		}
		break;
	case WM_ACTIVATE:
		{
			OnActive(wParam,(HWND)lParam);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		{
			return OnCommand(wParam,lParam);
		}
	case WM_GETMINMAXINFO:
		{
			return OnGetMinMaxInfo(wParam,lParam);
		}
	case WM_SYSCOMMAND:
	{
		LRESULT result = OnSystemCommand(wParam, lParam);
		if (result>0)
		{
			return result;
		}
		break;
	}
	case  WM_KEYDOWN:
	{
		return OnKeyDown(wParam, lParam);
	}
	break;
	case  WM_SHOWWINDOW:
	{
		return OnShowWindow(wParam, lParam);
	}
	break;
	case  WM_KEYUP:
	{
		return OnKeyUp(wParam, lParam);
	}
	break;
	}

	if (message>WM_USER)
	{
		return OnMessage(message,wParam,lParam);
	}
//	return OnMessage(message, wParam, lParam);
	return ::DefWindowProc(m_hWnd, message, wParam, lParam);
}   

BOOL CRXSkinWnd::OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

LRESULT  CRXSkinWnd::OnSysKeydown(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT  CRXSkinWnd::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CRXSkinWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


LRESULT CRXSkinWnd::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CRXSkinWnd::OnSysChar(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


BOOL  CRXSkinWnd::OnPreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
{
	return TRUE;
}

VOID CRXSkinWnd::OnPaint()
{
	m_bDCReady = true;
}

void CRXSkinWnd::MoveWindowEx(RECT rc)
{
	//::SetWin
	if (m_hWnd) {
		::SetWindowPos(m_hWnd,
			NULL,
			rc.left,
			rc.top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			SWP_NOZORDER);
	}
	//::MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
}

VOID CRXSkinWnd::OnActive(WPARAM nState, HWND hWnd)
{

}

void CRXSkinWnd::OnCancelMode()
{

}

LRESULT CRXSkinWnd::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	m_oControlMgr.OnKillFocus(wParam, lParam);
	return 0;
}

LRESULT CRXSkinWnd::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	m_oControlMgr.OnSetFocus(wParam, lParam);
	return 0;
}

void CRXSkinWnd::AddAnchor(HWND hWnd, CRXSkinResizePoint ptTopLeft, CRXSkinResizePoint ptBottomRight)
{

}

void CRXSkinWnd::SendMessage(const UINT32& dwMsgID, const WPARAM& wParam /* = NULL */, const LPARAM& lParam /* = NULL */)
{
	::SendMessage(m_hWnd, dwMsgID, wParam, lParam);
}

HBRUSH CRXSkinWnd::OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC)
{
	HBRUSH brush = m_oControlMgr.OnCtlColorControl(dwMsgID,hWndChild, hDC);
	if (brush)
	{
		return brush;
	}
	return (HBRUSH)::DefWindowProc(m_hWnd, dwMsgID, (WPARAM)hWndChild, (LPARAM)hDC);
}

LRESULT CRXSkinWnd::OnShowWindow(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL CRXSkinWnd::OnTimer(WPARAM wParam, LPARAM lParam)
{
	vector<STimerEvent>* poVec = FindTimer(wParam);
	if (poVec)
	{
		for (uint32_t t=0;t<(uint32_t)poVec->size();t++)
		{
			CRXControl* poControl = m_oControlMgr.FindControl((*poVec)[t].dwControlID);
			if (poControl)
			{
				poControl->OnTimer(wParam, lParam);
			}
		}
	}
	return FALSE;
}

BOOL CRXSkinWnd::OnMouseWheel(UINT nFlags, short zDelta,POINT pt)
{
	return FALSE;
}

VOID CRXSkinWnd::OnScrollBarChanged(EScrollType enType,INT32 nPercent)
{
}

LRESULT CRXSkinWnd::OnSystemCommand(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CRXSkinWnd::SetCapture()
{
	::SetCapture(m_hWnd);
}

CRXControl* CRXSkinWnd::HitTest(POINT pt)
{
	return m_oControlMgr.HitTest(pt);
}

void CRXSkinWnd::ReleaseCapture()
{
	::ReleaseCapture();
}

bool CRXSkinWnd::SetTimer(CRXControl* poControl, int nEventID, int nTimeOut)
{
	vector<CRXSkinWnd::STimerEvent>* poVec = FindTimer(nEventID);
	if (NULL == poVec)
	{
		STimerEvent oEvent;
		oEvent.dwControlID = poControl->GetID();
		oEvent.dwEventID = nEventID;
		oEvent.dwInterval = nTimeOut;
		vector<CRXSkinWnd::STimerEvent> oVec;
		oVec.push_back(oEvent);
		m_mapTimer.insert(make_pair(nEventID, oVec));
	}
	else
	{
		bool bFind = false;
		for (size_t i = 0; i < (uint32_t)poVec->size(); i++)
		{
			if ((*poVec)[i].dwControlID == poControl->GetID())
			{
				bFind = true;
				break;
			}
		}
		if (bFind == false)
		{
			STimerEvent oEvent;
			oEvent.dwControlID = poControl->GetID();
			oEvent.dwEventID = nEventID;
			oEvent.dwInterval = nTimeOut;
			poVec->push_back(oEvent);
		}
	}
	::SetTimer(m_hWnd, nEventID, nTimeOut, NULL);
	INT32 nError = GetLastError();
	printf("");
	return true;
}

vector<CRXSkinWnd::STimerEvent>* CRXSkinWnd::FindTimer(const uint32_t& dwEventID)
{	
	map<uint32_t, vector<STimerEvent>>::iterator itr = m_mapTimer.find(dwEventID);
	if (itr==m_mapTimer.end())
	{
		return NULL;
	}
	return &(itr->second);
}

void   CRXSkinWnd::RemoveTimer(const uint32_t& dwEventID, const uint32_t& dwControlID)
{
	vector<CRXSkinWnd::STimerEvent>* poVec = FindTimer(dwEventID);
	if (poVec)
	{
		for (vector<CRXSkinWnd::STimerEvent>::iterator itr = poVec->begin();itr!=poVec->end();itr++)
		{
			if ((*itr).dwControlID == dwControlID)
			{
				poVec->erase(itr);
			}
		}
	}
}

void CRXSkinWnd::KillTimer(CRXControl* poControl, int nEventID)
{
	RemoveTimer(nEventID, poControl->GetID());
}

void CRXSkinWnd::Show()
{
	if(m_hWnd)
		::ShowWindow(m_hWnd, SW_SHOW);
}

void CRXSkinWnd::Hide()
{
	if (m_hWnd)
	{
		::ShowWindow(m_hWnd, SW_HIDE);
	}
	
}

bool CRXSkinWnd::IsVisible()
{
	return ::IsWindowVisible(m_hWnd);
}