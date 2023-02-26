#include "MoveableObject.h"
#include "../Collision/Collider.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Animation.h"
#include "../Scene/Camera.h"
#include "../UI/UIWidgetWorld.h"
#include "../Stage/Stage.h"
#include "../Stage/Tile.h"
#include "../Collision/Collision.h"


CMoveableObject::CMoveableObject() :
	m_MoveSpeed(0.f),
	m_bEnableTarget(true),
	m_pTarget(nullptr),
	m_TileInteraction(TileInteraction::Block),
	m_PhysicsSimulate(false),
	m_IsGround(false),
	m_FallTime(0.f),
	m_GravityAccel(18.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_UpBlock(false),
	m_pJumpEndFunc(nullptr),
	m_Raycast(true),
	m_IsFly(false),
	m_Velocity(0.f),
	m_VelocityMax(7.f),
	m_bUseAccel(true),
	m_Force(0.f)
{
}

CMoveableObject::CMoveableObject(const CMoveableObject& obj) :
	CGameObject(obj)
{
}

CMoveableObject::~CMoveableObject()
{
}

void CMoveableObject::Jump()
{
	if (m_Jump) return;

	m_IsGround = false;
	m_Jump = true;
	m_FallStartY = m_vPos.y;
	m_Velocity = m_JumpVelocity;
	m_FallTime = 0.f;
}

void CMoveableObject::JumpEnd()
{
	m_IsGround = true;
	m_Jump = false;
	m_FallStartY = m_vPos.y;
	m_IsFalling = false;
}


void CMoveableObject::TileInteractionBlock()
{
	// ������Ʈ�� Ÿ���� �İ����� �Ǵ��ؾ� �Ѵ�.
// �̵� ���� ��쿡�� ó���Ѵ�.
	if (m_vMove.x != 0.f || m_vMove.y != 0.f)
	{
		// ������Ʈ�� �̿��ؼ� �簢�� ������ ���Ѵ�.
		Vector2	vSize = m_vSize;
		if (m_pAnimation)
		{
			/*const AnimationFrameData& FrameData =
				m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

			vSize = FrameData.Size;*/
		}

		Vector2	vLT;
		Vector2	vRB;

		CStage* pStage = m_pScene->GetStage();

		if (pStage)
		{
			m_bTileBlock = false;

			// ���� �ö󰡰� ���� ���
			if (m_vMove.y < 0.f)
			{
				// �� ��� ���� �ö󰡰� ���� ���� ��� �����ش�.
				if (m_UpBlock)
				{
					// ��, �� Ȥ�� ���� �ε��� ��쿡�� üũ�� ���ֵ��� �Ѵ�.
					if (m_vMove.x < 0.f)
					{
						Vector2	vLine1Start, vLine1End;
						Vector2	vLine2Start, vLine2End;

						vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
						vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

						vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
						vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;

						Vector2	vIndexStart, vIndexEnd;
						vIndexStart.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
						vIndexStart.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

						vIndexEnd.x = m_vPos.x - m_vPivot.x * vSize.x;
						vIndexEnd.y = m_vPos.y - m_vPivot.y * vSize.y;

						int	StartX, StartY, EndX, EndY;
						StartX = pStage->GetTileIndexX(vIndexStart);
						StartY = pStage->GetTileIndexY(vIndexStart);

						EndX = pStage->GetTileIndexX(vIndexEnd);
						EndY = pStage->GetTileIndexY(vIndexEnd);

						bool	LoopEnd = false;

						for (int i = StartY; i >= EndY; --i)
						{
							//if (LoopEnd)
							//	break;

							for (int j = StartX; j >= EndX; --j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);

								if (!pTile) return;

								if (pTile->GetTileType() == TileType::Block)
								{
									// ���� Ÿ���� �Ʒ�, ������ ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.

									// �Ʒ��� üũ�Ѵ�.
									vLine2Start.x = pTile->GetPos().x;
									vLine2Start.y = pTile->GetPos().y + pTile->GetSize().y;

									vLine2End.x = pTile->GetPos().x + pTile->GetSize().y;
									vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

									Vector2	vIntersect;

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Bottom = pTile->GetPos().y + pTile->GetSize().y;
										float	Top = m_vPos.y - m_vPivot.y * vSize.y;

										m_vMove.y += ((Bottom - Top) + 0.1f);
										m_vPos.y += ((Bottom - Top) + 0.1f);


										m_Jump = false;
										m_Velocity = 0.f;
										m_FallStartY = m_vPos.y;
										m_FallTime = 0.f;

										LoopEnd = true;
										m_bTileBlock = true;

										vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
										vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

										vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
										vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;
									}

									// �������� üũ�Ѵ�.
									vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
									vLine2Start.y = pTile->GetPos().y;

									vLine2End = pTile->GetPos() + pTile->GetSize();

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Left = m_vPos.x - m_vPivot.x * vSize.x;
										float	Right = pTile->GetPos().x + pTile->GetSize().x;

										m_vMove.x += ((Right - Left) + 0.1f);
										m_vPos.x += ((Right - Left) + 0.1f);

										LoopEnd = true;
										m_bTileBlock = true;

										vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
										vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

										vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
										vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;
									}

									//if (LoopEnd)
									//	break;
								}
							}
						}

						// ���� ����� �ε��� Ÿ���� ���ٸ� ���� �ϴ� ������ �̿��Ͽ�
						// �ѹ� �� üũ���ش�.
						vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
						vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

						vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
						vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;

						bool	Hit = LoopEnd;

						LoopEnd = false;

						for (int i = StartY; i >= EndY; --i)
						{
							//if (LoopEnd)
							//	break;

							for (int j = StartX; j >= EndX; --j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);

								if (!pTile) return;

								if (pTile->GetTileType() == TileType::Block)
								{
									// ���� Ÿ���� �Ʒ�, ������ ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.
									Vector2	vIntersect;

									// �������� üũ�Ѵ�.
									vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
									vLine2Start.y = pTile->GetPos().y;

									vLine2End = pTile->GetPos() + pTile->GetSize();

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Left = m_vPos.x - m_vPivot.x * vSize.x;
										float	Right = pTile->GetPos().x + pTile->GetSize().x;

										m_vMove.x += ((Right - Left) + 0.1f);
										m_vPos.x += ((Right - Left) + 0.1f);

										LoopEnd = true;
										m_bTileBlock = true;

										vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
										vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

										vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
										vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
									}

									//break;
								}
							}
						}

						if (!Hit)
						{
							// ������ ��� ������ üũ�Ѵ�.
							vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
							vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

							vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
							vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;

							for (int i = StartY; i >= EndY; --i)
							{
								//if (LoopEnd)
								//	break;

								for (int j = StartX; j >= EndX; --j)
								{
									int	Index = i * pStage->GetTileCountX() + j;

									CTile* pTile = pStage->GetTile(Index);

									if (!pTile) return;

									if (pTile->GetTileType() == TileType::Block)
									{
										// ���� Ÿ���� �Ʒ�, ������ ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.

										// �Ʒ��� üũ�Ѵ�.
										vLine2Start.x = pTile->GetPos().x;
										vLine2Start.y = pTile->GetPos().y + pTile->GetSize().y;

										vLine2End.x = pTile->GetPos().x + pTile->GetSize().y;
										vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

										Vector2	vIntersect;

										if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
											vLine2Start, vLine2End))
										{
											float	Bottom = pTile->GetPos().y + pTile->GetSize().y;
											float	Top = m_vPos.y - m_vPivot.y * vSize.y;

											m_vMove.y += ((Bottom - Top) + 0.1f);
											m_vPos.y += ((Bottom - Top) + 0.1f);

											m_Jump = false;
											m_Velocity = 0.f;
											m_FallStartY = m_vPos.y;
											m_FallTime = 0.f;

											LoopEnd = true;
											m_bTileBlock = true;

											vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
											vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

											vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
											vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;
											//break;
										}
									}
								}
							}
						}
					}

					else if (m_vMove.x > 0.f)
					{
						Vector2	vLine1Start, vLine1End;
						Vector2	vLine2Start, vLine2End;

						vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
						vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

						vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
						vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;

						Vector2	vIndexStart, vIndexEnd;

						vIndexStart.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
						vIndexStart.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

						vIndexEnd.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
						vIndexEnd.y = m_vPos.y - m_vPivot.y * vSize.y;

						int	StartX, StartY, EndX, EndY;
						StartX = pStage->GetTileIndexX(vIndexStart);
						StartY = pStage->GetTileIndexY(vIndexStart);

						EndX = pStage->GetTileIndexX(vIndexEnd);
						EndY = pStage->GetTileIndexY(vIndexEnd);

						// �ٷ� ������ ������ Ÿ�ϰ� ��ĭ �� ���� �Ʒ� Ÿ���� ������ Ÿ���̶�� ��, ��������
						// �о��ش�.

						bool	LoopEnd = false;

						for (int i = StartY; i >= EndY; --i)
						{
							//if (LoopEnd)
							//	break;

							for (int j = StartX; j <= EndX; ++j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);

								if (!pTile) return;

								if (pTile->GetTileType() == TileType::Block)
								{
									// ���� Ÿ���� �Ʒ�, ���� ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.

									// �Ʒ��� üũ�Ѵ�.
									vLine2Start.x = pTile->GetPos().x;
									vLine2Start.y = pTile->GetPos().y + pTile->GetSize().y;

									vLine2End.x = pTile->GetPos().x + pTile->GetSize().x;
									vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

									Vector2	vIntersect;

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Bottom = pTile->GetPos().y + pTile->GetSize().y;
										float	Top = m_vPos.y - m_vPivot.y * vSize.y;

										m_vMove.y += ((Bottom - Top) + 1.f);
										m_vPos.y += ((Bottom - Top) + 1.f);

										m_Jump = false;
										m_Velocity = 0.f;
										m_FallStartY = m_vPos.y;
										m_FallTime = 0.f;

										LoopEnd = true;
										m_bTileBlock = true;

										vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
										vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

										vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
										vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;
									}

									// ������ üũ�Ѵ�.
									vLine2Start = pTile->GetPos();
									vLine2End.x = pTile->GetPos().x;
									vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Left = pTile->GetPos().x;
										float	Right = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;

										m_vMove.x -= ((Right - Left) + 0.1f);
										m_vPos.x -= ((Right - Left) + 0.1f);

										LoopEnd = true;
										m_bTileBlock = true;

										vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
										vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

										vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
										vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;
									}

									//if (LoopEnd)
									//	break;
								}
							}
						}

						// ������ ����� �ε��� Ÿ���� ���ٸ� ������ �ϴ� ������ �̿��Ͽ�
						// �ѹ� �� üũ���ش�.
						vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
						vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

						vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
						vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;

						bool	Hit = LoopEnd;
						LoopEnd = false;

						for (int i = StartY; i >= EndY; --i)
						{
							//if (LoopEnd)
							//	break;

							for (int j = StartX; j <= EndX; ++j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);

								if (!pTile) return;

								if (pTile->GetTileType() == TileType::Block)
								{
									// ���� Ÿ���� �Ʒ�, ���� ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.
									Vector2	vIntersect;

									// ������ üũ�Ѵ�.
									vLine2Start = pTile->GetPos();
									vLine2End.x = pTile->GetPos().x;
									vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Left = pTile->GetPos().x;
										float	Right = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;

										m_vMove.x -= ((Right - Left) + 0.1f);
										m_vPos.x -= ((Right - Left) + 0.1f);

										LoopEnd = true;
										m_bTileBlock = true;

										vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
										vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

										vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
										vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
										//break;
									}
								}
							}
						}

						if (!Hit)
						{
							// ���� ��� ���� üũ
							vLine1Start = m_vPrevPos - m_vPivot * vSize;
							vLine1End = m_vPos - m_vPivot * vSize;

							for (int i = StartY; i >= EndY; --i)
							{
								//if (LoopEnd)
								//	break;

								for (int j = StartX; j <= EndX; ++j)
								{
									int	Index = i * pStage->GetTileCountX() + j;

									CTile* pTile = pStage->GetTile(Index);

									if (!pTile) return;

									if (pTile->GetTileType() == TileType::Block)
									{
										// ���� Ÿ���� �Ʒ�, ������ ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.

										// �Ʒ��� üũ�Ѵ�.
										vLine2Start.x = pTile->GetPos().x;
										vLine2Start.y = pTile->GetPos().y + pTile->GetSize().y;

										vLine2End.x = pTile->GetPos().x + pTile->GetSize().y;
										vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

										Vector2	vIntersect;

										if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
											vLine2Start, vLine2End))
										{
											float	Bottom = pTile->GetPos().y + pTile->GetSize().y;
											float	Top = m_vPos.y - m_vPivot.y * vSize.y;

											m_vMove.y += ((Bottom - Top) + 1.f);
											m_vPos.y += ((Bottom - Top) + 1.f);

											m_Jump = false;
											m_Velocity = 0.f;
											m_FallStartY = m_vPos.y;
											m_FallTime = 0.f;

											LoopEnd = true;
											m_bTileBlock = true;

											vLine1Start = m_vPrevPos - m_vPivot * vSize;
											vLine1End = m_vPos - m_vPivot * vSize;
											//break;
										}
									}
								}
							}
						}
					}

					else
					{
						vRB = m_vPrevPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;

						// ������ �浹ü ������ ������.
						// ������������ LT�� ���� �������� RB�� �����ش�.
						vLT = m_vPos - m_vPivot * vSize;
						float	Top = vLT.y;

						int	StartIndexX, StartIndexY, EndIndexX, EndIndexY;
						StartIndexX = pStage->GetTileClampIndexX(vLT);
						EndIndexX = pStage->GetTileClampIndexX(vRB);

						StartIndexY = pStage->GetTileClampIndexY(vRB);
						EndIndexY = pStage->GetTileClampIndexY(vLT);

						bool	LoopEnd = false;

						for (int i = StartIndexY; i >= EndIndexY; --i)
						{
							if (LoopEnd)
								break;

							for (int j = StartIndexX; j <= EndIndexX; ++j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);

								if (!pTile) return;

								if (pTile->GetTileType() == TileType::Block)
								{
									float	TileBottom = pTile->GetPos().y + pTile->GetSize().y;

									m_vMove.y += ((TileBottom - Top) + 1.f);
									m_vPos.y += ((TileBottom - Top) + 1.f);

									m_Jump = false;
									m_Velocity = 0.f;
									m_FallStartY = m_vPos.y;
									m_FallTime = 0.f;

									LoopEnd = true;

									if (m_Name == "Bullet")
										m_bTileBlock = true;

									break;
								}
							}
						}
					}
				}
			}

			// �Ʒ��� �������� ���� ���
			else if (m_vMove.y > 0.f)
			{
				// ��, �� Ȥ�� �Ʒ��� �ε��� ��쿡�� üũ�� ���ֵ��� �Ѵ�.
				if (m_vMove.x < 0.f)
				{
					Vector2	vLine1Start, vLine1End;
					Vector2	vLine2Start, vLine2End;

					vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
					vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

					vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
					vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;

					Vector2	vIndexStart, vIndexEnd;
					vIndexStart = vLine1End;
					vIndexEnd.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
					vIndexEnd.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

					int	StartX, StartY, EndX, EndY;
					StartX = pStage->GetTileIndexX(vIndexEnd);
					StartY = pStage->GetTileIndexY(vIndexEnd);

					EndY = pStage->GetTileIndexY(vIndexStart);
					EndX = pStage->GetTileIndexX(vIndexStart);

					bool	LoopEnd = false;

					for (int i = StartY; i <= EndY; ++i)
					{
						//if (LoopEnd)
						//	break;

						for (int j = StartX; j >= EndX; --j)
						{
							int	Index = i * pStage->GetTileCountX() + j;

							CTile* pTile = pStage->GetTile(Index);

							if (!pTile) return;

							if (pTile->GetTileType() == TileType::Block)
							{
								// ���� Ÿ���� ��, ������ ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.

								// ���� üũ�Ѵ�.
								vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
								vLine2Start.y = pTile->GetPos().y;

								vLine2End = pTile->GetPos();

								Vector2	vIntersect;

								if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
									vLine2Start, vLine2End))
								{
									float	Bottom = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
									float	Top = pTile->GetPos().y;

									m_vMove.y -= ((Bottom - Top) + 0.1f);
									m_vPos.y -= ((Bottom - Top) + 0.1f);

									m_FallStartY = m_vPos.y;
									JumpEnd();

									LoopEnd = true;
									m_bTileBlock = true;

									vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
									vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

									vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
									vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
								}

								// �������� üũ�Ѵ�.
								vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
								vLine2Start.y = pTile->GetPos().y;

								vLine2End = pTile->GetPos() + pTile->GetSize();

								if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
									vLine2Start, vLine2End))
								{
									float	Left = m_vPos.x - m_vPivot.x * vSize.x;
									float	Right = pTile->GetPos().x + pTile->GetSize().x;

									m_vMove.x += ((Right - Left) + 0.1f);
									m_vPos.x += ((Right - Left) + 0.1f);

									LoopEnd = true;
									m_bTileBlock = true;

									vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
									vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

									vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
									vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
								}
							}

							//if (LoopEnd)
							//	break;
						}
					}

					vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
					vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

					vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
					vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;

					bool	Hit = LoopEnd;
					LoopEnd = false;

					for (int i = StartY; i <= EndY; ++i)
					{
						//if (LoopEnd)
						//	break;

						for (int j = StartX; j >= EndX; --j)
						{
							int	Index = i * pStage->GetTileCountX() + j;

							CTile* pTile = pStage->GetTile(Index);

							if (!pTile) return;

							if (pTile->GetTileType() == TileType::Block)
							{
								// ���� Ÿ���� ��, ������ ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.
								Vector2	vIntersect;

								// �������� üũ�Ѵ�.
								vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
								vLine2Start.y = pTile->GetPos().y;

								vLine2End = pTile->GetPos() + pTile->GetSize();

								if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
									vLine2Start, vLine2End))
								{
									float	Left = m_vPos.x - m_vPivot.x * vSize.x;
									float	Right = pTile->GetPos().x + pTile->GetSize().x;

									m_vMove.x += ((Right - Left) + 0.1f);
									m_vPos.x += ((Right - Left) + 0.1f);

									LoopEnd = true;
									m_bTileBlock = true;

									vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
									vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

									vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
									vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;
									//break;
								}
							}
						}
					}

					if (!Hit)
					{
						// ������ �ϴ� üũ
						vLine1Start = m_vPrevPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
						vLine1End = m_vPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;

						for (int i = StartY; i <= EndY; ++i)
						{
							//if (LoopEnd)
							//	break;

							for (int j = StartX; j >= EndX; --j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);

								if (!pTile) return;

								vLine1End = pTile->GetPos() + pTile->GetSize() * 0.5f;

								if (pTile->GetTileType() == TileType::Block)
								{
									// ���� üũ�Ѵ�.
									vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
									vLine2Start.y = pTile->GetPos().y;

									vLine2End = pTile->GetPos();

									Vector2	vIntersect;

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Bottom = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
										float	Top = pTile->GetPos().y;

										m_vMove.y -= ((Bottom - Top) + 0.1f);
										m_vPos.y -= ((Bottom - Top) + 0.1f);

										m_IsGround = true;
										m_FallStartY = m_vPos.y;
										JumpEnd();
										LoopEnd = true;
										m_bTileBlock = true;

										vLine1Start = m_vPrevPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
										vLine1End = m_vPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;

										//break;
									}
								}
							}
						}
					}
				}

				else if (m_vMove.x > 0.f)
				{
					Vector2	vLine1Start, vLine1End;
					Vector2	vLine2Start, vLine2End;

					vLine1Start = m_vPrevPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
					vLine1End = m_vPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;

					Vector2	vIndexStart, vIndexEnd;
					vIndexStart = m_vPrevPos - m_vPivot * vSize;
					vIndexEnd = vLine1End;

					int	StartX, StartY, EndX, EndY;
					StartX = pStage->GetTileIndexX(vIndexStart);
					StartY = pStage->GetTileIndexY(vIndexStart);

					EndX = pStage->GetTileIndexX(vIndexEnd);
					EndY = pStage->GetTileIndexY(vIndexEnd);

					// �ٷ� ������ ������ Ÿ�ϰ� ��ĭ �� ���� �Ʒ� Ÿ���� ������ Ÿ���̶�� ��, ��������
					// �о��ش�.

					bool	LoopEnd = false;

					for (int i = StartY; i <= EndY; ++i)
					{
						//if (LoopEnd)
						//	break;

						for (int j = StartX; j <= EndX; ++j)
						{
							int	Index = i * pStage->GetTileCountX() + j;

							CTile* pTile = pStage->GetTile(Index);

							if (!pTile) return;

							if (pTile->GetTileType() == TileType::Block)
							{
								// ���� Ÿ���� ��, ���� ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.
								
								// ���� üũ�Ѵ�.
								vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
								vLine2Start.y = pTile->GetPos().y;

								vLine2End = pTile->GetPos();

								Vector2	vIntersect;

								if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
									vLine2Start, vLine2End))
								{
									float	Bottom = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
									float	Top = pTile->GetPos().y;

									m_vMove.y -= ((Bottom - Top) + 0.1f);
									m_vPos.y -= ((Bottom - Top) + 0.1f);

									m_FallStartY = m_vPos.y;
									JumpEnd();

									LoopEnd = true;
									m_bTileBlock = true;

									vLine1Start = m_vPrevPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
									vLine1End = m_vPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
								}

								// ������ üũ�Ѵ�.
								vLine2Start = pTile->GetPos();
								vLine2End.x = pTile->GetPos().x;
								vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

								if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
									vLine2Start, vLine2End))
								{
									float	Left = pTile->GetPos().x;
									float	Right = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;

									m_vMove.x -= ((Right - Left) + 0.1f);
									m_vPos.x -= ((Right - Left) + 0.1f);

									LoopEnd = true;
									m_bTileBlock = true;

									vLine1Start = m_vPrevPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
									vLine1End = m_vPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
								}
							}

							//if (LoopEnd)
							//	break;
						}
					}

					vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
					vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

					vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
					vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;

					bool	Hit = LoopEnd;
					LoopEnd = false;

					for (int i = StartY; i <= EndY; ++i)
					{
						//if (LoopEnd)
						//	break;

						for (int j = StartX; j <= EndX; ++j)
						{
							int	Index = i * pStage->GetTileCountX() + j;

							CTile* pTile = pStage->GetTile(Index);
							if (!pTile) return;
							if (pTile->GetTileType() == TileType::Block)
							{
								// ���� Ÿ���� ��, ���� ������ ���� �����ϴ����� �Ǵ��ؾ� �Ѵ�.
								Vector2	vIntersect;
								
								// ������ üũ�Ѵ�.
								vLine2Start = pTile->GetPos();
								vLine2End.x = pTile->GetPos().x;
								vLine2End.y = pTile->GetPos().y + pTile->GetSize().y;

								if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
									vLine2Start, vLine2End))
								{
									float	Left = pTile->GetPos().x;
									float	Right = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;

									m_vMove.x -= ((Right - Left) + 0.1f);
									m_vPos.x -= ((Right - Left) + 0.1f);

									LoopEnd = true;
									m_bTileBlock = true;

									vLine1Start.x = m_vPrevPos.x + (1.f - m_vPivot.x) * vSize.x;
									vLine1Start.y = m_vPrevPos.y - m_vPivot.y * vSize.y;

									vLine1End.x = m_vPos.x + (1.f - m_vPivot.x) * vSize.x;
									vLine1End.y = m_vPos.y - m_vPivot.y * vSize.y;

									//break;
								}
							}
						}
					}

					if (!Hit)
					{
						// ���� �ϴ� üũ
						vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
						vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

						vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
						vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;

						for (int i = StartY; i <= EndY; ++i)
						{
							for (int j = StartX; j <= EndX; ++j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);
								if (!pTile) return;
								vLine1End = pTile->GetPos() + pTile->GetSize() * 0.5f;

								if (pTile->GetTileType() == TileType::Block)
								{
									// ���� üũ�Ѵ�.
									vLine2Start.x = pTile->GetPos().x + pTile->GetSize().x;
									vLine2Start.y = pTile->GetPos().y;
									
									vLine2End = pTile->GetPos();

									Vector2	vIntersect;

									if (CCollision::CollisionLineToLine(vIntersect, vLine1Start, vLine1End,
										vLine2Start, vLine2End))
									{
										float	Bottom = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
										float	Top = pTile->GetPos().y;

										m_vMove.y -= ((Bottom - Top) + 0.1f);
										m_vPos.y -= ((Bottom - Top) + 0.1f);

										m_IsGround = true;
										m_FallStartY = m_vPos.y;
										JumpEnd();
										LoopEnd = true;
										m_bTileBlock = true;
										vLine1Start.x = m_vPrevPos.x - m_vPivot.x * vSize.x;
										vLine1Start.y = m_vPrevPos.y + (1.f - m_vPivot.y) * vSize.y;

										vLine1End.x = m_vPos.x - m_vPivot.x * vSize.x;
										vLine1End.y = m_vPos.y + (1.f - m_vPivot.y) * vSize.y;
									}
								}
							}
						}
					}
				}

				else
				{
					vRB = m_vPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;
					float	Bottom = vRB.y;

					// ������ �浹ü ������ ������.
					// ������������ LT�� ���� �������� RB�� �����ش�.
					vLT = m_vPrevPos - m_vPivot * vSize;

					int	StartIndexX, StartIndexY, EndIndexX, EndIndexY;
					StartIndexX = pStage->GetTileClampIndexX(vLT);
					EndIndexX = pStage->GetTileClampIndexX(vRB);

					StartIndexY = pStage->GetTileClampIndexY(vLT);
					EndIndexY = pStage->GetTileClampIndexY(vRB);

					bool	LoopEnd = false;

					for (int i = StartIndexY; i <= EndIndexY; ++i)
					{
						if (LoopEnd)
							break;

						for (int j = StartIndexX; j <= EndIndexX; ++j)
						{
							int	Index = i * pStage->GetTileCountX() + j;

							CTile* pTile = pStage->GetTile(Index);
							if (!pTile) return;
							if (pTile->GetTileType() == TileType::Block)
							{
								float	TileTop = pTile->GetPos().y;
								if (TileTop <= Bottom && TileTop >= m_vPrevPos.y)
								{
									m_vMove.y -= ((Bottom - TileTop) + 0.1f);
									m_vPos.y -= ((Bottom - TileTop) + 0.1f);
									
									
									m_FallStartY = m_vPos.y;
									JumpEnd();

									LoopEnd = true;
									m_bTileBlock = true;

									break;
								}
							}
						}
					}
				}
			}

			// ��, ��θ� �����̰� ���� ���
			else
			{

				// ����
				if (m_vMove.x < 0.f)
				{
					m_IsGround = false;

					vLT = m_vPos - m_vPivot * vSize;
					vRB = m_vPrevPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;

					int	StartX = pStage->GetTileIndexX(vRB);
					int	EndX = pStage->GetTileIndexX(vLT);

					int	StartY = pStage->GetTileIndexY(vLT);
					int	EndY = pStage->GetTileIndexY(vRB) + 1;

					if (StartY != -1 && EndY != -1 && StartX != -1 && EndX != -1)
					{
						bool	LoopEnd = false;
						for (int i = StartY; i <= EndY - 1; ++i)
						{
							if (LoopEnd)
								break;

							for (int j = StartX; j >= EndX; --j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);
								if (!pTile) return;
								if (pTile->GetTileType() == TileType::Block)
								{
									m_vPos.x += ((pTile->GetPos().x + pTile->GetSize().x) - vLT.x) + 0.1f;
									m_vMove.x += ((pTile->GetPos().x + pTile->GetSize().x) - vLT.x) + 0.1f;

									LoopEnd = true;
									m_bTileBlock = true;
									JumpEnd();
									break;
								}
							}
						}

						for (int j = StartX; j >= EndX; --j)
						{
							int	Index  = EndY * pStage->GetTileCountX() + j;

							CTile* pTile = pStage->GetTile(Index);
							if (!pTile) return;
							if (pTile->GetTileType() == TileType::Block)
							{
								JumpEnd();

								if (m_Name != "Bullet")
									m_bTileBlock = true;
								break;
							}
						}
					}									
				}

				// ������
				else
				{
					m_IsGround = false;
					// m_vPrevPos
					vLT = m_vPrevPos - m_vPivot * vSize;
					vRB = m_vPos + (Vector2(1.f, 1.f) - m_vPivot) * vSize;

					int	StartX = pStage->GetTileIndexX(vLT);
					int	EndX = pStage->GetTileIndexX(vRB);
					
					int	StartY = pStage->GetTileIndexY(vLT);
					int	EndY = pStage->GetTileIndexY(vRB) + 1;

					if (StartY != -1 && EndY != -1 && StartX != -1 && EndX != -1)
					{
						bool	LoopEnd = false;
						for (int i = StartY; i <= EndY - 1; ++i)
						{
							if (LoopEnd)
								break;

							for (int j = StartX; j <= EndX; ++j)
							{
								int	Index = i * pStage->GetTileCountX() + j;

								CTile* pTile = pStage->GetTile(Index);
								if (!pTile) return;
								if (pTile->GetTileType() == TileType::Block)
								{
									m_vPos.x -= ((vRB.x - pTile->GetPos().x) + 0.1f);
									m_vMove.x -= ((vRB.x - pTile->GetPos().x) + 0.1f);

									LoopEnd = true;
									m_bTileBlock = true;
									JumpEnd();

									break;
								}
							}
						}

						for (int j = StartX; j <= EndX; ++j)
						{
							int	Index = (EndY) * pStage->GetTileCountX() + j;

							CTile* pTile = pStage->GetTile(Index);
							if (!pTile) return;
							if (pTile->GetTileType() == TileType::Block)
							{
								JumpEnd();

								if (m_Name != "Bullet")
									m_bTileBlock = true;

								break;
							}
						}
					}
				}
			}
		}
	}
}

