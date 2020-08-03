#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	//アニメーションクリップの読み込み。
	m_animClips[enAnimationClip_idle].Load(L"animData/unityChan/idle.tka");
	m_animClips[enAnimationClip_run].Load(L"animData/unityChan/run.tka");
	m_animClips[enAnimationClip_jump].Load(L"animData/unityChan/jump.tka");
	m_animClips[enAnimationClip_damage].Load(L"animData/unityChan/damage.tka");
	m_animClips[enAnimationClip_down].Load(L"animData/unityChan/KneelDown.tka");
	m_animClips[enAnimationClip_clear].Load(L"animData/unityChan/clear.tka");

	m_animClips[enAnimationClip_idle].SetLoopFlag(false);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);
	m_animClips[enAnimationClip_jump].SetLoopFlag(false);
	m_animClips[enAnimationClip_damage].SetLoopFlag(false);
	m_animClips[enAnimationClip_down].SetLoopFlag(false);
	m_animClips[enAnimationClip_clear].SetLoopFlag(false);

	//プレイヤーのモデルデータ読み込み。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo", m_animClips, enAnimationClip_Num, enFbxUpAxisY);
	m_skinModelRender->SetShadowCasterFlag(true);

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle);

	//キャラコンの設定。
	m_charaCon.Init(
		20.0f,		//幅
		80.0f,		//高さ
		m_position	//座標
	);

	//HPのUIの設定。
	CVector2 hp_pivot = { 1.0f,1.0f };		//HP用のピボット。
	CVector3 hpPos = { 640.0f, 360.0f, 0.0f };	//HPの座標。
	//HPバー。
	m_hpBarSprite = NewGO<prefab::CSpriteRender>(0);
	m_hpBarSprite->Init(L"sprite/Player_HP_Bar.dds", 480.0f, 48.0f);
	m_hpBarSprite->SetScale(CVector3::One);
	m_hpBarSprite->SetPivot(hp_pivot);
	m_hpBarSprite->SetPosition({ 640.0f,360.0f,0.0f });
	//HP。
	m_hpSprite = NewGO<prefab::CSpriteRender>(0);
	m_hpSprite->Init(L"sprite/Player_HP.dds", 440.0f, 28.0f);
	m_hpSprite->SetScale(m_hpScale);
	m_hpSprite->SetPivot(hp_pivot);
	m_hpSprite->SetPosition({ 620.0f,350.0f,0.0f });

	return true;
}

void Player::Update()
{	
	if (!isClear) {
		hitStop -= GameTime().GetFrameDeltaTime();
		if (m_HP > 0 && hitStop < 0.0f) {
			Move_Jump();//移動処理など。
			Turn();

			if (m_charaCon.IsOnGround()
				&& (fabsf(m_moveSpeed.x) < 0.001f
					&& fabsf(m_moveSpeed.z) < 0.001f)) {
				//地上で動きがないため待機モーション。
				m_skinModelRender->PlayAnimation(enAnimationClip_idle);
			}
			else if (m_charaCon.IsOnGround()) {
				//地上で移動しているため移動モーション。
				m_skinModelRender->PlayAnimation(enAnimationClip_run);
			}
			else
			{
				//それ以外ならジャンプモーション。
				m_skinModelRender->PlayAnimation(enAnimationClip_jump);
			}

		}
		else if (m_HP > 0 && hitStop > 0.0f) {
			//ノックバックの減衰。
			m_moveSpeed.x -= m_moveSpeed.x * 5.0f * GameTime().GetFrameDeltaTime();
			m_moveSpeed.z -= m_moveSpeed.z * 5.0f * GameTime().GetFrameDeltaTime();
			//落下処理。
			m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
			//落下速度の上限。
			m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);
		}
		if (m_HP <= 0 && !isDeath) {
			//アニメーションの再生。
			m_skinModelRender->SetUpdateAnimationFlag(false);
			m_skinModelRender->PlayAnimation(enAnimationClip_down);
			isDeath = true;		//死亡フラグを立てる。
		}
		else if (!m_skinModelRender->IsPlayingAnimation() && isDeath) {
			DeleteGO(this);		//死亡アニメーションが終わったら自身を削除。
		}

		//キャラコンを使用して座標を更新。
		m_position = m_charaCon.Execute(m_moveSpeed);

		if (m_position.y < -2000.0f) {
			DeleteGO(this);		//落ちたら死亡。
		}
	}
	//else if (isClear && m_skinModelRender->IsPlayingAnimation()) {

	//}
	//スキンモデルレンダーに座標と回転を反映。
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
}

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_hpBarSprite);
	DeleteGO(m_hpSprite);
}

