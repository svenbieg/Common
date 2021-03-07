//============
// Window.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include "Core/Controls/Container.h"
#include "Window.h"


//===========
// Namespace
//===========

namespace Core {


//========
// Common
//========

VOID Window::Invalidate()
{
SetFlag(uFlags, WindowFlags::Repaint);
}

VOID Window::InvalidateArrangement()
{
if(GetFlag(uFlags, WindowFlags::Rearrange))
	return;
SetFlag(uFlags, WindowFlags::Rearrange);
if(pParent)
	pParent->InvalidateArrangement();
}

VOID Window::Move(RECT const& rc)
{
RECT rcnew(rc);
Moving(this, rcnew);
RECT rcold(rcRect);
rcRect=rc;
if(CompareMemory(&rcold, &rcnew, sizeof(RECT))!=0)
	{
	Moved(this, rcRect);
	Invalidate();
	}
}

VOID Window::Rearrange(Handle<DeviceContext> hdc, RECT& rc)
{
ClearFlag(uFlags, WindowFlags::Rearrange);
}

BOOL Window::Render(Handle<DeviceContext> hdc)
{
if(GetFlag(uFlags, WindowFlags::Rearrange))
	{
	Graphics::RECT const& rc=hdc->Clip;
	RECT rcc(0, 0, rc.Right-rc.Left, rc.Bottom-rc.Top);
	this->Rearrange(hdc, rcc);
	ClearFlag(uFlags, WindowFlags::Rearrange);
	}
if(!GetFlag(uFlags, WindowFlags::Repaint))
	return false;
ClearFlag(uFlags, WindowFlags::Repaint);
return false;
}


//============================
// Con-/Destructors Protected
//============================

Window::Window(Container* pparent):
// Common
Visible(this, true),
// Common Protected
pParent(pparent),
rcRect({ 0, 0, 0, 0 }),
uFlags(0)
{
Visible.Changed.Add(this, &Window::OnVisibleChanged);
Visible.Reading.Add(this, &Window::OnVisibleReading);
}


//================
// Common Private
//================

VOID Window::OnVisibleChanged(BOOL bvisible)
{
if(pParent)
	{
	pParent->InvalidateArrangement();
	return;
	}
Invalidate();
}

VOID Window::OnVisibleReading(BOOL& bvisible)
{
if(!bvisible)
	return;
if(pParent)
	bvisible=pParent->Visible;
}

}