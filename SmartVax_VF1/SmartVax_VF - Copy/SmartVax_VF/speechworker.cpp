#include "speechworker.h"
#include "speechworker.h"
#include <QDebug>
#include <QThread>

SpeechWorker::SpeechWorker(QObject *parent) : QObject(parent) {}

void SpeechWorker::startRecognition()
{
    m_running = true;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        emit error("COM initialization failed");
        return;
    }

    ISpRecognizer *pRecognizer = nullptr;
    hr = CoCreateInstance(CLSID_SpSharedRecognizer, nullptr, CLSCTX_ALL,
                          IID_ISpRecognizer, (void **)&pRecognizer);
    if (FAILED(hr)) {
        emit error("Failed to create recognizer");
        CoUninitialize();
        return;
    }

    ISpRecoContext *pContext = nullptr;
    hr = pRecognizer->CreateRecoContext(&pContext);
    if (FAILED(hr)) {
        emit error("Failed to create recognition context");
        pRecognizer->Release();
        CoUninitialize();
        return;
    }

    // Set up event notification
    hr = pContext->SetNotifyWin32Event();
    HANDLE hEvent = pContext->GetNotifyEventHandle();

    ISpRecoGrammar *pGrammar = nullptr;
    hr = pContext->CreateGrammar(1, &pGrammar);
    hr = pGrammar->LoadDictation(NULL, SPLO_STATIC);
    hr = pGrammar->SetDictationState(SPRS_ACTIVE);

    while (m_running) {
        WaitForSingleObject(hEvent, 100); // Check every 100ms
        SPEVENT event;
        ULONG fetched;
        while (m_running && pContext->GetEvents(1, &event, &fetched) == S_OK) {
            if (event.eEventId == SPEI_RECOGNITION) {
                ISpPhrase *pPhrase = reinterpret_cast<ISpPhrase*>(event.lParam);
                SPPHRASE *pElements;
                if (SUCCEEDED(pPhrase->GetPhrase(&pElements))) {
                    wchar_t *pwszText;
                    if (SUCCEEDED(pPhrase->GetText(SP_GETWHOLE_TEXT,
                                                   SP_GETWHOLE_TEXT,
                                                   TRUE,
                                                   &pwszText,
                                                   NULL))) {
                        QString text = QString::fromWCharArray(pwszText);
                        CoTaskMemFree(pwszText);
                        emit recognized(text);
                    }
                    CoTaskMemFree(pElements);
                }
            }
            pPhrase->Release();
        }
    }

    // Cleanup
    pGrammar->Release();
    pContext->Release();
    pRecognizer->Release();
    CoUninitialize();
}

void SpeechWorker::stopRecognition()
{
    m_running = false;
}
SpeechWorker::SpeechWorker() {}
