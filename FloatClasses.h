//================
// FloatClasses.h
//================

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
	Handle<String> ToString()override { return new String("%.2f", Get()); }

	// Modification
	BOOL FromString(Handle<String> Value) { return VariableFromString<Float, FLOAT>(this, Value, "%f"); }
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
	~Handle() { HandleDestroy(pObject); }

	// Access
	operator FLOAT()const { return VariableGet<Float, FLOAT>(pObject, 0.f); }
	Float* operator->()const { return pObject; }

	// Comparison
	BOOL operator==(FLOAT Value) { return VariableEqual(pObject, Value); }
	BOOL operator!=(FLOAT Value) { return !VariableEqual(pObject, Value); }
	BOOL operator>(FLOAT Value) { return VariableGreater(pObject, Value); }
	BOOL operator>=(FLOAT Value) { return VariableGreaterOrEqual(pObject, Value); }
	BOOL operator<(FLOAT Value) { return VariableLess(pObject, Value); }
	BOOL operator<=(FLOAT Value) { return VariableLessOrEqual(pObject, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&pObject); return *this; }
	Handle& operator=(FLOAT Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(Float* Object) { HandleAssign(&pObject, Object); return *this; }
	Handle& operator=(Handle<Float> const& Handle) { HandleAssign(&pObject, Handle.pObject); return *this; }

private:
	// Common
	Float* pObject;
};
