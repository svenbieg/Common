//=============
// Directory.h
//=============

#pragma once


//=======
// Using
//=======

#include "Storage/Database/Entries/DirectoryEntry.h"
#include "Storage/Directory.h"
#include "File.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {


//======================
// Forward-Declarations
//======================

class Database;


//===========
// Directory
//===========

class Directory: public Storage::Directory
{
private:
	// Using
	using ChildMap=::Collections::Map<Handle<String>, UINT64>;
	using ChildIterator=ChildMap::Iterator;
	using DirectoryEntry=Storage::Database::Entries::DirectoryEntry;

public:
	// Friends
	friend File;

	// Con-/Destructors
	Directory(Handle<String> Name, Handle<Directory> Parent, Handle<Database> Database, UINT64 Offset);
	~Directory();

	// Storage.Directory
	VOID Close()override;
	Handle<Storage::File> CreateFile(Handle<String> Path, FileCreateMode CreateMode=FileCreateMode::OpenExisting, FileAccessMode AccessMode=FileAccessMode::ReadOnly, FileShareMode ShareMode=FileShareMode::ShareRead)override;
	Handle<Convertible> Get(Handle<String> Path)override;

protected:
	// Storage.Directory
	VOID FindClose()override;
	Handle<Convertible> FindCurrent()override;
	Handle<Convertible> FindFirst()override;
	Handle<Convertible> FindNext()override;

private:
	// Common
	UINT64 CreateFileInternal(Handle<String> Path, FileCreateMode CreateMode);
	Handle<Convertible> GetConvertible(Handle<String> Name);
	Handle<Convertible> GetConvertible(Handle<String> Name, UINT64 Offset);
	Handle<Convertible> hCurrent;
	Handle<Database> hDatabase;
	Handle<DirectoryEntry> hEntry;
	Handle<ChildIterator> hIt;
};

}}