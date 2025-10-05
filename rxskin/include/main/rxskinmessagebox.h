#pragma once
#include "rxskindialog.h"
#include "common/rxskinutil.h"
class CRXSkinMessageBox : public CRXSkinDialog
{
public:
	CRXSkinMessageBox(void);
	~CRXSkinMessageBox(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnClickedClose();
	virtual VOID Destroy();
	VOID    SetTipType(EMessageBoxTipType enTipType);
	VOID    SetType(EMessageBoxType enType);
	VOID    SetContent(const TCHAR* pszContent);
private:
	EMessageBoxTipType	m_enTipType;
	EMessageBoxType		m_enType;
	CRXSkinString       m_strContent;
	INT32               m_nOffsetX;
	INT32               m_nOffsetTop;
	INT32               m_nOffsetBottom;
	UINT32              m_dwContentWidth;
};
