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
	Handle(): pObject(nullptr) {}
	Handle(decltype(nullptr)): pObject(nullptr) {}
	Handle(Float* Object) { HandleCreate(&pObject, Object); }
	Handle(Handle const& Handle) { HandleCreate(&pObject, Handle.pObject); }
	Handle(Handle&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleClear(&pObject); }

	// Access
	operator FLOAT()const { return VariableGet<Float, FLOAT>(pObject, 0.f); }
	Float* operator->()const { return pObject; }

	// Comparison
	bool operator==(FLOAT Value) { return VariableEqual(pObject, Value); }
	bool operator!=(FLOAT Value) { return !VariableEqual(pObject, Value); }
	bool operator>(FLOAT Value) { return VariableAbove(pObject, Value); }
	bool operator>=(FLOAT Value) { return VariableAboveOrEqual(pObject, Value); }
	bool operator<(FLOAT Value) { return VariableBelow(pObject, Value); }
	bool operator<=(FLOAT Value) { return VariableBelowOrEqual(pObject, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&pObject); return *this; }
	Handle& operator=(FLOAT Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(Float* Object) { HandleAssign(&pObject, Object); return *this; }
	Handle& operator=(Handle<Float> const& Handle) { HandleAssign(&pObject, Handle.pObject); return *this; }

private:
	// Common
	Float* pObject;
};
