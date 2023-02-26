#pragma once
#include "SceneMode.h"

class CStage02Mode :
	public CSceneMode
{
	friend class CScene;

protected:
	CStage02Mode();
	virtual ~CStage02Mode();

public:
	virtual bool Init();

private:
	void InitSound();
};