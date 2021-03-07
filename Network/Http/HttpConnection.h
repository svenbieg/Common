//==================
// HttpConnection.h
//==================

#pragma once


//=======
// Using
//=======

#include "Network/Tcp/TcpConnection.h"
#include "Storage/Streams/InputStream.h"
#include "HttpResponse.h"
#include "HttpRequest.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//========
// Status
//========

enum class HttpConnectionStatus
{
Closed,
Closing,
Content,
Request,
Requesting,
Responding,
Response,
};


//=================
// Http-Connection
//=================

class HttpConnection: public Storage::Streams::InputStream
{
private:
	// Using
	using TcpConnection=Network::Tcp::TcpConnection;

public:
	// Con-/Destructors
	HttpConnection(Handle<TcpConnection> TcpConnection);
	~HttpConnection();

	// Common
	VOID Close();
	Event<HttpConnection> Closed;
	Event<HttpConnection> DataReceived;
	Event<HttpConnection, Handle<HttpRequest>> Request;
	Event<HttpConnection, Handle<HttpResponse>> Response;
	VOID Send(Handle<HttpRequest> Request);
	VOID Send(Handle<HttpResponse> Response);
	VOID SetTimeout(UINT Timeout);

	// Input-Stream
	SIZE_T Available()override;
	SIZE_T Read(VOID* Buffer, SIZE_T Size)override;

private:
	// Common
	VOID CloseInternal();
	VOID OnTcpConnectionClosed(Handle<TcpConnection> TcpConnection);
	VOID OnTcpConnectionDataReceived(Handle<TcpConnection> TcpConnection);
	VOID OnTcpConnectionDataSent(Handle<TcpConnection> TcpConnection);
	VOID OnTcpConnectionSend(Handle<TcpConnection> TcpConnection);
	Handle<HttpMessage> hMessage;
	Handle<TcpConnection> hTcpConnection;
	UINT64 uPosition;
	HttpConnectionStatus uStatus;
};

}}
