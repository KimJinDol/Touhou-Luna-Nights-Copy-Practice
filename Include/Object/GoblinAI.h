#pragma once
#include "AI.h"
class CGoblinAI :
    public CAICharacter
{
	friend class CScene;

protected:
	CGoblinAI();
	CGoblinAI(const CGoblinAI& obj);
	virtual ~CGoblinAI();

private:
	CSharedPtr<class CColliderRect> m_AttackColl;
	float	m_AttackCollLimitTime;
	float	m_AttackCollMoveTime;
	bool	m_bAttackCollBack;
	float	m_AttackCollSpeed;
	float	m_StandbyTime;
	float	m_curStandbyTime;
	bool	m_bIsStandby;


public:


public:
	void AttackAnim();
	void Attack();
	void AttackEnd();
	void StanbyEnd();

	virtual void Die();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CGoblinAI* Clone()	const;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
	void BeginAttackCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

