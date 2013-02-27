// ****************************************************************************
// TextEditor.cpp
// ****************************************************************************

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "TextEditor.h"
#include "Error.h"

// ----------------------------------------------------------------------------
// STATIC MEMBER VARIABLES
// ----------------------------------------------------------------------------

TextEditor* TextEditor::_instance = NULL;

// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

TextEditor::TextEditor() :
    _editControlHandle(NULL)
{
    // No-op
}

// ----------------------------------------------------------------------------

TextEditor::~TextEditor()
{
    // No-op
}

// ----------------------------------------------------------------------------

TextEditor* TextEditor::GetInstance()
{
    if (!_instance)
    {
        _instance = new TextEditor();
    }

    return _instance;
}

// ----------------------------------------------------------------------------

bool TextEditor::Initialize(HWND notepadWindowHandle)
{
    _editControlHandle = NULL;
    EnumChildWindows(notepadWindowHandle, StaticFindEditControl, NULL);
    if (!_editControlHandle)
    {
        DisplayErrorMessageToUser("Failed to find notepad text edit area");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

void TextEditor::InsertNewLineWithSameIndentation()
{
    GetContents();

    char previousLine[MAX_USER_STRING_LENGTH];
    if (GetPreviousLine(_caret, previousLine, MAX_USER_STRING_LENGTH) != -1)
    {
        int i = 0;
        while (previousLine[i] == ' ' || previousLine[i] == '\t')
            i++;
        previousLine[i] = '\0';

        InsertNewLineWithText(previousLine);
    }
    else
    {
        InsertNewLineWithText("");
    }
}

// ----------------------------------------------------------------------------

void TextEditor::GetContents()
{
    _contentsLength = GetWindowText(_editControlHandle, _contents, MAX_FILE_LENGTH);
    _caret = (int)LOWORD(Edit_GetSel(_editControlHandle));
}

// ----------------------------------------------------------------------------

void TextEditor::ReplaceText(int caret, int length, const char* text)
{
    SendMessage(_editControlHandle, EM_SETSEL, WPARAM(caret), LPARAM(caret + length));
    SendMessage(_editControlHandle, EM_REPLACESEL, WPARAM(TRUE), LPARAM(text));
}

// ----------------------------------------------------------------------------

void TextEditor::InsertNewLineWithText(const char* text)
{
    char previousLine[MAX_USER_STRING_LENGTH];
    int previousCaret;

    char textToAdd[MAX_USER_STRING_LENGTH];
    sprintf_s(textToAdd, MAX_USER_STRING_LENGTH, "\r\n%s", text);

    // Previous line all whitespace? If so, remove the whitespace
    if ((previousCaret = GetPreviousLine(_caret, previousLine, MAX_USER_STRING_LENGTH)) != -1)
    {
        int i = 0;
        bool allWhitespace = true;
        while (previousLine[i] != '\0')
        {
            if (previousLine[i] != ' ' && previousLine[i] != '\t')
            {
                allWhitespace = false;
                break;
            }
            i++;
        }

        int lineStart = previousCaret + 2;
        if (allWhitespace)
            ReplaceText(lineStart, (int)strlen(previousLine), textToAdd);
        else ReplaceText(_caret, 0, textToAdd);

        return;
    }

    ReplaceText(_caret, 0, textToAdd);
}

// ----------------------------------------------------------------------------

BOOL CALLBACK TextEditor::StaticFindEditControl(HWND hWnd, LPARAM lParam)
{
    char className[MAX_USER_STRING_LENGTH];
    GetClassName(hWnd, className, MAX_USER_STRING_LENGTH);
    if (strcmp(className, "Edit") == 0)
    {
        TextEditor::GetInstance()->_editControlHandle = hWnd;
        return FALSE;
    }
    return TRUE;
}

// ----------------------------------------------------------------------------

int TextEditor::GetPreviousLine(int caret, char* line, int maxLength)
{
    if (caret == 0)
        return -1;

    // Find the previous line start and end positions
    int lineStart = 0;
    int lineEnd = caret - 1;
    for (int i = lineEnd; i >= 0; --i)
    {
        if (_contents[i] == '\r')
        {
            lineStart = i + 2;
            break;
        }

        if (i == 0)
        {
            lineStart = 0;
            break;
        }
    }

    // Copy the line
    int i = 0;
    for (i = 0; i < min(maxLength - 1, lineEnd - lineStart + 1); ++i)
        line[i] = _contents[i + lineStart];
    line[i] = '\0';

    return max(lineStart - 2, 0);
}

// ----------------------------------------------------------------------------