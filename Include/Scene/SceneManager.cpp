
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
	// ������ Scene�� �����Ѵ�.
	SAFE_DELETE(m_pNextScene);
	SAFE_DELETE(m_pScene);
}

bool CSceneManager::Init()
{
	// �⺻ ����� �ϳ� �������ش�.
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
		// ���� ����� �����Ѵ�.
		SAFE_DELETE(m_pScene);

		// �Է� �ݹ��� �ʱ�ȭ�Ѵ�.
		CInput::GetInst()->ClearCallback();

		m_pScene = m_pNextScene;
		m_pNextScene = nullptr;
		return true;
	}

	return false;
}
