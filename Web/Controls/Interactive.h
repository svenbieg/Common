//===============
// Interactive.h
//===============

#pragma once


//=======
// Using
//=======

#include "Container.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//=============
// Interactive
//=============

class Interactive: public Control
{
public:
	// Common
	SIZE_T WriteUpdateToStream(Handle<OutputStream> Stream, UINT LastTime)override;

protected:
	// Con-/Destructors
	Interactive(Handle<Variable> Variable, Handle<String> Class=nullptr, Handle<String> Style=nullptr): Interactive(Container::Current, Variable, Class, Style) {}
	Interactive(Handle<Container> Parent, Handle<Variable> Variable, Handle<String> Class=nullptr, Handle<String> Style=nullptr);
	~Interactive();

	// Common
	VOID OnVariableChanged(Handle<Variable> Variable);
	Handle<Variable> hVariable;
	UINT uTimeChanged;
};

}}