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
	Handle(): m_Object(nullptr) {}
	Handle(decltype(nullptr)): m_Object(nullptr) {}
	Handle(Handle const& Handle) { HandleCreate(&m_Object, Handle.m_Object); }
	Handle(Handle&& Handle)noexcept: m_Object(Handle.m_Object) { Handle.m_Object=nullptr; }
	Handle(StringVariable* Variable)noexcept { HandleCreate<StringVariable, StringVariable>(&m_Object, Variable); }
	Handle(Handle<String> Id, Handle<String> Value) { HandleCreate<StringVariable, StringVariable>(&m_Object, new StringVariable(Id, Value)); }
	~Handle() { HandleClear(&m_Object); }

	// Access
	operator StringVariable*() { return m_Object; }
	operator Handle<String>() { return m_Object? m_Object->Get(): nullptr; }
	StringVariable* operator->() { return m_Object; }

	// Comparison
	BOOL operator==(decltype(nullptr))const
		{
		if(!m_Object)
			return true;
		auto hstr=m_Object->Get();
		return hstr==nullptr;
		}
	BOOL operator==(LPCSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr==Value;
		}
	BOOL operator==(LPCWSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr==Value;
		}
	BOOL operator==(Handle<String> const& Handle)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr==Handle;
		}
	inline BOOL operator!=(decltype(nullptr))const { return !operator==(nullptr); }
	inline BOOL operator!=(LPCSTR Value)const { return !operator==(Value); }
	inline BOOL operator!=(LPCWSTR Value)const { return !operator==(Value); }
	inline BOOL operator!=(Handle<String> const& Handle)const { return !operator==(Handle); }
	BOOL operator>(decltype(nullptr))const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr>nullptr;
		}
	BOOL operator>(LPCSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr>Value;
		}
	BOOL operator>(LPCWSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr>Value;
		}
	BOOL operator>(Handle<String> const& Handle)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr>Handle;
		}
	BOOL operator>=(decltype(nullptr))const { return true; }
	BOOL operator>=(LPCSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr>=Value;
		}
	BOOL operator>=(LPCWSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr>=Value;
		}
	BOOL operator>=(Handle<String> const& Handle)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr>=Handle;
		}
	BOOL operator<(decltype(nullptr))const { return false; }
	BOOL operator<(LPCSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr<Value;
		}
	BOOL operator<(LPCWSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr<Value;
		}
	BOOL operator<(Handle<String> const& Handle)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr<Handle;
		}
	BOOL operator<=(decltype(nullptr))const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr<=nullptr;
		}
	BOOL operator<=(LPCSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr<=Value;
		}
	BOOL operator<=(LPCWSTR Value)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr<=Value;
		}
	BOOL operator<=(Handle<String> const& Handle)const
		{
		if(!m_Object)
			return false;
		auto hstr=m_Object->Get();
		return hstr<=Handle;
		}

	// Assignment
	Handle& operator=(decltype(nullptr)) { HandleClear(&m_Object); return *this; }
	Handle& operator=(LPCSTR Value)
		{
		if(m_Object)
			{
			m_Object->Set(Value);
			return *this;
			}
		m_Object=new StringVariable(Value);
		m_Object->AddReference();
		return *this;
		}
	Handle& operator=(LPCWSTR Value)
		{
		if(m_Object)
			{
			m_Object->Set(Value);
			return *this;
			}
		m_Object=new StringVariable(Value);
		m_Object->AddReference();
		return *this;
		}
	Handle& operator=(String* String)
		{
		if(m_Object)
			{
			m_Object->Set(String);
			return *this;
			}
		m_Object=new StringVariable(String);
		m_Object->AddReference();
		return *this;
		}
	Handle& operator=(StringVariable* Object) { HandleAssign<StringVariable, StringVariable>(&m_Object, Object); return *this; }
	Handle& operator=(Handle<String> const& Handle) { return operator=(Handle.m_Object); }
	Handle& operator=(Handle<StringVariable> const& Handle) { return operator=(Handle.m_Object); }

private:
	StringVariable* m_Object;
};
