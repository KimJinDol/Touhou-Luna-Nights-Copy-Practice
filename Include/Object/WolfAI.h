#pragma once
#include "AI.h"

class CWolfAI :
    public CAICharacter
{
	friend class CScene;

protected:
	CWolfAI();
	CWolfAI(const CWolfAI& obj);
	virtual ~CWolfAI();

private:
	float	m_JumpXMove;
	bool	m_bIsJump;
	float   m_AttackRange;	// 물어뜯기(점프)가 가능한 거리
	float	m_BackStepRange;
	bool	m_bBackStep;		// 물어뜯기 가능 거리까지 뒷걸음질
	bool	m_bIsPlaySound;
	CharacterView	m_BackStepView;

public:
	void SetBackStepView(CharacterView View)
	{
		m_BackStepView = View;
	}

public:
	void AttackAnim();
	void Attack();
	void AttackEnd();

	virtual void Die();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CWolfAI* Clone()	const;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

