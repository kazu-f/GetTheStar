#include "stdafx.h"
#include "JumpStand.h"


JumpStand::JumpStand()
{
}

JumpStand::~JumpStand()
{
}

bool JumpStand::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/JumpStand.cmo");
	
	m_skinModelRender->SetPosition(m_position);	//座標
	m_skinModelRender->SetScale(m_scale);		//スケール。
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetShadowReceiverFlag(true);

	////回転の設定。
	//m_rotation.SetRotationDeg(CVector3::AxisX, 90.0f);
	//m_skinModelRender->SetRotation(m_rotation);

	//静的物理オブジェクトの作成
	m_phyStaticObject.CreateMesh(m_position, m_rotation, m_scale, m_skinModelRender);

	////ボックス形状のゴーストを作成。
	//CVector3 ghostPos = m_position;
	//ghostPos.y += 40.0f;
	//m_ghostObject.CreateBox(
	//	ghostPos,			//座標。
	//	CQuaternion::Identity,			//回転。
	//	{ 200.0f,boxHigh,200.0f }		//ボックスのサイズ。x,y,z
	//);

	const float boxX = 200.0f;	//ボックスのx。
	const float boxZ = 200.0f;	//ボックスのz。
	const float boxHigh = 90.0f;//ボックスのy。
	//aabbの設定。
	aabb[0] = m_position;
	aabb[0].x += boxX / -2.0f * m_scale.x;
	aabb[0].z += boxZ / -2.0f * m_scale.z;
	
	aabb[1] = m_position;
	aabb[1].x += boxX / 2.0f * m_scale.x;
	aabb[1].y += boxHigh * m_scale.y;
	aabb[1].z += boxZ / 2.0f * m_scale.z;
	


	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("Player");


	return true;
}

void JumpStand::Update()
{
	if (m_player != nullptr) {
		//PhysicsWorld().ContactTest(m_player->GetCharaCon(), [&](const btCollisionObject& contactObject) {
		//	if (m_ghostObject.IsSelf(contactObject) == true) 
		//	{
		//		CVector3 playerMoveSpeed = m_player->GetMoveSpeed();
		//		//CVector3 playerPos = m_player->GetPosition();
		//		if (m_player->IsPlayerOnGround()) 
		//		{
		//			playerMoveSpeed.y = jumpSpeed;
		//			m_player->SetMoveSpeed(playerMoveSpeed);
		//			m_player->PlayerMove(playerMoveSpeed);
		//		}
		//		else
		//		{
		//			playerMoveSpeed.y = min(0.0f, playerMoveSpeed.y);
		//			m_player->SetMoveSpeed(playerMoveSpeed);
		//			m_player->PlayerMove(playerMoveSpeed);
		//		}
		//		
		//	}
		//	});

		//プレイヤーの座標を取得。
		CVector3 playerPos = m_player->GetPosition();
		const float playerHigh = 120.0f;	//プレイヤーの高さ。
		const float w = 20.0f;	//プレイヤーの幅。
		if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z			//X座標とZ座標の範囲内に
			&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z		//プレイヤーがいるか
			&& aabb[0].y < playerPos.y + playerHigh && aabb[1].y > playerPos.y)
		{
			CVector3 playerMoveSpeed = m_player->GetMoveSpeed();
			if (m_player->IsPlayerOnGround()		//地面についているかどうか。
				&& playerPos.y > aabb[1].y * 0.8f)	//ボックスの上面にいるかどうか。
			{
				//ジャンプさせる。
				playerMoveSpeed.y = jumpSpeed;
				m_player->SetMoveSpeed(playerMoveSpeed);
				m_player->PlayerMove(playerMoveSpeed);
			}
			else if (playerPos.y < aabb[0].y)
			{
				//下から抜けてこないようにする。
				playerMoveSpeed.y = min(0.0f, playerMoveSpeed.y);
				m_player->SetMoveSpeed(playerMoveSpeed);
				m_player->PlayerMove(playerMoveSpeed);
			}
		}

		if (m_player->IsDead()) {
			m_player = nullptr;
		}

	}
}

void JumpStand::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
