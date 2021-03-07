//========
// Guid.h
//========

#pragma once


//============
// Guid-Class
//============

namespace Details {

class GUID
{
public:
	// Con-/Destructors
	GUID() { ZeroMemory(pData, 16); }
	GUID(GUID const& Id) { CopyMemory(pData, Id.pData, 16); }
	GUID(DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11);

	// Comparison
	BOOL operator==(GUID const& Id) { return CompareMemory(pData, Id.pData, 16)==0; }
	BOOL operator>(GUID const& Id) { return CompareMemory(pData, Id.pData, 16)>0; }
	BOOL operator>=(GUID const& Id) { return CompareMemory(pData, Id.pData, 16)>=0; }
	BOOL operator<(GUID const& Id) { return CompareMemory(pData, Id.pData, 16)<0; }
	BOOL operator<=(GUID const& Id) { return CompareMemory(pData, Id.pData, 16)<=0; }

private:
	// Common
	BYTE pData[16];
};

}


//======
// Guid
//======

class Guid: public TypedVariable<Details::GUID>
{
private:
	// Using
	using GUID=Details::GUID;

public:
	// Con-/Destructors
	Guid(): Guid(nullptr) {}
	Guid(GUID const& Value): Guid(nullptr, Value) {}
	Guid(DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11):
		Guid(nullptr, GUID(Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11)) {}
	Guid(Handle<String> Name): Guid(Name, GUID()) {}
	Guid(Handle<String> Name, GUID const& Value): TypedVariable(Name, Value) {}
	Guid(Handle<String> Name, DWORD Data1, WORD Data2, WORD Data3, BYTE Data4, BYTE Data5, BYTE Data6, BYTE Data7, BYTE Data8, BYTE Data9, BYTE Data10, BYTE Data11):
		Guid(Name, GUID(Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11)) {}
};


//=============
// Guid-Handle
//=============

template <>
class Handle<Guid>
{
private:
	// Using
	using GUID=Details::GUID;

public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(decltype(nullptr)): pObject(nullptr) {}
	Handle(Guid* Object) { HandleCreate(&pObject, Object); }
	Handle(Handle const& Handle) { HandleCreate(&pObject, Handle.pObject); }
	Handle(Handle&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleDestroy(pObject); }

	// Access
	operator GUID()const { return VariableGet<Guid, GUID>(pObject, GUID()); }
	Guid* operator->()const { return pObject; }

	// Comparison
	BOOL operator==(GUID Value) { return VariableEqual(pObject, Value); }
	BOOL operator!=(GUID Value) { return !VariableEqual(pObject, Value); }
	BOOL operator>(GUID Value) { return VariableGreater(pObject, Value); }
	BOOL operator>=(GUID Value) { return VariableGreaterOrEqual(pObject, Value); }
	BOOL operator<(GUID Value) { return VariableLess(pObject, Value); }
	BOOL operator<=(GUID Value) { return VariableLessOrEqual(pObject, Value); }

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&pObject); return *this; }
	Handle& operator=(GUID Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(Guid* Object) { HandleAssign(&pObject, Object); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&pObject, Handle.pObject); return *this; }

private:
	// Common
	Guid* pObject;
};