void Player::Damage(CVector3& moveSpeed)
{
	if (hitStop > 0.0f) {
		return;
	}
	m_HP--;
	hitStop = 0.8f;
	//スプライトのスケールを減らす。
	m_hpScale.x -= 1.0f / 3.0f;
	m_hpSprite->SetScale(m_hpScale);
	if (m_HP > 0) {
		//ノックバック。
		CVector3 damege = moveSpeed;
		damege.y += 50.0f;
		SetMoveSpeed(damege);
		//アニメーション。
		m_skinModelRender->SetUpdateAnimationFlag(false);
		m_skinModelRender->PlayAnimation(enAnimationClip_damage);
	}
	else {
		SetMoveSpeed(CVector3::Zero);
	}
}

void Player::Move_Jump()
{
	//左スティック入力を受け取る。
	float x = Pad(0).GetLStickXF();
	float y = Pad(0).GetLStickYF();
	//カメラの前方向と右方向を取得。
	CVector3 cameraForward = MainCamera().GetForward();	//カメラの前方向。
	CVector3 cameraRight = MainCamera().GetRight();	//カメラの右方向。
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	////XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed += cameraForward * y * m_speed;	//奥方向への移動速度を加算。
	m_moveSpeed += cameraRight * x * m_speed;		//右方向への移動速度を加算。

	if (isnan(m_moveSpeed.Length())) {	//非数回避。カメラのほうで治せたらいいんだけど...
		m_moveSpeed = CVector3::Zero;
	}

	//ジャンプの処理。
	const float jumpSpeed = 640.0f;		//ジャンプスピード。
	const float jumpAccele = 30.0f;		//ジャンプの加速度。
	const float minJumpSpeed = 500.0f;	//ジャンプの最低速度。
	//地面にいるときのみジャンプが可能。
	if (Pad(0).IsTrigger(enButtonA)
		&& m_charaCon.IsOnGround()) {
		isJump = true;		//Aボタンが押されたらジャンプフラグを立てる。
		m_moveSpeed.y = minJumpSpeed;	//初速度。
	}
	//ジャンプフラグが立った時の処理。
	if (Pad(0).IsPress(enButtonA)
		&& isJump && jumpTimer < 0.2f) {
		/*m_moveSpeed.y += jumpSpeed / 0.1f * GameTime().GetFrameDeltaTime();*/		//加速しすぎ。
		if (jumpTimer > 0.08f) {
			m_moveSpeed.y = max(minJumpSpeed, min(m_moveSpeed.y += jumpAccele, jumpSpeed));	//一番マシ？
			//m_moveSpeed.y = jumpSpeed * (1.0f - jumpTimer * 2.0f);	//これはダメそう。
			//m_moveSpeed.y = jumpSpeed / 2.0f + jumpSpeed * jumpTimer * 2.0f;	//違和感がやばい。
			//m_moveSpeed.y = jumpSpeed;	//押し加減で差が出にくい。
		}
		jumpTimer += GameTime().GetFrameDeltaTime();
	}
	else {
		//Aボタンが離されたらジャンプフラグを消す。
		isJump = false;
		jumpTimer = 0.0f;
	}

	//重力処理。
	m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
	//落下速度の上限。
	m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);


	return;
}

void Player::Turn()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f)
	{
		//動きがないときには処理を行わない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotation(CVector3::AxisY, angle);
}
