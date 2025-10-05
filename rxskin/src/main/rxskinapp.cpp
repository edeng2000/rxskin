#include "pch.h"
#include "main\rxskinapp.h"
#include "common\rxskinservice.h"
#include "tchar.h"

CRXSkinApp* m_poApp = NULL;;
CRXSkinApp::CRXSkinApp(void)
{
	memset(m_szCommandLine,0,512*sizeof(TCHAR));
	m_hInstance = NULL;
	m_dwAppID = 0;
	m_poApp = this;
	m_bLMousePressed = false;
	m_bRMousePressed = false;
}

CRXSkinApp::~CRXSkinApp(void)
{

}

TCHAR* CRXSkinApp::GetCommandLine()
{
	return m_szCommandLine;
}

bool CRXSkinApp::IsLeftMousePressed()
{
	return m_bLMousePressed;
}

bool CRXSkinApp::IsRightMousePressed()
{
	return m_bRMousePressed;
}


HINSTANCE CRXSkinApp::GetInstance()
{
	return m_hInstance;
}

VOID CRXSkinApp::SetInstance(HINSTANCE hInstance, const TCHAR* pszCommandLine, INT32 nCmdShow)
{
	m_hInstance = hInstance;
	memset(m_szCommandLine, 0, 512 * sizeof(TCHAR));
	_tcsncpy(m_szCommandLine, pszCommandLine, 512);
	m_nCmdShow = nCmdShow;
}

int CRXSkinApp::ExitInstance()
{
	return TRUE;
}

BOOL CRXSkinApp::InitInstance()
{
	CRXSkinService::CreateInstance();
	CRXSkinService::Instance()->SetInstance(m_hInstance);


	return TRUE;
}

VOID CRXSkinApp::SetAppID(const UINT32 dwAppID)
{
	m_dwAppID = dwAppID;
}



BOOL CRXSkinApp::Run()
{
	MSG msg;
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(m_dwAppID));
	while (GetMessage(&msg, NULL, 0, 0))
	{		
// 		CRXSkinWnd* poCurWnd = CRXSkinService::Instance()->FindWnd(msg.hwnd);
// 		if (NULL == poCurWnd)
// 		{
// 			::DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
// 			continue;
// 		}
	//	poCurWnd->OnProcessMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	
		BOOL bResult = FALSE;
		for (int m=0;m < (int)m_vecKeyUp.size();m ++)
		{
			bResult = (*(m_vecKeyUp[m].poFunc))(m_vecKeyUp[m].poThis, msg);
			if (bResult)
			{
				continue;
			}
		}
		if (msg.message == WM_KEYDOWN )
		{
			printf("");

		}
		if (msg.message == WM_KEYDOWN && TCHAR(msg.wParam) == VK_RETURN)
		{
		
			map<HWND, CRXSkinEdit*>::iterator itr = m_mapEdit.find(msg.hwnd);
			if (itr!=m_mapEdit.end())
			{
				itr->second->OnReturn();
			}
		}
		if (msg.message == WM_LBUTTONDOWN)
		{
			m_bLMousePressed = true;
		}
		if (msg.message == WM_LBUTTONUP)
		{
			m_bLMousePressed = false;
		}
		
		if (msg.message == WM_RBUTTONDOWN)
		{
			m_bRMousePressed = true;
		}
		if (msg.message == WM_RBUTTONUP)
		{
			m_bRMousePressed = false;
		}
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			::TranslateMessage(&msg);
			try {
				::DispatchMessage(&msg);
			}
			catch (...) {
				LOG_FATAL("EXCEPTION");
			}
		}
		CRXSkinService::Instance()->RecycleWnd();
	}
	return TRUE;
}



void CRXSkinApp::AddMessageCallBack(void* _poThis, PPROCESSFUNC _func)
{
	SMessageCallBack oCall;
	oCall.poFunc = _func;
	oCall.poThis = _poThis;
	m_vecKeyUp.push_back(oCall);
}

void CRXSkinApp::RemoveMessageCallBack(void* _poThis)
{
	vector<SMessageCallBack>::iterator itr = m_vecKeyUp.begin();
	for (;itr!=m_vecKeyUp.end();)
	{
		if ((*itr).poThis == _poThis)
		{
			itr = m_vecKeyUp.erase(itr);
		}
		else
			itr++;
	}
	
}

void CRXSkinApp::AddSkinEdit(HWND hWnd, CRXSkinEdit* poEdit)
{
	m_mapEdit[hWnd] = poEdit;
}

void CRXSkinApp::RemoveSkinEdit(HWND hWnd)
{
	map<HWND, CRXSkinEdit*>::iterator itr = m_mapEdit.find(hWnd);
	if (itr!=m_mapEdit.end())
	{
		m_mapEdit.erase(itr);
	}
}

CRXLogger* m_poRXSkinLogger = NULL;
void RXSKIN_API RXSkinSetLogger(CRXLogger* poLogger)
{
	m_poRXSkinLogger = poLogger;
}