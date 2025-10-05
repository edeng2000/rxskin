#pragma once

#include <Windows.h>
#include <Richedit.h>
#include <TextServ.h>
#include <memory>
#include <list>
#include <map>
using namespace std;
#include "common/rximage.h"
#include "resize/rxskinresizepoint.h"
#include "resize/rxskinresizerect.h"
#include "common/rxskindefines.h"
#include "common/rxskinutil.h"

class CRXSkinRichEditBody;
class RXSKIN_API SAutoBuf
{
public:
	SAutoBuf();

	SAutoBuf(size_t nElements);

	~SAutoBuf();

	operator char* () const;

	char* operator ->() const;

	const char& operator[] (int i) const;

	char& operator[] (int i);

	void Attach(char* pBuf, size_t size);

	//return buffer, must be freed using soui_mem_wrapper::SouiFree
	char* Detach();

	char* Allocate(size_t nElements);

	size_t size();

	void Free();
private:
	char* m_pBuf;
	size_t m_nSize;
	bool   m_bExternalBuf;
};
class RXSKIN_API CRXSkinTextHost : public ITextHost {
public:
    CRXSkinTextHost(CRXSkinRichEditBody* poOwner);
    STDMETHOD_(HRESULT, QueryInterface)(REFIID riid, void** ppvObject) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;    
    virtual HDC TxGetDC() override;
    INT TxReleaseDC(HDC hdc) override;
    BOOL TxShowScrollBar(INT fnBar, BOOL fShow) override;
    BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags) override;
    BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw) override;
    BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw) override;

    void TxInvalidateRect(LPCRECT prc, BOOL fMode) override;
    void TxViewChange(BOOL fUpdate) override;
    BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight) override;
    BOOL TxShowCaret(BOOL fShow) override;

    BOOL TxSetCaretPos(INT x, INT y) override;

    BOOL TxSetTimer(UINT idTimer, UINT uTimeout) override;

    void TxKillTimer(UINT idTimer) override;
    void TxScrollWindowEx(INT dx,
        INT dy,
        LPCRECT lprcScroll,
        LPCRECT lprcClip,
        HRGN hrgnUpdate,
        LPRECT lprcUpdate,
        UINT fuScroll) override;

    void TxSetCapture(BOOL fCapture) override;
    void TxSetFocus() override;
    void TxSetCursor(HCURSOR hcur, BOOL fText) override;
    BOOL TxScreenToClient(LPPOINT lppt) override;

    BOOL TxClientToScreen(LPPOINT lppt) override;

    HRESULT TxActivate(LONG* plOldState) override;
    HRESULT TxDeactivate(LONG lNewState) override;

    HRESULT TxGetClientRect(LPRECT prc) override;

    HRESULT TxGetViewInset(LPRECT prc) override;

    HRESULT TxGetCharFormat(const CHARFORMATW** ppCF) override;
    HRESULT TxGetParaFormat(const PARAFORMAT** ppPF) override;
    COLORREF TxGetSysColor(int nIndex) override;

    HRESULT TxGetBackStyle(TXTBACKSTYLE* pstyle) override;

    HRESULT TxGetMaxLength(DWORD* plength) override;

    HRESULT TxGetScrollBars(DWORD* pdwScrollBar) override;
    HRESULT TxGetPasswordChar(_Out_ TCHAR* pch) override;
    HRESULT TxGetAcceleratorPos(LONG* pcp) override;
    HRESULT TxGetExtent(LPSIZEL lpExtent) override;

    HRESULT OnTxCharFormatChange(const CHARFORMATW* pCF) override;
    HRESULT OnTxParaFormatChange(const PARAFORMAT* pPF) override;

    HRESULT TxGetPropertyBits(DWORD dwMask, DWORD* pdwBits) override;

    HRESULT TxNotify(DWORD iNotify, void* pv) override;

    HIMC TxImmGetContext() override;

    void TxImmReleaseContext(HIMC himc) override;

    HRESULT TxGetSelectionBarWidth(LONG* lSelBarWidth) override;
    void    Create();
	ITextServices* GetTextServices() {
        return pserv;
	}
private:
    LONG reference_count_{ 1 };
    CRXSkinRichEditBody* m_poOwner;
	ITextServices* pserv;   /**< pointer to Text Services object */

    std::unique_ptr<CHARFORMATW> char_format_;
    std::unique_ptr<PARAFORMAT> para_format_;
};
