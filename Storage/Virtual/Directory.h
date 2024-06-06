//=============
// Directory.h
//=============

#pragma once


//=======
// Using
//=======

#include "Clusters/shared_map.hpp"
#include "Storage/Directory.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Virtual {


//======================
// Forward-Declarations
//======================

class DirectoryIterator;


//===========
// Directory
//===========

class Directory: public Storage::Directory
{
public:
	// Friends
	friend DirectoryIterator;

	// Con-/Destructors
	Directory(Handle<String> Path);

	// Common
	BOOL Add(Handle<String> Name, Handle<Object> Object, BOOL Notify=true);
	Event<Directory> Changed;
	VOID Clear();
	Handle<Object> GetChild(Handle<String> Name);
	BOOL Remove(Handle<String> Name, BOOL Notify=true);
	VOID RemoveAll();

	// Storage.Directory
	Handle<Storage::File> CreateFile(Handle<String> Path, FileCreateMode Create=FileCreateMode::OpenExisting, FileAccessMode Access=FileAccessMode::ReadWrite, FileShareMode Share=FileShareMode::ShareRead)override;
	Handle<Storage::DirectoryIterator> First()override;
	Handle<Object> Get(Handle<String> Path)override;

private:
	// Common
	Clusters::shared_map<Handle<String>, Handle<Object>> cMap;
};


//==========
// Iterator
//==========

class DirectoryIterator: public Storage::DirectoryIterator
{
public:
	// Con-/Destructors
	DirectoryIterator(Handle<Directory> Directory);

	// Common
	BOOL Find(Handle<String> Name) { return cIt.find(Name); }
	BOOL First() { return cIt.set_position(0); }
	Handle<Object> GetCurrent()const override { return cIt->get_value(); }
	UINT GetPosition()const { return cIt.get_position(); }
	BOOL HasCurrent()const override { return cIt.has_current(); }
	BOOL MoveNext()override { return cIt.move_next(); }
	BOOL MovePrevious() { return cIt.move_previous(); }
	BOOL RemoveCurrent() { return cIt.remove_current(); }
	VOID SetPosition(UINT Position) { cIt.set_position(Position); }

private:
	// Common
	typename Clusters::shared_map<Handle<String>, Handle<Object>>::iterator cIt;
	Handle<Directory> hDirectory;
};

}}