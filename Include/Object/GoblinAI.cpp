#include "GoblinAI.h"
#include "../AI/AISelectGoblin.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "../AI/AIIdle.h"
#include "../AI/AITrace.h"
#include "../AI/AIGoblinAttack.h"

CGoblinAI::CGoblinAI() :
	m_AttackCollLimitTime(0.3f),
	m_AttackCollMoveTime(0.f),
	m_bAttackCollBack(false),
	m_AttackCollSpeed(600.f),
	m_StandbyTime(2.f),
	m_curStandbyTime(0.f),
	m_bIsStandby(false)
{
}

CGoblinAI::CGoblinAI(const CGoblinAI& obj)
{
}

CGoblinAI::~CGoblinAI()
{

}

void CGoblinAI::AttackAnim()
{
	if (m_bIsAttack) return;

	m_bIsAttack = true;

	if (m_View == CharacterView::Right)
		ChangeAnimation("Attack");
	else
		ChangeAnimation("Attack_Left");
}

void CGoblinAI::Attack()
{
	if (!m_bIsAttack) return;
	
	if (m_View == CharacterView::Right)
		m_AttackColl->SetOffset(m_vSize.x / 2, m_vSize.y / -2);
	else
		m_AttackColl->SetOffset(m_vSize.x / -2, m_vSize.y / -2);

	m_AttackColl->Enable(true);
}

void CGoblinAI::AttackEnd()
{
	m_AttackCollMoveTime = 0.f;
	m_AttackColl->SetOffset(m_vSize / -2);
	m_AttackColl->Enable(false);
	m_bIsStandby = true;

	if (m_View == CharacterView::Right)
		ChangeAnimation("Idle");
	else
		ChangeAnimation("Idle_Left");
}

void CGoblinAI::StanbyEnd()
{
	m_bIsStandby = false;
	m_curStandbyTime = 0.f;
	m_bIsAttack = false;
	m_pAIController->SetBoolData("AttackComplete", true);
}

void CGoblinAI::Die()
{
	CAICharacter::Die();
}

bool CGoblinAI::Init()
{
	if (!CAICharacter::Init())
		return false;

	m_vPivot = Vector2(0.5f, 1.f);

	SetView(CharacterView::Left);
	SetSize(100.f, 150.f);

	m_AD = 15.f;
	m_HPMax = 20.f;
	m_HP = m_HPMax;

	m_GravityAccel = 15.f;

	RectComponent = AddCollider<CColliderRect>("GoblinBody");
	RectComponent->SetExtent(m_vSize);
	RectComponent->SetOffset(0.f, -m_vSize.y / 2);
	RectComponent->SetCollisionProfile("Monster");
	RectComponent->SetBeginCollisionFunction(this, &CGoblinAI::BeginCollision);

	m_AttackColl = AddCollider<CColliderRect>("AttackColl");
	m_AttackColl->SetExtent(50.f, 50.f);
	m_AttackColl->SetCollisionProfile("MonsterAttack");
	m_AttackColl->SetBeginCollisionFunction(this, &CGoblinAI::BeginAttackCollision);
	m_AttackColl->Enable(false);

	CreateAI<CAIIdle>("Idle");
	CreateAI<CAITrace>("Trace");
	CreateAI<CAIGoblinAttack>("GoblinAttack");

	CreateAISelect<CAISelectGoblin>("GoblinSelect");

	SetTraceDistance(500.f);
	SetAttackDistance(350.f);

	SetAIBoolData("AttackComplete", true);
	SetAIFloatData("TraceDistance", 200.f);
	SetAIFloatData("AttackDistance", m_AttackDistance);


	// Idle
	AddAnimation("Idle", AnimationType::Sprite, true, "Enemy_Atlas00", 0.4f);
	AddAnimationFrameData("Idle", 5, 4, 6, Vector2(512.f, 512.f), Vector2(0.52, 0.3));

	AddAnimation("Idle_Left", AnimationType::Sprite, true, "Enemy_Atlas00_Left", 0.4f);
	AddAnimationFrameData("Idle_Left", 5, 4, 6, Vector2(512.f, 512.f), Vector2(0.47, 0.3));

	// Attack
	AddAnimation("Attack", AnimationType::Sprite, false, "Enemy_Atlas00", 2.f);
	AddAnimationFrameData("Attack", 9, 1, 20, Vector2(512.f, 512.f), Vector2(0.52, 0.3));
	AddAnimationFrameData("Attack", 0,4,6, Vector2(512.f, 512.f), Vector2(0.52, 0.3));
	m_pAnimation->SetNotifyFunction("Attack", 16, this, &CGoblinAI::Attack);
	m_pAnimation->SetEndFunction("Attack", this, &CGoblinAI::AttackEnd);


	AddAnimation("Attack_Left", AnimationType::Sprite, false, "Enemy_Atlas00_Left", 2.f);
	AddAnimationFrameData("Attack_Left", 9, 1, 21, Vector2(512.f, 512.f), Vector2(0.47, 0.3));
	AddAnimationFrameData("Attack_Left", 1, 4, 5, Vector2(512.f, 512.f), Vector2(0.47, 0.3));
	m_pAnimation->SetNotifyFunction("Attack_Left", 16, this, &CGoblinAI::Attack);
	m_pAnimation->SetEndFunction("Attack_Left", this, &CGoblinAI::AttackEnd);
}

