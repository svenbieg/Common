//==========
// Cell.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Row.h"

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

Cell::Cell(Handle<String> hclass):
Cell(Row::Current, hclass)
{}

Cell::Cell(Handle<Row> hparent, Handle<String> hclass):
Container(hparent, hclass)
{}


//========
// Access
//========

SIZE_T Cell::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("<td");
usize+=WriteControlAttributes(hstream);
usize+=writer.Print(">");
usize+=WriteContent(hstream);
usize+=writer.Print("</td>");
return usize;
}

}}}
