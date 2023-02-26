#include "WolfAI.h"
#include "../AI/AISelectWolf.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "../AI/AIIdle.h"
#include "../AI/AITrace.h"
#include "../AI/AIWolfAttack.h"

CWolfAI::CWolfAI()	:
	m_bIsJump(false),
	m_bBackStep(false),
	m_JumpXMove(400.f),
	m_AttackRange(150.f),
	m_bIsPlaySound(false),
	m_BackStepView(CharacterView::Left)
{
}

CWolfAI::CWolfAI(const CWolfAI& obj)    :
    CAICharacter(obj)
{
}

CWolfAI::~CWolfAI()
{
}

bool CWolfAI::Init()
{
    if (!CAICharacter::Init())
        return false;

	m_vPivot = Vector2(0.5f, 1.f);

	SetView(CharacterView::Left);
	SetSize(100.f, 63.f);

	m_AD = 15.f;
	m_HPMax = 20.f;
	m_HP = m_HPMax;

	m_JumpVelocity = 40.f;
	m_GravityAccel = 15.f;

	m_pJumpEndFunc = std::bind(&CWolfAI::JumpEnd, this);

	RectComponent = AddCollider<CColliderRect>("WolfBody");
	RectComponent->SetExtent(m_vSize);
	RectComponent->SetOffset(0.f, -m_vSize.y / 2);
	RectComponent->SetCollisionProfile("Monster");
	RectComponent->SetBeginCollisionFunction(this, &CWolfAI::BeginCollision);

	CreateAI<CAIIdle>("Idle");
	CreateAI<CAITrace>("Trace");
	CreateAI<CAIWolfAttack>("Attack");

	CreateAISelect<CAISelectWolf>("WolfSelect");

    SetTraceDistance(450.f);
    SetAttackDistance(200.f);

	SetAIBoolData("AttackComplete", true);
	SetAIFloatData("TraceDistance", 500.f);
	SetAIFloatData("AttackDistance", m_AttackDistance);


    // Idle
	AddAnimation("Idle", AnimationType::Sprite, true, "Enemy_Atlas00", 0.4f);
	AddAnimationFrameData("Idle", 5, 1, 4, Vector2(512.f, 512.f), Vector2(0.5f, 0.43f));

	AddAnimation("Idle_Left", AnimationType::Sprite, true, "Enemy_Atlas00_Left", 0.4f);
	AddAnimationFrameData("Idle_Left", 5, 1, 4, Vector2(512.f, 512.f), Vector2(0.5f, 0.43f));

	// Run
	AddAnimation("Run", AnimationType::Sprite, true, "Enemy_Atlas00", 0.4f);
	AddAnimationFrameData("Run", 1, 1, 4, Vector2(512.f, 512.f), Vector2(0.5f, 0.42f));

	AddAnimation("Run_Left", AnimationType::Sprite, true, "Enemy_Atlas00_Left", 0.4f);
	AddAnimationFrameData("Run_Left", 1, 1, 4, Vector2(512.f, 512.f), Vector2(0.5f, 0.42f));

	// Attack
	AddAnimation("Attack", AnimationType::Sprite, false, "Enemy_Atlas00", 0.8f);
	AddAnimationFrameData("Attack", 8, 0, 2, Vector2(512.f, 512.f), Vector2(0.5f, 0.4f));
	m_pAnimation->SetNotifyFunction("Attack", 1, this, &CWolfAI::Attack);

	AddAnimation("Attack_Left", AnimationType::Sprite, false, "Enemy_Atlas00_Left", 0.8f);
	AddAnimationFrameData("Attack_Left", 8, 0, 2, Vector2(512.f, 512.f), Vector2(0.5f, 0.4f));
	m_pAnimation->SetNotifyFunction("Attack_Left", 1, this, &CWolfAI::Attack);

	return true;
}

void CWolfAI::Start()
{
	CAICharacter::Start();
}

