//=============
// IpAddress.h
//=============

#pragma once


//===========
// Namespace
//===========

namespace Network {


//============
// IP-Address
//============

class IpAddress: public TypedVariable<UINT>
{
public:
	// Con-/Destrucotrs
	IpAddress(UINT Address=0): IpAddress(nullptr, Address) {}
	IpAddress(BYTE A1, BYTE A2, BYTE A3, BYTE A4): IpAddress(nullptr, A1, A2, A3, A4) {}
	IpAddress(Handle<String> Name, UINT Address=0);
	IpAddress(Handle<String> Name, BYTE A1, BYTE A2, BYTE A3, BYTE A4);

	// Access
	operator UINT() { return Get(); }
	Handle<String> ToString()override;

	// Modification
	BOOL FromString(Handle<String> Address)override;

	// Common
	static UINT From(BYTE A1, BYTE A2, BYTE A3, BYTE A4);
};

}


//===================
// Handle IP-Address
//===================

template <>
class Handle<Network::IpAddress>
{
private:
	// Using
	using _ip_t=Network::IpAddress;

public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(_ip_t* Object) { HandleCreate(&pObject, Object); }
	Handle(Handle<_ip_t> const& Handle) { HandleCreate(&pObject, Handle.pObject); }
	Handle(Handle<_ip_t>&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleDestroy(pObject); }

	// Access
	operator UINT()const { return VariableGet<_ip_t, UINT>(pObject, 0); }
	_ip_t* operator->()const { return pObject; }

	// Comparison
	BOOL operator==(UINT Value)const { return VariableEqual(pObject, Value); }
	BOOL operator!=(UINT Value)const { return !VariableEqual(pObject, Value); }

	// Assignment
	Handle& operator=(UINT Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(_ip_t* Object) { HandleAssign(&pObject, Object); return *this; }

private:
	// Common
	_ip_t* pObject;
};
