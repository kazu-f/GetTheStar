#include "stdafx.h"
#include "MoveBoard.h"

MoveBoard::MoveBoard()
{
}

MoveBoard::~MoveBoard()
{
}

bool MoveBoard::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/MoveBoard.cmo");

	//回転の設定。
	m_skinModelRender->SetRotation(m_rotation);

	m_phyStaticObject.CreateMesh(m_position, m_rotation, m_scale, m_skinModelRender);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetShadowReceiverFlag(true);

	//aabbの設定。
	const float boxX = 100.0f;
	const float boxY = 90.0f;
	const float boxZ = 100.0f;
	aabb[0].x -= boxX;
	aabb[0].z -= boxZ;
	aabb[1].x += boxX;
	aabb[1].z += boxZ;
	aabb[1].y += boxY;

	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("Player");

	timeLimit = 5.0f;
	m_timer = timeLimit / 2.0f;	//タイムリミットの半分を代入。
	AxisUpdate();

	return true;
}

void MoveBoard::Update()
{
	Move();
	if (m_player != nullptr) {
		PushPlayer();
		if (m_player->IsDead()) {
			m_player = nullptr;
		}
	}
}

void MoveBoard::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

//移動処理。
void MoveBoard::Move()
{
	m_timer += GameTime().GetFrameDeltaTime();
	if (m_timer > timeLimit) {
		//m_speed *= -1.00f;
		CQuaternion qRot;
		qRot.SetRotationDeg(m_up, 180.0f);
		m_rotation.Multiply(qRot);
		AxisUpdate();
		m_timer = 0.00f;
	}

	//移動速度の設定。
	m_moveSpeed = m_front * m_speed;
	//移動処理。
	m_position += m_moveSpeed * GameTime().GetFrameDeltaTime();
	m_phyStaticObject.SetPositionAndRotation(m_position, m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

//void MoveBoard::PushPlayer()
//{
//	//平行移動行列と回転行列。
//	CMatrix mTrans, mRot;
//	mTrans.MakeTranslation(m_position);
//	mRot.MakeRotationFromQuaternion(m_rotation);
//	//箱のワールド行列を作成。
//	CMatrix mWorld;
//	mWorld = mRot * mTrans;
//	//箱のワールド行列と回転行列の逆行列を作成。
//	CMatrix mWorldInv, mRotInv;
//	mWorldInv.Inverse(mWorld);
//	mRotInv.Inverse(mRot);
//
//	const float w = 20.0f;	//プレイヤーの幅。
//	const float playerHigh = 120.0f;	//プレイヤーの高さ。
//	//プレイヤーの座標を箱の逆行列を使って変換。
//	CVector3 playerPos = m_player->GetPosition();
//	mWorldInv.Apply(playerPos);
//	//箱からプレイヤーまでの箱座標系のベクトル。
//	CVector3 toPlayerXZ = playerPos;
//	toPlayerXZ.y = 0.0f;
//	//移動方向を箱座標系に変換。
//	CVector3 vMove = m_moveSpeed;
//	mRotInv.Apply(vMove);
//	//移動方向とプレイヤーまでのベクトルの角度?を求める。
//	toPlayerXZ.Normalize();
//	vMove.Normalize();
//	float dir = vMove.Dot(toPlayerXZ);
//
//	//aabbの判定をとる。
//	if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z		//X座標とZ座標の範囲内に
//		&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z		//プレイヤーがいるか
//		&& playerPos.y < aabb[1].y && playerPos.y + playerHigh > aabb[0].y
//		&& dir > 0.7f) {	//移動方向側にプレイヤーがいるか
//		//押し出しの処理。
//		m_player->PlayerMove(m_moveSpeed);
//		
//		//プレイヤーの座標を箱の逆行列を使って変換。
//		playerPos = m_player->GetPosition();
//		mWorldInv.Apply(playerPos);
//		if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z
//			&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z) {
//			//下からぶつかったときの処理
//			CVector3 playerMove = m_player->GetMoveSpeed();
//			playerMove.y = min(0.0f, playerMove.y);
//			m_player->SetMoveSpeed(playerMove);
//		}
//	}
//	else if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z	//X座標とZ座標の範囲内に
//		&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z) 	//プレイヤーがいるか
//	{
//		if (playerPos.y < aabb[1].y && playerPos.y > aabb[0].y	//プレイヤーが上にいるか。
//			&& m_player->IsPlayerOnGround() == true) {			//地面についているか。
//		//上に乗ってる時の処理
//			m_player->PlayerMove(m_moveSpeed);
//		}
//		else if (playerPos.y + playerHigh > aabb[0].y
//			&& playerPos.y < aabb[0].y) {
//			//下からぶつかったときの処理
//			CVector3 playerMove = m_player->GetMoveSpeed();
//			playerMove.y = min(0.0f, playerMove.y);
//			m_player->SetMoveSpeed(playerMove);
//		}
//	}
//}
