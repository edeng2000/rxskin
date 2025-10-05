#ifndef _RXSKIN_COLOR_PICKER_H_____
#define _RXSKIN_COLOR_PICKER_H_____
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include "rxskinscrollbar.h"
#include "fundation/rxstring.h"
#include <map>
#include <vector>
using namespace std;

typedef struct {
	COLORREF crColour;
	TCHAR* szName;
} ColourTableEntry;
class CRXSkinColorPicker;
class RXSKIN_API CRXSkinColorCell :public CRXSkinItem
{
public:
	CRXSkinColorCell();
	~CRXSkinColorCell();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	void SetColor(const COLORREF& clr) { m_crlSelect = clr; }
	COLORREF GetColor() { return m_crlSelect; }
	void SetOwner(CRXSkinColorPicker* poOwner) { m_poOwner = poOwner; }
private:
	COLORREF  m_crlSelect;
	CRXSkinColorPicker* m_poOwner;
};



class RXSKIN_API CRXSkinColorPicker : public CRXSkinWnd
{
public:
	CRXSkinColorPicker(void);
	~CRXSkinColorPicker(void);
	void Show(CRXControl* poControl);
	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual VOID  OnPaint();
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual BOOL  OnMouseHover(UINT nFlags, POINT point);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual VOID  OnActive(WPARAM nState, HWND hWnd);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDblClk(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonDblClk(UINT nFlags, POINT point);
	virtual BOOL  OnMButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnMButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnMButtonDblClk(UINT nFlags, POINT point);
	virtual void  OnCancelMode();
	virtual void  OnFinalMessage(HWND hWnd);
private:
	static ColourTableEntry m_Colours[];
	vector<CRXSkinColorCell*> m_vecCells;
	UINT32  m_dwParentID;
	CRXSkinColorCell*  m_poMouseOnCell;
private:
	CRXSkinColorCell* _HitTest(POINT pt);
};



#endif
