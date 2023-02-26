#pragma once
#include "SceneMode.h"
class CStage01Mode :
    public CSceneMode
{
	friend class CScene;

protected:
	CStage01Mode();
	virtual ~CStage01Mode();

public:
	virtual bool Init();

private:
	void InitSound();
};

