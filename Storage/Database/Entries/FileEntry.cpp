//===============
// FileEntry.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Devices/Storage/DeviceBuffer.h"
#include "Storage/Database/Database.h"
#include "Storage/Streams/StreamReader.h"
#include "FileEntry.h"

using namespace Devices::Storage;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Entries {


//==================
// Con-/Destructors
//==================

FileEntry::FileEntry(Handle<Database> hdb, UINT64 offset):
Entry("Storage.Database.Entries.FileEntry", hdb, offset),
iShareRead(0),
iShareWrite(0),
uBlockPos(0),
uCurrent(0),
uSize(0),
uTotalSize(0)
{}


//========
// Common
//========

BOOL FileEntry::Create(FileCreateMode create, FileAccessMode access, FileShareMode share)
{
ScopedLock lock(cCriticalSection);
if(iShareRead<0)
	return false;
if(access==FileAccessMode::ReadWrite)
	{
	if(iShareWrite<0)
		return false;
	}
if(!uTotalSize)
	{
	if(!CreateInternal())
		return false;
	}
switch(share)
	{
	case FileShareMode::Exclusive:
		{
		iShareRead--;
		iShareWrite--;
		break;
		}
	case FileShareMode::ShareRead:
		{
		iShareWrite--;
		break;
		}
	default:
		{
		break;
		}
	}
Entry::Create();
return true;
}

VOID FileEntry::Destroy(FileShareMode share)
{
ScopedLock lock(cCriticalSection);
if(!uTotalSize)
	return;
switch(share)
	{
	case FileShareMode::Exclusive:
		{
		iShareRead++;
		iShareWrite++;
		break;
		}
	case FileShareMode::ShareRead:
		{
		iShareWrite++;
		break;
		}
	default:
		{
		break;
		}
	}
Entry::Destroy();
}

VOID FileEntry::Flush()
{
ScopedLock lock(cCriticalSection);
if(!uTotalSize)
	return;
hDatabase->Flush();
}

UINT64 FileEntry::GetSize()
{
ScopedLock lock(cCriticalSection);
return uSize;
}

SIZE_T FileEntry::Read(UINT64 offset, VOID* pbuf, SIZE_T size)
{
if(!pbuf||!size)
	return 0;
ScopedLock lock(cCriticalSection);
if(!uTotalSize)
	return 0;
if(!hFragmentMap)
	{
	auto device=hDatabase->GetDevice();
	UINT64 dev_size=device->GetSize();
	UINT block_size=device->GetBlockSize();
	UINT addr_size=dev_size>MAX_UINT? 8: 4;
	UINT header_size=sizeof(EntryType)+addr_size;
	UINT block_free=block_size-header_size;
	UINT available=block_free-offset;
	if(size>available)
		size=available;
	return hDatabase->Read(uOffset+header_size+offset, pbuf, size);
	}
SIZE_T read=0;
while(read<size)
	{
	SIZE_T copy=size-read;
	UINT64 frag_pos=offset;
	UINT64 frag_free=GetFragment(&frag_pos);
	if(frag_free<copy)
		copy=(SIZE_T)frag_free;
	if(!copy)
		break;
	SIZE_T copied=hDatabase->Read(frag_pos, pbuf, copy);
	if(!copied)
		break;
	read+=copied;
	offset+=copied;
	}
return read;
}

BOOL FileEntry::SetSize(UINT64 size)
{
ScopedLock lock(cCriticalSection);
if(!uTotalSize)
	return false;
if(!SetSizeInternal(size))
	return false;
return true;
}

SIZE_T FileEntry::Write(UINT64 offset, VOID const* pbufv, SIZE_T size)
{
if(!pbufv||!size)
	return 0;
ScopedLock lock(cCriticalSection);
if(!uTotalSize)
	return 0;
auto hdevice=hDatabase->GetDevice();
UINT block_size=hdevice->GetBlockSize();
if(!hFragmentMap)
	{
	UINT64 dev_size=hdevice->GetSize();
	UINT addr_size=dev_size>MAX_UINT? 8: 4;
	UINT header_size=sizeof(EntryType)+addr_size;
	UINT block_free=block_size-header_size;
	UINT block_pos=(UINT)offset+header_size;
	if(block_pos+size<=block_free)
		{
		UINT write_end=(UINT)(offset+size);
		UINT64 new_size=MAX(uSize, write_end);
		if(block_pos<uBlockPos)
			{
			Handle<Buffer> buf=new Buffer(header_size);
			EntryType id=EntryType::File;
			buf->Write(&id, sizeof(EntryType));
			buf->Write(&new_size, addr_size);
			hDatabase->Write(uOffset, buf);
			uSize=new_size;
			}
		if(uSize!=new_size)
			{
			Handle<Buffer> buf=new Buffer(addr_size);
			buf->Write(&new_size, addr_size);
			hDatabase->Write(uOffset+sizeof(EntryType), buf);
			uSize=new_size;
			}
		Handle<Buffer> buf=new Buffer(size);
		buf->Write(pbufv, size);
		hDatabase->Write(uOffset+block_pos, buf);
		uBlockPos=block_pos+(UINT)size;
		return size;
		}
	}
UINT64 available=0;
if(uSize>offset)
	available=uSize-offset;
if(size>available)
	{
	if(!SetSizeInternal(offset+size))
		return 0;
	}
BYTE* pbuf=(BYTE*)pbufv;
SIZE_T written=0;
while(written<size)
	{
	SIZE_T copy=size-written;
	UINT64 frag_pos=offset;
	UINT64 frag_free=GetFragment(&frag_pos);
	if(frag_free<copy)
		copy=(SIZE_T)frag_free;
	if(frag_pos%block_size==0)
		{
		SIZE_T erase=BlockAlign(copy, block_size);
		hdevice->Erase(frag_pos, erase);
		}
	hdevice->Write(frag_pos, &pbuf[written], copy);
	written+=copy;
	offset+=copy;
	}
return written;
}


//================
// Common Private
//================

BOOL FileEntry::CreateInternal()
{
hBlockList=nullptr;
hFragmentMap=nullptr;
uBlockPos=0;
uCurrent=0;
uSize=0;
uTotalSize=0;
auto hdevice=hDatabase->GetDevice();
UINT block_size=hdevice->GetBlockSize();
Handle<DeviceBuffer> dev_buf=new DeviceBuffer(hdevice, uOffset, 32);
StreamReader reader(dev_buf);
SIZE_T block_pos=0;
EntryType id=EntryType::None;
block_pos+=reader.Read(&id, sizeof(EntryType));
if(id!=EntryType::File)
	return false;
uTotalSize=block_size;
UINT64 dev_size=hdevice->GetSize();
UINT addr_size=dev_size>0xFFFFFFFF? sizeof(UINT64): sizeof(UINT);
UINT64 size=0;
block_pos+=reader.Read(&size, addr_size);
if(size>=dev_size)
	{
	uBlockPos=sizeof(EntryType);
	return true;
	}
uBlockPos=(UINT)block_pos;
uSize=size;
UINT block_free=block_size-block_pos;
if(uSize<=block_free)
	return true;
UINT64 offset=0;
uCurrent=uOffset;
while(1)
	{
	UINT64 next=0;
	block_pos+=reader.Read(&next, addr_size);
	while(block_pos+2*addr_size<block_size)
		{
		Fragment frag;
		frag.Offset=0;
		frag.Size=0;
		reader.Read(&frag.Offset, addr_size);
		if(frag.Offset>=dev_size)
			break;
		block_pos+=addr_size;
		block_pos+=reader.Read(&frag.Size, addr_size);
		if(!hFragmentMap)
			hFragmentMap=new FragmentMap();
		hFragmentMap->Add(offset, frag);
		offset+=frag.Size;
		uTotalSize+=frag.Size;
		}
	if(next>=dev_size)
		break;
	if(!hBlockList)
		hBlockList=new BlockList();
	hBlockList->Append(next);
	dev_buf->Seek(next);
	uCurrent=next;
	block_pos=0;
	uTotalSize+=block_size;
	}
uBlockPos=(UINT)block_pos;
return true;
}

UINT64 FileEntry::GetFragment(UINT64* offset)
{
UINT64 pos=*offset;
if(pos>=uSize)
	return 0;
auto hit=hFragmentMap->Find(pos);
if(!hit->HasCurrent())
	return 0;
if(hit->GetCurrentId()>pos)
	hit->MovePrevious();
UINT64 frag_start=hit->GetCurrentId();
UINT64 frag_pos=pos-frag_start;
auto frag=hit->GetCurrentItem();
*offset=frag.Offset+frag_pos;
return frag.Size-frag_pos;
}

BOOL FileEntry::SetSizeInternal(UINT64 size)
{
if(size==uSize)
	return true;
auto hdevice=hDatabase->GetDevice();
UINT block_size=hdevice->GetBlockSize();
UINT64 dev_size=hdevice->GetSize();
UINT addr_size=dev_size>MAX_UINT? 8: 4;
UINT header_size=sizeof(EntryType)+addr_size;
UINT block_free=block_size-header_size;
UINT64 alloc_size=BlockAlign(size, block_size);
if(size<=block_free)
	alloc_size=0;
Fragment frag;
UINT64 frag_start=0;
UINT64 frag_end=0;
if(hFragmentMap)
	{
	uBlockPos=0;
	auto frag_it=hFragmentMap->First();
	while(frag_it->HasCurrent())
		{
		frag_start=frag_it->GetCurrentId();
		frag=frag_it->GetCurrentItem();
		frag_end=frag_start+frag.Size;
		if(frag_end>=alloc_size)
			break;
		frag_it->MoveNext();
		}
	if(frag_end>alloc_size)
		{
		if(frag_start<alloc_size)
			{
			UINT64 free_size=frag_end-alloc_size;
			UINT64 free_pos=frag.Offset+frag.Size-free_size;
			if(!hDatabase->Free(free_pos, free_size))
				{
				CreateInternal();
				return false;
				}
			frag.Size-=free_size;
			frag_it->SetCurrentItem(frag);
			frag_end=alloc_size;
			}
		}
	if(frag_end==alloc_size)
		frag_it->MoveNext();
	if(frag_end>=alloc_size)
		{
		while(frag_it->HasCurrent())
			{
			auto frag=frag_it->GetCurrentItem();
			if(!hDatabase->Free(frag.Offset, frag.Size))
				{
				CreateInternal();
				return false;
				}
			frag_it->RemoveCurrent();
			}
		}
	}
if(alloc_size>frag_end)
	{
	frag.Size=alloc_size-frag_end;
	frag.Offset=hDatabase->Alloc(frag.Size);
	if(!frag.Offset)
		{
		CreateInternal();
		return false;
		}
	if(!hFragmentMap)
		hFragmentMap=new FragmentMap();
	hFragmentMap->Add(frag_end, frag);
	}
if(hFragmentMap)
	{
	if(!hFragmentMap->GetCount())
		hFragmentMap=nullptr;
	}
Handle<BlockIterator> block_it;
if(hBlockList)
	block_it=hBlockList->First();
if(hFragmentMap)
	{
	Handle<Buffer> buf=new Buffer(block_size);
	SIZE_T block_pos=0;
	EntryType id=EntryType::File;
	block_pos+=buf->Write(&id, sizeof(EntryType));
	block_pos+=buf->Write(&size, addr_size);
	SIZE_T next_pos=block_pos;
	block_pos+=buf->Fill(-1, addr_size);
	auto frag_it=hFragmentMap->First();
	uCurrent=uOffset;
	uTotalSize=block_size;
	while(frag_it->HasCurrent())
		{
		while(block_pos+2*addr_size<=block_size)
			{
			frag=frag_it->GetCurrentItem();
			block_pos+=buf->Write(&frag.Offset, addr_size);
			block_pos+=buf->Write(&frag.Size, addr_size);
			uTotalSize+=frag.Size;
			if(!frag_it->MoveNext())
				break;
			}
		UINT64 next=0;
		if(frag_it->HasCurrent())
			{
			if(block_it)
				{
				next=block_it->GetCurrent();
				if(!block_it->MoveNext())
					block_it=nullptr;
				}
			else
				{
				next=hDatabase->Alloc(0, EntryType::New);
				if(!next)
					{
					CreateInternal();
					return false;
					}
				if(!hBlockList)
					hBlockList=new BlockList();
				hBlockList->Append(next);
				}
			buf->Seek(next_pos);
			buf->Write(&next, addr_size);
			}
		Handle<Buffer> block_buf=new Buffer(block_pos);
		CopyMemory(block_buf->Begin(), buf->Begin(), block_pos);
		hDatabase->Write(uCurrent, block_buf);
		if(!next)
			break;
		uCurrent=next;
		uTotalSize+=block_size;
		buf->Seek(0);
		block_pos=buf->Fill(-1, addr_size);
		next_pos=0;
		}
	uBlockPos=(UINT)block_pos;
	}
else
	{
	if(uBlockPos==sizeof(EntryType))
		{
		Handle<Buffer> buf=new Buffer(addr_size);
		buf->Write(&size, addr_size);
		hDatabase->Write(uOffset+sizeof(EntryType), buf);
		uBlockPos=header_size;
		}
	else
		{
		if(size==0)
			{
			Handle<Buffer> buf=new Buffer(sizeof(EntryType));
			EntryType id=EntryType::File;
			buf->Write(&id, sizeof(EntryType));
			hDatabase->Write(uOffset, buf);
			uBlockPos=sizeof(EntryType);
			}
		else
			{
			Handle<Buffer> buf=new Buffer(header_size);
			EntryType id=EntryType::File;
			buf->Write(&id, sizeof(EntryType));
			buf->Write(&size, addr_size);
			hDatabase->Write(uOffset, buf);
			uBlockPos=header_size;
			}
		}
	uCurrent=0;
	uTotalSize=block_size;
	}
if(block_it)
	{
	while(block_it->HasCurrent())
		{
		UINT64 offset=block_it->GetCurrent();
		if(!hDatabase->Free(offset, block_size))
			{
			CreateInternal();
			return false;
			}
		block_it->RemoveCurrent();
		}
	}
if(hBlockList)
	{
	if(!hBlockList->GetCount())
		hBlockList=nullptr;
	}
uSize=size;
hDatabase->Flush();
return true;
}

}}}