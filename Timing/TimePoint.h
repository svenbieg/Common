//=============
// TimePoint.h
//=============

#pragma once


//=======
// Using
//=======

#include "Culture/LanguageCode.h"
#include "Timing/TimeSpan.h"


//===========
// Namespace
//===========

namespace Timing {


//======================
// Forward-Declarations
//======================

class Clock;


//========
// Struct
//========

class TIME_POINT
{
public:
	// Con-/Destructors
	TIME_POINT()=default;
	TIME_POINT(UINT64 Value): Value(Value) {}
	TIME_POINT(TIME_POINT const& Value): Value(Value.Value) {}

	// Access
	inline operator BOOL() { return Value!=0; }

	// Comparison
	inline BOOL operator==(TIME_POINT const& Value) { return this->Value==Value.Value; }
	inline BOOL operator!=(TIME_POINT const& Value) { return this->Value!=Value.Value; }
	inline BOOL operator>(TIME_POINT const& Value) { return this->Value>Value.Value; }
	inline BOOL operator>=(TIME_POINT const& Value) { return this->Value>=Value.Value; }
	inline BOOL operator<(TIME_POINT const& Value) { return this->Value<Value.Value; }
	inline BOOL operator<=(TIME_POINT const& Value) { return this->Value<=Value.Value; }

	// Common
	union
		{
		struct
			{
			BYTE Second;
			BYTE Minute;
			BYTE Hour;
			BYTE DayOfWeek;
			BYTE DayOfMonth;
			BYTE Month;
			WORD Year;
			};
		UINT64 Value=0;
		};
};


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

class TimePoint: public Variable
{
public:
	// Con-/Destructors
	static Handle<TimePoint> Create();
	static Handle<TimePoint> Create(TIME_POINT const& TimePoint);
	static Handle<TimePoint> Create(Handle<String> Name);
	static Handle<TimePoint> Create(Handle<String> Name, TIME_POINT const& TimePoint);

	// Access
	TIME_POINT Get();
	static inline TIME_POINT Get(TimePoint* Value) { return Value? Value->Get(): TIME_POINT(); }
	static UINT GetDayOfWeek(LPCSTR String);
	static UINT GetDayOfYear(TIME_POINT const& TimePoint);
	inline Handle<String> GetName()const override { return m_Name; }
	static UINT GetMonth(LPCSTR String);
	inline BOOL IsAbsolute() { return Get().Year!=0; }
	Event<TimePoint, TIME_POINT&> Reading;
	inline UINT64 ToSeconds() { return ToSeconds(Get()); }
	static UINT64 ToSeconds(TIME_POINT const& TimePoint);
	Handle<String> ToString(LanguageCode Language=LanguageCode::None)override;
	Handle<String> ToString(TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static Handle<String> ToString(TIME_POINT const& TimePoint, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	static UINT ToString(TIME_POINT const& TimePoint, LPSTR Buffer, UINT Size, TimeFormat Format, LanguageCode Language=LanguageCode::None);
	SIZE_T WriteToStream(OutputStream* Stream)override;

	// Modification
	VOID Clear(BOOL Notify=true);
	static VOID FromSeconds(TIME_POINT* TimePoint, UINT64 Seconds);
	static BOOL FromTimeStamp(TIME_POINT* TimePoint, LPCSTR TimeStamp);
	SIZE_T ReadFromStream(InputStream* Stream, BOOL Notify=true)override;
	BOOL Set(TIME_POINT const& TimePoint, BOOL Notify=true);

private:
	// Con-/Destructors
	TimePoint(Handle<String> Name, TIME_POINT const& Value);

	// Common
	UINT64 GetTickCount(TIME_POINT const& TimePoint);
	VOID OnClockSecond(Clock* Clock);
	static UINT ToStringDateTime(TIME_POINT const& TimePoint, LPSTR Buffer, UINT Size, LanguageCode Language);
	static UINT ToStringFull(TIME_POINT const& TimePoint, LPSTR Buffer, UINT Size, LanguageCode Language);
	static UINT ToStringRelative(UINT64 TickCount, LPSTR Buffer, UINT Size, TimeFormat Format, LanguageCode Language);
	static UINT ToStringTime(TIME_POINT const& TimePoint, LPSTR Buffer, UINT Size, LanguageCode Language);
	VOID UpdateClock();
	Handle<Clock> m_Clock;
	Handle<String> m_Name;
	TIME_POINT m_Value;
};

}


//========
// Handle
//========

template <>
class Handle<Timing::TimePoint>
{
public:
	// Friends
	template <class _friend_t> friend class Handle;

