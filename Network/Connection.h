//==============
// Connection.h
//==============

#pragma once


//=======
// Using
//=======

#include "IpAddress.h"


//===========
// Namespace
//===========

namespace Network {


//============
// Connection
//============

class Connection: public Object
{
public:
	// Common
	static Handle<Connection> Current;
	Handle<String> HostName;
	Handle<IpAddress> StationIp;
	Handle<IpAddress> StationGateway;
	Handle<IpAddress> StationSubnet;

protected:
	// Con-/Destructors
	Connection();
};

}