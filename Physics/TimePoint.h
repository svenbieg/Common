//=============
// TimePoint.h
//=============

#pragma once


//=======
// Using
//=======

#include "Culture/LanguageClass.h"
#include "Core/Timer.h"
#include "TimeSpan.h"


//======================
// Forward-Declarations
//======================

namespace Devices
{
class Clock;
}


//===========
// Namespace
//===========

namespace Physics {


//========
// Struct
//========

struct TIMEPOINT
{
BYTE Second;
BYTE Minute;
BYTE Hour;
BYTE DayOfWeek;
BYTE DayOfMonth;
BYTE Month;
WORD Year;
};

inline BOOL operator==(TIMEPOINT const& Value1, TIMEPOINT const& Value2)
{
return CompareMemory(&Value1, &Value2, sizeof(TIMEPOINT))==0;
}

inline BOOL operator!=(TIMEPOINT const& Value1, TIMEPOINT const& Value2)
{
return CompareMemory(&Value1, &Value2, sizeof(TIMEPOINT))!=0;
}

VOID TimePointFromTickCount(TIMEPOINT* TimePoint, UINT64 TickCount);
UINT64 TimePointToTickCount(TIMEPOINT const& TimePoint);


//=============
// Time-Format
//=============

enum class TimeFormat
{
Full,
DateTime,
Time
};


//============
// Time-Point
//============

class TimePoint: public TypedVariable<TIMEPOINT>
{
private:
	// Using
	using Clock=Devices::Clock;
	using LanguageCode=Culture::LanguageCode;
	using OutputStream=Storage::Streams::OutputStream;
	using Timer=Core::Timer;

public:
	// Friends
	friend class Devices::Clock;

	// Con-/Destructors
	TimePoint();
	TimePoint(TIMEPOINT const& TimePoint);
	TimePoint(Handle<String> Name);
	TimePoint(Handle<String> Name, TIMEPOINT const& TimePoint);

	// Access
	static UINT GetDayOfWeek(LPCSTR String);
	static UINT GetMonth(LPCSTR String);
	BOOL IsAbsolute();
	UINT64 ToSeconds();
	static UINT64 ToSeconds(TIMEPOINT const& TimePoint);
	Handle<String> ToString()override;
	Handle<String> ToString(TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static Handle<String> ToString(TIMEPOINT const& TimePoint, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static UINT ToString(TIMEPOINT const& TimePoint, LPTSTR Buffer, UINT Size, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	SIZE_T WriteToStream(Handle<OutputStream> Stream)override;

	// Comparison
	BOOL operator==(TIMEPOINT const& TimePoint);

	// Modification
	VOID Clear();
	static BOOL FromTimeStamp(TIMEPOINT* TimePoint, LPCSTR TimeStamp);
	VOID Set(TIMEPOINT const& TimePoint)override;

private:
	// Common
	VOID OnChanged(Handle<Variable> Variable);
	VOID OnTimerTriggered(Handle<Timer> Timer);
	VOID UpdateTimer();
	Handle<Timer> hTimer;
};

}


//===================
// Handle Time-Point
//===================

template <>
class Handle<Physics::TimePoint>
{
private:
	// Using
	using TIMEPOINT=Physics::TIMEPOINT;
	using TimePoint=Physics::TimePoint;

public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Con-/Destructors
	Handle(): pObject(nullptr) {}
	Handle(TimePoint* TimePoint) { HandleCreate(&pObject, TimePoint); }
	Handle(Handle<TimePoint> const& Handle) { HandleCreate(&pObject, Handle.pObject); }
	Handle(Handle<TimePoint>&& Handle)noexcept: pObject(Handle.pObject) { Handle.pObject=nullptr; }
	~Handle() { HandleDestroy(pObject); }

	// Access
	operator TIMEPOINT()const { return VariableGet<TimePoint, TIMEPOINT>(pObject, { 0, 0, 0, 0, 0, 0, 0 }); }
	operator TimePoint*()const { return pObject; }
	TimePoint* operator->()const { return pObject; }

	// Comparison
	BOOL operator==(TIMEPOINT const& Value) { return VariableEqual(pObject, Value); }
	BOOL operator!=(TIMEPOINT const& Value) { return !VariableEqual(pObject, Value); }

	// Assignment
	Handle<TimePoint>& operator=(decltype(nullptr)) { HandleClear(&pObject); return *this; }
	Handle<TimePoint>& operator=(TIMEPOINT const& Value) { VariableAssign(&pObject, Value); return *this; }
	Handle& operator=(TimePoint* Value) { HandleAssign(&pObject, Value); return *this; }
	Handle& operator=(Handle const& Handle) { HandleAssign(&pObject, Handle.pObject); return *this; }

private:
	// Common
	TimePoint* pObject;
};
