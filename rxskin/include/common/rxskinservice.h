#pragma once
#include "RXSkin.h"
#include "drawmgr.h"
#include "common/rxskinutil.h"
#include "common/imagemgr.h"
#include <map>
#include <unordered_map>
#include "mkmutex.h"
#include "rxsingleton.h"
using namespace std;

#pragma warning(disable: 4251)
class CRXSkinWnd;

class RXSKIN_API CRXSkinService
{
public:

	CRXSkinService(void);
	~CRXSkinService(void);
	CRXSkinService(const CRXSkinService&) = delete; // ��ֹ�������캯��
	CRXSkinService& operator=(const CRXSkinService&) = delete; // ��ֹ��ֵ������


	typedef unordered_map<HWND,CRXSkinWnd*>        CRXSkinWndMap;
	typedef CRXSkinWndMap::iterator      CRXSkinWndMapItr;
	static CRXSkinService* Instance();

	HINSTANCE GetInstance();
	VOID      SetInstance(HINSTANCE hInstance);

	BOOL   Init(const TCHAR* pszConfigFile);
	BOOL   InitResource(const TCHAR* pszResource);
	VOID   UnInit();

	UINT32   GetLogLevel();

	VOID     RemoveLogLevel(const UINT32 nLevel);
	VOID     AddLogLevel(const UINT32 nLevel);

	CRXSkinWnd*       RemoveWnd(const HWND hWnd);
	VOID              AddWnd(CRXSkinWnd* poWnd);
	CRXSkinWnd*       FindWnd(const HWND hWnd);
	CRXSkinWndMap&    GetWndMap();

	CDrawMgr&         GetDrawMgr();
	CImageMgr&        GetImageMgr();

	CRXSkinStdPtrArray& GetPreMessages();
	void    AddRecycleWnd(CRXSkinWnd* poWnd);
	void    AddModelWnd(HWND hWnd);
	void    RecycleWnd();
	void    SetModelWnd(CRXSkinWnd* poWnd);
	EResourceType GetResourceModel() { return m_enResourceModel; }

	bool    IsFromZipResource() { return m_enResourceModel == RESOURCE_FROM_ZIP; }
	bool    IsFromFileResource() { return m_enResourceModel == RESOURCE_FROM_FILE; }
	void    SetResourceModel(const EResourceType& enModel) 
	{
		m_enResourceModel = enModel; 
	}

	void    SetResourceRootPath(const TCHAR* pszPath) { m_strResourceRootPath = pszPath; }

	const tstring& GetResourceRootPath() { return m_strResourceRootPath; }
private:
	CRXSkinWndMap     m_oWndMap;
	HINSTANCE         m_hInstance;
	UINT32		      m_dwLogLevel;
	CDrawMgr          m_oDrawMgr;
	CImageMgr         m_oImageMgr;
	CRXSkinStdPtrArray m_aPreMessages;
	CRXSkinWnd* m_poModelWnd;
	HMODULE			  m_hSciLexerDll;
	vector<HWND> m_vecModel;
	vector<CRXSkinWnd*> m_vecRecycle;
	EResourceType  m_enResourceModel;
	tstring           m_strResourceRootPath;
private:
	VOID              _DeleteAllWnd();
};
