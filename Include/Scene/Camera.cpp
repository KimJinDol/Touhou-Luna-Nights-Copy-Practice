
#include "Camera.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "../Timer.h"

CCamera::CCamera() :
    m_pTarget(nullptr),
    m_curFreqTimeX(0.f),
    m_curFreqTimeY(0.f),
    m_FrequencyX(0.1f),
    m_FrequencyY(0.1f),
    m_AmplitudeX(4.f),
    m_AmplitudeY(4.f),
    m_bIsShake(true)
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init()
{
    return true;
}

void CCamera::Update(float DeltaTime)
{
    if (m_pTarget)
    {
        if (m_pTarget->IsDestroy())
        {
            m_pTarget->Release();
            m_pTarget = nullptr;
        }

        else
        {
            /*m_vPos = m_pTarget->GetPos() - m_vTargetPivot * m_vResolution +
                  m_vTargetOffset;*/
            m_vPos -= m_ShakePos;

            CameraShake(DeltaTime);

            Vector2 LerpPos;
            LerpPos = m_ShakePos;
            
            if (m_vTargetPivot.x != 0.f)
            {
                if (m_vPos.x + m_vTargetPivot.x * m_vResolution.x < m_pTarget->GetPos().x)
                {
                    LerpPos.x = m_pTarget->GetPos().x - m_vTargetPivot.x * m_vResolution.x + m_vTargetOffset.x;
                    m_vPos.x = BasicMath::FInterpTo(m_vPos.x, LerpPos.x, CGameManager::GetInst()->GetTimer()->GetDeltaTime(), 5.f);
                }
                else if (m_vPos.x + (m_vTargetPivot.x) * m_vResolution.x > m_pTarget->GetPos().x)
                {
                    LerpPos.x = m_pTarget->GetPos().x - m_vTargetPivot.x * m_vResolution.x + m_vTargetOffset.x;
                    m_vPos.x = BasicMath::FInterpTo(m_vPos.x, LerpPos.x, CGameManager::GetInst()->GetTimer()->GetDeltaTime(), 5.f);
                }
            }

            if (m_vTargetPivot.y != 0.f)
            {
                if (m_vPos.y + m_vTargetPivot.y * m_vResolution.y < m_pTarget->GetPos().y)
                {
                    LerpPos.y = m_pTarget->GetPos().y - m_vTargetPivot.y * m_vResolution.y + m_vTargetOffset.y;
                    m_vPos.y = BasicMath::FInterpTo(m_vPos.y, LerpPos.y, CGameManager::GetInst()->GetTimer()->GetDeltaTime(), 5.f);
                }
                else if (m_vPos.y + (1 - m_vTargetPivot.y) * m_vResolution.y > m_pTarget->GetPos().y)
                {
                    LerpPos.y = m_pTarget->GetPos().y - (1 - m_vTargetPivot.y) * m_vResolution.y + m_vTargetOffset.y;
                    m_vPos.y = BasicMath::FInterpTo(m_vPos.y, LerpPos.y, CGameManager::GetInst()->GetTimer()->GetDeltaTime(), 5.f);
                }
            }

            m_vPos += m_ShakePos;
        }
    }

    // 카메라가 월드를 벗어날 수 없도록 제한한다.
    if (m_vPos.x < 0.f)
        m_vPos.x = 0.f;

    else if (m_vPos.x + m_vResolution.x > m_vWorldSize.x)
        m_vPos.x = m_vWorldSize.x - m_vResolution.x;

    if (m_vPos.y < 0.f)
        m_vPos.y = 0.f;

    else if (m_vPos.y + m_vResolution.y > m_vWorldSize.y)
        m_vPos.y = m_vWorldSize.y - m_vResolution.y;
}

void CCamera::CameraShake(float AmplitudeX, float FrequencyX, float AmplitudeY, float FrequencyY, float Time)
{
    m_AmplitudeX = AmplitudeX;
    m_AmplitudeY = AmplitudeY;

    m_FrequencyX = FrequencyX;
    m_FrequencyY = FrequencyY;

    m_ShakeTime = Time;

    m_bIsShake = true;
}

void CCamera::CameraShake(float DeltaTime)
{
    m_ShakeTime -= DeltaTime;

    if (m_bIsShake && m_ShakeTime > 0.f)
    {
        if (m_AmplitudeX)
        {
            m_curFreqTimeX -= DeltaTime;

            if (m_curFreqTimeX <= 0.f)
            {
                m_curFreqTimeX = m_FrequencyX;
                int Dir = rand() % 2 == 1 ? 1 : -1;
                m_ShakePos.x = rand() % m_AmplitudeX * Dir;
            }
        }

        if (m_AmplitudeY)
        {
            m_curFreqTimeY -= DeltaTime;

            if (m_curFreqTimeY <= 0.f)
            {
                m_curFreqTimeY = m_FrequencyY;
                int Dir = rand() % 2 == 1 ? 1 : -1;
                m_ShakePos.y = rand() % m_AmplitudeY * Dir;
            }
        }
    }
    else
        m_ShakePos = Vector2(0.f, 0.f);
}