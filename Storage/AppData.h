//===========
// AppData.h
//===========

#pragma once


//=======
// Using
//=======

#include "Storage/Virtual/Directory.h"
#include "Directory.h"
#include "Workspace.h"


//===========
// Namespace
//===========

namespace Storage {


//==========
// App-Data
//==========

class AppData: public Workspace
{
public:
	// Con-/Destructors
	AppData();

	// Common
	BOOL Add(Handle<String> Name, Handle<Convertible> Object);
	Handle<File> CreateFile(Handle<String> Path, FileCreateMode Create=FileCreateMode::OpenExisting, FileAccessMode Access=FileAccessMode::ReadOnly, FileShareMode Share=FileShareMode::ShareRead);

private:
	// Common
	Handle<Storage::Virtual::Directory> hVirtual;
};

}