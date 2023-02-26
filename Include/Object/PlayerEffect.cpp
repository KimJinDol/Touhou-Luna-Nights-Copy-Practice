#include "PlayerEffect.h"

CPlayerEffect::CPlayerEffect()
{
}

CPlayerEffect::CPlayerEffect(const CPlayerEffect& obj) :
    CEffect(obj)
{
}

CPlayerEffect::~CPlayerEffect()
{
}

bool CPlayerEffect::Init()
{
    CEffect::Init();

    return true;
}

void CPlayerEffect::Start()
{
    CEffect::Start();
}

void CPlayerEffect::Update(float DeltaTime)
{
    CEffect::Update(DeltaTime);
}

void CPlayerEffect::Update(float DeltaTime, TimeMode eMode)
{
    CEffect::Update(DeltaTime);
}

void CPlayerEffect::Render(HDC hDC, float DeltaTime)
{
    CEffect::Render(hDC, DeltaTime);
}

CPlayerEffect* CPlayerEffect::Clone() const
{
    return new CPlayerEffect(*this);
}

void CPlayerEffect::SetTimeMode(TimeMode eMode)
{
}
