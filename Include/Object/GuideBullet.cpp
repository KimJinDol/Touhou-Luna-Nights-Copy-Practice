
#include "GuideBullet.h"
#include "BulletEffect.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Smoke.h"

CGuideBullet::CGuideBullet()
{
}

CGuideBullet::CGuideBullet(const CGuideBullet& obj) :
	CMoveableObject(obj)
{
	m_SmokeTime = obj.m_SmokeTime;
	m_SmokeTimeMax = obj.m_SmokeTimeMax;
}

CGuideBullet::~CGuideBullet()
{
}

bool CGuideBullet::Init()
{
	m_MoveSpeed = 500.f;

	m_SmokeTime = 0.f;
	m_SmokeTimeMax = 0.1f;
	m_DelayTime = 0.f;
	m_DelayTimeMax = 2.f;

	EnableTarget(false);

	return true;
}

void CGuideBullet::Start()
{
	CMoveableObject::Start();
}

void CGuideBullet::Update(float DeltaTime)
{
	m_DelayTime += DeltaTime;

	if (m_DelayTime >= m_DelayTimeMax)
	{
		Vector2	vTargetPos;

		if (m_pTarget)
			vTargetPos = m_pTarget->GetPos();

		float	Angle = m_vPos.GetAngle(vTargetPos);

		m_vMoveDir.x = cosf(DegreeToRadian(Angle));
		m_vMoveDir.y = sinf(DegreeToRadian(Angle));

		m_vPos.x += m_vMoveDir.x * m_MoveSpeed * DeltaTime;
		m_vPos.y += m_vMoveDir.y * m_MoveSpeed * DeltaTime;

		m_SmokeTime += DeltaTime;
		if (m_SmokeTime >= m_SmokeTimeMax)
		{
			m_SmokeTime -= m_SmokeTimeMax;

			CSharedPtr<CSmoke>	Smoke = m_pScene->CreateGameObject<CSmoke>("Smoke",
				"Smoke", m_vPos, Vector2(92.5f, 92.5f));
		}
	}

	CMoveableObject::Update(DeltaTime);
}

void CGuideBullet::Render(HDC hDC, float DeltaTime)
{
	/*Ellipse(hDC, (int)(m_vPos.x - m_vSize.x * m_vPivot.x), (int)(m_vPos.y - m_vSize.y * m_vPivot.x),
		(int)(m_vPos.x + m_vSize.x * (1.f - m_vPivot.x)), (int)(m_vPos.y + m_vSize.y * (1.f - m_vPivot.y)));*/

	CMoveableObject::Render(hDC, DeltaTime);
}

CGuideBullet* CGuideBullet::Clone() const
{
	return new CGuideBullet(*this);
}

void CGuideBullet::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	CRef::Destroy();

	m_pScene->GetResourceManager()->SoundPlay("Hit");

	if (m_vMoveDir.x > 0.f)
	{
		CSharedPtr<CBulletEffect> Effect = m_pScene->CreateGameObject<CBulletEffect>("Effect",
			m_vPos, Vector2(178.f, 164.f));

		Effect->AddAnimation("Hit", AnimationType::Sprite, false,
			"HitRight", TEXT("Hit2.bmp"), 0.5f);
		Effect->SetAnimationReverse("Hit", true);

		m_pScene->GetResourceManager()->SetColorKey("HitRight",
			255, 0, 255);

		for (int i = 0; i < 6; ++i)
		{
			Effect->AddAnimationFrameData("Hit",
				Vector2(i * 178.f, 0.f), Vector2(178.f, 164.f));
		}

		Effect->SetAnimationEndFunction<CBulletEffect>("Hit", Effect.Get(),
			&CBulletEffect::EffectEnd);
	}

	else
	{
		CSharedPtr<CBulletEffect> Effect = m_pScene->CreateGameObject<CBulletEffect>("Effect",
			m_vPos, Vector2(178.f, 164.f));

		Effect->AddAnimation("Hit", AnimationType::Sprite, false,
			"HitLeft", TEXT("Hit.bmp"), 0.5f);

		m_pScene->GetResourceManager()->SetColorKey("HitLeft",
			255, 0, 255);

		for (int i = 0; i < 6; ++i)
		{
			Effect->AddAnimationFrameData("Hit",
				Vector2(i * 178.f, 0.f), Vector2(178.f, 164.f));
		}

		Effect->SetAnimationEndFunction<CBulletEffect>("Hit", Effect.Get(),
			&CBulletEffect::EffectEnd);
	}
}
