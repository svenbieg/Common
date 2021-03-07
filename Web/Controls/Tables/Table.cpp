//===========
// Table.cpp
//===========

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

Table::Table(Handle<Container> hparent, Handle<String> hclass, Handle<String> hcaption):
Container(hparent, hclass),
Caption(hcaption)
{
Columns=new ColumnIndex();
Current=this;
}


//========
// Access
//========

Handle<Table> Table::Current;

SIZE_T Table::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("<table");
usize+=WriteControlAttributes(hstream);
usize+=writer.Print(">\r\n");
if(Caption)
	{
	usize+=writer.Print("<caption>");
	usize+=writer.Print(Caption);
	usize+=writer.Print("</caption>\r\n");
	}
if(Columns->GetCount())
	{
	usize+=writer.Print("<colgroup>\r\n");
	for(auto hit=Columns->First(); hit->HasCurrent(); hit->MoveNext())
		{
		auto hcol=hit->GetCurrent();
		usize+=writer.Print("\t");
		usize+=hcol->WriteToStream(hstream);
		usize+=writer.Print("\r\n");
		}
	usize+=writer.Print("</colgroup>\r\n");
	}
usize+=writer.Print("<tbody>");
usize+=WriteContent(hstream);
usize+=writer.Print("</tbody></table>\r\n");
usize+=WriteBreaks(hstream);
return usize;
}

}}}
