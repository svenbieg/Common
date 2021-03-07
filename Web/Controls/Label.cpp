//===========
// Label.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Label.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Label::Label(Handle<Container> hparent, Handle<Variable> hvar):
Interactive(hparent, hvar)
{}


//========
// Common
//========

SIZE_T Label::WriteToStream(Handle<OutputStream> hstream)
{
StreamWriter writer(hstream);
auto hid=hVariable->GetName();
SIZE_T usize=0;
usize+=writer.Print("<label id=\"");
usize+=writer.Print(hid);
usize+=writer.Print("\"");
usize+=WriteControlAttributes(hstream);
usize+=writer.Print(">");
if(hVariable)
	usize+=writer.Print(hVariable->ToString());
usize+=writer.Print("</label>");
usize+=WriteBreaks(hstream);
return usize;
}

}}
