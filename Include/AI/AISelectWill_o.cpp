#include "AISelectWill_o.h"
#include "../Object/AI.h"
#include "../Resource/Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"
#include "AIController.h"
#include "../AI/AIBase.h"

CAISelectWill_o::CAISelectWill_o()
{
}

CAISelectWill_o::~CAISelectWill_o()
{
}

bool CAISelectWill_o::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{

	CSceneMode* pSceneMode = pScene->GetSceneMode();
	CGameObject* pPlayer = pSceneMode->GetPlayer();

	// 플레이어와의 거리를 체크한다.
	float   Distance = pPlayer->GetPos().Distance(pOwner->GetCenterPos());

	float   TraceDistance = pController->GetFloatData("TraceDistance");

	if (Distance <= TraceDistance)
	{
		pController->SetGameObjectData("Target", pPlayer);

		pController->ChangeAI("Trace");
		pOwner->SetAIState(AIState::Trace);
	}

	return true;
}
