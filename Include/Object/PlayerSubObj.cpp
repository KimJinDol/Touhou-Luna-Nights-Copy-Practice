
#include "PlayerSubObj.h"
#include "../Collision/ColliderRect.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderSphere.h"

CPlayerSubObj::CPlayerSubObj()	:
	m_FireTime(0.f),
	m_FireTimeMax(1.f)
{
}

CPlayerSubObj::CPlayerSubObj(const CPlayerSubObj& obj) :
	CMoveableObject(obj)
{
}

CPlayerSubObj::~CPlayerSubObj()
{
}

bool CPlayerSubObj::Init()
{
	SetPivot(0.5f, 0.5f);

	CSharedPtr<CColliderRect>	PortalBody = AddCollider<CColliderRect>("PlayerSubBody");

	Vector2	Size = m_vSize + Vector2(2.f, 2.f);

	PortalBody->SetExtent(Size.x, Size.y);
	PortalBody->SetCollisionProfile("Player");

	PortalBody->SetBeginCollisionFunction(this, &CPlayerSubObj::BeginCollision);

	return true;
}

void CPlayerSubObj::Start()
{
	CMoveableObject::Start();
}

void CPlayerSubObj::Update(float DeltaTime)
{
	CMoveableObject::Update(DeltaTime);

	m_FireTime += DeltaTime;

	if (m_FireTime >= m_FireTimeMax)
	{
		m_FireTime -= m_FireTimeMax;

		Vector2	vSize(25.f, 25.f);

		Vector2	vPos;
		vPos.x = m_vPos.x + m_vSize.x * (1.f - m_vPivot.x) + 12.5f;
		vPos.y = m_vPos.y + m_vSize.y * (0.5f - m_vPivot.y);

		// 총알을 발사한다.
		CSharedPtr<CBullet>	Bullet = m_pScene->CreateGameObject<CBullet>("Bullet",
			vPos, vSize);

		Bullet->SetPivot(0.5f, 0.5f);
		Bullet->SetMoveDir(1.f, 0.f);

		CSharedPtr<CColliderSphere>	BulletBody = Bullet->AddCollider<CColliderSphere>("PlayerBullet");

		BulletBody->SetRadius(13.5f);
		//BulletBody->SetExtent(52.f, 52.f);
		BulletBody->SetCollisionProfile("PlayerAttack");

		BulletBody->SetBeginCollisionFunction(Bullet.Get(), &CBullet::BeginCollision);
	}
}

void CPlayerSubObj::Render(HDC hDC, float DeltaTime)
{
	/*Ellipse(hDC, (int)(m_vPos.x - m_vSize.x * m_vPivot.x), (int)(m_vPos.y - m_vSize.y * m_vPivot.x),
		(int)(m_vPos.x + m_vSize.x * (1.f - m_vPivot.x)), (int)(m_vPos.y + m_vSize.y * (1.f - m_vPivot.y)));*/

	CMoveableObject::Render(hDC, DeltaTime);
}

CPlayerSubObj* CPlayerSubObj::Clone() const
{
	return new CPlayerSubObj(*this);
}

void CPlayerSubObj::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
}
