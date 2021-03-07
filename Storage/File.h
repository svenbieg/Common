//========
// File.h
//========

#pragma once


//=======
// Using
//=======

#include "Storage/Container.h"
#include "Convertible.h"
#include "FileHelper.h"


//===========
// Namespace
//===========

namespace Storage {


//======
// File
//======

class File: public Convertible, public Container
{
public:
	// Common
	virtual VOID Close()=0;
	virtual BOOL Create(FileCreateMode Create=FileCreateMode::OpenExisting, FileAccessMode Access=FileAccessMode::ReadOnly, FileShareMode Share=FileShareMode::ShareRead)=0;
	Handle<String> GetName();
	Handle<String> GetPath();
	virtual BOOL SetSize(UINT64 Size)=0;

protected:
	// Con-/Destructors
	File(Handle<String> Path, Handle<String> Type);

	// Common
	CriticalSection cCriticalSection;
	Handle<String> hPath;
};

}