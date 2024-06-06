//==============
// Sentence.cpp
//==============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Sentence.h"

using namespace Culture;
using namespace Resources::Strings;
using namespace Storage::Streams;


//========
// Common
//========

LPCSTR Translate(STRING const* string_ptr, LanguageCode lng)
{
if(!string_ptr)
	return nullptr;
auto string=string_ptr;
while(string->Language!=lng)
	{
	if(string->Language==LanguageCode::None)
		break;
	string++;
	}
return string->Value;
}


//==================
// Con-/Destructors
//==================

Sentence::Sentence():
pString(nullptr),
pValue(nullptr)
{}

Sentence::Sentence(LPCSTR value):
pString(nullptr),
pValue(value)
{}

Sentence::Sentence(STRING const* value):
pString(value),
pValue(nullptr)
{}

Sentence::~Sentence()
{
if(pString&&pValue)
	{
	delete pString;
	delete pValue;
	}
}


//========
// Common
//========

LPCSTR Sentence::Begin(LanguageCode lng)const
{
if(pString)
	return Translate(pString, lng);
return pValue;
}

template <class _char_t> INT SentenceCompare(STRING const* string, _char_t const* value)
{
if(!value)
	return 1;
while(string)
	{
	if(StringCompare(string->Value, value, 0, false)==0)
		return 0;
	if(string->Language==LanguageCode::None)
		break;
	string++;
	}
return 1;
}

INT Sentence::Compare(LPCSTR value)const
{
if(pString)
	return SentenceCompare<CHAR>(pString, value);
return StringCompare(pValue, value, 0, false)==0;
}

INT Sentence::Compare(LPCWSTR value)const
{
if(pString)
	return SentenceCompare<WCHAR>(pString, value);
return StringCompare(pValue, value, 0, false)==0;
}

INT Sentence::Compare(STRING const* string)const
{
if(pString==string)
	return 0;
if(!pString)
	return -1;
if(!string)
	return 1;
auto string1=pString;
while(string1)
	{
	auto string2=string;
	while(string2)
		{
		if(string1->Language==string2->Language)
			{
			if(StringCompare(string1->Value, string2->Value, 0, false)==0)
				return 0;
			}
		if(string2->Language==LanguageCode::None)
			break;
		string2++;
		}
	if(string1->Language==LanguageCode::None)
		break;
	string1++;
	}
return 1;
}

INT Sentence::Compare(Sentence const* sentence)const
{
STRING const* string=sentence? sentence->pString: nullptr;
return Compare(string);
}

SIZE_T Sentence::ReadFromStream(InputStream* stream)
{
SIZE_T size=0;
StreamReader reader(stream);
auto str=reader.ReadString(&size);
if(!str)
	return size;
auto str_ptr=str->Begin();
UINT len=StringLength(str_ptr);
pValue=new CHAR[len+1];
LPSTR value_ptr=const_cast<LPSTR>(pValue);
UINT count=0;
for(UINT pos=0; pos<len; pos++)
	{
	TCHAR c=str_ptr[pos];
	if(CharCompare(c, '\r')==0)
		{
		c=0;
		count++;
		}
	value_ptr[pos]=CharToChar<CHAR, TCHAR>(c);
	}
pString=new STRING[count+1];
auto string=const_cast<STRING*>(pString);
UINT id=0;
for(UINT pos=0; pos<len; pos++)
	{
	auto lng=LanguageCodeFromString(&value_ptr[pos]);
	if(lng==LanguageCode::Unknown)
		break;
	pos+=2;
	string[id].Language=lng;
	string[id].Value=&value_ptr[pos];
	id++;
	while(value_ptr[pos])
		pos++;
	}
return size;
}

SIZE_T Sentence::WriteToStream(OutputStream* stream)
{
SIZE_T size=0;
StreamWriter writer(stream);
if(pString)
	{
	SIZE_T size=0;
	auto string=pString;
	while(1)
		{
		auto lng=string->Language;
		auto lng_str=LanguageCodeToString(lng);
		size+=writer.Print(lng_str);
		size+=writer.Print(string->Value);
		size+=writer.PrintChar('\r');
		if(lng==LanguageCode::None)
			break;
		string++;
		}
	size+=writer.Print("\0");
	return size;
	}
auto lng_str=LanguageCodeToString(LanguageCode::None);
size+=writer.Print(lng_str);
size+=writer.Print(pValue);
size+=writer.Print("\r\0");
return size;
}
