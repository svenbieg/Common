//===========
// Control.h
//===========

#pragma once


//=======
// Using
//=======

#include "Core/Window.h"
#include "Devices/Input/VirtualKey.h"
#include "Graphics/Font.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//=========
// Control
//=========

class Control: public virtual Window
{
public:
	// Con-/Destructors
	Control();
	Control(Container* Parent);

	// Using
	using CoreControl=Control;
	using COLOR=Graphics::COLOR;
	using POINT=Graphics::POINT;
	using VirtualKey=Devices::Input::VirtualKey;

	// Common
	Property<Control, UINT> Column;
	static COLOR DefaultColor;
	virtual VOID DoClick(POINT const& Point) {}
	virtual VOID DoDoubleClick(POINT const& Point) {}
	virtual VOID DoKey(VirtualKey Key, BOOL Down) {}
	virtual VOID DoLButton(POINT const& Point, BOOL Down) {}
	virtual VOID DoMovePointer(POINT const& Point) {}
	virtual VOID DoPointerWheel(POINT const& Point, INT Delta) {}
	virtual VOID DoRButton(POINT const& Point, BOOL Down) {}
	virtual VOID DoContextMenu(POINT const& Point) {}
	SIZE GetMinSize(Handle<DeviceContext> DeviceContext);
	Property<Control, UINT> Line;
	Property<Control, RECT> Margin;
	SIZE MaxSize;
	SIZE MinSize;
	Property<Control, RECT> Padding;
	virtual VOID Rearrange(Handle<DeviceContext> DeviceContext, RECT& Rect)override;
	virtual BOOL Render(Handle<DeviceContext> DeviceContext)override;

protected:
	// Flags
	enum class ControlFlags: UINT
		{
		Last=(UINT)WindowFlags::Last
		};

	// Common
	virtual SIZE GetMinSizeInternal(Handle<DeviceContext> DeviceContext);

private:
	// Common
	VOID OnPaddingChanged(RECT Padding);
};

}}