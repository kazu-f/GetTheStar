#pragma once
class BackGround : public IGameObject
{
public:
	//メンバ関数
	BackGround();
	~BackGround();
	bool Start();
	//void Update();
	void OnDestroy();

	//メンバ変数
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー
	CPhysicsStaticObject m_phyStaticObject;						//静的物理オブジェクト
	//CVector3 m_position = CVector3::Zero;						//座標
	//CVector3 scale = CVector3::One;								//スケール
	//CQuaternion m_rotation = CQuaternion::Identity;			//回転。
};

