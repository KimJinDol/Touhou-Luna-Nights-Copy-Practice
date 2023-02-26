#pragma once
#include "SceneMode.h"
class CStartScene :
    public CSceneMode
{
	friend class CScene;

protected:
	CStartScene();
	virtual ~CStartScene();

public:
	virtual bool Init();

private:
	void InitSound();
};

