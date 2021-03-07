//============
// Button.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Web/WebPage.h"
#include "Button.h"

using namespace Network::Http;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//=========
// Scripts
//=========

const CHAR ButtonScript[]=
"function onButtonClick(id) {\r\n"
"var request=new XMLHttpRequest();\r\n"
"var param='?id='.concat(id);\r\n"
"param=param.concat('&update=').concat(lastupdate);\r\n"
"request.open('NOTIFY', param);\r\n"
"request.addEventListener('load', function(e) {\r\n"
"\tif(request.status==200)\r\n"
"\t\tdocument.dispatchEvent(new CustomEvent('eventUpdateWebPage', { detail: request.responseText }));\r\n"
"\t});\r\n"
"request.send();\r\n"
"}\r\n\r\n";


//==================
// Con-/Destructors
//==================

Button::Button(Handle<Container> hparent, Handle<String> hid, Handle<String> htext, Handle<String> hscript, Handle<Variable> hvar, Handle<String> hparam):
Control(hparent),
Id(hid),
Script(hscript),
Text(htext),
hParam(hparam),
hVariable(hvar)
{
auto hpage=WebPage::Current;
hpage->Scripts->Add(ButtonScript);
hpage->Controls->Add(Id, this);
}

Button::~Button()
{
WebPage::Current->Controls->Remove(Id);
}


//========
// Common
//========

VOID Button::OnNotified(Handle<String> hparams)
{
if(hVariable&&hParam)
	hVariable->FromString(hParam);
Clicked(this);
}

SIZE_T Button::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("<button id=\"");
usize+=writer.Print(Id);
usize+=writer.Print("\"");
usize+=WriteControlAttributes(hstream);
usize+=writer.Print(" onclick=\"");
if(Script)
	{
	usize+=writer.Print(Script);
	usize+=writer.Print(";");
	}
usize+=writer.Print("onButtonClick('");
usize+=writer.Print(Id);
usize+=writer.Print("')\" type=\"button\">");
usize+=writer.Print(Text);
usize+=writer.Print("</button>");
usize+=WriteBreaks(hstream);
return usize;
}

}}
