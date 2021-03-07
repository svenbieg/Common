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
if(!hEntry)
	return 0;
UINT64 size=hEntry->GetSize();
if(uPosition>=size)
	return 0;
UINT64 available=size-uPosition;
if(available>MAX_SIZE_T)
	return MAX_SIZE_T;
return (SIZE_T)available;
}

SIZE_T File::Read(VOID* pbuf, SIZE_T size)
{
ScopedLock lock(cCriticalSection);
if(!hEntry)
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
if(!hEntry)
	return;
hEntry->Flush();
}

SIZE_T File::Write(VOID const* pbuf, SIZE_T size)
{
ScopedLock lock(cCriticalSection);
if(!hEntry)
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
if(!hEntry)
	return 0;
return hEntry->GetSize();
}

BOOL File::Seek(UINT64 offset)
{
ScopedLock lock(cCriticalSection);
if(!hEntry)
	return false;
UINT64 size=hEntry->GetSize();
if(offset>=size)
	return false;
uPosition=offset;
return true;
}


//==============
// Storage.File
//==============

VOID File::Close()
{
ScopedLock lock(cCriticalSection);
if(uShareMode==FileShareMode::None)
	return;
if(hEntry)
	hEntry->Destroy(uShareMode);
uShareMode=FileShareMode::None;
}

BOOL File::Create(FileCreateMode create, FileAccessMode access, FileShareMode share)
{
return hEntry->Create(create, access, share);
}

BOOL File::SetSize(UINT64 size)
{
return hEntry->SetSize(size);
}

}}