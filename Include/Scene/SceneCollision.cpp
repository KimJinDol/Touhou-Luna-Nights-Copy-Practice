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
	// ���콺�� UI���� �浹�Ѵ�. ����� ���߿� ��µǴ� ��ü�� ���� ��µ� ��ü�� ��� �׸��� ������
	// �浹�� �ݴ�� �ݺ��ϸ� �浹�� �غ��� �Ѵ�.
	// ���콺�� UI�� �浹�غ��� ���� �浹�Ǵ� UI�� �����Ѵٸ� �ٸ� ��� ��ü�� ���ؼ�
	// ���콺�� �浹�� �������ֵ��� �Ѵ�.
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

			// ���� ������Ʈ�� �ҼӵǾ� �ִ� �浹ü ������ �浹�� ���Ѵ�.
			CSharedPtr<CGameObject>	DestObj = pDest->GetOwner();

			if (SrcObj == DestObj)
				continue;

			// ���� �ٸ� ������Ʈ�� �Ҽӵ� �浹ü��� �� �浹ü�� Profile�� ���ͼ� �浹�� �ؾ��ϴ�
			// ��ü������ �Ǵ��Ѵ�.
			CollisionProfile* pDestProfile = pDest->GetCollisionProfile();

			CollisionState	SrcState = pSrcProfile->vecState[(int)pDestProfile->Channel];
			CollisionState	DestState = pDestProfile->vecState[(int)pSrcProfile->Channel];

			// ���� �ϳ��� ����� �浹�� �ϰ� ���� ���� ����� �浹�� �����Ѵ�.
			if (SrcState == CollisionState::Ignore || DestState == CollisionState::Ignore)
			{
				if (pSrc->CheckCollisionList(pDest))
				{
					pSrc->DeleteCollisionList(pDest);
					pDest->DeleteCollisionList(pSrc);
				}

				continue;
			}

			// �浹�� ���´� 2���� ���·� �������ش�.
			// �浹�� �����Ҷ� �浹�� ����ɶ�
			if (SrcState == DestState)
			{
				// �� ��ü�� �浹�Ǿ������� üũ�Ѵ�.
				if (pSrc->Collision(pDest))
				{
					// ������ �浹�ǰ� �־������� �Ǵ��Ѵ�.
					// ���� ó�� �浹�ɶ�
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
						// �浹��Ͽ� ������ �������ش�.
						pSrc->AddCollisionList(pDest);
						pDest->AddCollisionList(pSrc);

						// �浹�� ���۵ɶ� ȣ��� �Լ��� �����Ǿ� �ִٸ� ȣ�����ش�.
						pSrc->CallBeginCollision(pDest, DeltaTime);
						pDest->CallBeginCollision(pSrc, DeltaTime);
					}
				}

				// �浹�� �ȵǾ��� ��� ������ �浹�ϴ� ��ü������ �Ǵ��Ѵ�.
				else if (pSrc->CheckCollisionList(pDest))
				{
					// ���� �浹�Ǵ� ��ü ��Ͽ��� �����Ѵ�.
					pSrc->DeleteCollisionList(pDest);
					pDest->DeleteCollisionList(pSrc);

					// �浹�Ǵٰ� �������� ȣ��� �Լ��� ȣ���Ѵ�.
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
			// ���� ������ �ִ� �ݶ��̴��� �Ǵ�.
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
