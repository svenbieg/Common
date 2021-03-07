//===============
// TextBlock.cpp
//===============

#include "pch.h"


//=======
// Using
//=======

#include "Container.h"
#include "TextBlock.h"

using namespace Graphics;


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//==================
// Con-/Destructors
//==================

TextBlock::TextBlock(Handle<String> htext):
TextBlock(Container::Current, htext)
{}

TextBlock::TextBlock(Container* pparent, Handle<String> htext):
Window(pparent),
Control(pparent),
Color(this, DefaultColor),
Text(this, htext)
{
Color.Changed.Add(this, &TextBlock::OnColorChanged);
Text.Changed.Add(this, &TextBlock::OnTextChanged);
}


//========
// Common
//========

BOOL TextBlock::Render(Handle<DeviceContext> hdc)
{
BOOL brendered=GetFlag(uFlags, WindowFlags::Repaint);
Control::Render(hdc);
if(!Text)
	return brendered;
hdc->Font=Font;
hdc->DrawText(Text, Color);
return brendered;
}


//==================
// Common Protected
//==================

Graphics::SIZE TextBlock::GetMinSizeInternal(Handle<DeviceContext> hdc)
{
SIZE minsize=Control::GetMinSizeInternal(hdc);
SIZE textsize=hdc->MeasureText(Font, Text);
minsize.Width+=textsize.Width;
minsize.Height+=textsize.Height;
return minsize;
}


//================
// Common Private
//================

VOID TextBlock::OnColorChanged(COLOR c)
{
Invalidate();
}

VOID TextBlock::OnTextChanged(Handle<String> htext)
{
if(pParent)
	pParent->InvalidateArrangement();
Invalidate();
}

}}