#include "stdafx.h"
#include "MoveBoardBase.h"

void MoveBoardBase::PushPlayer()
{
	//プレイヤーと離れすぎているなら処理をしないようにする。
	float lenRes = max(aabb[0].Length(), aabb[1].Length());
	lenRes += 100.0f;
	CVector3 diff = m_position - m_player->GetPosition();
	float toLen = diff.Length();
	if (toLen > lenRes) {
		return;		//離れすぎている。
	}

	//平行移動行列と回転行列。
	CMatrix mTrans, mRot;
	mTrans.MakeTranslation(m_position);
	mRot.MakeRotationFromQuaternion(m_rotation);
	//箱のワールド行列を作成。
	CMatrix mWorld;
	mWorld = mRot * mTrans;
	//箱のワールド行列と回転行列の逆行列を作成。
	CMatrix mWorldInv, mRotInv;
	mWorldInv.Inverse(mWorld);
	mRotInv.Inverse(mRot);

	const float w = 20.0f;	//プレイヤーの幅。
	const float playerHigh = 120.0f;	//プレイヤーの高さ。
	//プレイヤーの座標を箱の逆行列を使って変換。
	CVector3 playerPos = m_player->GetPosition();
	mWorldInv.Apply(playerPos);
	//箱からプレイヤーまでの箱座標系のベクトル。
	CVector3 toPlayerXZ = playerPos;
	toPlayerXZ.y = 0.0f;
	//float len = toPlayerXZ.Length();
	//移動方向を箱座標系に変換。
	CVector3 vMove = m_moveSpeed;
	mRotInv.Apply(vMove);
	//移動方向とプレイヤーまでのベクトルの角度?を求める。
	toPlayerXZ.Normalize();
	vMove.Normalize();
	float dir = vMove.Dot(toPlayerXZ);

	//オブジェクトの原点から前面の頂点の角度をとる。
	CVector3 toVertex = aabb[0];
	toVertex.y = 0.0f;
	//float toVertexLen = max(toVertex.x, toVertex.z);
	toVertex.Normalize();
	//float dirVertex = toVertex.Dot(vMove);	//進行方向と頂点までのベクトルの角度をとる。
	//if (dirVertex < 0.0f) {
	//	toVertex *= -1.0f;		//方向を反転。
	//}
	float angle = vMove.Dot(toVertex);		//進行方向と頂点の角度。

	//aabbの判定をとる。
	if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z		//X座標とZ座標の範囲内に
		&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z		//プレイヤーがいるか
		&& playerPos.y < aabb[1].y && playerPos.y + playerHigh > aabb[0].y
		/*&& dir > angle*/) {
		if (playerPos.y > aabb[0].y	//プレイヤーが上にいるか。
			&& m_player->IsPlayerOnGround() == true) {
			//上に乗ってる時の処理
			m_player->PlayerMove(m_moveSpeed);
		}
		else if (dir > angle){		//移動方向側にプレイヤーがいるか
			//ボックスより外にいるか。
			//押し出しの処理。
			m_player->PlayerMove(m_moveSpeed);
			//プレイヤーの座標を箱の逆行列を使って変換。
			playerPos = m_player->GetPosition();
			mWorldInv.Apply(playerPos);
			if (aabb[0].x < playerPos.x && aabb[0].z < playerPos.z		//X座標とZ座標の範囲内に
				&& aabb[1].x > playerPos.x && aabb[1].z > playerPos.z		//プレイヤーがいるか
				&& playerPos.y < aabb[0].y) {	//プレイヤーが下にいるか。
				//下からぶつかったときの処理
				CVector3 playerMove = m_player->GetMoveSpeed();
				playerMove.y = min(0.0f, playerMove.y);
				m_player->SetMoveSpeed(playerMove);
			}
		}
		else if (playerPos.y < aabb[0].y) {	//プレイヤーが下にいるか。
			//下からぶつかったときの処理
			CVector3 playerMove = m_player->GetMoveSpeed();
			playerMove.y = min(0.0f, playerMove.y);
			m_player->SetMoveSpeed(playerMove);
		}
	}
}