void CMoveableObject::TileInteractionDestroy()
{
	if (m_vMove.x != 0.f || m_vMove.y != 0.f)
	{
		// ������Ʈ�� �̿��ؼ� �簢�� ������ ���Ѵ�.
		Vector2	vSize = m_vSize;
		if (m_pAnimation)
		{
			/*const AnimationFrameData& FrameData =
				m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

			vSize = FrameData.Size;*/
		}

		Vector2	vLT = m_vPos - m_vPivot * vSize;
		Vector2	vRB = vLT + vSize;

		CStage* pStage = m_pScene->GetStage();
		bool	Block = false;
		// ���� ��, ���� �Ʒ��� ���
		if (m_vMove.x < 0.f)
		{
			// ���� ���� �ε����� �Ǵ��Ͽ� �� �� ���� Ÿ���� ���� ��� ������ �Ѵ�.
			int IndexX = pStage->GetTileIndexX(vLT);
			int	StartIndexY = pStage->GetTileIndexY(vLT);
			int	EndIndexY = pStage->GetTileIndexY(Vector2(vLT.x, vRB.y));

			if (StartIndexY != -1 && EndIndexY != -1 && IndexX != -1)
			{
				for (int i = StartIndexY; i <= EndIndexY; ++i) 
				{
					int	Index = i * pStage->GetTileCountX() + IndexX;

					CTile* pTile = pStage->GetTile(Index);
					if (!pTile) return;
					if (pTile->GetTileType() == TileType::Block)
					{
						Block = true;

						CRef::Destroy();

						if (m_TileInteractionFunc)
							m_TileInteractionFunc();

						break;
					}
				}
			}

			if (!Block)
			{
				if (m_vMove.y < 0.f)
				{
					int	StartIndex = pStage->GetTileIndex(vLT);
					int	EndIndex = pStage->GetTileIndex(Vector2(vRB.x, vLT.y));

					if (StartIndex != -1 && EndIndex != -1)
					{
						for (int i = StartIndex; i <= EndIndex; ++i)
						{
							CTile* pTile = pStage->GetTile(i);

							if (pTile->GetTileType() == TileType::Block)
							{
								CRef::Destroy();

								if (m_TileInteractionFunc)
									m_TileInteractionFunc();

								break;
							}
						}
					}
				}

				else
				{
					int	StartIndex = pStage->GetTileIndex(Vector2(vLT.x, vRB.y));
					int	EndIndex = pStage->GetTileIndex(vRB);

					if (StartIndex != -1 && EndIndex != -1)
					{
						for (int i = StartIndex; i <= EndIndex; ++i)
						{
							CTile* pTile = pStage->GetTile(i);

							if (pTile->GetTileType() == TileType::Block)
							{
								CRef::Destroy();

								if (m_TileInteractionFunc)
									m_TileInteractionFunc();

								break;
							}
						}
					}
				}
			}
		}

		// ������ ��, ������ �Ʒ��� ���
		else
		{
			int IndexX = pStage->GetTileIndexX(vRB);
			int	StartIndexY = pStage->GetTileIndexY(Vector2(vRB.x, vLT.y));
			int	EndIndexY = pStage->GetTileIndexY(vRB);

			if (StartIndexY != -1 && EndIndexY != -1 && IndexX != -1)
			{
				for (int i = StartIndexY; i <= EndIndexY; ++i)
				{
					int	Index = i * pStage->GetTileCountX() + IndexX;

					CTile* pTile = pStage->GetTile(Index);
					if (!pTile) return;
					if (pTile->GetTileType() == TileType::Block)
					{
						Block = true;
						CRef::Destroy();

						if (m_TileInteractionFunc)
							m_TileInteractionFunc();

						break;
					}
				}
			}

			if (!Block)
			{
				if (m_vMove.y < 0.f)
				{
					int	StartIndex = pStage->GetTileIndex(vLT);
					int	EndIndex = pStage->GetTileIndex(Vector2(vRB.x, vLT.y));

					if (StartIndex != -1 && EndIndex != -1)
					{
						for (int i = StartIndex; i <= EndIndex; ++i)
						{
							CTile* pTile = pStage->GetTile(i);

							if (pTile->GetTileType() == TileType::Block)
							{
								CRef::Destroy();

								if (m_TileInteractionFunc)
									m_TileInteractionFunc();

								break;
							}
						}
					}
				}

				else
				{
					int	StartIndex = pStage->GetTileIndex(Vector2(vLT.x, vRB.y));
					int	EndIndex = pStage->GetTileIndex(vRB);

					if (StartIndex != -1 && EndIndex != -1)
					{
						for (int i = StartIndex; i <= EndIndex; ++i)
						{
							CTile* pTile = pStage->GetTile(i);

							if (pTile->GetTileType() == TileType::Block)
							{
								CRef::Destroy();

								if (m_TileInteractionFunc)
									m_TileInteractionFunc();

								break;
							}
						}
					}
				}
			}
		}
	}
}

