//============
// LogBox.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Web/WebPage.h"
#include "LogBox.h"

using namespace Physics;
using namespace Storage::Collections;
using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//==================
// Con-/Destructors
//==================

LogBox::LogBox(Handle<Container> hparent, Handle<String> hid, Handle<Log> hlog):
Control(hparent, "logbox"),
Id(hid),
hLog(hlog),
uTimeChanged(0)
{
auto hpage=WebPage::Current;
hpage->Controls->Add(Id, this);
hpage->Styles->Add("table.logbox td { vertical-align:top; }\r\n");
hpage->Styles->Add("table.logbox th { padding-right:8px; text-align:right; vertical-align:top; }\r\n");
hClearButton=new Button(nullptr, "LogBoxClear", "Löschen", nullptr, nullptr, nullptr);
hClearButton->Clicked.Add(this, &LogBox::OnClearButtonClick);
hClearButton->Break();
hLog->Changed.Add(this, &LogBox::OnLogChanged);
}


//========
// Common
//========

SIZE_T LogBox::WriteToStream(Handle<OutputStream> hstream)
{
StreamWriter writer(hstream);
SIZE_T usize=0;
usize+=writer.Print("<table id=\"");
usize+=writer.Print(Id);
usize+=writer.Print("\"");
usize+=WriteControlAttributes(hstream);
usize+=writer.Print("><tbody>");
for(auto hit=hLog->First(); hit->HasCurrent(); hit->MoveNext())
	{
	Handle<LogEntry> hentry=hit->GetCurrent();
	usize+=writer.Print("<tr><th>");
	usize+=writer.Print(hentry->Time->ToString(TimeFormat::DateTime));
	usize+=writer.Print("</th><td>");
	usize+=writer.Print(hentry->Message);
	usize+=writer.Print("</td></tr>");
	}
usize+=writer.Print("</tbody></table><br />");
usize+=hClearButton->WriteToStream(hstream);
usize+=WriteBreaks(hstream);
return usize;
}

SIZE_T LogBox::WriteUpdateToStream(Handle<OutputStream> hstream, UINT ulasttime)
{
if(uTimeChanged<ulasttime)
	return 0;
StreamWriter writer(hstream);
SIZE_T usize=0;
usize+=writer.Print(Id);
usize+=writer.Print("=");
for(auto hit=hLog->First(); hit->HasCurrent(); hit->MoveNext())
	{
	Handle<LogEntry> hentry=hit->GetCurrent();
	usize+=writer.Print("<tr><th>");
	usize+=writer.Print(hentry->Time->ToString(TimeFormat::DateTime));
	usize+=writer.Print("</th><td>");
	usize+=writer.Print(hentry->Message);
	usize+=writer.Print("</td></tr>");
	}
usize+=writer.Print(";");
return usize;
}


//================
// Common Private
//================

VOID LogBox::OnClearButtonClick(Handle<Button> hbtn)
{
hLog->Clear();
hLog->Write("Protokoll zurückgesetzt");
}

VOID LogBox::OnLogChanged(Handle<Log> Log)
{
uTimeChanged=GetTickCount();
}

}}
