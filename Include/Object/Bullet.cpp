#include "Bullet.h"
#include "BulletEffect.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneMode.h"
#include "../Collision/ColliderRect.h"
#include "../Object/PlayerEffect.h"
#include "../GameManager.h"
#include "../Timer.h"
#include "../Scene/Camera.h"
#include "Player.h"


CBullet::CBullet() :
	m_bIsCollectionable(false),
	m_bCanRotate(false),
	m_bCheckHit(false),
	m_Distance(0.f),
	m_pPlayer(nullptr),
	m_AD(0.f),
	m_Angle(0.f),
	m_BulletMaskTex(nullptr),
	m_RotateTime(0.5f),
	m_bIsPlayCollectEffectSound(false),
	m_bBulletDestroy(false)
{
}

CBullet::CBullet(const CBullet& obj)	:
	CMoveableObject(obj)
{
	m_AD = obj.m_AD;
}

CBullet::~CBullet()
{
}


void CBullet::SetView(BulletView View)
{
	if (View == BulletView::Right)
	{
		m_View = BulletView::Right;
		m_vMoveDir = Vector2(1.f, 0.f);
		SetTexture("PlayerBullet", TEXT("SakuyaIzayoi/bullet_sprite.bmp"), Vector2(0.f, 0.f), Vector2(64.f, 64.f));
		m_BulletMaskTex = m_pScene->GetResourceManager()->FindTexture("Bullet_Mask");
	}
	else if (View == BulletView::Left)
	{
		m_View = BulletView::Left;
		m_vMoveDir = Vector2(-1.f, 0.f);
		SetTexture("PlayerBullet_Left", TEXT("SakuyaIzayoi/bullet_sprite_Left.bmp"), Vector2(0.f, 0.f), Vector2(64.f, 64.f));
		m_BulletMaskTex = m_pScene->GetResourceManager()->FindTexture("Bullet_Mask_Left");
	}

	else if (View == BulletView::RightUp)
	{
		m_View = BulletView::Right;
		m_vMoveDir = Vector2(0.f, -1.f);
		m_Angle = 270.f;
		SetTexture("PlayerBullet", TEXT("SakuyaIzayoi/bullet_sprite.bmp"), Vector2(0.f, 0.f), Vector2(64.f, 64.f));
		m_BulletMaskTex = m_pScene->GetResourceManager()->FindTexture("Bullet_Mask");
	}

	else if (View == BulletView::LeftUp)
	{
		m_View = BulletView::Left;
		m_vMoveDir = Vector2(0.f, -1.f);
		m_Angle = 90.f;
		SetTexture("PlayerBullet_Left", TEXT("SakuyaIzayoi/bullet_sprite_Left.bmp"), Vector2(0.f, 0.f), Vector2(64.f, 64.f));
		m_BulletMaskTex = m_pScene->GetResourceManager()->FindTexture("Bullet_Mask_Left");
	}

	else if (View == BulletView::RightDown)
	{
		m_View = BulletView::Right;
		m_vMoveDir = Vector2(-cosf(40.f), sinf(40.f));
		m_Angle = 45.f;
		SetTexture("PlayerBullet", TEXT("SakuyaIzayoi/bullet_sprite.bmp"), Vector2(0.f, 0.f), Vector2(64.f, 64.f));
		m_BulletMaskTex = m_pScene->GetResourceManager()->FindTexture("Bullet_Mask");
	}

	else if (View == BulletView::LeftDown)
	{
		m_View = BulletView::Left;
		m_vMoveDir = Vector2(cosf(40.f), sinf(40.f));
		m_Angle = -45.f;
		SetTexture("PlayerBullet_Left", TEXT("SakuyaIzayoi/bullet_sprite_Left.bmp"), Vector2(0.f, 0.f), Vector2(64.f, 64.f));
		m_BulletMaskTex = m_pScene->GetResourceManager()->FindTexture("Bullet_Mask_Left");
	}
}

