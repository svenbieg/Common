//==============
// HttpServer.h
//==============

#pragma once


//=======
// Using
//=======

#include "Collections/Index.h"
#include "Network/Tcp/TcpServer.h"
#include "Storage/File.h"
#include "Web/WebPage.h"
#include "HttpConnection.h"


//===========
// Namespace
//===========

namespace Network {
	namespace Http {


//========
// Status
//========

enum class HttpServerStatus
{
Closed,
Listening
};


//=============
// Http-Server
//=============

class HttpServer: public Object
{
private:
	// Using
	using ConnectionList=Collections::Index<Handle<HttpConnection>>;
	using File=Storage::File;
	using TcpConnection=Network::Tcp::TcpConnection;
	using TcpServer=Network::Tcp::TcpServer;
	using WebPage=Web::WebPage;

public:
	// Con-/Destructors
	HttpServer();
	~HttpServer();

	// Common
	VOID Close();
	Event<HttpServer, Handle<HttpConnection>> ConnectionReceived;
	Handle<ConnectionList> Connections;
	static Handle<HttpServer> Current;
	VOID Listen(WORD Port=80);

private:
	// Common
	VOID OnConnectionClosed(Handle<HttpConnection> Connection);
	VOID OnConnectionDataReceived(Handle<HttpConnection> Connection);
	VOID OnConnectionReceived(Handle<TcpServer> TcpServer, Handle<TcpConnection> TcpConnection);
	VOID OnConnectionRequest(Handle<HttpConnection> Connection, Handle<HttpRequest> Request);
	Handle<File> hFile;
	Handle<TcpServer> hTcpServer;
	UINT64 uFileEnd;
	UINT64 uFileSize;
	UINT64 uFileStart;
	HttpServerStatus uStatus;
};

}}
