//=======
// Row.h
//=======

#pragma once


//=======
// Using
//=======

#include "Cell.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {
		namespace Tables {


//======================
// Forward-Declarations
//======================

class Table;


//=====
// Row
//=====

class Row: public Container
{
public:
	// Con-/Destructors
	Row();
	Row(Handle<Table> Parent);

	// Access
	static Handle<Row> Current;
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;

	// Modification
	VOID Select()override { Current=this; Container::Select(); }
};

}}}
