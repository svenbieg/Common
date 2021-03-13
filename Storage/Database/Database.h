//============
// Database.h
//============

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Devices/Storage/BlockDevice.h"
#include "Storage/Database/Entries/Entry.h"
#include "Storage/Buffer.h"
#include "Directory.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {


//==========
// Database
//==========

class Database: public Object
{
private:
	// Using
	using BlockDevice=Devices::Storage::BlockDevice;
	using BufferMap=::Collections::Map<UINT64, Handle<Buffer>>;
	using Entry=Storage::Database::Entries::Entry;
	using EntryMap=::Collections::Map<UINT64, Handle<Entry>>;
	using EntryType=Storage::Database::Entries::EntryType;

public:
	// Friends
	friend Entry;

	// Con-/Destructors
	Database(Handle<BlockDevice> Device=BlockDevice::Current);

	// Common
	UINT64 Alloc(UINT64 Size=0, EntryType Id=EntryType::None);
	VOID Cancel();
	VOID Clear();
	static Handle<Database> Current;
	VOID Flush();
	BOOL Free(UINT64 Offset, UINT64 Size=0);
	Handle<BlockDevice> GetDevice()const { return hDevice; }
	Handle<Entry> GetEntry(UINT64 Offset);
	SIZE_T Read(UINT64 Offset, VOID* Buffer, SIZE_T Size);
	Handle<Directory> Root;
	VOID Write(UINT64 Offset, Handle<Buffer> Buffer);

private:
	// Common
	VOID CancelInternal();
	VOID ClearInternal();
	VOID DoUpdate();
	VOID FlushInternal();
	Handle<Buffer> GetBuffer(UINT64* Offset, SIZE_T* Size);
	Handle<Buffer> GetReadBuffer(UINT64* Offset, SIZE_T* Size);
	VOID Initialize();
	VOID ReadSize();
	UINT ReadSize(UINT64 Offset, UINT64* Size);
	VOID WriteInternal(UINT64 Offset, Handle<Buffer> Buffer);
	VOID WriteSize();
	CriticalSection cCriticalSection;
	Handle<BufferMap> hBufferMap;
	Handle<BlockDevice> hDevice;
	Handle<EntryMap> hEntryMap;
	Handle<Directory> hSystemRoot;
	UINT64 uOldSize;
	UINT64 uSize;
	UINT uSizeEnd;
};

}}