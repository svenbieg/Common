//=============
// Control.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Web/WebPage.h"
#include "Control.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//========
// Common
//========

SIZE_T Control::WriteToStream(Handle<OutputStream> hstream)
{
return WriteBreaks(hstream);
}


//============================
// Con-/Destructors Protected
//============================

Control::Control():
uBreaks(0)
{}

Control::Control(Handle<Container> hparent, Handle<String> hclass, Handle<String> hstyle):
Class(hclass),
Parent(hparent),
Style(hstyle),
uBreaks(0)
{
if(hparent!=nullptr)
	hparent->Children->Append(this);
}


//==================
// Common Protected
//==================

SIZE_T Control::WriteBreaks(Handle<OutputStream> hstream)
{
if(uBreaks==0)
	return 0;
SIZE_T usize=0;
StreamWriter writer(hstream);
for(UINT u=0; u<uBreaks; u++)
	usize+=writer.Print("<br />");
usize+=writer.Print("\r\n");
return usize;
}

SIZE_T Control::WriteControlAttributes(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
if(Class)
	{
	usize+=writer.Print(" class=\"");
	usize+=writer.Print(Class);
	usize+=writer.Print("\"");
	}
if(Style)
	{
	usize+=writer.Print(" style=\"");
	usize+=writer.Print(Style);
	usize+=writer.Print("\"");
	}
return usize;
}

}}
