//=========
// Row.cpp
//=========

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

Row::Row():
Row(Table::Current)
{}

Row::Row(Handle<Table> hparent):
Container(hparent)
{
Current=this;
}


//========
// Access
//========

Handle<Row> Row::Current;

SIZE_T Row::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("<tr>");
usize+=WriteContent(hstream);
usize+=writer.Print("</tr>\r\n");
return usize;
}

}}}
