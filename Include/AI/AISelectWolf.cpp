#include "AISelectWolf.h"
#include "../Object/AI.h"
#include "../Resource/Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"
#include "AIController.h"
#include "../AI/AIBase.h"

CAISelectWolf::CAISelectWolf()
{
}

CAISelectWolf::~CAISelectWolf()
{
}

bool CAISelectWolf::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
	//if (!CAISelect::Run(pController, pOwner, pScene, DeltaTime))
	//	return false;

		// Attack 테스트가 진행중이라면 트리를 진행시키지 않는다.
	if (!pController->GetBoolData("AttackComplete")) 
		return true;

	CSceneMode* pSceneMode = pScene->GetSceneMode();

	CGameObject* pPlayer = pSceneMode->GetPlayer();

	// 플레이어와의 거리를 체크한다.
	float   Distance = pPlayer->GetPos().Distance(pOwner->GetPos());

	float   TraceDistance = pController->GetFloatData("TraceDistance");

	if (Distance <= TraceDistance)
	{
		pController->SetGameObjectData("Target", pPlayer);

		// 공격거리를 체크한다.
		float   AttackDist = pController->GetFloatData("AttackDistance");

		if (Distance <= AttackDist)
		{
			pController->SetBoolData("AttackComplete", false);
			pController->ChangeAI("Attack");
			pOwner->SetAIState(AIState::Attack);

			pController->SetFloatData("TargetPosX", pPlayer->GetPos().x);
			pController->SetFloatData("TargetPosY", pPlayer->GetPos().y);
		}

		else
		{
			pController->ChangeAI("Trace");
			pOwner->SetAIState(AIState::Trace);
		}
	}

	else
	{
		if (pOwner->GetView() == CharacterView::Right)
			pOwner->ChangeAnimation("Idle");
		else
			pOwner->ChangeAnimation("Idle_Left");

		pController->SetGameObjectData("Target", nullptr);
		pController->ChangeAI("Idle");
		pOwner->SetAIState(AIState::Standby);
	}

	return true;
}
