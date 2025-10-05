#ifndef ___RX_NEW_FOLDER_WND_INCLUDE____
#define ___RX_NEW_FOLDER_WND_INCLUDE____


#include "rxutil.h"
class CRXSkinNewFolder : public CRXSkinDialog
{
public:
	CRXSkinNewFolder();
	~CRXSkinNewFolder();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonUp(UINT nFlags, POINT point);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual VOID  OnClickedClose();
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void  OnFinalMessage(HWND hWnd);
	void  SetParentPath(const TCHAR* pszPath) { m_strPath = pszPath; };
private:
	CRXSkinEdit* m_edtNewFolderName;
	tstring m_strPath;
private:
	VOID         _OnClickedOK();
	VOID         _OnClickedCancel();
};
#endif
