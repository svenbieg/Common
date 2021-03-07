//===============
// Temperature.h
//===============

#pragma once


//=======
// Using
//=======

#include "HandleHelper.h"
#include "Variable.h"


//===========
// Namespace
//===========

namespace Physics {


//=============
// Temperature
//=============

class Temperature: public TypedVariable<FLOAT>
{
public:
	// Con-/Destructors
	Temperature(FLOAT DegreesCelsius=0.f);
	Temperature(Handle<String> Name, FLOAT DegreesCelsius=0.f);

	// Common
	Handle<String> ToString()override;
	static Handle<String> Unit;
};

}


//====================
// Temperature-Handle
//====================

template<>
class Handle<Physics::Temperature>
{
private:
	// Using
	using _temp_t=Physics::Temperature;

public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(_temp_t* Object) { HandleCreate(&pObject, Object); }
	Handle(Handle<_temp_t>&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleDestroy(pObject); }

	// Access
	operator FLOAT() { return VariableGet(pObject, -300.f); }
	_temp_t* operator->()const { return pObject; }

	// Comparison
	BOOL operator==(FLOAT Value)const { return VariableEqual(pObject, Value, -300.f); }

	// Assignment
	Handle& operator=(FLOAT Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(_temp_t* Object) { HandleAssign(&pObject, Object); return *this; }
	Handle& operator=(Handle<_temp_t> const& Handle) { HandleAssign(&pObject, Handle.pObject); return *this; }

private:
	// Common
	_temp_t* pObject;
};
