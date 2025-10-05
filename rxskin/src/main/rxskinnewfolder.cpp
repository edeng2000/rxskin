#include "pch.h"
#include "main/rxskinnewfolder.h"
#include "rxutil.h"
#include "common/graphics.h"
#include <olectl.h>
#include <WindowsX.h>
#include "commctrl.h"
#include "rxfile.h"
#include "rxdir.h"
#define DEFALT_LINE_OFFSET_X       10
#define DEFALT_LINE_OFFSET_Y       10

CRXSkinNewFolder::CRXSkinNewFolder()
{
}

CRXSkinNewFolder ::~CRXSkinNewFolder()
{
}

BOOL CRXSkinNewFolder::OnLButtonUp(UINT nFlags, POINT point)
{
	BOOL bResult = CRXSkinDialog::OnLButtonUp(nFlags, point);
	return bResult;
}

BOOL CRXSkinNewFolder::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = CRXSkinDialog::OnLButtonDown(nFlags, point);
	if (FALSE == bClicked)
	{
		CRXSkinRect rcCaption = GetCaptionRect();
		if (rcCaption.PtInRect(point))
		{
			::PostMessage(GetHWnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			return TRUE;
		}
	}
	return bClicked;
}

BOOL CRXSkinNewFolder::OnCreate(LPCREATESTRUCT lpCreate)
{
	BOOL bResult = CRXSkinDialog::OnCreate(lpCreate);
	if (bResult)
	{
		::SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE)
			& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
		::SetWindowPos(GetHWnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

		tstring strPath = GetModulePath();
		strPath += _T("../configs/new_folder.xml");
		ReadConfig(strPath.c_str());
		CenterWindow();
		m_edtNewFolderName = (CRXSkinEdit*)GetControlMgr().FindControl(ID_EDIT_NEW_FOLDER);
	}

	return bResult;
}

BOOL CRXSkinNewFolder::Create(const TCHAR* pszWindowClassName,
	const TCHAR* pszWindowName,
	const UINT32 dwStyle,
	const HWND hParentWnd, HICON hIcon /* = NULL */)
{
	BOOL bResult = CRXSkinDialog::Create(pszWindowClassName, pszWindowName, dwStyle, hParentWnd, hIcon);
	if (bResult)
	{
	}
	return bResult;
}

void CRXSkinNewFolder::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinDialog::OnPaintBk(hDC);
}

LRESULT CRXSkinNewFolder::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = CRXSkinDialog::OnHandleMessage(message, wParam, lParam);

	return nResult;
}

VOID CRXSkinNewFolder::OnClickedClose()
{
	HWND hWndParent = ::GetWindowOwner(m_hWnd);
	if (hWndParent)
	{
		::EnableWindow(hWndParent, TRUE);
	}
	::ShowWindow(GetHWnd(), SW_HIDE);
	Destroy();
	///CRXSkinDialog::OnClickedClose();

}

BOOL CRXSkinNewFolder::OnRButtonUp(UINT nFlags, POINT point)
{
	return GetControlMgr().OnRButtonUp(point) == NULL ? FALSE : TRUE;
}

BOOL CRXSkinNewFolder::OnSize(WPARAM wParam, LPARAM lParam)
{
	return CRXSkinDialog::OnSize(wParam, lParam);
}

void CRXSkinNewFolder::OnFinalMessage(HWND hWnd)
{
	CRXSkinDialog::OnFinalMessage(hWnd);
	delete this;
}

BOOL CRXSkinNewFolder::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CRXSkinDialog::OnCommand(wParam, lParam);
	switch (wParam)
	{
	case ID_BUTTON_OK:
	{
		_OnClickedOK();
	}
	break;
	case ID_BUTTON_CANCEL:
	{
		_OnClickedCancel();
	}
	break;

	default:
		break;
	}
	return TRUE;
}


VOID CRXSkinNewFolder::_OnClickedOK()
{
	if (m_edtNewFolderName)
	{
		tstring strTxt = m_edtNewFolderName->GetText();
		strTxt = _RXTStrTrimLeft(strTxt);
		strTxt = _RXTStrTrimRight(strTxt);
		if (strTxt.size() <= 0)
		{
			RXSkinMessageBox(GetHWnd(), _T("新建文件夹"), _T("文件夹名字不能为空！"), RXSKIN_MB_OK);
			return;
		}
		TCHAR szFullPath[MAX_PATH] = {0};
		_RXTsprintf(szFullPath, _T("%s\\%s"), m_strPath.c_str(), strTxt.c_str());
		RXCreateDirectory(szFullPath, TRUE);
	
	}
	OnClickedClose();
}
VOID CRXSkinNewFolder::_OnClickedCancel()
{
	OnClickedClose();
}
