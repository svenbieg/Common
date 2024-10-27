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
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override { return new String("%i", (INT)Get()); }

	// Modification
	BOOL FromString(Handle<String> String, BOOL Notify=true)override;
	static BOOL FromString(Handle<String> String, BOOL* Value);
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
	Handle(): m_Object(nullptr) {}
	Handle(Bool* Object) { HandleCreate(&m_Object, Object); }
	Handle(Handle<Bool>&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	~Handle() { HandleClear(&m_Object); }

	// Access
	operator bool()const { return VariableGet(m_Object, false); }
	Bool* operator->()const { return m_Object; }

	// Comparison
	bool operator==(BOOL Value) { return VariableEqual(m_Object, Value); }
	bool operator!=(BOOL Value) { return !VariableEqual(m_Object, Value); }

	// Assignment
	Handle& operator=(BOOL Value) { VariableAssign(&m_Object, Value); return *this; }
	Handle& operator=(Bool* Object) { HandleAssign(&m_Object, Object); return *this; }

private:
	// Common
	Bool* m_Object;
};
