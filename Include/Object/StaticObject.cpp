#include "StaticObject.h"

CStaticObject::CStaticObject()
{
}

CStaticObject::CStaticObject(const CStaticObject& obj)	:
	CGameObject(obj)
{
}

CStaticObject::~CStaticObject()
{
}


bool CStaticObject::Init()
{
	return true;
}

void CStaticObject::Start()
{
	CGameObject::Start();
}

void CStaticObject::SetInput()
{
	CGameObject::SetInput();
}

void CStaticObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CStaticObject::Collision()
{
	CGameObject::Collision();
}

void CStaticObject::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}

CStaticObject* CStaticObject::Clone() const
{
	return new CStaticObject(*this);
}
