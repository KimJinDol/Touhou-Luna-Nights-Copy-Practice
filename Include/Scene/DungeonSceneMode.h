#pragma once
#include "SceneMode.h"
class CDungeonSceneMode :
    public CSceneMode
{
	friend class CScene;

protected:
	CDungeonSceneMode();
	virtual ~CDungeonSceneMode();

public:
	virtual bool Init();
};

