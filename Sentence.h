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
		pString=string;
		pValue=value;
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
	STRING const* pString;
	LPCSTR pValue;
};


//=================
// Sentence-Handle
//=================

template <>
class Handle<Sentence>
{
public:
	// Using
	using STRING=Resources::Strings::STRING;

	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(std::nullptr_t): pObject(nullptr) {}
	Handle(LPCSTR Value) { HandleCreate<Sentence, Sentence>(&pObject, new Sentence(Value)); }
	Handle(STRING const* Value) { HandleCreate<Sentence, Sentence>(&pObject, new Sentence(Value)); }
	Handle(Sentence* Value) { HandleCreate<Sentence, Sentence>(&pObject, Value); }
	Handle(Handle<Sentence> const& Handle) { HandleCreate<Sentence, Sentence>(&pObject, Handle.pObject); }
	Handle(Handle<Sentence>&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleClear(&pObject); }

	// Access
	inline operator Sentence*()const { return pObject; }
	inline Sentence* operator->()const { return pObject; }

	// Assignment
	inline Handle& operator=(std::nullptr_t) { HandleClear(&pObject); return *this; }
	inline Handle& operator=(LPCSTR Value) { HandleAssign<Sentence, Sentence>(&pObject, new Sentence(Value)); return *this; }
	inline Handle& operator=(STRING const* Value) { HandleAssign<Sentence, Sentence>(&pObject, new Sentence(Value)); return *this; }
	inline Handle& operator=(Handle<Sentence> Value) { HandleAssign<Sentence, Sentence>(&pObject, Value.pObject); return *this; }
	inline Handle& operator=(Sentence* Value) { HandleAssign<Sentence, Sentence>(&pObject, Value); return *this; }

	// Comparison
	inline bool operator==(std::nullptr_t)const { return pObject==nullptr; }
	inline bool operator==(Handle<Sentence> const& Value)const { return Compare(pObject, Value.pObject)==0; }
	inline bool operator!=(std::nullptr_t)const { return pObject!=nullptr; }
	inline bool operator!=(Handle<Sentence> const& Value)const { return Compare(pObject, Value.pObject)!=0; }
	inline bool operator>(Handle<Sentence> const& Value)const { return Compare(pObject, Value.pObject)>0; }
	inline bool operator>=(Handle<Sentence> const& Value)const { return Compare(pObject, Value.pObject)>=0; }
	inline bool operator<(Handle<Sentence> const& Value)const { return Compare(pObject, Value.pObject)<0; }
	inline bool operator<=(Handle<Sentence> const& Value)const { return Compare(pObject, Value.pObject)<=0; }

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
	Sentence* pObject;
};
