//============
// Variable.h
//============

#pragma once


//=======
// Using
//=======

#include "Collections/Map.h"
#include "Culture/Language.h"
#include "Storage/Streams/InputStream.h"
#include "Storage/Streams/OutputStream.h"
#include "StringClass.h"
#include "VariableHelper.h"


//======================
// Forward-Declarations
//======================

class Variable;

typedef Collections::Map<Handle<String>, Variable*> VariableList;


//=========
// Globals
//=========

extern Handle<VariableList> Globals;


//==========
// Variable
//==========

class Variable: public Object
{
public:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using LanguageCode=Culture::LanguageCode;
	using Mutex=Concurrency::Mutex;
	using OutputStream=Storage::Streams::OutputStream;
	using ScopedLock=Concurrency::ScopedLock;
	using SharedLock=Concurrency::SharedLock;

	// Common
	Event<Variable> Changed;
	virtual BOOL FromString(Handle<String> Value, BOOL Notify=true) { return false; }
	Handle<String> const Name;
	virtual SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true) { return 0; }
	virtual Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage) { return nullptr; }
	virtual SIZE_T WriteToStream(OutputStream* Stream) { return 0; }

protected:
	// Con-/Destructors
	Variable(Handle<String> Name);
	~Variable();

	// Common
	Mutex m_Mutex;
};


//================
// Typed Variable
//================

template <class _value_t>
class TypedVariable: public Variable
{
public:
	// Common
	_value_t Get()
		{
		SharedLock lock(m_Mutex);
		_value_t value=m_Value;
		lock.Unlock();
		Reading(this, value);
		return value;
		}
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override
		{
		if(!Stream)
			return sizeof(_value_t);
		ScopedLock lock(m_Mutex);
		_value_t value;
		SIZE_T size=Stream->Read(&value, sizeof(_value_t));
		if(size==sizeof(_value_t))
			Set(value, Notify);
		return size;
		}
	Event<Variable, _value_t&> Reading;
	virtual VOID Set(_value_t const& Value, BOOL Notify=true)
		{
		ScopedLock lock(m_Mutex);
		if(m_Value==Value)
			return;
		m_Value=Value;
		lock.Unlock();
		if(Notify)
			Changed(this);
		}
	SIZE_T WriteToStream(OutputStream* Stream)override
		{
		if(!Stream)
			return sizeof(_value_t);
		SharedLock lock(m_Mutex);
		_value_t value=m_Value;
		lock.Unlock();
		Reading(this, value);
		return Stream->Write(&value, sizeof(_value_t));
		}

protected:
	// Con-/Destructors
	TypedVariable(Handle<String> Name, _value_t Value): Variable(Name), m_Value(Value) {}
	
	// Common
	_value_t m_Value;
};
