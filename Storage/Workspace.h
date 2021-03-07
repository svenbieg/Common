//=============
// Workspace.h
//=============

#pragma once


//=======
// Using
//=======

#include "Collections/Array.h"
#include "Directory.h"


//===========
// Namespace
//===========

namespace Storage {


//===========
// Workspace
//===========

class Workspace: public Object
{
private:
	// Using
	using DirectoryList=::Collections::Array<Handle<Directory>>;

public:
	// Con-/Destructors
	Workspace();

	// Common
	Handle<DirectoryList> Directories;
	Handle<Convertible> Get(Handle<String> Path);
};

}