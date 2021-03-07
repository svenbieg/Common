//============
// Column.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Table.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {
		namespace Tables {


//==================
// Con-/Destructors
//==================

Column::Column(Handle<String> hstyle):
Column(Table::Current, hstyle)
{}

Column::Column(Handle<Table> hparent, Handle<String> hstyle):
Style(hstyle)
{
if(hparent)
	hparent->Columns->Add(this);
}


//========
// Access
//========

SIZE_T Column::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("<col");
if(Style)
	{
	usize+=writer.Print(" style=\"");
	usize+=writer.Print(Style);
	usize+=writer.Print("\"");
	}
usize+=writer.Print(" />");
return usize;
}

}}}
