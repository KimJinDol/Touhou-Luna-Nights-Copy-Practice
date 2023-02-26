#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
private:
	std::unordered_map<std::string, CollisionProfile*>	m_mapProfile;

public:
	bool Init();
	bool CreateProfile(const std::string& Name, CollisionChannel Channel, bool CollisionEnable,
		CollisionState DefaultState = CollisionState::Collision);
	bool SetCollisionState(const std::string& Name, CollisionChannel Channel,
		CollisionState ChangeState);
	CollisionProfile* FindProfile(const std::string& Name);

	DECLARE_SINGLE(CCollisionManager)
};

