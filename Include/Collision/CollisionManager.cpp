
#include "CollisionManager.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	auto	iter = m_mapProfile.begin();
	auto	iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CCollisionManager::Init()
{
	CreateProfile("Object", CollisionChannel::Object, true);

	for (int i = 0; i < (int)CollisionChannel::Max; ++i)
	{
		SetCollisionState("Object", (CollisionChannel)i, CollisionState::Collision);
	}

	CreateProfile("Player", CollisionChannel::Player, true, CollisionState::Ignore);
	SetCollisionState("Player", CollisionChannel::Monster, CollisionState::Collision);
	SetCollisionState("Player", CollisionChannel::Object, CollisionState::Collision);
	SetCollisionState("Player", CollisionChannel::MonsterAttack, CollisionState::Collision);
	SetCollisionState("Player", CollisionChannel::MonsterDetect, CollisionState::Collision);
	SetCollisionState("Player", CollisionChannel::PlayerTrigger, CollisionState::Collision);
	SetCollisionState("Player", CollisionChannel::Block, CollisionState::Block);

	CreateProfile("Monster", CollisionChannel::Monster, true, CollisionState::Ignore);
	SetCollisionState("Monster", CollisionChannel::Player, CollisionState::Collision);
	SetCollisionState("Monster", CollisionChannel::Object, CollisionState::Collision);
	SetCollisionState("Monster", CollisionChannel::PlayerAttack, CollisionState::Collision);
	SetCollisionState("Monster", CollisionChannel::MonsterTrigger, CollisionState::Collision);
	SetCollisionState("Monster", CollisionChannel::Monster, CollisionState::Collision);
	SetCollisionState("Monster", CollisionChannel::Block, CollisionState::Block);

	CreateProfile("PlayerAttack", CollisionChannel::PlayerAttack, true, CollisionState::Ignore);
	SetCollisionState("PlayerAttack", CollisionChannel::Monster, CollisionState::Collision);
	SetCollisionState("PlayerAttack", CollisionChannel::Object, CollisionState::Collision);
	SetCollisionState("PlayerAttack", CollisionChannel::Map, CollisionState::Collision);

	CreateProfile("MonsterAttack", CollisionChannel::MonsterAttack, true, CollisionState::Ignore);
	SetCollisionState("MonsterAttack", CollisionChannel::Player, CollisionState::Collision);
	SetCollisionState("MonsterAttack", CollisionChannel::Object, CollisionState::Collision);

	CreateProfile("MonsterDetect", CollisionChannel::MonsterDetect, true, CollisionState::Ignore);
	SetCollisionState("MonsterDetect", CollisionChannel::Player, CollisionState::Collision);

	CreateProfile("PlayerTrigger", CollisionChannel::PlayerTrigger, true, CollisionState::Ignore);
	SetCollisionState("PlayerTrigger", CollisionChannel::Player, CollisionState::Collision);

	CreateProfile("MonsterTrigger", CollisionChannel::MonsterTrigger, true, CollisionState::Ignore);
	SetCollisionState("PlayerTrigger", CollisionChannel::Monster, CollisionState::Collision);

	CreateProfile("Map", CollisionChannel::Map, true, CollisionState::Ignore);
	/*SetCollisionState("Map", CollisionChannel::Player, CollisionState::Block);*/
	SetCollisionState("Map", CollisionChannel::Raycast, CollisionState::Block);
	SetCollisionState("Map", CollisionChannel::PlayerAttack, CollisionState::Collision);
	
	CreateProfile("Raycast", CollisionChannel::Raycast, true, CollisionState::Ignore);
	SetCollisionState("Raycast", CollisionChannel::Map, CollisionState::Block);
	SetCollisionState("Raycast", CollisionChannel::Monster, CollisionState::Ignore);
	SetCollisionState("Raycast", CollisionChannel::Player, CollisionState::Ignore);

	CreateProfile("Block", CollisionChannel::Block, true, CollisionState::Ignore);
	SetCollisionState("Block", CollisionChannel::Map, CollisionState::Ignore);
	SetCollisionState("Block", CollisionChannel::Monster, CollisionState::Block);
	SetCollisionState("Block", CollisionChannel::Player, CollisionState::Block);
	
	/*
	Object,
	Player,
	Monster,
	PlayerAttack,
	MonsterAttack,
	MonsterDetect,
	Item,
	*/

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& Name, CollisionChannel Channel, bool CollisionEnable,
	CollisionState DefaultState)
{
	CollisionProfile* pProfile = FindProfile(Name);

	// 같은 이름으로 생성된 Profile이 있을 경우
	if (pProfile)
		return false;

	pProfile = new CollisionProfile;

	pProfile->Name = Name;
	pProfile->Channel = Channel;
	pProfile->CollisionEnable = CollisionEnable;

	for (int i = 0; i < (int)CollisionChannel::Max; ++i)
	{
		pProfile->vecState.push_back(DefaultState);
	}

	m_mapProfile.insert(std::make_pair(Name, pProfile));

	return true;
}

bool CCollisionManager::SetCollisionState(const std::string& Name, CollisionChannel Channel,
	CollisionState ChangeState)
{
	CollisionProfile* pProfile = FindProfile(Name);

	// 같은 이름으로 생성된 Profile이 있을 경우
	if (!pProfile)
		return false;

	pProfile->vecState[(int)Channel] = ChangeState;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& Name)
{
	auto	iter = m_mapProfile.find(Name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}
