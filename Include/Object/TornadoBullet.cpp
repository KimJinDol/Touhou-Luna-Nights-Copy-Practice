
#include "TornadoBullet.h"

CTornadoBullet::CTornadoBullet()
{
}

CTornadoBullet::CTornadoBullet(const CTornadoBullet& obj) :
	CBullet(obj)
{
}

CTornadoBullet::~CTornadoBullet()
{
}

bool CTornadoBullet::Init()
{
	if (!CBullet::Init())
		return false;

	m_RotationAngle = 0.f;
	m_RotationRange = 100.f;

	return true;
}

void CTornadoBullet::Start()
{
	CBullet::Start();

	m_MovePos = m_vPos;
}

void CTornadoBullet::Update(float DeltaTime)
{
	m_MovePos.x += m_vMoveDir.x * m_MoveSpeed * DeltaTime;
	m_MovePos.y += m_vMoveDir.y * m_MoveSpeed * DeltaTime;

	m_Distance -= m_MoveSpeed * DeltaTime;

	if (m_Distance <= 0.f)
		Release();

	m_RotationAngle += 720.f * DeltaTime;

	// 현재 부모에서 구해준 위치를 중심으로 회전된 위치를 만들어준다.
	m_vPos.x = m_MovePos.x + cosf(DegreeToRadian(m_RotationAngle)) * m_RotationRange;
	m_vPos.y = m_MovePos.y + sinf(DegreeToRadian(m_RotationAngle)) * m_RotationRange;

	CBullet::Update(DeltaTime);
}

void CTornadoBullet::Render(HDC hDC, float DeltaTime)
{
	CBullet::Render(hDC, DeltaTime);
}

CTornadoBullet* CTornadoBullet::Clone() const
{
	return new CTornadoBullet(*this);
}
