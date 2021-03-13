//==================
// DirectoryEntry.h
//==================

#pragma once


//=======
// Using
//=======

#include "Collections/Cluster.h"
#include "Collections/Map.h"
#include "Storage/Streams/OutputStream.h"
#include "Entry.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Entries {


//=================
// Directory-Entry
//=================

class DirectoryEntry: public Entry
{
private:
	// Using
	using BlockList=::Collections::Cluster<UINT64>;
	using BlockIterator=BlockList::Iterator;
	using ChildMap=::Collections::Map<Handle<String>, UINT64>;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	DirectoryEntry(Handle<Database> Database, UINT64 Offset);

	// Common
	Handle<ChildMap> Children;
	VOID Close();
	VOID Flush();
	BOOL Open();

private:
	// Common
	VOID OnChildAdded(Handle<ChildMap> Map, Handle<String> Name, UINT64 Offset);
	VOID OnChildRemoved(Handle<ChildMap> Map);
	BOOL CreateInternal();
	BOOL bChanged;
	Handle<BlockList> hBlockList;
	UINT uBlockPos;
	UINT64 uCurrent;
};

}}}