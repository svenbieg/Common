//===============
// HyperLink.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "HyperLink.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//==================
// Con-/Destructors
//==================

HyperLink::HyperLink(Handle<Container> hparent, Handle<String> htext, Handle<String> hlink):
Container(hparent),
Link(hlink),
Text(htext)
{}


//========
// Access
//========

SIZE_T HyperLink::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("<a");
usize+=WriteControlAttributes(hstream);
usize+=writer.Print(" href=\"");
usize+=writer.Print(Link);
usize+=writer.Print("\">");
usize+=WriteContent(hstream);
usize+=writer.Print(Text);
usize+=writer.Print("</a>");
usize+=WriteBreaks(hstream);
return usize;
}

}}