void CMoveableObject::Start()
{
	CGameObject::Start();

	m_vPrevPos = m_vPos;
	m_FallStartY = m_vPos.y;
}

void CMoveableObject::SetInput()
{
	CGameObject::SetInput();
}

void CMoveableObject::Update(float DeltaTime)
{
	//if (m_hitResult.hitPoint == Vector2(-1, -1) && !m_bTileBlock)
	//	m_IsGround = false;
	// �߷��� �����Ѵ�.
	if (!m_IsGround && m_PhysicsSimulate)
	{
		if (!m_IsFly)
		{
			float	SaveY = 0.f;

			if (m_bUseAccel)
			{
				m_FallTime += DeltaTime * m_GravityAccel;

				float Velocity = 0.f;

				// t�� �� y��	
				// V : �ӵ�     A : ���ӵ�    G : �߷�
				// y = V * A - 0.5f * G * A * A
				// 0 = -0.5GA^2 + VA - y
				// (-b +- ��Ʈ(b^2 - 4ac) / 2a
				if (m_Jump)
					Velocity = (m_JumpVelocity * m_FallTime);

				Velocity += m_Force * DeltaTime;
				
				// m_Velocity - (GRAVITY * m_FallTime) < -m_VelocityMax ? m_Velocity = -m_VelocityMax : m_Velocity -= GRAVITY * m_FallTime;
				m_Velocity = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime) - m_vPos.y;
				
				SaveY = m_vPos.y;

				m_vPos.y += m_Velocity;
			}
		
			else
			{
				SaveY = m_vPos.y;
				m_vPos.y += (m_Velocity * DeltaTime);
			}

			float	MoveY = m_vPos.y - SaveY;

			m_vMove.y += MoveY;

			if (m_vMove.y > 0.f)
				m_IsFalling = true;
			else
				m_IsFalling = false;
		}
	}
	else if (m_IsGround)
	{
		m_FallTime = 0.f;
		m_Velocity = 0.f;
		m_IsFalling = false;
	}

	// �������� ���̵�� Ż���� �����ѽ� �Ǵ� �� ��� ó�� ���ֱ�.
	if (m_vMove.x < 0)
	{
		if (!m_pScene->GetStage()->GetExitLeft() && m_vPos.x - (m_vPivot.x * m_vSize.x) < 0)
			m_vPos.x = (m_vPivot.x * m_vSize.x);
	}

	if (m_vMove.x > 0)
	{
		if (!m_pScene->GetStage()->GetExitRight() && m_vPos.x + ((1 - m_vPivot.x) * m_vSize.x) > m_pScene->GetMainCamera()->GetWorldSize().x)
			m_vPos.x = m_pScene->GetMainCamera()->GetWorldSize().x - ((1 - m_vPivot.x) * m_vSize.x);
	}

	switch (m_TileInteraction)
	{
	case TileInteraction::Block:
		TileInteractionBlock();
		break;
	case TileInteraction::Destroy:
		TileInteractionDestroy();
		break;
	}

	//if (m_pTarget && m_bEnableTarget)
	//{
	//	Vector2	vMove = m_pTarget->GetPos() - m_TargetPrevPos;
	//	m_TargetPrevPos = m_pTarget->GetPos();

	//	m_vPos += vMove;
	//}

	if (RectComponent->GetBlock())
	{
		if (RectComponent->GetBlockRectInfo().left && m_vMoveDir.x < 0.f)
			m_vPos.x += m_vMove.x;
		if (RectComponent->GetBlockRectInfo().right && m_vMoveDir.x > 0.f)
			m_vPos.x -= m_vMove.x;
	}

	CGameObject::Update(DeltaTime);
}

