#include "Character.h"

CCharacter::CCharacter()
{
    m_View = CharacterView::Right;
}

CCharacter::CCharacter(const CCharacter& obj)   :
    CMoveableObject(obj)
{
	m_HP = obj.m_HP;
	m_HPMax = obj.m_HPMax;
	m_MP = obj.m_MP;
	m_MPMax = obj.m_MPMax;

	m_View = obj.m_View;
}

CCharacter::~CCharacter()
{
}

bool CCharacter::Init()
{
    //if (!CMoveableObject::Init())
    //    return false;

	SetPhysicsSimulate(true);
	SetJumpVelocity(50.f);
	SetUpBlock(true);

    return true;
}

void CCharacter::Start()
{
    CMoveableObject::Start();
}

void CCharacter::SetInput()
{
    CMoveableObject::SetInput();
}

void CCharacter::Update(float DeltaTime)
{
	if (m_vMoveDir.x > 0 || m_vMove.x > 0)
		m_View = CharacterView::Right;
	else if (m_vMoveDir.x < 0 || m_vMove.x < 0)
		m_View = CharacterView::Left;

    CMoveableObject::Update(DeltaTime);
}

void CCharacter::Render(HDC hDC, float DeltaTime)
{
    CMoveableObject::Render(hDC, DeltaTime);
}

CCharacter* CCharacter::Clone() const
{
    return nullptr;
}

float CCharacter::TakeDamage(float Damage, CGameObject* DamageObject)
{
	SetHP(m_HP - Damage);

	return m_HP;
}