bool CBullet::Init()
{
	m_pScene->GetResourceManager()->LoadTexture("Bullet_Mask", TEXT("SakuyaIzayoi/Bullet_Mask.bmp"));
	m_pScene->GetResourceManager()->LoadTexture("Bullet_Mask_Left", TEXT("SakuyaIzayoi/Bullet_Mask_Left.bmp"));
	
	SetUpBlock(true);
	SetPhysicsSimulate(true);
	m_GravityAccel = 0.f;
	m_IsFly = true;

	SetTimeMode(TimeMode::Move);
	
	SetSize(Vector2(32.f, 10.f));
	SetView();
	SetPivot(0.5f, 0.5f);

	RectComponent = AddCollider<CColliderRect>("PlayerBullet");
	RectComponent->SetExtent(m_vSize.x / 2, m_vSize.y);
	RectComponent->SetCollisionProfile("PlayerAttack");
	RectComponent->SetBeginCollisionFunction(this, &CBullet::BeginCollision);
	RectComponent->SetOffset(0.f, 0.f);
	m_Raycast = false;

	m_pPlayer = m_pScene->GetSceneMode()->GetPlayer();
	m_AD = 5.f;
	m_bCheckHit = false;
	m_CanCollectDist = 200.f;

	m_bIsRender = false;

	return true;
}

void CBullet::Start()
{
	CMoveableObject::Start();
}

void CBullet::Update(float DeltaTime)
{
	if (m_bBulletDestroy)
		CRef::Destroy();

	if (!m_bCheckHit)
	{
		Move(Vector2(m_vMoveDir.x * m_MoveSpeed * DeltaTime, m_vMoveDir.y * m_MoveSpeed * DeltaTime));
		m_Distance -= m_MoveSpeed * DeltaTime;
	}

	if (m_Distance <= 0.f)
		Release();
	
	if (!m_bCheckHit && m_bTileBlock)
		TileHit();

	// 회수 가능한 상태가 되었을 때
	if (m_bCheckHit && !m_bCanRotate)
	{
		// 플레이어와 일정 거리 이하가 됐을 때 회전하면서 회수
		if (m_vPos.Distance(m_pPlayer->GetCenterPos()) < m_CanCollectDist)
		{
			m_bCanRotate = true;
		}
	}

	if (m_bCanRotate)
	{
		float TargetAngle = 0.f;

		if (m_View == BulletView::Right)
		{
			 TargetAngle = (m_vPos.GetAngle(m_pPlayer->GetCenterPos()) + 180);

			if (TargetAngle > 180)
				TargetAngle -= 360.f;
			
				m_Angle = BasicMath::FInterpTo(m_Angle, TargetAngle, DeltaTime, 15.f);
		}
		else
		{
			TargetAngle = (m_vPos.GetAngle(m_pPlayer->GetCenterPos()));

			if (TargetAngle > 180)
				TargetAngle -= 360.f;

			m_Angle = BasicMath::FInterpTo(m_Angle, TargetAngle, DeltaTime, 15.f);
		}

		m_RotateTime -= DeltaTime;
		if (m_RotateTime <= 0.f)
			m_bIsCollectionable = true;
	}

	if (m_bIsCollectionable)
	{
		if (!m_bIsPlayCollectEffectSound)
		{
			m_bIsPlayCollectEffectSound = true;
			m_pScene->GetResourceManager()->SoundPlay("knife_unplug");
		}

		if (m_vPos.Distance(m_pPlayer->GetCenterPos()) > 60.f)
			m_vPos = BasicMath::VInterpTo(m_vPos, m_pPlayer->GetCenterPos(), DeltaTime, 9.f);
		else
		{
			m_pPlayer->AddTimeStopGauge(1.f);
			CRef::Destroy();
		}
	}
	
	CMoveableObject::Update(DeltaTime);
}

void CBullet::Update(float DeltaTime, TimeMode eMode)
{
	if (m_Distance > 0.f && !m_bCheckHit)
	{
		Move(Vector2(m_vMoveDir.x * m_MoveSpeed * DeltaTime, m_vMoveDir.y * m_MoveSpeed * DeltaTime));

		m_Distance -= m_MoveSpeed * DeltaTime;
	}


	CMoveableObject::Update(DeltaTime);
}

void CBullet::PrevRender()
{
	CGameObject::PrevRender();

	if (m_Culling && !m_bBulletDestroy)
		m_bBulletDestroy = true;
}

