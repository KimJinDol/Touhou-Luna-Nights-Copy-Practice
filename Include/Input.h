#pragma once

#include "GameInfo.h"

typedef struct _tagKeyState
{
	unsigned int	Key;
	bool	State[(int)EKeyState::Max];

	_tagKeyState()
	{
		Key = 0;
		State[(int)EKeyState::Down] = false;
		State[(int)EKeyState::Press] = false;
		State[(int)EKeyState::Up] = false;
	}
}KeyState, *PKeyState;

typedef struct _tagKeyInfo
{
	std::string		Name;
	KeyState		State;
	std::function<void(float)>	Callback[(int)EKeyState::Max];
	bool	KeyCtrl;
	bool	KeyAlt;
	bool	KeyShift;

	_tagKeyInfo()
	{
		KeyCtrl = false;
		KeyAlt = false;
		KeyShift = false;
	}
}KeyInfo, *PKeyInfo;

class CInput
{
private:
	std::vector<KeyState>	m_vecKeyState;
	std::vector<int>		m_vecAddKeyIndex;
	std::unordered_map<std::string, PKeyInfo>	m_mapInfo;
	bool	m_KeyCtrl;
	bool	m_KeyAlt;
	bool	m_KeyShift;
	Vector2	m_vMousePos;
	Vector2	m_vMouseMove;

public:
	bool GetKeyState(const std::string& Name, EKeyState State = EKeyState::Press)
	{
		KeyInfo* pInfo = FindKeyInfo(Name);

		if (!pInfo) return false;

		return pInfo->State.State[(int)State];
	}

	Vector2 GetMousePos()	const
	{
		return m_vMousePos;
	}

	Vector2 GetMouseMove()	const
	{
		return m_vMouseMove;
	}

public:
	bool Init();
	void Update(float DeltaTime);

private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState();
	void UpdateKeyInfo(float DeltaTime);

public:
	bool CreateKey(const char* pName, unsigned int Key);
	bool SetCtrlKey(const char* pName, bool State);
	bool SetAltKey(const char* pName, bool State);
	bool SetShiftKey(const char* pName, bool State);
	void ClearCallback();

private:
	PKeyInfo FindKeyInfo(const std::string& Name)
	{
		auto	iter = m_mapInfo.find(Name);

		if (iter == m_mapInfo.end())
			return nullptr;

		return iter->second;
	}

public:
	// 원하는 키가 동작되었을 경우 호출될 함수를 등록한다.
	template <typename T>
	void SetKeyCallback(const char* pName, EKeyState state,
		T* pObj, void(T::*pFunc)(float))
	{
		PKeyInfo	pInfo = FindKeyInfo(pName);

		// 등록된 정보가 없을 경우 함수등록을 취소한다.
		if (!pInfo)
			return;

		// std::bind 함수는 함수포인터를 function 객체로 만들어주는 기능을
		// 제공하는 함수이다.
		// 멤버함수일 경우
		// 1번인자에 함수의 주소가 들어간다.
		// 2번 인자는 만약 멤버함수라면 호출해줄 객체가 들어간다.
		// 3번 인자부터는 만약 함수포인터에 인자가 존재할 경우
		// 인자의 번호들을 나열해준다.
		// 인자가 없을 경우 3번 인자는 무시한다.
		// 1개일 경우 std::placeholders::_1
		// 2개일 경우 std::placeholders::_1, std::placeholders::_2
		// 이런식으로 부여해주는 것이다.
		pInfo->Callback[(int)state] = std::bind(pFunc, pObj, std::placeholders::_1);

		// 전역함수라면 1번인자에 함수의 주소가 들어가고 2번인자부터 인자의 번호를
		// 나열해주면 된다.
	}

	DECLARE_SINGLE(CInput)
};

