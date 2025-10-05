#include "pch.h"
#include "controls/rxskinwebbrowser.h"
#include <atlbase.h>
#include <exdispid.h>
#include "common/graphics.h"
#include "windowsx.h"
#include "commctrl.h"
#include <shlguid.h>

CRXSkinWebBrowser::CRXSkinWebBrowser(HWND _hWndParent):m_pCmdTarg(NULL)
{
	m_pOleObject = nullptr;
	m_pOleInPlaceObject = nullptr;
	m_pWebBrowser2 = nullptr;
	m_iComRefCount = 0;
	m_hWndParent = nullptr;
	m_hWndControl = nullptr;
	m_poWebBrowserEventHandler = NULL;
	m_iComRefCount = 0;
	//::SetRect(&rObject, -300, -300, 300, 300);
	m_hWndParent = _hWndParent;
	m_pDocument = NULL;	
	m_pDispatch = NULL;
	m_hWebBrowserWnd = NULL;
	m_clrBorder = CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor();
}

bool CRXSkinWebBrowser::CreateBrowser()
{
	HRESULT hr;
	hr = ::OleCreate(CLSID_WebBrowser,
		IID_IOleObject, OLERENDER_DRAW, 0, this, this,
		(void**)&m_pOleObject);

	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = m_pOleObject->SetClientSite(this);
	hr = OleSetContainedObject(m_pOleObject, TRUE);


	hr = m_pOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE,
		NULL, this, -1, m_hWndParent, &m_rcObject);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("oleObject->DoVerb() failed"),
			_T("Error"),
			MB_ICONERROR);
		return FALSE;
	}

	hr = m_pOleObject->QueryInterface(&m_pWebBrowser2);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("oleObject->QueryInterface(&webBrowser2) failed"),
			_T("Error"),
			MB_ICONERROR);
		return FALSE;
	}
	DWORD dwCookie = 0;
	AtlAdvise(static_cast<IUnknown*>(m_pWebBrowser2), static_cast<IDispatch*>(this), DIID_DWebBrowserEvents2, &dwCookie);

	GetControlWindow();
	ShowWindow(m_hWndControl, SW_SHOW);

	return TRUE;
}

RECT CRXSkinWebBrowser::PixelToHiMetric(const RECT& _rc)
{
	static bool s_initialized = false;
	static int s_pixelsPerInchX, s_pixelsPerInchY;
	if (!s_initialized)
	{
		HDC hdc = ::GetDC(0);
		s_pixelsPerInchX = ::GetDeviceCaps(hdc, LOGPIXELSX);
		s_pixelsPerInchY = ::GetDeviceCaps(hdc, LOGPIXELSY);
		::ReleaseDC(0, hdc);
		s_initialized = true;
	}

	RECT rc;
	rc.left = MulDiv(2540, _rc.left, s_pixelsPerInchX);
	rc.top = MulDiv(2540, _rc.top, s_pixelsPerInchY);
	rc.right = MulDiv(2540, _rc.right, s_pixelsPerInchX);
	rc.bottom = MulDiv(2540, _rc.bottom, s_pixelsPerInchY);
	return rc;
}

void CRXSkinWebBrowser::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
	m_rcObject = rcItem;

	{
		RECT hiMetricRect = PixelToHiMetric(m_rcObject);
		SIZEL sz;
		sz.cx = hiMetricRect.right - hiMetricRect.left;
		sz.cy = hiMetricRect.bottom - hiMetricRect.top;
		if(m_pOleObject)
			m_pOleObject->SetExtent(DVASPECT_CONTENT, &sz);
	}

	if (m_pOleInPlaceObject != 0)
	{
		m_pOleInPlaceObject->SetObjectRects(&m_rcObject, &m_rcObject);
	}
}

void CRXSkinWebBrowser::Refresh()
{
	this->m_pWebBrowser2->Refresh();
}

void CRXSkinWebBrowser::Navigate(LPCWSTR lpszUrl)
{
	bstr_t url(lpszUrl);
	variant_t flags(0x02u); //navNoHistory
	this->m_pWebBrowser2->Navigate(url, &flags, 0, 0, 0);
}

