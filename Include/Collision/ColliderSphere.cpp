
#include "ColliderSphere.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../UI/MouseWidget.h"

CColliderSphere::CColliderSphere()
{
	m_Type = ColliderType::Sphere;
}

CColliderSphere::CColliderSphere(const CColliderSphere& collider) :
	CCollider(collider)
{
	m_Info = collider.m_Info;
	m_Offset = collider.m_Offset;
}

CColliderSphere::~CColliderSphere()
{
}

bool CColliderSphere::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderSphere::Start()
{
	CCollider::Start();
}

void CColliderSphere::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);

	Vector2	vPos = m_Owner->GetPos();

	m_Info.Center = m_Offset + vPos;
}

void CColliderSphere::Render(HDC hDC, float DeltaTime)
{
	CCollider::Render(hDC, DeltaTime);

#ifdef _DEBUG

	HPEN	Pen = CGameManager::GetInst()->GetGreenPen();

	if (!m_CollisionList.empty())
		Pen = CGameManager::GetInst()->GetRedPen();

	CCamera* pCamera = m_pScene->GetMainCamera();

	SphereInfo	Info = m_Info;

	Info.Center = m_Info.Center - pCamera->GetPos();

	HGDIOBJ Prev = SelectObject(hDC, Pen);

	MoveToEx(hDC, (int)(Info.Center.x + Info.Radius), (int)Info.Center.y, nullptr);

	for (int i = 1; i <= 12; ++i)
	{
		float	Radian = DegreeToRadian(i * (360.f / 12.f));

		float	x = Info.Center.x + cosf(Radian) * Info.Radius;
		float	y = Info.Center.y + sinf(Radian) * Info.Radius;

		LineTo(hDC, (int)x, (int)y);
	}

	SelectObject(hDC, Prev);

#endif // _DEBUG
}

CColliderSphere* CColliderSphere::Clone() const
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider* pDest)
{
	switch (pDest->GetColliderType())
	{
	case ColliderType::Rect:
		return CCollision::CollisionRectToSphere((CColliderRect*)pDest, this);
	case ColliderType::Sphere:
		return CCollision::CollisionSphereToSphere(this, (CColliderSphere*)pDest);
	case ColliderType::Pixel:
		return CCollision::CollisionSphereToPixel(this, (CColliderPixel*)pDest);
	}

	return false;
}

bool CColliderSphere::CollisionMouse()
{
	CCamera* pCamera = m_pScene->GetMainCamera();

	Vector2	WorldMousePos = CInput::GetInst()->GetMousePos() + pCamera->GetPos();

	float	Dist = WorldMousePos.Distance(m_Info.Center);

	bool	bResult = false;

	if (Dist <= m_Info.Radius)
	{
		CMouseWidget* pMouseWidget = (CMouseWidget*)m_pScene->GetMouseWidget();

		pMouseWidget->SetCollisionObject(this);

		m_bCollisionMouse = true;

		bResult = true;
	}

	else
		m_bCollisionMouse = false;

	return bResult;
}
