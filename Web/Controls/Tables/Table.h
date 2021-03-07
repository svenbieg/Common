//=========
// Table.h
//=========

#pragma once


//=======
// Using
//=======

#include "Collections/Index.h"
#include "Column.h"
#include "Row.h"


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {
		namespace Tables {


//=======
// Table
//=======

class Table: public Container
{
private:
	// Using
	using ColumnIndex=Collections::Index<Handle<Column>>;
	using OutputStream=Storage::Streams::OutputStream;

public:
	// Friends
	friend Column;

	// Con-/Destructors
	Table(Handle<String> Class=nullptr, Handle<String> Caption=nullptr): Table(Container::Current, Class, Caption) {}
	Table(Handle<Container> Parent, Handle<String> Class=nullptr, Handle<String> Caption=nullptr);

	// Common
	Handle<String> Caption;
	Handle<ColumnIndex> Columns;
	static Handle<Table> Current;
	VOID Select()override { Current=this; Container::Select(); }
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;
};

}}}