void CMoveableObject::Collision()
{
	CGameObject::Collision();

	m_bHit = false;
	//m_hitResult.hitPoint = Vector2(-1, -1);
	//if (m_hitResult.hitPoint == Vector2(-1, -1) && !m_bTileBlock)
	//	m_IsGround = false;

	if (!m_Raycast || (m_Jump && !m_IsFalling)) return;


	if (m_vMove.y >= 0)
	{
		// ���� �Ʒ� üũ
		HitResult hitResult_LB;
		Vector2 Start_LB = m_vPos;
		Start_LB.x -= m_vSize.x * m_vPivot.x;
		Start_LB.y += 5.f;

		hitResult_LB.Callback = std::bind(&CMoveableObject::GroundWalkLeftBottom, this, std::placeholders::_1);
		m_pScene->GetCollision()->Raycast(hitResult_LB, this, CollisionChannel::Raycast, Start_LB, Vector2(0, 1), m_vPrevPos.y - m_vPos.y -10.f);

		// ������ �Ʒ� üũ
		HitResult hitResult_RB;
		Vector2 Start_RB = m_vPos;
		Start_RB.x += m_vSize.x * (1 - m_vPivot.x);
		Start_RB.y += 5.f;

		hitResult_RB.Callback = std::bind(&CMoveableObject::GroundWalkRightBottom, this, std::placeholders::_1);
		m_pScene->GetCollision()->Raycast(hitResult_RB, this, CollisionChannel::Raycast, Start_RB, Vector2(0, 1), m_vPrevPos.y - m_vPos.y - 10.f);
	}
}

