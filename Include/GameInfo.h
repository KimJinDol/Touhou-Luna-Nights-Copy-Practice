#pragma once

#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
// crtdbg.h : 메모리 릭 체크를 위한 기능을 제공해주는 헤더파일이다.
#include <crtdbg.h>
#include <string>
#include <functional>
#include <algorithm>
#include <typeinfo>

#include "resource.h"
#include "Macro.h"
#include "Math.h"
#include "Vector2.h"
#include "Flag.h"
#include "SharedPtr.h"
#include "fmod.hpp"

// lib 파일을 링크를 걸어서 lib파일 안에 있는 코드를 여기에서 사용할 수 있게 해준다.
#pragma comment(lib, "msimg32.lib")

#ifdef _WIN64

#pragma comment(lib, "fmod64_vc.lib")

#else

#pragma comment(lib, "fmod_vc.lib")

#endif // _WIN64

#define	GRAVITY		9.8f

#define	KEY_MOUSEWHEEL	0x100
#define	KEY_MOUSEX		0x101
#define	KEY_MOUSEY		0x102

#define	KEYCOUNT_MAX	259

#define	ROOT_PATH		"RootPath"
#define	TEXTURE_PATH	"TexturePath"
#define	SOUND_PATH		"SoundPath"
#define	DATA_PATH		"DataPath"


typedef struct _tagResolution
{
	int	Width;
	int	Height;
}Resolution;

typedef struct _tagRectInfo
{
	float	Left;
	float	Top;
	float	Right;
	float	Bottom;

	RECT	HitRect;

	_tagRectInfo() :
		Left(0.f),
		Top(0.f),
		Right(0.f),
		Bottom(0.f)
	{
	}
}RectInfo;

typedef struct _tagSphereInfo
{
	Vector2	Center;
	float	Radius;

	_tagSphereInfo()	:
		Radius(0.f)
	{
	}
}SphereInfo;

typedef struct _tagCollisionProfile
{
	std::string			Name;
	CollisionChannel	Channel;
	bool				CollisionEnable;

	// 각 채널과 어떤 처리를 해야 하는지를 저장해둔다.
	std::vector<CollisionState>	vecState;
}CollisionProfile;
