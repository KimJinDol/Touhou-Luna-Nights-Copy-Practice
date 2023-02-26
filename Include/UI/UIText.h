#pragma once
#include "UIControl.h"
class CUIText :
    public CUIControl
{
	friend class CUIWidget;
	friend class CUITextNumber;

protected:
	CUIText();
	virtual ~CUIText();

private:	
	std::string		m_Text;
	MessageBoxType	m_BoxType;

	Vector2			m_LeftStartPos;
	Vector2			m_RightStartPos;
	Vector2			m_LeftSize;
	Vector2			m_RightSize;
	Vector2			m_AddedCenterSize;

	CSharedPtr<CTexture>	m_Mask;
	CSharedPtr<CTexture>	m_LeftTex;
	CSharedPtr<CTexture>	m_RightTex;

public:
	void SetLeftTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName = TEXTURE_PATH);
	void SetRightTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName = TEXTURE_PATH);

	void SetBoxType(MessageBoxType eType);

	void SetText(const std::string& Text);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual bool CollisionMouse(const Vector2& MousePos);
	virtual void Collision();
	virtual void PrevRender();
	virtual void Render(HDC hDC, float DeltaTime);
};

