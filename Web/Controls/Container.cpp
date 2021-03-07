//===============
// Container.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Container.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//========
// Access
//========

Handle<Container> Container::Current;

SIZE_T Container::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=WriteContent(hstream);
usize+=WriteBreaks(hstream);
return usize;
}


//============================
// Con-/Destructors Protected
//============================

Container::Container(Handle<Container> hparent, Handle<String> hclass, Handle<String> hstyle):
Control(hparent, hclass, hstyle)
{
Children=new ControlCluster();
Current=this;
}


//==================
// Common Protected
//==================

SIZE_T Container::WriteContent(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	Handle<Control> hchild=hit->GetCurrent();
	usize+=hchild->WriteToStream(hstream);
	}
return usize;
}

}}
