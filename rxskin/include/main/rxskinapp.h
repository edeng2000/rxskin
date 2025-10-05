#pragma once
#include "RXSkin.h"
#include "controls/rxskinedit.h"
#include <vector>
#include <windows.h>
#include <queue>
#include <map>
using namespace std;

typedef BOOL(*PPROCESSFUNC)(VOID* poReceiver, MSG _msg);
struct SMessageCallBack
{
	void* poThis;
	PPROCESSFUNC  poFunc;
	SMessageCallBack()
	{
		poFunc = NULL;
		poThis = NULL;
	}
	SMessageCallBack(const SMessageCallBack& oCallBack)
	{
		poThis = oCallBack.poThis;
		poFunc = oCallBack.poFunc;
	}
};



class RXSKIN_API CRXSkinApp
{
public:
	CRXSkinApp(void);
	~CRXSkinApp(void);
	VOID      SetInstance(HINSTANCE hInstance, const TCHAR* pszCommandLine,INT32 nCmdShow);
	VOID      SetAppID(const UINT32 dwAppID);
	HINSTANCE GetInstance();
	TCHAR*    GetCommandLine();

	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	virtual BOOL Run();

	void    AddMessageCallBack(void* _poThis, PPROCESSFUNC _func);
	void    RemoveMessageCallBack(void* _poThis);

	VOID    AddSkinEdit(HWND hWnd, CRXSkinEdit* poEdit);
	void    RemoveSkinEdit(HWND hWnd);
	bool    IsLeftMousePressed();
	bool    IsRightMousePressed();
	UINT32  GetAppID() { return m_dwAppID; }


private:
	TCHAR     m_szCommandLine[512];
	HINSTANCE m_hInstance;
	INT32     m_nCmdShow;
	UINT32    m_dwAppID;
	bool      m_bLMousePressed;
	bool      m_bRMousePressed;
	vector<SMessageCallBack> m_vecKeyUp;

	map<HWND, CRXSkinEdit*>  m_mapEdit;
private:
};

extern CRXSkinApp* m_poApp;
