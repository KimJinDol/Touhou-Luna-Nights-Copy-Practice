#include "SceneCollision.h"
#include "../Collision/Collider.h"
#include "../Collision/ColliderLine.h"
#include "../Object/GameObject.h"
#include "../UI/UIWidget.h"
#include "../GameManager.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Object/MoveableObject.h"

CSceneCollision::CSceneCollision()
{
	m_vecCollider.reserve(200);
}

CSceneCollision::~CSceneCollision()
{
	auto iter = m_listRaycast.begin();
	auto iterEnd = m_listRaycast.end();

	for (; iter != iterEnd; iter++)
	{
		delete (*iter);
	}
}

void CSceneCollision::AddCollider(CCollider* pCollider)
{
	m_vecCollider.push_back(pCollider);
}

void CSceneCollision::CollisionMouse(float DeltaTime, std::vector<class CUIWidget*>& WidgetList)
{
	// 마우스와 UI들을 충돌한다. 출력은 나중에 출력되는 물체가 먼저 출력된 물체를 덮어서 그리기 때문에
	// 충돌은 반대로 반복하며 충돌을 해봐야 한다.
	// 마우스와 UI를 충돌해보고 만약 충돌되는 UI가 존재한다면 다른 모든 물체에 대해서
	// 마우스와 충돌을 제거해주도록 한다.
	auto	iterUI = WidgetList.begin();
	auto	iterUIEnd = WidgetList.end();

	for (; iterUI != iterUIEnd; )
	{
		if ((*iterUI)->IsDestroy())
		{
			iterUI = WidgetList.erase(iterUI);
			iterUIEnd = WidgetList.end();
			continue;
		}

		++iterUI;
	}

	auto iterUIReverse = WidgetList.rbegin();
	auto iterUIEndReverse = WidgetList.rend();

	for (; iterUIReverse != iterUIEndReverse; ++iterUIReverse)
	{
		if (!(*iterUIReverse)->IsEnable())
			continue;

		if ((*iterUIReverse)->CollisionMouse())
			break;
	}

}

void CSceneCollision::Collision(float DeltaTime)
{
	size_t	Size = m_vecCollider.size();

	if (Size <= 1)
	{
		m_vecCollider.clear();
		return;
	}

	for (size_t i = 0; i < Size; ++i)
	{
			CCollider* pSrc = m_vecCollider[i];
			pSrc->SetBlock(false);

			CSharedPtr<CGameObject> SrcObj = pSrc->GetOwner();

			CollisionProfile* pSrcProfile = pSrc->GetCollisionProfile();

		for (size_t j = i + 1; j < Size; ++j)
		{
			CCollider* pDest = m_vecCollider[j];

			// 같은 오브젝트에 소속되어 있는 충돌체 끼리는 충돌을 안한다.
			CSharedPtr<CGameObject>	DestObj = pDest->GetOwner();

			if (SrcObj == DestObj)
				continue;

			// 서로 다른 오브젝트에 소속된 충돌체라면 더 충돌체의 Profile을 얻어와서 충돌을 해야하는
			// 물체들인지 판단한다.
			CollisionProfile* pDestProfile = pDest->GetCollisionProfile();

			CollisionState	SrcState = pSrcProfile->vecState[(int)pDestProfile->Channel];
			CollisionState	DestState = pDestProfile->vecState[(int)pSrcProfile->Channel];

			// 둘중 하나라도 상대방과 충돌을 하고 싶지 않을 경우라면 충돌을 무시한다.
			if (SrcState == CollisionState::Ignore || DestState == CollisionState::Ignore)
			{
				if (pSrc->CheckCollisionList(pDest))
				{
					pSrc->DeleteCollisionList(pDest);
					pDest->DeleteCollisionList(pSrc);
				}

				continue;
			}

			// 충돌의 상태는 2가지 상태로 나누어준다.
			// 충돌을 시작할때 충돌이 종료될때
			if (SrcState == DestState)
			{
				// 두 물체가 충돌되었는지를 체크한다.
				if (pSrc->Collision(pDest))
				{
					// 이전에 충돌되고 있었는지를 판단한다.
					// 지금 처음 충돌될때
					if (SrcState == CollisionState::Block)
					{
						pSrc->SetBlock(true);
						bool Left = false;
						bool Right = false;
						bool Top = false;
						bool Bottom = false;

						if (pSrc->GetOwner() && pDest->GetOwner() && pSrc->GetOwner()->GetPos().x < pDest->GetOwner()->GetPos().x)
							Right = true;
						if (pSrc->GetOwner() && pDest->GetOwner() && pSrc->GetOwner()->GetPos().x > pDest->GetOwner()->GetPos().x)
							Left = true;
						/*if (pSrc->GetOwner() && pDest->GetOwner() && pSrc->GetOwner()->GetPos().y > pDest->GetOwner()->GetPos().y)
							Top = true;
						if (pSrc->GetOwner() && pDest->GetOwner() && pSrc->GetOwner()->GetPos().y < pDest->GetOwner()->GetPos().y)
							Bottom = true;*/

						pSrc->SetBlockRect(Left, Right, Top, Bottom);
						pDest->SetBlockRect(Right, Left, Bottom, Top);
					}

					if (!pSrc->CheckCollisionList(pDest))
					{
						// 충돌목록에 상대방을 지정해준다.
						pSrc->AddCollisionList(pDest);
						pDest->AddCollisionList(pSrc);

						// 충돌이 시작될때 호출될 함수가 지정되어 있다면 호출해준다.
						pSrc->CallBeginCollision(pDest, DeltaTime);
						pDest->CallBeginCollision(pSrc, DeltaTime);
					}
				}

				// 충돌이 안되었을 경우 이전에 충돌하던 물체인지를 판단한다.
				else if (pSrc->CheckCollisionList(pDest))
				{
					// 서로 충돌되던 물체 목록에서 제거한다.
					pSrc->DeleteCollisionList(pDest);
					pDest->DeleteCollisionList(pSrc);

					// 충돌되다가 떨어질때 호출될 함수를 호출한다.
					pSrc->CallEndCollision(pDest, DeltaTime);
					pDest->CallEndCollision(pSrc, DeltaTime);
				}
			}
		}

		// Raycast
		auto iter = m_listRaycast.begin();
		auto iterEnd = m_listRaycast.end();

		Vector2 tempHitResult;
		for (; iter != iterEnd; iter++)
		{
			// 가장 가까이 있는 콜라이더를 판단.
			if (pSrc->GetOwner() == (*iter)->Owner) continue;

			CollisionProfile* RayProfile = (*iter)->Line->GetCollisionProfile();

			CollisionState RayState = RayProfile->vecState[(int)pSrcProfile->Channel];
			CollisionState SrcState = pSrcProfile->vecState[(int)RayProfile->Channel];

			if (RayState == CollisionState::Block && SrcState == CollisionState::Block)
			{
				if ((*iter)->Line->Collision(pSrc, tempHitResult))
				{
					float Dist1 = (*iter)->Line->GetStartPoint().Distance(tempHitResult); // temp
					float Dist2 = (*iter)->Line->GetStartPoint().Distance((*iter)->hitResult.hitPoint); // hitPoint

					// if more close then prior hit point
					if (Dist1 < Dist2 || (*iter)->hitResult.hitPoint.y == -1.f)
					{
						(*iter)->hitResult.hitCollider = pSrc;
						(*iter)->hitResult.hitPoint = tempHitResult;
					}
				}
			}
		}
	}

	auto iter = m_listRaycast.begin();
	auto iterEnd = m_listRaycast.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->hitResult.hitCollider)
		{
			(*iter)->hitResult.Callback((*iter)->hitResult);
		}
	}

	m_vecCollider.clear();
}

