//========
// Form.h
//========

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


//======
// Form
//======

class Form: public Container
{
public:
	// Con-/Destructors
	Form() {}

	// Access
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}
