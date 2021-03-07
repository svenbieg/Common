//===============
// Directory.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Database/Entries/FileEntry.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Database.h"
#include "PathHelper.h"

using namespace Storage::Database::Entries;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {


//==============
// Offset-Flags
//==============

const UINT64 DirectoryOffsetMask=(UINT64)~3;

enum class DirectoryOffsetFlags: UINT64
{
None=0,
Directory=1
};


//==================
// Con-/Destructors
//==================

Directory::Directory(Handle<String> hname, Handle<Directory> hparent, Handle<Database> hdb, UINT64 offset):
Storage::Directory(hname, "Storage.Database.Directory", hparent),
hDatabase(hdb)
{
offset&=DirectoryOffsetMask;
auto entry=hdb->GetEntry(offset);
if(entry)
	{
	hEntry=entry->As<DirectoryEntry>("Storage.Database.Entries.DirectoryEntry");
	if(!hEntry)
		return;
	}
else
	{
	hEntry=new DirectoryEntry(hdb, offset);
	}
hEntry->Create();
}

Directory::~Directory()
{
Close();
}


//===================
// Storage.Directory
//===================

VOID Directory::Close()
{
ScopedLock lock(cCriticalSection);
FindClose();
if(hEntry)
	{
	hEntry->Destroy();
	hEntry=nullptr;
	}
}

Handle<Storage::File> Directory::CreateFile(Handle<String> hpath, FileCreateMode create, FileAccessMode access, FileShareMode share)
{
ScopedLock lock(cCriticalSection);
if(!hEntry)
	return nullptr;
UINT64 offset=CreateFileInternal(hpath, create);
if(!offset)
	return nullptr;
Handle<File> hfile=new File(hpath, hDatabase, offset);
if(!hfile->Create(create, access, share))
	return nullptr;
return hfile;
}

Handle<Convertible> Directory::Get(Handle<String> hpath)
{
ScopedLock lock(cCriticalSection);
if(!hEntry)
	return nullptr;
if(!hpath)
	return this;
auto ppath=hpath->Begin();
UINT pos=0;
while(PathIsSeparator(ppath[pos]))
	pos++;
UINT len=PathGetComponentLength(&ppath[pos]);
if(!len)
	return nullptr;
Handle<String> name=new String(len, &ppath[pos]);
pos+=len;
auto obj=GetConvertible(name);
if(!obj)
	return nullptr;
if(ppath[pos]==0)
	return obj;
auto sub_dir=obj->As<Directory>("Storage.Database.Directory");
if(!sub_dir)
	return nullptr;
return sub_dir->Get(&ppath[pos]);
}


//=============================
// Storage.Directory Protected
//=============================

VOID Directory::FindClose()
{
if(!hIt)
	return;
cCriticalSection.Unlock();
hCurrent=nullptr;
hIt=nullptr;
}

Handle<Convertible> Directory::FindCurrent()
{
return hCurrent;
}

Handle<Convertible> Directory::FindFirst()
{
cCriticalSection.Lock();
if(!hEntry)
	{
	cCriticalSection.Unlock();
	return nullptr;
	}
hIt=hEntry->Children->First();
if(!hIt->HasCurrent())
	{
	hIt=nullptr;
	cCriticalSection.Unlock();
	return nullptr;
	}
auto name=hIt->GetCurrentId();
auto offset=hIt->GetCurrentItem();
hCurrent=GetConvertible(name, offset);
return hCurrent;
}

Handle<Convertible> Directory::FindNext()
{
if(!hIt)
	return nullptr;
if(!hIt->MoveNext())
	{
	FindClose();
	return nullptr;
	}
auto name=hIt->GetCurrentId();
auto offset=hIt->GetCurrentItem();
hCurrent=GetConvertible(name, offset);
return hCurrent;
}


//================
// Common Private
//================

UINT64 Directory::CreateFileInternal(Handle<String> hpath, FileCreateMode create)
{
if(!hpath||hpath->IsEmpty())
	return 0;
auto path=hpath->Begin();
UINT pos=0;
while(PathIsSeparator(path[pos]))
	pos++;
UINT len=PathGetComponentLength(&path[pos]);
if(!len)
	return 0;
Handle<String> name=new String(len, &path[pos]);
pos+=len;
UINT64 offset=hEntry->Children->Get(name);
if(path[pos]==0)
	{
	if(offset)
		{
		if(GetFlag(offset, DirectoryOffsetFlags::Directory))
			return 0;
		if(create==FileCreateMode::CreateNew)
			return 0;
		return offset;
		}
	if(create==FileCreateMode::OpenExisting)
		return 0;
	offset=hDatabase->Alloc(0, EntryType::File);
	if(!offset)
		return 0;
	hEntry->Children->Add(name, offset);
	return offset;
	}
if(offset)
	{
	if(!GetFlag(offset, DirectoryOffsetFlags::Directory))
		return 0;
	Handle<Directory> sub_dir=new Directory(name, this, hDatabase, offset);
	return sub_dir->CreateFileInternal(&path[pos], create);
	}
if(create==FileCreateMode::OpenExisting)
	return 0;
offset=hDatabase->Alloc(0, EntryType::Directory);
if(!offset)
	return 0;
SetFlag(offset, DirectoryOffsetFlags::Directory);
hEntry->Children->Add(name, offset);
Handle<Directory> sub_dir=new Directory(name, this, hDatabase, offset);
return sub_dir->CreateFileInternal(&path[pos], create);
}

Handle<Convertible> Directory::GetConvertible(Handle<String> name)
{
UINT64 offset=hEntry->Children->Get(name);
if(!offset)
	return nullptr;
return GetConvertible(name, offset);
}

Handle<Convertible> Directory::GetConvertible(Handle<String> name, UINT64 offset)
{
if(GetFlag(offset, DirectoryOffsetFlags::Directory))
	return new Directory(name, this, hDatabase, offset);
Handle<String> file_path=new String("%s/%s", hPath->Begin(), name->Begin());
return new File(file_path, hDatabase, offset);
}

}}