#pragma once
#include "Character.h"
#include "../Interface.h"

class CAICharacter :
    public CCharacter
{
	friend class CScene;

protected:
	CAICharacter();
	CAICharacter(const CAICharacter& obj);
	virtual ~CAICharacter();

protected:
	class CAIController* m_pAIController;

	AIState		m_State;

	float		m_TraceDistance;
	float		m_AttackDistance;

	bool		m_CheckFirstPassBy; // 처음으로 플레이어와 스친건지 판단
	float		m_AD; // 공격력

	bool		m_bIsAttack;

public:
	bool IsAttack()
	{
		return m_bIsAttack;
	}

public:
	bool CheckFirstPassBy()	const
	{
		return m_CheckFirstPassBy;
	}

	float GetAD()	const
	{
		return m_AD;
	}

public:
	void SetAIState(AIState State)
	{
		m_State = State;
	}

	void SetCheckFirstPassBy(bool Value = true)
	{
		m_CheckFirstPassBy = Value;
	}

protected:
	void SetTraceDistance(float Dist)
	{
		m_TraceDistance = Dist;
	}

	void SetAttackDistance(float Dist)
	{
		m_AttackDistance = Dist;
	}

public:
	virtual void Die();
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);

public:
	void SetAIIntData(const std::string& Name, int Data);
	void SetAIFloatData(const std::string& Name, float Data);
	void SetAIBoolData(const std::string& Name, bool Data);
	void SetAIStringData(const std::string& Name, std::string Data);
	void SetAIGameObjectData(const std::string& Name, class CGameObject* Data);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CAICharacter* Clone() const = 0;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);

public:
	template <typename T>
	T* CreateAIController()
	{
		T* pController = new T;

		pController->SetScene(m_pScene);
		pController->SetOwner(this);

		m_pAIController = (CAIController*)pController;

		return pController;
	}

	template <typename T>
	T* CreateAI(const std::string& Name)
	{
		if (!m_pAIController)
			return nullptr;

		return m_pAIController->CreateAI<T>(Name);
	}

	template <typename T>
	T* CreateAISelect(const std::string& Name)
	{
		if (!m_pAIController)
			return nullptr;

		return m_pAIController->CreateAISelect<T>(Name);
	}
};