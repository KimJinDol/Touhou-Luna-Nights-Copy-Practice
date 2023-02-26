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
	float   m_AttackRange;	// ������(����)�� ������ �Ÿ�
	float	m_BackStepRange;
	bool	m_bBackStep;		// ������ ���� �Ÿ����� �ް�����
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

