#include "ColliderPixel.h"
#include "../PathManager.h"
#include "Collision.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"


CColliderPixel::CColliderPixel()
{
	m_Type = ColliderType::Pixel;
	m_Width = 0.f;
	m_Height = 0.f;
}

CColliderPixel::CColliderPixel(const CColliderPixel& collider)
{
	m_Width = collider.m_Width;
	m_Height = collider.m_Height;
}

CColliderPixel::~CColliderPixel()
{
}

bool CColliderPixel::SetPixelInfo(const char* pFileName, const std::string& strPathKey)
{
	const PathInfo* pInfo = GET_SINGLE(CPathManager)->FindPath(strPathKey);
	const char* pPath = pInfo->PathMultibyte;
	std::string  strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	FILE* pFile = NULL;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)  return false;

	BITMAPFILEHEADER        fh; // ��Ʈ�� ���� ��� ����ü
	BITMAPINFOHEADER        ih; // ��Ʈ�� ���� ��� ����ü

	fread(&fh, sizeof(fh), 1, pFile);
	fread(&ih, sizeof(ih), 1, pFile);

	m_Width = ih.biWidth;
	m_Height = ih.biHeight;

	m_vecPixel.clear();

	m_vecPixel.resize(m_Width * m_Height);

	fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);

	PPIXEL  pPixelArr = new PIXEL[m_Width];

	// ��Ʈ�� �ȼ��� ������ �Ʒ����� �� �������� �귯���� ������
	// �� �Ʒ��� ���������ش�.
	for (int i = 0; i < m_Height / 2; ++i)
	{
		// ���� �ε����� �ȼ� �� ���� �����ϱ� ���� �����صд�.
		memcpy(pPixelArr, &m_vecPixel[i * m_Width], sizeof(PIXEL) * m_Width);
		memcpy(&m_vecPixel[i * m_Width], &m_vecPixel[(m_Height - i - 1) * m_Width], sizeof(PIXEL) * m_Width);
		memcpy(&m_vecPixel[(m_Height - i - 1) * m_Width], pPixelArr, sizeof(PIXEL) * m_Width);
	}

	delete[] pPixelArr;

	fclose(pFile);

	//     fopen_s(&pFile, "Test.bmp", "wb");
	// 
	//     if(!pFile) return false;
	// 
	//     fwrite(&fh, sizeof(fh), 1, pFile);
	//     fwrite(&ih, sizeof(ih), 1, pFile);
	//     fwrite(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);
	// 
	//     fclose(pFile);

	return true;
}

bool CColliderPixel::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderPixel::Start()
{
	CCollider::Start();
}

void CColliderPixel::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderPixel::Render(HDC hDC, float DeltaTime)
{
	CCollider::Render(hDC, DeltaTime);
}

CColliderPixel* CColliderPixel::Clone() const
{
	return new CColliderPixel(*this);
}

bool CColliderPixel::CollisionMouse()
{
	// �ʿ��� �� ¥��.
	return false;
}

bool CColliderPixel::Collision(CCollider* pDest)
{
	switch (pDest->GetColliderType())
	{
	case ColliderType::Rect:
		return CCollision::CollisionRectToPixel((CColliderRect*)pDest, this);
	case ColliderType::Sphere:
		return CCollision::CollisionSphereToPixel((CColliderSphere*)pDest, this);
	case ColliderType::Point:
		break;
	}

	return false;
}
