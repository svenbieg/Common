//============
// Button.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Button.h"
#include "Container.h"


//========
// Common
//========

namespace Core {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Button::Button(Container* pparent):
Window(pparent),
Control(pparent),
Interactive(pparent),
Down(this, false),
Text(this)
{
MinSize.Width=80;
Padding={ 20, 4, 20, 4 };
Text.Changed.Add(this, &Button::OnTextChanged);
}


//==================
// Common Protected
//==================

Graphics::SIZE Button::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Interactive::GetMinSizeInternal(hdc);
SIZE textsize=hdc->MeasureText(Font, Text);
minsize.Width+=textsize.Width;
minsize.Height+=textsize.Height;
return minsize;
}


//================
// Common Private
//================

VOID Button::OnTextChanged(Handle<String> htext)
{
if(pParent)
	pParent->InvalidateArrangement();
}

}}