#pragma once
#include "UIControl.h"
class CUIProgressBar :
    public CUIControl
{
    friend class CUIWidget;

protected:
    CUIProgressBar();
    virtual ~CUIProgressBar();

protected:
    float   m_Min;
    float   m_Max;
    float   m_Value;

public:
    float GetMax()  const
    {
        return m_Max;
    }

    float GetMin()  const
    {
        return m_Min;
    }

    float GetValue()  const
    {
        return m_Value;
    }

public:
    void SetMin(float Min)
    {
        m_Min = Min;
    }

    void SetMax(float Max)
    {
        m_Max = Max;
    }

    void SetValue(float Value)
    {
        m_Value = Value;

        if (m_Value < m_Min)
            m_Value = m_Min;

        else if (m_Value > m_Max)
            m_Value = m_Max;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual bool CollisionMouse(const Vector2& MousePos);
    virtual void Collision();
    virtual void PrevRender();
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime, const Vector2& vPos);

public:
    virtual void CollisionEnd();
};

