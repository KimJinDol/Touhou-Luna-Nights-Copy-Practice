#pragma once

#include "../GameInfo.h"
#include "../Object/Player.h"

class CSceneMode
{
	friend class CScene;

protected:
	CSceneMode();
	virtual ~CSceneMode() = 0;

protected:
	class CScene* m_pScene;
	CSharedPtr<class CPlayer>	m_Player;
	bool		m_Start;
	static TimeMode m_CurTimeMode;
	float	m_WorldTimeSpeed; // 시간이 흐르는 속도 (플레이어를 제외한 모든 것이 이 속도의 영향을 받는다.)

	bool	m_bShowTileSide;

	Vector2 m_PlayerSpawnPos;

	bool	m_bIsAllStop;

public:
	bool IsShowTileSide()	const
	{
		return m_bShowTileSide;
	}

	float GetWorldTimeSpeed() const
	{
		return m_WorldTimeSpeed;
	}

	TimeMode GetTimeMode() const
	{
		return m_CurTimeMode;
	}

	class CPlayer* GetPlayer()	const
	{
		return (class CPlayer*)(m_Player.Get());
	}

public:
	void AllStop()
	{
		m_Player->SetEnable(false, false, false);
		m_bIsAllStop = true;
	}

	void ReleaseAllStop()
	{
		m_Player->SetEnable();
		m_bIsAllStop = false;
	}

	void SetPlayerSpawnPos(const Vector2& Pos)
	{
		m_PlayerSpawnPos = Pos;
		if (m_Player)
			m_Player->SetPos(Pos);
	}

	void SetIsShowTileSide(float DeltaTime)
	{
		m_bShowTileSide = m_bShowTileSide ? false : true;
	}

	void SetTimeMode(TimeMode Mode);

	void SetPlayer(CPlayer* pPlayer)
	{
		m_Player = pPlayer;
	}

	void SetPlayer(CSharedPtr<CPlayer> Player)
	{
		m_Player = Player;
	}

public:
	virtual void Start();
	virtual bool Init() = 0;
	virtual void Update(float DeltaTime);

#ifdef _DEBUG
private:
	void ChangeEditorScene(float DeltaTime);
	void ChangeStage00Scene(float DeltaTime);
	void ChangeStage01Scene(float DeltaTime);
	void ChangeStage02Scene(float DeltaTime);
	void ChangeStage03Scene(float DeltaTime);
	void ChangeStage04Scene(float DeltaTime);
#endif
};

