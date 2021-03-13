//==============
// Database.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Database/Entries/EntryType.h"
#include "Database.h"

using namespace Storage::Database::Entries;


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {


//==========
// Settings
//==========

const UINT DatabaseVersion=0x20210312;


//========
// Header
//========

typedef struct
{
EntryType Id;
UINT Version;
}DatabaseHeader;


//==================
// Con-/Destructors
//==================

Database::Database(Handle<BlockDevice> hdevice):
hDevice(hdevice),
uOldSize(0),
uSize(0),
uSizeEnd(0)
{
Current=this;
if(!hDevice)
	hDevice=BlockDevice::GetDefault();
ReadSize();
DoUpdate();
Initialize();
}


//========
// Common
//========

UINT64 Database::Alloc(UINT64 size, EntryType id)
{
ScopedLock lock(cCriticalSection);
UINT block_size=hDevice->GetBlockSize();
if(!size)
	size=block_size;
size=BlockAlign(size, block_size);
UINT64 total_size=hDevice->GetSize();
if(uSize+size>total_size)
	{
	CancelInternal();
	return 0;
	}
UINT64 offset=uSize;
uSize+=size;
hDevice->SetSize(uSize);
if(id!=EntryType::None)
	{
	Handle<Buffer> buf=new Buffer(sizeof(EntryType));
	buf->Write(&id, sizeof(EntryType));
	WriteInternal(offset, buf);
	}
return offset;
}

VOID Database::Cancel()
{
ScopedLock lock(cCriticalSection);
CancelInternal();
}

VOID Database::Clear()
{
	{
	ScopedLock lock(cCriticalSection);
	ClearInternal();
	}
Initialize();
}

Handle<Database> Database::Current;

VOID Database::Flush()
{
ScopedLock lock(cCriticalSection);
FlushInternal();
}

BOOL Database::Free(UINT64 offset, UINT64 size)
{
ScopedLock lock(cCriticalSection);
return true;
}

Handle<Entry> Database::GetEntry(UINT64 offset)
{
ScopedLock lock(cCriticalSection);
return hEntryMap->Get(offset);
}

SIZE_T Database::Read(UINT64 offset, VOID* pbufv, SIZE_T size)
{
ScopedLock lock(cCriticalSection);
if(offset>=uSize)
	return 0;
UINT64 available=uSize-offset;
if(available<size)
	size=(SIZE_T)available;
BYTE* pdst=(BYTE*)pbufv;
SIZE_T read=0;
while(read<size)
	{
	UINT64 buf_pos=offset;
	SIZE_T copy=size-read;
	auto hbuf=GetReadBuffer(&buf_pos, &copy);
	if(hbuf)
		{
		BYTE* pbuf=hbuf->Begin();
		SIZE_T buf_size=hbuf->GetSize();
		SIZE_T buf_start=(SIZE_T)(offset-buf_pos);
		if(buf_start<buf_size)
			{
			SIZE_T buf_copy=buf_size-buf_start;
			if(buf_copy>copy)
				buf_copy=copy;
			CopyMemory(&pdst[read], &pbuf[buf_start], buf_copy);
			read+=buf_copy;
			offset+=buf_copy;
			copy-=buf_copy;
			}
		if(copy>0)
			FillMemory(&pdst[read], copy, -1);
		}
	else
		{
		hDevice->Read(offset, &pdst[read], copy);
		}
	read+=copy;
	offset+=copy;
	}
return read;
}

VOID Database::Write(UINT64 offset, Handle<Buffer> hbuf)
{
if(!hbuf)
	return;
ScopedLock lock(cCriticalSection);
WriteInternal(offset, hbuf);
}


//================
// Common Private
//================

VOID Database::CancelInternal()
{
hDevice->SetSize(uOldSize);
hBufferMap=nullptr;
uSize=uOldSize;
}

VOID Database::ClearInternal()
{
CancelInternal();
UINT64 total_size=hDevice->GetSize();
UINT addr_size=total_size>MAX_UINT? 8: 4;
UINT value_size=addr_size+sizeof(UINT);
UINT block_size=hDevice->GetBlockSize();
hDevice->Erase(0, 5*block_size);
hDevice->SetSize(4*block_size);
EntryType id=EntryType::Directory;
hDevice->Write(2*block_size, &id, sizeof(EntryType));
hDevice->Write(3*block_size, &id, sizeof(EntryType));
hDevice->Flush();
UINT buf_size=sizeof(DatabaseHeader)+value_size;
Handle<Buffer> hbuf=new Buffer(buf_size);
DatabaseHeader header;
header.Id=EntryType::Database;
header.Version=DatabaseVersion;
hbuf->Write(&header, sizeof(DatabaseHeader));
hbuf->Write(&uSize, addr_size);
hbuf->Zero(sizeof(UINT));
hDevice->Write(0, hbuf->Begin(), buf_size);
hDevice->Flush();
hDevice->Write(block_size, hbuf->Begin(), buf_size);
hDevice->Flush();
uSize=4*block_size;
uSizeEnd=buf_size;
uOldSize=uSize;
}

VOID Database::DoUpdate()
{
uOldSize=uSize;
UINT64 update=uSize;
EntryType id=EntryType::None;
update+=hDevice->Read(update, &id, sizeof(EntryType));
if(id!=EntryType::Update)
	return;
UINT64 total_size=hDevice->GetSize();
UINT addr_size=total_size>MAX_UINT? 8: 4;
UINT block_size=hDevice->GetBlockSize();
while(1)
	{
	UINT64 offset=0;
	update+=hDevice->Read(update, &offset, addr_size);
	if(offset>=total_size)
		break;
	UINT size=0;
	update+=hDevice->Read(update, &size, sizeof(UINT));
	Handle<Buffer> hbuf=new Buffer(size);
	update+=hDevice->Read(update, hbuf->Begin(), size);
	SIZE_T erase=BlockAlign(size, block_size);
	if(offset%block_size)
		erase=0;
	if(erase)
		hDevice->Erase(offset, erase);
	if(hbuf)
		hDevice->Write(offset, hbuf->Begin(), size);
	hDevice->Flush();
	}
if(!hDevice->SetSize(uSize))
	{
	id=EntryType::None;
	hDevice->Write(uSize, &id, sizeof(EntryType));
	hDevice->Flush();
	}
}

VOID Database::FlushInternal()
{
if(!hBufferMap)
	return;
UINT64 total_size=hDevice->GetSize();
UINT addr_size=total_size>MAX_UINT? 8: 4;
UINT block_size=hDevice->GetBlockSize();
hDevice->Erase(uSize, block_size);
UINT64 current=uSize;
SIZE_T block_pos=sizeof(EntryType);
for(auto hit=hBufferMap->First(); hit->HasCurrent(); hit->MoveNext())
	{
	UINT64 offset=hit->GetCurrentId();
	Handle<Buffer> hbuf=hit->GetCurrentItem();
	UINT size=(UINT)hbuf->GetSize();
	UINT entry_size=addr_size+sizeof(UINT)+size;
	if(block_pos+entry_size>=block_size)
		{
		UINT erase=block_pos+entry_size-block_size;
		erase=MAX(erase, 1);
		erase=BlockAlign(erase, block_size);
		hDevice->Erase(current+block_size, erase);
		}
	block_pos+=hDevice->Write(current+block_pos, &offset, addr_size);
	block_pos+=hDevice->Write(current+block_pos, &size, sizeof(UINT));
	block_pos+=hDevice->Write(current+block_pos, hbuf->Begin(), size);
	UINT64 next=current+block_pos;
	current=next-(next%block_size);
	block_pos=(SIZE_T)(next%block_size);
	}
EntryType id=EntryType::Update;
hDevice->Write(uSize, &id, sizeof(EntryType));
hDevice->Flush();
WriteSize();
for(auto hit=hBufferMap->First(); hit->HasCurrent(); hit->MoveNext())
	{
	UINT64 offset=hit->GetCurrentId();
	Handle<Buffer> hbuf=hit->GetCurrentItem();
	SIZE_T size=hbuf->GetSize();
	SIZE_T erase=BlockAlign(size, block_size);
	if(offset%block_size)
		erase=0;
	if(erase)
		hDevice->Erase(offset, erase);
	hDevice->Write(offset, hbuf->Begin(), size);
	hDevice->Flush();
	}
if(!hDevice->SetSize(uSize))
	{
	id=EntryType::None;
	hDevice->Write(uSize, &id, sizeof(EntryType));
	hDevice->Flush();
	}
hBufferMap=nullptr;
}

Handle<Buffer> Database::GetBuffer(UINT64* poffset, SIZE_T* psize)
{
if(!hBufferMap)
	return nullptr;
UINT64 offset=*poffset;
auto hit=hBufferMap->Find(offset);
if(hit->HasCurrent())
	{
	if(hit->GetCurrentId()>offset)
		{
		if(hit->GetPosition()>0)
			hit->MovePrevious();
		}
	UINT block_size=hDevice->GetBlockSize();
	while(hit->HasCurrent())
		{
		UINT64 buf_pos=hit->GetCurrentId();
		auto hbuf=hit->GetCurrentItem();
		SIZE_T buf_size=hbuf->GetSize();
		if(buf_pos%block_size==0)
			buf_size=BlockAlign(buf_size, block_size);
		UINT64 buf_end=buf_pos+buf_size;
		if(buf_end<=offset)
			{
			hit->MoveNext();
			continue;
			}
		*poffset=buf_pos;
		*psize=buf_size;
		return hbuf;
		}
	}
return nullptr;
}

Handle<Buffer> Database::GetReadBuffer(UINT64* poffset, SIZE_T* psize)
{
UINT64 offset=*poffset;
SIZE_T size=*psize;
UINT64 buf_pos=offset;
SIZE_T buf_size=0;
auto hbuf=GetBuffer(&buf_pos, &buf_size);
if(hbuf)
	{
	if(buf_pos>offset)
		{
		SIZE_T copy=(SIZE_T)(buf_pos-offset);
		copy=MIN(size, copy);
		*psize=copy;
		return nullptr;
		}
	SIZE_T copy=buf_size;
	if(buf_pos<offset)
		copy-=(SIZE_T)(offset-buf_pos);
	UINT64 buf_end=buf_pos+buf_size;
	UINT64 end=offset+size;
	if(buf_end>end)
		copy-=(SIZE_T)(buf_end-end);
	*poffset=buf_pos;
	*psize=copy;
	return hbuf;
	}
return nullptr;
}

VOID Database::Initialize()
{
hEntryMap=new EntryMap();
UINT block_size=hDevice->GetBlockSize();
Root=new Directory("Internal", nullptr, this, 2*block_size);
hSystemRoot=new Directory("System", nullptr, this, 3*block_size);
}

VOID Database::ReadSize()
{
UINT block_size=hDevice->GetBlockSize();
UINT64 size1=0;
UINT block_pos1=ReadSize(0, &size1);
UINT64 size2=0;
UINT block_pos2=ReadSize(block_size, &size2);
if(block_pos1==0)
	{
	if(block_pos2==0)
		{
		ClearInternal();
		return;
		}
	uSize=size2;
	return;
	}
if(block_pos2==0)
	{
	uSize=size1;
	return;
	}
if(block_pos1==block_pos2)
	uSizeEnd=block_pos1;
uSize=size1;
}

UINT Database::ReadSize(UINT64 offset, UINT64* psize)
{
DatabaseHeader header;
offset+=hDevice->Read(offset, &header, sizeof(DatabaseHeader));
if(header.Id!=EntryType::Database)
	return 0;
if(header.Version!=DatabaseVersion)
	return 0;
UINT block_size=hDevice->GetBlockSize();
UINT64 dev_size=hDevice->GetSize();
UINT addr_size=dev_size>MAX_UINT? 8: 4;
UINT value_size=addr_size+sizeof(UINT);
UINT block_pos=sizeof(DatabaseHeader);
for(; block_pos+value_size<block_size; block_pos+=value_size)
	{
	UINT64 size=0;
	offset+=hDevice->Read(offset, &size, addr_size);
	if(size>=dev_size)
		break;
	UINT valid=-1;
	offset+=hDevice->Read(offset, &valid, sizeof(UINT));
	if(valid!=0)
		return 0;
	*psize=size;
	}
return block_pos;
}

VOID Database::WriteInternal(UINT64 offset, Handle<Buffer> hbuf)
{
UINT block_size=hDevice->GetBlockSize();
if(offset<2*block_size)
	return;
BYTE* pbuf=hbuf->Begin();
SIZE_T buf_size=hbuf->GetSize();
if(offset>=uOldSize)
	{
	SIZE_T erase=BlockAlign(buf_size, block_size);
	if(offset%block_size)
		erase=0;
	if(erase)
		hDevice->Erase(offset, erase);
	hDevice->Write(offset, pbuf, buf_size);
	hDevice->Flush();
	return;
	}
UINT64 buf_end=offset+buf_size;
Handle<Buffer> hbefore;
SIZE_T before_size=0;
Handle<Buffer> hnext;
SIZE_T next_start=0;
SIZE_T next_size=0;
if(hBufferMap)
	{
	auto hit=hBufferMap->Find(offset);
	UINT64 cur_pos=hit->GetCurrentId();
	if(cur_pos>offset)
		{
		if(hit->GetPosition()>0)
			hit->MovePrevious();
		}
	while(hit->HasCurrent())
		{
		cur_pos=hit->GetCurrentId();
		auto hcur=hit->GetCurrentItem();
		SIZE_T cur_size=hcur->GetSize();
		UINT64 cur_end=cur_pos+cur_size;
		if(cur_end<offset)
			{
			hit->MoveNext();
			continue;
			}
		if(cur_end==offset)
			{
			hit->RemoveCurrent();
			hbefore=hcur;
			before_size=cur_size;
			continue;
			}
		if(cur_pos>buf_end)
			break;
		if(cur_pos==buf_end)
			{
			hit->RemoveCurrent();
			hnext=hcur;
			next_size=cur_size;
			break;
			}
		if(cur_pos==offset)
			{
			if(cur_size==buf_size)
				{
				hit->SetCurrentItem(hbuf);
				return;
				}
			}
		if(cur_pos<=offset)
			{
			SIZE_T start=(SIZE_T)(offset-cur_pos);
			if(cur_end>=buf_end)
				{
				BYTE* pcur=hcur->Begin();
				CopyMemory(&pcur[start], pbuf, buf_size);
				return;
				}
			hbefore=hcur;
			before_size=start;
			hit->RemoveCurrent();
			continue;
			}
		if(cur_end>buf_end)
			{
			hnext=hcur;
			next_start=(SIZE_T)(buf_end-cur_pos);
			next_size=cur_size-next_start;
			hit->RemoveCurrent();
			break;
			}
		hit->RemoveCurrent();
		}
	}
Handle<Buffer> hnew=hbuf;
UINT64 new_pos=offset;
SIZE_T new_size=buf_size;
if(hbefore)
	{
	new_pos-=before_size;
	new_size+=before_size;
	}
if(hnext)
	new_size+=next_size;
if(new_size>buf_size)
	{
	hnew=new Buffer(new_size);
	if(hbefore)
		hnew->Write(hbefore->Begin(), before_size);
	hnew->Write(pbuf, buf_size);
	if(hnext)
		{
		BYTE* pnext=hnext->Begin();
		hnew->Write(&pnext[next_start], next_size);
		}
	}
if(!hBufferMap)
	hBufferMap=new BufferMap();
hBufferMap->Add(new_pos, hnew);
}

VOID Database::WriteSize()
{
if(uSize==uOldSize)
	return;
UINT64 total_size=hDevice->GetSize();
UINT addr_size=total_size>MAX_UINT? 8: 4;
UINT value_size=addr_size+sizeof(UINT);
UINT block_size=hDevice->GetBlockSize();
if(uSizeEnd==0||uSizeEnd+value_size>block_size)
	{
	UINT buf_size=sizeof(DatabaseHeader)+value_size;
	Handle<Buffer> hbuf=new Buffer(buf_size);
	DatabaseHeader header;
	header.Id=EntryType::Database;
	header.Version=DatabaseVersion;
	hbuf->Write(&header, sizeof(DatabaseHeader));
	hbuf->Write(&uSize, addr_size);
	hbuf->Zero(sizeof(UINT));
	hDevice->Erase(0, block_size);
	hDevice->Write(0, hbuf->Begin(), buf_size);
	hDevice->Flush();
	hDevice->Erase(block_size, block_size);
	hDevice->Write(block_size, hbuf->Begin(), buf_size);
	hDevice->Flush();
	uOldSize=uSize;
	uSizeEnd=buf_size;
	return;
	}
UINT buf_size=value_size;
Handle<Buffer> hbuf=new Buffer(buf_size);
hbuf->Write(&uSize, addr_size);
hbuf->Zero(sizeof(UINT));
hDevice->Write(uSizeEnd, hbuf->Begin(), buf_size);
hDevice->Flush();
hDevice->Write(block_size+uSizeEnd, hbuf->Begin(), buf_size);
hDevice->Flush();
uOldSize=uSize;
uSizeEnd+=buf_size;
}

}}