//========
// Enum.h
//========

#pragma once


//=======
// Using
//=======

#include "Collections/index.hpp"
#include "Culture/Sentence.h"
#include "Variable.h"


//======================
// Forward-Declarations
//======================

class EnumIterator;


//======
// Enum
//======

class Enum: public Variable
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Friends
	friend EnumIterator;

	// Con-/Destructors
	Enum(Handle<String> Name);

	// Access
	Handle<EnumIterator> First();
	Handle<Sentence> Get();
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override;
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	VOID Add(Handle<Sentence> Value);
	BOOL FromString(Handle<String> Value, BOOL Notify=true)override;
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override;
	BOOL Set(Handle<Sentence> Value, BOOL Notify=true);

private:
	// Common
	Handle<Sentence> m_Value;
	Collections::index<Handle<Sentence>> m_Values;
};


//=============
// Enum-Handle
//=============

template <>
class Handle<Enum>
{
public:
	// Using
	using Sentence=Culture::Sentence;
	using STRING=Resources::Strings::STRING;

	// Friends
	template <class _friend_t> friend class Handle;
	template <class _owner_t, class _obj_t> friend class DynamicHandle;

	// Con-/Destructors
	Handle(): m_Object(nullptr) {}
	Handle(nullptr_t): m_Object(nullptr) {}
	Handle(Enum* Pointer) { HandleCreate<Enum, Enum>(&m_Object, Pointer); }
	Handle(Handle<Enum> const& Handle) { HandleCreate<Enum, Enum>(&m_Object, Handle.m_Object); }
	Handle(Handle<Enum>&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	~Handle() { HandleClear(&m_Object); }

	// Access
	inline operator Handle<Sentence>()const { return m_Object? m_Object->Get(): nullptr; }
	inline Enum* operator->()const { return m_Object; }

	// Comparison
	inline bool operator==(STRING const* Value)const
		{
		Handle<Sentence> value=m_Object? m_Object->Get(): nullptr;
		if(!value)
			{
			if(Value)
				return false;
			return true;
			}
		return value->Compare(Value)==0;
		}
	inline BOOL operator!=(STRING const* Value)const { return !operator==(Value); }

	// Assignment
	inline Handle& operator=(Enum* Value) { HandleAssign(&m_Object, Value); return *this; }
	inline Handle& operator=(STRING const* Value)
		{
		if(m_Object)
			m_Object->Set(Value);
		return *this;
		}

private:
	Enum* m_Object;
};


//==========
// Iterator
//==========

class EnumIterator: public Object
{
public:
	// Using
	using Sentence=Culture::Sentence;

	// Con-/Destructors
	EnumIterator(Handle<Enum> Enum);
	~EnumIterator();

	// Access
	Handle<Sentence> GetCurrent()const { return m_It.get_current(); }
	UINT GetPosition()const { return m_It.get_position(); }
	BOOL HasCurrent()const { return m_It.has_current(); }

	// Navigation
	BOOL First() { return m_It.begin(); }
	BOOL Last() { return m_It.rbegin(); }
	BOOL MoveNext() { return m_It.move_next(); }
	BOOL MovePrevious() { return m_It.move_previous(); }
	BOOL SetPosition(UINT Position) { return m_It.set_position(Position); }

private:
	// Common
	Handle<Enum> m_Enum;
	typename Collections::index<Handle<Sentence>>::iterator m_It;
};
