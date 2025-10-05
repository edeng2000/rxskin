#pragma once
#include <ExDisp.h>
#include <ExDispid.h>
#include <mshtmhst.h>

class CRXSkinWebBrowser;
class CWebBrowserEventHandler
{
public:
	CWebBrowserEventHandler() {}
	~CWebBrowserEventHandler() {}

	virtual void BeforeNavigate2(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url, VARIANT*& Flags, VARIANT*& TargetFrameName, VARIANT*& PostData, VARIANT*& Headers, VARIANT_BOOL*& Cancel) {}
	virtual void NavigateError(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url, VARIANT*& TargetFrameName, VARIANT*& StatusCode, VARIANT_BOOL*& Cancel) {}
	virtual void NavigateComplete2(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url) {}
	virtual void ProgressChange(CRXSkinWebBrowser* pWeb, LONG nProgress, LONG nProgressMax) {}
	virtual void NewWindow3(CRXSkinWebBrowser* pWeb, IDispatch** pDisp, VARIANT_BOOL*& Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl) {}
	virtual void CommandStateChange(CRXSkinWebBrowser* pWeb, long Command, VARIANT_BOOL Enable) {};
	virtual void TitleChange(CRXSkinWebBrowser* pWeb, BSTR bstrTitle) {};
	virtual void DocumentComplete(CRXSkinWebBrowser* pWeb, IDispatch* pDisp, VARIANT*& url) {}

	// interface IDocHostUIHandler
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(CRXSkinWebBrowser* pWeb,
		/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR* ppt,
		/* [in] */ IUnknown __RPC_FAR* pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR* pdispReserved)
	{
		//return E_NOTIMPL;
		//返回 E_NOTIMPL 正常弹出系统右键菜单
		return S_OK;
		//返回S_OK 则可屏蔽系统右键菜单
	}

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(CRXSkinWebBrowser* pWeb,
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR* pInfo)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE ShowUI(CRXSkinWebBrowser* pWeb,
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject __RPC_FAR* pActiveObject,
		/* [in] */ IOleCommandTarget __RPC_FAR* pCommandTarget,
		/* [in] */ IOleInPlaceFrame __RPC_FAR* pFrame,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR* pDoc)
	{
		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE HideUI(CRXSkinWebBrowser* pWeb)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE UpdateUI(CRXSkinWebBrowser* pWeb)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnableModeless(CRXSkinWebBrowser* pWeb,
		/* [in] */ BOOL fEnable)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(CRXSkinWebBrowser* pWeb,
		/* [in] */ BOOL fActivate)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(CRXSkinWebBrowser* pWeb,
		/* [in] */ BOOL fActivate)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder(CRXSkinWebBrowser* pWeb,
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR* pUIWindow,
		/* [in] */ BOOL fRameWindow)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(CRXSkinWebBrowser* pWeb,
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID __RPC_FAR* pguidCmdGroup,
		/* [in] */ DWORD nCmdID)
	{
		return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(CRXSkinWebBrowser* pWeb,
		/* [out] */ LPOLESTR __RPC_FAR* pchKey,
		/* [in] */ DWORD dw)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget(CRXSkinWebBrowser* pWeb,
		/* [in] */ IDropTarget __RPC_FAR* pDropTarget,
		/* [out] */ IDropTarget __RPC_FAR* __RPC_FAR* ppDropTarget)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetExternal(CRXSkinWebBrowser* pWeb,
		/* [out] */ IDispatch __RPC_FAR* __RPC_FAR* ppDispatch)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl(CRXSkinWebBrowser* pWeb,
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR __RPC_FAR* pchURLIn,
		/* [out] */ OLECHAR __RPC_FAR* __RPC_FAR* ppchURLOut)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject(CRXSkinWebBrowser* pWeb,
		/* [in] */ IDataObject __RPC_FAR* pDO,
		/* [out] */ IDataObject __RPC_FAR* __RPC_FAR* ppDORet)
	{
		return S_OK;
	}

	// 	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( 
	// 		/* [annotation][out] */ 
	// 		__deref_out  LPOLESTR *pchKey,
	// 		/* [in] */ DWORD dw)
	// 	{
	// 		return E_NOTIMPL;
	// 	}

	// IDownloadManager
	virtual HRESULT STDMETHODCALLTYPE Download(CRXSkinWebBrowser* pWeb,
		/* [in] */ IMoniker* pmk,
		/* [in] */ IBindCtx* pbc,
		/* [in] */ DWORD dwBindVerb,
		/* [in] */ LONG grfBINDF,
		/* [in] */ BINDINFO* pBindInfo,
		/* [in] */ LPCOLESTR pszHeaders,
		/* [in] */ LPCOLESTR pszRedir,
		/* [in] */ UINT uiCP)
	{
		return S_OK;
	}
};
