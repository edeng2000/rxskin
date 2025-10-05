#pragma once

#include <comdef.h>
#include <Exdisp.h>
#include <tchar.h>
#include <Windows.h>
#include <mshtmhst.h>
#include <atlbase.h>    // CComPtr
#include <MsHTML.h>
#include "docobj.h"
#include <MsHtmcid.h>
#include "common/WebBrowserEventHandler.h"
#ifndef nullptr
//#define nullptr NULL
#endif 

class RXSKIN_API  CRXSkinWebBrowser :
	public CRXControl,
	public IOleClientSite,
	public IOleInPlaceSite,
	public IStorage,
	public IOleCommandTarget,
	public IDocHostUIHandler,
	public IDispatch
{
public:
	enum MoveType
	{
		moveToDocStart,
		moveToDocEnd,
		moveFromCurrentPos
	};

	CRXSkinWebBrowser(HWND hWndParent);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl() { delete this; };
	virtual BOOL PtInRect(POINT pt) { return FALSE; };
	virtual BOOL OnClick(POINT pt) { return FALSE; };
	virtual BOOL OnMouseMove(POINT pt) { return FALSE; };
	virtual BOOL OnLButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnLButtonDblClk(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; };
	virtual VOID SetVisible(const BOOL bIsVisible);
	bool CreateBrowser();
	HWND GetControlWnd() { return m_hWndControl; }
	RECT PixelToHiMetric(const RECT& _rc);
	HRESULT MoveCursorPosition(MoveType a_eType, int a_iCharacters = 0);
	void  ClearInsertImage();
	virtual void SetRect(const CRXSkinRect rcItem);

	// ----- Control methods -----
	void Refresh();

	void Navigate(LPCWSTR lpszUrl);
	bool PutScrollTop();
	void WriteText(CRXSkinString strText);
	// ----- IUnknown -----

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
		void** ppvObject) override;

	virtual ULONG STDMETHODCALLTYPE AddRef(void);

	virtual ULONG STDMETHODCALLTYPE Release(void);

	// ---------- IOleWindow ----------

	virtual HRESULT STDMETHODCALLTYPE GetWindow(
		__RPC__deref_out_opt HWND* phwnd) override;

	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(
		BOOL fEnterMode) override;

	// ---------- IOleInPlaceSite ----------

	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE OnUIActivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE GetWindowContext(
		__RPC__deref_out_opt IOleInPlaceFrame** ppFrame,
		__RPC__deref_out_opt IOleInPlaceUIWindow** ppDoc,
		__RPC__out LPRECT lprcPosRect,
		__RPC__out LPRECT lprcClipRect,
		__RPC__inout LPOLEINPLACEFRAMEINFO lpFrameInfo) override;

	virtual HRESULT STDMETHODCALLTYPE Scroll(
		SIZE scrollExtant) override;

	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate(
		BOOL fUndoable) override;

	virtual HWND GetControlWindow();

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState(void) override;

	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void) override;

	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange(
		__RPC__in LPCRECT lprcPosRect) override;

	// ---------- IOleClientSite ----------

	virtual HRESULT STDMETHODCALLTYPE SaveObject(void) override;

	virtual HRESULT STDMETHODCALLTYPE GetMoniker(
		DWORD dwAssign,
		DWORD dwWhichMoniker,
		__RPC__deref_out_opt IMoniker** ppmk) override;

	virtual HRESULT STDMETHODCALLTYPE GetContainer(
		__RPC__deref_out_opt IOleContainer** ppContainer) override;

	virtual HRESULT STDMETHODCALLTYPE ShowObject(void) override;
	virtual HRESULT STDMETHODCALLTYPE OnShowWindow(
		BOOL fShow) override;

	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout(void) override;

	// ----- IStorage -----

	virtual HRESULT STDMETHODCALLTYPE CreateStream(
		__RPC__in_string const OLECHAR* pwcsName,
		DWORD grfMode,
		DWORD reserved1,
		DWORD reserved2,
		__RPC__deref_out_opt IStream** ppstm) override;

	virtual HRESULT STDMETHODCALLTYPE OpenStream(
		const OLECHAR* pwcsName,
		void* reserved1,
		DWORD grfMode,
		DWORD reserved2,
		IStream** ppstm) override;

	virtual HRESULT STDMETHODCALLTYPE CreateStorage(
		__RPC__in_string const OLECHAR* pwcsName,
		DWORD grfMode,
		DWORD reserved1,
		DWORD reserved2,
		__RPC__deref_out_opt IStorage** ppstg) override;

	virtual HRESULT STDMETHODCALLTYPE OpenStorage(
		__RPC__in_opt_string const OLECHAR* pwcsName,
		__RPC__in_opt IStorage* pstgPriority,
		DWORD grfMode,
		__RPC__deref_opt_in_opt SNB snbExclude,
		DWORD reserved,
		__RPC__deref_out_opt IStorage** ppstg) override;

	virtual HRESULT STDMETHODCALLTYPE CopyTo(
		DWORD ciidExclude,
		const IID* rgiidExclude,
		__RPC__in_opt  SNB snbExclude,
		IStorage* pstgDest) override;

	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(
		__RPC__in_string const OLECHAR* pwcsName,
		__RPC__in_opt IStorage* pstgDest,
		__RPC__in_string const OLECHAR* pwcsNewName,
		DWORD grfFlags) override;

	virtual HRESULT STDMETHODCALLTYPE Commit(
		DWORD grfCommitFlags) override;

	virtual HRESULT STDMETHODCALLTYPE Revert(void) override;

	virtual HRESULT STDMETHODCALLTYPE EnumElements(
		DWORD reserved1,
		void* reserved2,
		DWORD reserved3,
		IEnumSTATSTG** ppenum) override;

	virtual HRESULT STDMETHODCALLTYPE DestroyElement(
		__RPC__in_string const OLECHAR* pwcsName) override;

	virtual HRESULT STDMETHODCALLTYPE RenameElement(
		__RPC__in_string const OLECHAR* pwcsOldName,
		__RPC__in_string const OLECHAR* pwcsNewName) override;

	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(
		__RPC__in_opt_string const OLECHAR* pwcsName,
		__RPC__in_opt const FILETIME* pctime,
		__RPC__in_opt const FILETIME* patime,
		__RPC__in_opt const FILETIME* pmtime) override;

	virtual HRESULT STDMETHODCALLTYPE SetClass(
		__RPC__in REFCLSID clsid) override;
	virtual HRESULT STDMETHODCALLTYPE SetStateBits(
		DWORD grfStateBits,
		DWORD grfMask) override;

	virtual HRESULT STDMETHODCALLTYPE Stat(
		__RPC__out STATSTG* pstatstg,
		DWORD grfStatFlag) override;

	//IOleCommandTarget
	STDMETHOD(QueryStatus)(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD* prgCmds, OLECMDTEXT* pCmdText);
	STDMETHOD(Exec)(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT* pvaIn, VARIANT* pvaOut);

	HRESULT ExecCommand(const GUID* pGuid, long cmdID, long cmdExecOpt, VARIANT* pInVar = NULL, VARIANT* pOutVar = NULL);
	HRESULT ExecCommand(long cmdID, long cmdExecOpt, VARIANT* pInVar = NULL, VARIANT* pOutVar = NULL);
	long QueryStatus(long cmdID);
	
	HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD dwID, POINT* pptScreen, IUnknown* pcmdtReserved, IDispatch* pdispReserved);
	HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO* pInfo);
	HRESULT STDMETHODCALLTYPE ShowUI(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);
	HRESULT STDMETHODCALLTYPE HideUI(void);
	HRESULT STDMETHODCALLTYPE UpdateUI(void);
	HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable);
	HRESULT STDMETHODCALLTYPE OnDocWindowActivate(BOOL fActivate);
	HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(BOOL fActivate);
	HRESULT STDMETHODCALLTYPE ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fRameWindow);
	HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID);
	HRESULT STDMETHODCALLTYPE GetOptionKeyPath(LPOLESTR* pchKey, DWORD dw);
	HRESULT STDMETHODCALLTYPE GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
	HRESULT STDMETHODCALLTYPE GetExternal(IDispatch** ppDispatch);
	HRESULT STDMETHODCALLTYPE TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut);
	HRESULT STDMETHODCALLTYPE FilterDataObject(IDataObject* pDO, IDataObject** ppDORet);

	// IDispatch
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo);
	HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
	HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
	HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void    OnFinalMessage(HWND hWnd);
	void SetWebBrowserEventHandler(CWebBrowserEventHandler* poHandler) { m_poWebBrowserEventHandler = poHandler; }
	BOOL SetDesignModel(const BOOL& bDesigned);
	void Bold(BOOL bState);
	void Italic(BOOL bState);
	void FontSize(int size);
	void SetForeColor(COLORREF clr);
	short GetFontSize();
	void FontName(const TCHAR* pszFontName);
	void UnderLine(BOOL bState);
	void Copy();
	void Cut();
	VOID Paste();
	VOID SelectAll();
	BOOL IsBold();
	BOOL IsUnderline();
	BOOL IsStrikeOut();
	BOOL IsItalic();
	BOOL CanPaste();
	void InsertImage(const TCHAR* pszID, const TCHAR* pszImageUrl,const TCHAR* pszType = _T("emotion"));
	void SetImageInfo(const TCHAR* pszID, const TCHAR* pszImageUrl, const TCHAR* pszType);
	void  ModifyHistoryMsgForImage(const CRXSkinString& strID, const CRXSkinString& strUrl);
	CRXSkinString  GetFontName();
	virtual TCHAR* GetText();
	
	void AppendText(CRXSkinString strText);
	bool IsReady() { return m_pDocument != NULL; };
	void Empty();
	LPDISPATCH GetHtmlDocument() const;
	BOOL PrepareInterfaces();
	BOOL QueryDocumentState(BSTR a_bstrItemToQuery);
	HWND GetBrowserWindow();
	void ShowScrollBar();
	void SetBorderColor(COLORREF clr) { m_clrBorder = clr; }
	COLORREF GetBorderColor() { return m_clrBorder; }
	CRXSkinString  GetAttibute(CRXSkinString strName, IHTMLElement* poElement);
	CRXSkinString  VarToStr(CComVariant& v_Variant);
	static void BSTR2CString(BSTR b, CRXSkinString& s)
	{
		s = _T("");

		if (b == NULL)
			return; // empty for NULL BSTR

#ifdef UNICODE
		s = b;
#else
		LPSTR p = s.GetBuffer(SysStringLen(b) + 1);
		::WideCharToMultiByte(CP_ACP,            // ANSI Code Page
			0,                 // no flags
			b,                 // source widechar string
			-1,                // assume NUL-terminated
			p,                 // target buffer
			SysStringLen(b) + 1, // target buffer length
			NULL,              // use system default char
			NULL);             // don't care if default used
		s.ReleaseBuffer();
#endif
	}
