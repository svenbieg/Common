//================
// HttpClient.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "HttpClient.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//==================
// Con-/Destructors
//==================

HttpClient::HttpClient():
uStatus(HttpClientStatus::Closed)
{}


//========
// Common
//========

VOID HttpClient::Close()
{
if(uStatus==HttpClientStatus::Closed)
	return;
hTcpClient=nullptr;
uStatus=HttpClientStatus::Closed;
Closed(this);
}

VOID HttpClient::Connect(Handle<String> hhost, WORD uport)
{
ASSERT(hhost);
Close();
hTcpClient=new TcpClient();
hTcpClient->Closed.Add(this, &HttpClient::OnTcpClientClosed);
hTcpClient->Connected.Add(this, &HttpClient::OnTcpClientConnected);
hTcpClient->Connect(hhost, uport);
uStatus=HttpClientStatus::Connecting;
}


//================
// Common Private
//================

VOID HttpClient::OnTcpClientClosed(Handle<TcpClient> htcpclient)
{
Close();
}

VOID HttpClient::OnTcpClientConnected(Handle<TcpClient> htcpclient, Handle<TcpConnection> htcpcon)
{
uStatus=HttpClientStatus::Connected;
Handle<HttpConnection> hcon=new HttpConnection(htcpcon);
Connected(this, hcon);
}

}}