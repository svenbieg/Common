//=================
// LanguageClass.h
//=================

#pragma once


//=======
// Using
//=======

#include "Language.h"
#include "Variable.h"


//===========
// Namespace
//===========

namespace Culture {


//==========
// Language
//==========

class Language: public TypedVariable<LanguageCode>
{
public:
	// Con-/Destructors
	Language(Handle<String> Name, LanguageCode Language);

	// Common
	Handle<String> ToString(LanguageCode Language=Culture::CurrentLanguage)override;
};

}
