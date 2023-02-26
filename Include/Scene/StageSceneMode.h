#pragma once
#include "SceneMode.h"

class CStageSceneMode :
    public CSceneMode
{
	friend class CScene;

protected:
	CStageSceneMode();
	virtual ~CStageSceneMode();

private:
	CSharedPtr<class CMap> m_BlackMask;
	
public:
	virtual bool Init();
	virtual void Start();
private:
	void StartDialogue();
	void BlackReverse();
	void NextScene();
};

