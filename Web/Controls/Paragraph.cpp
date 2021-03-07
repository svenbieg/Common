//===============
// Paragraph.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Paragraph.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Paragraph::Paragraph(Handle<Container> hparent):
Container(hparent)
{}


//========
// Common
//========

SIZE_T Paragraph::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("\r\n<p>\r\n");
usize+=WriteContent(hstream);
usize+=writer.Print("\r\n</p>\r\n");
usize+=WriteBreaks(hstream);
return usize;
}

}}