void CMoveableObject::GroundWalkLeftBottom(const HitResult& hitResult)
{
	// ���������� ���������� �Ǵ� �ϰ� �������̶�� return.
	m_hitResult = hitResult;

	if (hitResult.hitCollider->GetCollisionProfile()->Name == "Map")
	{
		CColliderLine* pLine = Cast<CColliderLine*, CCollider*>(hitResult.hitCollider);

		if (pLine && pLine->GetStartPoint().GetAngle(pLine->GetEndPoint()) >= 55)
		{
			if ((int)pLine->GetStartPoint().GetAngle(pLine->GetEndPoint()) % 180!= 0)
			return;
		}
		JumpEnd();
		m_vMove.y += m_vPos.y - hitResult.hitPoint.y;
		m_vPos.y = hitResult.hitPoint.y;

		m_bHit = true;
	}
}

void CMoveableObject::GroundWalkRightBottom(const HitResult& hitResult)
{
	// ���������� ���������� �Ǵ� �ϰ� �������̶�� return.

	if (m_bHit)
		return;

	m_hitResult = hitResult;

	if (hitResult.hitCollider->GetCollisionProfile()->Name == "Map")
	{
		CColliderLine* pLine = Cast<CColliderLine*, CCollider*>(hitResult.hitCollider);

		if (pLine && pLine->GetStartPoint().GetAngle(pLine->GetEndPoint()) <= 305)
		{
			if((int)pLine->GetStartPoint().GetAngle(pLine->GetEndPoint()) % 180 != 0)
			return;
		}
		JumpEnd();
		m_vMove.y += m_vPos.y - hitResult.hitPoint.y;
		m_vPos.y = hitResult.hitPoint.y;

		m_bHit = true;
	}
}


void CMoveableObject::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);


	m_vPrevPos = m_vPos;
	m_vMove = Vector2(0.f, 0.f);
}