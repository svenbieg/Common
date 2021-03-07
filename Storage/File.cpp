//==========
// File.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "File.h"
#include "PathHelper.h"


//===========
// Namespace
//===========

namespace Storage {


//========
// Common
//========

Handle<String> File::GetName()
{
ScopedLock lock(cCriticalSection);
return PathGetName(hPath);
}

Handle<String> File::GetPath()
{
ScopedLock lock(cCriticalSection);
return hPath;
}


//============================
// Con-/Destructors Protected
//============================

File::File(Handle<String> hpath, Handle<String> htype):
Convertible(htype+";Storage.File"),
hPath(hpath)
{}

}