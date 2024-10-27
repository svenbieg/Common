//==============
// FloatClass.h
//==============

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//=======
// Float
//=======

class Float: public TypedVariable<FLOAT>
{
public:
	// Con-/Destructors
	Float(FLOAT Value=0.f): TypedVariable(nullptr, Value) {}
	Float(Handle<String> Name, FLOAT Value=0.f): TypedVariable(Name, Value) {}

	// Access
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override { return new String("%.2f", Get()); }

	// Modification
	BOOL FromString(Handle<String> Value, BOOL Notify=true)override { return VariableFromString<Float, FLOAT>(this, Value, "%f", Notify); }
};


//==============
// Float-Handle
//==============

template <>
class Handle<Float>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): m_Object(nullptr) {}
	Handle(decltype(nullptr)): m_Object(nullptr) {}
	Handle(Float* Object) { HandleCreate(&m_Object, Object); }
	Handle(Handle const& Handle) { HandleCreate(&m_Object, Handle.m_Object); }
	Handle(Handle&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	~Handle() { HandleClear(&m_Object); }

	// Access
	operator FLOAT()const { return VariableGet<Float, FLOAT>(m_Object, 0.f); }
	Float* operator->()const { return m_Object; }

	// Comparison
	bool operator==(FLOAT Value) { return VariableEqual(m_Object, Value); }
	bool operator!=(FLOAT Value) { return !VariableEqual(m_Object, Value); }
	bool operator>(FLOAT Value) { return VariableAbove(m_Object, Value); }
	bool operator>=(FLOAT Value) { return VariableAboveOrEqual(m_Object, Value); }
	bool operator<(FLOAT Value) { return VariableBelow(m_Object, Value); }
	bool operator<=(FLOAT Value) { return VariableBelowOrEqual(m_Object, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&m_Object); return *this; }
	Handle& operator=(FLOAT Value) { VariableAssign(&m_Object, Value); return *this; }
	Handle& operator=(Float* Object) { HandleAssign(&m_Object, Object); return *this; }
	Handle& operator=(Handle<Float> const& Handle) { HandleAssign(&m_Object, Handle.m_Object); return *this; }

private:
	// Common
	Float* m_Object;
};
