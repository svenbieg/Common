//===================
// ColorVariable.cpp
//===================

#include "pch.h"


//=======
// Using
//=======

#include "ColorVariable.h"


//===========
// Namespace
//===========

namespace Graphics {


//==================
// Con-/Destructors
//==================

ColorVariable::ColorVariable(COLOR c):
ColorVariable(nullptr, c)
{}

ColorVariable::ColorVariable(Colors c):
ColorVariable(nullptr, (COLOR)c)
{}

ColorVariable::ColorVariable(Handle<String> hname, COLOR c):
TypedVariable(hname, c),
Alpha(this, c.GetAlpha()),
Blue(this, c.GetBlue()),
Green(this, c.GetGreen()),
Red(this, c.GetRed()),
uFlags(ColorFlags::None)
{
Alpha.Changed.Add(this, &ColorVariable::OnAlphaChanged);
Blue.Changed.Add(this, &ColorVariable::OnBlueChanged);
Green.Changed.Add(this, &ColorVariable::OnGreenChanged);
Red.Changed.Add(this, &ColorVariable::OnRedChanged);
}


//========
// Access
//========

Handle<String> ColorVariable::ToString()
{
ScopedLock lock(cCriticalSection);
return tValue.ToString();
}


//==============
// Modification
//==============

VOID ColorVariable::Set(COLOR const& c)
{
SetFlag(uFlags, ColorFlags::Setting);
ScopedLock lock(cCriticalSection);
if(tValue==c)
	return;
tValue=c;
lock.Release();
Red=c.GetRed();
Green=c.GetGreen();
Blue=c.GetBlue();
Alpha=c.GetAlpha();
Changed(this);
ClearFlag(uFlags, ColorFlags::Setting);
}


//================
// Common Private
//================

VOID ColorVariable::OnAlphaChanged(BYTE a)
{
if(GetFlag(uFlags, ColorFlags::Setting))
	return;
ScopedLock lock(cCriticalSection);
if(tValue.GetAlpha()==a)
	return;
tValue.SetAlpha(a);
lock.Release();
Changed(this);
}

VOID ColorVariable::OnBlueChanged(BYTE b)
{
if(GetFlag(uFlags, ColorFlags::Setting))
	return;
ScopedLock lock(cCriticalSection);
if(tValue.GetBlue()==b)
	return;
tValue.SetBlue(b);
lock.Release();
Changed(this);
}

VOID ColorVariable::OnGreenChanged(BYTE g)
{
if(GetFlag(uFlags, ColorFlags::Setting))
	return;
ScopedLock lock(cCriticalSection);
if(tValue.GetGreen()==g)
	return;
tValue.SetGreen(g);
lock.Release();
Changed(this);
}

VOID ColorVariable::OnRedChanged(BYTE r)
{
if(GetFlag(uFlags, ColorFlags::Setting))
	return;
ScopedLock lock(cCriticalSection);
if(tValue.GetRed()==r)
	return;
tValue.SetRed(r);
lock.Release();
Changed(this);
}

}
