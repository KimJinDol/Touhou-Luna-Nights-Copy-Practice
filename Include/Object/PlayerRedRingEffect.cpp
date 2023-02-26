#include "PlayerRedRingEffect.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CPlayerRedRingEffect::CPlayerRedRingEffect()
{
}

CPlayerRedRingEffect::CPlayerRedRingEffect(const CPlayerRedRingEffect& obj) :
    CPlayerEffect(obj)
{
}

CPlayerRedRingEffect::~CPlayerRedRingEffect()
{
}

bool CPlayerRedRingEffect::Init()
{
    CPlayerEffect::Init();

    m_vFixedSize = m_vSize;

    return true;
}

void CPlayerRedRingEffect::Start()
{
}

void CPlayerRedRingEffect::Update(float DeltaTime)
{
	m_vSize -= m_vFixedSize * DeltaTime * 2.f;

	if (m_vSize.x <= 0.f)
        CRef::Destroy();
}

void CPlayerRedRingEffect::Update(float DeltaTime, TimeMode eMode)
{
    m_vSize -= m_vFixedSize * DeltaTime * 2.f;

    if (m_vSize.x <= 0.f)
        CRef::Destroy();
}

void CPlayerRedRingEffect::Render(HDC hDC, float DeltaTime)
{
    SelectObject(hDC, CGameManager::GetInst()->GetRedBrush());

    m_vRenderPos = m_vPos - m_pScene->GetMainCamera()->GetPos();
    Ellipse(hDC, (int)(m_vRenderPos.x - m_vSize.x * m_vPivot.x), (int)(m_vRenderPos.y - m_vSize.y * m_vPivot.y),
        (int)(m_vRenderPos.x + m_vSize.x * (1.f - m_vPivot.x)), (int)(m_vRenderPos.y + m_vSize.y * (1.f - m_vPivot.y)));
}

CPlayerRedRingEffect* CPlayerRedRingEffect::Clone() const
{
    return new CPlayerRedRingEffect(*this);
}

void CPlayerRedRingEffect::SetTimeMode(TimeMode eMode)
{
}