void CWolfAI::Update(float DeltaTime)
{
	CAICharacter::Update(DeltaTime);

	if (m_bBackStep)
	{
		if (m_vPos.Distance(Vector2(m_pAIController->GetFloatData("TargetPosX"), m_pAIController->GetFloatData("TargetPosY"))) < m_AttackRange)
		{
			// 뒷걸음질 중 플레이어가 반대방향으로 움직인다면 애니도 바꿔준다.
			if (m_pAIController->GetGameObjectData("Target")->GetPos().x < m_vPos.x)
			{
				Move(Vector2(m_MoveSpeed * DeltaTime, 0.f));
				ChangeAnimation("Run_Left");
				m_BackStepView = CharacterView::Left;
			}
			else
			{
				Move(Vector2(-m_MoveSpeed * DeltaTime, 0.f));
				ChangeAnimation("Run");
				m_BackStepView = CharacterView::Right;
			}
		}
	}

	if (m_Jump)
	{
		if (!m_bIsPlaySound)
		{
			m_bIsPlaySound = true;
			m_pScene->GetResourceManager()->SoundPlay("cat");
		}

		/*m_vMove.y -= 1.f;*/
		Move(Vector2(m_JumpXMove * DeltaTime * (m_BackStepView == CharacterView::Right ? 1 : -1), 0.f));
	}
	else if (!m_Jump && m_bIsJump)
		AttackEnd();

	CCharacter::Update(DeltaTime);

	if(!m_bBackStep && m_pAIController->GetCurrentAI()->GetName() == "Trace")
	{
		if (m_vMoveDir.x > 0)
			ChangeAnimation("Run");
		else
			ChangeAnimation("Run_Left");
	}
}

void CWolfAI::AttackAnim()
{
	if (m_bIsAttack) return;

	// 플레이어와 너무 가까우면 뒤로 물러나서 점프 시도
	
	if (/*!m_bBackStep &&*/m_vPos.Distance(Vector2(m_pAIController->GetFloatData("TargetPosX"), m_pAIController->GetFloatData("TargetPosY"))) < m_AttackRange)
	{
		m_bBackStep = true;
		return;
	} 

	if (m_pAIController->GetGameObjectData("Target")->GetPos().x > m_vPos.x)
	{
		m_BackStepView = CharacterView::Right;
		ChangeAnimation("Attack");
	}
	else
	{
		m_BackStepView = CharacterView::Left;
		ChangeAnimation("Attack_Left");
	}

	m_bIsAttack = true;
	m_bBackStep = false;
}

void CWolfAI::Attack()
{
	if (!m_bIsAttack) return;

	m_bIsJump = true;
	CMoveableObject::Jump();
}

void CWolfAI::AttackEnd()
{
	m_bIsPlaySound = false;
	m_bIsAttack = false;
	m_bIsJump = false;
	m_pAIController->SetBoolData("AttackComplete", true);

	if (m_View == CharacterView::Right)
		ChangeAnimation("Idle");
	else
		ChangeAnimation("Idle_Left");
}

void CWolfAI::Die()
{
	CAICharacter::Die();
}

void CWolfAI::Render(HDC hDC, float DeltaTime)
{
#ifdef _DEBUG

	SHOW_OBJPOS("Wolf");

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

	char BackStep[MAX_PATH] = "BackStep : ";
	sprintf_s(BackStep, "BackStep : %d", m_bBackStep);
	TextOutA(hDC, 1000, 540, BackStep, (int)strlen(BackStep));

	char View[MAX_PATH] = "View : ";
	sprintf_s(View, "v : %d", m_View);
	TextOutA(hDC, 1000, 580, View, (int)strlen(View));
	
	char Collision[MAX_PATH] = "Collision : ";
	sprintf_s(Collision, "Collision : %d", RectComponent->GetCollisionProfile()->Channel);
	TextOutA(hDC, 1000, 600, Collision, (int)strlen(Collision));

#endif

	CAICharacter::Render(hDC, DeltaTime);
}

CWolfAI* CWolfAI::Clone() const
{
	return new CWolfAI(*this);
}

void CWolfAI::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	CAICharacter::BeginCollision(pSrc, pDest, DeltaTime);
}
