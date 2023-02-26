#pragma once

enum class EKeyState
{
	Down,	// 누를때
	Press,	// 누르고있을때
	Up,		// 누르던 키를 해제할때
	Max
};

enum class ColliderType
{
	Rect,
	Sphere,
	Point,
	Line,
	Pixel
};

enum class CollisionChannel
{
	Object,
	Player,
	Monster,
	PlayerAttack,
	MonsterAttack,
	MonsterDetect,
	Item,
	PlayerTrigger,
	MonsterTrigger,
	Map,
	Raycast,
	Block,
	Max
};

enum class CollisionState
{
	Ignore,
	Collision,
	Block
};

enum class SceneType
{
	None,
	Stage00,
	Stage01,
	Stage02,
	Stage03,
	Stage04,
	StageEnd
};

enum class AnimationType
{
	Sprite,
	Frame
};

enum class CharacterView
{
	Left,
	Right
};

enum class UIShape
{
	Rect,
	Sphere
};

enum class TimeMode
{
	Move,
	Stop,
	Snail
};

enum class TileType
{
	None,
	Block,
	Slow,
	Damage,
	End
};

enum class TileEditMode
{
	Type,
	Collision,
	Image,
	Clear,
	End
};

enum class TileInteraction
{
	None,
	Block,
	Destroy
};

enum class AIState
{
	Standby,
	Trace,
	Attack
};

enum class MessageBoxType
{
	MBT_1,
	MBT_2,
	MBT_3,
	MBT_4
};