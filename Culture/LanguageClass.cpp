//===================
// LanguageClass.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "Resources/Strings/Language.h"
#include "LanguageClass.h"
#include "Sentence.h"

using namespace Resources::Strings;


//===========
// Namespace
//===========

namespace Culture {


//==================
// Con-/Destructors
//==================

Language::Language(Handle<String> name, LanguageCode lng):
TypedVariable(name, lng)
{}


//========
// Common
//========

typedef struct
{
LNG Language;
STRING const* String;
}TRANSLATION;

constexpr TRANSLATION TRANS_LANGUAGE[]=
	{
	{ LNG::DE, STR_GERMAN },
	{ LNG::EN, STR_ENGLISH },
	{ LNG::None, nullptr }
	};

Handle<String> Language::ToString(LanguageCode lng_str)
{
auto lng=Get();
TRANSLATION const* trans=TRANS_LANGUAGE;
while(trans->Language!=LNG::None)
	{
	if(trans->Language==lng)
		return Translate(trans->String, lng_str);
	trans++;
	}
return "STR_LANGUAGE";
}

}