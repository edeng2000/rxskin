#pragma once
#include <RichOle.h>
#include <atlbase.h>
#include <Windows.h>
#include <Richedit.h>
#include <TextServ.h>
#include <memory>
constexpr GUID CLSID_MyOLEObject = { 0xe16f8acd, 0x5b3a, 0x4167, { 0xa4, 0x49, 0xdc, 0x57, 0xd, 0xd4, 0x44, 0x59 } };

class MyOLEObject : public IOleObject, public IViewObject {
public:
    static constexpr LONG Width = 40;
    static constexpr LONG Height = 22;

    explicit MyOLEObject(ITextServices* text_service) : text_service_(text_service) {

    }
    STDMETHOD_(HRESULT, QueryInterface)(REFIID riid, LPVOID* ppvObj) override {

        if (!ppvObj) {
            return E_INVALIDARG;
        }

        if (riid == IID_IUnknown || riid == IID_IOleObject) {
            *ppvObj = static_cast<IOleObject*>(this);
            AddRef();
            return S_OK;
        }

        if (riid == IID_IViewObject) {
            *ppvObj = static_cast<IViewObject*>(this);
            AddRef();
            return S_OK;
        }

        *ppvObj = nullptr;
        return E_NOINTERFACE;
    }
    STDMETHOD_(ULONG, AddRef)() override {
        return InterlockedIncrement(&reference_count_);
    }
    STDMETHOD_(ULONG, Release)() override {
        auto result = InterlockedDecrement(&reference_count_);
        if (result == 0) {
            delete this;
        }
        return result;
    }

    STDMETHOD_(HRESULT, SetClientSite)(IOleClientSite* pClientSite) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetClientSite)(IOleClientSite** ppClientSite) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, SetHostNames)(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, Close)(DWORD dwSaveOption) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, SetMoniker)(DWORD dwWhichMoniker, IMoniker* pmk) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, InitFromData)(IDataObject* pDataObject, BOOL fCreation, DWORD dwReserved) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetClipboardData)(DWORD dwReserved, IDataObject** ppDataObject) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, DoVerb)(
        LONG iVerb,
        LPMSG lpmsg,
        IOleClientSite* pActiveSite,
        LONG lindex,
        HWND hwndParent,
        LPCRECT lprcPosRect) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, EnumVerbs)(IEnumOLEVERB** ppEnumOleVerb) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, Update)() override
   {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, IsUpToDate)(void) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetUserClassID)(CLSID* pClsid) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetUserType)(DWORD dwFormOfType, LPOLESTR* pszUserType) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, SetExtent)(DWORD dwDrawAspect, SIZEL* psizel) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetExtent)(DWORD dwDrawAspect, SIZEL* psizel) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, Advise)(IAdviseSink* pAdvSink, DWORD* pdwConnection) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, Unadvise)(DWORD dwConnectio) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, EnumAdvise)(IEnumSTATDATA** ppenumAdvise) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetMiscStatus)(DWORD dwAspect, DWORD* pdwStatus) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, SetColorScheme)(LOGPALETTE* pLogpal) override {
        return E_NOTIMPL;
    }

    STDMETHOD_(HRESULT, GetColorSet)(DWORD dwDrawAspect, LONG lindex, void* pvAspect, DVTARGETDEVICE* ptd, HDC hicTargetDev, LOGPALETTE** ppColorSet) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, Freeze)(DWORD dwDrawAspect, LONG lindex, void* pvAspect, DWORD* pdwFreeze) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, Unfreeze)(DWORD dwFreeze) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, SetAdvise)(DWORD aspects, DWORD advf, IAdviseSink* pAdvSink) override {
        return E_NOTIMPL;
    }
    STDMETHOD_(HRESULT, GetAdvise)(DWORD* pAspects, DWORD* pAdvf, IAdviseSink** ppAdvSink) override {
        return E_NOTIMPL;
    }

private:
    bool IsSelected() const {

        CHARRANGE select_range{};
        text_service_->TxSendMessage(EM_EXGETSEL, 0, reinterpret_cast<LPARAM>(&select_range), nullptr);

        if (select_range.cpMin == select_range.cpMax) {
            return false;
        }

        CComPtr<IRichEditOle> rich_edit_ole{};
        text_service_->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&rich_edit_ole, nullptr);

        auto object_count = rich_edit_ole->GetObjectCount();
        for (int index = 0; index < object_count; ++index) {

            REOBJECT object_info{};
            object_info.cbStruct = sizeof(object_info);
            HRESULT hresult = rich_edit_ole->GetObject(index, &object_info, REO_GETOBJ_POLEOBJ);
            if (FAILED(hresult)) {
                continue;
            }

            //Use CComPtr to auto release the OLE object.
            CComPtr<IOleObject> ole_object;
            ole_object.Attach(object_info.poleobj);

            if (ole_object.p == this) {

                if ((select_range.cpMin == 0 && select_range.cpMax == -1) ||
                    (select_range.cpMin <= object_info.cp && object_info.cp < select_range.cpMax)) {

                    return true;
                }
                else {
                    return false;
                }
            }
        }

        return false;
    }

    LONG reference_count_{ 1 };
    ITextServices* text_service_{};
};