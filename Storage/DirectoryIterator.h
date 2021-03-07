//=====================
// DirectoryIterator.h
//=====================

#pragma once


//=======
// Using
//=======

#include "Convertible.h"


//===========
// Namespace
//===========

namespace Storage {


//======================
// Forward-Declarations
//======================

class Directory;


//====================
// Directory-Iterator
//====================

class DirectoryIterator: public Object
{
public:
	// Con-/Destructors
	DirectoryIterator(Handle<Directory> Directory);
	~DirectoryIterator();

	// Access
	Handle<Convertible> GetCurrent()const;
	BOOL HasCurrent()const;

	// Modification
	BOOL MoveNext()const;

private:
	// Common
	Handle<Directory> hDirectory;
};

}