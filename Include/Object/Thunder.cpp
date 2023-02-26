#include "Thunder.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CThunder::CThunder()	:
	m_ActiveTime(2.f),
	m_curActiveTime(0.f),
	m_UnActiveTime(0.4f),
	m_curUnActiveTime(0.f),
	m_AD(35.f)
{
}

CThunder::CThunder(const CThunder& obj)
{
}

CThunder::~CThunder()
{
}

bool CThunder::Init()
{
	if (!CStaticObject::Init())
		return false;

	m_vPivot = Vector2(0.5f, 0.5f);
	m_vSize = Vector2(68.f, 272.f);
	// Collider
	RectComponent = AddCollider<CColliderRect>("ThunderBody");
	RectComponent->SetExtent(m_vSize);
	RectComponent->SetCollisionProfile("Monster");
	

	AddAnimation("ThunderAnim", AnimationType::Sprite, true, "Effect_Atlass00", 0.4f);
	AddAnimationFrameData("ThunderAnim", 0, 16, 8, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
	
	return true;
}

void CThunder::Update(float DeltaTime)
{
	CStaticObject::Update(DeltaTime);

	if (m_bIsRender)
	{
		m_curActiveTime += DeltaTime;

		if (m_curActiveTime> m_ActiveTime)
			ThunderEnd();
	}
	else
	{
		m_curUnActiveTime += DeltaTime;
		if (m_curUnActiveTime > m_UnActiveTime)
			ThunderStart();
	}
}

void CThunder::ThunderStart()
{
	m_bIsRender = true;
	RectComponent->Enable(true);
	m_curUnActiveTime = 0.f;

	if (!m_Culling)
		m_pScene->GetResourceManager()->SoundPlay("thunder");
}

void CThunder::ThunderEnd()
{
	m_bIsRender = false;
	RectComponent->Enable(false);
	m_curActiveTime = 0.f;
}

CThunder* CThunder::Clone() const
{
	return nullptr;
}