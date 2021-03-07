//==========
// Window.h
//==========

#pragma once


//=======
// Using
//=======

#include "Graphics/DeviceContext.h"


//======================
// Forward-Declarations
//======================

namespace Core
{
namespace Controls
	{
	class Container;
	}
}


//===========
// Namespace
//===========

namespace Core {


//========
// Window
//========

class Window: public virtual Object
{
public:
	// Using
	using Container=Core::Controls::Container;
	using CoreWindow=Window;
	using DeviceContext=Graphics::DeviceContext;
	using RECT=Graphics::RECT;
	using SIZE=Graphics::SIZE;

	// Common
	Handle<Graphics::Font> Font;
	RECT const& GetRect() { return rcRect; }
	SIZE GetSize() { return { (UINT)(rcRect.Right-rcRect.Left), (UINT)(rcRect.Bottom-rcRect.Top) }; }
	virtual VOID Invalidate();
	VOID InvalidateArrangement();
	VOID Move(RECT const& Rect);
	Event<Window, RECT const&> Moved;
	Event<Window, RECT&> Moving;
	virtual VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect);
	virtual BOOL Render(Handle<DeviceContext> DeviceContext);
	Property<Window, BOOL> Visible;

protected:
	// Flags
	enum class WindowFlags: UINT
		{
		Repaint=1,
		Rearrange=Repaint<<1,
		Last=Rearrange
		};

	// Con-/Destructors
	Window(Container* Parent);

	// Common
	Container* pParent;
	RECT rcRect;
	UINT uFlags;

private:
	// Common
	VOID OnVisibleChanged(BOOL Visible);
	VOID OnVisibleReading(BOOL& Visible);
};

}