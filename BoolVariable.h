//================
// BoolVariable.h
//================

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//===============
// Bool-Variable
//===============

class BoolVariable: public Variable
{
public:
	// Con-/Destructors
	static Handle<BoolVariable> Create(BOOL Value=false);
	static Handle<BoolVariable> Create(Handle<String> Name, BOOL Value=false);

	// Access
	BOOL Get();
	static inline BOOL Get(BoolVariable* Value) { return Value? Value->Get(): false; }
	inline Handle<String> GetName()const override { return m_Name; }
	Event<Variable, BOOL&> Reading;
	inline Handle<String> ToString(LanguageCode Language)override { return String::Create("%i", (INT)Get()); }
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	BOOL FromString(Handle<String> String, BOOL Notify=true)override;
	static BOOL FromString(Handle<String> String, BOOL* Value);
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override;
	BOOL Set(BOOL Value, BOOL Notify=true);

private:
	// Con-/Destructors
	BoolVariable(Handle<String> Name, BOOL Value): m_Name(Name), m_Value(Value) {}

	// Common
	Handle<String> m_Name;
	BOOL m_Value;
};


//========
// Handle
//========

template <>
class Handle<BoolVariable>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	inline Handle(): m_Object(nullptr) {}
	inline Handle(nullptr_t): m_Object(nullptr) {}
	inline Handle(BoolVariable* Copy) { Handle<Object>::Create(&m_Object, Copy); }
	inline Handle(Handle const& Copy): Handle(Copy.m_Object) {}
	inline Handle(Handle&& Move)noexcept: m_Object(Move.m_Object) { Move.m_Object=nullptr; }
	inline ~Handle() { Handle<Object>::Clear(&m_Object); }

	// Access
	inline operator BOOL()const { return m_Object&&m_Object->Get(); }
	inline operator BoolVariable*()const { return m_Object; }
	inline BoolVariable* operator->()const { return m_Object; }

	// Comparison
	inline BOOL operator==(nullptr_t)const { return BoolVariable::Get(m_Object)==false; }
	inline BOOL operator==(BoolVariable* Value)const { return BoolVariable::Get(m_Object)==BoolVariable::Get(Value); }
	inline BOOL operator==(BOOL Value)const { return BoolVariable::Get(m_Object)==Value; }
	inline BOOL operator!=(nullptr_t)const { return BoolVariable::Get(m_Object)!=false; }
	inline BOOL operator!=(BoolVariable* Value)const { return BoolVariable::Get(m_Object)!=BoolVariable::Get(Value); }
	inline BOOL operator!=(BOOL Value)const { return BoolVariable::Get(m_Object)!=Value; }

	// Assignment
	inline Handle& operator=(nullptr_t) { Handle<Object>::Clear(&m_Object); return *this; }
	inline Handle& operator=(BoolVariable* Copy) { Handle<Object>::Set(&m_Object, Copy); return *this; }
	inline Handle& operator=(Handle const& Copy) { return operator=(Copy.m_Object); }
	Handle& operator=(BOOL Value)
		{
		if(!m_Object)
			{
			auto value=BoolVariable::Create(Value);
			return operator=(value);
			}
		m_Object->Set(Value);
		return *this;
		}

private:
	// Common
	BoolVariable* m_Object;
};


//==================
// Con-/Destructors
//==================

inline Handle<BoolVariable> BoolVariable::Create(BOOL Value)
{
return new BoolVariable(nullptr, Value);
}

inline Handle<BoolVariable> BoolVariable::Create(Handle<String> Name, BOOL Value)
{
return new BoolVariable(Name, Value);
}
