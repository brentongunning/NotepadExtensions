// ****************************************************************************
// Autosaver.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

#define AUTOSAVE_TIMER_ID 1001

#define AUTOSAVE_FREQUENCY_IN_SECONDS 5
#define AUTOSAVE_TIMER_FREQUENCY_IN_MILLISECONDS 1000

// ----------------------------------------------------------------------------
// CLASSES
// ----------------------------------------------------------------------------

// Automatically saves notepad work periodically
class Autosaver
{
protected:
    Autosaver();
    ~Autosaver();

public:
    static Autosaver* GetInstance();

    bool Start(HWND notepadWindowHandle);
    void Stop();

    void OnUpdate();

private:
    VOID AutoSaveTimerProc(HWND hwnd,  UINT uMsg,UINT_PTR idEvent, DWORD dwTime);
    static VOID CALLBACK StaticAutoSaveTimerProc(HWND hwnd,  UINT uMsg,UINT_PTR idEvent, DWORD dwTime);

private:
    static Autosaver* _instance;

    time_t _lastUpdateTime;
    bool _saveNeeded;
    UINT_PTR _autosaveTimer;
    HWND _notepadWindowHandle;
};

// ----------------------------------------------------------------------------