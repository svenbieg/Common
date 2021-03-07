//================
// PathHelper.cpp
//================

#include "pch.h"


//=======
// Using
//=======

#include "CharHelper.h"
#include "PathHelper.h"


//========
// Access
//========

template <class _char_t>
UINT PathGetComponent(_char_t const* ppath, UINT* ppos, UINT uid)
{
ASSERT(ppath&&ppos);
for(UINT upos=0; ppath[upos]; upos++)
	{
	if(PathIsSeparator(ppath[upos]))
		continue;
	UINT ustart=upos;
	for(upos++; ppath[upos]; upos++)
		{
		if(PathIsSeparator(ppath[upos]))
			break;
		}
	UINT uend=upos;
	if(!uid)
		{
		*ppos=ustart;
		return uend-ustart;
		}
	uid--;
	}
return 0;
}

UINT PathGetComponent(LPCSTR ppath, UINT* ppos, UINT uid)
{
return PathGetComponent<CHAR>(ppath, ppos, uid);
}

UINT PathGetComponent(LPCWSTR ppath, UINT* ppos, UINT uid)
{
return PathGetComponent<WCHAR>(ppath, ppos, uid);
}

template <class _char_t>
UINT PathGetComponentLength(_char_t const* ppath)
{
if(!ppath)
	return 0;
UINT upos=0;
for(; ppath[upos]; upos++)
	{
	if(PathIsSeparator(ppath[upos]))
		break;
	}
return upos;
}

UINT PathGetComponentLength(LPCSTR ppath)
{
return PathGetComponentLength<CHAR>(ppath);
}

UINT PathGetComponentLength(LPCWSTR ppath)
{
return PathGetComponentLength<WCHAR>(ppath);
}

template <class _char_t>
UINT PathGetDirectoryLength(_char_t const* ppath)
{
UINT ulen=0;
for(UINT upos=0; ppath[upos]; upos++)
	{
	if(PathIsSeparator(ppath[upos]))
		ulen=upos+1;
	if(ppath[upos]=='.')
		break;
	}
return ulen;
}

template <class _char_t>
Handle<String> PathGetDirectory(_char_t const* ppath)
{
UINT start=0;
while(PathIsSeparator(ppath[start]))
	start++;
UINT len=PathGetDirectoryLength<_char_t>(&ppath[start]);
while(PathIsSeparator(ppath[start+len]))
	len--;
if(!len)
	return nullptr;
return new String(len, &ppath[start]);
}

Handle<String> PathGetDirectory(LPCSTR ppath)
{
return PathGetDirectory<CHAR>(ppath);
}

Handle<String> PathGetDirectory(LPCWSTR ppath)
{
return PathGetDirectory<WCHAR>(ppath);
}

UINT PathGetDirectoryLength(LPCSTR ppath)
{
return PathGetDirectoryLength<CHAR>(ppath);
}

UINT PathGetDirectoryLength(LPCWSTR ppath)
{
return PathGetDirectoryLength<WCHAR>(ppath);
}

template <class _char_t>
_char_t const* PathGetExtension(_char_t const* ppath)
{
if(!ppath)
	return nullptr;
UINT upos=0;
for(; ppath[upos]; upos++)
	{
	if(ppath[upos]=='.')
		{
		upos++;
		break;
		}
	}
return &ppath[upos];
}

LPCSTR PathGetExtension(LPCSTR ppath)
{
return PathGetExtension<CHAR>(ppath);
}

LPCWSTR PathGetExtension(LPCWSTR ppath)
{
return PathGetExtension<WCHAR>(ppath);
}

template <class _char_t>
UINT PathGetExtension(_char_t const* ppath, UINT* ppos)
{
if(!ppath)
	return 0;
UINT upos=0;
for(; ppath[upos]; upos++)
	{
	if(ppath[upos]=='.')
		break;
	}
if(ppath[upos]!='.')
	return 0;
UINT ulen=0;
for(upos++; ppath[upos]; upos++)
	{
	ulen++;
	}
*ppos=upos;
return ulen;
}

UINT PathGetExtension(LPCSTR ppath, UINT* ppos)
{
return PathGetExtension<CHAR>(ppath, ppos);
}

UINT PathGetExtension(LPCWSTR ppath, UINT* ppos)
{
return PathGetExtension<WCHAR>(ppath, ppos);
}

Handle<String> PathGetName(Handle<String> hpath)
{
if(!hpath)
	return nullptr;
auto ppath=hpath->Begin();
UINT upos=0;
UINT ulen=PathGetName(ppath, &upos);
if(!ulen)
	return nullptr;
return new String(ulen, &ppath[upos]);
}

template <class _char_t>
UINT PathGetName(_char_t const* ppath, UINT* ppos)
{
UINT upos=PathGetDirectoryLength<_char_t>(ppath);
*ppos=upos;
UINT ulen=0;
for(; ppath[upos]; upos++)
	ulen++;
return ulen;
}

UINT PathGetName(LPCSTR ppath, UINT* ppos)
{
return PathGetName<CHAR>(ppath, ppos);
}

UINT PathGetName(LPCWSTR ppath, UINT* ppos)
{
return PathGetName<WCHAR>(ppath, ppos);
}

template <class _char_t>
BOOL PathIsSeparator(_char_t c)
{
if(CharCompare(c, '\\')==0)
	return true;
if(CharCompare(c, '/')==0)
	return true;
return false;
}

BOOL PathIsSeparator(CHAR c)
{
return PathIsSeparator<CHAR>(c);
}

BOOL PathIsSeparator(WCHAR c)
{
return PathIsSeparator<WCHAR>(c);
}
