//================
// Connection.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "Connection.h"


//===========
// Namespace
//===========

namespace Network {


//========
// Common
//========

Handle<Connection> Connection::Current;


//============================
// Con-/Destructors Protected
//============================

Connection::Connection()
{
Current=this;
StationIp=new IpAddress("NetworkStationIp");
StationGateway=new IpAddress("NetworkStationGateway");
StationSubnet=new IpAddress("NetworkStationSubnet", 255, 255, 255, 0);
}

}