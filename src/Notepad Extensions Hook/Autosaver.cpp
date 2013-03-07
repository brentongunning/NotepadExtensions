// ****************************************************************************
// Autosaver.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "Autosaver.h"
#include "Error.h"
#include "TextEditor.h"

// ----------------------------------------------------------------------------
// STATIC MEMBER VARIABLES
// ----------------------------------------------------------------------------

Autosaver* Autosaver::_instance = NULL;

// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

Autosaver::Autosaver() :
    _saveCheckNeeded(false),
    _autosaveTimer(NULL),
    _notepadWindowHandle(NULL),
    _lastContents("")
{
    // No-op
}

// ----------------------------------------------------------------------------

Autosaver::~Autosaver()
{
    // No-op
}

// ----------------------------------------------------------------------------

Autosaver* Autosaver::GetInstance()
{
    if (!_instance)
    {
        _instance = new Autosaver();
    }

    return _instance;
}

// ----------------------------------------------------------------------------

bool Autosaver::Start(HWND notepadWindowHandle)
{
    _notepadWindowHandle = notepadWindowHandle;
    _lastUpdateTime = time(0);
    _saveCheckNeeded = false;
	_lastContents = "";

    // Start the autosave timer
    _autosaveTimer = SetTimer(NULL, AUTOSAVE_TIMER_ID, AUTOSAVE_TIMER_FREQUENCY_IN_MILLISECONDS, StaticAutoSaveTimerProc);
    if (!_autosaveTimer)
    {
        DisplayWin32ErrorMessageToUser("Failed to start autosave timer");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

void Autosaver::Stop()
{
    KillTimer(NULL, _autosaveTimer);
}

// ----------------------------------------------------------------------------

void Autosaver::OnUpdate()
{
    _lastUpdateTime = time(0);
    _saveCheckNeeded = true;
}

// ----------------------------------------------------------------------------

VOID Autosaver::AutoSaveTimerProc(HWND hwnd,  UINT uMsg,UINT_PTR idEvent, DWORD dwTime)
{
    if (!_saveCheckNeeded || GetForegroundWindow() != _notepadWindowHandle)
        return;

    // Make sure the file has been saved once
    char title[MAX_USER_STRING_LENGTH];
    GetWindowText(_notepadWindowHandle, title, MAX_USER_STRING_LENGTH);
    if (strcmp(title, "Untitled - Notepad") == 0)
        return;

    // See how many seconds have passed since the last time we saved
    time_t now = time(0);
    double diff = difftime(now, _lastUpdateTime);
    if (diff > AUTOSAVE_FREQUENCY_IN_SECONDS)
    {
        _lastUpdateTime = time(0);
        _saveCheckNeeded = false;

		string currentContents = TextEditor::GetInstance()->ReadContents();
		if (currentContents != _lastContents)
		{
			// Send a CTRL+S event to save
			INPUT input[4];
			memset(input, 0, sizeof(INPUT) * 4);
			input[0].type = INPUT_KEYBOARD;
			input[0].ki.wVk = VK_CONTROL;
			input[1].type = INPUT_KEYBOARD;
			input[1].ki.wVk = 0x53; // VK_S
			input[2].type = INPUT_KEYBOARD;
			input[2].ki.wVk = 0x53; // VK_S
			input[2].ki.dwFlags = KEYEVENTF_KEYUP;
			input[3].type = INPUT_KEYBOARD;
			input[3].ki.wVk = VK_CONTROL;
			input[3].ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(4, input, sizeof(INPUT));

			_lastContents = currentContents;
		}
    }
}

// ----------------------------------------------------------------------------

VOID CALLBACK Autosaver::StaticAutoSaveTimerProc(HWND hwnd,  UINT uMsg,UINT_PTR idEvent, DWORD dwTime)
{
    Autosaver::GetInstance()->AutoSaveTimerProc(hwnd, uMsg, idEvent, dwTime);
}

// ----------------------------------------------------------------------------