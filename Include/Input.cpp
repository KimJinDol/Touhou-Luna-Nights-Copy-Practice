#include "Input.h"
#include "GameManager.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()
{
	// resize : 미리 인자로 들어가는 개수만큼을 추가된것으로 인식시켜준다.
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
	여기에서 이 게임에서 사용하는 키를 설정해줍니다.
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
	// 키 상태를 업데이트한다.
	UpdateKeyState();

	// 마우스 정보 업데이트.
	UpdateMouse(DeltaTime);

	// 키 정보를 업데이트하고 원하는 키가 눌러졌다면 등록된 함수를
	// 호출하도록 한다.
	UpdateKeyInfo(DeltaTime);
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT	ptMouse;

	// 이 함수는 마우스의 위치를 얻어온다.
	// 그런데 위치가 스크린 좌표 기준으로 나오게 된다.
	GetCursorPos(&ptMouse);

	// 스크린 좌표를 클라이언트 좌표로 변경한다.
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

		// 0xff보다 크다는 것은 1바이트로 저장할 수 있는 이상의 값이
		// 저장되었다는 것이므로 우리 코드에서는 이 값들은 모두 마우스
		// 관련된 값들이 들어가 있다.
		// 마우스왼쪽버튼, 오른쪽버튼 클릭은 기본 값에 들어가 있고
		// 이 이후의 값들은 마우스 휠, 마우스 좌우 이동, 마우스 상하이동
		// 이 들어가있는 것이다.
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
				// 지금 막 누른 것인지 계속 누르고 있는 것인지를
				// 판단해야 한다.
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

			// 현재 키는 안눌렀지만 이전프레임에 키가 눌러졋다면
			// 지금 키를 해제하는 것이 된다.
			else if (m_vecKeyState[Index].State[(int)EKeyState::Press])
			{
				m_vecKeyState[Index].State[(int)EKeyState::Up] = true;
				m_vecKeyState[Index].State[(int)EKeyState::Down] = false;
				m_vecKeyState[Index].State[(int)EKeyState::Press] = false;
			}

			// 이전 프레임에 Up이 true가 되었다면 모두 초기화한다.
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
			// 호출할 콜백함수가 있을 경우 호출을 해준다.
			if (iter->second->Callback[(int)EKeyState::Down])
				iter->second->Callback[(int)EKeyState::Down](DeltaTime);
		}

		if (m_vecKeyState[Index].State[(int)EKeyState::Press] &&
			m_KeyCtrl == iter->second->KeyCtrl &&
			m_KeyAlt == iter->second->KeyAlt &&
			m_KeyShift == iter->second->KeyShift)
		{
			// 호출할 콜백함수가 있을 경우 호출을 해준다.
			if (iter->second->Callback[(int)EKeyState::Press])
				iter->second->Callback[(int)EKeyState::Press](DeltaTime);
		}

		if (m_vecKeyState[Index].State[(int)EKeyState::Up] &&
			m_KeyCtrl == iter->second->KeyCtrl &&
			m_KeyAlt == iter->second->KeyAlt &&
			m_KeyShift == iter->second->KeyShift)
		{
			// 호출할 콜백함수가 있을 경우 호출을 해준다.
			if (iter->second->Callback[(int)EKeyState::Up])
				iter->second->Callback[(int)EKeyState::Up](DeltaTime);
		}
	}
}

bool CInput::CreateKey(const char* pName, unsigned int Key)
{
	PKeyInfo	pInfo = FindKeyInfo(pName);

	// 같은 이름으로 등록된 키가 이미 있을 경우
	// 키를 중복해서 생성하는 것을 멈춘다.
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