private:
	bool OnInvoke(DISPID dispIdMember, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult);
	HRESULT OnNewWindow3(DISPPARAMS* pDispParams);
	HRESULT OnBeforeNavigate2(DISPPARAMS* pDispParams);
	HRESULT OnNavigateComplete2(DISPPARAMS* pDispParams);
	HRESULT OnNavigateError(DISPPARAMS* pDispParams);
	HRESULT OnDocumentComplete(DISPPARAMS* pDispParams);
	HRESULT OnCommandStateChange(DISPPARAMS* pDispParams);
	HRESULT ExecHelperNN(UINT nID,
		long nMinSupportLevel = OLECMDF_SUPPORTED | OLECMDF_ENABLED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);
	HRESULT ExecHelperSetVal(UINT nCmdID,
		LPCTSTR szID = NULL,
		long nMinSupportLevel = OLECMDF_SUPPORTED | OLECMDF_ENABLED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperSetVal(UINT nCmdID,
		bool bValue,
		long nMinSupportLevel = OLECMDF_SUPPORTED | OLECMDF_ENABLED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperSetVal(UINT nCmdID,
		short nNewVal,
		long nMinSupportLevel = OLECMDF_SUPPORTED | OLECMDF_ENABLED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperSetVal(UINT nCmdID,
		int nNewVal,
		long nMinSupportLevel = OLECMDF_SUPPORTED | OLECMDF_ENABLED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
		bool& bValue,
		long nMinSupportLevel = OLECMDF_SUPPORTED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
		short& nValue,
		long nMinSupportLevel = OLECMDF_SUPPORTED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
		int& nValue,
		long nMinSupportLevel = OLECMDF_SUPPORTED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);

	HRESULT ExecHelperGetVal(UINT nCmdID,
		CRXSkinString& strValue,
		long nMinSupportLevel = OLECMDF_SUPPORTED,
		long nExecOpt = OLECMDEXECOPT_DODEFAULT);
	static LRESULT CALLBACK _WndBrowserProc(HWND hWnd,
		UINT message,
		WPARAM        wParam,
		LPARAM        lParam);
protected:
	IOleObject* m_pOleObject;
	IOleInPlaceObject* m_pOleInPlaceObject;	
	IOleCommandTarget* m_pCmdTarg;
	IWebBrowser2* m_pWebBrowser2;
	CWebBrowserEventHandler* m_poWebBrowserEventHandler;
	IHTMLDocument2* m_pDocument;
	LONG m_iComRefCount;
	IDispatch* m_pDispatch;
	RECT m_rcObject;
	HWND m_hWndParent;
	HWND m_hWndControl;
	HWND m_hWebBrowserWnd;
	COLORREF  m_clrBorder;
	WNDPROC m_OldWndProc;
	CRXSkinMenu  m_oMenu;
private:
	class CStreamOnCString : public IStream
	{
	public:
		CStreamOnCString();
		CStreamOnCString(LPCTSTR szData);

		BOOL CopyData(CRXSkinString& target);
		BOOL SetData(LPCTSTR szData);
		STDMETHOD(QueryInterface)(REFIID iid, void** ppUnk);
		ULONG STDMETHODCALLTYPE AddRef(void);
		ULONG STDMETHODCALLTYPE Release(void);
		STDMETHOD(Read)(void* pv, ULONG cb, ULONG* pcbRead);
		STDMETHOD(Write)(const void* pv, ULONG cb, ULONG* pcbWritten);
		STDMETHOD(Seek)(LARGE_INTEGER, DWORD, ULARGE_INTEGER*);
		STDMETHOD(SetSize)(ULARGE_INTEGER);
		STDMETHOD(CopyTo)(IStream*, ULARGE_INTEGER, ULARGE_INTEGER*, ULARGE_INTEGER*);
		STDMETHOD(Commit)(DWORD);
		STDMETHOD(Revert)(void);
		STDMETHOD(LockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
		STDMETHOD(UnlockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
		STDMETHOD(Stat)(STATSTG*, DWORD);
		STDMETHOD(Clone)(IStream**);

	protected:
		CRXSkinString m_strStream;
		CRXSkinString m_strAnsi; //IZ replaced CStringA with CString (not sure if it will work in unicode right)
		UINT m_current_index;

		//IZ: added to simulate Append method of CString from MFC70
		void AppendToCString(CRXSkinString& a_sString, LPCTSTR a_pstrToAdd, UINT a_uiLength);

	};//CStreamOnCString
};
