#include <windows.h>
#include <objbase.h>
#include <mmreg.h>
#define WIN32_LEAN_AND_MEAN
#ifndef DECLSPEC_IMPORT
#define DECLSPEC_IMPORT __declspec(dllimport)
#endif
#define _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <sapi.h>
#include "speech.h"
#include <QString>
#include <QThread>
Speech::Speech(QObject *parent) : QObject(parent) {}

void Speech::recognizeSpeech()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        emit errorOccurred("COM initialization failed");
        return;
    }

    ISpRecognizer *pRecognizer = nullptr;
    ISpRecoContext *pContext = nullptr;
    ISpRecoGrammar *pGrammar = nullptr;
    QString recognizedText;

    // Create recognizer
    hr = CoCreateInstance(CLSID_SpSharedRecognizer, NULL,
                          CLSCTX_ALL, IID_ISpRecognizer,
                          (void **)&pRecognizer);

    if (SUCCEEDED(hr)) hr = pRecognizer->CreateRecoContext(&pContext);
    if (SUCCEEDED(hr)) hr = pContext->CreateGrammar(1, &pGrammar);
    if (SUCCEEDED(hr)) hr = pGrammar->LoadDictation(NULL, SPLO_STATIC);
    if (SUCCEEDED(hr)) hr = pGrammar->SetDictationState(SPRS_ACTIVE);

    if (SUCCEEDED(hr)) {
        // Set up event notification
        ULONGLONG interest = SPFEI(SPEI_RECOGNITION);
        pContext->SetInterest(interest, interest);

        // Wait for recognition with timeout
        bool gotResult = false;
        for(int i = 0; i < 100; i++) { // 10 second timeout
            SPEVENT event;
            ULONG fetched;
            while (pContext->GetEvents(1, &event, &fetched) == S_OK) {
                if (event.eEventId == SPEI_RECOGNITION) {
                    ISpRecoResult *pResult = reinterpret_cast<ISpRecoResult*>(event.lParam);
                    LPWSTR pwszText;
                    if (SUCCEEDED(pResult->GetText(SP_GETWHOLE_TEXT, SP_GETWHOLE_TEXT, TRUE, &pwszText, NULL))) {
                        recognizedText = QString::fromWCharArray(pwszText);
                        CoTaskMemFree(pwszText);
                        gotResult = true;
                    }
                }
                ::SpClearEvent(&event);
            }
            if (gotResult) break;
            QThread::msleep(100); // Sleep to prevent busy waiting
        }

        if (!gotResult) {
            emit errorOccurred("No speech detected");
        }
    }

    // Cleanup
    if (pGrammar) pGrammar->Release();
    if (pContext) pContext->Release();
    if (pRecognizer) pRecognizer->Release();

    CoUninitialize();

    if (!recognizedText.isEmpty()) {
        emit recognitionCompleted(recognizedText);
    } else {
        emit errorOccurred("Recognition failed");
    }
}
