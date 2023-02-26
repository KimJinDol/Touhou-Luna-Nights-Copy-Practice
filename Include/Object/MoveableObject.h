#pragma once
#include "GameObject.h"
#include "../Scene/SceneCollision.h"

class CMoveableObject :
    public CGameObject
{
	friend class CScene;

protected:
	CMoveableObject();
	CMoveableObject(const CMoveableObject& obj);
	virtual ~CMoveableObject();
protected:

	CGameObject* m_pTarget;
	bool		m_bEnableTarget;
	Vector2		m_TargetPrevPos;
	Vector2		m_vMoveDir;
	Vector2		m_vMove;
	Vector2		m_vPrevPos;
	float		m_MoveSpeed;
	TileInteraction	m_TileInteraction;
	std::function<void()>	m_TileInteractionFunc;
	bool		m_IsGround;
	bool		m_PhysicsSimulate;
	bool		m_Raycast;
	float		m_FallTime;
	bool		m_IsFalling;
	float		m_GravityAccel;
	float		m_Force;
	bool		m_bUseAccel;
	float		m_FallStartY;
	bool		m_Jump;
	bool		m_IsFly;
	float		m_JumpVelocity;
	float		m_Velocity;
	float		m_VelocityMax;
	bool		m_UpBlock;
	bool		m_bTileBlock; // 타일에 의해 BLocking 됐을 때 true
	bool		m_bIsBlock;
	std::function<void()>	m_pJumpEndFunc;

	HitResult m_hitResult;
	bool		m_bHit;


public:
	bool IsBlockByTile()	const
	{
		return m_bTileBlock;
	}

	HitResult GetHitResult() const
	{
		return m_hitResult;
	}

	bool IsHitResult()	const
	{
		return m_hitResult.hitCollider != nullptr ? true : false;
	}

	bool IsGround()	const
	{
		return m_IsGround;
	}

	bool IsJump()	const
	{
		return m_Jump;
	}

	bool IsPhysicsSimulate()	const
	{
		return m_PhysicsSimulate;
	}

	void EnableTarget(bool bTarget)
	{
		m_bEnableTarget = bTarget;
	}

	Vector2 GetMoveDir()	const
	{
		return m_vMoveDir;
	}

	float GetMoveSpeed()	const
	{
		return m_MoveSpeed;
	}

	Vector2 GetMove()	const
	{
		return m_vMove;
	}

public:
	void SetUpBlock(bool Block)
	{
		m_UpBlock = Block;
	}

	void SetJumpVelocity(float Velocity)
	{
		m_JumpVelocity = Velocity;
	}

	void SetPhysicsSimulate(bool Physics)
	{
		m_PhysicsSimulate = Physics;
	}

	void SetTarget(CGameObject* pTarget)
	{
		m_pTarget = pTarget;

		m_TargetPrevPos = pTarget->GetPos();
	}

	void SetMoveSpeed(float MoveSpeed)
	{
		m_MoveSpeed = MoveSpeed;
	}

	void SetMoveDir(float x, float y)
	{
		m_vMoveDir = Vector2(x, y);
	}

	void SetMoveDir(const Vector2& vDir)
	{
		m_vMoveDir = vDir;
	}

	void Move(const Vector2& vMove)
	{
		m_vMove += vMove;
		m_vPos += vMove;
	}

	void Jump();
	void JumpEnd();

	void GroundWalkRightBottom(const struct HitResult& hitResult);
	void GroundWalkLeftBottom(const struct HitResult& hitResult);


private:
	void TileInteractionBlock();
	void TileInteractionDestroy();

public:
	virtual bool Init() = 0;
	virtual void Start();
	virtual void SetInput();
	virtual void Update(float DeltaTime);
	virtual void Collision();
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CMoveableObject* Clone() const = 0;

public:

	template <typename T>
	void SetTileInteractionFunction(T* pObj, void(T::* pFunc)())
	{
		m_TileInteractionFunc = std::bind(pFunc, pObj);
	}
};

