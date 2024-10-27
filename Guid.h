//========
// Guid.h
//========

#pragma once


//=======
// Using
//=======

#include "Variable.h"


//========
// Struct
//========

class GLOBAL_UNIQUE_ID
{
public:
	// Con-/Destructors
	GLOBAL_UNIQUE_ID() { ZeroMemory(pData, 16); }
	GLOBAL_UNIQUE_ID(GLOBAL_UNIQUE_ID const& Id) { CopyMemory(pData, Id.pData, 16); }
	GLOBAL_UNIQUE_ID(DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11);

	// Access
	Handle<String> ToString()const { return ToString(*this); }
	static Handle<String> ToString(GLOBAL_UNIQUE_ID const& Id);

	// Comparison
	BOOL operator==(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(pData, Id.pData, 16)==0; }
	BOOL operator>(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(pData, Id.pData, 16)>0; }
	BOOL operator>=(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(pData, Id.pData, 16)>=0; }
	BOOL operator<(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(pData, Id.pData, 16)<0; }
	BOOL operator<=(GLOBAL_UNIQUE_ID const& Id) { return CompareMemory(pData, Id.pData, 16)<=0; }

private:
	// Common
	BYTE pData[16];
};


//==========
// Variable
//==========

class Guid: public TypedVariable<GLOBAL_UNIQUE_ID>
{
public:
	// Con-/Destructors
	Guid(): Guid(nullptr) {}
	Guid(GLOBAL_UNIQUE_ID const& Value): Guid(nullptr, Value) {}
	Guid(DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11):
		Guid(nullptr, GLOBAL_UNIQUE_ID(Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11)) {}
	Guid(Handle<String> Name): Guid(Name, GLOBAL_UNIQUE_ID()) {}
	Guid(Handle<String> Name, GLOBAL_UNIQUE_ID const& Value): TypedVariable(Name, Value) {}
	Guid(Handle<String> Name, DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11):
		Guid(Name, GLOBAL_UNIQUE_ID(Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11)) {}

	// Common
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override;
};


//=============
// Guid-Handle
//=============

template <>
class Handle<Guid>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): m_Object(nullptr) {}
	Handle(decltype(nullptr)): m_Object(nullptr) {}
	Handle(Guid* Object) { HandleCreate(&m_Object, Object); }
	Handle(Handle const& Handle) { HandleCreate(&m_Object, Handle.m_Object); }
	Handle(Handle&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	~Handle() { HandleClear(&m_Object); }

	// Access
	operator GLOBAL_UNIQUE_ID()const { return VariableGet<Guid, GLOBAL_UNIQUE_ID>(m_Object, GLOBAL_UNIQUE_ID()); }
	Guid* operator->()const { return m_Object; }

	// Comparison
	BOOL operator==(GLOBAL_UNIQUE_ID Value) { return VariableEqual(m_Object, Value); }
	BOOL operator!=(GLOBAL_UNIQUE_ID Value) { return !VariableEqual(m_Object, Value); }
	BOOL operator>(GLOBAL_UNIQUE_ID Value) { return VariableAbove(m_Object, Value); }
	BOOL operator>=(GLOBAL_UNIQUE_ID Value) { return VariableAboveOrEqual(m_Object, Value); }
	BOOL operator<(GLOBAL_UNIQUE_ID Value) { return VariableBelow(m_Object, Value); }
	BOOL operator<=(GLOBAL_UNIQUE_ID Value) { return VariableBelowOrEqual(m_Object, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&m_Object); return *this; }
	Handle& operator=(GLOBAL_UNIQUE_ID Value) { VariableAssign(&m_Object, Value); return *this; }
	Handle& operator=(Guid* Object) { HandleAssign(&m_Object, Object); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&m_Object, Handle.m_Object); return *this; }

private:
	// Common
	Guid* m_Object;
};