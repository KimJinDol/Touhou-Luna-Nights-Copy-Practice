#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"

struct BackInfo
{
	CSharedPtr<CTexture>    BackTexture;
	Vector2     Pos;
	Vector2     Size;
};


class CStage
{
	friend class CScene;
	friend class CEditorDlg;

protected:
	CStage();
	virtual ~CStage();

protected:
	class CScene* m_pScene;
	std::vector<BackInfo*>    m_vecBackTexture;
	bool            m_Start;
	class CTile**   m_pTileArray;
	int             m_TileCount;
	int             m_TileCapacity;
	int             m_TileCountX;
	int             m_TileCountY;
	Vector2         m_TileSize;
	bool            m_TileUpdate;
	bool            m_TileRender;
	bool            m_CreateTile;
	int             m_StartX;
	int             m_StartY;
	int             m_EndX;
	int             m_EndY;

	bool			m_bCanExitLeft; // 왼쪽 끝으로 넘어갈 수 있는지
	bool			m_bCanExitRight; // 오른쪽 끝으로 넘어갈 수 있는지

public:
	bool GetExitLeft()	const
	{
		return m_bCanExitLeft;
	}

	bool GetExitRight()	const
	{
		return m_bCanExitRight;
	}

	bool IsStart()  const
	{
		return m_Start;
	}

	int GetTileCountX() const
	{
		return m_TileCountX;
	}

	int GetTileCountY() const
	{
		return m_TileCountY;
	}

	int GetTileCount()  const
	{
		return m_TileCount;
	}

	int GetTileIndexX(const Vector2& Pos)
	{
		if (m_TileSize.x == 0.f || m_TileSize.y == 0.f)
			return -1;

		int IndexX = (int)Pos.x / (int)m_TileSize.x;

		if (IndexX < 0 || IndexX >= m_TileCountX)
			return -1;

		return IndexX;
	}

	int GetTileClampIndexX(const Vector2& Pos)
	{
		if (m_TileSize.x == 0.f || m_TileSize.y == 0.f)
			return -1;

		int IndexX = (int)Pos.x / (int)m_TileSize.x;

		if (IndexX < 0)
			return 0;

		else if (IndexX >= m_TileCountX)
			return m_TileCountX - 1;

		return IndexX;
	}

	int GetTileIndexY(const Vector2& Pos)
	{
		if (m_TileSize.x == 0.f || m_TileSize.y == 0.f)
			return -1;

		int IndexY = (int)Pos.y / (int)m_TileSize.y;

		if (IndexY < 0 || IndexY >= m_TileCountY)
			return -1;

		return IndexY;
	}

	int GetTileClampIndexY(const Vector2& Pos)
	{
		if (m_TileSize.x == 0.f || m_TileSize.y == 0.f)
			return -1;

		int IndexY = (int)Pos.y / (int)m_TileSize.y;

		if (IndexY < 0)
			return 0;

		else if (IndexY >= m_TileCountY)
			return m_TileCountY - 1;

		return IndexY;
	}

	int GetTileIndex(const Vector2& Pos)
	{
		if (m_TileSize.x == 0.f || m_TileSize.y == 0.f)
			return -1;

		int IndexX = (int)Pos.x / (int)m_TileSize.x;
		int IndexY = (int)Pos.y / (int)m_TileSize.y;

		if (IndexX < 0 || IndexX >= m_TileCountX || IndexY < 0 || IndexY >= m_TileCountY)
			return -1;

		return IndexY * m_TileCountX + IndexX;
	}

	class CTile* GetTile(const Vector2& Pos)
	{
		int Index = GetTileIndex(Pos);

		if (Index == -1)
			return nullptr;

		return m_pTileArray[Index];
	}

	class CTile* GetTile(int Index)
	{
		if (Index < 0 || Index >= m_TileCount)
			return nullptr;

		return m_pTileArray[Index];
	}

public:
	void AddBackTexture(const std::string& Name);
	void AddBackTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName = TEXTURE_PATH);
	void SetBackTexture(int Index, const std::string& Name);
	void SetBackTexture(int Index, const std::string& Name, const TCHAR* pFileName, const std::string& PathName = TEXTURE_PATH);
	void SetPos(int Index, const Vector2& Pos);
	void SetPos(int Index, float x, float y);
	void SetSize(int Index, const Vector2& Size);
	void SetSize(int Index, float x, float y);


public:
	bool CreateTile(bool NewStage, int CountX, int CountY, const Vector2& TileSize, const std::string& BaseTileTextureName = "",
		const TCHAR* pTextureFileName = nullptr, const std::string& BaseTileTexturePathName = TEXTURE_PATH);
	void SetTileColorKey(unsigned char r, unsigned char g, unsigned char b);
	void SetAllTileTexture(const std::string& Name, const TCHAR* pFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetAllTileTexture(const std::string& Name);

public:
	void SetExitSide(bool LeftValue, bool RightValue)
	{
		m_bCanExitLeft = LeftValue;
		m_bCanExitRight = RightValue;
	}

	void ChangeTileType(const Vector2& Pos, TileType Type);
	void ChangeTileCollision(const Vector2& Pos);
	void MouseOnTileCollision(const Vector2& Pos, class CTile* pTile);
	void ChangeTileImage(const Vector2& Pos, class CTexture* pTexture,
		const Vector2& StartPos, const Vector2& Size);
	void AddTileImage(const Vector2& Pos, class CTexture* pTexture, const Vector2& StartPos, const Vector2& Size);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void SaveFile(const char* pFullPath);
	void LoadFile(const char* pFullPath);
	void SaveFile(const char* pFileName, const std::string& PathName);
	void LoadFile(const char* pFileName, const std::string& PathName);
};

