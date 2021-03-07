//===============
// Directory.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Directory.h"
#include "PathHelper.h"


//===========
// Namespace
//===========

namespace Storage {


//========
// Common
//========

Handle<DirectoryIterator> Directory::First()
{
return new Iterator(this);
}

Handle<String> Directory::GetName()
{
ScopedLock lock(cCriticalSection);
return PathGetName(hPath->Begin());
}

Handle<String> Directory::GetPath()
{
ScopedLock lock(cCriticalSection);
return hPath;
}

Handle<Directory> Directory::GetRoot()
{
ScopedLock lock(cCriticalSection);
if(!Parent)
	return this;
return Parent->GetRoot();
}


//============================
// Con-/Destructors Protected
//============================

Directory::Directory(Handle<String> name, Handle<String> type, Handle<Directory> parent):
Convertible(type+";Storage.Directory"),
Parent(parent),
hPath(GetPathInternal(parent, name))
{}


//================
// Common Private
//================

Handle<String> Directory::GetPathInternal(Handle<Directory> parent, Handle<String> name)
{
TCHAR ppath[MAX_PATH];
ppath[0]=0;
StringCopy(ppath, MAX_PATH, name->Begin());
while(parent)
	{
	StringInsert(ppath, MAX_PATH, "/", 0);
	StringInsert(ppath, MAX_PATH, parent->GetName()->Begin(), 0);
	parent=parent->Parent;
	}
return ppath;
}

}