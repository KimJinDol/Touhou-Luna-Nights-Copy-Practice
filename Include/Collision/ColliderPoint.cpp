
#include "ColliderPoint.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "Collision.h"
#include "ColliderSphere.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../UI/MouseWidget.h"

CColliderPoint::CColliderPoint()
{
	m_Type = ColliderType::Point;
}

CColliderPoint::CColliderPoint(const CColliderPoint& collider) :
	CCollider(collider)
{
	m_Pos = collider.m_Pos;
	m_Offset = collider.m_Offset;
}

CColliderPoint::~CColliderPoint()
{
}

bool CColliderPoint::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderPoint::Start()
{
	CCollider::Start();
}

void CColliderPoint::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);

	m_Pos = m_Owner->GetPos() + m_Offset;
}

void CColliderPoint::Render(HDC hDC, float DeltaTime)
{
	CCollider::Render(hDC, DeltaTime);

#ifdef _DEBUG

	HBRUSH	Brush = CGameManager::GetInst()->GetGreenBrush();

	if (!m_CollisionList.empty())
		Brush = CGameManager::GetInst()->GetRedBrush();

	CCamera* pCamera = m_pScene->GetMainCamera();

	Vector2	Pos = m_Pos - pCamera->GetPos();

	RECT	rc = { (LONG)(m_Pos.x - 3.f), (LONG)(m_Pos.y - 3.f),
		(LONG)(m_Pos.x + 3.f), (LONG)(m_Pos.y + 3.f) };

	FrameRect(hDC, &rc, Brush);

#endif // _DEBUG
}

CColliderPoint* CColliderPoint::Clone() const
{
	return new CColliderPoint(*this);
}

bool CColliderPoint::Collision(CCollider* pDest)
{
	switch (pDest->GetColliderType())
	{
	case ColliderType::Rect:
		//return CCollision::CollisionRectToRect(this, (CColliderPoint*)pDest);
		break;
	case ColliderType::Sphere:
		//return CCollision::CollisionRectToSphere(this, (CColliderSphere*)pDest);
		break;
	case ColliderType::Point:
		break;
	}

	return false;
}

bool CColliderPoint::CollisionMouse()
{
	CCamera* pCamera = m_pScene->GetMainCamera();

	Vector2	WorldMousePos = CInput::GetInst()->GetMousePos() + pCamera->GetPos();

	bool bResult = WorldMousePos.x == m_Pos.x && WorldMousePos.y == m_Pos.y;

	if (bResult)
	{
		CMouseWidget* pMouseWidget = (CMouseWidget*)m_pScene->GetMouseWidget();

		pMouseWidget->SetCollisionObject(this);

		m_bCollisionMouse = true;
	}

	else
		m_bCollisionMouse = false;

	return bResult;
}
