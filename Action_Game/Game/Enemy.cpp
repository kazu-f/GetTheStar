#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

bool Enemy::Start()
{
	//スキンモデルレンダーの設定。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Enemy.cmo");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetShadowCasterFlag(true);
	//キャラコンの設定。
	m_charaCon.Init(
		30.0f,		//半径。
		10.0f,		//高さ。
		m_position	//座標。
	);
	
	//初期座標の保存。
	m_firstPosition = m_position;

	//プレイヤーのインスタンスを検索。
	m_player = FindGO<Player>("Player");

	return true;
}

void Enemy::Update()
{
	m_timer -= GameTime().GetFrameDeltaTime();

	AxisUpdate();
	if (m_player != nullptr) {
		CVector3 toPlayer = m_player->GetPosition() - m_position;	//エネミーからプレイヤーまでのベクトル。
		CVector3 toFirstPos = m_firstPosition - m_position;		//エネミーの現在座標から初期座標までのベクトル。
		if (m_timer < 0.0f) {
			if (toPlayer.Length() < 500.0f) {	//プレイヤーとの距離が500.0fを下回ったら、
				EnemyRotation(toPlayer, 2.0f * GameTime().GetFrameDeltaTime());
				AxisUpdate();
				EnemyMove(toPlayer);
			}
			else if (toFirstPos.Length() > 5.0f) {//初期座標から離れていたら、
				//プレイヤーを見失った時の処理。
				EnemyRotation(toFirstPos, 1.0f);
				AxisUpdate();
				m_speed = defaultSpeed;
				//移動速度を出す。
				m_moveSpeed.x = m_forward.x * m_speed;
				m_moveSpeed.z = m_forward.z * m_speed;
				m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
				//落下速度の上限。
				m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);
				m_position = m_charaCon.Execute(m_moveSpeed);		//移動の処理。
			}

		}
		else {
			//ノックバックの速度の減衰。
			m_moveSpeed.x -= m_moveSpeed.x * 5.0f * GameTime().GetFrameDeltaTime();
			m_moveSpeed.z -= m_moveSpeed.z * 5.0f * GameTime().GetFrameDeltaTime();
			m_position = m_charaCon.Execute(m_moveSpeed);
			if (toPlayer.y > 10.0f && toPlayer.Length() < 55.0f) {
				Steped();
				DeleteGO(this);
			}
		}

		if (m_player->IsDead()) {	//プレイヤーの死亡時の処理。
			m_player = nullptr;
		}

	}

	if (m_position.y < -2000.0f) {
		DeleteGO(this);		//落ちたら死亡。
	}

	//スキンモデルレンダーの更新。
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);

}

void Enemy::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}


//回転から方向ベクトルを作成。
void Enemy::AxisUpdate()
{
	CMatrix mRot;
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_right.Set(mRot.m[0][0], mRot.m[0][1], mRot.m[0][2]);
	m_up.Set(mRot.m[1][0], mRot.m[1][1], mRot.m[1][2]);
	m_forward.Set(mRot.m[2][0], mRot.m[2][1], mRot.m[2][2]);
	m_right.Normalize();
	m_up.Normalize();
	m_forward.Normalize();
}

//エネミーをXZ平面上に移動方向に向かって回転させる。
//CVector3 vecMove	移動方向	CVector3型
//Normalizeは不必要。
//float angleSpeed	回転速度。
void Enemy::EnemyRotation(const CVector3& vecMove, float angleSpeed)
{
	//ベクトルをXZ平面上にして正規化。
	CVector3 vMove = vecMove;
	vMove.y = 0.0f;
	if (vMove.Length() < 40.0f) {
		return;
	}
	vMove.Normalize();
	//回転量を求める。
	float angle = vMove.Dot(m_forward);
	angle = min(1.0f, max(-1.0f, angle));

	CQuaternion qRot;

	//なんかうまくいかない
	angle = acosf(angle);	//こいつが抜けてるせいか...
							//あくまで内積で出るのはcosθの値で、
							//ラジアン単位の角度なわけではないってことやな...

	//Degree角に変換。
	float deg = CMath::RadToDeg(angle);

	if (deg > 60.0f) {
		m_speed -= decele * GameTime().GetFrameDeltaTime();
		deg = max(60.0f, deg);
	}


	//回転の向きを決める。
	float dir = vMove.Dot(m_right);
	if (dir < 0) {
		deg *= -1.0f;
	}

	qRot.SetRotationDeg(CVector3::AxisY, deg * angleSpeed);
	m_rotation.Multiply(qRot);
}

void Enemy::EnemyMove(const CVector3& vecMove)
{
	CVector3 toFirstPos = m_firstPosition - m_position;		//エネミーの現在座標から初期座標までのベクトル。
	//移動速度を求める。
	m_speed = min(MAXspeed, max(defaultSpeed, m_speed + (accele * GameTime().GetFrameDeltaTime())));
	m_moveSpeed.x = m_forward.x * m_speed;
	m_moveSpeed.z = m_forward.z * m_speed;
	m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
	//落下速度の上限。
	m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);

	CVector3 oldPos = m_position;		//古い座標を保持。
	m_position = m_charaCon.Execute(m_moveSpeed);	//	移動処理。
	//初期座標から離れすぎた場合に戻す。
	toFirstPos = m_firstPosition - m_position;
	if (toFirstPos.Length() > 1000.0f) {
		m_position = oldPos;
		m_charaCon.SetPosition(m_position);
	}
	if (!m_charaCon.IsOnGround()) {		//落ちないように戻す。
		CVector3 vReturn = oldPos - m_position;
		vReturn.y = 0.0f;
		vReturn.Normalize();
		m_position = oldPos;
		m_charaCon.SetPosition(m_position);
		m_charaCon.Execute(vReturn * defaultSpeed);
	}
	if (vecMove.Length() < 55.0f) {	//プレイヤーと接触したら、
		if (vecMove.y > 10.0f) {
			Steped();
			DeleteGO(this);
		}
		else {
			//ノックバック。
			m_player->Damage(m_moveSpeed);
			m_moveSpeed.x *= -1.0f;
			m_moveSpeed.z *= -1.0f;
			//speedとtimerの設定。
			m_speed = 0.0f;
			m_timer = 3.0f;
		}
	}
}

void Enemy::Steped()
{
	CVector3 upSpeed = m_player->GetMoveSpeed();
	upSpeed.y = max(200.0f, upSpeed.y + 200.0f);
	m_player->SetMoveSpeed(upSpeed);
}
