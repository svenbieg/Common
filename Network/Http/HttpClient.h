//==============
// HttpClient.h
//==============

#pragma once


//=======
// Using
//=======

#include "Network/Tcp/TcpClient.h"
#include "HttpConnection.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//========
// Status
//========

enum class HttpClientStatus
{
Closed,
Connecting,
Connected
};


//=============
// Http-Client
//=============

class HttpClient: public Object
{
private:
	// Using
	using TcpClient=Network::Tcp::TcpClient;
	using TcpConnection=Network::Tcp::TcpConnection;

public:
	// Con-/Destructors
	HttpClient();

	// Common
	VOID Close();
	Event<HttpClient> Closed;
	VOID Connect(Handle<String> Host, WORD Port=80);
	Event<HttpClient, Handle<HttpConnection>> Connected;
	Event<HttpClient> ConnectionFailed;

private:
	// Common
	VOID CloseInternal();
	VOID OnTcpClientClosed(Handle<TcpClient> TcpClient);
	VOID OnTcpClientConnected(Handle<TcpClient> TcpClient, Handle<TcpConnection> TcpConnection);
	Handle<TcpClient> hTcpClient;
	HttpClientStatus uStatus;
};

}}