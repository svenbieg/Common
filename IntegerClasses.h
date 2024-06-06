//==================
// IntegerClasses.h
//==================

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//=======
// Int32
//=======

class Int32: public TypedVariable<INT>
{
public:
	// Con-/Destructors
	Int32(INT Value=0): TypedVariable(nullptr, Value) {}
	Int32(Handle<String> Name, INT Value=0): TypedVariable(Name, Value) {}

	// Access
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override { return new String("%i", Get()); }

	// Modification
	BOOL FromString(Handle<String> Value, BOOL Notify=true)override { return VariableFromString<Int32, INT>(this, Value, "%i", Notify); }
};


//==============
// Int32-Handle
//==============

template <>
class Handle<Int32>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(decltype(nullptr)): pObject(nullptr) {}
	Handle(Int32* Object) { HandleCreate(&pObject, Object); }
	Handle(Handle const& Handle) { HandleCreate(&pObject, Handle.pObject); }
	Handle(Handle&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleClear(&pObject); }

	// Access
	operator INT()const { return VariableGet(pObject, 0); }
	Int32* operator->()const { return pObject; }

	// Comparison
	bool operator==(INT Value) { return VariableEqual(pObject, Value); }
	bool operator!=(INT Value) { return !VariableEqual(pObject, Value); }
	bool operator>(INT Value) { return VariableAbove(pObject, Value); }
	bool operator>=(INT Value) { return VariableAboveOrEqual(pObject, Value); }
	bool operator<(INT Value) { return VariableBelow(pObject, Value); }
	bool operator<=(INT Value) { return VariableBelowOrEqual(pObject, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&pObject); return *this; }
	Handle& operator=(INT Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(Int32* Object) { HandleAssign(&pObject, Object); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&pObject, Handle.pObject); return *this; }

private:
	// Common
	Int32* pObject;
};


//=======
// Int64
//=======

class Int64: public TypedVariable<INT64>
{
public:
	// Con-/Destructors
	Int64(INT64 Value=0): TypedVariable(nullptr, Value) {}
	Int64(Handle<String> Name, INT64 Value=0): TypedVariable(Name, Value) {}

	// Access
	Handle<String> ToString()override { return new String("%i", Get()); }

	// Modification
	BOOL FromString(Handle<String> Value, BOOL Notify=true)override { return VariableFromString<Int64, INT64>(this, Value, "%i", Notify); }
};


//==============
// Int64-Handle
//==============

template <>
class Handle<Int64>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(decltype(nullptr)): pObject(nullptr) {}
	Handle(Int64* Object) { HandleCreate(&pObject, Object); }
	Handle(Handle const& Handle) { HandleCreate(&pObject, Handle.pObject); }
	Handle(Handle&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleClear(&pObject); }

	// Access
	operator INT64()const { return VariableGet<Int64, INT64>(pObject, 0); }
	Int64* operator->()const { return pObject; }

	// Comparison
	bool operator==(INT64 Value) { return VariableEqual(pObject, Value); }
	bool operator!=(INT64 Value) { return !VariableEqual(pObject, Value); }
	bool operator>(INT64 Value) { return VariableAbove(pObject, Value); }
	bool operator>=(INT64 Value) { return VariableAboveOrEqual(pObject, Value); }
	bool operator<(INT64 Value) { return VariableBelow(pObject, Value); }
	bool operator<=(INT64 Value) { return VariableBelowOrEqual(pObject, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&pObject); return *this; }
	Handle& operator=(INT64 Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(Int64* Object) { HandleAssign(&pObject, Object); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&pObject, Handle.pObject); return *this; }

private:
	// Common
	Int64* pObject;
};
