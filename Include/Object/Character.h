#pragma once
#include "MoveableObject.h"

class CCharacter :
    public CMoveableObject
{
	friend class CScene;

protected:
	CCharacter();
	CCharacter(const CCharacter& obj);
	virtual ~CCharacter();

protected:
	float		m_HP;
	float		m_HPMax;
	float		m_MP;
	float		m_MPMax;

	CharacterView		m_View;

public:
	CharacterView GetView()	const
	{
		return m_View;
	}

public:
	virtual float GetHP()	const
	{
		return m_HP;
	}

	virtual float GetMP()	const
	{
		return m_MP;
	}

public:
	void SetView(CharacterView View)
	{
		m_View = View;
	} 

protected:
	virtual void SetHP(float HP)
	{
		m_HP = BasicMath::Clamp<float>(HP, 0, m_HPMax);

		if (m_HP <= 0)
			this->Die();
	}

	virtual void SetMP(float MP)
	{
		m_MP = BasicMath::Clamp<float>(MP, 0, m_MPMax);
	}

	virtual void Die() = 0;
	// 남은 HP 반환
	virtual float TakeDamage(float Damage, CGameObject* DamageObject);

public:
	virtual bool Init();
	virtual void Start();
	virtual void SetInput();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CCharacter* Clone()	const;

};

