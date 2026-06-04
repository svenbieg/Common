//===========
// Clock.cpp
//===========

#include "Clock.h"


//=======
// Using
//=======

#include "Concurrency/DispatchedQueue.h"
#include "Devices/Timers/SystemTimer.h"
#include "Timing/ClockHelper.h"

using namespace Concurrency;
using namespace Devices::Timers;


//===========
// Namespace
//===========

namespace Timing {


//========
// Common
//========

TIME_POINT const& Clock::Now()
{
ClockHelper::GetTime(&m_Now);
return m_Now;
}

BOOL Clock::Update(TIME_POINT* tp)
{
if(m_Now.Year==0)
	return false;
UINT64 secs=TimePoint::ToSeconds(m_Now);
UINT64 ticks=0;
MemoryHelper::Copy(&ticks, tp, sizeof(UINT64));
secs-=ticks/1000;
TimePoint::FromSeconds(tp, secs);
return true;
}


//==========================
// Con-/Destructors Private
//==========================

Clock::Clock():
m_This(this)
{
Day.Add(this, &Clock::OnDay);
Hour.Add(this, &Clock::OnHour);
Minute.Add(this, &Clock::OnMinute);
Month.Add(this, &Clock::OnMonth);
Second.Add(this, &Clock::OnSecond);
Tick.Add(this, &Clock::OnTick);
m_ClockTask=Task::Create(this, &Clock::ClockTask);
}


//================
// Common Private
//================

VOID Clock::ClockTask()
{
auto task=Task::Get();
while(!task->Cancelled)
	{
	Task::Sleep(100);
	DispatchedQueue::Append(this, [this](){ Tick(this); });
	}
}

VOID Clock::OnDay()
{
if(m_Before.Month!=m_Now.Month)
	Month(this);
}

VOID Clock::OnHour()
{
if(m_Before.DayOfMonth!=m_Now.DayOfMonth)
	Day(this);
}

VOID Clock::OnMinute()
{
if(m_Before.Hour!=m_Now.Hour)
	Hour(this);
}

VOID Clock::OnMonth()
{
if(m_Before.Year!=m_Now.Year)
	Year(this);
}

VOID Clock::OnSecond()
{
if(m_Before.Minute!=m_Now.Minute)
	Minute(this);
}

VOID Clock::OnTick()
{
m_Before=m_Now;
ClockHelper::GetTime(&m_Now);
if(m_Before.Second!=m_Now.Second)
	Second(this);
}

}