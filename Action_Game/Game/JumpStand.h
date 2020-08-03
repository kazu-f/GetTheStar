#pragma once
#include "Player.h"
//#include "tkEngine/physics/tkPhysicsGhostObject.h"


class JumpStand : public IGameObject
{
public:
	JumpStand();
	~JumpStand();
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
	/*
	スケールの設定。
	*/
	void SetScale(const CVector3& scale) 
	{
		m_scale = scale;
	}

private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー。
	CPhysicsStaticObject m_phyStaticObject;				//静的物理オブジェクト。
	CVector3 m_position = CVector3::Zero;			//座標。
	CVector3 m_scale = CVector3::One;				//スケール。
	CQuaternion m_rotation = CQuaternion::Identity;		//回転。
	Player* m_player;		//プレイヤー。
	//CPhysicsGhostObject m_ghostObject;		//ゴースト。なんか重くなるから却下。
	CVector3 aabb[2] = { CVector3::Zero,CVector3::Zero };	//aabb。0が小さく、1が大きい。
	//float jumpSpeed = 1920.0f;
	const float jumpSpeed = 980.0f;
};

