#pragma once

#include "GameInfo.h"

struct TimerInfo
{
	std::string				Name;
	float					Second;			// rest time
	std::function<void()>   Function;		// callBack
};

class CTimer
{
public:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_Second;
	LARGE_INTEGER	m_Time;

	float			m_DeltaTime;
	float			m_FPS;
	float			m_FPSTime;
	int				m_FPSTick;

	// Timer
	std::list<TimerInfo*> m_listTimer;

public:
	float GetDeltaTime()	const
	{
		return m_DeltaTime;
	}

	float GetFPS()	const
	{
		return m_FPS;
	}

public:
	bool Init();
	void Update();

	TimerInfo* FindTimer(const std::string& Name);
	std::list<TimerInfo*>::iterator GetTimerIterator(const std::string& Name);

	void RemoveTimer(const std::string& Name)
	{
		std::list<TimerInfo*>::iterator iter = GetTimerIterator(Name);		

		if (iter == m_listTimer.end()) return;

		delete *iter;
		m_listTimer.erase(iter);
	}

	void ClearTimer()
	{
		auto iter = m_listTimer.begin();
		auto iterEnd = m_listTimer.end();

		for (; iter != iterEnd; iter++)
			delete *iter;

		m_listTimer.clear();
	}

public:
	template <typename T>
	void StartTimer(const std::string& Name, float Second,
		T* pObj, void (T::* pFunc)())
	{
		TimerInfo* pInfo = FindTimer(Name);

		if (pInfo)
			return;

		pInfo = new TimerInfo();
		pInfo->Name = Name;
		pInfo->Function = std::bind(pFunc, pObj);
		pInfo->Second = Second;
		m_listTimer.push_back(pInfo);
	}

	//template <typename T, typename ... Args>
	//void StartTimer(const std::string& Name, float Second,
	//	T* pObj, void (T::* pFunc)(), Args ...args)
	//{
	//	TimerInfo* pInfo = FindTimer(Name);

	//	if (pInfo)
	//		return;

	//	pInfo = new TimerInfo();
	//	pInfo->Function = std::bind(pFunc, pObj);
	//	pInfo->Second = Second;
	//	m_listTimer.push_back(pInfo);
	//}
};

