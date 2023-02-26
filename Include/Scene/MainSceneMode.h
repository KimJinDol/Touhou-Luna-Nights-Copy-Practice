#pragma once
#include "SceneMode.h"
class CMainSceneMode :
    public CSceneMode
{
	friend class CScene;

protected:
	CMainSceneMode();
	virtual ~CMainSceneMode();

public:
	virtual bool Init();

private:
	void InitSound();
};

