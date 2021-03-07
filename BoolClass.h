//=============
// BoolClass.h
//=============

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//======
// Bool
//======

class Bool: public TypedVariable<BOOL>
{
public:
	// Con-/Destructors
	Bool(BOOL Value=false);
	Bool(Handle<String> Name, BOOL Value=false);

	// Access
	Handle<String> ToString()override { return new String("%i", (INT)Get()); }

	// Modification
	BOOL FromString(Handle<String> String)override;
	static BOOL FromString(Handle<String> String, BOOL& Value);
};


//=============
// Bool-Handle
//=============

template <>
class Handle<Bool>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(Bool* Object) { HandleCreate(&pObject, Object); }
	Handle(Handle<Bool>&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleDestroy(pObject); }

	// Access
	operator BOOL()const { return VariableGet(pObject, false); }
	Bool* operator->()const { return pObject; }

	// Comparison
	BOOL operator==(BOOL Value) { return VariableEqual(pObject, Value); }
	BOOL operator!=(BOOL Value) { return !VariableEqual(pObject, Value); }

	// Assignment
	Handle& operator=(BOOL Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(Bool* Object) { HandleAssign(&pObject, Object); return *this; }

private:
	// Common
	Bool* pObject;
};
