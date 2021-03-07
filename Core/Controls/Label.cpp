//===========
// Label.cpp
//===========

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "Label.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//==================
// Con-/Destructors
//==================

Label::Label(Handle<Variable> hvar):
Label(Container::Current, hvar)
{}

Label::Label(Container* pparent, Handle<Variable> hvar):
Window(pparent),
Control(pparent),
Color(this, DefaultColor),
hVariable(hvar)
{
Color.Changed.Add(this, &Label::OnColorChanged);
hVariable->Changed.Add(this, &Label::OnVariableChanged);
}

Label::~Label()
{
hVariable->Changed.Remove(this);
}


//========
// Common
//========

BOOL Label::Render(Handle<DeviceContext> hdc)
{
BOOL brendered=GetFlag(uFlags, WindowFlags::Repaint);
Control::Render(hdc);
auto htext=hVariable->ToString();
if(!htext)
	return brendered;
hdc->Font=Font;
hdc->DrawText(htext, Color);
return brendered;
}


//==================
// Common Protected
//==================

Graphics::SIZE Label::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Control::GetMinSizeInternal(hdc);
auto htext=hVariable->ToString();
SIZE textsize=hdc->MeasureText(Font, htext);
minsize.Width+=textsize.Width;
minsize.Height+=textsize.Height;
return minsize;
}


//================
// Common Private
//================

VOID Label::OnColorChanged(COLOR c)
{
Invalidate();
}

VOID Label::OnVariableChanged(Handle<Variable> hvar)
{
if(pParent)
	pParent->InvalidateArrangement();
Invalidate();
}

}}