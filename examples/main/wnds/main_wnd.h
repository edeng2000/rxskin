#ifndef ___RX_RXINSTALL_WND_INCLUDE____
#define ___RX_RXINSTALL_WND_INCLUDE____
#include "rxskinincludes.h"

#include "rxutil.h"
class CMainWnd : public CRXSkinDialog
{
public:
	CMainWnd();
	~CMainWnd();
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
private:
	CRXSkinBrowser* m_poBtnBrowser;
	CRXCheckButton* m_poBtnAgreeProtocol;
	CRXImageButton* m_poBtnInstall;
	CRXSkinStatic* m_poStaticDetail;
	CRXSkinStatic* m_poStaticPath;
	CRXSkinProgress* m_poProgress;
	CRXHyperLink* m_poBtnViewProtocol;
	bool m_bFinished;
};
#endif
