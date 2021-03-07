//=============
// Directory.h
//=============

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/OutputStream.h"
#include "Storage/Directory.h"


//===========
// Namespace
//===========

namespace Web {


//===========
// Directory
//===========

class Directory: public Object
{
private:
	// Using
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	Directory(Handle<Storage::Directory> Directory);

	// Common
	SIZE_T WriteToStream(Handle<OutputStream> Stream);

private:
	// Common
	Handle<Storage::Directory> hDirectory;
};

}