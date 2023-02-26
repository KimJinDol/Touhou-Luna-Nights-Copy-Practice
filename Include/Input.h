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
	// ���ϴ� Ű�� ���۵Ǿ��� ��� ȣ��� �Լ��� ����Ѵ�.
	template <typename T>
	void SetKeyCallback(const char* pName, EKeyState state,
		T* pObj, void(T::*pFunc)(float))
	{
		PKeyInfo	pInfo = FindKeyInfo(pName);

		// ��ϵ� ������ ���� ��� �Լ������ ����Ѵ�.
		if (!pInfo)
			return;

		// std::bind �Լ��� �Լ������͸� function ��ü�� ������ִ� �����
		// �����ϴ� �Լ��̴�.
		// ����Լ��� ���
		// 1�����ڿ� �Լ��� �ּҰ� ����.
		// 2�� ���ڴ� ���� ����Լ���� ȣ������ ��ü�� ����.
		// 3�� ���ں��ʹ� ���� �Լ������Ϳ� ���ڰ� ������ ���
		// ������ ��ȣ���� �������ش�.
		// ���ڰ� ���� ��� 3�� ���ڴ� �����Ѵ�.
		// 1���� ��� std::placeholders::_1
		// 2���� ��� std::placeholders::_1, std::placeholders::_2
		// �̷������� �ο����ִ� ���̴�.
		pInfo->Callback[(int)state] = std::bind(pFunc, pObj, std::placeholders::_1);

		// �����Լ���� 1�����ڿ� �Լ��� �ּҰ� ���� 2�����ں��� ������ ��ȣ��
		// �������ָ� �ȴ�.
	}

	DECLARE_SINGLE(CInput)
};

