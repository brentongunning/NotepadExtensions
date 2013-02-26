// ****************************************************************************
// SystemHook.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// CLASSES
// ----------------------------------------------------------------------------

// A CBT hook installed system-wide to detect notepad windows and
// install the NotepadHook. We don't install the NotepadHook across
// all programs for performance. There is only one instance per DLL.
class SystemHook
{
protected:
    SystemHook();
    ~SystemHook();

public:
    static SystemHook* GetInstance();

    void SetDllHandle(HINSTANCE dllHandle);

    bool Install();
    void Uninstall();

    void OnDllAttach();
    void OnDllDetach();

private:
    LRESULT CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticCBTProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static SystemHook* _instance;

    HINSTANCE _dllHandle;
    HHOOK _cbtHookHandle;
    bool _isNotepad;
};

// ----------------------------------------------------------------------------