//=============
// EditBox.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Web/WebPage.h"
#include "EditBox.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//=========
// Scripts
//=========

const CHAR EditBoxScript[]=
"function onEditBoxChanged(id, value) {\r\n"
"var request=new XMLHttpRequest();\r\n"
"request.open('NOTIFY', '?id='.concat(id, \"&value=\", value));\r\n"
"request.addEventListener('load', function(e) {\r\n"
"\tif(request.status==200)\r\n"
"\t\tdocument.dispatchEvent(new CustomEvent('eventUpdateWebPage', { detail: request.responseText }));\r\n"
"\t});\r\n"
"request.send();\r\n"
"}\r\n\r\n";



//==================
// Con-/Destructors
//==================

EditBox::EditBox(Handle<Container> hparent, Handle<Variable> hvar, Handle<String> hclass, Handle<String> hstyle):
Interactive(hparent, hvar, hclass, hstyle),
InputType(EditBoxInputType::Text)
{
WebPage::Current->Scripts->Add(EditBoxScript);
}


//========
// Common
//========

VOID EditBox::OnNotified(Handle<String> hparams)
{
auto hvalue=hparams->Get("value");
hVariable->FromString(hvalue);
}

SIZE_T EditBox::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
auto hid=hVariable->GetName();
if(Label||Unit)
	{
	usize+=writer.Print("<label for=\"");
	usize+=writer.Print(hid);
	usize+=writer.Print("\">");
	if(Label)
		{
		usize+=writer.Print(Label);
		usize+=writer.Print("&nbsp;");
		}
	}
usize+=writer.Print("<input id=\"");
usize+=writer.Print(hid);
usize+=writer.Print("\"");
usize+=WriteControlAttributes(hstream);
if(InputType!=EditBoxInputType::Text)
	{
	usize+=writer.Print("\" type=\"");
	switch(InputType)
		{
		default:
		case EditBoxInputType::Number:
			{
			usize+=writer.Print("number");
			break;
			}
		case EditBoxInputType::Password:
			{
			usize+=writer.Print("password");
			break;
			}
		}
	usize+=writer.Print("\"");
	}
usize+=writer.Print(" value=\"");
if(hVariable)
	usize+=writer.Print(hVariable->ToString());
usize+=writer.Print("\" onchange=\"onEditBoxChanged('");
usize+=writer.Print(hid);
usize+=writer.Print("', this.value)\" />");
if(Label||Unit)
	{
	if(Unit)
		{
		usize+=writer.Print("&nbsp;");
		usize+=writer.Print(Unit);
		}
	usize+=writer.Print("</label>");
	}
usize+=WriteBreaks(hstream);
return usize;
}

}}
