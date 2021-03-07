//=============
// Container.h
//=============

#pragma once


//=======
// Using
//=======

#include "Collections/Cluster.h"
#include "Control.h"


//===========
// Namespace
//===========

namespace Core {
	namespace Controls {


//===========
// Container
//===========

class Container: public virtual Control
{
public:
	// Using
	using ControlList=Collections::Cluster<Handle<Control>>;
	using CoreContainer=Container;

	// Con-/Destructors
	Container(Container* Parent);
	~Container();

	// Common
	Handle<ControlList> Children;
	static Container* Current;
	virtual BOOL Render(Handle<DeviceContext> DeviceContext)override;
	VOID Select() { Current=this; }

protected:
	// Flags
	enum class ContainerFlags: UINT
		{
		Last=(UINT)ControlFlags::Last
		};

private:
	// Common
	VOID OnVisibleChanged(BOOL Visible);
};

}}