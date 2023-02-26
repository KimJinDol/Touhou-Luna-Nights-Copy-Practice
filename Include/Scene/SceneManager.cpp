
#include "SceneManager.h"
#include "../Input.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_pScene(nullptr),
	m_pNextScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	// 생성한 Scene을 제거한다.
	SAFE_DELETE(m_pNextScene);
	SAFE_DELETE(m_pScene);
}

bool CSceneManager::Init()
{
	// 기본 장면을 하나 생성해준다.
	m_pScene = new CScene;

	m_pScene->Init();

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	m_pScene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::Collision(float DeltaTime)
{
	m_pScene->Collision(DeltaTime);

	return ChangeScene();
}

void CSceneManager::Render(HDC hDC, float DeltaTime)
{
	m_pScene->Render(hDC, DeltaTime);
}

CScene* CSceneManager::CreateNextScene()
{
	SAFE_DELETE(m_pNextScene);
	m_pNextScene = new CScene;

	m_pNextScene->Init();

	return m_pNextScene;
}

bool CSceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		// 기존 장면을 제거한다.
		SAFE_DELETE(m_pScene);

		// 입력 콜백을 초기화한다.
		CInput::GetInst()->ClearCallback();

		m_pScene = m_pNextScene;
		m_pNextScene = nullptr;
		return true;
	}

	return false;
}
