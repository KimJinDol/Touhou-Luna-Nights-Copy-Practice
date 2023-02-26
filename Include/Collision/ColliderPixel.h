#pragma once
#include "Collider.h"

typedef struct _tagPixel
{
	unsigned char   r;
	unsigned char   g;
	unsigned char   b;
} PIXEL, * PPIXEL;

class CColliderPixel :
    public CCollider
{
	friend class CGameObject;
	friend class CScene;

private:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& collider);
	virtual ~CColliderPixel();

private:
	std::vector<PIXEL>   m_vecPixel;
	int			m_Width;
	int			m_Height;
	Vector2     m_vHitPoint;

public:
	const std::vector<PIXEL>& GetPixel() const
	{
		return m_vecPixel;
	}

	int GetWidth()  const
	{
		return m_Width;
	}

	int GetHeight() const
	{
		return m_Height;
	}


	Vector2 GetHitPoint()   const
	{
		return m_vHitPoint;
	}

public:
	bool SetPixelInfo(const char* pFileName, const std::string& strPathKey = TEXTURE_PATH);

	void SetHitPoint(const Vector2& vPos)
	{
		m_vHitPoint = vPos;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual bool Collision(CCollider* pDest);
	virtual CColliderPixel* Clone()	const;
	virtual bool CollisionMouse();
};

