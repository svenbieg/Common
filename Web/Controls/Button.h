//==========
// Button.h
//==========

#pragma once


//=======
// Using
//=======

#include "Network/Http/HttpConnection.h"
#include "Container.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//========
// Button
//========

class Button: public Control
{
public:
	// Con-/Destructors
	Button(Handle<String> Id, Handle<String> Text): Button(Container::Current, Id, Text, nullptr, nullptr, nullptr) {}
	Button(Handle<String> Id, Handle<String> Text, Handle<String> Script): Button(Container::Current, Id, Text, Script, nullptr, nullptr) {}
	Button(Handle<String> Id, Handle<String> Text, Handle<Variable> Variable, Handle<String> Param): Button(Container::Current, Id, Text, nullptr, Variable, Param) {}
	Button(Handle<Container> Parent, Handle<String> Id, Handle<String> Text, Handle<String> Script, Handle<Variable> Variable, Handle<String> Param);
	~Button();

	// Common
	Event<Button> Clicked;
	Handle<String> Id;
	VOID OnNotified(Handle<String> Params)override;
	Handle<String> Script;
	Handle<String> Text;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;

private:
	// Common
	Handle<String> hParam;
	Handle<Variable> hVariable;
};

}}
