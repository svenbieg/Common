//=====================
// DirectoryIterator.h
//=====================

#pragma once


//=======
// Using
//=======

#include "Clusters/slist.hpp"
#include "Convertible.h"


//===========
// Namespace
//===========

namespace Storage {
	namespace Virtual {


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
	DirectoryIterator(Handle<Directory> Directory, SIZE_T Position);
	~DirectoryIterator();

	// Access
	Handle<Convertible> GetCurrent()const { return cIt.get_current_item(); }
	SIZE_T GetPosition()const { return cIt.get_position(); }
	BOOL HasCurrent()const { return cIt.has_current(); }

	// Modification
	BOOL Find(Handle<String> Name) { return cIt.find(Name); }
	BOOL First() { return cIt.set_position(0); }
	BOOL Last();
	BOOL MoveNext() { return cIt.move_next(); }
	BOOL MovePrevious() { return cIt.move_previous(); }
	BOOL RemoveCurrent() { return cIt.remove_current(); }
	VOID SetPosition(SIZE_T Position) { cIt.set_position(Position); }

private:
	// Common
	typename Clusters::slist<Handle<String>, Handle<Convertible>>::iterator cIt;
	Handle<Directory> hDirectory;
};

}}