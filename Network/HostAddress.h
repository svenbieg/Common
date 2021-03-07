//===============
// HostAddress.h
//===============

#pragma once


//=======
// Using
//=======

#include "IpAddress.h"


//===========
// Namespace
//===========

namespace Network {


//==============
// Host-Address
//==============

class HostAddress: public Object
{
public:
	// Con-/Destructor
	HostAddress();
	HostAddress(Handle<IpAddress> Address, WORD Port);
	HostAddress(LPCSTR Address);
	HostAddress(HostAddress const& Address): Ip(Address.Ip), Port(Address.Port) {}

	// Assignment
	HostAddress& operator=(UINT Value) { Ip->Set(Value); Port=0; return *this; }

	// Comparison
	BOOL operator==(HostAddress const& Address)const { return CompareMemory(this, &Address, sizeof(HostAddress)==0); }
	BOOL operator!=(HostAddress const& Address)const { return CompareMemory(this, &Address, sizeof(HostAddress)!=0); }
	BOOL operator>=(HostAddress const& Address)const
		{
		if(Ip->Get()<Address.Ip->Get())
			return false;
		if(Ip->Get()>Address.Ip->Get())
			return true;
		return Port>=Address.Port;
		}
	BOOL operator<=(HostAddress const& Address)const
		{
		if(Ip->Get()>Address.Ip->Get())
			return false;
		if(Ip->Get()<Address.Ip->Get())
			return true;
		return Port<=Address.Port;
		}
	BOOL operator>(HostAddress const& Address)const
		{
		if(Ip->Get()<Address.Ip->Get())
			return false;
		if(Ip->Get()>Address.Ip->Get())
			return true;
		return Port>Address.Port;
		}
	BOOL operator<(HostAddress const& Address)const
		{
		if(Ip->Get()>Address.Ip->Get())
			return false;
		if(Ip->Get()<Address.Ip->Get())
			return true;
		return Port<Address.Port;
		}

	// Common
	Handle<IpAddress> Ip;
	WORD Port;
	Handle<String> ToString();
};

}
