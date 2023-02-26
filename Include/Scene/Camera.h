#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"

class CCamera
{
    friend class CScene;

private:
    CCamera();
    ~CCamera();

private:
    Vector2     m_vPos;
    Vector2     m_vResolution;
    Vector2     m_vWorldSize;
    CSharedPtr<CGameObject> m_pTarget;
    Vector2     m_vTargetOffset;    // 타겟으로부터 얼마나 떨어져서 쫓아갈지.
    Vector2     m_vTargetPivot;

    /*Camera Shake*/
    int       m_AmplitudeX; // X쉐이크 강도
    float       m_FrequencyX; // x쉐이크 빠르기

    int       m_AmplitudeY; // Y쉐이크 강도
    float       m_FrequencyY; // x쉐이크 빠르기

    float       m_ShakeTime; // 쉐이크 타임

    bool        m_bIsShake;

    float       m_curFreqTimeX;
    float       m_curFreqTimeY;
    Vector2     m_ShakePos;

    Vector2     m_OriginPos;

public:
    Vector2 GetPos()    const
    {
        return m_vPos;
    }

    Vector2 GetResolution() const
    {
        return m_vResolution;
    }

    Vector2 GetWorldSize()  const
    {
        return m_vWorldSize;
    }

    Vector2 GetTargetOffset()   const
    {
        return m_vTargetOffset;
    }

    Vector2 GetTargetPivot()    const
    {
        return m_vTargetPivot;
    }

public:
    void SetEnableShake(bool Value)
    {
        m_bIsShake = Value;
    }

    void UnableCameraShake()
    {
        m_bIsShake = false;

        //if (m_vTargetPivot.x != 0.f)
        //{
        //    if (m_vPos.x + m_vTargetPivot.x * m_vResolution.x < m_pTarget->GetPos().x)
        //        m_vPos.x = m_pTarget->GetPos().x - m_vTargetPivot.x * m_vResolution.x + m_vTargetOffset.x;
        //    
        //    else if (m_vPos.x + (m_vTargetPivot.x) * m_vResolution.x > m_pTarget->GetPos().x)
        //        m_vPos.x = m_pTarget->GetPos().x - m_vTargetPivot.x * m_vResolution.x + m_vTargetOffset.x;
        //}

        //if (m_vTargetPivot.y != 0.f)
        //{
        //    if (m_vPos.y + m_vTargetPivot.y * m_vResolution.y < m_pTarget->GetPos().y)
        //        m_vPos.y = m_pTarget->GetPos().y - m_vTargetPivot.y * m_vResolution.y + m_vTargetOffset.y;
        //   
        //    else if (m_vPos.y + (1 - m_vTargetPivot.y) * m_vResolution.y > m_pTarget->GetPos().y)
        //        m_vPos.y = m_pTarget->GetPos().y - (1 - m_vTargetPivot.y) * m_vResolution.y + m_vTargetOffset.y;
        //    
        //}
    }

    void SetPos(const Vector2& vPos)
    {
        m_vPos = vPos;
    }

    void SetPos(float x, float y)
    {
        m_vPos = Vector2(x, y);
    }

    void SetResolution(const Vector2& vResolution)
    {
        m_vResolution = vResolution;
    }

    void SetResolution(float x, float y)
    {
        m_vResolution = Vector2(x, y);
    }

    void SetWorldSize(const Vector2& vWorldSize)
    {
        m_vWorldSize = vWorldSize;
    }

    void SetWorldSize(float x, float y)
    {
        m_vWorldSize = Vector2(x, y);
    }

    void SetTargetOffset(const Vector2& vTargetOffset)
    {
        m_vTargetOffset = vTargetOffset;
    }

    void SetTargetOffset(float x, float y)
    {
        m_vTargetOffset = Vector2(x, y);
    }

    void SetTargetPivot(const Vector2& vTargetPivot)
    {
        m_vTargetPivot = vTargetPivot;
    }

    void SetTargetPivot(float x, float y)
    {
        m_vTargetPivot = Vector2(x, y);
    }

    void SetTarget(class CGameObject* pTarget)
    {
        m_pTarget = pTarget;
    }

    void Move(const Vector2& vMove)
    {
        m_vPos += vMove;
    }

public:
    bool Init();
    void Update(float DeltaTime);

public:
    void CameraShake(float AmplitudeX, float FrequencyX, float AmplitudeY, float FrequencyY, float Time);

private:
    void CameraShake(float DeltaTime);

};

