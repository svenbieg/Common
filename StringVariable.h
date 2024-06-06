//==================
// StringVariable.h
//==================

#pragma once


//=======
// Using
//=======

#include "StringClass.h"
#include "Variable.h"


//=================
// String-Variable
//=================

class StringVariable: public Variable
{
public:
	// Con-/Destructors
	StringVariable(Handle<String> Name, Handle<String> Value=nullptr);

	// Access
	Handle<String> Get();
	Event<Variable, Handle<String>&> Reading;
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override { return Get(); }
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	BOOL FromString(Handle<String> Value, BOOL Notify=true)override;
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify)override;
	BOOL Set(Handle<String> Value, BOOL Notify=true);

private:
	// Common
	Handle<String> hValue;
};


//========================
// Handle String-Variable
//========================

template <>
class Handle<StringVariable>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(decltype(nullptr)): pObject(nullptr) {}
	Handle(Handle const& Handle) { HandleCreate(&pObject, Handle.pObject); }
	Handle(Handle&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	Handle(StringVariable* Variable)noexcept { HandleCreate<StringVariable, StringVariable>(&pObject, Variable); }
	Handle(Handle<String> Id, Handle<String> Value) { HandleCreate<StringVariable, StringVariable>(&pObject, new StringVariable(Id, Value)); }
	~Handle() { HandleClear(&pObject); }

	// Access
	operator StringVariable*() { return pObject; }
	operator Handle<String>() { return pObject? pObject->Get(): nullptr; }
	StringVariable* operator->() { return pObject; }

	// Comparison
	BOOL operator==(decltype(nullptr))const
		{
		if(!pObject)
			return true;
		auto hstr=pObject->Get();
		return hstr==nullptr;
		}
	BOOL operator==(LPCSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr==Value;
		}
	BOOL operator==(LPCWSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr==Value;
		}
	BOOL operator==(Handle<String> const& Handle)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr==Handle;
		}
	inline BOOL operator!=(decltype(nullptr))const { return !operator==(nullptr); }
	inline BOOL operator!=(LPCSTR Value)const { return !operator==(Value); }
	inline BOOL operator!=(LPCWSTR Value)const { return !operator==(Value); }
	inline BOOL operator!=(Handle<String> const& Handle)const { return !operator==(Handle); }
	BOOL operator>(decltype(nullptr))const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr>nullptr;
		}
	BOOL operator>(LPCSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr>Value;
		}
	BOOL operator>(LPCWSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr>Value;
		}
	BOOL operator>(Handle<String> const& Handle)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr>Handle;
		}
	BOOL operator>=(decltype(nullptr))const { return true; }
	BOOL operator>=(LPCSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr>=Value;
		}
	BOOL operator>=(LPCWSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr>=Value;
		}
	BOOL operator>=(Handle<String> const& Handle)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr>=Handle;
		}
	BOOL operator<(decltype(nullptr))const { return false; }
	BOOL operator<(LPCSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr<Value;
		}
	BOOL operator<(LPCWSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr<Value;
		}
	BOOL operator<(Handle<String> const& Handle)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr<Handle;
		}
	BOOL operator<=(decltype(nullptr))const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr<=nullptr;
		}
	BOOL operator<=(LPCSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr<=Value;
		}
	BOOL operator<=(LPCWSTR Value)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr<=Value;
		}
	BOOL operator<=(Handle<String> const& Handle)const
		{
		if(!pObject)
			return false;
		auto hstr=pObject->Get();
		return hstr<=Handle;
		}

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&pObject); return *this; }
	Handle& operator=(LPCSTR Value)
		{
		if(pObject)
			{
			pObject->Set(Value);
			return *this;
			}
		pObject=new StringVariable(Value);
		pObject->AddReference();
		return *this;
		}
	Handle& operator=(LPCWSTR Value)
		{
		if(pObject)
			{
			pObject->Set(Value);
			return *this;
			}
		pObject=new StringVariable(Value);
		pObject->AddReference();
		return *this;
		}
	Handle& operator=(String* String)
		{
		if(pObject)
			{
			pObject->Set(String);
			return *this;
			}
		pObject=new StringVariable(String);
		pObject->AddReference();
		return *this;
		}
	Handle& operator=(StringVariable* Object) { HandleAssign<StringVariable, StringVariable>(&pObject, Object); return *this; }
	Handle& operator=(Handle<String> const& Handle) { return operator=(Handle.pObject); }
	Handle& operator=(Handle<StringVariable> const& Handle) { return operator=(Handle.pObject); }

private:
	StringVariable* pObject;
};
