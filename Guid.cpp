//==========
// Guid.cpp
//==========

#include "pch.h"


//=======
// Using
//=======

#include "Guid.h"


//============
// Guid-Class
//============

namespace Details {

GUID::GUID(DWORD data1, WORD data2, WORD data3, BYTE data4, BYTE data5, BYTE data6, BYTE data7, BYTE data8, BYTE data9, BYTE data10, BYTE data11)
{
CopyMemory(pData, &data1, 4);
CopyMemory(&pData[4], &data2, 2);
CopyMemory(&pData[6], &data3, 2);
pData[8]=data4;
pData[9]=data5;
pData[10]=data6;
pData[11]=data7;
pData[12]=data8;
pData[13]=data9;
pData[14]=data10;
pData[15]=data11;
}

}