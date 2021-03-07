//=========
// Label.h
//=========

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


//=======
// Label
//=======

class Label: public Interactive
{
public:
	// Con-/Destructors
	Label(Handle<Variable> Variable): Label(Container::Current, Variable) {}
	Label(Handle<Container> Parent, Handle<Variable> Variable);

	// Common
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}
