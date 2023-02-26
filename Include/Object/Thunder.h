#pragma once
#include "StaticObject.h"
#include "../Interface.h"

class CThunder :
	public CStaticObject, public DamageableInterface
{
	friend class CScene;

protected:
	CThunder();
	CThunder(const CThunder& obj);
	virtual ~CThunder();

private:
	float	m_ActiveTime;
	float	m_curActiveTime;
	float	m_UnActiveTime;
	float	m_curUnActiveTime;

	float	m_AD;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	virtual float GetAD() const 
	{
		return m_AD;
	}

private:
	void ThunderStart();
	void ThunderEnd();


	virtual CThunder* Clone() const;
};

