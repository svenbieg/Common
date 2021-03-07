//==========
// Stream.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/File.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Streams {


//======================
// Forward-Declarations
//======================

class Database;


//========
// Stream
//========

class Stream: public Object
{
public:
	// Common
	VOID Close(FileShareMode Share);
	BOOL Open(FileAccessMode Access, FileShareMode Share);

protected:
	// Con-/Destructors
	Stream(Handle<Database> Database, UINT64 Offset);

private:
	// Common
	BOOL OpenInternal();
	CriticalSection cCriticalSection;
	Handle<Database> hDatabase;
	INT iShareRead;
	INT iShareWrite;
	UINT64 uOffset;
};

}}}