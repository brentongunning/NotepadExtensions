// ****************************************************************************
// Autosaver.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

#define AUTOSAVE_TIMER_ID 1001

#define AUTOSAVE_FREQUENCY_IN_SECONDS 10
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

    // Call this when there are keyboard events possibly triggering an auto-update
    void OnUpdate();

private:
    VOID AutoSaveTimerProc(HWND hwnd,  UINT uMsg,UINT_PTR idEvent, DWORD dwTime);
    static VOID CALLBACK StaticAutoSaveTimerProc(HWND hwnd,  UINT uMsg,UINT_PTR idEvent, DWORD dwTime);

private:
    static Autosaver* _instance;

    time_t _lastUpdateTime;         // The last time that a key press occurred in notepad
    bool _saveCheckNeeded;          // Whether or not we should check that a save is needed
	string _lastContents;           // The contents of the text area the last time we saved
    UINT_PTR _autosaveTimer;        // ID to the Win32 timer used to periodically check and save
    HWND _notepadWindowHandle;      // Win32 Handle to the notepad instance
};

// ----------------------------------------------------------------------------