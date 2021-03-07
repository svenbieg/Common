//=========
// Entry.h
//=========

#pragma once


//=======
// Using
//=======

#include "Storage/File.h"
#include "Convertible.h"
#include "EntryType.h"


//======================
// Forward-Declarations
//======================

namespace Storage
{
namespace Database
	{
	class Database;
	}
}


//===========
// Namespace
//===========

namespace Storage {
	namespace Database {
		namespace Entries {


//=======
// Entry
//=======

class Entry: public Convertible
{
protected:
	// Con-/Destructors
	Entry(Handle<String> Type, Handle<Database> Database, UINT64 Offset);

	// Common
	VOID Create();
	VOID Destroy();
	CriticalSection cCriticalSection;
	Handle<Database> hDatabase;
	UINT64 uOffset;

private:
	// Common
	UINT uAccessCount;
};

}}}