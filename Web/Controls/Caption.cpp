//=============
// Caption.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include <stdarg.h>
#include "Storage/Streams/StreamWriter.h"
#include "Caption.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Caption::Caption(Handle<Container> hparent, Handle<String> htext, UINT ulevel):
Control(hparent),
Text(htext),
uLevel(ulevel)
{
StringCopy(pTag, 6, "<<h1>");
uLevel=MAX(uLevel, 1);
uLevel=MIN(uLevel, 9);
pTag[3]=(CHAR)(uLevel+'0');
}


//========
// Access
//========

SIZE_T Caption::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("\r\n");
pTag[1]='<';
usize+=writer.Print(&pTag[1]);
usize+=writer.Print(Text);
pTag[1]='/';
usize+=writer.Print(pTag);
usize+=WriteBreaks(hstream);
usize+=writer.Print("\r\n");
return usize;
}

}}
