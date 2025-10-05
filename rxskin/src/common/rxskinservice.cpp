#include "pch.h"
#include "common\RXSkinService.h"
#include "main\rxskindialog.h"
#include "common\rxskinutil.h"
#include "common\rximagemodel.h"
#include "common/rxskinresourcemgr.h"
#include "common/rxskinfont.h"
#include <Windows.h>
#include <Windowsx.h>
#include <ObjBase.h>

RX_IMPLEMENT_SINGLETON(CRXSkinService);
CRXSkinService::CRXSkinService(void)
{
	m_hInstance = NULL;
	m_hSciLexerDll = NULL;
	m_poModelWnd = NULL;
	m_enResourceModel = RESOURCE_FROM_FILE;
}

CRXSkinService::~CRXSkinService(void)
{
}

void CRXSkinService::SetModelWnd(CRXSkinWnd* poWnd)
{
	m_poModelWnd = poWnd;
}


BOOL CRXSkinService::Init(const TCHAR* pszConfigFile)
{
	if(NULL == CRXImageModule::Instance())
		CRXImageModule::CreateInstance();
	if(NULL == CRXSkinResourceMgr::Instance())
		CRXSkinResourceMgr::CreateInstance();
	if (NULL == CRXSkinFontMgr::Instance())
		CRXSkinFontMgr::CreateInstance();
	CRXSkinFontMgr::Instance()->Init(pszConfigFile);
	if (FALSE == m_oDrawMgr.Init(pszConfigFile))
	{
		return FALSE;
	}
	RXSkinInitTable();
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	return TRUE;
}

BOOL CRXSkinService::InitResource(const TCHAR* pszResource)
{
	CRXImageModule::CreateInstance();
	CRXSkinResourceMgr::CreateInstance();
	if (FALSE == m_oDrawMgr.InitResource(pszResource))
	{
		return FALSE;
	}
	RXSkinInitTable();
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	return TRUE;
}

VOID CRXSkinService::UnInit()
{
	m_oDrawMgr.UnInit();
	m_oImageMgr.UnInit();
	_DeleteAllWnd();
	
	if (m_hSciLexerDll)
	{
		FreeLibrary(m_hSciLexerDll);
		m_hSciLexerDll = NULL;
	}

	CRXImageModule::DestroyInstance();
}

VOID CRXSkinService::_DeleteAllWnd()
{
// 	CRXSkinDialogMapItr itr = m_oWndMap.begin();
// 	for (;itr!=m_oWndMap.end();itr++)
// 	{
// 		itr->second->Destroy();
// 	}
// 	m_oWndMap.clear();
}

CRXSkinService::CRXSkinWndMap& CRXSkinService::GetWndMap()
{
	return m_oWndMap;
}

CRXSkinWnd* CRXSkinService::FindWnd(const HWND hWnd)
{
	//m_oLock.Lock();
	CRXSkinWndMapItr itr = m_oWndMap.find((HWND)hWnd);
	if (itr!=m_oWndMap.end())
	{
		CRXSkinWnd* poWnd = itr->second;
	//	m_oLock.Unlock();
		return poWnd;
	}
//	m_oLock.Unlock();
	return NULL;
}

VOID CRXSkinService::AddWnd(CRXSkinWnd* poWnd)
{
	if (NULL == FindWnd(poWnd->GetHWnd()))
	{
	//	m_oLock.Lock();
		m_oWndMap.insert(make_pair(poWnd->GetHWnd(),poWnd));
	//	m_oLock.Unlock();
	}
}

CRXSkinWnd* CRXSkinService::RemoveWnd(const HWND hWnd)
{
//	m_oLock.Lock();
	CRXSkinWndMapItr itr = m_oWndMap.find((HWND)hWnd);
	if (itr!=m_oWndMap.end())
	{
		CRXSkinWnd* poWnd = itr->second;
		m_oWndMap.erase(itr);
	//	m_oLock.Unlock();
		return poWnd;
	}
//	m_oLock.Unlock();
	return NULL;
}

void CRXSkinService::AddRecycleWnd(CRXSkinWnd* poWnd)
{
	vector<HWND>::iterator itr = std::find(m_vecModel.begin(), m_vecModel.end(), poWnd->GetHWnd());
	if (itr!=m_vecModel.end())
	{
		return;
	}
	m_vecRecycle.push_back(poWnd);
}

void CRXSkinService::RecycleWnd()
{
	for (uint32_t t=0;t<(uint32_t)m_vecRecycle.size();t++)
	{
		m_vecRecycle[t]->OnFinalMessage(m_vecRecycle[t]->GetHWnd());
	}
	m_vecRecycle.clear();
}

VOID CRXSkinService::SetInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	if (m_hSciLexerDll)
	{
		FreeLibrary(m_hSciLexerDll);
		m_hSciLexerDll = NULL;
	}
	tstring strPath = GetModulePath();
	strPath += _T("/plugins/Scintilla.dll");
	m_hSciLexerDll = LoadLibrary(strPath.c_str());
	if (NULL == m_hSciLexerDll)
	{
		return;
	}

}

HINSTANCE CRXSkinService::GetInstance()
{
	return m_hInstance;
}

VOID CRXSkinService::AddLogLevel(const UINT32 nLevel)
{
	m_dwLogLevel |= nLevel;
}

VOID CRXSkinService::RemoveLogLevel(const UINT32 nLevel)
{
	m_dwLogLevel &= ~nLevel;
}

UINT32 CRXSkinService::GetLogLevel()
{
	return m_dwLogLevel;
}

CDrawMgr& CRXSkinService::GetDrawMgr()
{
	return m_oDrawMgr;
}

CImageMgr& CRXSkinService::GetImageMgr()
{
	return m_oImageMgr;
}

CRXSkinStdPtrArray& CRXSkinService::GetPreMessages()
{
	return m_aPreMessages;
}


void CRXSkinService::AddModelWnd(HWND hWnd)
{
	vector<HWND>::iterator itr = std::find(m_vecModel.begin(), m_vecModel.end(), hWnd);
	if (itr==m_vecModel.end())
	{
		m_vecModel.push_back(hWnd);
	}
}