#include "UITextNumber.h"
#include "UIImage.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture.h"
#include "../Scene/Camera.h"


CUITextNumber::CUITextNumber()  :
    m_MaxChipers(0),
    m_Chipers(0)
{
	m_vecImageStartPos.resize(10);
}

CUITextNumber::~CUITextNumber()
{
	auto iter = m_vecTextNumber.begin();
	auto iterEnd = m_vecTextNumber.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter)->Image);
		SAFE_DELETE(*iter);
	}
	m_vecTextNumber.clear();

	m_vecImageStartPos.clear();
}

void CUITextNumber::SetTextImageSize(const Vector2& vSize)
{
	auto iter = m_vecTextNumber.begin();
	auto iterEnd = m_vecTextNumber.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Image->SetSize(vSize);
	}
}

void CUITextNumber::SetMaxChipers(int MaxChipers)
{
	if (!m_Texture) return;

	for (; m_MaxChipers < MaxChipers;)
	{
		m_MaxChipers++;

		TextNumber* pTextNumber = new TextNumber;
		pTextNumber->Image = new CUIImage;
		std::string Name = std::to_string(m_MaxChipers);
		pTextNumber->Image->SetName(Name);
		if (!pTextNumber->Image->Init())
			return;

		pTextNumber->Image->m_pOwner = m_pOwner;
		pTextNumber->Image->m_pScene = m_pScene;
		pTextNumber->Image->SetTexture(m_Texture->GetName());
		pTextNumber->Image->SetSize(m_vSize);
		m_vecTextNumber.push_back(pTextNumber);
	}
}

void CUITextNumber::SetNumber(int Number)
{
	m_Number = Number;
	int tempChipers = 1;

	while (Number / 10 > 0)
	{
		Number /= 10;
		tempChipers++;
	}

	if (tempChipers > m_MaxChipers)
		return;

	m_Chipers = tempChipers;

	// 가장 작은 자릿수부터 포지션 값 계산
	Vector2 TextImagePos = m_vPos;
	TextImagePos.x = m_vPos.x + (m_Chipers - 1) * 0.5f * m_vTextXSize;

	int tempNum = m_Number;
	for (int i = 0; i < tempChipers; i++)
	{
		m_vecTextNumber[i]->Num = tempNum % 10;
		m_vecTextNumber[i]->Image->SetPos(TextImagePos);
		m_vecTextNumber[i]->Image->SetTextureImageStartPos(m_vecImageStartPos[m_vecTextNumber[i]->Num]);
		tempNum *= 0.1f;
		TextImagePos.x -= m_vTextXSize;
	}
}

void CUITextNumber::SetTextNumberTexture(int x, int y, const Vector2& Size)
{
	if (!m_Texture) return;

	Vector2 ImageStartPos = Vector2(Size.x * x, Size.y * y);

	for (int i = 0; i < 10; i++)
	{
		m_vecImageStartPos[i] = ImageStartPos;

		if (ImageStartPos.x + Size.x >= m_Texture->GetTextureInfo()->BmpInfo.bmWidth)
		{
			ImageStartPos.y += Size.y;
			ImageStartPos.x = 0;
		}
		else
			ImageStartPos.x += Size.x;
	}
}


bool CUITextNumber::Init()
{
	if (!CUIControl::Init())
		return false;

	return true;
}

void CUITextNumber::Update(float DeltaTime)
{
    CUIControl::Update(DeltaTime);
}

bool CUITextNumber::CollisionMouse(const Vector2& MousePos)
{
	return CUIControl::CollisionMouse(MousePos);
}

void CUITextNumber::Collision()
{
	CUIControl::Collision();
}

void CUITextNumber::PrevRender()
{
	for (int i = 0; i < m_Chipers; i++)
	{
		m_vecTextNumber[i]->Image->PrevRender();
	}
}

void CUITextNumber::Render(HDC hDC, float DeltaTime)
{	
	for (int i = 0; i < m_Chipers; i++)
	{
		m_vecTextNumber[i]->Image->Render(hDC, DeltaTime);
	}
}

void CUITextNumber::Render(HDC hDC, float DeltaTime, const Vector2& vPos)
{
	for (int i = 0; i < m_Chipers; i++)
	{
		m_vecTextNumber[i]->Image->Render(hDC, DeltaTime, vPos);
	}
}
