//===========
// EditBox.h
//===========

#pragma once


//=======
// Using
//=======

#include "Interactive.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//============
// Input-Type
//============

enum class EditBoxInputType
{
Text,
Number,
Password
};


//==========
// Edit-Box
//==========

class EditBox: public Interactive
{
public:
	// Con-/Destructors
	EditBox(Handle<Variable> Variable, Handle<String> Class=nullptr, Handle<String> Style=nullptr): EditBox(Container::Current, Variable, Class, Style) {}
	EditBox(Handle<Container> Parent, Handle<Variable> Variable, Handle<String> Class=nullptr, Handle<String> Style=nullptr);

	// Common
	EditBoxInputType InputType;
	Handle<String> Label;
	VOID OnNotified(Handle<String> Params)override;
	Handle<String> Unit;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}
