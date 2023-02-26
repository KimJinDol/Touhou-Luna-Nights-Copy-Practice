
#include "StartScene.h"
#include "../Object/Archer.h"
#include "Scene.h"
#include "../Collision/ColliderRect.h"
#include "../Object/Portal.h"
#include "SceneResource.h"
#include "../Object/Smoke.h"
#include "Camera.h"
#include "../UI/StartSceneWidget.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	// 사운드 초기화
	InitSound();

	// UI 생성
	CSharedPtr<CStartSceneWidget> StartWidget = m_pScene->CreateWidget<CStartSceneWidget>("StartSceneWidget");


	return true;
}

void CStartScene::InitSound()
{
	CSceneResource* pResource = m_pScene->GetResourceManager();

	//pResource->LoadSound("BGM", true, "BGM1", "MainBgm.mp3");
	//pResource->LoadSound("Effect", false, "Hit", "1Up.wav");
	//pResource->LoadSound("Effect", false, "Portal", "Demasia.mp3");
	pResource->SetVolume(100);
}
