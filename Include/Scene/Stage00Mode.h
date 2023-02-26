#pragma once
#include "SceneMode.h"

class CStage00Mode:
    public CSceneMode
{
	friend class CScene;

protected:
	CStage00Mode();
	virtual ~CStage00Mode();

private:
	static bool	m_bCheckDialogue;

public:
	virtual bool Init();
	virtual void Start();
private:
	void PlaySound(const std::string& Name);
	void PlayMainBgm();
	void StartDialogue();
	void ShowPlayer();
};