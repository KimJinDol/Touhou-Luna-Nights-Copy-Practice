#include "Player.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"
#include "../Input.h"
#include "../Collision/ColliderRect.h"
#include "../Collision/ColliderSphere.h"
#include "TornadoBullet.h"
#include "PlayerSubObj.h"
#include "PlayerRotationObj.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneResource.h"
#include "GuideBullet.h"
#include "../GameManager.h"
#include "../Timer.h"
#include "../Scene/Camera.h"
#include "PlayerRedRingEffect.h"
#include "../UI/PlayerWidget.h"
#include "../UI/UIWidget.h"
#include "../UI/UIImage.h"
#include "../UI/UIPlayerWidgetWorld.h"
#include "../UI/UIProgressBar.h"
#include "AI.h"
#include "../UI/TextManager.h"
#include "../Collision/CollisionManager.h"

float CPlayer::s_HP = 100.f;
float CPlayer::s_MP = 100.f;
float CPlayer::s_TimeStopGauge = 100.f;
bool CPlayer::s_bCanTimeStopSkill = false;

CPlayer::CPlayer() :
	m_fSuperAttackTime(0.12f),
	m_fCurrentSuperAttackTime(m_fSuperAttackTime),
	m_IsDialogue(false),
	m_TimeStopKnifeNumMax(18),
	m_TimeStopKnifeNum(m_TimeStopKnifeNumMax),
	m_BlinkTime(0.05f),
	m_curBlinkTime(m_BlinkTime),
	m_DamageView(CharacterView::Right),
	m_IsInteraction(false),
	m_bCanInvincible(true)
{
}

CPlayer::CPlayer(const CPlayer& obj)	:
	CCharacter(obj)
{
	m_bCanAttack = obj.m_bCanAttack;
	m_bCanTimeStop = obj.m_bCanTimeStop;
	s_TimeStopGauge = obj.s_TimeStopGauge;
}

