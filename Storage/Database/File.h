//========
// File.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Storage/Database/Entries/FileEntry.h"
#include "Storage/File.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {


//======================
// Forward-Declarations
//======================

class Directory;


//======
// File
//======

class File: public Storage::File
{
private:
	// Using
	using FileEntry=Storage::Database::Entries::FileEntry;

public:
	// Friends
	friend Directory;

	// Con-/Destructors
	File(Handle<String> Path, Handle<Database> Database, UINT64 Offset);
	~File();

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

	// Output-Stream
	VOID Flush()override;
	SIZE_T Write(VOID const* Buffer, SIZE_T Size)override;

	// Container
	virtual FileSize GetSize()override;
	BOOL Seek(UINT64 Offset)override;

	// Storage.File
	VOID Close()override;
	BOOL Create(FileCreateMode Create=FileCreateMode::OpenExisting, FileAccessMode Access=FileAccessMode::ReadOnly, FileShareMode Share=FileShareMode::ShareRead)override;
	BOOL SetSize(UINT64 Size)override;

private:
	// Common
	Handle<FileEntry> hEntry;
	UINT64 uPosition;
	FileShareMode uShareMode;
};

}}