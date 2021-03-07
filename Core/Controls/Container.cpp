//===============
// Container.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Container::Container(Container* pparent):
Window(pparent),
Control(pparent)
{
Current=this;
Children=new ControlList();
Visible.Changed.Add(this, &Container::OnVisibleChanged);
}

Container::~Container()
{
if(Current==this)
	Current=nullptr;
}


//========
// Common
//========

Container* Container::Current=nullptr;

BOOL Container::Render(Handle<DeviceContext> hdc)
{
Control::Render(hdc);
Graphics::POINT pt=hdc->Offset;
Graphics::RECT rc=hdc->Clip;
BOOL brendered=false;
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	RECT rcc=hchild->GetRect();
	rcc.Left+=pt.Left;
	rcc.Top+=pt.Top;
	rcc.Right+=pt.Left;
	rcc.Bottom+=pt.Top;
	if(rcc.Left>=rc.Right)
		continue;
	if(rcc.Top>=rc.Bottom)
		continue;
	if(rcc.Right<=rc.Left)
		continue;
	if(rcc.Bottom<=rc.Top)
		continue;
	auto ptc=Graphics::POINT({ rcc.Left, rcc.Top });
	rcc.Left=MAX(rcc.Left, rc.Left);
	rcc.Top=MAX(rcc.Top, rc.Top);
	rcc.Right=MIN(rcc.Right, rc.Right);
	rcc.Bottom=MIN(rcc.Bottom, rc.Bottom);
	hdc->Offset=ptc;
	hdc->Clip=rcc;
	if(hchild->Render(hdc))
		brendered=true;
	}
return brendered;
}


//================
// Common Private
//================

VOID Container::OnVisibleChanged(BOOL bvisible)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild)
		continue;
	if(hchild->Visible.GetInternal())
		hchild->Visible.Changed(this, bvisible);
	}
}

}}