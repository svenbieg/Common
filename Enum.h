//========
// Enum.h
//========

#pragma once


//=======
// Using
//=======

#include "Clusters/index.hpp"
#include "Sentence.h"
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
	Clusters::index<Handle<Sentence>> cEnum;
	Handle<Sentence> hValue;
};


//=============
// Enum-Handle
//=============

template <>
class Handle<Enum>
{
public:
	// Using
	using STRING=Resources::Strings::STRING;

	// Friends
	template <class _friend_t> friend class Handle;
	template <class _owner_t, class _obj_t> friend class DynamicHandle;

	// Con-/Destructors
	Handle(): m_Object(nullptr) {}
	Handle(decltype(nullptr)): m_Object(nullptr) {}
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
	// Con-/Destructors
	EnumIterator(Handle<Enum> Enum);
	~EnumIterator();

	// Access
	Handle<Sentence> GetCurrent()const { return cIt.get_current(); }
	UINT GetPosition()const { return cIt.get_position(); }
	BOOL HasCurrent()const { return cIt.has_current(); }

	// Navigation
	BOOL First() { return cIt.begin(); }
	BOOL Last() { return cIt.rbegin(); }
	BOOL MoveNext() { return cIt.move_next(); }
	BOOL MovePrevious() { return cIt.move_previous(); }
	BOOL SetPosition(UINT Position) { return cIt.set_position(Position); }

private:
	// Common
	typename Clusters::index<Handle<Sentence>>::iterator cIt;
	Handle<Enum> hEnum;
};
