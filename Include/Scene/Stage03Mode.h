#pragma once
#include "SceneMode.h"
class CStage03Mode :
    public CSceneMode
{
	friend class CScene;

protected:
	CStage03Mode();
	virtual ~CStage03Mode();

public:
	virtual bool Init();

private:
	void InitSound();
};