void CBullet::Render(HDC hDC, float DeltaTime)
{
	m_vPrevPos = m_vPos;
	m_vMove = Vector2(0.f, 0.f);

	if (m_Texture)
	{
		m_vRenderPos -= (m_vTexturePixelSize - m_vSize) * m_vPivot;


		Vector2 Center = m_vRenderPos + (m_vTexturePixelSize / 2);

		// 각 포인트마다 센터에서부터 거리 구하기
		float Distance[3];
		Distance[0] = Center.Distance(m_vRenderPos);
		Distance[1] = Center.Distance(Vector2(m_vRenderPos.x + m_vTexturePixelSize.x, m_vRenderPos.y));
		Distance[2] = Center.Distance(Vector2(m_vRenderPos.x, m_vRenderPos.y + m_vTexturePixelSize.y));

		// 회전 변환
		POINT PointArr[3];
			
		PointArr[0].x = (LONG)(cosf((225 + m_Angle) * PI / 180.f) * Distance[0]) + Center.x;
		PointArr[0].y = (LONG)(sinf((225 + m_Angle) * PI / 180.f) * Distance[0]) + Center.y;

		PointArr[1].x = (LONG)(cosf((315 + m_Angle) * PI / 180.f) * Distance[1]) + Center.x;
		PointArr[1].y = (LONG)(sinf((315 + m_Angle) * PI / 180.f) * Distance[1]) + Center.y;

		PointArr[2].x = (LONG)(cosf((135 + m_Angle) * PI / 180.f) * Distance[2]) + Center.x;
		PointArr[2].y = (LONG)(sinf((135 + m_Angle) * PI / 180.f) * Distance[2]) + Center.y;

		PlgBlt(hDC, PointArr, m_Texture->GetTextureInfo()->hDC, 0, 0,
			(int)m_vTexturePixelSize.x, (int)m_vTexturePixelSize.y, m_BulletMaskTex->GetTextureInfo()->hBmp, 0, 0);

	}

	else
	{
		Rectangle(hDC, (int)m_vRenderPos.x, (int)m_vRenderPos.y, (int)(m_vRenderPos.x + m_vSize.x),
			(int)(m_vRenderPos.y + m_vSize.y));
	}

	if (RectComponent->IsEnable())
		RectComponent->Render(hDC, DeltaTime);

//#ifdef _DEBUG
//	char Angle[MAX_PATH] = "Angle : ";
//	sprintf_s(Angle, "Angle : %d", (int)m_Angle);
//	TextOutA(hDC, 300, 420, Angle, (int)strlen(Angle));
//#endif
}

CBullet* CBullet::Clone() const
{
	return new CBullet(*this);
}

void CBullet::SetTimeMode(TimeMode eMode)
{
	switch (eMode)
	{
	case TimeMode::Move:
		m_Distance = 2500.f;
		m_MoveSpeed = 1300.f;
		break;
	case TimeMode::Stop:
		m_Distance = 50.f;
		m_MoveSpeed = 500.f;
		break;
	}
}

void CBullet::TileHit()
{
	m_bCheckHit = true;

	CSharedPtr<CPlayerEffect> BulletStickEffect = m_pScene->CreateGameObject<CPlayerEffect>("PlayerBullet_Stick_Effect");
	BulletStickEffect->SetPos(m_vPos);
	BulletStickEffect->SetPivot(0.5f, 0.5f);
	BulletStickEffect->SetUseLocalSpace(false);
	BulletStickEffect->AddAnimation("PlayerBullet_Stick_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", TEXT("Effect/Effect_Atlass00.bmp"), .2f);
	BulletStickEffect->AddAnimationFrameData("PlayerBullet_Stick_Effect_Anim", 0, 9, 9, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
	BulletStickEffect->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("knife_stop");
	m_MoveSpeed = 0.f;

	RectComponent->Enable(false);
	m_PhysicsSimulate = false;
}

void CBullet::CollectionBulletRotateEnd()
{
	m_bIsCollectionable = true;
}


void CBullet::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	if (pDest->GetCollisionProfile()->Name == "Monster")
	{
		CSharedPtr<CPlayerEffect> MonsterHitEffect = m_pScene->CreateGameObject<CPlayerEffect>("PlayerBullet_MonsterHit_Effect");
		MonsterHitEffect->SetPos(m_vPos);
		MonsterHitEffect->SetPivot(0.5f, 0.5f);
		MonsterHitEffect->SetUseLocalSpace(false);
		MonsterHitEffect->AddAnimation("PlayerBullet_MonsterHit_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", TEXT("Effect/Effect_Atlass00.bmp"), .2f);
		MonsterHitEffect->AddAnimationFrameData("PlayerBullet_MonsterHit_Effect_Anim", 6, 10, 7, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
		MonsterHitEffect->SetAutoDestroy();

		m_pScene->GetMainCamera()->CameraShake(5.f, 0.01f, 5.f, 0.01f, 0.2f);

		CRef::Destroy();
	}
	else
		TileHit();
}
