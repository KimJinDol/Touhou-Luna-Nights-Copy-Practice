#pragma once
#include "UIControl.h"
class CUIImage :
    public CUIControl
{
	friend class CUIWidget;
	friend class CUITextNumber;

protected:
    CUIImage();
    virtual ~CUIImage();

private:
	char m_Text[MAX_PATH];
	Vector2 m_TextPos;
	bool	m_IsText;



public:
	void SetText(const char* Text, const Vector2& Pos);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual bool CollisionMouse(const Vector2& MousePos);
	virtual void Collision();
	virtual void PrevRender();
	virtual void Render(HDC hDC, float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime, const Vector2& vPos);
};

