#include "pch.h"
#include "main\rxskinwaiting.h"

#include "common\rxskinservice.h"
#include "common\rxskinmemdc.h"
#include "fundation\rxstring.h"
#include "common\rxskindefines.h"

#include "fundation\rxstring.h"



CRXSkinWaiiting::CRXSkinWaiiting(void)
{
	m_nOffsetTop = m_nOffsetBottom = m_nOffsetX = 0;
	m_dwContentWidth = 120;
}

CRXSkinWaiiting::~CRXSkinWaiiting(void)
{
}


VOID CRXSkinWaiiting::Destroy()
{
	CRXSkinDialog::Destroy();
	CRXSkinService::Instance()->GetPreMessages().Empty();
}

BOOL CRXSkinWaiiting::OnCreate(LPCREATESTRUCT lpCreate)
{
	CRXSkinDialog::OnCreate(lpCreate);
	::SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE)
		& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
	::SetWindowPos(GetHWnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	CRXSkinString strPath = GetModulePath();
	strPath += _T("../configs/waiting.xml");
	ReadConfig(strPath.GetData());
	return TRUE;
}

VOID CRXSkinWaiiting::OnClickedClose()
{
	CRXSkinDialog::OnClickedClose();
}

VOID CRXSkinWaiiting::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinDialog::OnPaintBk(hDC);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, CRXSkinService::Instance()->GetDrawMgr().GetNormalFont());
	RECT rcTxt;
	rcTxt.left = 0;

	::SelectObject(hDC, hOldFont);
}

BOOL CRXSkinWaiiting::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinDialog::OnCommand(wParam, lParam);
	if (FALSE == bResult)
	{
		switch (wParam)
		{
		case IDOK:
		{
			m_nModelRet = IDOK;
			Destroy();
			return TRUE;
		}
		break;
		case IDCANCEL:
		{
			m_nModelRet = IDCANCEL;
			Destroy();
			return TRUE;
		}
		break;
		case IDABORT:
		{
			m_nModelRet = IDABORT;
			Destroy();
			return TRUE;
		}
		break;
		case IDRETRY:
		{
			m_nModelRet = IDRETRY;
			Destroy();
			return TRUE;
		}
		break;
		case IDIGNORE:
		{
			m_nModelRet = IDIGNORE;
			Destroy();
			return TRUE;
		}
		break;
		case IDNO:
		{
			m_nModelRet = IDNO;
			Destroy();
			return TRUE;
		}
		break;
		case IDYES:
		{
			m_nModelRet = IDYES;
			Destroy();
			return TRUE;
		}
		break;
		default:
			break;
		}
	}
	return bResult;
}