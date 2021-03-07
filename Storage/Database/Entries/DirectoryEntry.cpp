//====================
// DirectoryEntry.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include "Devices/Storage/DeviceBuffer.h"
#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Storage/Database/Database.h"
#include "DirectoryEntry.h"

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

DirectoryEntry::DirectoryEntry(Handle<Database> hdb, UINT64 offset):
Entry("Storage.Database.Entries.DirectoryEntry", hdb, offset),
bChanged(false),
uBlockPos(0),
uCurrent(0)
{}


//========
// Common
//========

BOOL DirectoryEntry::Create()
{
ScopedLock lock(cCriticalSection);
if(!CreateInternal())
	return false;
Entry::Create();
return true;
}

VOID DirectoryEntry::Destroy()
{
ScopedLock lock(cCriticalSection);
if(!uCurrent)
	return;
Entry::Destroy();
}

VOID DirectoryEntry::Flush()
{
ScopedLock lock(cCriticalSection);
if(!bChanged)
	return;
bChanged=false;
auto hdevice=hDatabase->GetDevice();
UINT64 total_size=hdevice->GetSize();
UINT addr_size=total_size>0xFFFFFFFF? 8: 4;
UINT block_size=hdevice->GetBlockSize();
Handle<BlockIterator> block_it=nullptr;
if(hBlockList)
	block_it=hBlockList->First();
auto child_it=Children->First();
Handle<Buffer> buf=new Buffer(block_size);
StreamWriter writer(buf);
SIZE_T block_pos=0;
EntryType id=EntryType::Directory;
block_pos+=writer.Write(&id, sizeof(EntryType));
UINT64 current=uOffset;
while(current<total_size)
	{
	UINT next_pos=block_pos;
	UINT64 next=-1;
	block_pos+=writer.Write(&next, addr_size);
	while(child_it->HasCurrent())
		{
		auto name=child_it->GetCurrentId();
		UINT64 offset=child_it->GetCurrentItem();
		StreamWriter entry_writer(nullptr);
		SIZE_T entry_size=0;
		entry_size+=entry_writer.WriteString(name);
		entry_size+=entry_writer.CompressUInt64(offset);
		if(block_pos+entry_size>block_size)
			break;
		block_pos+=writer.WriteString(name);
		block_pos+=writer.CompressUInt64(offset);
		child_it->MoveNext();
		}
	if(child_it->HasCurrent())
		{
		if(block_it&&block_it->HasCurrent())
			{
			next=block_it->GetCurrent();
			block_it->MoveNext();
			}
		else
			{
			next=hDatabase->Alloc(block_size);
			if(!next)
				{
				CreateInternal();
				return;
				}
			if(!hBlockList)
				hBlockList=new BlockList();
			hBlockList->Append(next);
			}
		buf->Seek(next_pos);
		writer.Write(&next, addr_size);
		}
	UINT write_size=BlockAlign(block_pos, 4);
	if(write_size==block_size)
		{
		hDatabase->Write(current, buf);
		}
	else
		{
		Handle<Buffer> block=new Buffer(write_size);
		CopyMemory(block->Begin(), buf->Begin(), write_size);
		hDatabase->Write(current, block);
		}
	current=next;
	block_pos=0;
	}
uCurrent=current;
uBlockPos=(UINT)block_pos;
if(block_it)
	{
	while(block_it->HasCurrent())
		{
		UINT64 offset=block_it->GetCurrent();
		if(!hDatabase->Free(offset, block_size))
			{
			CreateInternal();
			return;
			}
		block_it->RemoveCurrent();
		}
	}
if(hBlockList)
	{
	if(hBlockList->GetCount()==0)
		hBlockList=nullptr;
	}
hDatabase->Flush();
}


//================
// Common Private
//================

VOID DirectoryEntry::OnChildAdded(Handle<ChildMap> hmap, Handle<String> hname, UINT64 offset)
{
ScopedLock lock(cCriticalSection);
StreamWriter writer(nullptr);
SIZE_T entry_size=0;
entry_size+=writer.WriteString(hname);
entry_size+=writer.CompressUInt64(offset);
auto hdevice=hDatabase->GetDevice();
UINT64 total_size=hdevice->GetSize();
UINT block_size=hdevice->GetBlockSize();
UINT addr_size=total_size>MAX_UINT? 8: 4;
if(uBlockPos+entry_size>block_size)
	{
	UINT64 next=hDatabase->Alloc(0, EntryType::New);
	if(!next)
		{
		CreateInternal();
		return;
		}
	UINT next_pos=0;
	if(uCurrent==uOffset)
		next_pos+=sizeof(EntryType);
	Handle<Buffer> hbuf=new Buffer(addr_size);
	hbuf->Write(&next, addr_size);
	hDatabase->Write(uCurrent+next_pos, hbuf);
	uCurrent=next;
	uBlockPos=addr_size;
	}
Handle<Buffer> hbuf=new Buffer(entry_size);
writer.SetStream(hbuf);
writer.WriteString(hname);
writer.CompressUInt64(offset);
hDatabase->Write(uCurrent+uBlockPos, hbuf);
hDatabase->Flush();
uBlockPos+=(UINT)entry_size;
}

VOID DirectoryEntry::OnChildRemoved(Handle<ChildMap> hmap)
{
bChanged=true;
}

BOOL DirectoryEntry::CreateInternal()
{
Children=new ChildMap();
bChanged=false;
hBlockList=nullptr;
uBlockPos=0;
uCurrent=0;
auto hdevice=hDatabase->GetDevice();
UINT64 dev_size=hdevice->GetSize();
UINT addr_size=dev_size>MAX_UINT? 8: 4;
UINT block_size=hdevice->GetBlockSize();
Handle<DeviceBuffer> dev_buf=new DeviceBuffer(hdevice, uOffset, 32);
StreamReader reader(dev_buf);
SIZE_T block_pos=0;
EntryType id=EntryType::None;
block_pos+=reader.Read(&id, sizeof(EntryType));
if(id!=EntryType::Directory)
	return false;
uCurrent=uOffset;
while(1)
	{
	UINT64 next=0;
	block_pos+=reader.Read(&next, addr_size);
	while(block_pos<block_size)
		{
		SIZE_T name_size=0;
		Handle<String> name=reader.ReadString('\377', &name_size);
		if(name_size<=1)
			break;
		block_pos+=name_size;
		UINT64 offset=0;
		block_pos+=reader.ExtractUInt64(&offset);
		Children->Add(name, offset);
		}
	if(next>=dev_size)
		break;
	if(!hBlockList)
		hBlockList=new BlockList();
	hBlockList->Append(next);
	dev_buf->Seek(next);
	uCurrent=next;
	block_pos=0;
	}
Children->Added.Add(this, &DirectoryEntry::OnChildAdded);
Children->Removed.Add(this, &DirectoryEntry::OnChildRemoved);
uBlockPos=(UINT)block_pos;
return true;
}

}}}