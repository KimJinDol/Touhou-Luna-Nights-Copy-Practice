#include "ColliderRect.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "Collision.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderLine.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../UI/MouseWidget.h"

CColliderRect::CColliderRect()
{
	m_Type = ColliderType::Rect;
	m_Width = 0.f;
	m_Height = 0.f;
}

CColliderRect::CColliderRect(const CColliderRect& collider)	:
	CCollider(collider)
{
	m_Width = collider.m_Width;
	m_Height = collider.m_Height;
	m_Info = collider.m_Info;
}

CColliderRect::~CColliderRect()
{
}

bool CColliderRect::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderRect::Start()
{
	CCollider::Start();
}

void CColliderRect::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);

	Vector2	vPos = m_Owner->GetPos();

	m_Info.Left = vPos.x - m_Width / 2.f + m_vOffsetPos.x;
	m_Info.Right = vPos.x + m_Width / 2.f + m_vOffsetPos.x;
	m_Info.Top = vPos.y - m_Height / 2.f + m_vOffsetPos.y;
	m_Info.Bottom = vPos.y + m_Height / 2.f + m_vOffsetPos.y;
}

void CColliderRect::Render(HDC hDC, float DeltaTime)
{
	CCollider::Render(hDC, DeltaTime);
	
#ifdef _DEBUG

	HBRUSH	Brush = CGameManager::GetInst()->GetGreenBrush();

	if (!m_CollisionList.empty())
		Brush = CGameManager::GetInst()->GetRedBrush();

	CCamera* pCamera = m_pScene->GetMainCamera();

	RectInfo	Info = m_Info;
	Info.Left -= pCamera->GetPos().x;
	Info.Right -= pCamera->GetPos().x;
	Info.Top -= pCamera->GetPos().y;
	Info.Bottom -= pCamera->GetPos().y;

	RECT	rc = { (LONG)Info.Left, (LONG)Info.Top, (LONG)Info.Right, (LONG)Info.Bottom };

	FrameRect(hDC, &rc, Brush);

#endif // _DEBUG
}

CColliderRect* CColliderRect::Clone() const
{
	return new CColliderRect(*this);
}

bool CColliderRect::Collision(CCollider* pDest)
{
	switch (pDest->GetColliderType())
	{
	case ColliderType::Rect:
		return CCollision::CollisionRectToRect(this, (CColliderRect*)pDest);
	case ColliderType::Sphere:
		return CCollision::CollisionRectToSphere(this, (CColliderSphere*)pDest);
	case ColliderType::Pixel:
		return CCollision::CollisionRectToPixel(this, (CColliderPixel*)pDest);
	case ColliderType::Line:
		Vector2 Interect;
		return CCollision::CollisionRectToLine(Interect, this, (CColliderLine*)pDest);
	}
	return false;
}

bool CColliderRect::CollisionMouse()
{
	CCamera* pCamera = m_pScene->GetMainCamera();

	Vector2	WorldMousePos = CInput::GetInst()->GetMousePos() + pCamera->GetPos();

	if (m_Info.Left > WorldMousePos.x)
	{
		m_bCollisionMouse = false;
		return false;
	}

	else if (m_Info.Right < WorldMousePos.x)
	{
		m_bCollisionMouse = false;
		return false;
	}

	else if (m_Info.Top > WorldMousePos.y)
	{
		m_bCollisionMouse = false;
		return false;
	}

	else if (m_Info.Bottom < WorldMousePos.y)
	{
		m_bCollisionMouse = false;
		return false;
	}

	CMouseWidget* pMouseWidget = (CMouseWidget*)m_pScene->GetMouseWidget();

	pMouseWidget->SetCollisionObject(this);

	m_bCollisionMouse = true;

	return true;
}
