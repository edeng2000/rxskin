#ifndef ___RX_RXINSTALL_PROTOCOL_WND_INCLUDE____
#define ___RX_RXINSTALL_PROTOCOL_WND_INCLUDE____

#include "rxskinincludes.h"
#include "rxutil.h"
class CProtocolWnd : public CRXSkinDialog,public CWebBrowserEventHandler
{
public:
	CProtocolWnd();
	~CProtocolWnd();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual VOID  OnClickedClose();
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void  OnFinalMessage(HWND hWnd);

	virtual void BeforeNavigate2(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url, VARIANT*& Flags, VARIANT*& TargetFrameName, VARIANT*& PostData, VARIANT*& Headers, VARIANT_BOOL*& Cancel) {}
	virtual void NavigateError(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url, VARIANT*& TargetFrameName, VARIANT*& StatusCode, VARIANT_BOOL*& Cancel) {}
	virtual void NavigateComplete2(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url) {}
	virtual void ProgressChange(CRXSkinWebBrowser* pWeb, LONG nProgress, LONG nProgressMax) {}
	virtual void NewWindow3(CRXSkinWebBrowser* pWeb, IDispatch** pDisp, VARIANT_BOOL*& Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl) {}
	virtual void CommandStateChange(CRXSkinWebBrowser* pWeb, long Command, VARIANT_BOOL Enable) {};
	virtual void TitleChange(CRXSkinWebBrowser* pWeb, BSTR bstrTitle) {};
	virtual void DocumentComplete(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url);
private:
	CRXSkinWebBrowser* m_poWebBrowser;
	CRXSkinRichEdit* m_poEdtProtocol;
};
#endif
