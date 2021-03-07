//==========
// Form.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Form.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//========
// Access
//========

SIZE_T Form::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("<form>\r\n");
usize+=WriteContent(hstream);
usize+=writer.Print("</form>\r\n");
return usize;
}

}}
