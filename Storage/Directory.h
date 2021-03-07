//=============
// Directory.h
//=============

#pragma once


//=======
// Using
//=======

#include "DirectoryIterator.h"
#include "File.h"


//===========
// Namespace
//===========

namespace Storage {


//===========
// Directory
//===========

class Directory: public Convertible
{
public:
	// Friends
	friend DirectoryIterator;

	// Types
	typedef DirectoryIterator Iterator;

	// Common
	virtual VOID Close()=0;
	virtual Handle<File> CreateFile(Handle<String> Path, FileCreateMode CreateMode, FileAccessMode AccessMode, FileShareMode ShareMode)=0;
	Handle<Iterator> First();
	virtual Handle<Convertible> Get(Handle<String> Path)=0;
	Handle<String> GetName();
	Handle<String> GetPath();
	Handle<Directory> GetRoot();
	Handle<Directory> Parent;

protected:
	// Con-/Destructors
	Directory(Handle<String> Name, Handle<String> Type, Handle<Directory> Parent=nullptr);

	// Common
	virtual VOID FindClose()=0;
	virtual Handle<Convertible> FindCurrent()=0;
	virtual Handle<Convertible> FindFirst()=0;
	virtual Handle<Convertible> FindNext()=0;
	CriticalSection cCriticalSection;
	Handle<String> hPath;

private:
	// Common
	Handle<String> GetPathInternal(Handle<Directory> Parent, Handle<String> Name);
};

}