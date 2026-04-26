//=================
// FloatVariable.h
//=================

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//================
// Float-Variable
//================

class FloatVariable: public Variable
{
public:
	// Con-/Destructors
	static Handle<FloatVariable> Create(FLOAT Value=0.f);
	static Handle<FloatVariable> Create(Handle<String> Name, FLOAT Value=0.f);

	// Access
	FLOAT Get();
	static inline FLOAT Get(FloatVariable* Value) { return Value? Value->Get(): 0.f; }
	inline Handle<String> GetName()const override { return m_Name; }
	Event<FloatVariable, FLOAT&> Reading;
	inline Handle<String> ToString(LanguageCode Language)override { return String::Create("%.2f", Get()); }
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	BOOL FromString(Handle<String> Value, BOOL Notify=true)override;
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override;
	BOOL Set(FLOAT Value, BOOL Notify=true);

private:
	// Con-/Destructors
	FloatVariable(Handle<String> Name, FLOAT Value): m_Name(Name), m_Value(Value) {}

	// Common
	Handle<String> m_Name;
	FLOAT m_Value;
};


//========
// Handle
//========

template <>
class Handle<FloatVariable>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	inline Handle(): m_Object(nullptr) {}
	inline Handle(nullptr_t): m_Object(nullptr) {}
	inline Handle(FloatVariable* Copy) { Handle<Object>::Create(&m_Object, Copy); }
	inline Handle(Handle const& Copy): Handle(Copy.m_Object) {}
	inline Handle(Handle&& Move)noexcept: m_Object(Move.m_Object) { Move.m_Object=nullptr; }
	inline ~Handle() { Handle<Object>::Clear(&m_Object); }

	// Access
	inline operator BOOL()const { return FloatVariable::Get(m_Object)!=0.f; }
	inline operator FloatVariable*()const { return m_Object; }
	inline FloatVariable* operator->()const { return m_Object; }

	// Comparison
	inline BOOL operator==(nullptr_t)const { return FloatVariable::Get(m_Object)==0.f; }
	inline BOOL operator==(FloatVariable* Value)const { return FloatVariable::Get(m_Object)==FloatVariable::Get(Value); }
	inline BOOL operator==(FLOAT Value)const { return FloatVariable::Get(m_Object)==Value; }
	inline BOOL operator!=(nullptr_t)const { return FloatVariable::Get(m_Object)!=0.f; }
	inline BOOL operator!=(FloatVariable* Value)const { return FloatVariable::Get(m_Object)!=FloatVariable::Get(Value); }
	inline BOOL operator!=(FLOAT Value)const { return FloatVariable::Get(m_Object)!=Value; }
	inline BOOL operator>(nullptr_t)const { return FloatVariable::Get(m_Object)>0.f; }
	inline BOOL operator>(FloatVariable* Value)const { return FloatVariable::Get(m_Object)>FloatVariable::Get(Value); }
	inline BOOL operator>(FLOAT Value)const { return FloatVariable::Get(m_Object)>Value; }
	inline BOOL operator>=(nullptr_t)const { return FloatVariable::Get(m_Object)>=0.f; }
	inline BOOL operator>=(FloatVariable* Value)const { return FloatVariable::Get(m_Object)>=FloatVariable::Get(Value); }
	inline BOOL operator>=(FLOAT Value)const { return FloatVariable::Get(m_Object)>=Value; }
	inline BOOL operator<(nullptr_t)const { return FloatVariable::Get(m_Object)<0.f; }
	inline BOOL operator<(FloatVariable* Value)const { return FloatVariable::Get(m_Object)<FloatVariable::Get(Value); }
	inline BOOL operator<(FLOAT Value)const { return FloatVariable::Get(m_Object)<Value; }
	inline BOOL operator<=(nullptr_t)const { return FloatVariable::Get(m_Object)<=0.f; }
	inline BOOL operator<=(FloatVariable* Value)const { return FloatVariable::Get(m_Object)<=FloatVariable::Get(Value); }
	inline BOOL operator<=(FLOAT Value)const { return FloatVariable::Get(m_Object)<=Value; }

	// Assignment
	inline Handle& operator=(nullptr_t) { Handle<Object>::Clear(&m_Object); return *this; }
	inline Handle& operator=(FloatVariable* Copy) { Handle<Object>::Set(&m_Object, Copy); return *this; }
	inline Handle& operator=(Handle const& Copy) { return operator=(Copy.m_Object); }
	Handle& operator=(FLOAT Value)
		{
		if(!m_Object)
			{
			auto value=FloatVariable::Create(Value);
			return operator=(value);
			}
		m_Object->Set(Value);
		return *this;
		}

private:
	// Common
	FloatVariable* m_Object;
};


//==================
// Con-/Destructors
//==================

inline Handle<FloatVariable> FloatVariable::Create(FLOAT Value)
{
return new FloatVariable(nullptr, Value);
}

inline Handle<FloatVariable> FloatVariable::Create(Handle<String> Name, FLOAT Value)
{
return new FloatVariable(Name, Value);
}
