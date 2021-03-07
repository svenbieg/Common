//=============
// Control.cpp
//=============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Control::Control():
Control(Container::Current)
{}

Control::Control(Container* pparent):
Window(pparent),
// Common
Column(this, 0),
Line(this, 0),
Margin(this, { 0, 0, 0, 0}),
MaxSize({ 0, 0}),
MinSize({ 0, 0}),
Padding(this, { 0, 0, 0, 0})
{
if(pParent)
	{
	pParent->Children->Append(this);
	Font=pParent->Font;
	}
Margin.Changed.Add(this, &Control::OnPaddingChanged);
Padding.Changed.Add(this, &Control::OnPaddingChanged);
}


//========
// Common
//========

COLOR Control::DefaultColor=Colors::Black;

Graphics::SIZE Control::GetMinSize(Handle<DeviceContext> hdc)
{
SIZE minsize=this->GetMinSizeInternal(hdc);
minsize.Width=MAX(minsize.Width, MinSize.Width);
minsize.Height=MAX(minsize.Height, MinSize.Height);
return minsize;
}

VOID Control::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
Window::Rearrange(hdc, rc);
RECT const& padding=Padding.GetInternal();
rc.Left+=padding.Left;
rc.Top+=padding.Top;
rc.Right-=padding.Right;
rc.Bottom-=padding.Bottom;
}

BOOL Control::Render(Handle<DeviceContext> hdc)
{
if(!Window::Render(hdc))
	return false;
RECT const& padding=Padding.GetInternal();
POINT& pt=hdc->Offset;
pt.Left+=padding.Left;
pt.Top+=padding.Top;
RECT& rc=hdc->Clip;
rc.Left+=padding.Left;
rc.Top+=padding.Top;
rc.Right-=padding.Right;
rc.Bottom-=padding.Bottom;
return false;
}


//==================
// Common Protected
//==================

Graphics::SIZE Control::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize;
RECT const& padding=Padding.GetInternal();
minsize.Width=padding.Left+padding.Right;
minsize.Height=padding.Top+padding.Bottom;
return minsize;
}


//================
// Common Private
//================

VOID Control::OnPaddingChanged(RECT padding)
{
if(pParent)
	pParent->InvalidateArrangement();
}

}}