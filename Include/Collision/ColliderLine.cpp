#include "ColliderLine.h"
#include "ColliderRect.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "Collision.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../UI/MouseWidget.h"
#include "../Stage/Tile.h"

CColliderLine::CColliderLine()
{
    m_Type = ColliderType::Line;
    m_StartPoint = Vector2(0.f, 0.f);
    m_EndPoint = Vector2(0.f, 0.f);
}

CColliderLine::CColliderLine(const CColliderLine& collider)
{
    m_StartPoint = collider.m_StartPoint;
    m_EndPoint = collider.m_EndPoint;
}

CColliderLine::~CColliderLine()
{
}

bool CColliderLine::Init()
{
    if (!CCollider::Init())
        return false;


    return true;
}

void CColliderLine::Start()
{
    CCollider::Start();
}

void CColliderLine::Update(float DeltaTime)
{
    CCollider::Update(DeltaTime);
}

void CColliderLine::Render(HDC hDC, float DeltaTime)
{
    CCollider::Render(hDC, DeltaTime);

#ifdef _DEBUG

	HPEN	Pen = CGameManager::GetInst()->GetGreenPen();
    
	if (!m_CollisionList.empty())
		Pen = CGameManager::GetInst()->GetRedPen();

	CCamera* pCamera = m_pScene->GetMainCamera();

    Vector2 StartPoint = m_StartPoint - pCamera->GetPos();
    Vector2 EndPoint = m_EndPoint - pCamera->GetPos();

    HGDIOBJ Prev = SelectObject(hDC, Pen);

	MoveToEx(hDC, (int)StartPoint.x, (int)StartPoint.y, nullptr);

    LineTo(hDC, (int)EndPoint.x, (int)EndPoint.y);

    SelectObject(hDC, Prev);
#endif // _DEBUG
}

CColliderLine* CColliderLine::Clone() const
{
    return new CColliderLine(*this);
}

bool CColliderLine::Collision(CCollider* pDest)
{
    Vector2 vInterect;
	switch (pDest->GetColliderType())
	{
	case ColliderType::Rect:
        return CCollision::CollisionRectToLine(vInterect, (CColliderRect*)pDest, this);
	case ColliderType::Sphere:
        break;
    case ColliderType::Line:
        return CCollision::CollisionLineToLine(vInterect, m_StartPoint, m_EndPoint, ((CColliderLine*)pDest)->m_StartPoint, ((CColliderLine*)pDest)->m_EndPoint);
	}

	return false;
}

bool CColliderLine::Collision(CCollider* pDest, Vector2& vInterect)
{
	switch (pDest->GetColliderType())
	{
	case ColliderType::Rect:
		return CCollision::CollisionRectToLine(vInterect, (CColliderRect*)pDest, this);
	case ColliderType::Sphere:
		break;
	case ColliderType::Line:
        return CCollision::CollisionLineToLine(vInterect, m_StartPoint, m_EndPoint, ((CColliderLine*)pDest)->m_StartPoint, ((CColliderLine*)pDest)->m_EndPoint);
	}

	return false;
}

bool CColliderLine::CollisionMouse()
{
    return false;
}
