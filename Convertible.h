//===============
// Convertible.h
//===============

#pragma once


//=======
// Using
//=======

#include "StringClass.h"


//=============
// Convertible
//=============

class Convertible: public virtual Object
{
public:
	// Common
	template <class _convert_t> Handle<_convert_t> As(Handle<String> Type)
		{
		if(!HasType(Type))
			return nullptr;
		return (_convert_t*)this;
		}
	Handle<String> GetType()const { return hType; }
	BOOL HasType(Handle<String> Type);

protected:
	// Con-/Destructors
	Convertible(Handle<String> Type);

	// Common
	Handle<String> hType;
};
