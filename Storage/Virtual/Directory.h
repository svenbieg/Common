//=============
// Directory.h
//=============

#pragma once


//=======
// Using
//=======

#include "Storage/Directory.h"
#include "DirectoryIterator.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Virtual {


//===========
// Directory
//===========

class Directory: public Storage::Directory
{
public:
	// Types
	typedef DirectoryIterator Iterator;

	// Friends
	friend Iterator;

	// Con-/Destructors
	Directory(Handle<String> Name, Handle<Directory> Parent=nullptr);

	// Common
	BOOL Add(Handle<String> Name, Handle<Convertible> Object);
	Event<Directory> Changed;
	VOID Clear();
	Handle<Iterator> First();
	Handle<Convertible> GetChild(Handle<String> Name);
	Handle<Iterator> Last();
	BOOL Remove(Handle<String> Name);

	// Storage.Directory
	VOID Close()override;
	Handle<Storage::File> CreateFile(Handle<String> Path, FileCreateMode Create=FileCreateMode::OpenExisting, FileAccessMode Access=FileAccessMode::ReadWrite, FileShareMode Share=FileShareMode::ShareRead)override;
	Handle<Convertible> Get(Handle<String> Path)override;

protected:
	// Storage.Directory
	VOID FindClose()override;
	Handle<Convertible> FindCurrent()override;
	Handle<Convertible> FindFirst()override;
	Handle<Convertible> FindNext()override;

private:
	// Common
	Handle<Iterator> hIt;
	Clusters::slist<Handle<String>, Handle<Convertible>> cList;
};

}}