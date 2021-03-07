//=================
// Interactive.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamWriter.h"
#include "Web/WebPage.h"
#include "Interactive.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Web {
	namespace Controls {


//========
// Common
//========

SIZE_T Interactive::WriteUpdateToStream(Handle<OutputStream> hstream, UINT ulasttime)
{
if(uTimeChanged<=ulasttime)
	return 0;
if(!hVariable)
	return 0;
Handle<String> hid=hVariable->GetName();
if(!hid)
	return 0;
Handle<String> hvalue=hVariable->ToString();
if(!hvalue)
	return 0;
StreamWriter writer(hstream);
SIZE_T usize=0;
usize+=writer.Print(hid);
usize+=writer.Print("=");
usize+=writer.Print(hvalue);
usize+=writer.Print(";");
return usize;
}


//============================
// Con-/Destructors Protected
//============================

Interactive::Interactive(Handle<Container> hparent, Handle<Variable> hvar, Handle<String> hclass, Handle<String> hstyle):
Control(hparent, hclass, hstyle),
hVariable(hvar),
uTimeChanged(0)
{
if(!hVariable)
	return;
WebPage::Current->Controls->Add(hVariable->GetName(), this);
hVariable->Changed.Add(this, &Interactive::OnVariableChanged);
}

Interactive::~Interactive()
{
if(!hVariable)
	return;
WebPage::Current->Controls->Remove(hVariable->GetName());
hVariable->Changed.Remove(this);
}


//================
// Common Private
//================

VOID Interactive::OnVariableChanged(Handle<Variable> hvar)
{
uTimeChanged=GetTickCount();
}

}}