#include "Timer.h"

CTimer::CTimer()	:
	m_Second{},
	m_Time{},
	m_DeltaTime(0.f),
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_FPSTick(0)
{
}

CTimer::~CTimer()
{
	auto iter = m_listTimer.begin();
	auto iterEnd = m_listTimer.end();

	for (; iter != iterEnd; iter++)
	{
		delete *iter;
	}

	m_listTimer.clear();
}

bool CTimer::Init()
{
	// GetTickCount() : 1/1000 시간을 측정해줄 수 있다.
	// QueryPerformanceFrequency() : 고성능 타이머이다. 훨씬 더 정밀한 시간을 측정해줄 수 있다.
	// 시스템이 최대 어느정도 까지의 타이머 해상도를 지원하는지 판단해준다.
	// QueryPerformanceCounter() : 고성능 타이머의 현재 카운터를 알아올때 사용한다.
	QueryPerformanceFrequency(&m_Second);
	QueryPerformanceCounter(&m_Time);

	return true;
}

// 매 프레임마다 동작된다.
void CTimer::Update()
{
	LARGE_INTEGER	Time;
	QueryPerformanceCounter(&Time);

	// 현재 카운터에서 이전 프레임의 카운터를 빼서 간격이 어느정도인지를 구한 후에
	// 1초당 고성능 타이머의 시간을 얻어와서 나누어주면 현재 이전프레임으로부터 몇초가
	// 흘렀는지를 알 수 있다.
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	// 카운터를 갱신한다.
	m_Time = Time;

	m_FPSTime += m_DeltaTime;
	++m_FPSTick;

	// 프레임이 60번 흘렀을 경우
	if (m_FPSTick == 60)
	{
		m_FPS = m_FPSTick / m_FPSTime;
		m_FPSTime = 0.f;
		m_FPSTick = 0;
	}

	// Timer
	std::list<TimerInfo*>::iterator iter = m_listTimer.begin();
	std::list<TimerInfo*>::iterator iterEnd = m_listTimer.end();

	for (;iter != iterEnd;)
	{
		(*iter)->Second -= m_DeltaTime;
		if ((*iter)->Second <= 0 )
		{
			(*iter)->Function();
			delete* iter;
			iter = m_listTimer.erase(iter);
			iterEnd = m_listTimer.end();
			continue;
		}
		
		++iter;
	}
}

TimerInfo* CTimer::FindTimer(const std::string& Name)
{
	std::list<TimerInfo*>::iterator iter = m_listTimer.begin();
	std::list<TimerInfo*>::iterator iterEnd = m_listTimer.end();
	
	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->Name == Name)
		{
			return (*iter);
		}
	}

	return nullptr;
}

std::list<TimerInfo*>::iterator CTimer::GetTimerIterator(const  std::string& Name)
{
	std::list<TimerInfo*>::iterator iter = m_listTimer.begin();
	std::list<TimerInfo*>::iterator iterEnd = m_listTimer.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->Name == Name)
		{
			return iter;
		}
	}

	return iterEnd;
}