void CSceneCollision::Render(HDC hDC, float DeltaTime)
{
	auto iter = m_listRaycast.begin();
	auto iterEnd = m_listRaycast.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->hitResult.hitCollider && Cast<CColliderLine*, CCollider*>((*iter)->hitResult.hitCollider))
		{
#ifdef _DEBUG
			HPEN	Pen = CGameManager::GetInst()->GetPurplePen();

			CCamera* pCamera = m_pScene->GetMainCamera();
			Vector2 StartPoint = ((CColliderLine*)(*iter)->hitResult.hitCollider)->m_StartPoint - pCamera->GetPos();
			Vector2 EndPoint = ((CColliderLine*)(*iter)->hitResult.hitCollider)->m_EndPoint - pCamera->GetPos();

			HGDIOBJ Prev = SelectObject(hDC, Pen);

			MoveToEx(hDC, (int)StartPoint.x, (int)StartPoint.y, nullptr);

			LineTo(hDC, (int)EndPoint.x, (int)EndPoint.y);

			SelectObject(hDC, Prev);
#endif // _DEBUG
		}
		{
#ifdef _DEBUG
			HPEN	Pen = CGameManager::GetInst()->GetPurplePen();

			CCamera* pCamera = m_pScene->GetMainCamera();
			Vector2 StartPoint = (*iter)->Line->m_StartPoint - pCamera->GetPos();
			Vector2 EndPoint = (*iter)->Line->m_EndPoint - pCamera->GetPos();

			HGDIOBJ Prev = SelectObject(hDC, Pen);

			MoveToEx(hDC, (int)StartPoint.x, (int)StartPoint.y, nullptr);

			LineTo(hDC, (int)EndPoint.x, (int)EndPoint.y);

			SelectObject(hDC, Prev);
#endif // _DEBUG
		}
		delete (*iter);

	}

	m_listRaycast.clear();

}

void CSceneCollision::Raycast(HitResult hitResult, CGameObject* Owner, CollisionChannel Channel, const Vector2& StartPos, const Vector2& RayDir, float Distance)
{
	RaycastInfo* pInfo = new RaycastInfo(hitResult);

	pInfo->Owner = Owner;
	pInfo->Line = new CColliderLine;
	pInfo->Line->SetCollisionProfile("Raycast");
	pInfo->Line->SetStartPoint(StartPos);
	pInfo->Line->SetEndPoint(StartPos + (RayDir * Distance));
	
	m_listRaycast.push_back(pInfo);
}
