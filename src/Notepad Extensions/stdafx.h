// ****************************************************************************
// stdafx.h
// ****************************************************************************

#pragma once

// ----------------------------------------------------------------------------
// INCLUDES
// ----------------------------------------------------------------------------

#include <windows.h>

#include <time.h>

#include <string>

using namespace std;

// ----------------------------------------------------------------------------
// MACROS
// ----------------------------------------------------------------------------

#define SAFE_DELETE(x)          if (x) {delete (x); (x) = 0;}
#define SAFE_DELETE_ARRAY(x)    if (x) {delete [] (x); (x) = 0;}

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------

const unsigned int MAX_USER_STRING_LENGTH = 256;

// ----------------------------------------------------------------------------