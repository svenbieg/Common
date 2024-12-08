//============
// Sentence.h
//============

#pragma once


//=======
// Using
//=======

#include "Resources/Strings/String.h"
#include "Storage/Streams/InputStream.h"
#include "Storage/Streams/OutputStream.h"
#include "StringClass.h"


//===========
// Namespace
//===========

namespace Culture {


//========
// Common
//========

LPCSTR Translate(Resources::Strings::STRING const* Value, Culture::LanguageCode Language=Culture::CurrentLanguage);


//==========
// Sentence
//==========

class Sentence: public Object
{
public:
	// Using
	using InputStream=Storage::Streams::InputStream;
	using LanguageCode=Culture::LanguageCode;
	using OutputStream=Storage::Streams::OutputStream;
	using STRING=Resources::Strings::STRING;

	// Con-/Destructors
	Sentence();
	Sentence(LPCSTR Value);
	Sentence(STRING const* Value);
	template <class... _args_t> Sentence(LPCSTR Format, _args_t... Arguments)
		{
		UnknownClass args[]={ Arguments... };
		VariableArguments vargs(args, ArraySize(args));
		UINT len=StringPrintArgs((LPSTR)nullptr, 0, Format, vargs);
		LPSTR value=new CHAR[len+1];
		StringPrintArgs(value, len+1, Format, vargs);
		STRING* string=new STRING[1];
		string->Language=LanguageCode::None;
		string->Value=value;
		m_String=string;
		m_Value=value;
		}
	~Sentence();

	// Common
	LPCSTR Begin(LanguageCode Language=Culture::CurrentLanguage)const;
	INT Compare(LPCSTR Value)const;
	INT Compare(LPCWSTR Value)const;
	INT Compare(STRING const* Value)const;
	INT Compare(Sentence const* Value)const;
	SIZE_T ReadFromStream(InputStream* Stream);
	SIZE_T WriteToStream(OutputStream* Stream);

private:
	// Common
	STRING const* m_String;
	LPCSTR m_Value;
};

}


//========
// Handle
//========

template <>
class Handle<Culture::Sentence>: public ::Details::HandleBase<Culture::Sentence>
{
public:
	// Using
	using _base_t=::Details::HandleBase<Culture::Sentence>;
	using _base_t::_base_t;
	using Sentence=Culture::Sentence;
	using STRING=Resources::Strings::STRING;

	// Con-/Destructors
	Handle(LPCSTR Value) { Create(new Sentence(Value)); }
	Handle(STRING const* Value) { Create(new Sentence(Value)); }

	// Assignment
	inline Handle& operator=(LPCSTR Value) { Set(new Sentence(Value)); return *this; }
	inline Handle& operator=(STRING const* Value) { Set(new Sentence(Value)); return *this; }

	// Comparison
	inline bool operator==(Sentence* Value)const override { return Compare(m_Object, Value)==0; }
	inline bool operator>(Handle<Sentence> const& Value)const { return Compare(m_Object, Value.m_Object)>0; }
	inline bool operator>=(Handle<Sentence> const& Value)const { return Compare(m_Object, Value.m_Object)>=0; }
	inline bool operator<(Handle<Sentence> const& Value)const { return Compare(m_Object, Value.m_Object)<0; }
	inline bool operator<=(Handle<Sentence> const& Value)const { return Compare(m_Object, Value.m_Object)<=0; }

private:
	// Common
	static inline INT Compare(Sentence const* Sentence1, Sentence const* Sentence2)
		{
		if(Sentence1)
			return Sentence1->Compare(Sentence2);
		if(Sentence2)
			return -1;
		return 0;
		}
};