CPlayer::~CPlayer()
{
	if (GET_TIMER->FindTimer("Invincible") != nullptr)
	{
		ReleaseInvincible();
		GET_TIMER->RemoveTimer("Invincible");
	}
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	m_vPivot = Vector2(0.5f, 1.f);
	m_vSize = Vector2(85.f, 130.f);
	m_MoveSpeed = 420.f;

	m_Level = 1;

	m_HP = s_HP;
	m_HPMax = 100;

	m_MP = s_MP;
	m_MPMax = 100;
	m_MPIncreasePerSec = 0.5f;

	m_pJumpEndFunc = std::bind(&CPlayer::JumpEnd, this);

	m_PlayerAnim = PlayerAnimation::Idle;
	m_bAutoIdle = true;
	m_bIsMove = false;
	m_bCanMove = true;
	m_bCanAttack = true;
	m_bCanJump = true;
	m_bIsJump = false;
	m_bIsGliding = false;
	m_bCanTimeStop = true;
	m_bCanSlowTime = true;

	m_bIsAction = false;
	m_ComboMax = 4;
	m_CurCombo = 0;
	m_MPCost_Action = 3;

	m_TimeStopGaugeIncreaseSpeed = 5.f;
	/*m_TimeStopKeyInputTime = 0.f;*/
	
	m_bIsSlowTime = false;
	m_SlowTimeGauge = 0.f;
	m_SlowTimeMaxGauge = 1.f;
	m_SlowTimeCanInputGauge = 0.25f;

	 
	// Physics
	m_BasicJumpVelocity = 70.f;
	SetJumpVelocity(m_BasicJumpVelocity);

	// Collider
	RectComponent = AddCollider<CColliderRect>("PlayerBody");
	RectComponent->SetExtent(m_vSize);
	/*PlayerBody->SetOffset(0.f, -PlayerBody->GetHeight()/2);*/
	RectComponent->SetCollisionProfile("Player");
	RectComponent->SetBeginCollisionFunction(this, &CPlayer::BeginCollision);
 

	/*SetColorKey(255, 0, 255);*/

	// Idle
	AddAnimation("PlayerStop", AnimationType::Sprite, true, "Player_Atlas00", 1.5f);
	AddAnimationFrameData("PlayerStop", 9, 9, 19);

	AddAnimation("PlayerStop_Left", AnimationType::Sprite, true, "Player_Atlas00_Left", 1.5f);
	AddAnimationFrameData("PlayerStop_Left", 9, 9, 19);

	// Fight Idle
	AddAnimation("PlayerFightStop", AnimationType::Sprite, true, "Player_Atlas01", 2.f);
	AddAnimationFrameData("PlayerFightStop", 7, 3, 21);

	AddAnimation("PlayerFightStop_Left", AnimationType::Sprite, true, "Player_Atlas01_Left", 2.f);
	AddAnimationFrameData("PlayerFightStop_Left", 7, 3, 21);

	// RunStart
	AddAnimation("PlayerRunStart", AnimationType::Sprite, false, "Player_Atlas00", 0.5f);
	AddAnimationFrameData("PlayerRunStart", 4, 8, 8);

	AddAnimation("PlayerRunStart_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.5f);
	AddAnimationFrameData("PlayerRunStart_Left", 4, 8, 8);
	
	// Run
	AddAnimation("PlayerRun", AnimationType::Sprite, true, "Player_Atlas00", 0.7f);
	AddAnimationFrameData("PlayerRun", 1, 3, 16);
	SetNextAnimation("PlayerRunStart", "PlayerRun");

	AddAnimation("PlayerRun_Left", AnimationType::Sprite, true, "Player_Atlas00_Left", 0.7f);
	AddAnimationFrameData("PlayerRun_Left", 1, 3, 16);
	SetNextAnimation("PlayerRunStart_Left", "PlayerRun_Left");
	
	// RunStop
	AddAnimation("PlayerRunStop", AnimationType::Sprite, false, "Player_Atlas00", 0.4f);
	AddAnimationFrameData("PlayerRunStop", 2, 9, 7);
	SetNextAnimation("PlayerRunStop", "PlayerStop");

	AddAnimation("PlayerRunStop_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.4f);
	AddAnimationFrameData("PlayerRunStop_Left", 2, 9, 7);
	SetNextAnimation("PlayerRunStop_Left", "PlayerStop_Left");

	// RunBack
	AddAnimation("PlayerRunBack", AnimationType::Sprite, false, "Player_Atlas00", 0.5f);
	AddAnimationFrameData("PlayerRunBack", 4, 7, 9);
	SetNextAnimation("PlayerRunBack", "PlayerRun");

	AddAnimation("PlayerRunBack_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.5f);
	AddAnimationFrameData("PlayerRunBack_Left", 4, 7, 9);
	SetNextAnimation("PlayerRunBack_Left", "PlayerRun_Left");

	// JumpLoop
	AddAnimation("PlayerJumpLoop", AnimationType::Sprite, true, "Player_Atlas00", .2f);
	AddAnimationFrameData("PlayerJumpLoop", 7, 2, 2);
	// SetNextAnimation("PlayerJumpLoop", "PlayerStop");

	AddAnimation("PlayerJumpLoop_Left", AnimationType::Sprite, true, "Player_Atlas00_Left", .2f);
	AddAnimationFrameData("PlayerJumpLoop_Left", 7, 2, 2);
	// SetNextAnimation("PlayerJumpLoop_Left", "PlayerStop_Left");

	// Falling
	AddAnimation("PlayerFalling", AnimationType::Sprite, false, "Player_Atlas00", 0.3f);
	AddAnimationFrameData("PlayerFalling", 4, 2, 5);
	SetNextAnimation("PlayerFalling", "PlayerJumpLoop");

	AddAnimation("PlayerFalling_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.3f);
	AddAnimationFrameData("PlayerFalling_Left", 4, 2, 5);
	SetNextAnimation("PlayerFalling_Left", "PlayerJumpLoop_Left");

	// JumpStart
	AddAnimation("PlayerJumpStart", AnimationType::Sprite, true, "Player_Atlas00", 0.2f);
	AddAnimationFrameData("PlayerJumpStart", 9, 2, 2);
	//SetNextAnimation("PlayerJumpStart", "PlayerFalling");

	AddAnimation("PlayerJumpStart_Left", AnimationType::Sprite, true, "Player_Atlas00_Left", 0.2f);
	AddAnimationFrameData("PlayerJumpStart_Left", 9, 2, 2);
	//SetNextAnimation("PlayerJumpStart_Left", "PlayerFalling_Left");

	// Gliding Loop
	AddAnimation("PlayerGlidingLoop", AnimationType::Sprite, true, "Player_Atlas00", 0.2f);
	AddAnimationFrameData("PlayerGlidingLoop", 3, 17, 3);

	AddAnimation("PlayerGlidingLoop_Left", AnimationType::Sprite, true, "Player_Atlas00_Left", .2f);
	AddAnimationFrameData("PlayerGlidingLoop_Left", 3, 17, 3);

	// Gliding Start
	AddAnimation("PlayerGlidingStart", AnimationType::Sprite, false, "Player_Atlas00", 0.2f);
	AddAnimationFrameData("PlayerGlidingStart", 0, 17, 3);
	SetNextAnimation("PlayerGlidingStart", "PlayerGlidingLoop");

	AddAnimation("PlayerGlidingStart_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.2f);
	AddAnimationFrameData("PlayerGlidingStart_Left", 0, 17, 3);
	SetNextAnimation("PlayerGlidingStart_Left", "PlayerGlidingLoop_Left");

	// Damage
	AddAnimation("PlayerDamage", AnimationType::Sprite, false, "Player_Atlas01", 1.f);
	AddAnimationFrameData("PlayerDamage", 4, 0, 4);

	AddAnimation("PlayerDamage_Left", AnimationType::Sprite, false, "Player_Atlas01_Left", 1.f);
	AddAnimationFrameData("PlayerDamage_Left", 4, 0, 4);

	// Fall Down
	AddAnimation("PlayerFallDown", AnimationType::Sprite, false, "Player_Atlas01", 0.6f);
	AddAnimationFrameData("PlayerFallDown", 9, 2, 8);
	m_pAnimation->SetEndFunction("PlayerFallDown", this, &CPlayer::EndPlayDamageAnim);

	AddAnimation("PlayerFallDown_Left", AnimationType::Sprite, false, "Player_Atlas01_Left", 0.6f);
	AddAnimationFrameData("PlayerFallDown_Left", 9, 2, 8);
	m_pAnimation->SetEndFunction("PlayerFallDown_Left", this, &CPlayer::EndPlayDamageAnim);



	// Action1
	AddAnimation("PlayerAction1", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction1", 8, 1, 6);
	SetAnimationEndFunction("PlayerAction1", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction1",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction1_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction1_Left", 8, 1, 6);
	SetAnimationEndFunction("PlayerAction1_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction1_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction1_Run", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction1_Run", 7, 4, 6);
	SetAnimationEndFunction("PlayerAction1_Run", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction1_Run",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction1_Run_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction1_Run_Left", 7, 4, 6);
	SetAnimationEndFunction("PlayerAction1_Run_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction1_Run_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttackAir", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);
	AddAnimationFrameData("PlayerSuperAttackAir", 6, 19, 4);
	SetAnimationEndFunction("PlayerSuperAttackAir", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttackAir",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttackAir_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttackAir_Left", 6, 19, 4);
	SetAnimationEndFunction("PlayerSuperAttackAir_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttackAir_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack", 0, 18, 4);
	SetAnimationEndFunction("PlayerSuperAttack", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack_Left", 0, 18, 4);
	SetAnimationEndFunction("PlayerSuperAttack_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack_Left",
		2, this, &CPlayer::Action);

	// Action2
	AddAnimation("PlayerAction2", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction2", 0, 0, 6);
	SetAnimationEndFunction("PlayerAction2", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction2",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction2_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction2_Left", 0, 0, 6);
	SetAnimationEndFunction("PlayerAction2_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction2_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction2_Run", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction2_Run", 5, 6, 6);
	SetAnimationEndFunction("PlayerAction2_Run", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction2_Run",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction2_Run_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction2_Run_Left", 5, 6, 6);
	SetAnimationEndFunction("PlayerAction2_Run_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction2_Run_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack2Air", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack2Air", 0, 20, 4);
	SetAnimationEndFunction("PlayerSuperAttack2Air", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack2Air",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack2Air_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack2Air_Left", 0, 20, 4);
	SetAnimationEndFunction("PlayerSuperAttack2Air_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack2Air_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack2", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack2", 4, 18, 4);
	SetAnimationEndFunction("PlayerSuperAttack2", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack2",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack2_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack2_Left", 4, 18, 4);
	SetAnimationEndFunction("PlayerSuperAttack2_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack2_Left",
		2, this, &CPlayer::Action);

	// Action3
	AddAnimation("PlayerAction3", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction3", 6, 0, 6);
	SetAnimationEndFunction("PlayerAction3", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction3",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction3_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction3_Left", 6, 0, 6);
	SetAnimationEndFunction("PlayerAction3_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction3_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction3_Run", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction3_Run", 3, 5, 6);
	SetAnimationEndFunction("PlayerAction3_Run", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction3_Run",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction3_Run_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction3_Run_Left", 3, 5, 6);
	SetAnimationEndFunction("PlayerAction3_Run_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction3_Run_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack3Air", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack3Air", 4, 20, 4);
	SetAnimationEndFunction("PlayerSuperAttack3Air", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack3Air",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack3Air_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack3Air_Left", 4, 20, 4);
	SetAnimationEndFunction("PlayerSuperAttack3Air_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack3Air_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack3", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack3", 8, 18, 4);
	SetAnimationEndFunction("PlayerSuperAttack3", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack3",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack3_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack3_Left", 8, 18, 4);
	SetAnimationEndFunction("PlayerSuperAttack3_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack3_Left",
		2, this, &CPlayer::Action);

	// Action4
	AddAnimation("PlayerAction4", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction4", 2, 1, 6);
	SetAnimationEndFunction("PlayerAction4", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction4",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction4_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction4_Left", 2, 1, 6);
	SetAnimationEndFunction("PlayerAction4_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction4_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction4_Run", AnimationType::Sprite, false, "Player_Atlas00", 0.32f);
	AddAnimationFrameData("PlayerAction4_Run", 9, 5, 6);
	SetAnimationEndFunction("PlayerAction4_Run", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction4_Run",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerAction4_Run_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.32f);
	AddAnimationFrameData("PlayerAction4_Run_Left", 9, 5, 6);
	SetAnimationEndFunction("PlayerAction4_Run_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerAction4_Run_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack4Air", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);	
	AddAnimationFrameData("PlayerSuperAttack4Air", 8, 20, 4);
	SetAnimationEndFunction("PlayerSuperAttack4Air", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack4Air",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack4Air_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack4Air_Left", 8, 20, 4);
	SetAnimationEndFunction("PlayerSuperAttack4Air_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack4Air_Left",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack4", AnimationType::Sprite, false, "Player_Atlas00", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack4", 1, 19, 4);
	SetAnimationEndFunction("PlayerSuperAttack4", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack4",
		2, this, &CPlayer::Action);

	AddAnimation("PlayerSuperAttack4_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.25f);
	AddAnimationFrameData("PlayerSuperAttack4_Left", 1, 19, 4);
	SetAnimationEndFunction("PlayerSuperAttack4_Left", this, &CPlayer::ActionComboCheck);
	SetAnimationNotifyFunction<CPlayer>("PlayerSuperAttack4_Left",
		2, this, &CPlayer::Action);

	// UpShot
	AddAnimation("PlayerUpShot", AnimationType::Sprite, false, "Player_Atlas00", 0.43f);
	AddAnimationFrameData("PlayerUpShot", 0, 14, 10);
	SetAnimationEndFunction("PlayerUpShot", this, &CPlayer::UpShotEnd);
	SetAnimationNotifyFunction<CPlayer>("PlayerUpShot",
		2, this, &CPlayer::UpShot);

	AddAnimation("PlayerUpShot_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.43f);
	AddAnimationFrameData("PlayerUpShot_Left", 0, 14, 10);
	SetAnimationEndFunction("PlayerUpShot_Left", this, &CPlayer::UpShotEnd);
	SetAnimationNotifyFunction<CPlayer>("PlayerUpShot_Left",
		2, this, &CPlayer::UpShot);

	// UpShot Run
	AddAnimation("PlayerUpShot_Run", AnimationType::Sprite, false, "Player_Atlas00", 0.43f);
	AddAnimationFrameData("PlayerUpShot_Run", 0, 16, 10);
	SetAnimationEndFunction("PlayerUpShot_Run", this, &CPlayer::UpShotEnd);
	SetAnimationNotifyFunction<CPlayer>("PlayerUpShot_Run",
		2, this, &CPlayer::UpShot);

	AddAnimation("PlayerUpShot_Run_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.43f);
	AddAnimationFrameData("PlayerUpShot_Run_Left", 0, 16, 10);
	SetAnimationEndFunction("PlayerUpShot_Run_Left", this, &CPlayer::UpShotEnd);
	SetAnimationNotifyFunction<CPlayer>("PlayerUpShot_Run_Left",
		2, this, &CPlayer::UpShot);

	// under shot
	AddAnimation("PlayerUnderShot", AnimationType::Sprite, false, "Player_Atlas00", 0.43f);
	AddAnimationFrameData("PlayerUnderShot", 0, 22, 6);
	SetAnimationEndFunction("PlayerUnderShot", this, &CPlayer::DownShotEnd);
	SetAnimationNotifyFunction<CPlayer>("PlayerUnderShot",
		2, this, &CPlayer::DownShot);

	AddAnimation("PlayerUnderShot_Left", AnimationType::Sprite, false, "Player_Atlas00_Left", 0.43f);
	AddAnimationFrameData("PlayerUnderShot_Left", 0, 22, 6);
	SetAnimationEndFunction("PlayerUnderShot_Left", this, &CPlayer::DownShotEnd);
	SetAnimationNotifyFunction<CPlayer>("PlayerUnderShot_Left",
		2, this, &CPlayer::DownShot);


	// TimeStop
	AddAnimation("PlayerTimeStop", AnimationType::Sprite, true, "Player_Atlas00", .3f);
	AddAnimationFrameData("PlayerTimeStop", 8, 11, 4);

	AddAnimation("PlayerTimeStop_Left", AnimationType::Sprite, true, "Player_Atlas00_Left", .3f);
	AddAnimationFrameData("PlayerTimeStop_Left", 8, 11, 4);

	AddAnimation("PlayerTimeStopAir", AnimationType::Sprite, true, "Player_Atlas00", .3f);
	AddAnimationFrameData("PlayerTimeStopAir", 2, 12, 4);

	AddAnimation("PlayerTimeStopAir_Left", AnimationType::Sprite, true, "Player_Atlas00_Left", .3f);
	AddAnimationFrameData("PlayerTimeStopAir_Left", 2, 12, 4);

	// Slow Stand
	AddAnimation("PlayerSlowStand", AnimationType::Sprite, true, "Player_Atlas01", 0.5f);
	AddAnimationFrameData("PlayerSlowStand", 3, 8, 4);

	AddAnimation("PlayerSlowStand_Left", AnimationType::Sprite, true, "Player_Atlas01_Left", 0.5f);
	AddAnimationFrameData("PlayerSlowStand_Left", 3, 8, 4);

	AddAnimation("PlayerSlowAir", AnimationType::Sprite, true, "Player_Atlas01", 0.5f);
	AddAnimationFrameData("PlayerSlowAir", 9, 7, 4);

	AddAnimation("PlayerSlowAir_Left", AnimationType::Sprite, true, "Player_Atlas01_Left", 0.5f);
	AddAnimationFrameData("PlayerSlowAir_Left", 9, 7, 4);

	// mini Down Loop
	AddAnimation("PlayerMiniDownLoop", AnimationType::Sprite, true, "Player_Atlas01", 0.4f);
	AddAnimationFrameData("PlayerMiniDownLoop", 5, 7, 4);

	// mini Down Start
	AddAnimation("PlayerMiniDown", AnimationType::Sprite, false, "Player_Atlas01", 0.6f);
	AddAnimationFrameData("PlayerMiniDown", 7, 6, 8);
	SetNextAnimation("PlayerMiniDown", "PlayerMiniDownLoop");

	AddAnimation("PlayerDie", AnimationType::Sprite, false, "Player_Atlas01", 1.5f);
	AddAnimationFrameData("PlayerDie", 7, 0, 15);
	SetAnimationEndFunction("PlayerDie", this, &CPlayer::DieEnd);

	AddAnimation("PlayerDie_Left", AnimationType::Sprite, false, "Player_Atlas01_Left", 1.5f);
	AddAnimationFrameData("PlayerDie_Left", 7, 0, 15);
	SetAnimationEndFunction("PlayerDie_Left", this, &CPlayer::DieEnd);
	

	m_pPlayerWidget = m_pScene->CreateWidget<CPlayerWidget>("PlayerWidget");
	m_pPlayerWidget->SetOwner(this);
	m_pPlayerWidget->SetLevel(1);
	if (!s_bCanTimeStopSkill)
		EnableTimeStopSkill(false);

	m_pSnailWorldWidget = CreateWorldWidget<CUIPlayerWidgetWorld>();
	m_pSnailWorldWidget->SetOffset(0.f, -180.f);
	//for (int i = 0; i < 6; ++i)
	//{
	//	AddAnimationFrameData("IdleRight",
	//		Vector2(i * 82.f, 0.f), Vector2(82.f, 73.f));
	//}

	//AddAnimation("WalkRight", AnimationType::Sprite, true,
	//	"PlayerWalkRight", TEXT("Player/Right/awalk.bmp"), 0.5f);

	//m_pScene->GetResourceManager()->SetColorKey("PlayerWalkRight",
	//	255, 0, 255);

	return true;
}

void CPlayer::Start()
{
	CCharacter::Start();

	if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Move)
	{
		m_pPlayerWidget->SetTimeState(TimeMode::Move);

	}
	else if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Stop)
	{
		m_pPlayerWidget->SetTimeState(TimeMode::Stop);
		m_bIsTimeStop = true;
	}
	else
	{
		m_pPlayerWidget->SetTimeState(TimeMode::Snail);
		m_bIsSlowTime = true;
	}

	if (GET_TIMER->FindTimer("SlowTime") != nullptr)
	{
		GET_TIMER->FindTimer("SlowTime")->Function = std::bind(&CPlayer::SlowTimeEnd, this);
	}

	if (GET_TIMER->FindTimer("Invincible") != nullptr)
	{
		GET_TIMER->FindTimer("Invincible")->Function = std::bind(&CPlayer::ReleaseInvincible, this);
	}
}

// 입력 등록은 무조건 이 함수에서만 한다.
void CPlayer::SetInput()
{
	CCharacter::SetInput();

	//CInput::GetInst()->SetKeyCallback<CPlayer>("MoveUp", EKeyState::Push,
	//	this, &CPlayer::MoveUp);
	//CInput::GetInst()->SetKeyCallback<CPlayer>("MoveDown", EKeyState::Push,
	//	this, &CPlayer::MoveDown);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveLeft", EKeyState::Up,
		this, &CPlayer::InitMoveDirX);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveRight", EKeyState::Up,
		this, &CPlayer::InitMoveDirX);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveLeft", EKeyState::Press,
		this, &CPlayer::MoveLeft);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveRight", EKeyState::Press,
		this, &CPlayer::MoveRight);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveUp", EKeyState::Press,
		this, &CPlayer::UpKey);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveUp", EKeyState::Up,
		this, &CPlayer::ReleaseUpDownKey);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveDown", EKeyState::Press,
		this, &CPlayer::DownKey);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveDown", EKeyState::Up,
		this, &CPlayer::ReleaseUpDownKey);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveUp", EKeyState::Press,
		this, &CPlayer::UpKey);
	CInput::GetInst()->SetKeyCallback<CPlayer>("Action", EKeyState::Down,
		this, &CPlayer::InteractionKeyInput);
	CInput::GetInst()->SetKeyCallback<CPlayer>("TimeStop", EKeyState::Down,
		this, &CPlayer::TimeStopInput);
	CInput::GetInst()->SetKeyCallback<CPlayer>("Action", EKeyState::Press,
		this, &CPlayer::SlowTimeInput);
	CInput::GetInst()->SetKeyCallback<CPlayer>("Action", EKeyState::Up,
		this, &CPlayer::SlowTimeKeyUp);
	CInput::GetInst()->SetKeyCallback<CPlayer>("Jump", EKeyState::Down,
		this, &CPlayer::Jump);
	CInput::GetInst()->SetKeyCallback<CPlayer>("Jump", EKeyState::Up,
		this, &CPlayer::GlidingEnd);
}

void CPlayer::Update(float DeltaTime)
{

	if (m_bIsPlayDamageAnim)
	{
		Move(Vector2(DeltaTime * 200.f * (m_DamageView == CharacterView::Right ? -1 : 1), 0.f));

		m_curBlinkTime -= DeltaTime;
		if (m_curBlinkTime <= 0.f)
		{
			m_curBlinkTime = m_BlinkTime;
			if (m_bIsRender)
				m_bIsRender = false;
			else
				m_bIsRender = true;
		}

		//auto iter = RectComponent->GetCollisionLinst().begin();
		//auto iterEnd = RectComponent->GetCollisionLinst().end();

		//for (; iter != iterEnd; iter++)
		//{
		//	DamageableInterface* pDamageAble = Cast<DamageableInterface*>(*iter);
		//	if (pDamageAble)
		//	{
		//		TakeDamage(pDamageAble->GetAD(), (CGameObject*)pDamageAble);
		//	}
		//}
	}

	CCharacter::Update(DeltaTime);

	SetMP(s_MP + (DeltaTime * m_MPIncreasePerSec));

	if (m_vMove.x == 0.f && m_IsGround)
	{
		m_bIsMove = false;
		m_vMoveDir.x = 0;

		if (m_bAutoIdle)
		{
			if (m_PlayerAnim == PlayerAnimation::Walk || m_PlayerAnim == PlayerAnimation::Attack || m_PlayerAnim == PlayerAnimation::Gliding || m_PlayerAnim == PlayerAnimation::Jump 
				|| m_PlayerAnim == PlayerAnimation::FallDown || m_PlayerAnim == PlayerAnimation::None)
			{
				if (m_View == CharacterView::Right)
					ChangeAnimation("PlayerRunStop");
				else
					ChangeAnimation("PlayerRunStop_Left");
			}

			m_PlayerAnim = PlayerAnimation::Idle;

			if (m_bIsAction)
				ActionEnd();
		}
	}

	if (!m_Jump && m_bIsJump && !m_IsFalling)
	{
		JumpEnd();
		m_bUseAccel = true;
		m_bIsGliding = false;
	}


	// Tiem stop 게이지 상승 조건
	if (!m_bIsTimeStop)
	{
		SetTimeStopGauge(s_TimeStopGauge + m_TimeStopGaugeIncreaseSpeed * DeltaTime * m_pScene->GetSceneMode()->GetWorldTimeSpeed());
	}
	else
	{
		SetTimeStopGauge(s_TimeStopGauge - m_TimeStopGaugeIncreaseSpeed * DeltaTime * m_pScene->GetSceneMode()->GetWorldTimeSpeed());
	}

	if (m_IsFalling && m_PlayerAnim != PlayerAnimation::TimeStop && m_PlayerAnim != PlayerAnimation::SlowTime)
	{
		if (m_bIsGliding)
		{
			if (!m_bIsAction && !m_bIsPlayDamageAnim)
			{
				if (m_View == CharacterView::Right)
				{
					if (m_pAnimation->GetCurrentAnimation() != "PlayerGlidingLoop")
						ChangeAnimation("PlayerGlidingStart");
					else if (m_pAnimation->GetCurrentAnimation() == "PlayerGlidingLoop_Left")
						ChangeAnimation("PlayerGlidingLoop");
				}
				else if (m_View == CharacterView::Left)
				{
					if (m_pAnimation->GetCurrentAnimation() != "PlayerGlidingLoop_Left")
						ChangeAnimation("PlayerGlidingStart_Left");
					else if (m_pAnimation->GetCurrentAnimation() == "PlayerGlidingLoop")
						ChangeAnimation("PlayerGlidingLoop_Left");
				}

				m_PlayerAnim = PlayerAnimation::Gliding;
			}
			
			m_bUseAccel = false;
			m_Velocity = 150.f;
		}
		else
		{
			if (!m_bIsAction && !m_bIsPlayDamageAnim)
			{
				if (m_View == CharacterView::Right)
				{
					if (m_pAnimation->GetCurrentAnimation() != "PlayerJumpLoop")
						ChangeAnimation("PlayerFalling");
					else if (m_pAnimation->GetCurrentAnimation() == "PlayerJumpLoop_Left")
						ChangeAnimation("PlayerJumpLoop");
				}
				else
				{
					if (m_pAnimation->GetCurrentAnimation() != "PlayerJumpLoop_Left")
						ChangeAnimation("PlayerFalling_Left");
					else if (m_pAnimation->GetCurrentAnimation() == "PlayerJumpLoop")
						ChangeAnimation("PlayerJumpLoop_Left");
				}

				m_PlayerAnim = PlayerAnimation::Jump;
			}


			if (!m_bUseAccel)
			{
				m_Jump = false;
				m_Velocity = 0.f;
				m_FallStartY = m_vPos.y;
				m_FallTime = 0.f;
			}

			m_bUseAccel = true;
		}
	}
	else if (m_Jump && m_PlayerAnim != PlayerAnimation::TimeStop && m_PlayerAnim != PlayerAnimation::SlowTime)
	{
		if (!m_bIsAction && !m_bIsPlayDamageAnim)
		{
			if (m_View == CharacterView::Right)
			{
				ChangeAnimation("PlayerJumpStart");
			}
			else if (m_View == CharacterView::Left)
			{
				ChangeAnimation("PlayerJumpStart_Left");
			}
		}
	}

	if (m_bIsSuperAttack)
	{
		m_fCurrentSuperAttackTime -= DeltaTime;

		if (m_fCurrentSuperAttackTime <= 0.f)
		{
			m_pScene->GetResourceManager()->SoundPlay("whip");
			m_fCurrentSuperAttackTime = m_fSuperAttackTime;
		}
	}

}

void CPlayer::Update(float DeltaTime, TimeMode eMode)
{
	Update(DeltaTime);
}

void CPlayer::Render(HDC hDC, float DeltaTime)
{
#ifdef _DEBUG
	char Player[MAX_PATH] = "Player : ";
	sprintf_s(Player, "Player : %d, %d", (int)m_vPos.x, (int)m_vPos.y);
	TextOutA(hDC, 20, 400, Player, (int)strlen(Player));

	char Move[MAX_PATH] = "Move : ";
	sprintf_s(Move, "Move : %d, %d", (int)m_vMove.x, (int)m_vMove.y);
	TextOutA(hDC, 20, 420, Move, (int)strlen(Move));

	char IsGround[MAX_PATH] = "IsGround : ";
	sprintf_s(IsGround, "IsGround : %d", (int)(m_IsGround));
	TextOutA(hDC, 20, 440, IsGround, (int)strlen(IsGround));

	char IsJump[MAX_PATH] = "IsJump : ";
	sprintf_s(IsJump, "IsJump : %d", (int)(m_Jump));
	TextOutA(hDC, 20, 460, IsJump, (int)strlen(IsJump));

	char TileBlock[MAX_PATH] = "TileBlock : ";
	sprintf_s(TileBlock, "TileBlock : %d", m_bTileBlock);
	TextOutA(hDC, 20, 480, TileBlock, (int)strlen(TileBlock));

	char Raycast[MAX_PATH] = "Raycast : ";
	sprintf_s(Raycast, "Raycast : %d, %d", (int)GetHitResult().hitPoint.x, (int)(GetHitResult().hitPoint.y));
	TextOutA(hDC, 20, 500, Raycast, (int)strlen(Raycast));

	char MP[MAX_PATH] = "MP : ";
	sprintf_s(MP, "MP : %f", s_MP);
	TextOutA(hDC, 20, 520, MP, (int)strlen(MP));

	char Invincible[MAX_PATH] = "Invincible : ";
	sprintf_s(Invincible, "Invincible : %d", m_bIsInvincible);
	TextOutA(hDC, 20, 540, Invincible, (int)strlen(Invincible));

	char IsFalling[MAX_PATH] = "IsFalling : ";
	sprintf_s(IsFalling, "IsFalling : %d", m_IsFalling);
	TextOutA(hDC, 20, 560, IsFalling, (int)strlen(IsFalling));

	char Velocity[MAX_PATH] = "Velocity : ";
	sprintf_s(Velocity, "Velocity : %f", m_Velocity);
	TextOutA(hDC, 20, 580, Velocity, (int)strlen(Velocity));

	char PhysicsSimulate[MAX_PATH] = "PhysicsSimulate : ";
	sprintf_s(PhysicsSimulate, "Pysics : %d", m_PhysicsSimulate);
	TextOutA(hDC, 20, 600, PhysicsSimulate, (int)strlen(PhysicsSimulate));

	char FallTime[MAX_PATH] = "FallTime : ";
	sprintf_s(FallTime, "FallTime : %f", m_FallTime);
	TextOutA(hDC, 20, 620, FallTime, (int)strlen(FallTime));

	char UseAccel[MAX_PATH] = "UseAccel : ";
	sprintf_s(UseAccel, "UseAccel : %d", m_bUseAccel);
	TextOutA(hDC, 20, 640, UseAccel, (int)strlen(UseAccel));
#endif

	CCharacter::Render(hDC, DeltaTime);
}

CPlayer* CPlayer::Clone() const
{
	return new CPlayer(*this);
}

void CPlayer::SetMaxStat()
{
	m_pSnailWorldWidget->ShowFullStat();
	SetHP(m_HPMax);
	CCharacter::SetMP(m_MPMax);
	s_MP = m_MP;
}

void CPlayer::SetHP(float HP)
{
	CCharacter::SetHP(HP);
	s_HP = m_HP;
}

void CPlayer::SetMP(float MP)
{
	if (s_MP < m_MPMax && MP >= m_MPMax)
		m_pSnailWorldWidget->ShowFullMP();

	CCharacter::SetMP(MP);
	s_MP = m_MP;
}

void CPlayer::DieAnim()
{
	m_PlayerAnim = PlayerAnimation::FallDown;

	if (m_DamageView == CharacterView::Right)
		ChangeAnimation("PlayerDie");
	else
		ChangeAnimation("PlayerDie_Left");
}

void CPlayer::Die()
{
	//m_pPlayerWidget->SetWidgetEnable(false);
	//GET_TIMER->RemoveTimer("Invincible");
	//ReleaseInvincible();
	//SetEnable(false, false, false);
	//DieAnim();
}

void CPlayer::DieEnd()
{
}

void CPlayer::Graze()
{

}

float CPlayer::TakeDamage(float Damage, CGameObject* DamageObject)
{
	if (m_bIsInvincible) return s_HP;

	ActionEnd();

	// SlowTime 차징 중이었다면 취소
	m_SlowTimeGauge = 0.f;
	SlowTimeKeyUp(GET_TIMER->GetDeltaTime());
	
	if (GET_TIMER->FindTimer("Invincible"))
		GET_TIMER->RemoveTimer("Invincible");

	if (m_bCanInvincible)
	{
		Invincible();
		GET_TIMER->StartTimer<CPlayer>("Invincible", 1.f, this, &CPlayer::ReleaseInvincible);
	}

	CCharacter::TakeDamage(Damage, DamageObject);

	SetEnable(false, false, false);
	
	m_Jump = false;
	m_JumpVelocity = 20.f;
	CMoveableObject::Jump();

	CSharedPtr<CPlayerEffect> DamageEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Damage_Effect", m_vPos, Vector2(512.f, 512.f));
	DamageEffect->SetOwner(this);
	DamageEffect->SetPivot(0.5f, 0.6f);
	DamageEffect->SetUseLocalSpace(true);
	DamageEffect->AddAnimation("Player_Damage_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", .5f);
	DamageEffect->AddAnimationFrameData("Player_Damage_Effect_Anim", 7, 4, 8, Vector2(512.f, 512.f));
	DamageEffect->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("down");

	m_bAutoIdle = false;
	m_PlayerAnim = PlayerAnimation::FallDown;
	m_bIsPlayDamageAnim = true;

	if (m_View == CharacterView::Right)
	{
		ChangeAnimation("PlayerFallDown");
		m_DamageView = CharacterView::Right;
	}
	else
	{
		m_DamageView = CharacterView::Left;
		ChangeAnimation("PlayerFallDown_Left");
	}
	// 데미지 애니 처리

	return s_HP;
}

void CPlayer::Invincible()
{
	m_bIsInvincible = true;

	if (RectComponent->GetCollisionProfile()->vecState.size() > 0)
	RectComponent->GetCollisionProfile()->vecState[(int)CollisionChannel::Monster] = CollisionState::Ignore;
}

void CPlayer::ReleaseInvincible()
{
	m_bIsInvincible = false;

	if (RectComponent->GetCollisionProfile()->vecState.size() > 0)
		RectComponent->GetCollisionProfile()->vecState[(int)CollisionChannel::Monster] = CollisionState::Collision;
}

void CPlayer::InteractionKeyInput(float DeltaTime)
{
	if (m_IsInteraction)
	{
		if (m_IsDialogue)
		{
			std::string strText = GET_SINGLE(CTextManager)->GetText();
			if (strText == "#end")
				ReleaseDialogue();
			else if (m_pPlayerWidget != nullptr)
			{
				m_pPlayerWidget->SetText(strText);
				m_pScene->GetResourceManager()->SoundPlay("message");
			}
			return;
		}
		else
		{
			m_IsInteraction = false;
			m_pPlayerWidget->SetEnableItemWidget(false); 
			m_pPlayerWidget->SetEnableTimeStopWidget(true);
			SetEnable();
			m_pScene->GetSceneMode()->ReleaseAllStop();
		}
	}
	else
		ActionInput(DeltaTime);
}

void CPlayer::MoveLeft(float DeltaTime)
{
	if (!m_bCanMove || m_bDestroy) return;

	if (m_vMoveDir.x == 1)
		return;

	m_vPos.x -= m_MoveSpeed * DeltaTime;
	m_vMove.x -= m_MoveSpeed * DeltaTime;

	if (m_PlayerAnim != PlayerAnimation::Walk && !m_bIsAction && !m_bIsJump && !m_IsFalling)
	{
		if (m_pAnimation->GetCurrentAnimation() == "PlayerRun" ||
			m_pAnimation->GetCurrentAnimation() == "PlayerRunStart" ||
			m_pAnimation->GetCurrentAnimation() == "PlayerRunStop" ||
			(m_PlayerAnim == PlayerAnimation::Attack && m_ActionView == CharacterView::Right))
			ChangeAnimation("PlayerRunBack_Left");

		else
			ChangeAnimation("PlayerRunStart_Left");

		CSharedPtr<CPlayerEffect> SmokeEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Smoke_Effect", m_vPos, Vector2(512.f, 512.f));
		SmokeEffect->SetOwner(this);
		SmokeEffect->SetUseLocalSpace(false);
		SmokeEffect->AddAnimation("Player_Smoke_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.2f);
		SmokeEffect->AddAnimationFrameData("Player_Smoke_Effect_Anim", 1, 12, 8, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
		SmokeEffect->SetAutoDestroy();

		m_bAutoIdle = true;
		m_PlayerAnim = PlayerAnimation::Walk;
	}
	m_bIsMove = true;
	m_vMoveDir.x = -1;
	
	m_View = CharacterView::Left;
}

void CPlayer::MoveRight(float DeltaTime)
{
	if (!m_bCanMove || m_bDestroy) return;
	
	if (m_vMoveDir.x == -1)
		return;

	m_vPos.x += m_MoveSpeed * DeltaTime;
	m_vMove.x += m_MoveSpeed * DeltaTime;

	if (m_PlayerAnim != PlayerAnimation::Walk && !m_bIsAction && !m_bIsJump && !m_IsFalling)
	{
		if (m_pAnimation->GetCurrentAnimation() == "PlayerRun_Left" ||
			m_pAnimation->GetCurrentAnimation() == "PlayerRunStart_Left" ||
			m_pAnimation->GetCurrentAnimation() == "PlayerRunStop_Left" ||
			(m_PlayerAnim == PlayerAnimation::Attack && m_ActionView == CharacterView::Left))

			ChangeAnimation("PlayerRunBack");

		else
			ChangeAnimation("PlayerRunStart");

		CSharedPtr<CPlayerEffect> SmokeEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Smoke_Effect", m_vPos, Vector2(512.f, 512.f));
		SmokeEffect->SetOwner(this);
		SmokeEffect->SetUseLocalSpace(false);
		SmokeEffect->AddAnimation("Player_Smoke_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.2f);
		SmokeEffect->AddAnimationFrameData("Player_Smoke_Effect_Anim", 1, 12, 8, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
		SmokeEffect->SetAutoDestroy();

		m_bAutoIdle = true;
		m_PlayerAnim = PlayerAnimation::Walk;
	}
	
	m_bIsMove = true;
	m_vMoveDir.x = 1;
	
	m_View = CharacterView::Right;
}

void CPlayer::InitMoveDirX(float DeltaTime)
{
	if (!m_bCanMove) return; 

	m_vMoveDir.x = 0.f;
	m_PlayerAnim = PlayerAnimation::None;
}

void CPlayer::UpKey(float DeltaTime)
{
	if (m_bDestroy) return;

	m_UpDownKey = 1;

	if (m_pSnailWorldWidget->IsShowUpVision())
	{
		GET_SINGLE(CTextManager)->SetEnableAllStop(true);
		Dialogue();
	}
}

void CPlayer::DownKey(float DeltaTime)
{
	if (m_bDestroy) return;

	m_UpDownKey = -1;
}

void CPlayer::ReleaseUpDownKey(float DeltaTime)
{
	if (m_bDestroy) return;

	m_UpDownKey = 0;
}

void CPlayer::Jump(float DeltaTime)
{
	if (!m_bCanJump || m_bDestroy) return;

	if (m_bIsJump)
	{
		m_bIsGliding = true;
		return;
	}

	m_bIsJump = true;
	m_bIsGliding = true;
	m_bAutoIdle = false;
	ActionEnd();

	CMoveableObject::Jump();

	m_PlayerAnim = PlayerAnimation::Jump;

	if (m_View == CharacterView::Right)
		ChangeAnimation("PlayerJumpStart");
	else
		ChangeAnimation("PlayerJumpStart_Left");
}

void CPlayer::JumpEnd()
{	
	m_bIsJump = false;

	CSharedPtr<CPlayerEffect> SmokeEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Smoke_Effect", m_vPos, Vector2(512.f, 512.f));
	SmokeEffect->SetOwner(this);
	SmokeEffect->SetUseLocalSpace(false);
	SmokeEffect->AddAnimation("Player_Smoke_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.2f);
	SmokeEffect->AddAnimationFrameData("Player_Smoke_Effect_Anim", 1, 12, 8, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
	SmokeEffect->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("landing");

	if (!m_bIsPlayDamageAnim)
		m_bAutoIdle = true;
}

void CPlayer::GlidingEnd(float DeltaTime)
{
	m_bIsGliding = false;
}

void CPlayer::ActionInput(float DeltaTime)
{
	if (m_bDestroy) return;

	if (!m_bCanAttack || m_bIsTimeStop && m_TimeStopKnifeNum <= 0) return;

	if (m_UpDownKey == 1)
	{
		UpShotInput(DeltaTime);
		return;
	}
	else if (m_UpDownKey == -1 && m_bIsJump)
	{
		DownShotInput(DeltaTime);
		return;
	}

	if (s_MP - m_MPCost_Action < 0)
	{
		m_pSnailWorldWidget->ShowNoMP();
		return;
	}

	// 첫 콤보 입력 시
	if (!m_bIsAction)
	{
		m_PlayerAnim = PlayerAnimation::Attack;
		m_bIsAction = true;
		m_CurCombo = 1;
		m_bAutoIdle = false;
		m_vMoveDir = Vector2(0.f, 0.f);


		if (m_View == CharacterView::Left)
		{
			if (m_bIsGliding)
				ChangeAnimation("PlayerSuperAttackAir_Left");
			else if (m_bIsMove)
				ChangeAnimation("PlayerAction1_Run_Left");
			else
				ChangeAnimation("PlayerAction1_Left");

			m_ActionView = CharacterView::Left;
		}
		else
		{
			if (m_bIsGliding)
				ChangeAnimation("PlayerSuperAttackAir");
			else if (m_bIsMove)
				ChangeAnimation("PlayerAction1_Run");
			else
				ChangeAnimation("PlayerAction1");

			m_ActionView = CharacterView::Right;
		}
	}
	// 콤보 중...
	else
	{
		// 현재 콤보 카운트에서 아직 다음 콤보 입력이 들어오지 않았다면
		if (!m_bNextComboInput)
		{
			m_bNextComboInput = true;
		}
	}

	if (m_bIsTimeStop)  
	{
		SetTimeStopGauge(s_TimeStopGauge - 10.f);
	}
	
}

void CPlayer::ActionComboCheck()
{
	if (m_bNextComboInput && m_ActionView == m_View)
	{
		m_CurCombo != m_ComboMax ? m_CurCombo++ : m_CurCombo = 1;
		switch (m_CurCombo)
		{
		case 1:
			if (m_bIsGliding)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerSuperAttackAir_Left");
				else
					ChangeAnimation("PlayerSuperAttackAir");
			}
			else if (m_bIsMove)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerAction1_Run_Left");
				else
					ChangeAnimation("PlayerAction1_Run");
			}
			else
			{
				if (m_bIsSuperAttack)
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerSuperAttack_Left");
					else
						ChangeAnimation("PlayerSuperAttack");
				}
				else
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerAction1_Left");
					else
						ChangeAnimation("PlayerAction1");
				}
			}
			break;
		case 2:
			if (m_bIsGliding)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerSuperAttack2Air_Left");
				else
					ChangeAnimation("PlayerSuperAttack2Air");
			}
			else if (m_bIsMove)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerAction2_Run_Left");
				else
					ChangeAnimation("PlayerAction2_Run");
			}
			else
			{
				m_bIsSuperAttack = true;

				if (m_bIsSuperAttack)
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerSuperAttack2_Left");
					else
						ChangeAnimation("PlayerSuperAttack2");
				}
				else
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerAction2_Left");
					else
						ChangeAnimation("PlayerAction2");
				}
			}
			break;
		case 3:
			if (m_bIsGliding)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerSuperAttack3Air_Left");
				else
					ChangeAnimation("PlayerSuperAttack3Air");
			}
			else if (m_bIsMove)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerAction3_Run_Left");
				else
					ChangeAnimation("PlayerAction3_Run");
			}
			else
			{
				if (m_bIsSuperAttack)
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerSuperAttack3_Left");
					else
						ChangeAnimation("PlayerSuperAttack3");
				}
				else
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerAction3_Left");
					else
						ChangeAnimation("PlayerAction3");
				}
			}
			break;
		case 4:
			if (m_bIsGliding)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerSuperAttack4Air_Left");
				else
					ChangeAnimation("PlayerSuperAttack4Air");
			}
			else if (m_bIsMove)
			{
				if (m_View == CharacterView::Left)
					ChangeAnimation("PlayerAction4_Run_Left");
				else
					ChangeAnimation("PlayerAction4_Run");
			}
			else
			{
				if (m_bIsSuperAttack)
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerSuperAttack4_Left");
					else
						ChangeAnimation("PlayerSuperAttack4");
				}
				else
				{
					if (m_View == CharacterView::Left)
						ChangeAnimation("PlayerAction4_Left");
					else
						ChangeAnimation("PlayerAction4");
				}
			}
			break;
		}

		m_bNextComboInput = false;
	}
	else
	{
		ActionEnd();
	}
}

void CPlayer::Action()
{
	Vector2 vPos;

	CSharedPtr<CPlayerEffect> ActionEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Action_Effect", m_vPos, Vector2(512.f, 512.f));
	ActionEffect->SetOwner(this);
	ActionEffect->SetUseLocalSpace(true);
	ActionEffect->AddAnimation("Player_Action_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.2f);
	ActionEffect->AddAnimationFrameData("Player_Action_Effect_Anim", 2, 6, 8, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
	ActionEffect->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("kengeki00");

	SetMP(s_MP - m_MPCost_Action);

	if (m_bIsTimeStop)
	{
		m_pSnailWorldWidget->SetKnifeNum(m_TimeStopKnifeNum -= 3);
		m_pPlayerWidget->PlayTimeGaugeEffect();
	}

	if (m_ActionView == CharacterView::Right)
	{
		ActionEffect->SetOffset(70.f, -70.f);


		vPos.x = m_vPos.x + m_vSize.x * (1.f - m_vPivot.x) + 20.f;
		vPos.y = m_vPos.y + m_vSize.y * (0.5f - m_vPivot.y);
		CSharedPtr<CBullet>	Bullet = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet->SetView();

		vPos -= 20.f;
		CSharedPtr<CBullet>	Bullet2 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet2->SetView();

		vPos.x -= 20.f;
		vPos.y += 40.f;
		CSharedPtr<CBullet>	Bullet3 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet3->SetView();
	}

	else
	{
		ActionEffect->SetOffset(-70.f, -70.f);

		vPos.x = m_vPos.x - m_vSize.x * m_vPivot.x - 20.f;
		vPos.y = m_vPos.y + m_vSize.y * (0.5f - m_vPivot.y);
		CSharedPtr<CBullet>	Bullet = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet->SetView(BulletView::Left);

		vPos.x += 20.f;
		vPos.y -= 20.f;
		CSharedPtr<CBullet>	Bullet2 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet2->SetView(BulletView::Left);

		vPos.x += 20.f;
		vPos.y += 40.f;
		CSharedPtr<CBullet>	Bullet3 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet3->SetView(BulletView::Left);
	}		
}

void CPlayer::ActionEnd()
{
	//if (m_View == CharacterView::Left)
	//	ChangeAnimation("PlayerStop_Left");
	//else
	//	ChangeAnimation("PlayerStop");

	m_CurCombo = 0;
	m_bIsSuperAttack = false;
	m_bIsAction = false;

	m_bAutoIdle = true;
}

void CPlayer::UpShotInput(float DeltaTime)
{
	if (m_bDestroy) return;

	m_PlayerAnim = PlayerAnimation::Attack;
	m_bIsAction = true;
	m_bAutoIdle = false;
	m_vMoveDir = Vector2(0.f, 0.f);


	if (m_View == CharacterView::Left)
	{
		if (m_bIsMove)
			ChangeAnimation("PlayerUpShot_Run_Left");
		else
			ChangeAnimation("PlayerUpShot_Left");

		m_UpShotView = CharacterView::Left;
	}
	else
	{
		if (m_bIsMove)
			ChangeAnimation("PlayerUpShot_Run");
		else
			ChangeAnimation("PlayerUpShot");

		m_UpShotView = CharacterView::Right;
	}

	if (m_bIsJump)
	{
		m_Jump = false;
		m_FallStartY = m_vPos.y;
		m_FallTime = 0.f;
	}
}

void CPlayer::UpShot()
{
	Vector2 vPos;

	CSharedPtr<CPlayerEffect> ActionEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Action_Effect", m_vPos, Vector2(512.f, 512.f));
	ActionEffect->SetOwner(this);
	ActionEffect->SetUseLocalSpace(true);
	ActionEffect->AddAnimation("Player_Action_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.2f);
	ActionEffect->AddAnimationFrameData("Player_Action_Effect_Anim", 0, 13, 7, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f)); 
	ActionEffect->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("kengeki00");

	SetMP(s_MP - m_MPCost_Action);

	if (m_bIsTimeStop)
	{
		m_pSnailWorldWidget->SetKnifeNum(m_TimeStopKnifeNum -= 3);
		m_pPlayerWidget->PlayTimeGaugeEffect();
	}

	if (m_UpShotView == CharacterView::Right)
	{
		ActionEffect->SetOffset(0.f, -m_vSize.y - 20.f);

		vPos.y = m_vPos.y + m_vSize.y * (0.5f - m_vPivot.y) - 60.f;
		vPos.x = m_vPos.x + m_vSize.x * (1.f - m_vPivot.x) - 60.f;
		CSharedPtr<CBullet>	Bullet = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet->SetView(BulletView::RightUp);

		vPos.x += 20.f;
		vPos.y -= 20.f;
		CSharedPtr<CBullet>	Bullet2 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet2->SetView(BulletView::RightUp);

		vPos.y += 20.f;
		vPos.x += 20.f;
		CSharedPtr<CBullet>	Bullet3 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet3->SetView(BulletView::RightUp);
	}

	else
	{
		ActionEffect->SetOffset(0.f, -m_vSize.y - 20.f);

		vPos.y = m_vPos.y + m_vSize.y * (0.5f - m_vPivot.y) - 60.f;
		vPos.x = m_vPos.x + m_vSize.x * (1.f - m_vPivot.x) - 60.f;
		CSharedPtr<CBullet>	Bullet = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet->SetView(BulletView::LeftUp);

		vPos.x += 20.f;
		vPos.y -= 20.f;
		CSharedPtr<CBullet>	Bullet2 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet2->SetView(BulletView::LeftUp);

		vPos.y += 20.f;
		vPos.x += 20.f;
		CSharedPtr<CBullet>	Bullet3 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet3->SetView(BulletView::LeftUp);
	}
}

void CPlayer::UpShotEnd()
{
	m_bIsAction = false;
	m_bAutoIdle = true;
}

void CPlayer::DownShotInput(float DeltaTime)
{
	if (m_bDestroy) return;

	m_PlayerAnim = PlayerAnimation::Attack;
	m_bIsAction = true;
	m_bAutoIdle = false;
	// m_vMoveDir = Vector2(0.f, 0.f);


	if (m_View == CharacterView::Left)
	{
		ChangeAnimation("PlayerUnderShot_Left");

		m_DownShotView = CharacterView::Left;
	}
	else
	{
		ChangeAnimation("PlayerUnderShot");

		m_DownShotView = CharacterView::Right;
	}

	m_PhysicsSimulate = true;
	m_Jump = false;
	m_Velocity = 0.f;
	m_FallTime = -1.f;
	m_FallStartY = m_vPos.y;
}

void CPlayer::DownShot()
{
	Vector2 vPos;

	SetMP(s_MP - m_MPCost_Action);

	if (m_bIsTimeStop)
	{
		m_pSnailWorldWidget->SetKnifeNum(m_TimeStopKnifeNum -= 3);
		m_pPlayerWidget->PlayTimeGaugeEffect();
	}

	m_pScene->GetResourceManager()->SoundPlay("kengeki00");

	if (m_DownShotView == CharacterView::Right)
	{
		CSharedPtr<CPlayerEffect> ActionEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Action_Effect", m_vPos, Vector2(512.f, 512.f));
		ActionEffect->SetOwner(this);
		ActionEffect->SetUseLocalSpace(true);
		ActionEffect->AddAnimation("Player_Action_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.2f);
		ActionEffect->AddAnimationFrameData("Player_Action_Effect_Anim", 0, 14, 7, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
		ActionEffect->SetAutoDestroy();
		ActionEffect->SetOffset(30.f, 0.f);

		vPos.x = m_vPos.x + m_vSize.x * (1.f - m_vPivot.x) + 55.f;
		vPos.y = m_vPos.y + m_vSize.y * (0.5f - m_vPivot.y) + 20.f;
		CSharedPtr<CBullet>	Bullet = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet->SetView(BulletView::RightDown);

		vPos.y += 55.f;
		CSharedPtr<CBullet>	Bullet2 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet2->SetView(BulletView::RightDown);

		vPos.x -= 55.f;
		CSharedPtr<CBullet>	Bullet3 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet3->SetView(BulletView::RightDown);
	}

	else
	{
		CSharedPtr<CPlayerEffect> ActionEffect = m_pScene->CreateGameObject<CPlayerEffect>("Player_Action_Effect", m_vPos, Vector2(512.f, 512.f));
		ActionEffect->SetOwner(this);
		ActionEffect->SetUseLocalSpace(true);
		ActionEffect->AddAnimation("Player_Action_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.2f);
		ActionEffect->AddAnimationFrameData("Player_Action_Effect_Anim", 7, 14, 7, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
		ActionEffect->SetAutoDestroy();
		ActionEffect->SetOffset(-30.f, 0.f);

		vPos.x = m_vPos.x - m_vSize.x * m_vPivot.x - 55.f;
		vPos.y = m_vPos.y + m_vSize.y * (0.5f - m_vPivot.y) + 20.f;
		CSharedPtr<CBullet>	Bullet = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet->SetView(BulletView::LeftDown);

		vPos.y += 55.f;
		CSharedPtr<CBullet>	Bullet2 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet2->SetView(BulletView::LeftDown);

		vPos.x += 55.f;
		CSharedPtr<CBullet>	Bullet3 = m_pScene->CreateGameObject<CBullet>("Bullet", vPos);
		Bullet3->SetView(BulletView::LeftDown);
	}
}

void CPlayer::DownShotEnd()
{
	m_bIsAction = false;
	m_bAutoIdle = true;
	m_IsFalling = true;
	if (m_bIsGliding)
		m_PhysicsSimulate = true;
}

void CPlayer::EnableTimeStopSkill(bool Value)
{
	s_bCanTimeStopSkill = Value;

	if (Value)
		m_pPlayerWidget->SetEnableTimeStopWidget(true);
	else
		m_pPlayerWidget->SetEnableTimeStopWidget(false);
}

void CPlayer::SetTimeMode(TimeMode eMode)
{
}

void CPlayer::TimeStopInput(float DeltaTime)
{
	if (m_bDestroy) return;

	if (! s_bCanTimeStopSkill || !m_bCanTimeStop || m_PlayerAnim == PlayerAnimation::TimeStop) return;

	if (!m_bIsTimeStop)
	{
		ActionEnd();
		SlowTimeKeyUp(0.f);
		// 현재 Snail 모드라면 Snail -> Time Stop 모드로 변경
		if (m_bIsSlowTime)
		{
			GET_TIMER->RemoveTimer("SlowTime");
			SlowTimeEnd();
		}

		m_bIsTimeStop = true;
		m_pPlayerWidget->SetTimeState(TimeMode::Stop);
		m_pScene->GetSceneMode()->SetTimeMode(TimeMode::Stop);
		m_pPlayerWidget->PlaySecondeHandSound();
		m_pSnailWorldWidget->SetKnifeNum(m_TimeStopKnifeNum = m_TimeStopKnifeNumMax);
		m_PlayerAnim = PlayerAnimation::TimeStop;
		m_bAutoIdle = false;
		m_vMoveDir = Vector2(0.f, 0.f);
		SetEnable(false, false, false);

		GET_TIMER->StartTimer<CPlayer>("TimeStop", 1.f, this, &CPlayer::TimeStopAnimEnd);
		Invincible();

		Vector2 EffPos;

		if (m_bIsJump)
		{
			m_PhysicsSimulate = false;

			if (m_View == CharacterView::Left)
			{
				ChangeAnimation("PlayerTimeStopAir_Left");
				EffPos = Vector2(m_vPos.x + 40.f, m_vPos.y - 80.f);
			}
			else
			{
				ChangeAnimation("PlayerTimeStopAir");
				EffPos = Vector2(m_vPos.x - 40.f, m_vPos.y - 80.f);
			}
		}
		else
		{
			if (m_View == CharacterView::Left)
			{
				ChangeAnimation("PlayerTimeStop_Left");
				EffPos = Vector2(m_vPos.x - 20.f, m_vPos.y - 130.f);
			}
			else
			{
				ChangeAnimation("PlayerTimeStop");
				EffPos = Vector2(m_vPos.x + 20.f, m_vPos.y - 130.f);
			}
		}

		CSharedPtr<CPlayerRedRingEffect> pEffect = m_pScene->CreateGameObject<CPlayerRedRingEffect>("Player_TimeStopRedRing_Effect",
			EffPos, Vector2(2000.f, 2000.f), "Stage");

		m_pScene->GetResourceManager()->SoundPlay("timeStop");

	}
	else
	{
		// 타임스탑 모드 종료
		m_bIsTimeStop = false;
		m_pPlayerWidget->SetTimeState(TimeMode::Move);
		m_pScene->GetSceneMode()->SetTimeMode(TimeMode::Move);

		CSharedPtr<CPlayerEffect> TimeStopEndRing = m_pScene->CreateGameObject<CPlayerEffect>("Player_TimeStopRing_Effect", Vector2(m_vPos.x, m_vPos.y - m_vSize.y /2), Vector2(512.f, 512.f));
		TimeStopEndRing->SetOwner(this);
		TimeStopEndRing->SetUseLocalSpace(false);
		TimeStopEndRing->AddAnimation("Player_TimeStopRing_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.3f);
		TimeStopEndRing->AddAnimationFrameData("Player_TimeStopRing_Effect_Anim", 0, 7, 20, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
		TimeStopEndRing->SetAutoDestroy();

		m_pScene->GetResourceManager()->SoundPlay("timeStopEnd");
		m_pSnailWorldWidget->HideKnifeNum();
	}
}

void CPlayer::TimeStopAnimEnd()
{
	m_PlayerAnim = PlayerAnimation::Idle;
	if (m_View == CharacterView::Left)
		ChangeAnimation("PlayerStop_Left");
	else
		ChangeAnimation("PlayerStop");

	m_PhysicsSimulate = true;
	m_bAutoIdle = true;
	m_bIsAction = false;
	SetEnable();

	ReleaseInvincible();
}

void CPlayer::SetTimeStopGauge(float Gauge)
{
	if (s_TimeStopGauge < m_MPMax && Gauge >= m_MPMax)
		m_pSnailWorldWidget->ShowFullTimeGauge();

	s_TimeStopGauge = BasicMath::Clamp<float>(Gauge, 0.f, 100.f);
	m_pPlayerWidget->SetTimeStopGauge(s_TimeStopGauge);

	if (s_TimeStopGauge <= 0 && m_bIsTimeStop)
		TimeStopInput(0.f);
}

void CPlayer::AddTimeStopGauge(float Gauge)
{
	SetTimeStopGauge(s_TimeStopGauge + Gauge);
}

void CPlayer::SlowTimeInput(float DeltaTime)
{
	if (m_bDestroy) return;

	// Time Stop 모드일 땐 Snail 불가능.
	if (!m_bCanSlowTime || m_bIsTimeStop) return;

	m_SlowTimeGauge = BasicMath::Clamp<float>(m_SlowTimeGauge + DeltaTime, 0.f, m_SlowTimeMaxGauge);
	if (m_bIsSlowTime)
		m_SlowTimeGauge = BasicMath::Clamp<float>(m_SlowTimeGauge, 0.f, m_SlowTimeCanInputGauge + ((m_SlowTimeMaxGauge - m_SlowTimeCanInputGauge) / 4) * 3);

	if (m_SlowTimeGauge > m_SlowTimeCanInputGauge + ((m_SlowTimeMaxGauge - m_SlowTimeCanInputGauge) / 4) * 3)
	{
		m_pSnailWorldWidget->FillSnailGauge(3);
	}
	else if (m_SlowTimeGauge > m_SlowTimeCanInputGauge + ((m_SlowTimeMaxGauge - m_SlowTimeCanInputGauge) / 4) * 2)
	{
		m_pSnailWorldWidget->FillSnailGauge(2);
	}
	else if (m_SlowTimeGauge > m_SlowTimeCanInputGauge + ((m_SlowTimeMaxGauge - m_SlowTimeCanInputGauge) / 4) * 1)
	{
		m_pSnailWorldWidget->FillSnailGauge(1);
	}
	else if (m_SlowTimeGauge > m_SlowTimeCanInputGauge && !m_bCheckSnailWidgetEnable)
	{
		m_pSnailWorldWidget->SnailGaugeEnable(true);
		m_pSnailWorldWidget->FillSnailGauge(0);
		m_bCheckSnailWidgetEnable = true;
	}

}

void CPlayer::SlowTimeKeyUp(float DeltaTime)
{
	if (m_bIsTimeStop) return;
	
	if (m_SlowTimeGauge >= m_SlowTimeMaxGauge)
	{
		m_bIsSlowTime = true;
		SlowTime();
	}

	m_pSnailWorldWidget->SnailGaugeEnable(false);
	m_bCheckSnailWidgetEnable = false;
	m_SlowTimeGauge = 0.f;
}

void CPlayer::SlowTime()
{
	m_bCanTimeStop = false;
	m_bIsSlowTime = true;
	m_bAutoIdle = false;
	SetEnable(false, false, false);
	m_pPlayerWidget->SetTimeState(TimeMode::Snail);
	m_pScene->GetSceneMode()->SetTimeMode(TimeMode::Snail);
	m_PlayerAnim = PlayerAnimation::SlowTime;

	GET_TIMER->StartTimer<CPlayer>("SlowTimeAnim", .8f, this, &CPlayer::SlowTimeAnimEnd);
	Invincible();

	if (m_View == CharacterView::Right)
	{
		if (!m_bIsJump)
		{
			ChangeAnimation("PlayerSlowStand");

			CSharedPtr<CPlayerEffect> SlowTimeSunabokori = m_pScene->CreateGameObject<CPlayerEffect>("Player_SlowTime_Effect", Vector2(m_vPos.x, m_vPos.y), Vector2(512.f, 512.f));
			SlowTimeSunabokori->SetOwner(this);
			SlowTimeSunabokori->SetPivot(0.5f, 1.f);
			SlowTimeSunabokori->SetUseLocalSpace(true);
			SlowTimeSunabokori->AddAnimation("Player_Sunabokori_Effect_Anim", AnimationType::Sprite, "Effect_Atlass01", 1.f);
			SlowTimeSunabokori->AddAnimationFrameData("Player_Sunabokori_Effect_Anim", 0, 0, 23, Vector2(512.f, 512.f));
			SlowTimeSunabokori->SetAutoDestroy();
			SlowTimeSunabokori->SetOffset(Vector2(-30.f, 0.f));
		}
		else
		{
			m_PhysicsSimulate = false;
			ChangeAnimation("PlayerSlowAir");
		}
	}
	else
	{
		if (!m_bIsJump)
		{
			ChangeAnimation("PlayerSlowStand_Left");

			CSharedPtr<CPlayerEffect> SlowTimeSunabokori_Left = m_pScene->CreateGameObject<CPlayerEffect>("Player_SlowTime_Effect_Left", m_vPos, Vector2(512.f, 512.f));
			SlowTimeSunabokori_Left->SetOwner(this);
			SlowTimeSunabokori_Left->SetPivot(0.5f, 1.f);
			SlowTimeSunabokori_Left->SetUseLocalSpace(true);
			SlowTimeSunabokori_Left->AddAnimation("Player_Sunabokori_Effect_Left_Anim", AnimationType::Sprite, "Effect_Atlass01_Left", 1.f);
			SlowTimeSunabokori_Left->AddAnimationFrameData("Player_Sunabokori_Effect_Left_Anim", 0, 0, 23, Vector2(512.f, 512.f));
			SlowTimeSunabokori_Left->SetAutoDestroy();
			SlowTimeSunabokori_Left->SetOffset(Vector2(30.f, 0.f));
		}
		else
		{
			m_PhysicsSimulate = false;
			ChangeAnimation("PlayerSlowAir_Left");
		}
	}

	CSharedPtr<CPlayerEffect> TimeStopEndRing = m_pScene->CreateGameObject<CPlayerEffect>("Player_TimeStopRing_Effect", GetCenterPos(), Vector2(512.f, 512.f));
	TimeStopEndRing->SetOwner(this);
	TimeStopEndRing->SetUseLocalSpace(false);
	TimeStopEndRing->AddAnimation("Player_TimeStopRing_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", 0.3f);
	TimeStopEndRing->AddAnimationFrameData("Player_TimeStopRing_Effect_Anim", 0, 7, 20, Vector2(512.f, 512.f), Vector2(0.5f, 0.5f));
	TimeStopEndRing->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("TimeSlow");
}

void CPlayer::SlowTimeAnimEnd()
{
	m_PlayerAnim = PlayerAnimation::None;
	m_PhysicsSimulate = true;
	m_bAutoIdle = true;
	m_bCanTimeStop = true;
	m_PhysicsSimulate = true;
	SetEnable();
	ReleaseInvincible();
	GET_TIMER->StartTimer<CPlayer>("SlowTime", 3.f, this, &CPlayer::SlowTimeEnd);
}

void CPlayer::SlowTimeEnd()
{
	m_bIsSlowTime = false;
	m_pPlayerWidget->SetTimeState(TimeMode::Move);
	m_pScene->GetSceneMode()->SetTimeMode(TimeMode::Move);
}

void CPlayer::Dialogue()
{
	m_IsDialogue = true;
	m_IsInteraction = true;

	if (GET_SINGLE(CTextManager)->GetEnableAllStop())
		m_pScene->GetSceneMode()->AllStop();

	GET_SINGLE(CTextManager)->FindDialogueCursor();
	m_pPlayerWidget->SetText(GET_SINGLE(CTextManager)->GetText());	
}

void CPlayer::ReleaseDialogue()
{
	m_IsDialogue = false;
	m_IsInteraction = false;
	m_pScene->GetSceneMode()->ReleaseAllStop();
	m_pPlayerWidget->HideText();
}

void CPlayer::LearnSkill(const std::string& Name)
{
	m_IsInteraction = true;
	SetEnable(false, false, false);
	m_pScene->GetSceneMode()->AllStop();

	EnableTimeStopSkill(true);
	m_pPlayerWidget->GetItem(Name);
}

void CPlayer::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	if (pDest->GetCollisionProfile()->Name == "Monster" || pDest->GetCollisionProfile()->Name == "MonsterAttack")
	{

		CAICharacter* pAI = Cast<CAICharacter*>(pDest->GetOwner());

		if (pAI)
		{
			m_bCanInvincible = true;
			if (pAI->IsAttack())
				TakeDamage(pAI->GetAD(), pAI);
			else
			{
				if (pAI->CheckFirstPassBy())
					TakeDamage(pAI->GetAD(), pAI);
				else
				{
					Graze();
					pAI->SetCheckFirstPassBy();
				}
			}
		}
		else
		{
			m_bCanInvincible = false;
			DamageableInterface* pDamageable = Cast<DamageableInterface*>(pDest->GetOwner());
			if (pDamageable)
			{
				TakeDamage(pDamageable->GetAD(), (CGameObject*)pDamageable);
			}
		}
	}
}
