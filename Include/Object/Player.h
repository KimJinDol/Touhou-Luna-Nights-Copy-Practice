#pragma once

#include "Character.h"
#include "../interface.h"


enum class PlayerAnimation
{
	None,
	Idle,
	Walk,
	Jump,
	Attack,
	TimeStop,
	SlowTime,
	Damage,
	FallDown,
	Gliding
};

class CPlayer :
    public CCharacter, public ITimeStopInterface, public ISlowTimeInterface
{
	friend class CScene;

protected: 
	CPlayer();
	CPlayer(const CPlayer& obj);
	virtual ~CPlayer();

protected:
	bool	m_bAutoIdle;			// 애니메이션이 끝나면 자동으로 idle로 돌아가게 할 건지.
	bool	m_bIsMove;
	bool	m_bIsJump;
	bool	m_bIsGliding;
	PlayerAnimation	m_PlayerAnim;

	CSharedPtr<class CPlayerWidget>	m_pPlayerWidget;
	CSharedPtr<class CUIPlayerWidgetWorld> m_pSnailWorldWidget;

private:
	// static
	static float s_HP;
	static float s_MP;

	float	m_BlinkTime;
	float	m_curBlinkTime;
	CharacterView	m_DamageView;

	// State
	int		m_Level;
	float	m_MPIncreasePerSec;
	bool	m_bCanInvincible;
	bool	m_bIsInvincible; 
	bool	m_bIsFight;
	bool	m_bIsPlayDamageAnim;

	// basic movement
	bool	m_bCanMove;
	bool	m_bCanJump;
	bool	m_bCanAttack;

	float	m_BasicJumpVelocity;

	bool	m_IsDialogue;
	bool	m_IsInteraction;

	// Action
	bool	m_bIsAction;
	bool	m_bIsSuperAttack;
	float   m_fSuperAttackTime;
	float   m_fCurrentSuperAttackTime;

	int		m_CurCombo;				// current combo count
	int		m_ComboMax;				// max combo count
	bool	m_bNextComboInput;		// jubgement variable that input Next combo action 
	int		m_MPCost_Action;
	CharacterView	m_ActionView;
	CharacterView	m_UpShotView;
	CharacterView	m_DownShotView;

	int		m_UpDownKey;			// 1 = Up, -1 = Down, 0 = None

	// TimeStop
	bool	m_bCanTimeStop;
	bool	m_bIsTimeStop;
	static bool		s_bCanTimeStopSkill;
	static float 	s_TimeStopGauge;
	float	m_TimeStopGaugeIncreaseSpeed;
	int		m_TimeStopKnifeNumMax;
	int		m_TimeStopKnifeNum;

	//float	m_TimeStopKeyInputTime;		// key press time
	//const float	m_TimeStopKeyInputMaxTime = 1.f;

	// Slow Time
	bool	m_bCanSlowTime;
	bool	m_bCanSlowTimeInput; // 공격 버튼을 누르고 0.5초 뒤에 게이지가 누적되도록 할 것
	bool	m_bCheckSnailWidgetEnable;	// Snail 위젯 애니메이션 활성화를 시켰는지 판단하는 변수
	bool	m_bIsSlowTime;
	float	m_SlowTimeGauge;
	float	m_SlowTimeMaxGauge;
	float	m_SlowTimeCanInputGauge;	
	

public:
	virtual bool Init();
	virtual void Start();
	virtual void SetInput();
	virtual void Update(float DeltaTime);
	virtual void Update(float DeltaTime, TimeMode eMode);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CPlayer* Clone()	const;

public:
	void SetEnable(bool bMove = true, bool bJump = true, bool bAttack = true)
	{
		m_bCanMove = bMove;
		m_bCanJump = bJump;
		m_bCanAttack = bAttack;
		m_bCanTimeStop = bAttack;
		m_bCanSlowTime = bAttack;
	}

public:
	CPlayerWidget* GetPlayerWidget()	const
	{
		return m_pPlayerWidget.Get();
	}

	CUIPlayerWidgetWorld* GetPlayerWorldWidget()	const
	{
		return m_pSnailWorldWidget.Get();
	}

	virtual float GetHP()	const
	{
		return s_HP;
	}

	virtual float GetMP()	const
	{
		return s_MP;
	}

public:
	void SetMaxStat();

protected:
	virtual void SetHP(float HP);
	virtual void SetMP(float MP);
	
	void Invincible();
	void ReleaseInvincible();
	void EndPlayDamageAnim()
	{
		m_bAutoIdle = true;
		m_JumpVelocity = m_BasicJumpVelocity;
		m_bIsPlayDamageAnim = false;
		m_bCanTimeStop = true;
		m_bIsRender = true;
		SetEnable();
	}

	void DieAnim();
	virtual void Die();
	void DieEnd();
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);
	void Graze();

private:
	void InteractionKeyInput(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void InitMoveDirX(float DeltaTime);
	void UpKey(float DeltaTime);
	void DownKey(float DeltaTime);
	void ReleaseUpDownKey(float DeltaTime);

	void Jump(float DeltaTime);
	void JumpEnd();

	void GlidingEnd(float DeltaTime);

	// Action
	void ActionInput(float DeltaTime);
	void ActionComboCheck();
	void Action();
	void ActionEnd();

	// UpShot
	void UpShotInput(float DeltaTime);
	void UpShot();
	void UpShotEnd();

	// DownShot
	void DownShotInput(float DeltaTime);
	void DownShot();
	void DownShotEnd();

public:
	// TimeStop
	void EnableTimeStopSkill(bool Value);

private:
	virtual void SetTimeMode(TimeMode eMode);
	void TimeStopInput(float DeltaTime);
	void TimeStopAnimEnd();
	void SetTimeStopGauge(float Gauge);
public:
	void AddTimeStopGauge(float Gauge);

private:
	// SlowTime
	void SlowTimeInput(float DeltaTime);
	void SlowTimeKeyUp(float DeltaTime);
	void SlowTime();
	void SlowTimeAnimEnd();
	void SlowTimeEnd();

public:
	void Dialogue();
	void ReleaseDialogue();

	void LearnSkill(const std::string& Name);

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);

};

