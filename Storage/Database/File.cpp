//==========
// File.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Database.h"
#include "File.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {


//==================
// Con-/Destructors
//==================

File::File(Handle<String> path, Handle<Database> hdb, UINT64 offset):
Storage::File(path, "Storage.Database.File"),
uAccessMode(FileAccessMode::None),
uPosition(0),
uShareMode(FileShareMode::None)
{
auto entry=hdb->GetEntry(offset);
if(entry)
	{
	hEntry=entry->As<FileEntry>("Storage.Database.Entries.FileEntry");
	}
else
	{
	hEntry=new FileEntry(hdb, offset);
	}
}

File::~File()
{
Close();
}


//==============
// Input-Stream
//==============

SIZE_T File::Available()
{
ScopedLock lock(cCriticalSection);
if(uAccessMode==FileAccessMode::None)
	return 0;
UINT64 available=hEntry->Available(uPosition);
if(available>MAX_SIZE_T)
	return MAX_SIZE_T;
return (SIZE_T)available;
}

SIZE_T File::Read(VOID* pbuf, SIZE_T size)
{
ScopedLock lock(cCriticalSection);
if(uAccessMode==FileAccessMode::None)
	return 0;
SIZE_T read=hEntry->Read(uPosition, pbuf, size);
uPosition+=read;
return read;
}


//===============
// Output-Stream
//===============

VOID File::Flush()
{
ScopedLock lock(cCriticalSection);
if(uAccessMode==FileAccessMode::None)
	return;
hEntry->Flush();
}

SIZE_T File::Write(VOID const* pbuf, SIZE_T size)
{
ScopedLock lock(cCriticalSection);
if(uAccessMode!=FileAccessMode::ReadWrite)
	return 0;
SIZE_T written=hEntry->Write(uPosition, pbuf, size);
uPosition+=written;
return written;
}


//===========
// Container
//===========

FileSize File::GetSize()
{
ScopedLock lock(cCriticalSection);
if(uAccessMode==FileAccessMode::None)
	return 0;
return hEntry->GetSize();
}

BOOL File::Seek(UINT64 offset)
{
ScopedLock lock(cCriticalSection);
if(uAccessMode==FileAccessMode::None)
	return false;
if(hEntry->Seek(offset))
	{
	uPosition=offset;
	return true;
	}
return false;
}


//==============
// Storage.File
//==============

VOID File::Close()
{
ScopedLock lock(cCriticalSection);
if(uAccessMode==FileAccessMode::None)
	return;
hEntry->Close(uShareMode);
uAccessMode=FileAccessMode::None;
uShareMode=FileShareMode::None;
}

BOOL File::Create(FileCreateMode create, FileAccessMode access, FileShareMode share)
{
ScopedLock lock(cCriticalSection);
if(!hEntry)
	return false;
if(!hEntry->Open(access, share))
	return false;
uAccessMode=access;
uShareMode=share;
return true;
}

BOOL File::SetSize(UINT64 size)
{
ScopedLock lock(cCriticalSection);
if(uAccessMode!=FileAccessMode::ReadWrite)
	return false;
if(hEntry->SetSize(size))
	{
	uPosition=0;
	return true;
	}
return false;
}

}}