//==========
// Column.h
//==========

#pragma once


//=======
// Using
//=======

#include "Storage/Streams/OutputStream.h"


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


//========
// Column
//========

class Column: public Object
{
private:
	// Using
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Con-/Destructors
	Column(Handle<String> Style=nullptr);
	Column(Handle<Table> Parent, Handle<String> Style=nullptr);

	// Common
	Handle<String> Style;
	SIZE_T WriteToStream(Handle<OutputStream> Stream);
};

}}}
