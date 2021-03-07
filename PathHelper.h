//==============
// PathHelper.h
//==============

#pragma once


//=======
// Using
//=======

#include "StringClass.h"


//========
// Access
//========

UINT PathGetComponent(LPCSTR Path, UINT* Position, UINT Id=0);
UINT PathGetComponent(LPCWSTR Path, UINT* Position, UINT Id=0);
UINT PathGetComponentLength(LPCSTR Path);
UINT PathGetComponentLength(LPCWSTR Path);
Handle<String> PathGetDirectory(LPCSTR Path);
Handle<String> PathGetDirectory(LPCWSTR Path);
UINT PathGetDirectoryLength(LPCSTR Path);
UINT PathGetDirectoryLength(LPCWSTR Path);
LPCSTR PathGetExtension(LPCSTR Path);
LPCWSTR PathGetExtension(LPCWSTR Path);
UINT PathGetExtension(LPCSTR Path, UINT* Position);
UINT PathGetExtension(LPCWSTR Path, UINT* Position);
Handle<String> PathGetName(Handle<String> Path);
UINT PathGetName(LPCSTR Path, UINT* Position);
UINT PathGetName(LPCWSTR Path, UINT* Position);
BOOL PathIsSeparator(CHAR Char);
BOOL PathIsSeparator(WCHAR Char);
