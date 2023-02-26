
#include "Smoke.h"
#include "../Resource/Animation.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"

CSmoke::CSmoke()
{
}

CSmoke::CSmoke(const CSmoke& obj)   :
    CMoveableObject(obj)
{
	SetAnimationEndFunction<CSmoke>("Smoke", this,
		&CSmoke::EffectEnd);
}

CSmoke::~CSmoke()
{
}

bool CSmoke::Init()
{
	SetPivot(0.5f, 0.5f);

	AddAnimation("Smoke", AnimationType::Sprite, true,
		"Smoke", TEXT("smoke.bmp"), 0.5f);

	for (int i = 0; i < 6; ++i)
	{
		AddAnimationFrameData("Smoke",
			Vector2(i * 92.5f, 0.f), Vector2(92.5f, 92.5f));
	}

	m_pScene->GetResourceManager()->SetColorKey("Smoke",
		255, 0, 255);

	SetAnimationEndFunction<CSmoke>("Smoke", this,
		&CSmoke::EffectEnd);

    return true;
}

void CSmoke::Start()
{
	CMoveableObject::Start();
}

void CSmoke::Update(float DeltaTime)
{
	CMoveableObject::Update(DeltaTime);
}

void CSmoke::Render(HDC hDC, float DeltaTime)
{
	CMoveableObject::Render(hDC, DeltaTime);
}

CSmoke* CSmoke::Clone() const
{
    return new CSmoke(*this);
}

void CSmoke::EffectEnd()
{
	CRef::Destroy();
}
