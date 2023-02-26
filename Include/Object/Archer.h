#pragma once
#include "Player.h"
class CArcher :
    public CPlayer
{
	friend class CScene;

protected:
	CArcher();
	CArcher(const CArcher& obj);
	virtual ~CArcher();

public:
	virtual bool Init();
	virtual void Start();
	virtual void SetInput();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CArcher* Clone()	const;

private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void Fire(float DeltaTime);
	void Skill1(float DeltaTime);
	void Skill2(float DeltaTime);
	void Skill3(float DeltaTime);
	void Skill4(float DeltaTime);

public:
	void FireNotify();
};

