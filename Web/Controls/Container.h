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

namespace Web {
	namespace Controls {


//===========
// Container
//===========

class Container: public Control
{
private:
	// Using
	using ControlCluster=Collections::Cluster<Handle<Control>>;

public:
	// Common
	Handle<ControlCluster> Children;
	static Handle<Container> Current;
	virtual VOID Select() { Current=this; }
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;

protected:
	// Con-/Destructors
	Container(Handle<Container> Parent=nullptr, Handle<String> Class=nullptr, Handle<String> Style=nullptr);

	// Common
	SIZE_T WriteContent(Handle<OutputStream> Stream);
};

}}
