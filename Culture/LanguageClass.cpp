//===================
// LanguageClass.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

//#include <locale.h>
#include "LanguageClass.h"


//===========
// Namespace
//===========

namespace Culture {


//==============
// String-Table
//==============

typedef struct
{
LPCSTR Name[2];
}STR_TABLE_ENTRY;

STR_TABLE_ENTRY LanguageStringTable[]=
	{
	{ "Deutsch", "Englisch (US)" },
	{ "German", "English (US)" }
	};


//==================
// Con-/Destructors
//==================

Language::Language(LanguageCode lng):
Language(nullptr, lng)
{}

Language::Language(Handle<String> hname, LanguageCode lng):
TypedVariable(hname, lng)
{
/*switch(lng)
	{
	case LanguageCode::DE:
		{
		setlocale(LC_ALL, "de_DE");
		break;
		}
	case LanguageCode::US:
		{
		setlocale(LC_ALL, "en_US");
		break;
		}
	default:
		break;
	}*/
}


//========
// Access
//========

LanguageCode Language::Current=LanguageCode::DE;

Handle<String> Language::ToString()
{
ScopedLock lock(cCriticalSection);
return ToString(tValue);
}

Handle<String> Language::ToString(LanguageCode code, LanguageCode lng)
{
TCHAR pstr[16];
ToString(code, pstr, 16, lng);
return pstr;
}

UINT Language::ToString(LanguageCode code, LPTSTR pstr, UINT usize, LanguageCode lng)
{
if(lng==LanguageCode::None)
	lng=Language::Current;
UINT ucode=(UINT)code;
UINT ulng=(UINT)lng;
return StringCopy(pstr, usize, LanguageStringTable[ucode].Name[ulng]);
}

}
