#pragma once
#include "MoveableObject.h"
#include "../Interface.h"

enum class BulletView
{
	Right,
	Left,
	RightUp,
	LeftUp,
	RightDown,
	LeftDown
};

class CBullet :
	public CMoveableObject, public ITimeStopInterface
{
	friend class CScene;

protected:
	CBullet();
	CBullet(const CBullet& obj);
	virtual ~CBullet();

protected:
	class CSharedPtr<CTexture> m_BulletMaskTex;

	float		m_Distance;
	CSharedPtr<class CPlayer> m_pPlayer;
	bool		m_bIsCollectionable;
	bool		m_bCanRotate;
	/*Vector2		m_CollectDir;	*/
	bool		m_bCheckHit;
	float		m_Angle;

	float		m_AD;

	float		m_CanCollectDist;
	bool		m_bIsPlayCollectEffectSound;

	float		m_RotateTime;

	BulletView	m_View;

	bool		m_bBulletDestroy;
	
public:
	float GetAD()	const
	{
		return m_AD;
	}

public:
	void SetDistance(float Distance)
	{
		m_Distance = Distance;
	}
	void SetView(BulletView View = BulletView::Right);
	
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Update(float DeltaTime, TimeMode eMode);
	virtual void PrevRender();
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CBullet* Clone()	const;

public:
	virtual void SetTimeMode(TimeMode eMode);

public:
	void TileHit();
	void CollectionBulletRotateEnd();
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

