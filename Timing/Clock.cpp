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

VOID Clock::DoTick()
{
m_Before=m_Now;
ClockHelper::GetTime(&m_Now);
if(m_Before.Second==m_Now.Second)
	return;
Second(this);
if(m_Before.Minute==m_Now.Minute)
	return;
Minute(this);
if(m_Before.Hour==m_Now.Hour)
	return;
Hour(this);
if(m_Before.DayOfMonth==m_Now.DayOfMonth)
	return;
Day(this);
if(m_Before.Month==m_Now.Month)
	return;
Month(this);
if(m_Before.Year==m_Now.Year)
	return;
Year(this);
}

}