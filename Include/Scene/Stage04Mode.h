#pragma once
#include "SceneMode.h"
class CStage04Mode :
    public CSceneMode
{
	friend class CScene;

protected:
	CStage04Mode();
	virtual ~CStage04Mode();
	
public:
	virtual bool Init();

};

