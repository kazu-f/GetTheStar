#pragma once
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void OnDestroy();
	/*
	座標を取得。
	return 座標。
	*/
	const CVector3& GetPosition() const
	{
		return m_position;
	};	
	/*
	プレイヤーの前方向を取得。
	*/
	const CVector3& GetPlayerFront() const
	{
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(m_rotation);
		CVector3 m_front;
		m_front.Set(mRot.m[2][0], mRot.m[2][1], mRot.m[2][2]);
		m_front.Normalize();

		return m_front;
	};
	/*
	移動速度を取得。
	return 移動速度。
	*/
	const CVector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/*
	キャラコンを取得。
	return キャラコン。
	*/
	CCharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
	/*
	座標を設定。
	*/
	void SetPosition(const CVector3& position)
	{
		m_position = position;
		m_charaCon.SetPosition(m_position);
	};
	/*
	移動速度を設定。
	*/
	void SetMoveSpeed(const CVector3& moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}
	/*
	地面にいるかどうか。
	*/
	bool IsPlayerOnGround() const
	{
		return m_charaCon.IsOnGround();
	};
	/*
	ゲームクリア判定。
	*/
	bool IsGameClear() const
	{
		return isClear;
	}
	/*
	プレイヤーをキャラコンで動かす。
	引数	移動速度。
	*/
	void PlayerMove(CVector3& moveSpeed)
	{
		m_position = m_charaCon.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
		m_skinModelRender->SetPosition(m_position);
	};
	/*
	プレイヤーにダメージを与える。
	*/
	void Damage(CVector3& moveSpeed);
	/*
	クリア時のアニメーションをさせる。
	*/
	void GameClear()
	{
		m_skinModelRender->SetUpdateAnimationFlag(false);
		m_skinModelRender->PlayAnimation(enAnimationClip_clear);
		isClear = true;
	}
	
private:
	void Move_Jump();
	void Turn();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダラー。
	CVector3 m_position;	//座標。
	CVector3 m_moveSpeed = CVector3::Zero;	//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
	CCharacterController m_charaCon;	//キャラコン。
	float jumpTimer = 0.0f;				//ジャンプのタイマー。
	bool isJump = false;				//ジャンプフラグ。
	const float m_speed = 600.0f;		//プレイヤーの速度。
	const float gravitation = 980.0f;	//重力。
	const float maxDropSpeed = -2560.0f;//最大落下速度。
	prefab::CSpriteRender* m_hpBarSprite = nullptr;	//HPバーのスプライト。
	prefab::CSpriteRender* m_hpSprite = nullptr;	//HPのスプライト。
	CVector3 m_hpScale = CVector3::One;				//HPのスケール。
	int m_HP = 3;				//プレイヤーのHP。
	float hitStop = 0.0f;		//ダメージを受けた後のヒットストップと無敵時間。
	bool isDeath = false;		//死亡フラグ。
	bool isClear = false;		//クリアフラグ。

	enum EnAnimationClip {
		enAnimationClip_idle,	//待機アニメーション。
		enAnimationClip_run,	//走りアニメーション。
		enAnimationClip_jump,	//ジャンプアニメーション。
		enAnimationClip_damage,	//ダメージを受けたときのアニメーション?。
		enAnimationClip_down,	//ゲームオーバーのアニメーション。
		enAnimationClip_clear,	//ゲームクリアのアニメーション。
		enAnimationClip_Num,	//アニメーションクリップの数。
	};
	CAnimationClip m_animClips[enAnimationClip_Num];
};

