#pragma once
#include "Player.h"

class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void OnDestroy();

	///*
	//座標を取得。
	//return 座標。
	//*/
	//const CVector3& GetPosition() const
	//{
	//	return m_position;
	//};
	///*
	//移動速度を取得。
	//return 移動速度。
	//*/
	//const CVector3& GetMoveSpeed() const
	//{
	//	return m_moveSpeed;
	//}
	///*
	//キャラコンを取得。
	//return キャラコン。
	//*/
	//CCharacterController& GetCharaCon()
	//{
	//	return m_charaCon;
	//}

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




private:
	void AxisUpdate();		//クォータニオンから方向を決める。
	void EnemyRotation(const CVector3& vecMove, float angleSpeed);	//エネミーの回転をする。
	void EnemyMove(const CVector3& vecMove);	//エネミーの移動。
	void Steped();		//プレイヤーに踏まれた時の処理。

	//メンバ変数。
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダー。
	CVector3 m_position = CVector3::Zero;	//座標。
	CVector3 m_firstPosition = CVector3::Zero;	//初期座標。
	CVector3 m_moveSpeed = CVector3::Zero;	//移動速度。
	CVector3 m_right = CVector3::Right;		//エネミーの右方向。
	CVector3 m_up = CVector3::Up;			//エネミーの上方向。
	CVector3 m_forward = CVector3::Front;	//エネミーの前方向。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
	CCharacterController m_charaCon;	//キャラコン。

	Player* m_player = nullptr;	//プレイヤー。

	float m_speed = 0.0f;		//速度。
	const float MAXspeed = 400.0f;		//最高速度。
	const float accele = 80.0f;			//加速度。
	const float defaultSpeed = 200.0f;	//デフォルトスピード。
	const float decele = 100.0f;		//減速度。
	const float gravitation = 980.0f;	//重力。
	const float maxDropSpeed = -2560.0f;//最大落下速度。
	float m_timer = 0.0f;
};

