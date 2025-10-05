#pragma once
#include "rxskindialog.h"
#include "common/rxskinutil.h"
class CRXSkinWaiiting : public CRXSkinDialog
{
public:
	CRXSkinWaiiting(void);
	~CRXSkinWaiiting(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnClickedClose();
	virtual VOID Destroy();
private:
	CRXSkinString       m_strContent;
	INT32               m_nOffsetX;
	INT32               m_nOffsetTop;
	INT32               m_nOffsetBottom;
	UINT32              m_dwContentWidth;
};
