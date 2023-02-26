#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"
#include "../Resource/Animation.h"
#include "../Collision/ColliderLine.h"

struct TileDrawInfo
{
	CSharedPtr<CTexture>    m_Texture;
	Vector2 m_ImagePos;
};

class CTile
{
	friend class CStage;

private:
	CTile();
	~CTile();

private:
	class CScene* m_pScene;
	class CStage* m_pStage;

private:
	CAnimation* m_pAnimation;
	std::vector<TileDrawInfo*> m_vecTileDrawInfo;
	int			m_DrawInfoCount;
	Vector2     m_Pos;
	Vector2		m_Size;
	TileType	m_TileType;
	int			m_IndexX;
	int			m_IndexY;
	int			m_Index;
	bool		m_Render;

	class CColliderLine*	m_Line;

public:
	class CColliderLine* GetCollisionLine() const
	{
		return m_Line;
	}

	TileType GetTileType()	const
	{
		return m_TileType;
	}

	Vector2 GetPos()	const
	{
		return m_Pos;
	}

	Vector2 GetSize()	const
	{
		return m_Size;
	}

	int GetTileIndex()	const
	{
		return m_Index;
	}

	int GetTileIndexX()	const
	{
		return m_IndexX;
	}

	int GetTileIndexY()	const
	{
		return m_IndexY;
	}

public:
	void SetCollisionLine(class CColliderLine* pLine)
	{
		if (!pLine) return;

		if (m_Line)
		{
			m_Line->SetEndPoint(m_Pos.x - m_Size.x, m_Pos.y);
			SAFE_RELEASE(m_Line);
		}

		m_Line = pLine;
		m_Line->AddRef();
	}

	void ClearImage()
	{
		auto iter = m_vecTileDrawInfo.begin();
		auto iterEnd = m_vecTileDrawInfo.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(*iter);
		}

		m_vecTileDrawInfo.clear();

		SAFE_DELETE(m_Line);

		m_DrawInfoCount = 0;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetImagePos(const Vector2& Pos)
	{
		if (m_DrawInfoCount > 0)
			m_vecTileDrawInfo[m_DrawInfoCount - 1]->m_ImagePos = Pos;
	}

	void SetImagePos(float x, float y)
	{
		if (m_DrawInfoCount > 0)
			m_vecTileDrawInfo[m_DrawInfoCount - 1]->m_ImagePos = Vector2(x, y);
	}

	void AddDrawInfo(const Vector2& Pos, class CTexture* pTexture)
	{
		auto iter = m_vecTileDrawInfo.begin();
		auto iterEnd = m_vecTileDrawInfo.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->m_Texture == pTexture && (*iter)->m_ImagePos.x == Pos.x && (*iter)->m_ImagePos.y == Pos.y)
				return;
		}

		TileDrawInfo* pInfo = new TileDrawInfo;
		pInfo->m_Texture = pTexture;
		pInfo->m_ImagePos = Pos;

		m_vecTileDrawInfo.push_back(pInfo);

		++m_DrawInfoCount;

		m_Render = true;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}

	void SetTileType(TileType Type)
	{
		m_TileType = Type;
	}

	void CreateTileCollisionLine();


	void SetIndex(int IndexX, int IndexY, int Index)
	{
		m_IndexX = IndexX;
		m_IndexY = IndexY;
		m_Index = Index;
	}

public:
	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* pFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTexture(class CTexture* pTexture);
	void SetColorKey(unsigned char r, unsigned char g, unsigned char b,
		int Index = 0);

public:
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		class CTexture* pTexture, const std::vector<AnimationFrameData>& vecFrame,
		float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		class CTexture* pTexture, float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		const std::string& TextureName, float PlayTime = 1.f);
	bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
		const std::string& TextureName, const TCHAR* pTexFileName,
		float PlayTime = 1.f);
	bool AddAnimationFrameData(const std::string& Name,
		const Vector2& ImageStartPos, const Vector2& Size);
	bool AddAnimationFrameData(const std::string& Name,
		const Vector2& ImageStartPos, const Vector2& Size,
		int Count);
	void SetPlayScale(const std::string& Name, float Scale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	void SetAnimationReverse(const std::string& Name, bool bReverse);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void Render(HDC hDC, float DeltaTime);
	void Collision(float DeltaTime);
	void RenderDebug(HDC hDC, float DeltaTime);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

};

