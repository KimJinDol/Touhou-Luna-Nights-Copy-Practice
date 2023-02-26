#include "Input.h"
#include "GameManager.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()
{
	// resize : �̸� ���ڷ� ���� ������ŭ�� �߰��Ȱ����� �νĽ����ش�.
	m_vecKeyState.resize(KEYCOUNT_MAX);

	for (int i = 0; i < KEYCOUNT_MAX; ++i)
	{
		m_vecKeyState[i].Key = i;
	}

	m_KeyCtrl = false;
	m_KeyAlt = false;
	m_KeyShift = false;
}

CInput::~CInput()
{
	auto	iter = m_mapInfo.begin();
	auto	iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CInput::Init()
{
	/*
	���⿡�� �� ���ӿ��� ����ϴ� Ű�� �������ݴϴ�.
	*/
	CreateKey("MoveUp", VK_UP);
	CreateKey("MoveDown", VK_DOWN);
	CreateKey("MoveLeft", VK_LEFT);
	CreateKey("MoveRight", VK_RIGHT);
	CreateKey("Action", 'Z');
	CreateKey("Jump", 'X');
	//SetCtrlKey("Fire", true);
	CreateKey("TimeStop", 'A');
	CreateKey("LButton", VK_LBUTTON);

	CreateKey("Tab", VK_TAB);

	CreateKey("OnEditor", 'L');
	SetCtrlKey("OnEditor", true);

	CreateKey("F1", VK_F1);
	CreateKey("F2", VK_F2);
	CreateKey("F3", VK_F3);
	CreateKey("F4", VK_F4);

	CreateKey("1", '1');
	SetCtrlKey("1", true);
	CreateKey("2", '2');
	SetCtrlKey("2", true);
	CreateKey("3", '3');
	SetCtrlKey("3", true);
	CreateKey("4", '4');
	SetCtrlKey("4", true);
	CreateKey("5", '5');
	SetCtrlKey("5", true);
	CreateKey("6", '6');
	SetCtrlKey("6", true);


	return true;
}

void CInput::Update(float DeltaTime)
{
	// Ű ���¸� ������Ʈ�Ѵ�.
	UpdateKeyState();

	// ���콺 ���� ������Ʈ.
	UpdateMouse(DeltaTime);

	// Ű ������ ������Ʈ�ϰ� ���ϴ� Ű�� �������ٸ� ��ϵ� �Լ���
	// ȣ���ϵ��� �Ѵ�.
	UpdateKeyInfo(DeltaTime);
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT	ptMouse;

	// �� �Լ��� ���콺�� ��ġ�� ���´�.
	// �׷��� ��ġ�� ��ũ�� ��ǥ �������� ������ �ȴ�.
	GetCursorPos(&ptMouse);

	// ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� �����Ѵ�.
	ScreenToClient(CGameManager::GetInst()->GetWindowHandle(), &ptMouse);

	Vector2	vPos = ptMouse;

	m_vMouseMove = vPos - m_vMousePos;

	m_vMousePos = vPos;
}

void CInput::UpdateKeyState()
{
	size_t	Size = m_vecAddKeyIndex.size();

	for (size_t i = 0; i < Size; ++i)
	{
		int	Index = m_vecAddKeyIndex[i];

		// 0xff���� ũ�ٴ� ���� 1����Ʈ�� ������ �� �ִ� �̻��� ����
		// ����Ǿ��ٴ� ���̹Ƿ� �츮 �ڵ忡���� �� ������ ��� ���콺
		// ���õ� ������ �� �ִ�.
		// ���콺���ʹ�ư, �����ʹ�ư Ŭ���� �⺻ ���� �� �ְ�
		// �� ������ ������ ���콺 ��, ���콺 �¿� �̵�, ���콺 �����̵�
		// �� ���ִ� ���̴�.
		if (m_vecKeyState[Index].Key > 0xff)
		{
			switch (m_vecKeyState[Index].Key)
			{
			case KEY_MOUSEWHEEL:
				break;
			case KEY_MOUSEX:
				break;
			case KEY_MOUSEY:
				break;
			}
		}

		else
		{
			if (GetAsyncKeyState(m_vecKeyState[Index].Key) & 0x8000)
			{
				// ���� �� ���� ������ ��� ������ �ִ� ��������
				// �Ǵ��ؾ� �Ѵ�.
				if (!m_vecKeyState[Index].State[(int)EKeyState::Down] &&
					!m_vecKeyState[Index].State[(int)EKeyState::Press])
				{
					m_vecKeyState[Index].State[(int)EKeyState::Down] = true;
					m_vecKeyState[Index].State[(int)EKeyState::Press] = true;
				}

				else
				{
					m_vecKeyState[Index].State[(int)EKeyState::Down] = false;
					m_vecKeyState[Index].State[(int)EKeyState::Press] = true;
				}
			}

			// ���� Ű�� �ȴ������� ���������ӿ� Ű�� �������ٸ�
			// ���� Ű�� �����ϴ� ���� �ȴ�.
			else if (m_vecKeyState[Index].State[(int)EKeyState::Press])
			{
				m_vecKeyState[Index].State[(int)EKeyState::Up] = true;
				m_vecKeyState[Index].State[(int)EKeyState::Down] = false;
				m_vecKeyState[Index].State[(int)EKeyState::Press] = false;
			}

			// ���� �����ӿ� Up�� true�� �Ǿ��ٸ� ��� �ʱ�ȭ�Ѵ�.
			else if (m_vecKeyState[Index].State[(int)EKeyState::Up])
			{
				m_vecKeyState[Index].State[(int)EKeyState::Up] = false;
			}
		}
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		m_KeyCtrl = true;
	}

	else
		m_KeyCtrl = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000)
	{
		m_KeyAlt = true;
	}

	else
		m_KeyAlt = false;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_KeyShift = true;
	}

	else
		m_KeyShift = false;
}

