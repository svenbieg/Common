//=============
// FileEntry.h
//=============

#pragma once


//=======
// Using
//=======

#include "Collections/Cluster.h"
#include "Collections/Map.h"
#include "Entry.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Entries {


//============
// File-Entry
//============

class FileEntry: public Entry
{
public:
	// Con-/Destructors
	FileEntry(Handle<Database> Database, UINT64 Offset);

	// Common
	BOOL Create(FileCreateMode Create, FileAccessMode Access, FileShareMode Share);
	VOID Destroy(FileShareMode Share);
	VOID Flush();
	UINT64 GetSize();
	SIZE_T Read(UINT64 Offset, VOID* Buffer, SIZE_T Size);
	BOOL SetSize(UINT64 Size);
	SIZE_T Write(UINT64 Offset, VOID const* Buffer, SIZE_T Size);

private:
	// Fragment
	typedef struct
		{
		UINT64 Offset;
		UINT64 Size;
		}Fragment;

	// Using
	using BlockList=::Collections::Cluster<UINT64>;
	using BlockIterator=BlockList::Iterator;
	using FragmentMap=::Collections::Map<UINT64, Fragment>;

	// Common
	BOOL CreateInternal();
	UINT64 GetFragment(UINT64* Offset);
	BOOL SetSizeInternal(UINT64 Size);
	Handle<BlockList> hBlockList;
	Handle<FragmentMap> hFragmentMap;
	INT iShareRead;
	INT iShareWrite;
	UINT uBlockPos;
	UINT64 uCurrent;
	UINT64 uSize;
	UINT64 uTotalSize;
};

}}}