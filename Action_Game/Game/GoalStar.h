#pragma once
#include "Player.h"

class GoalStar : public IGameObject
{
public:
	GoalStar();
	~GoalStar();
	bool Start();
	void Update();
	void OnDestroy();

	/*
	座標の設定。
	*/
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	};
	/*
	回転の設定。
	*/
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}


private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダー。
	Player* m_player = nullptr;		//プレイヤー。
	CVector3 m_position = CVector3::Zero;		//座標。
	CVector3 m_scale = CVector3::One;		//スケール。
	//CVector3 m_moveSpeed = CVector3::Zero;		//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity;		//回転。
	float m_firstPosy = 0.0f;		//初期のY座標。
	float m_timer = 0.0f;		//タイマー。
	//float m_ySpeed = 0.5f;		//ふわふわする速度。
	//float m_speed = 4.0f;		//基本速度。
	//bool m_floatingFlag = false;	//浮遊フラグ。
};

