//============
// Update.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Update.h"
#include "WebPage.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {


//==================
// Con-/Destructors
//==================

Update::Update(Handle<WebPage> hwebpage, UINT utime):
hWebPage(hwebpage),
uTime(utime)
{
uTimeNow=GetTickCount();
}


//========
// Common
//========

SIZE_T Update::WriteToStream(Handle<OutputStream> hstream)
{
SIZE_T usize=0;
StreamWriter writer(hstream);
usize+=writer.Print("time=%u;", uTimeNow);
for(auto hit=hWebPage->Controls->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hcontrol=hit->GetCurrentItem();
	usize+=hcontrol->WriteUpdateToStream(hstream, uTime);
	}
return usize;
}

}