//=================
// Interactive.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Interactive.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Interactive::Interactive():
Interactive(Container::Current)
{}

Interactive::Interactive(Container* pparent):
Window(pparent),
Control(pparent)
{
LButtonDown.Add(this, &Interactive::OnLButtonDown);
LButtonUp.Add(this, &Interactive::OnLButtonUp);
PointerMoved.Add(this, &Interactive::OnPointerMoved);
RButtonDown.Add(this, &Interactive::OnRButtonDown);
RButtonUp.Add(this, &Interactive::OnRButtonUp);
}


//========
// Common
//========

VOID Interactive::DoClick(POINT const& pt)
{
this->SetFocus();
Clicked(this, pt);
}

VOID Interactive::DoDoubleClick(POINT const& pt)
{
this->SetFocus();
DoubleClicked(this, pt);
}

VOID Interactive::DoKey(VirtualKey key, BOOL b)
{
b? KeyDown(this, key): KeyUp(this, key);
}

VOID Interactive::DoLButton(POINT const& pt, BOOL b)
{
b? LButtonDown(this, pt): LButtonUp(this, pt);
}

VOID Interactive::DoMovePointer(POINT const& pt)
{
PointerMoved(this, pt);
}

VOID Interactive::DoPointerWheel(POINT const& pt, INT idelta)
{
this->SetFocus();
PointerWheel(this, pt, idelta);
}

VOID Interactive::DoRButton(POINT const& pt, BOOL b)
{
b? RButtonDown(this, pt): RButtonUp(this, pt);
}

VOID Interactive::DoContextMenu(POINT const& pt)
{
ContextMenu(this, pt);
}

VOID Interactive::SetFocus()
{
if(pFocus!=this)
	{
	Handle<Interactive> hold=pFocus;
	pFocus=this;
	if(hold)
		hold->FocusLost(hold);
	Focused(this);
	}
}


//================
// Common Private
//================

VOID Interactive::OnLButtonDown(Handle<Interactive> hsender, POINT const& pt)
{
SetFlag(uFlags, InteractiveFlags::LButtonDown);
}

VOID Interactive::OnLButtonUp(Handle<Interactive> hsender, POINT const& pt)
{
BOOL bclick=GetFlag(uFlags, InteractiveFlags::LButtonDown);
ClearFlag(uFlags, InteractiveFlags::LButtonDown);
if(bclick)
	Clicked(this, pt);
}

VOID Interactive::OnPointerMoved(Handle<Interactive> hsender, POINT const& pt)
{
if(pPointerFocus!=this)
	{
	Handle<Interactive> hold=pPointerFocus;
	pPointerFocus=this;
	if(hold)
		hold->PointerLeft(hold);
	PointerEntered(this);
	}
}

VOID Interactive::OnRButtonDown(Handle<Interactive> hsender, POINT const& pt)
{
SetFlag(uFlags, InteractiveFlags::RButtonDown);
}

VOID Interactive::OnRButtonUp(Handle<Interactive> hsender, POINT const& pt)
{
BOOL bclick=GetFlag(uFlags, InteractiveFlags::RButtonDown);
ClearFlag(uFlags, InteractiveFlags::RButtonDown);
if(bclick)
	ContextMenu(this, pt);
}

Interactive* Interactive::pFocus=nullptr;
Interactive* Interactive::pPointerFocus=nullptr;

}}