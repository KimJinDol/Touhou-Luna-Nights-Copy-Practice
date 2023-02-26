
#include "BulletEffect.h"

CBulletEffect::CBulletEffect()
{
}

CBulletEffect::CBulletEffect(const CBulletEffect& obj) :
	CMoveableObject(obj)
{
}

CBulletEffect::~CBulletEffect()
{
}

bool CBulletEffect::Init()
{
	SetPivot(0.5f, 0.5f);

	return true;
}

void CBulletEffect::Start()
{
	CGameObject::Start();
}

void CBulletEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBulletEffect::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}

CBulletEffect* CBulletEffect::Clone() const
{
	return new CBulletEffect(*this);
}

void CBulletEffect::EffectEnd()
{
	CRef::Destroy();
}
