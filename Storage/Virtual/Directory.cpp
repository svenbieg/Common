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
	namespace Virtual {


//==================
// Con-/Destructors
//==================

Directory::Directory(Handle<String> hname, Handle<Directory> hparent):
Storage::Directory(hname, "Collections.Directory", hparent)
{}


//========
// Common
//========

BOOL Directory::Add(Handle<String> hname, Handle<Convertible> hobj)
{
if(!hobj)
	return false;
BOOL badded=false;
	{
	ScopedLock lock(cCriticalSection);
	badded=cList.add(hname, hobj);
	}
if(badded)
	{
	Changed(this);
	return true;
	}
return false;
}

VOID Directory::Clear()
{
	{
	ScopedLock lock(cCriticalSection);
	if(cList.get_count()==0)
		return;
	cList.clear();
	}
Changed(this);
}

Handle<Directory::Iterator> Directory::First()
{
return new Iterator(this, 0);
}

Handle<Convertible> Directory::GetChild(Handle<String> hname)
{
if(!hname||hname->IsEmpty())
	return nullptr;
ScopedLock lock(cCriticalSection);
return cList.get(hname);
}

Handle<Directory::Iterator> Directory::Last()
{
Handle<Iterator> hit=new Iterator(this);
hit->Last();
return hit;
}

BOOL Directory::Remove(Handle<String> hname)
{
BOOL bremoved=false;
	{
	ScopedLock lock(cCriticalSection);
	bremoved=cList.remove(hname);
	}
if(bremoved)
	{
	Changed(this);
	return true;
	}
return false;
}


//===================
// Storage.Directory
//===================

VOID Directory::Close()
{
FindClose();
}

Handle<Storage::File> Directory::CreateFile(Handle<String> hpath, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
return nullptr;
}

Handle<Convertible> Directory::Get(Handle<String> hpath)
{
if(!hpath||hpath->IsEmpty())
	return nullptr;
ScopedLock lock(cCriticalSection);
auto ppath=hpath->Begin();
UINT upos=0;
while(PathIsSeparator(ppath[upos]))
	upos++;
UINT uclen=PathGetComponentLength(&ppath[upos]);
if(!uclen)
	return this;
Handle<String> hname=new String(uclen, &ppath[upos]);
upos+=uclen;
auto hitem=cList.get(hname);
if(!hitem)
	return nullptr;
if(ppath[upos]==0)
	return hitem;
auto hsub=hitem->As<Storage::Directory>("Storage.Directory");
lock.Release();
if(hsub)
	return hsub->Get(&ppath[upos]);
return nullptr;
}


//=============================
// Stroage.Directory Protected
//=============================

VOID Directory::FindClose()
{
hIt=nullptr;
}

Handle<Convertible> Directory::FindCurrent()
{
if(!hIt)
	return nullptr;
return hIt->GetCurrent();
}

Handle<Convertible> Directory::FindFirst()
{
hIt=nullptr;
hIt=First();
return hIt->GetCurrent();
}

Handle<Convertible> Directory::FindNext()
{
if(!hIt)
	return nullptr;
hIt->MoveNext();
return hIt->GetCurrent();
}

}}