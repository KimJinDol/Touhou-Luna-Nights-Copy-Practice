
#include "AISelectGoblin.h"
#include "../Object/AI.h"
#include "../Resource/Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"
#include "AIController.h"
//#include "../Object/Goblin.h"

CAISelectGoblin::CAISelectGoblin()
{
}

CAISelectGoblin::~CAISelectGoblin()
{
}

bool CAISelectGoblin::Run(CAIController* pController, CAICharacter* pOwner,
    CScene* pScene, float DeltaTime)
{
    //if (!CAISelect::Run(pController, pOwner, pScene, DeltaTime))
    //    return false;

    if (!pController->GetBoolData("AttackComplete"))
        return true;

    CSceneMode* pSceneMode = pScene->GetSceneMode();

    CGameObject* pPlayer = pSceneMode->GetPlayer();

    // 플레이어와의 거리를 체크한다.
    float   Distance = pPlayer->GetPos().Distance(pOwner->GetPos());

	float   AttackDist = pController->GetFloatData("AttackDistance");

	if (Distance <= AttackDist)
	{
        pController->SetGameObjectData("Target", pPlayer);
        
		pController->SetBoolData("AttackComplete", false);
		pController->ChangeAI("GoblinAttack");
		pOwner->SetAIState(AIState::Attack);
    }

    else
    {
        pController->SetGameObjectData("Target", nullptr);
        pController->ChangeAI("Idle");
        pOwner->SetAIState(AIState::Standby);
    }

    return true;
}
