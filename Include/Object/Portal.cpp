
#include "Portal.h"
#include "../Collision/ColliderRect.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Stage00Mode.h"
#include "../Scene/Stage01Mode.h"
#include "../Scene/Stage02Mode.h"
#include "../Scene/Stage03Mode.h"
#include "../Scene/Stage04Mode.h"
#include "../Scene/StartScene.h"
#include "../Scene/DungeonSceneMode.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"

CPortal::CPortal()	:
	m_CurrentSceneType(SceneType::None),
	m_NextSceneType(SceneType::None)
{
}

CPortal::CPortal(const CPortal& obj) :
	CGameObject(obj)
{
}

CPortal::~CPortal()
{
}

bool CPortal::Init()
{
	CSharedPtr<CColliderRect>	PortalBody = AddCollider<CColliderRect>("PortalBody");

	Vector2	Size = m_vSize + Vector2(2.f, 2.f);

	PortalBody->SetExtent(Size.x, Size.y);
	PortalBody->SetOffset(m_vSize.x / 2.f, m_vSize.y / 2.f);
	PortalBody->SetCollisionProfile("PlayerTrigger");

	PortalBody->SetBeginCollisionFunction(this, &CPortal::BeginCollision);


	return true;
}

void CPortal::Start()
{
	CGameObject::Start();
}

void CPortal::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPortal::Update(float DeltaTime, TimeMode eMode)
{
	Update(DeltaTime);
}

void CPortal::Render(HDC hDC, float DeltaTime)
{
	/*Rectangle(hDC, (int)(m_vPos.x - m_vSize.x * m_vPivot.x), (int)(m_vPos.y - m_vSize.y * m_vPivot.x),
		(int)(m_vPos.x + m_vSize.x * (1.f - m_vPivot.x)), (int)(m_vPos.y + m_vSize.y * (1.f - m_vPivot.y)));*/

	CGameObject::Render(hDC, DeltaTime);
}

CPortal* CPortal::Clone() const
{
	return new CPortal(*this);
}

void CPortal::SetTimeMode(TimeMode eMode)
{
}

void CPortal::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	m_pScene->GetResourceManager()->SoundPlay("Portal");

	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();
	CCamera* pCamera = pScene->GetMainCamera();

	switch (m_NextSceneType)
	{
	case SceneType::Stage00:
	{
		CSceneManager::GetInst()->CreateSceneMode<CStage00Mode>(false)->
			SetPlayerSpawnPos(Vector2(3842.f, m_pScene->GetSceneMode()->GetPlayer()->GetPos().y));

		pCamera->SetPos(pCamera->GetWorldSize().x - pCamera->GetResolution().x, 350.f);
	}
	break;
	case SceneType::Stage01:
	{
		if (m_CurrentSceneType == SceneType::Stage00)
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage01Mode>(false)->
				SetPlayerSpawnPos(Vector2(34.f, m_pScene->GetSceneMode()->GetPlayer()->GetPos().y));

			pCamera->SetPos(0.f, 350.f);
		}

		else
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage01Mode>(false)->
				SetPlayerSpawnPos(Vector2(2760.f, m_pScene->GetSceneMode()->GetPlayer()->GetPos().y));

			pCamera->SetPos(pCamera->GetWorldSize().x - pCamera->GetResolution().x, 350.f);
		}
	}
	break;
	case SceneType::Stage02:
	{
		if (m_CurrentSceneType == SceneType::Stage01)
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage02Mode>(false)->
				SetPlayerSpawnPos(Vector2(34.f, m_pScene->GetSceneMode()->GetPlayer()->GetPos().y));

			pCamera->SetPos(0.f, 350.f);
		}

		else
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage02Mode>(false)->
				SetPlayerSpawnPos(Vector2(3000.f, m_pScene->GetSceneMode()->GetPlayer()->GetPos().y) - 1650.f);

			pCamera->SetPos(pCamera->GetWorldSize().x - pCamera->GetResolution().x, 350.f);
		}
	}
	break;
	case SceneType::Stage03:
	{
		if (m_CurrentSceneType == SceneType::Stage02)
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage03Mode>(false)->
				SetPlayerSpawnPos(Vector2(34.f, 1630.f + m_pScene->GetSceneMode()->GetPlayer()->GetPos().y));

			pCamera->SetPos(0.f, 3000.f);
		}

		else
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage03Mode>(false)->
				SetPlayerSpawnPos(Vector2(1345.f, 1019.f));

			pCamera->SetPos(78.f, 659.f);
		}
	}
	break;
	case SceneType::Stage04:
	{
		if (m_CurrentSceneType == SceneType::Stage03)
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage04Mode>(false)->
				SetPlayerSpawnPos(Vector2(34.f, 611.f));

			pCamera->SetPos(0.f, 0.f);
		}

		else
		{
			CSceneManager::GetInst()->CreateSceneMode<CStage04Mode>(false)->
				SetPlayerSpawnPos(Vector2(2788.f, m_pScene->GetSceneMode()->GetPlayer()->GetPos().y));

			pCamera->SetPos(pCamera->GetWorldSize().x - pCamera->GetResolution().x, 350.f);
		}
	}
	break;
	case SceneType::StageEnd:
	{
		CSceneManager::GetInst()->CreateSceneMode<CStartScene>(false);
		pCamera->SetPos(0.f, 0.f);
	}
	break;
	}
}
