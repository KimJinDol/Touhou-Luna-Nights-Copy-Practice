#pragma once
#include "UIControl.h"


struct TextNumber
{
    class CUIImage*  Image;
    int              Num;

    TextNumber() :
        Num(0),
        Image(nullptr)
    {
    }
    ~TextNumber()
    {
    }
};

class CUITextNumber :
    public CUIControl
{
    friend class CUIWidget;

protected:
    CUITextNumber();
    virtual ~CUITextNumber();

private:
    std::vector<TextNumber*>    m_vecTextNumber;
    std::vector<Vector2>        m_vecImageStartPos;
    
    int     m_Number;       // ���ε� �� ��
    int     m_Chipers;      // �ڸ���
    int     m_MaxChipers;   // �ִ� �ڸ���..
    int     m_vTextXSize;    // �ؽ�Ʈ ���� X ������



public:
    void SetTextImageSize(const Vector2& vSize);
    void SetTextXSize(int TextXSize)
    {
        m_vTextXSize = TextXSize;
    }

    void SetMaxChipers(int MaxChipers);
    void SetNumber(int Number);

	void SetTextNumberTexture(int x, int y, const Vector2& Size);


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual bool CollisionMouse(const Vector2& MousePos);
    virtual void Collision();
    virtual void PrevRender();
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime, const Vector2& vPos);
};

