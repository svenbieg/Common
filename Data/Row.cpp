//=========
// Row.cpp
//=========

#include "pch.h"


//=======
// Using
//=======

#include "Storage/Streams/StreamReader.h"
#include "Storage/Streams/StreamWriter.h"
#include "Row.h"

using namespace Storage::Streams;


//===========
// Namespace
//===========

namespace Data {


//==================
// Con-/Destructors
//==================

Row::Row(Handle<String> hname):
Name(hname)
{}


//========
// Common
//========

FLOAT Row::Calculate(FLOAT x, FLOAT const* pxs, FLOAT const* pys, SIZE_T ucount, FLOAT ferror)
{
for(SIZE_T u=0; u<ucount; u++)
	{
	if(x<pxs[u])
		{
		if(u==0)
			return ferror;
		FLOAT fx=x-pxs[u-1];
		FLOAT fdx=pxs[u]-pxs[u-1];
		FLOAT fdy=pys[u]-pys[u-1];
		FLOAT y=pys[u-1]+(fx/fdx)*fdy;
		return y;
		}
	}
return ferror;
}

SIZE_T Row::PrintToStream(Handle<OutputStream> hstream, UINT udiv, INT imin, INT imax, INT ioffset)
{
udiv=MAX(udiv, 1);
StreamWriter writer(hstream);
SIZE_T usize=0;
usize+=writer.Print("[ ");
UINT ucount=(UINT)Values->GetCount();
INT ivalue=0;
INT idiv=0;
for(UINT u=0; u<ucount; u++)
	{
	INT i=Values->GetAt(u);
	i+=ioffset;
	if((i>=imin)&&(i<=imax))
		{
		ivalue+=i;
		idiv++;
		}
	if((u%udiv)!=(udiv-1))
		continue;
	if(usize>2)
		usize+=writer.Print(", ");
	if(idiv==0)
		{
		ivalue=imax+1;
		}
	else
		{
		ivalue/=idiv;
		}
	usize+=writer.PrintInt(ivalue);
	ivalue=0;
	idiv=0;
	}
usize+=writer.Print(" ]");
return usize;
}

}