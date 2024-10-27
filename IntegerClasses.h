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
	Handle(): m_Object(nullptr) {}
	Handle(decltype(nullptr)): m_Object(nullptr) {}
	Handle(Int32* Object) { HandleCreate(&m_Object, Object); }
	Handle(Handle const& Handle) { HandleCreate(&m_Object, Handle.m_Object); }
	Handle(Handle&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	~Handle() { HandleClear(&m_Object); }

	// Access
	operator INT()const { return VariableGet(m_Object, 0); }
	Int32* operator->()const { return m_Object; }

	// Comparison
	bool operator==(INT Value) { return VariableEqual(m_Object, Value); }
	bool operator!=(INT Value) { return !VariableEqual(m_Object, Value); }
	bool operator>(INT Value) { return VariableAbove(m_Object, Value); }
	bool operator>=(INT Value) { return VariableAboveOrEqual(m_Object, Value); }
	bool operator<(INT Value) { return VariableBelow(m_Object, Value); }
	bool operator<=(INT Value) { return VariableBelowOrEqual(m_Object, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&m_Object); return *this; }
	Handle& operator=(INT Value) { VariableAssign(&m_Object, Value); return *this; }
	Handle& operator=(Int32* Object) { HandleAssign(&m_Object, Object); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&m_Object, Handle.m_Object); return *this; }

private:
	// Common
	Int32* m_Object;
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
	Handle(): m_Object(nullptr) {}
	Handle(decltype(nullptr)): m_Object(nullptr) {}
	Handle(Int64* Object) { HandleCreate(&m_Object, Object); }
	Handle(Handle const& Handle) { HandleCreate(&m_Object, Handle.m_Object); }
	Handle(Handle&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	~Handle() { HandleClear(&m_Object); }

	// Access
	operator INT64()const { return VariableGet<Int64, INT64>(m_Object, 0); }
	Int64* operator->()const { return m_Object; }

	// Comparison
	bool operator==(INT64 Value) { return VariableEqual(m_Object, Value); }
	bool operator!=(INT64 Value) { return !VariableEqual(m_Object, Value); }
	bool operator>(INT64 Value) { return VariableAbove(m_Object, Value); }
	bool operator>=(INT64 Value) { return VariableAboveOrEqual(m_Object, Value); }
	bool operator<(INT64 Value) { return VariableBelow(m_Object, Value); }
	bool operator<=(INT64 Value) { return VariableBelowOrEqual(m_Object, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&m_Object); return *this; }
	Handle& operator=(INT64 Value) { VariableAssign(&m_Object, Value); return *this; }
	Handle& operator=(Int64* Object) { HandleAssign(&m_Object, Object); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&m_Object, Handle.m_Object); return *this; }

private:
	// Common
	Int64* m_Object;
};
