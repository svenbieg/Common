//============
// Canvas.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Canvas.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Canvas::Canvas(Container* pparent):
Window(pparent),
Control(pparent),
Container(pparent)
{}


//========
// Common
//========

VOID Canvas::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	RECT rcc=hchild->GetRect();
	SIZE minsize=hchild->GetMinSize(hdc);
	rcc.Right=MIN(rcc.Right, rcc.Left+(INT)minsize.Width);
	rcc.Bottom=MIN(rcc.Bottom, rcc.Top+(INT)minsize.Height);
	if(rcc.Left<rc.Left)
		{
		INT i=rc.Left-rcc.Left;
		rcc.Left+=i;
		rcc.Right+=i;
		}
	if(rcc.Top<rc.Top)
		{
		INT i=rc.Top-rcc.Top;
		rcc.Top+=i;
		rcc.Bottom+=i;
		}
	if(rcc.Right>rc.Right)
		{
		INT i=rc.Right-rcc.Right;
		rcc.Left-=i;
		rcc.Right-=i;
		}
	if(rcc.Bottom>rc.Bottom)
		{
		INT i=rc.Bottom-rcc.Bottom;
		rcc.Top+=i;
		rcc.Bottom+=i;
		}
	rcc.Left=MAX(rcc.Left, rc.Left);
	rcc.Top=MAX(rcc.Top, rc.Top);
	rcc.Right=MIN(rcc.Right, rc.Right);
	rcc.Bottom=MIN(rcc.Bottom, rc.Bottom);
	hchild->Move(rcc);
	}
}


//==================
// Common Protected
//==================

Graphics::SIZE Canvas::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Control::GetMinSizeInternal(hdc);
INT iwidth=0;
INT iheight=0;
for(auto hit=Children->First(); hit->HasCurrent(); hit->MoveNext())
	{
	auto hchild=hit->GetCurrent();
	if(!hchild->Visible.GetInternal())
		continue;
	RECT const& rcc=hchild->GetRect();
	SIZE msc=hchild->GetMinSize(hdc);
	iwidth=MAX(iwidth, rcc.Left+(INT)msc.Width);
	iheight=MAX(iheight, rcc.Top+(INT)msc.Height);
	}
minsize.Width+=iwidth;
minsize.Height+=iheight;
return minsize;
}

}}