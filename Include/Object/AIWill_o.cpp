#include "AIWill_o.h"
#include "../AI/AITrace.h"
#include "../AI/AIIdle.h"
#include "../AI/AISelectWill_o.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"

CAIWill_o::CAIWill_o()
{
}

CAIWill_o::CAIWill_o(const CAIWill_o& obj)
{
}

CAIWill_o::~CAIWill_o()
{
}

void CAIWill_o::Die()
{
	CAICharacter::Die();
}

bool CAIWill_o::Init()
{
	if (!CAICharacter::Init())
		return false;

	m_AD = 5.f;
	m_HPMax = 10.f;
	m_HP = m_HPMax;

	SetPhysicsSimulate(false);
	SetUpBlock(false);
	m_Raycast = false;

	m_MoveSpeed = 150.f;

	m_vSize = Vector2(40.f, 55.f);

	RectComponent = AddCollider<CColliderRect>("Will_oBody");
	RectComponent->SetExtent(m_vSize);
	RectComponent->SetOffset(0.f,0.f);
	RectComponent->SetCollisionProfile("Monster");
	RectComponent->SetBeginCollisionFunction(this, &CAIWill_o::BeginCollision);

	CreateAI<CAIIdle>("Idle");
	CreateAI<CAITrace>("Trace");

	SetTraceDistance(400.f);
	SetAIFloatData("TraceDistance", 400.f);/*
	SetAIGameObjectData("Target", nullptr);*/

	CreateAISelect<CAISelectWill_o>("Will_oSelect");

	// Idle
	AddAnimation("Idle", AnimationType::Sprite, true, "Enemy_Atlas00_Left", 0.4f);
	AddAnimationFrameData("Idle", 0, 0, 8, Vector2(512.f, 512.f), Vector2(0.53f, 0.55f));

	return true;
}

void CAIWill_o::Start()
{
	CAICharacter::Start();
}

void CAIWill_o::Update(float DeltaTime)
{
	CAICharacter::Update(DeltaTime);

	CCharacter::Update(DeltaTime);
}

void CAIWill_o::Render(HDC hDC, float DeltaTime)
{
	CAICharacter::Render(hDC, DeltaTime);

#ifdef _DUBUG
	SHOW_OBJPOS(m_Name.c_str());
#endif
}

CAIWill_o* CAIWill_o::Clone() const
{
	return new CAIWill_o(*this);
}

void CAIWill_o::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	CAICharacter::BeginCollision(pSrc, pDest, DeltaTime);
}