	// Using
	using TimePoint=Timing::TimePoint;
	using TIME_POINT=Timing::TIME_POINT;

	// Con-/Destructors
	inline Handle(): m_Object(nullptr) {}
	inline Handle(nullptr_t): m_Object(nullptr) {}
	inline Handle(TimePoint* Copy) { Handle<Object>::Create(&m_Object, Copy); }
	inline Handle(Handle const& Copy): Handle(Copy.m_Object) {}
	inline Handle(Handle&& Move)noexcept: m_Object(Move.m_Object) { Move.m_Object=nullptr; }
	inline ~Handle() { Handle<Object>::Clear(&m_Object); }

	// Access
	inline operator BOOL()const { return m_Object&&m_Object->Get(); }
	inline operator TimePoint*()const { return m_Object; }
	inline TimePoint* operator->()const { return m_Object; }

	// Comparison
	inline BOOL operator==(nullptr_t)const { return TimePoint::Get(m_Object).Value==0; }
	inline BOOL operator==(TimePoint* Value)const { return TimePoint::Get(m_Object)==TimePoint::Get(Value); }
	inline BOOL operator==(TIME_POINT const& Value)const { return TimePoint::Get(m_Object)==Value; }
	inline BOOL operator!=(nullptr_t)const { return TimePoint::Get(m_Object).Value!=0; }
	inline BOOL operator!=(TimePoint* Value)const { return TimePoint::Get(m_Object)!=TimePoint::Get(Value); }
	inline BOOL operator!=(TIME_POINT const& Value)const { return TimePoint::Get(m_Object)!=Value; }
	inline BOOL operator>(nullptr_t)const { return TimePoint::Get(m_Object).Value>0; }
	inline BOOL operator>(TimePoint* Value)const { return TimePoint::Get(m_Object)>TimePoint::Get(Value); }
	inline BOOL operator>(TIME_POINT const& Value)const { return TimePoint::Get(m_Object)>Value; }
	inline BOOL operator>=(nullptr_t)const { return TimePoint::Get(m_Object).Value>=0; }
	inline BOOL operator>=(TimePoint* Value)const { return TimePoint::Get(m_Object)>=TimePoint::Get(Value); }
	inline BOOL operator>=(TIME_POINT const& Value)const { return TimePoint::Get(m_Object)>=Value; }
	inline BOOL operator<(nullptr_t)const { return false; }
	inline BOOL operator<(TimePoint* Value)const { return TimePoint::Get(m_Object)<TimePoint::Get(Value); }
	inline BOOL operator<(TIME_POINT const& Value)const { return TimePoint::Get(m_Object)<Value; }
	inline BOOL operator<=(nullptr_t)const { return TimePoint::Get(m_Object).Value<=0; }
	inline BOOL operator<=(TimePoint* Value)const { return TimePoint::Get(m_Object)<=TimePoint::Get(Value); }
	inline BOOL operator<=(TIME_POINT const& Value)const { return TimePoint::Get(m_Object)<=Value; }

	// Assignment
	inline Handle& operator=(nullptr_t) { Handle<Object>::Clear(&m_Object); return *this; }
	inline Handle& operator=(TimePoint* Copy) { Handle<Object>::Set(&m_Object, Copy); return *this; }
	inline Handle& operator=(Handle const& Copy) { return operator=(Copy.m_Object); }
	Handle& operator=(TIME_POINT const& Value)
		{
		if(!m_Object)
			{
			auto value=TimePoint::Create(Value);
			return operator=(value);
			}
		m_Object->Set(Value);
		return *this;
		}

private:
	// Common
	TimePoint* m_Object;
};


//==================
// Con-/Destructors
//==================

namespace Timing {

inline Handle<TimePoint> TimePoint::Create()
{
TIME_POINT tp={ 0 };
return new TimePoint(nullptr, tp);
}

inline Handle<TimePoint> TimePoint::Create(TIME_POINT const& Value)
{
return new TimePoint(nullptr, Value);
}

inline Handle<TimePoint> TimePoint::Create(Handle<String> Name)
{
TIME_POINT tp={ 0 };
return new TimePoint(Name, tp);
}

inline Handle<TimePoint> TimePoint::Create(Handle<String> Name, TIME_POINT const& Value)
{
return new TimePoint(Name, Value);
}

}