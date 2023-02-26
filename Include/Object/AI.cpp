#include "AI.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Object/PlayerEffect.h"
#include "../Object/Bullet.h"
#include "../Scene/SceneMode.h"

CAICharacter::CAICharacter()  :
	m_TraceDistance(0.f),
	m_AttackDistance(0.f),
	m_pAIController(nullptr),
	m_State(AIState::Standby),
	m_bIsAttack(false),
	m_CheckFirstPassBy(false),
	m_AD(0.f)
{
}

CAICharacter::CAICharacter(const CAICharacter& obj)
{
	if (obj.m_pAIController)
	{
		m_pAIController = obj.m_pAIController->Clone();
		m_pAIController->SetOwner(this);
	}

	m_TraceDistance = obj.m_TraceDistance;
	m_AttackDistance = obj.m_AttackDistance;
	m_State = obj.m_State;
	m_bIsAttack = false;
	m_CheckFirstPassBy = false;
	m_AD = obj.m_AD;
}

CAICharacter::~CAICharacter()
{
	SAFE_DELETE(m_pAIController);
}

bool CAICharacter::Init()
{
	if (!CCharacter::Init())
		return false;

	m_MoveSpeed = 250.f;

	CreateAIController<CAIController>();

	SetSize(82.f, 73.f);

	return true;
}

void CAICharacter::Start()
{
	CCharacter::Start();

	SetAIGameObjectData("Target", m_pScene->GetSceneMode()->GetPlayer());
}

void CAICharacter::Update(float DeltaTime)
{
	if (m_pAIController)
		m_pAIController->Run(DeltaTime);

}

void CAICharacter::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);

#ifdef _DEBUG
	switch (m_State)
	{
	case AIState::Attack:
	{	
		char State[MAX_PATH] = "State : Attack";
		TextOutA(hDC, 1000, 560, State, (int)strlen(State));
	}
	break;
	case AIState::Trace:
	{
		char State[MAX_PATH] = "State : Trace";
		TextOutA(hDC, 1000, 560, State, (int)strlen(State));
	}
	break;
	case AIState::Standby:
	{
		char State[MAX_PATH] = "State : Standby";
		TextOutA(hDC, 1000, 560, State, (int)strlen(State));
	}
		break;
	}

#endif
}

void CAICharacter::Die()
{
	CSharedPtr<CPlayerEffect> DieUI = m_pScene->CreateGameObject<CPlayerEffect>("AI_DieUI_Effect", GetCenterPos(), Vector2(256.f, 256.f));
	DieUI->SetPivot(0.5f, 0.5f);
	DieUI->SetLoop(true);
	DieUI->AddAnimation("AI_DieUI_Effect_Anim", AnimationType::Sprite, "UI_Atlass00", .2f);
	DieUI->AddAnimationFrameData("AI_DieUI_Effect_Anim", 6, 11, 2, Vector2(256.f, 256.f));
	DieUI->SetLifeTime(1.f);

	CSharedPtr<CPlayerEffect> DieEffect = m_pScene->CreateGameObject<CPlayerEffect>("AI_Die_Effect", GetCenterPos(), Vector2(512.f, 512.f));
	DieEffect->SetPivot(0.5f, 0.5f);
	DieEffect->AddAnimation("AI_Die_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", .5f);
	DieEffect->AddAnimationFrameData("AI_Die_Effect_Anim", 0, 0, 14, Vector2(512.f, 512.f));
	DieEffect->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("bomb");
	m_pScene->GetResourceManager()->SoundPlay("destroy");

	
	CRef::Destroy();
}

float CAICharacter::TakeDamage(float Damage, CGameObject* DamageObject)
{
	CSharedPtr<CPlayerEffect> pEffect = m_pScene->CreateGameObject<CPlayerEffect>("AI_Damage_Effect", GetCenterPos(), Vector2(512.f, 512.f));
	pEffect->SetPivot(0.5f, 0.5f);
	pEffect->AddAnimation("AI_Damage_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", .3f);
	pEffect->AddAnimationFrameData("AI_Damage_Effect_Anim", 3, 11, 8, Vector2(512.f, 512.f));
	pEffect->SetAutoDestroy();

	CSharedPtr<CPlayerEffect> pEffect2 = m_pScene->CreateGameObject<CPlayerEffect>("AI_Damage2_Effect", GetCenterPos(), Vector2(512.f, 512.f));
	pEffect2->SetPivot(0.5f, 0.5f);
	pEffect2->AddAnimation("AI_Damage2_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", .3f);
	pEffect2->AddAnimationFrameData("AI_Damage2_Effect_Anim", 6, 10, 7, Vector2(512.f, 512.f));
	pEffect2->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("HitEnemy");

	return CCharacter::TakeDamage(Damage, DamageObject);
}

void CAICharacter::SetAIIntData(const std::string& Name, int Data)
{
	if (m_pAIController)
		m_pAIController->SetIntData(Name, Data);
}

void CAICharacter::SetAIFloatData(const std::string& Name, float Data)
{
	if (m_pAIController)
		m_pAIController->SetFloatData(Name, Data);
}

void CAICharacter::SetAIBoolData(const std::string& Name, bool Data)
{
	if (m_pAIController)
		m_pAIController->SetBoolData(Name, Data);
}

void CAICharacter::SetAIStringData(const std::string& Name, std::string Data)
{
	if (m_pAIController)
		m_pAIController->SetStringData(Name, Data);
}

void CAICharacter::SetAIGameObjectData(const std::string& Name, CGameObject* Data)
{
	if (m_pAIController)
		m_pAIController->SetGameObjectData(Name, Data);
}

void CAICharacter::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	if (pDest->GetCollisionProfile()->Name == "PlayerAttack")
	{
		CBullet* pBullet = Cast<CBullet*, CGameObject*>(pDest->GetOwner());

		TakeDamage(pBullet->GetAD(), pBullet);
	}
}