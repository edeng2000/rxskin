#include "pch.h"
#include "controls\rxskinrichtexthost.h"
#include "common\RXSkinService.h"
#include "common\graphics.h"
#include "common\RXSkinMemDC.h"
#include "common\rxskinutil.h"
#include "rxlogger.h"
#include "rxtime.h"
#include <wingdi.h>
class RXSKIN_API STextServiceHelper {
public:
	static STextServiceHelper* instance() {
		static STextServiceHelper _this;
		return &_this;
	}
public:
	/**
	 * STextServiceHelper::CreateTextServices
	 * @brief
	 * @param  IUnknown *punkOuter
	 * @param  ITextHost *pITextHost
	 * @param  IUnknown **ppUnk
	 * @return 返回HRESULT
	 *
	 * Describe
	 */
	HRESULT CreateTextServices(IUnknown* punkOuter, ITextHost* pITextHost, IUnknown** ppUnk);

protected:
	/**
	 * STextServiceHelper::STextServiceHelper
	 * @brief    构造函数
	 *
	 * Describe  构造函数
	 */
	STextServiceHelper();
	/**
	 * STextServiceHelper::~STextServiceHelper
	 * @brief    析构函数
	 *
	 * Describe  析构函数
	 */
	~STextServiceHelper();

	HINSTANCE m_rich20;                          /**< richedit module */
	PCreateTextServices m_funCreateTextServices; /**< 回调函数 */
};
STextServiceHelper::STextServiceHelper()
{
#ifdef _WIN32
	m_rich20 = LoadLibrary(_T("Msftedit.dll"));
#else
	m_rich20 = LoadLibrary(_T("libmsftedit.so"));
#endif
	if (m_rich20)
		m_funCreateTextServices = (PCreateTextServices)GetProcAddress(m_rich20, "CreateTextServices");
	else {
#ifndef _WIN32
		const char* err = dlerror();
		printf("load so failed, err=%s\n", err);
#endif
	}
}
STextServiceHelper::~STextServiceHelper()
{
	if (m_rich20)
		FreeLibrary(m_rich20);
	m_funCreateTextServices = NULL;
}

HRESULT STextServiceHelper::CreateTextServices(IUnknown* punkOuter, ITextHost* pITextHost, IUnknown** ppUnk)
{
	if (!m_funCreateTextServices)
		return E_NOTIMPL;
	return m_funCreateTextServices(punkOuter, pITextHost, ppUnk);
}


CRXSkinTextHost::CRXSkinTextHost(CRXSkinRichEditBody* poOwner)
{
	m_poOwner = poOwner;
}

BOOL CRXSkinTextHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight) 
{
	return CreateCaret(m_poOwner->GetHWnd(), hbmp, xWidth, yHeight);
}

BOOL CRXSkinTextHost::TxShowCaret(BOOL fShow)
{
	if (fShow) {
		return ShowCaret(m_poOwner->GetHWnd());
	}
	else {
		return HideCaret(m_poOwner->GetHWnd());
	}
}

ULONG CRXSkinTextHost::Release()
{
	auto result = InterlockedDecrement(&reference_count_);
	if (result == 0) {
		delete this;
	}
	return result;
}

ULONG CRXSkinTextHost::AddRef()
{
	return InterlockedIncrement(&reference_count_);
}

HRESULT CRXSkinTextHost::QueryInterface(REFIID riid, void** ppvObject)
{
	if (!ppvObject) {
		return E_INVALIDARG;
	}

	if ((riid == IID_IUnknown) || (riid == IID_ITextHost)) {
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	*ppvObject = nullptr;
	return E_NOINTERFACE;
}
HDC CRXSkinTextHost::TxGetDC() 
{
	return GetDC(m_poOwner->GetHWnd());
}

INT CRXSkinTextHost::TxReleaseDC(HDC hdc)  
{
	return ReleaseDC(m_poOwner->GetHWnd(), hdc);
}

BOOL CRXSkinTextHost::TxShowScrollBar(INT fnBar, BOOL fShow) 
{
	return TRUE;
}

BOOL CRXSkinTextHost::TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags)  
{
	int wBar = 0;
	switch (fuSBFlags)
	{
	case SB_BOTH:
		wBar = SB_BOTH;
		break;
	case SB_VERT:
		wBar = SB_VERT;
		break;
	case SB_HORZ:
		wBar = SB_HORZ;
		break;
	}
	return m_poOwner->EnableScrollbar(wBar);
}

BOOL CRXSkinTextHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw) 
{
	return FALSE;
}

BOOL CRXSkinTextHost::TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw)  
{
	return FALSE;
}

void CRXSkinTextHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)  
{
	InvalidateRect(m_poOwner->GetHWnd(), prc, fMode);
}

