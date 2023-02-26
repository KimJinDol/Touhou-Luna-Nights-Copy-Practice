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
	// GetTickCount() : 1/1000 �ð��� �������� �� �ִ�.
	// QueryPerformanceFrequency() : ���� Ÿ�̸��̴�. �ξ� �� ������ �ð��� �������� �� �ִ�.
	// �ý����� �ִ� ������� ������ Ÿ�̸� �ػ󵵸� �����ϴ��� �Ǵ����ش�.
	// QueryPerformanceCounter() : ���� Ÿ�̸��� ���� ī���͸� �˾ƿö� ����Ѵ�.
	QueryPerformanceFrequency(&m_Second);
	QueryPerformanceCounter(&m_Time);

	return true;
}

// �� �����Ӹ��� ���۵ȴ�.
void CTimer::Update()
{
	LARGE_INTEGER	Time;
	QueryPerformanceCounter(&Time);

	// ���� ī���Ϳ��� ���� �������� ī���͸� ���� ������ ������������� ���� �Ŀ�
	// 1�ʴ� ���� Ÿ�̸��� �ð��� ���ͼ� �������ָ� ���� �������������κ��� ���ʰ�
	// �귶������ �� �� �ִ�.
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	// ī���͸� �����Ѵ�.
	m_Time = Time;

	m_FPSTime += m_DeltaTime;
	++m_FPSTick;

	// �������� 60�� �귶�� ���
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