// ----- IUnknown -----

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::QueryInterface(REFIID riid,
	void** ppvObject)
{
	if (riid == __uuidof(IUnknown))
	{
		(*ppvObject) = static_cast<IOleClientSite*>(this);
	}
	else if (riid == __uuidof(IOleInPlaceSite))
	{
		(*ppvObject) = static_cast<IOleInPlaceSite*>(this);
	}
	else if (riid == __uuidof(IOleCommandTarget))
	{
		(*ppvObject) = static_cast<IOleCommandTarget*>(this);
	}
	else if (riid == __uuidof(IDocHostUIHandler))
	{
		(*ppvObject) = static_cast<IDocHostUIHandler*>(this);
	}
	else if (riid == __uuidof(IDispatch))
	{
		(*ppvObject) = static_cast<IDispatch*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CRXSkinWebBrowser::AddRef(void)
{
	m_iComRefCount++;
	return m_iComRefCount;
}

ULONG STDMETHODCALLTYPE CRXSkinWebBrowser::Release(void)
{
	m_iComRefCount--;
	return m_iComRefCount;
}
BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	LPRECT rcParent;
	int idChild;
	idChild = GetWindowLong(hwndChild, GWL_ID);

	TCHAR szText[256] = { 0 };
	GetClassName(hwndChild, szText, 256);
	if (_tcscmp(szText,_T("Internet Explorer_Server"))==0)
	{
		*(HWND*)lParam = hwndChild;
	}
	rcParent = (LPRECT)lParam;
	return TRUE;
}

HWND CRXSkinWebBrowser::GetBrowserWindow()
{
	if (m_hWebBrowserWnd==NULL)
	{
		EnumChildWindows(m_hWndControl, EnumChildProc, (LPARAM)&m_hWebBrowserWnd);
	}
	return m_hWebBrowserWnd;
}

// ---------- IOleWindow ----------

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::GetWindow(
	__RPC__deref_out_opt HWND* phwnd)
{
	(*phwnd) = m_hWndParent;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::ContextSensitiveHelp(
	BOOL fEnterMode)
{
	return E_NOTIMPL;
}

// ---------- IOleInPlaceSite ----------

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::CanInPlaceActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OnInPlaceActivate(void)
{
	OleLockRunning(m_pOleObject, TRUE, FALSE);
	m_pOleObject->QueryInterface(&m_pOleInPlaceObject);
	m_pOleInPlaceObject->SetObjectRects(&m_rcObject, &m_rcObject);

	return S_OK;

}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OnUIActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::GetWindowContext(
	__RPC__deref_out_opt IOleInPlaceFrame** ppFrame,
	__RPC__deref_out_opt IOleInPlaceUIWindow** ppDoc,
	__RPC__out LPRECT lprcPosRect,
	__RPC__out LPRECT lprcClipRect,
	__RPC__inout LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	HWND hwnd = m_hWndParent;

	(*ppFrame) = NULL;
	(*ppDoc) = NULL;
	(*lprcPosRect).left = m_rcObject.left;
	(*lprcPosRect).top = m_rcObject.top;
	(*lprcPosRect).right = m_rcObject.right;
	(*lprcPosRect).bottom = m_rcObject.bottom;
	*lprcClipRect = *lprcPosRect;

	lpFrameInfo->fMDIApp = false;
	lpFrameInfo->hwndFrame = hwnd;
	lpFrameInfo->haccel = NULL;
	lpFrameInfo->cAccelEntries = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::Scroll(
	SIZE scrollExtant)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OnUIDeactivate(
	BOOL fUndoable)
{
	return S_OK;
}

HWND CRXSkinWebBrowser::GetControlWindow()
{
	if (m_hWndControl != 0)
		return m_hWndControl;

	if (m_pOleInPlaceObject == 0)
		return 0;

	m_pOleInPlaceObject->GetWindow(&m_hWndControl);
	return m_hWndControl;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OnInPlaceDeactivate(void)
{
	m_hWndControl = 0;
	m_pOleInPlaceObject = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::DiscardUndoState(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::DeactivateAndUndo(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OnPosRectChange(
	__RPC__in LPCRECT lprcPosRect)
{
	return E_NOTIMPL;
}

// ---------- IOleClientSite ----------

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::SaveObject(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::GetMoniker(
	DWORD dwAssign,
	DWORD dwWhichMoniker,
	__RPC__deref_out_opt IMoniker** ppmk)
{
	if ((dwAssign == OLEGETMONIKER_ONLYIFTHERE) &&
		(dwWhichMoniker == OLEWHICHMK_CONTAINER))
		return E_FAIL;

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::GetContainer(
	__RPC__deref_out_opt IOleContainer** ppContainer)
{
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::ShowObject(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OnShowWindow(
	BOOL fShow)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::RequestNewObjectLayout(void)
{
	return E_NOTIMPL;
}

// ----- IStorage -----

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::CreateStream(
	__RPC__in_string const OLECHAR* pwcsName,
	DWORD grfMode,
	DWORD reserved1,
	DWORD reserved2,
	__RPC__deref_out_opt IStream** ppstm)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OpenStream(
	const OLECHAR* pwcsName,
	void* reserved1,
	DWORD grfMode,
	DWORD reserved2,
	IStream** ppstm)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::CreateStorage(
	__RPC__in_string const OLECHAR* pwcsName,
	DWORD grfMode,
	DWORD reserved1,
	DWORD reserved2,
	__RPC__deref_out_opt IStorage** ppstg)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::OpenStorage(
	__RPC__in_opt_string const OLECHAR* pwcsName,
	__RPC__in_opt IStorage* pstgPriority,
	DWORD grfMode,
	__RPC__deref_opt_in_opt SNB snbExclude,
	DWORD reserved,
	__RPC__deref_out_opt IStorage** ppstg)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::CopyTo(
	DWORD ciidExclude,
	const IID* rgiidExclude,
	__RPC__in_opt  SNB snbExclude,
	IStorage* pstgDest)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::MoveElementTo(
	__RPC__in_string const OLECHAR* pwcsName,
	__RPC__in_opt IStorage* pstgDest,
	__RPC__in_string const OLECHAR* pwcsNewName,
	DWORD grfFlags)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::Commit(
	DWORD grfCommitFlags)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::Revert(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::EnumElements(
	DWORD reserved1,
	void* reserved2,
	DWORD reserved3,
	IEnumSTATSTG** ppenum)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::DestroyElement(
	__RPC__in_string const OLECHAR* pwcsName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::RenameElement(
	__RPC__in_string const OLECHAR* pwcsOldName,
	__RPC__in_string const OLECHAR* pwcsNewName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::SetElementTimes(
	__RPC__in_opt_string const OLECHAR* pwcsName,
	__RPC__in_opt const FILETIME* pctime,
	__RPC__in_opt const FILETIME* patime,
	__RPC__in_opt const FILETIME* pmtime)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::SetClass(
	__RPC__in REFCLSID clsid)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::SetStateBits(
	DWORD grfStateBits,
	DWORD grfMask)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CRXSkinWebBrowser::Stat(
	__RPC__out STATSTG* pstatstg,
	DWORD grfStatFlag)
{
	return E_NOTIMPL;
}


HRESULT CRXSkinWebBrowser::QueryStatus(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD* prgCmds, OLECMDTEXT* pCmdText)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::Exec(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT* pvaIn, VARIANT* pvaOut)
{
	if (!pguidCmdGroup || !IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
		return OLECMDERR_E_UNKNOWNGROUP;
	
	// 不弹出查找框
	if (nCmdID == OLECMDID_FIND)
	{
		return S_OK;
	}

	else if (nCmdID == OLECMDID_SAVEAS)
	{
		// 不要弹保存对话框
		return S_OK;
	}
	else if (nCmdID == OLECMDID_SHOWSCRIPTERROR)
	{
		// 屏蔽脚本错误的对话框
		(*pvaOut).vt = VT_BOOL;
		(*pvaOut).boolVal = VARIANT_TRUE;
		return S_OK;
	}
	else if (nCmdID == OLECMDID_SHOWMESSAGE)
	{
		(*pvaOut).vt = VT_BOOL;
		(*pvaOut).boolVal = VARIANT_TRUE;
		return S_OK;
	}

	return OLECMDERR_E_NOTSUPPORTED;
}

// IDocHostUIHandler
HRESULT CRXSkinWebBrowser::ShowContextMenu(DWORD dwID,
	POINT* pptScreen,
	IUnknown* pcmdtReserved,
	IDispatch* pdispReserved)
{

	if (m_poWebBrowserEventHandler)
	{
		return m_poWebBrowserEventHandler->ShowContextMenu(this, dwID, pptScreen, pcmdtReserved, pdispReserved);
	}
	return S_OK;
}

HRESULT CRXSkinWebBrowser::GetHostInfo(DOCHOSTUIINFO* pInfo)
{
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_THEME |
		DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE  
		;
	return S_OK;
}


HRESULT CRXSkinWebBrowser::ShowUI(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget,
	IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc)
{
	return S_FALSE;
}

HRESULT CRXSkinWebBrowser::HideUI(void)
{
	return S_FALSE;
}

HRESULT CRXSkinWebBrowser::UpdateUI(void)
{
	return S_FALSE;
}

HRESULT CRXSkinWebBrowser::EnableModeless(BOOL fEnable)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::OnDocWindowActivate(BOOL fActivate)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::OnFrameWindowActivate(BOOL fActivate)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fRameWindow)
{
	return S_OK;
}

HRESULT CRXSkinWebBrowser::TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID)
{
	if (lpMsg->message < WM_KEYFIRST || lpMsg->message > WM_KEYLAST)
		return S_FALSE;

	if (m_pWebBrowser2 == NULL)
		return E_NOTIMPL;

	// 当前Web窗口不是焦点,不处理加速键
	BOOL bIsChild = FALSE;
	HWND hTempWnd = NULL;
	HWND hWndFocus = ::GetFocus();

	hTempWnd = hWndFocus;
	while (hTempWnd != NULL)
	{
		if (hTempWnd == m_hWndControl)
		{
			bIsChild = TRUE;
			break;
		}
		hTempWnd = ::GetParent(hTempWnd);
	}
	if (!bIsChild)
		return S_FALSE;

	IOleInPlaceActiveObject* pObj;
	if (FAILED(m_pWebBrowser2->QueryInterface(IID_IOleInPlaceActiveObject, (LPVOID*)&pObj)))
		return S_FALSE;

	HRESULT hResult = pObj->TranslateAccelerator(lpMsg);
	pObj->Release();
	return hResult;
}

HRESULT CRXSkinWebBrowser::GetOptionKeyPath(LPOLESTR* pchKey, DWORD dw)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::GetExternal(IDispatch** ppDispatch)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
{
	return E_NOTIMPL;
}

HRESULT CRXSkinWebBrowser::FilterDataObject(IDataObject* pDO, IDataObject** ppDORet)
{
	return E_NOTIMPL;
}


HRESULT CRXSkinWebBrowser::GetTypeInfoCount(UINT* pctinfo)
{
	return S_OK;
}
HRESULT CRXSkinWebBrowser::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
	return S_OK;
}

HRESULT CRXSkinWebBrowser::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
	return DISP_E_UNKNOWNNAME;
}

HRESULT CRXSkinWebBrowser::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
	if (dispIdMember == DISPID_VALUE && (wFlags & DISPATCH_PROPERTYGET) != 0 && pVarResult)
	{
		::VariantInit(pVarResult);
		pVarResult->vt = VT_BSTR;
		pVarResult->bstrVal = ::SysAllocString(L"[WebBrowser Object]");
		return S_OK;
	}
	else if (dispIdMember != DISPID_UNKNOWN)
	{
		if (OnInvoke(dispIdMember, wFlags, pDispParams, pVarResult))
			return S_OK;
	}
	if (pVarResult)
		pVarResult->vt = VT_EMPTY;

	return S_OK;
}

#define HANDLE_INVOKE_EVENT(id, f) case id: ATLTRACE(L#id); hRes = f(pDispParams); break;

bool CRXSkinWebBrowser::OnInvoke(DISPID dispIdMember, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult)
{
	HRESULT hRes = S_FALSE;

	switch (dispIdMember) {
	case DISPID_BEFORENAVIGATE2:
		OnBeforeNavigate2(pDispParams);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		OnNavigateComplete2(pDispParams);
		break;
	case DISPID_NAVIGATEERROR:
		OnNavigateError(pDispParams);
		break;
	case DISPID_DOCUMENTCOMPLETE:
		OnDocumentComplete(pDispParams);
		break;
	case DISPID_COMMANDSTATECHANGE:
		printf("DISPID_COMMANDSTATECHANGE");
		break;
	case DISPID_STATUSTEXTCHANGE:
		break;
	case DISPID_VIEWUPDATE:
		printf("DISPID_VIEWUPDATE");
	break;	
	case DISPID_ONQUIT:
		printf("DISPID_ONQUIT");
	break;	
	case DISPID_NEWWINDOW2:
		printf("DISPID_NEWWINDOW2");
		break;
	case DISPID_NEWWINDOW3:
		printf("DISPID_NEWWINDOW3");
		OnNewWindow3(pDispParams);
		break;
	default:
		printf("break");
		break;
	}

	return hRes == S_OK;;
}

HRESULT CRXSkinWebBrowser::OnBeforeNavigate2(DISPPARAMS* pDispParams) {
	if (!m_poWebBrowserEventHandler) {
		return S_FALSE;
	}
	m_pDocument = NULL;
	m_poWebBrowserEventHandler->BeforeNavigate2(this,
		pDispParams->rgvarg[6].pdispVal, pDispParams->rgvarg[5].pvarVal,
		pDispParams->rgvarg[4].pvarVal, pDispParams->rgvarg[3].pvarVal,
		pDispParams->rgvarg[2].pvarVal, pDispParams->rgvarg[1].pvarVal,
		pDispParams->rgvarg[0].pboolVal);
	return S_OK;
}

HRESULT CRXSkinWebBrowser::OnNavigateComplete2(DISPPARAMS* pDispParams) {
	
	if (!m_poWebBrowserEventHandler) {
		return E_FAIL;
	}
	m_poWebBrowserEventHandler->NavigateComplete2(this,pDispParams->rgvarg[1].pdispVal,
		pDispParams->rgvarg[0].pvarVal);
	
	return S_OK;
}

HRESULT CRXSkinWebBrowser::OnNavigateError(DISPPARAMS* pDispParams) {
	if (!m_poWebBrowserEventHandler) {
		return E_FAIL;
	}
	m_poWebBrowserEventHandler->NavigateError( this,
		pDispParams->rgvarg[4].pdispVal, pDispParams->rgvarg[3].pvarVal,
		pDispParams->rgvarg[2].pvarVal, pDispParams->rgvarg[1].pvarVal,
		pDispParams->rgvarg[0].pboolVal);
	return S_OK;
}

void CRXSkinWebBrowser::WriteText(CRXSkinString strText)
{
	HRESULT hr = S_OK;
	VARIANT* param;
	SAFEARRAY* sfArray;

	if (m_pDocument == NULL)
		return;

	// Creates a new one-dimensional array
	sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	if (sfArray == NULL)
		return;

	BSTR bstr = SysAllocString(strText);
	hr = SafeArrayAccessData(sfArray, (LPVOID*)&param);
	param->vt = VT_BSTR;
	param->bstrVal = bstr;
	hr = SafeArrayUnaccessData(sfArray);
	hr = m_pDocument->writeln(sfArray);

	SysFreeString(bstr);
	if (sfArray != NULL)
	{
		SafeArrayDestroy(sfArray);
	}
	return;
}

void CRXSkinWebBrowser::ClearInsertImage()
{
	if (PrepareInterfaces() == FALSE)
	{
		return;
	}

	IHTMLSelectionObject* pSelection = NULL;
	HRESULT hResult = S_FALSE;
	hResult = m_pDocument->get_selection(&pSelection);
	if (SUCCEEDED(hResult) && pSelection)
	{
		pSelection->empty();
		pSelection->Release();
	}
}

HRESULT CRXSkinWebBrowser::MoveCursorPosition(MoveType a_eType, int a_iCharacters)
{
	if (PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	IHTMLSelectionObject* pSelection = NULL;
	HRESULT hResult = S_FALSE;
	hResult = m_pDocument->get_selection(&pSelection);
	if (SUCCEEDED(hResult) && pSelection)
	{
		IDispatch* pTextRangeDisp = NULL;
		hResult = pSelection->createRange(&pTextRangeDisp);
		if (SUCCEEDED(hResult) && pTextRangeDisp)
		{
			IHTMLTxtRange* pRange = NULL;
			hResult = pTextRangeDisp->QueryInterface(IID_IHTMLTxtRange, (void**)&pRange);
			if (SUCCEEDED(hResult) && pRange)
			{
				int iToMove = a_iCharacters;
				if (a_eType == moveToDocStart)
				{
					iToMove = -INT_MAX;
				}
				else if (a_eType == moveToDocEnd)
				{
					iToMove = INT_MAX;
				}

				long lActual = 0;
				hResult = pRange->move((BSTR)L"character", iToMove, &lActual); //this will move selection back

				if (SUCCEEDED(hResult))
				{
					hResult = pRange->select(); //this will show the new cursor point
				}

				pRange->Release();
			}
			pTextRangeDisp->Release();
		}
		pSelection->Release();
	}

	return hResult;
}

void CRXSkinWebBrowser::ShowScrollBar()
{

}

HRESULT CRXSkinWebBrowser::OnDocumentComplete(DISPPARAMS* pDispParams) {
	if (!m_poWebBrowserEventHandler) {
		return S_OK;
	}
	m_pWebBrowser2->get_Document((IDispatch**)&m_pDocument);
	if (m_pDocument == NULL)
		return S_FALSE;
	m_poWebBrowserEventHandler->DocumentComplete(this,pDispParams->rgvarg[1].pdispVal,
		pDispParams->rgvarg[0].pvarVal);
	return S_OK;
}

HRESULT CRXSkinWebBrowser::OnCommandStateChange(DISPPARAMS* pDispParams) {
	if (!m_poWebBrowserEventHandler) {
		return S_OK;
	}
	m_poWebBrowserEventHandler->CommandStateChange(this,
		pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0].boolVal);
	return S_OK;
}

HRESULT CRXSkinWebBrowser::OnNewWindow3(DISPPARAMS* pDispParams)
{
	ATLASSERT(pDispParams->cArgs == 5);

	LPCWSTR lpszUrl = pDispParams->rgvarg[0].bstrVal;
	LPCWSTR lpszReferrer = pDispParams->rgvarg[1].bstrVal;
	// pDispParams->rgvarg[2].lVal
	VARIANT_BOOL*& Cancel = pDispParams->rgvarg[3].pboolVal;
	IDispatch**& ppDisp = pDispParams->rgvarg[4].ppdispVal;

	if (lpszUrl && *lpszUrl)
	{
		::ShellExecute(nullptr, L"open", lpszUrl, nullptr, nullptr, SW_SHOW);
	}
	*Cancel = TRUE;

	::PostMessage(m_hWndParent, WM_CLOSE, 0, 0);

	return S_OK;
}


VOID CRXSkinWebBrowser::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (IsVisible() == FALSE)
	{
		return;
	}
	if (IsBorder())
	{
		DrawRectEdge(hDC, rcItem, 1, CRXSkinService::Instance()->GetDrawMgr().GetCommonBorderColor());
	}
}

void CRXSkinWebBrowser::Bold(BOOL bState)
{
	BOOL isNow = IsBold();
	if (isNow!=bState)
	{
		ExecHelperNN(IDM_BOLD);
	}	
}

void CRXSkinWebBrowser::Italic(BOOL bState)
{
	BOOL isNow = IsItalic();
	if (isNow != bState)
	{
		ExecHelperNN(IDM_ITALIC);
	}
}

void CRXSkinWebBrowser::UnderLine(BOOL bState)
{
	BOOL isNow = IsUnderline();
	if (isNow != bState)
	{
		ExecHelperNN(IDM_UNDERLINE);
	}
}

void CRXSkinWebBrowser::Copy()
{
	ExecHelperNN(IDM_COPY);
}

void CRXSkinWebBrowser::Cut()
{
	ExecHelperNN(IDM_CUT);
}
void CRXSkinWebBrowser::Paste()
{
	ExecHelperNN(IDM_PASTE);
}
void CRXSkinWebBrowser::SelectAll()
{
	ExecHelperNN(IDM_SELECTALL);
}

void CRXSkinWebBrowser::AppendText(CRXSkinString strText)
{
	CRXSkinString strBody = GetText();
	strBody += strText;
	IHTMLElement* pElement;
	if (m_pDocument == NULL) {
		return;
	}
	if (FAILED(m_pDocument->get_body(&pElement)))
		return;
	if (pElement==NULL)
	{
		return;
	}
	pElement->put_innerHTML((TCHAR*)strBody.GetData());
	pElement->Release();
}



HRESULT CRXSkinWebBrowser::ExecHelperGetVal(UINT nCmdID, bool& bValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	bValue = false;
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vOut;
		hr = ExecCommand(nCmdID, nExecOpt, NULL, &vOut);
		if (hr == S_OK)
		{
			switch (vOut.vt)
			{
			case VT_BOOL:
				bValue = vOut.boolVal == VARIANT_TRUE ? true : false;
				break;
			case VT_NULL:
			case VT_EMPTY:
				break;
			case VT_ERROR:
				hr = V_ERROR(&vOut);
				break;
			default:
				hr = E_UNEXPECTED;
				break;
			}
		}
	}
	return hr;
}

HRESULT CRXSkinWebBrowser::ExecHelperGetVal(UINT nCmdID, short& nValue, long nMinSupportLevel, long nExecOpt)
{
	int nTempVal;
	HRESULT hr = ExecHelperGetVal(nCmdID, nTempVal, nMinSupportLevel, nExecOpt);
	if (hr == S_OK)
		nValue = (short)nTempVal;
	return hr;
}


HRESULT CRXSkinWebBrowser::ExecHelperGetVal(UINT nCmdID, int& nValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
	nValue = 0;
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vOut;
		hr = ExecCommand(nCmdID, nExecOpt, NULL, &vOut);
		if (hr == S_OK)
		{
			switch (vOut.vt)
			{
			case VT_I4:
				nValue = vOut.lVal;
				break;
			case VT_I2:
				nValue = vOut.iVal;
				break;
			case VT_EMPTY:
			case VT_NULL:
				break;
			case VT_ERROR:
				hr = V_ERROR(&vOut);
				break;
			default:
				hr = E_UNEXPECTED;
				break;
			}
		}
	}
	return hr;
}

HRESULT CRXSkinWebBrowser::ExecHelperGetVal(UINT nCmdID, CRXSkinString& strValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	CComVariant vaRet;
	strValue.Empty();
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		hr = ExecCommand(nCmdID, nExecOpt, NULL, &vaRet);
		if (hr == S_OK)
		{
			switch (vaRet.vt)
			{
			case VT_BSTR:
			{
				strValue = vaRet.bstrVal;
			}
			break;
			case VT_NULL:
			case VT_EMPTY:
				break;
			case VT_ERROR:
				hr = V_ERROR(&vaRet);
				break;
			default:
				hr = E_UNEXPECTED;
				break;
			}
		}
	}
	return hr;
}

void CRXSkinWebBrowser::FontSize(int size)
{
	ExecHelperSetVal(IDM_FONTSIZE, (short)size);
}


void CRXSkinWebBrowser::SetForeColor(COLORREF clr)
{
	ExecHelperSetVal(IDM_FORECOLOR, (int)clr, OLECMDF_SUPPORTED | OLECMDF_ENABLED);
}

short  CRXSkinWebBrowser::GetFontSize()
{
	short nSize = 0;
	HRESULT hr = ExecHelperGetVal(IDM_FONTSIZE, nSize);
	if (SUCCEEDED(hr))
	{
		return nSize;
	}
	return 0;
}



HRESULT CRXSkinWebBrowser::ExecHelperSetVal(UINT nCmdID, LPCTSTR szID, long nMinSupportLevel, long nExecOpt)
{
	long lStatus = QueryStatus(nCmdID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		if (szID)
		{
			CComVariant vName(szID);
			return ExecCommand(nCmdID, nExecOpt, &vName);
		}
		else
			return ExecCommand(nCmdID, nExecOpt);
	}
	return E_FAIL;
}

HRESULT CRXSkinWebBrowser::ExecHelperSetVal(UINT nCmdID, bool bValue, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
//	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vIn(bValue);
		hr = ExecCommand(nCmdID, nExecOpt, &vIn);
	}
	return hr;
}

HRESULT CRXSkinWebBrowser::ExecHelperSetVal(UINT nCmdID, short nNewVal, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
//	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vIn((short)nNewVal);
		hr = ExecCommand(nCmdID, nExecOpt, &vIn);
	}
	return hr;
}

HRESULT CRXSkinWebBrowser::ExecHelperSetVal(UINT nCmdID, int nNewVal, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nCmdID);
//	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
	{
		CComVariant vIn((int)nNewVal);
		hr = ExecCommand(nCmdID, nExecOpt, &vIn);
	}
	return hr;
}


void CRXSkinWebBrowser::FontName(const TCHAR* pszFontName)
{
	ExecHelperSetVal(IDM_FONTNAME, pszFontName);
}
CRXSkinString CRXSkinWebBrowser::GetFontName()
{
	CRXSkinString csValue;
	ExecHelperGetVal(IDM_FONTNAME, csValue, OLECMDF_ENABLED | OLECMDF_SUPPORTED);
	return csValue;
}


BOOL CRXSkinWebBrowser::SetDesignModel(const BOOL& bDesigned)
{
	if (PrepareInterfaces() == FALSE)
	{
		return FALSE;
	}
	HRESULT hResult = m_pDocument->put_designMode(bDesigned ? (BSTR)L"On" : (BSTR)L"Off");
	return SUCCEEDED(hResult);
}

TCHAR* CRXSkinWebBrowser::GetText() {
	IHTMLElement* pElement;
	BSTR s;
	if (m_pDocument==NULL) {
		return (TCHAR*)_T("");
	}
	if (FAILED(m_pDocument->get_body(&pElement)))
		return (TCHAR*)_T("");;
	if (pElement==NULL)
	{
		return (TCHAR*)_T("");
	}
	pElement->get_innerHTML(&s);
	pElement->Release();
	if(s)
		return (s);
	return (TCHAR*)_T("");
}

void CRXSkinWebBrowser::Empty()
{
	IHTMLElement* pElement;
	if (m_pDocument == NULL) {
		return;
	}

	if (FAILED(m_pDocument->get_body(&pElement)))
		return ;
	pElement->put_innerHTML((BSTR)L"");
	pElement->Release();
}

// Construction
CRXSkinWebBrowser::CStreamOnCString::CStreamOnCString()
{
	m_current_index = 0;
}

CRXSkinWebBrowser::CStreamOnCString::CStreamOnCString(LPCTSTR szData) : m_strStream(szData)
{
	m_current_index = 0;
}

// Copies the data currently held in this
// object into a CString
BOOL CRXSkinWebBrowser::CStreamOnCString::CopyData(CRXSkinString& target)
{
	target = m_strStream;
	return TRUE;
}

// Sets the value of 
BOOL CRXSkinWebBrowser::CStreamOnCString::SetData(LPCTSTR szData)
{
	try //IZ replaced: _ATLTRY
	{
		m_strStream = szData;
	}
	catch (...) //IZ replaced: _ATLCATCHALL()
	{
		return FALSE;
	}
	return TRUE;
}

// Implementation
HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::QueryInterface(REFIID iid, void** ppUnk)
{
	if (::InlineIsEqualGUID(iid, IID_IUnknown) ||
		::InlineIsEqualGUID(iid, IID_IStream) ||
		::InlineIsEqualGUID(iid, IID_ISequentialStream))
	{
		*ppUnk = (void*)this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CRXSkinWebBrowser::CStreamOnCString::AddRef(void)
{
	return (ULONG)1;
}

ULONG STDMETHODCALLTYPE CRXSkinWebBrowser::CStreamOnCString::Release(void)
{
	return (ULONG)1;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
	if (pcbRead)
		*pcbRead = 0;
	if (m_strAnsi.GetLength() == 0)
		m_strAnsi = m_strStream;

	if (!pv)
		return E_POINTER;

	unsigned int length = m_strAnsi.GetLength();
	TCHAR* pStream = (TCHAR *)m_strAnsi.GetData(); //IZ added 0 parameter to GetBuffer call
	if (!pStream)
		return E_UNEXPECTED;

	TCHAR* pStart = pStream + m_current_index;
	TCHAR* pEnd = pStream + length;
	if (pStart >= pEnd)
		return S_FALSE; // no more data to read

	int bytes_left = (int)(pEnd - pStart);
	int bytes_to_copy = (int)min(bytes_left, (int)cb);
	if (bytes_to_copy <= 0)
	{
		// reset members so this stream can be used again
		m_current_index = 0;
		m_strAnsi.Empty();
		return S_FALSE;
	}

	memcpy(pv, pStream + m_current_index, bytes_to_copy);
	if (pcbRead)
		*pcbRead = (ULONG)bytes_to_copy;
	m_current_index += bytes_to_copy;
	//m_strAnsi.ReleaseBuffer(0);
	return S_OK;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::Write(const void* pv, ULONG cb, ULONG* pcbWritten)
{
	if (pcbWritten)
		*pcbWritten = 0;
#ifdef _UNICODE
	int flags = IS_TEXT_UNICODE_UNICODE_MASK;
	if (0 != IsTextUnicode((LPVOID)pv, cb, &flags))
	{
		if (flags & IS_TEXT_UNICODE_ASCII16 ||
			flags & IS_TEXT_UNICODE_STATISTICS)
		{
			// compiling UNICODE and got a UNICODE buffer
			UINT nCharsToSkip = flags & IS_TEXT_UNICODE_SIGNATURE ? 1 : 0;
			//IZTOCHECK m_strStream.Append((wchar_t*)( ((wchar_t*)pv)+nCharsToSkip), (cb/sizeof(wchar_t))-nCharsToSkip);
			AppendToCString(m_strStream, (wchar_t*)(((wchar_t*)pv) + nCharsToSkip), (cb / sizeof(wchar_t)) - nCharsToSkip);
			if (pcbWritten)
				*pcbWritten = cb;
		}
	}
	else
	{
		// compiling UNICODE and got an ansi buffer
		// convert ansi buffer to UNICODE buffer
		unsigned int buffer_size = cb;
		wchar_t* pBuffer = new wchar_t[buffer_size];
		if (pBuffer)
		{
			if (buffer_size >= (unsigned int)MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pv, cb, pBuffer, buffer_size))
			{
				//IZTOCHECK m_strStream.Append(pBuffer, buffer_size);
				AppendToCString(m_strStream, pBuffer, buffer_size);
			}
		}
		delete[] pBuffer;
	}
#else
	int flags = IS_TEXT_UNICODE_UNICODE_MASK;
	if (0 != IsTextUnicode((LPVOID)pv, cb, &flags))
	{
		if (flags & IS_TEXT_UNICODE_ASCII16 ||
			flags & IS_TEXT_UNICODE_STATISTICS)
		{
			// compiling ANSI and got a UNICODE buffer
			UINT nCharsToSkip = flags & IS_TEXT_UNICODE_SIGNATURE ? 1 : 0;
			unsigned int nChars = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pv, cb,
				NULL, 0, NULL, NULL);

			char* pBuff = new char[nChars];
			if (pBuff)
			{
				WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pv, cb, pBuff, nChars, NULL, NULL);
				//IZ replaced: m_strStream.Append(pBuff+nCharsToSkip, nChars-nCharsToSkip);
				AppendToCString(m_strStream, pBuff + nCharsToSkip, nChars - nCharsToSkip);

				if (pcbWritten)
					*pcbWritten = cb;
			}
			delete[] pBuff;
		}
	}
	else
	{
		// compiling ANSI and got an ANSI buffer
		//IZ replaced: m_strStream.Append((char*)pv, cb);
		AppendToCString(m_strStream, (TCHAR*)pv, cb);
		if (pcbWritten)
			*pcbWritten = cb;
	}
#endif
	return S_OK;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::Seek(LARGE_INTEGER, DWORD, ULARGE_INTEGER*)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::SetSize(ULARGE_INTEGER)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::CopyTo(IStream*, ULARGE_INTEGER, ULARGE_INTEGER*,
	ULARGE_INTEGER*)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::Commit(DWORD)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::Revert(void)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::Stat(STATSTG*, DWORD)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CRXSkinWebBrowser::CStreamOnCString::Clone(IStream**)
{
	return E_NOTIMPL;
}

LPDISPATCH CRXSkinWebBrowser::GetHtmlDocument() const
{
	LPDISPATCH result;
	HRESULT hr = m_pWebBrowser2->get_Document(&result);
	if (FAILED(hr))
	{
		return NULL;
	}
	return result;
}



//IZ: added to simulate Append method of CString from MFC70
void CRXSkinWebBrowser::CStreamOnCString::AppendToCString(CRXSkinString& a_sString, LPCTSTR a_pstrToAdd, UINT a_uiLength)
{
	TCHAR* pTmp = new TCHAR[a_uiLength + 1];
	if (pTmp)
	{
		RtlZeroMemory(pTmp, (a_uiLength * sizeof(TCHAR)) + sizeof(TCHAR));
		_tcsncpy(pTmp, a_pstrToAdd, a_uiLength);
		a_sString += pTmp;
		delete[] pTmp;
	}
}

HRESULT CRXSkinWebBrowser::ExecHelperNN(UINT nID, long nMinSupportLevel, long nExecOpt)
{
	HRESULT hr = E_FAIL;
	long lStatus = QueryStatus(nID);
	if ((lStatus & nMinSupportLevel) == nMinSupportLevel)
		hr = ExecCommand(nID, nExecOpt);
	return hr;
}

HRESULT CRXSkinWebBrowser::ExecCommand(const GUID* pGuid, long cmdID, long cmdExecOpt, VARIANT* pInVar, VARIANT* pOutVar)
{
	if (PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	return m_pCmdTarg->Exec(pGuid, cmdID, cmdExecOpt, pInVar, pOutVar);
}

HRESULT CRXSkinWebBrowser::ExecCommand(long cmdID, long cmdExecOpt, VARIANT* pInVar, VARIANT* pOutVar)
{
	return ExecCommand(&CGID_MSHTML, cmdID, cmdExecOpt, pInVar, pOutVar);
}
BOOL CRXSkinWebBrowser::PrepareInterfaces()
{
	HRESULT	hResult = E_FAIL;

	if (m_pDispatch == NULL)
	{
		m_pDispatch = GetHtmlDocument();

		if (m_pDispatch)
		{
			m_pDocument = NULL;
			m_pCmdTarg = NULL;
			hResult = m_pDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&m_pDocument);
			hResult = m_pDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&m_pCmdTarg);
		}
	}

	return m_pDispatch && m_pDocument && m_pCmdTarg;
}

long CRXSkinWebBrowser::QueryStatus(long cmdID)
{
	if (PrepareInterfaces() == FALSE)
	{
		return E_FAIL;
	}

	OLECMD ocmd = { (ULONG)cmdID, 0 };
	if (S_OK == m_pCmdTarg->QueryStatus(&CGID_MSHTML, 1, &ocmd, NULL))
		return ocmd.cmdf;

	return 0;
}

BOOL CRXSkinWebBrowser::CanPaste() //IZ TO DO
{
	if (PrepareInterfaces() == FALSE)
	{
		return FALSE;
	}

	VARIANT_BOOL vOut = 0;
	m_pDocument->queryCommandEnabled((BSTR)L"Paste", &vOut);
	return (vOut == VARIANT_TRUE);
}


BOOL CRXSkinWebBrowser::IsBold()
{
	return QueryDocumentState((BSTR)L"Bold");
}

BOOL CRXSkinWebBrowser::IsUnderline()
{
	return QueryDocumentState((BSTR)L"Underline");
}

BOOL CRXSkinWebBrowser::IsItalic()
{
	return QueryDocumentState((BSTR)L"Italic");
}

BOOL CRXSkinWebBrowser::IsStrikeOut()
{
	return QueryDocumentState((BSTR)L"StrikeThrough");
}


BOOL CRXSkinWebBrowser::QueryDocumentState(BSTR a_bstrItemToQuery)
{
	if (PrepareInterfaces() == FALSE)
	{
		return FALSE;
	}

	VARIANT_BOOL vOut = 0;
	m_pDocument->queryCommandState(a_bstrItemToQuery, &vOut);
	return (vOut == VARIANT_TRUE);
}

bool CRXSkinWebBrowser::PutScrollTop()
{
	HRESULT hr;
	IHTMLElementCollection* pCollection;
	hr = m_pDocument->get_all(&pCollection);
	if (FAILED(hr))
	{
		return false;
	}
	long len;
	hr = pCollection->get_length(&len);
	if (FAILED(hr))
	{
		return false;
	}

	// 遍历所有的元素
	for (long l = 0; l < len; l++)
	{
		VARIANT varIndex, var2;
		VariantInit(&varIndex);
		VariantInit(&var2);
		varIndex.vt = VT_I4;
		varIndex.lVal = l;
		IDispatch* spDisp = NULL;
		pCollection->item(varIndex, var2, &spDisp);
		IHTMLElement* pElem;
		spDisp->QueryInterface(IID_IHTMLElement, (LPVOID*)&pElem);
		CComBSTR tagName;
		pElem->get_tagName(&tagName);

		CRXSkinString str = tagName.m_str;
		str.MakeUpper();
		//if ( str.Compare( _T( "IFRAME" ) ) == 0 || str.Compare( _T( "FRAME" ) ) == 0 )     

		{
			IHTMLElement2* pElement2 = NULL;
			hr = pElem->QueryInterface(IID_IHTMLElement2, (void**)&pElement2);
			if (FAILED(hr))
			{
				return false;
			}
			long lScrollHeight;
			hr = pElement2->get_scrollHeight(&lScrollHeight);
			if (FAILED(hr))
			{
				return false;
			}

			hr = pElement2->put_scrollTop(lScrollHeight);
			if (FAILED(hr))
			{
				return false;
			}
			pElement2->Release();
		}

		pElem->Release();
		spDisp->Release();
	}
	pCollection->Release();
	return true;
}

void CRXSkinWebBrowser::InsertImage(const TCHAR* pszID, const TCHAR* pszImageUrl, const TCHAR* pszType)
{
	ExecHelperSetVal(IDM_IMAGE, pszImageUrl, OLECMDF_SUPPORTED,
		OLECMDEXECOPT_DONTPROMPTUSER);
	ClearInsertImage();	
	SetImageInfo(pszID, pszImageUrl, pszType);
	MoveCursorPosition(moveFromCurrentPos, 0);
}

void CRXSkinWebBrowser::SetImageInfo(const TCHAR* pszID, const TCHAR* pszImageUrl, const TCHAR* pszType)
{
	if (PrepareInterfaces() == FALSE)
	{
		return;
	}

	IHTMLElementCollection* pImages = NULL;
	HRESULT hResult = m_pDocument->get_images(&pImages);

	if (SUCCEEDED(hResult) && pImages)
	{
		long lNumberOfItems = 0;
		hResult = pImages->get_length(&lNumberOfItems);

		if (SUCCEEDED(hResult))
		{
			IDispatch* ppvDisp = NULL;
			IHTMLImgElement* ppvElement = NULL;

			for (long ix = 0; ix < lNumberOfItems; ix++)
			{
				CComVariant index = ix;
				hResult = pImages->item(index, index, &ppvDisp);

				if (SUCCEEDED(hResult) && ppvDisp)
				{
					hResult = ppvDisp->QueryInterface(IID_IHTMLImgElement, (void**)&ppvElement);
					ppvDisp->Release();

					if (SUCCEEDED(hResult) && ppvElement)
					{
						BSTR bstrText = NULL;
						CRXSkinString sSrc;
						hResult = ppvElement->get_src(&bstrText);
						BSTR2CString(bstrText, sSrc);
						SysFreeString(bstrText);
						sSrc.Replace(_T("/"),_T("\\"));
						sSrc = sSrc.Right(sSrc.GetLength() - 8);
						CRXSkinString strImageUrl = pszImageUrl;
						strImageUrl.Replace(_T("/"), _T("\\"));
						if (_tcscmp(strImageUrl, sSrc)==0)
						{
							IHTMLElement* poElement =NULL;
							hResult = ppvElement->QueryInterface(IID_IHTMLElement, (void**)&poElement);
							if (SUCCEEDED(hResult) && poElement)
							{
								CComVariant vInID((TCHAR*)pszID);
								poElement->setAttribute((BSTR)_T("strid"), vInID);
								CComVariant vInType((TCHAR*)pszType);
								poElement->setAttribute((BSTR)_T("type"), vInType);

								poElement->Release();
							}
						}
						ppvElement->Release();
					}
				}
			}
		}

		pImages->Release();
	}
}

LRESULT CRXSkinWebBrowser::OnHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CRXSkinWebBrowser::OnFinalMessage(HWND hWnd)
{

}

CRXSkinString  CRXSkinWebBrowser::GetAttibute(CRXSkinString strName, IHTMLElement* poElement)
{
	CComVariant v_AttrValue;
	CComBSTR   bs_AttrName = strName.GetData();
	poElement->getAttribute(bs_AttrName, 0, &v_AttrValue);
	return VarToStr(v_AttrValue);
}

CRXSkinString CRXSkinWebBrowser::VarToStr(CComVariant& v_Variant)
{
	CRXSkinString s_Result;

	if (v_Variant.vt == VT_I4)
	{
		s_Result.Format(_T("%d"), v_Variant.lVal);
	}
	else if (v_Variant.vt == VT_BOOL)
	{
		if (v_Variant.boolVal) s_Result = _T("true");
		else                   s_Result = _T("false");
	}
	else if (v_Variant.vt == VT_BSTR)
	{
		if (!v_Variant.bstrVal)
			return _T("");

		UINT u32_Len = SysStringLen(v_Variant.bstrVal);
		if (!u32_Len)
			return _T("");

		LPTSTR t_Buf = (LPTSTR)s_Result.GetData();

#ifdef _UNICODE // Unicode --> Unicode
		wcscpy(t_Buf, v_Variant.bstrVal);
#else // _MBCS     Unicode --> Ansi
		WideCharToMultiByte(CP_ACP, 0, v_Variant.bstrVal, u32_Len, t_Buf, u32_Len, 0, 0);
#endif


	}

	return s_Result;
}

void CRXSkinWebBrowser::ModifyHistoryMsgForImage(const CRXSkinString& strID, const CRXSkinString& strUrl)
{
	if (PrepareInterfaces() == FALSE)
	{
		return;
	}
	IHTMLElementCollection* pImages = NULL;
	HRESULT hResult = S_OK;
	hResult = m_pDocument->get_images(&pImages);

	if (SUCCEEDED(hResult) && pImages)
	{
		long lNumberOfItems = 0;
		hResult = pImages->get_length(&lNumberOfItems);

		if (SUCCEEDED(hResult))
		{
			IDispatch* ppvDisp = NULL;
			IHTMLImgElement* ppvElement = NULL;

			for (long ix = 0; ix < lNumberOfItems; ix++)
			{
				CComVariant index = ix;
				hResult = pImages->item(index, index, &ppvDisp);

				if (SUCCEEDED(hResult) && ppvDisp)
				{
					hResult = ppvDisp->QueryInterface(IID_IHTMLImgElement, (void**)&ppvElement);
					ppvDisp->Release();

					if (SUCCEEDED(hResult) && ppvElement)
					{
						IHTMLElement* poElement = NULL;
						hResult = ppvElement->QueryInterface(IID_IHTMLElement, (void**)&poElement);
						if (SUCCEEDED(hResult) && poElement)
						{
							CRXSkinString strAtrrID = GetAttibute(_T("strid"), poElement);
							if (_tcscmp(strAtrrID, strID) == 0)
							{
								TCHAR szUrl[MAX_PATH] = { 0 };
								_tcscpy(szUrl, strUrl);
								ppvElement->put_src(szUrl);
							}
							poElement->Release();
						}
						ppvElement->Release();
					}
				}
			}
		}
	}
}

LRESULT CALLBACK CRXSkinWebBrowser::_WndBrowserProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void CRXSkinWebBrowser::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
	GetBrowserWindow();
	if (m_hWndControl)
	{
	//	m_pWebBrowser2->put_Visible(bIsVisible);
		if (bIsVisible)
		{
			::ShowWindow(m_hWndControl, SW_SHOW);
		}
		else
			::ShowWindow(m_hWndControl, SW_HIDE);
	}
}