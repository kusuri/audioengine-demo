#if _WIN32
#define JUCE_CORE_INCLUDE_COM_SMART_PTR 1
#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1

#include "../JuceLibraryCode/JuceHeader.h"

#include "DemoAudioEngine.h"

struct DemoAudioEngineCallbacksIntf : IUnknown
{
    static constexpr UUID iid = { 0xda981f98, 0x3749, 0x4af0, {0x88, 0x13, 0xf7, 0xcd, 0xd5, 0x9e, 0x16, 0x90 } };
    virtual HRESULT STDMETHODCALLTYPE filePlaybackFinished() noexcept = 0;
};

struct DemoAudioEngineIntf : IUnknown
{
    static constexpr UUID iid = { 0xc1a01b40, 0xede0, 0x44ff, {0x93, 0xf8, 0xb5, 0x93, 0x96, 0xff, 0x67, 0x8d } };

    virtual HRESULT STDMETHODCALLTYPE play(const char* url) noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE stop() noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE pause() noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE resume() noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE setRoomSize(float roomSize) noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE setLowpassCutoff(float cutoff) noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE setCallback(IUnknown* callback) noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE addWaveformComponentToHWND(void* host, void** client) noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE removeWaveformComponentFromHWND() noexcept = 0;
};

//==============================================================================
class DemoAudioEngineBindings : public DemoAudioEngineIntf
{
public:
    //==============================================================================
    DemoAudioEngineBindings()
    {
        demoAudioEngine.setPlaybackFinishedCallback([this]() { filePlaybackFinished(); });
    }

    ~DemoAudioEngineBindings()
    {
        callback = nullptr;
        demoAudioEngine.setPlaybackFinishedCallback({});
    }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** result) override
    {
        if (result == nullptr)
            return E_POINTER;

        if (riid == DemoAudioEngineIntf::iid) *result = dynamic_cast<DemoAudioEngineIntf*> (this);
        else if (riid == IID_IUnknown)        *result = dynamic_cast<IUnknown*> (this);
        else                                            return E_NOINTERFACE;

        AddRef();
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef() noexcept override
    {
        return refCnt.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    ULONG STDMETHODCALLTYPE Release() noexcept override
    {
        auto value = refCnt.fetch_sub(1, std::memory_order_relaxed) - 1;

        if (value == 0)
            delete this;

        return value;
    }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE play(const char* url) noexcept override
    {
        demoAudioEngine.play(url);
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE stop() noexcept override { demoAudioEngine.stop();    return S_OK; }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE pause() noexcept override { demoAudioEngine.pause();  return S_OK; }
    HRESULT STDMETHODCALLTYPE resume() noexcept override { demoAudioEngine.resume(); return S_OK; }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE setRoomSize(float roomSize) noexcept override { demoAudioEngine.setRoomSize(roomSize);    return S_OK; }
    HRESULT STDMETHODCALLTYPE setLowpassCutoff(float cutoff) noexcept override { demoAudioEngine.setLowpassCutoff(cutoff); return S_OK; }

    HRESULT STDMETHODCALLTYPE setCallback(IUnknown* callbackToUse) noexcept override
    {
        DemoAudioEngineCallbacksIntf* callbackIntf = nullptr;

        if (callbackToUse != nullptr)
        {
            auto status = callbackToUse->QueryInterface(DemoAudioEngineCallbacksIntf::iid, (void**)&callbackIntf);

            if (status != S_OK)
                return status;
        }

        callback = callbackIntf;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE addWaveformComponentToHWND(void* host, void** client) noexcept override
    {
        if (client == nullptr || host == nullptr)
            return E_POINTER;

        *client = demoAudioEngine.addWaveformComponentToNativeParentView(host);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE removeWaveformComponentFromHWND() noexcept override
    {
        demoAudioEngine.removeWaveformComponentFromNativeParentView();
        return S_OK;
    }

private:
    void filePlaybackFinished()
    {
        if (callback != nullptr)
            callback->filePlaybackFinished();
    }

    ScopedJuceInitialiser_GUI juceInitialiser;
    std::atomic<int> refCnt = { 1 };
    DemoAudioEngine demoAudioEngine;
    ComSmartPtr<DemoAudioEngineCallbacksIntf> callback;
};

extern "C" __declspec(dllexport) IUnknown* __stdcall CreateDemoAudioEngine()
{
    return new DemoAudioEngineBindings;
}

#endif