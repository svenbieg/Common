//============
// Random.cpp
//============

#include "pch.h"


//=======
// Using
//=======

#include <random>
#include <stdlib.h>
#include "Random.h"


//===========
// Namespace
//===========

namespace Devices {


//========
// Common
//========

CHAR RandomChars[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
constexpr UINT RandomCharCount=ARRAYSIZE(RandomChars);

Handle<String> RandomString(UINT len)
{
Handle<String> str=new String(len, nullptr);
auto str_ptr=const_cast<LPTSTR>(str->Begin());
std::mt19937 rng(GetTickCount());
for(UINT u=0; u<len; u++)
	{
	UINT rnd=rng()%RandomCharCount;
	str_ptr[u]=CharToChar<TCHAR, CHAR>(RandomChars[rnd]);
	}
str_ptr[len]=0;
return str;
}

}