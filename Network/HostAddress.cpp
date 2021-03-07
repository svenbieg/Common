//=================
// HostAddress.cpp
//=================

#include "pch.h"


//=======
// Using
//=======

#include "HostAddress.h"


//===========
// Namespace
//===========

namespace Network {


//==================
// Con-/Destructors
//==================

HostAddress::HostAddress():
Ip(new IpAddress()),
Port(0)
{}

HostAddress::HostAddress(Handle<IpAddress> hip, WORD uport):
Ip(hip),
Port(uport)
{
if(!Ip)
	Ip=new IpAddress();
}

HostAddress::HostAddress(LPCSTR paddr):
Ip(new IpAddress()),
Port(0)
{
UINT u1=0;
UINT u2=0;
UINT u3=0;
UINT u4=0;
UINT uport=0;
if(StringScan(paddr, "%u.%u.%u.%u:%u", &u1, &u2, &u3, &u4, &uport)==5)
	{
	UINT uip;
	BYTE* pip=(BYTE*)&uip;
	pip[0]=(BYTE)u1;
	pip[1]=(BYTE)u2;
	pip[2]=(BYTE)u3;
	pip[3]=(BYTE)u4;
	Ip->Set(uip);
	Port=(WORD)uport;
	}
}


//========
// Access
//========

Handle<String> HostAddress::ToString()
{
UINT uip=Ip->Get();
BYTE* pip=(BYTE*)&uip;
UINT u1=pip[0];
UINT u2=pip[1];
UINT u3=pip[2];
UINT u4=pip[3];
return new String("%u.%u.%u.%u:%hu", u1, u2, u3, u4, Port);
}

}
