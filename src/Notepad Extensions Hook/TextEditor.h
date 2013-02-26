// ****************************************************************************
// TextEditor.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

#define MAX_FILE_LENGTH 20000

// ----------------------------------------------------------------------------
// CLASSES
// ----------------------------------------------------------------------------

// Manages text in the notepad text window
class TextEditor
{
protected:
    TextEditor();
    ~TextEditor();

public:
    static TextEditor* GetInstance();

    bool Initialize(HWND notepadWindowHandle);
    void InsertNewLineWithSameIndentation();

private:
    void GetContents();
    void ReplaceText(int caret, int length, const char* text);
    void InsertNewLineWithText(const char* text);

private:
    static BOOL CALLBACK StaticFindEditControl(HWND hWnd, LPARAM lParam);

    // Returns the caret position to continue searching, or -1 if at the beginning/end
    int GetPreviousLine(int caret, char* line, int maxLength);

private:
    static TextEditor* _instance;

    HWND _editControlHandle;
    char _contents[MAX_FILE_LENGTH];
    int _contentsLength;
    int _caret;
};

// ----------------------------------------------------------------------------