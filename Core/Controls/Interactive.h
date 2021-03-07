//===============
// Interactive.h
//===============

#pragma once


//=======
// Using
//=======

#include "Control.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//=============
// Interactive
//=============

class Interactive: public virtual Control
{
public:
	// Using
	using CoreInteractive=Interactive;

	// Con-/Destructors
	Interactive();
	Interactive(Container* Parent);

	// Common
	Event<Interactive, POINT const&> Clicked;
	Event<Interactive, POINT const&> ContextMenu;
	VOID DoClick(POINT const& Point)override;
	VOID DoDoubleClick(POINT const& Point)override;
	VOID DoKey(VirtualKey Key, BOOL Down)override;
	VOID DoLButton(POINT const& Point, BOOL Down)override;
	VOID DoMovePointer(POINT const& Point)override;
	VOID DoPointerWheel(POINT const& Point, INT Delta)override;
	VOID DoRButton(POINT const& Point, BOOL Down)override;
	VOID DoContextMenu(POINT const& Point)override;
	Event<Interactive, POINT const&> DoubleClicked;
	Event<Interactive> Focused;
	Event<Interactive> FocusLost;
	Event<Interactive, VirtualKey> KeyDown;
	Event<Interactive, VirtualKey> KeyUp;
	Event<Interactive, POINT const&> LButtonDown;
	Event<Interactive, POINT const&> LButtonUp;
	Event<Interactive> PointerEntered;
	Event<Interactive> PointerLeft;
	Event<Interactive, POINT const&> PointerMoved;
	Event<Interactive, POINT const&, INT> PointerWheel;
	Event<Interactive, POINT const&> RButtonDown;
	Event<Interactive, POINT const&> RButtonUp;
	VOID SetFocus();

protected:
	// Flags
	enum class InteractiveFlags: UINT
		{
		LButtonDown=(UINT)ControlFlags::Last<<1,
		RButtonDown=(UINT)ControlFlags::Last<<2
		};

private:
	// Common
	VOID OnLButtonDown(Handle<Interactive> Sender, POINT const& Point);
	VOID OnLButtonUp(Handle<Interactive> Sender, POINT const& Point);
	VOID OnPointerMoved(Handle<Interactive> Sender, POINT const& Point);
	VOID OnRButtonDown(Handle<Interactive> Sender, POINT const& Point);
	VOID OnRButtonUp(Handle<Interactive> Sender, POINT const& Point);
	static Interactive* pFocus;
	static Interactive* pPointerFocus;
};

}}