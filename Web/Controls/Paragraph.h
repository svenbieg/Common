//=============
// Paragraph.h
//=============

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


//===========
// Paragraph
//===========

class Paragraph: public Container
{
public:
	// Con-/Destructors
	Paragraph(): Paragraph(Container::Current) {}
	Paragraph(Handle<Container> Parent);

	// Common
	SIZE_T WriteToStream(Handle<OutputStream> Stream);
};

}}