void CRXSkinTextHost::TxViewChange(BOOL fUpdate)  
{

}


BOOL CRXSkinTextHost::TxSetTimer(UINT idTimer, UINT uTimeout) 
{
	SetTimer(m_poOwner->GetHWnd(), idTimer, uTimeout, nullptr);
	return TRUE;
}

void CRXSkinTextHost::TxKillTimer(UINT idTimer)  {
	KillTimer(m_poOwner->GetHWnd(), idTimer);
}

void CRXSkinTextHost::TxScrollWindowEx(INT dx,
	INT dy, 
	LPCRECT lprcScroll, 
	LPCRECT lprcClip,
	HRGN hrgnUpdate, 
	LPRECT lprcUpdate, 
	UINT fuScroll) {
	m_poOwner->Invalidate();
}

void CRXSkinTextHost::TxSetCapture(BOOL fCapture) 
{
	if (fCapture) {
		SetCapture(m_poOwner->GetHWnd());
	}
	else {
		ReleaseCapture();
	}
}

void CRXSkinTextHost::TxSetFocus() 
{

}

void CRXSkinTextHost::TxSetCursor(HCURSOR hcur, BOOL fText) 
{
	SetCursor(hcur);
}

BOOL CRXSkinTextHost::TxScreenToClient(LPPOINT lppt) 
{
	ScreenToClient(m_poOwner->GetHWnd(), lppt);
	return TRUE;
}

BOOL CRXSkinTextHost::TxClientToScreen(LPPOINT lppt) 
{
	ClientToScreen(m_poOwner->GetHWnd(), lppt);
	return TRUE;
}

HRESULT CRXSkinTextHost::TxActivate(LONG* plOldState) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxDeactivate(LONG lNewState) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetClientRect(LPRECT prc) 
{
	*prc = m_poOwner->GetEditRect();
	return S_OK;
}

HRESULT CRXSkinTextHost::TxGetViewInset(LPRECT prc) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetCharFormat(const CHARFORMATW** ppCF) 
{

	if (char_format_ == nullptr) {
		char_format_ = std::make_unique<CHARFORMATW>();
		char_format_->cbSize = sizeof(CHARFORMATW);
	}

	*ppCF = char_format_.get();
	return S_OK;
}

HRESULT CRXSkinTextHost::TxGetParaFormat(const PARAFORMAT** ppPF) 
{

	if (para_format_ == nullptr) {
		para_format_ = std::make_unique<PARAFORMAT>();
		para_format_->cbSize = sizeof PARAFORMAT;
	}

	*ppPF = para_format_.get();
	return S_OK;
}

COLORREF CRXSkinTextHost::TxGetSysColor(int nIndex) 
{
	return GetSysColor(nIndex);
}

HRESULT CRXSkinTextHost::TxGetBackStyle(TXTBACKSTYLE* pstyle) 
{
	*pstyle = TXTBACK_TRANSPARENT;
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetMaxLength(DWORD* plength) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetScrollBars(DWORD* pdwScrollBar) 
{
	//*pdwScrollBar =(WS_VSCROLL |  ES_AUTOVSCROLL | ES_DISABLENOSCROLL);

	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetPasswordChar(_Out_ TCHAR* pch) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetAcceleratorPos(LONG* pcp) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetExtent(LPSIZEL lpExtent) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::OnTxCharFormatChange(const CHARFORMATW* pCF) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::OnTxParaFormatChange(const PARAFORMAT* pPF) 
{
	return E_NOTIMPL;
}

HRESULT CRXSkinTextHost::TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits) 
{
	DWORD dwProperties = TXTBIT_RICHTEXT | TXTBIT_MULTILINE | TXTBIT_SELBARCHANGE;
	*pdwBits = dwProperties & dwMask;
	return S_OK;
}

HRESULT CRXSkinTextHost::TxNotify(DWORD iNotify, void* pv) 
{
	return E_NOTIMPL;
}

HIMC CRXSkinTextHost::TxImmGetContext() 
{
	return nullptr;
}

void CRXSkinTextHost::TxImmReleaseContext(HIMC himc) 
{

}

HRESULT CRXSkinTextHost::TxGetSelectionBarWidth(LONG* lSelBarWidth) 
{
	*lSelBarWidth = 30;
	return S_OK;
}

BOOL CRXSkinTextHost::TxSetCaretPos(INT x, INT y)  
{
	return SetCaretPos(x, y);
}

void CRXSkinTextHost::Create()
{
	IUnknown* pUnk;
	HRESULT hr;

	// Create Text Services component
	if (FAILED(STextServiceHelper::instance()->CreateTextServices(NULL, this, &pUnk)))
		return ;

	hr = pUnk->QueryInterface(IID_ITextServices, (void**)&pserv);

	pUnk->Release();
}