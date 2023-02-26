#pragma once
#include "SceneMode.h"
#include "../Stage/StageEditMode.h"

class CEditorSceneMode :
	public CSceneMode
{
	friend class CScene;

protected:
	CEditorSceneMode();
	virtual ~CEditorSceneMode();

protected:
	class CEditorDlg* m_pDlg;
	float	m_ScrollSpeed;
	Vector2 m_TileResourceModeCamPos;
	Vector2 m_EditorModeCamPos;

	class CTile* m_CollTile; // 콜리전 작업중인 타일

public:
	CEditorDlg* GetEditorDlg() const
	{
		return m_pDlg;
	}

	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void OpenEditorDlg(float DeltaTime);
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void MousePress(float DeltaTime);
	void MouseDown(float DeltaTime);
	void MouseUp(float DeltaTime);
	void ChangeTileScreen(float DeltaTime);
};

