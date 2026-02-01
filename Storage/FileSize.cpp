//==============
// FileSize.cpp
//==============

#include "FileSize.h"


//===========
// Namespace
//===========

namespace Storage {


//========
// Access
//========

Handle<String> FileSize::ToString(UINT64 size)
{
if(size>1024*1024*1024)
	return String::Create("%.2f GB", ((FLOAT)(size/1024/1024))/1024.f);
if(size>1024*1024)
	return String::Create("%.2f MB", ((FLOAT)(size/1024))/1024.f);
if(size>1024)
	return String::Create("%.2f KB", ((FLOAT)size)/1024.f);
return String::Create("%u Bytes", (UINT)size);
}

SIZE_T FileSize::WriteToStream(OutputStream* Stream)
{
if(!Stream)
	return sizeof(UINT64);
return Stream->Write(&m_Value, sizeof(UINT64));
}


//==============
// Modification
//==============

SIZE_T FileSize::ReadFromStream(InputStream* stream, BOOL notify)
{
if(!stream)
	return 0;
UINT64 value=0;
SIZE_T size=stream->Read(&value, sizeof(UINT64));
if(size==sizeof(UINT64))
	Set(value, notify);
return size;
}

BOOL FileSize::Set(UINT64 value, BOOL notify)
{
if(m_Value==value)
	return false;
m_Value=value;
if(notify)
	Changed(this);
return true;
}

}