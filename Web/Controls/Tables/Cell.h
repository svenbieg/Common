//========
// Cell.h
//========

#pragma once


//=======
// Using
//=======

#include "Web/Controls/Container.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {
		namespace Tables {


//======================
// Forward-Declarations
//======================

class Row;


//======
// Cell
//======

class Cell: public Container
{
public:
	// Con-/Destructors
	Cell(Handle<String> Class=nullptr);
	Cell(Handle<Row> Parent, Handle<String> Class=nullptr);

	// Access
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}}