void CInput::UpdateKeyInfo(float DeltaTime)
{
	auto	iter = m_mapInfo.begin();
	auto	iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		int	Index = iter->second->State.Key;

		if (m_vecKeyState[Index].State[(int)EKeyState::Down] &&
			m_KeyCtrl == iter->second->KeyCtrl &&
			m_KeyAlt == iter->second->KeyAlt &&
			m_KeyShift == iter->second->KeyShift)
		{
			// ȣ���� �ݹ��Լ��� ���� ��� ȣ���� ���ش�.
			if (iter->second->Callback[(int)EKeyState::Down])
				iter->second->Callback[(int)EKeyState::Down](DeltaTime);
		}

		if (m_vecKeyState[Index].State[(int)EKeyState::Press] &&
			m_KeyCtrl == iter->second->KeyCtrl &&
			m_KeyAlt == iter->second->KeyAlt &&
			m_KeyShift == iter->second->KeyShift)
		{
			// ȣ���� �ݹ��Լ��� ���� ��� ȣ���� ���ش�.
			if (iter->second->Callback[(int)EKeyState::Press])
				iter->second->Callback[(int)EKeyState::Press](DeltaTime);
		}

		if (m_vecKeyState[Index].State[(int)EKeyState::Up] &&
			m_KeyCtrl == iter->second->KeyCtrl &&
			m_KeyAlt == iter->second->KeyAlt &&
			m_KeyShift == iter->second->KeyShift)
		{
			// ȣ���� �ݹ��Լ��� ���� ��� ȣ���� ���ش�.
			if (iter->second->Callback[(int)EKeyState::Up])
				iter->second->Callback[(int)EKeyState::Up](DeltaTime);
		}
	}
}

bool CInput::CreateKey(const char* pName, unsigned int Key)
{
	PKeyInfo	pInfo = FindKeyInfo(pName);

	// ���� �̸����� ��ϵ� Ű�� �̹� ���� ���
	// Ű�� �ߺ��ؼ� �����ϴ� ���� �����.
	if (pInfo)
		return false;

	pInfo = new KeyInfo;

	pInfo->Name = pName;
	pInfo->State.Key = Key;

	m_mapInfo.insert(std::make_pair(pName, pInfo));

	bool	Acc = false;

	size_t	Size = m_vecAddKeyIndex.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecAddKeyIndex[i] == Key)
		{
			Acc = true;
			break;
		}
	}

	if (!Acc)
	{
		m_vecAddKeyIndex.push_back(Key);
	}

	return true;
}

bool CInput::SetCtrlKey(const char* pName, bool State)
{
	PKeyInfo	pInfo = FindKeyInfo(pName);

	if (!pInfo)
		return false;

	pInfo->KeyCtrl = State;

	return true;
}

bool CInput::SetAltKey(const char* pName, bool State)
{
	PKeyInfo	pInfo = FindKeyInfo(pName);

	if (!pInfo)
		return false;

	pInfo->KeyAlt = State;

	return true;
}

bool CInput::SetShiftKey(const char* pName, bool State)
{
	PKeyInfo	pInfo = FindKeyInfo(pName);

	if (!pInfo)
		return false;

	pInfo->KeyShift = State;

	return true;
}

void CInput::ClearCallback()
{
	auto	iter = m_mapInfo.begin();
	auto	iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < (int)EKeyState::Max; ++i)
		{
			iter->second->Callback[i] = nullptr;
		}
	}
}