void CGoblinAI::Start()
{
	CAICharacter::Start();
}

void CGoblinAI::Update(float DeltaTime)
{
	CAICharacter::Update(DeltaTime);

	if (m_State == AIState::Standby)
	{
		if (m_View == CharacterView::Right)
			ChangeAnimation("Idle");
		else
			ChangeAnimation("Idle_Left");
	}

	if (m_AttackColl->IsEnable() && m_bIsAttack)
	{
		m_AttackCollMoveTime += DeltaTime;
		if (m_AttackCollMoveTime < m_AttackCollLimitTime)
		{
			if (m_View == CharacterView::Right)
				m_AttackColl->AddOffset(DeltaTime * m_AttackCollSpeed, 0.f);
			else
				m_AttackColl->AddOffset(-DeltaTime * m_AttackCollSpeed, 0.f);
		}
		else
			m_AttackColl->Enable(false);
	}

	if (m_bIsStandby)
	{
		m_curStandbyTime += DeltaTime;
		if (m_curStandbyTime >= m_StandbyTime)
			StanbyEnd();
	}

	CCharacter::Update(DeltaTime);
}

void CGoblinAI::Render(HDC hDC, float DeltaTime)
{
#ifdef _DEBUG

	SHOW_OBJPOS("Goblin");

	char Move[MAX_PATH] = "MoveDir : ";
	sprintf_s(Move, "MoveDir : %d, %d", (int)m_vMoveDir.x, (int)m_vMoveDir.y);
	TextOutA(hDC, 1000, 420, Move, (int)strlen(Move));

	char MoveDir[MAX_PATH] = "Move : ";
	sprintf_s(MoveDir, "Move : %d, %d", (int)m_vMove.x, (int)m_vMove.y);
	TextOutA(hDC, 1000, 440, MoveDir, (int)strlen(MoveDir));

	char IsGround[MAX_PATH] = "IsGround : ";
	sprintf_s(IsGround, "IsGround : %d", (int)(m_IsGround));
	TextOutA(hDC, 1000, 460, IsGround, (int)strlen(IsGround));

	char IsJump[MAX_PATH] = "IsJump : ";
	sprintf_s(IsJump, "IsJump : %d", (int)(m_bIsAttack));
	TextOutA(hDC, 1000, 480, IsJump, (int)strlen(IsJump));

	char Raycast[MAX_PATH] = "Raycast : ";
	sprintf_s(Raycast, "Raycast : %d, %d", (int)GetHitResult().hitPoint.x, (int)(GetHitResult().hitPoint.y));
	TextOutA(hDC, 1000, 500, Raycast, (int)strlen(Raycast));

	char HP[MAX_PATH] = "HP : ";
	sprintf_s(HP, "HP : %f", m_HP);
	TextOutA(hDC, 1000, 520, HP, (int)strlen(HP));

	char View[MAX_PATH] = "View : ";
	sprintf_s(View, "v : %d", m_View);
	TextOutA(hDC, 1000, 580, View, (int)strlen(View));

	char Collision[MAX_PATH] = "Collision : ";
	sprintf_s(Collision, "Collision : %d", RectComponent->GetCollisionProfile()->Channel);
	TextOutA(hDC, 1000, 600, Collision, (int)strlen(Collision));

	char AttackCollEnable[MAX_PATH] = "AttackCollEnable : ";
	sprintf_s(AttackCollEnable, "AttackCollEnable : %d", m_AttackColl->IsEnable());
	TextOutA(hDC, 1000, 620, AttackCollEnable, (int)strlen(AttackCollEnable));

#endif

	CAICharacter::Render(hDC, DeltaTime);
}

CGoblinAI* CGoblinAI::Clone() const
{
    return nullptr;
}

void CGoblinAI::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	CAICharacter::BeginCollision(pSrc, pDest, DeltaTime);
}

void CGoblinAI::BeginAttackCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	//pDest->DeleteCollisionList(pSrc);
	//pSrc->DeleteCollisionList(pDest);

	//pSrc->GetOwner()->TakeDamage(m_AD, this);

	//m_AttackColl->Enable(false);
	//m_AttackColl->SetOffset(m_vSize.x / 2, m_vSize.y / -2);
}
