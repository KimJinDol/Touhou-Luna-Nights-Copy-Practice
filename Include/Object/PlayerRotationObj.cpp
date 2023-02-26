
#include "PlayerRotationObj.h"
#include "../Collision/ColliderRect.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderSphere.h"

CPlayerRotationObj::CPlayerRotationObj()	:
	m_TargetLength(100.f),
	m_Angle(0.f)
{
}

CPlayerRotationObj::CPlayerRotationObj(const CPlayerRotationObj& obj) :
	CMoveableObject(obj)
{
}

CPlayerRotationObj::~CPlayerRotationObj()
{
}

bool CPlayerRotationObj::Init()
{
	SetPivot(0.5f, 0.5f);

	CSharedPtr<CColliderRect>	PortalBody = AddCollider<CColliderRect>("PlayerRotationBody");

	Vector2	Size = m_vSize + Vector2(2.f, 2.f);

	PortalBody->SetExtent(Size.x, Size.y);
	PortalBody->SetCollisionProfile("Player");

	PortalBody->SetBeginCollisionFunction(this, &CPlayerRotationObj::BeginCollision);

	return true;
}

void CPlayerRotationObj::Start()
{
	CMoveableObject::Start();
}

void CPlayerRotationObj::Update(float DeltaTime)
{
	CMoveableObject::Update(DeltaTime);

	m_Angle += 180.f * DeltaTime;

	float	Radian = DegreeToRadian(m_Angle);

	Vector2	vTargetCenter;
	vTargetCenter	= m_pTarget->GetPos() + m_pTarget->GetSize() * 
		(Vector2(0.5f, 0.5f) - m_pTarget->GetPivot());

	m_vPos.x = vTargetCenter.x + cosf(Radian) * m_TargetLength;
	m_vPos.y = vTargetCenter.y + sinf(Radian) * m_TargetLength;
}

void CPlayerRotationObj::Render(HDC hDC, float DeltaTime)
{
	/*Ellipse(hDC, (int)(m_vPos.x - m_vSize.x * m_vPivot.x), (int)(m_vPos.y - m_vSize.y * m_vPivot.x),
		(int)(m_vPos.x + m_vSize.x * (1.f - m_vPivot.x)), (int)(m_vPos.y + m_vSize.y * (1.f - m_vPivot.y)));*/

	CMoveableObject::Render(hDC, DeltaTime);
}

CPlayerRotationObj* CPlayerRotationObj::Clone() const
{
	return new CPlayerRotationObj(*this);
}

void CPlayerRotationObj::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
}
