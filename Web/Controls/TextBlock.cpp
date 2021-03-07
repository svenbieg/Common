//===============
// TextBlock.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "TextBlock.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TextBlock::TextBlock(Handle<Container> hparent, Handle<String> htext):
Control(hparent),
Text(htext)
{}


//========
// Common
//========

SIZE_T TextBlock::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print(Text);
usize+=WriteBreaks(hstream);
return usize;
}

}}
