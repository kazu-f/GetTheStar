#pragma once
#include"Player.h"

class MoveBoardBase : public IGameObject
{
public:	
	//座標を設定。
	void SetPosition(const CVector3& position) 
	{
		m_position = position;
	}
	//回転を設定。
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}
	//速度の設定。
	//引数　float型。
	void SetSpeed(float speed)
	{
		m_speed = speed;
	}
	//タイマーの設定。
	void SetTimer(float time)
	{
		m_timer = time;
	}
	//タイマーの上限の設定。
	void SetTimeLimit(float time)
	{
		timeLimit = time;
	}


protected:
	//回転から方向ベクトルを作成。
	void AxisUpdate()
	{
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(m_rotation);
		m_right = { mRot.m[0][0],mRot.m[0][1], mRot.m[0][2] };
		m_up = { mRot.m[1][0],mRot.m[1][1], mRot.m[1][2] };
		m_front = { mRot.m[2][0],mRot.m[2][1], mRot.m[2][2] };
		//正規化。
		m_right.Normalize();
		m_up.Normalize();
		m_front.Normalize();
	}
	void PushPlayer();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_scale = CVector3::One;	//スケール。
	CVector3 m_position = CVector3::Zero;	//座標。
	CVector3 firstPos = CVector3::Zero;	//初期座標。
	CVector3 m_moveSpeed = CVector3::Zero;	//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
	CVector3 m_front = CVector3::Front;	//前方向。
	CVector3 m_right = CVector3::Right;	//右方向。
	CVector3 m_up = CVector3::Up;	//上方向。
	CVector3 aabb[2] = { CVector3::Zero,CVector3::Zero };	//aabb 0が小さく、1が大きい。
	//CQuaternion m_direction = CQuaternion::Identity;	//方向。
	float m_timer = 0.0f;		//タイマー。
	float timeLimit = 0.0f;		//タイマーの上限。
	float m_speed = 0.0f;	//速度。
	CPhysicsStaticObject m_phyStaticObject;	//静的物理オブジェクト。
	Player* m_player = nullptr;		//プレイヤー。
};

