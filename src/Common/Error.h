// ****************************************************************************
// Error.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// FUNCTIONS
// ----------------------------------------------------------------------------

// Displays a message box with the error message with an optional parameter
void DisplayErrorMessageToUser(const char* message, const char* param = NULL);

// Displays the last Win32 error from GetLastError() with the message
void DisplayWin32ErrorMessageToUser(const char* message);

// ----------------------------------------------------------------------------