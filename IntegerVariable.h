//===================
// IntegerVariable.h
//===================

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//==================
// Integer-Variable
//==================

class IntegerVariable: public Variable
{
public:
	// Con-/Destructors
	static Handle<IntegerVariable> Create(INT Value=0);
	static Handle<IntegerVariable> Create(Handle<String> Name, INT Value=0);

	// Access
	INT Get();
	static inline INT Get(IntegerVariable* Value) { return Value? Value->Get(): 0; }
	inline Handle<String> GetName()const override { return m_Name; }
	Event<Variable, INT&> Reading;
	inline Handle<String> ToString(LanguageCode Language)override { return String::Create("%i", Get()); }
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	BOOL FromString(Handle<String> String, BOOL Notify=true)override;
	static BOOL FromString(Handle<String> String, INT* Value);
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override;
	BOOL Set(INT Value, BOOL Notify=true);

private:
	// Con-/Destructors
	IntegerVariable(Handle<String> Name, INT Value): m_Name(Name), m_Value(Value) {}

	// Common
	Handle<String> m_Name;
	INT m_Value;
};


//========
// Handle
//========

template <>
class Handle<IntegerVariable>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	inline Handle(): m_Object(nullptr) {}
	inline Handle(nullptr_t): m_Object(nullptr) {}
	inline Handle(IntegerVariable* Copy) { Handle<Object>::Create(&m_Object, Copy); }
	inline Handle(Handle const& Copy): Handle(Copy.m_Object) {}
	inline Handle(Handle&& Move)noexcept: m_Object(Move.m_Object) { Move.m_Object=nullptr; }
	inline ~Handle() { Handle<Object>::Clear(&m_Object); }

	// Access
	inline operator BOOL()const { return IntegerVariable::Get(m_Object)!=0; }
	inline operator IntegerVariable*()const { return m_Object; }
	inline IntegerVariable* operator->()const { return m_Object; }

	// Comparison
	inline BOOL operator==(nullptr_t)const { return IntegerVariable::Get(m_Object)==0; }
	inline BOOL operator==(IntegerVariable* Value)const { return IntegerVariable::Get(m_Object)==IntegerVariable::Get(Value); }
	inline BOOL operator==(INT Value)const { return IntegerVariable::Get(m_Object)==Value; }
	inline BOOL operator!=(nullptr_t)const { return IntegerVariable::Get(m_Object)!=0; }
	inline BOOL operator!=(IntegerVariable* Value)const { return IntegerVariable::Get(m_Object)!=IntegerVariable::Get(Value); }
	inline BOOL operator!=(INT Value)const { return IntegerVariable::Get(m_Object)!=Value; }
	inline BOOL operator>(nullptr_t)const { return IntegerVariable::Get(m_Object)>0; }
	inline BOOL operator>(IntegerVariable* Value)const { return IntegerVariable::Get(m_Object)>IntegerVariable::Get(Value); }
	inline BOOL operator>(INT Value)const { return IntegerVariable::Get(m_Object)>Value; }
	inline BOOL operator>=(nullptr_t)const { return IntegerVariable::Get(m_Object)>=0; }
	inline BOOL operator>=(IntegerVariable* Value)const { return IntegerVariable::Get(m_Object)>=IntegerVariable::Get(Value); }
	inline BOOL operator>=(INT Value)const { return IntegerVariable::Get(m_Object)>=Value; }
	inline BOOL operator<(nullptr_t)const { return IntegerVariable::Get(m_Object)<0; }
	inline BOOL operator<(IntegerVariable* Value)const { return IntegerVariable::Get(m_Object)<IntegerVariable::Get(Value); }
	inline BOOL operator<(INT Value)const { return IntegerVariable::Get(m_Object)<Value; }
	inline BOOL operator<=(nullptr_t)const { return IntegerVariable::Get(m_Object)<=0; }
	inline BOOL operator<=(IntegerVariable* Value)const { return IntegerVariable::Get(m_Object)<=IntegerVariable::Get(Value); }
	inline BOOL operator<=(INT Value)const { return IntegerVariable::Get(m_Object)<=Value; }

	// Assignment
	inline Handle& operator=(nullptr_t) { Handle<Object>::Clear(&m_Object); return *this; }
	inline Handle& operator=(IntegerVariable* Copy) { Handle<Object>::Set(&m_Object, Copy); return *this; }
	inline Handle& operator=(Handle const& Copy) { return operator=(Copy.m_Object); }
	Handle& operator=(INT Value)
		{
		if(!m_Object)
			{
			auto value=IntegerVariable::Create(Value);
			return operator=(value);
			}
		m_Object->Set(Value);
		return *this;
		}

private:
	// Common
	IntegerVariable* m_Object;
};


//==================
// Con-/Destructors
//==================

inline Handle<IntegerVariable> IntegerVariable::Create(INT Value)
{
return new IntegerVariable(nullptr, Value);
}

inline Handle<IntegerVariable> IntegerVariable::Create(Handle<String> Name, INT Value)
{
return new IntegerVariable(Name, Value);
}
