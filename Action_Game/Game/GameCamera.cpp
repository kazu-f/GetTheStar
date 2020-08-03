#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("Player");

	//カメラを設定。
	//視点から注視点までのベクトル。
	toPos.Set(0.0f, 50.0f, 500.0f);
	//MainCamera().SetPosition(CVector3::Zero);
	//m_springCamera.SetPosition(CVector3::Zero);

	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);

	//ばねカメラの初期化。
	m_springCamera.Init(
		MainCamera(),		//ばねカメラの処理を行うカメラを指定する。
		2000.0f,			//カメラの移動速度の最大値。1000.0だと回転に間に合わんかったわ。
		true,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		5.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	return true;
}

void GameCamera::Update()
{
	if (m_player != nullptr){

		if (!m_player->IsGameClear()) {
			if (Pad(0).IsTrigger(enButtonLB1)) {
				CVector3 dir = m_player->GetPlayerFront();
				dir *= -1.0f;
				MoveFront(dir);
			}
			else { 
				MoveCamera(); 
			}

		}
		else {
			CVector3 dir = m_player->GetPlayerFront();
			MoveFront(dir);
		}

		if (m_player->IsDead()) {
			m_player = nullptr;
		}
	}

	//カメラの更新。
	m_springCamera.Update();
}

void GameCamera::OnDestroy()
{
}
/*
カメラの移動、更新処理。
*/
void GameCamera::MoveCamera()
{
	//カメラを更新。
	//注視点を計算する。
	CVector3 target = m_player->GetPosition();
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 50.0f;

	//古い座標を保持。
	CVector3 oldPos = toPos;
	//パッドの入力を使ってカメラを回転。
	float x = Pad(0).GetRStickXF();
	float y = Pad(0).GetRStickYF();
	//Y軸回りの回転。
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 3.0f * x);
	qRot.Multiply(toPos);
	//X軸回りの回転。
	CVector3 axisX;
	axisX.Cross(CVector3::AxisY, toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 3.0f * y);
	qRot.Multiply(toPos);
	//カメラ回転の上限値チェック。
	CVector3 toPosDir = toPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//上向きすぎ。
		toPos = oldPos;
	}
	if (toPosDir.y > 0.8f) {
		//下向きすぎ。
		toPos = oldPos;
	}

	//視点を計算する。
	//キャラクタの上あたりと相対座標を足してカメラのポジションを出す。
	CVector3 pos = target + toPos;


	//メインカメラに注視点と視点を設定する。
	m_springCamera.SetTarget(target);	//注視点　キャラクタの上あたり。
	m_springCamera.SetPosition(pos);	//カメラの座標。
}
/*
プレイヤーの正面へカメラを動かす。
*/
void GameCamera::MoveFront(const CVector3& dir)
{
	//カメラを更新。
	//注視点を計算する。
	CVector3 target = m_player->GetPosition();
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 50.0f;

	//CVector3 vDir = dir;
	//vDir.y = 0.0f;
	//vDir.Normalize();
	//CVector3 vDirXZ = vDir;		//XZ平面上のベクトル。
	//CVector3 vY = CVector3::AxisY;
	//vDir = vDir * 0.8f + vY * 0.2f;		//少しyを上げる。
	//vDir.Normalize();
	//CVector3 toPosDirXZ = toPos;
	//toPosDirXZ.y = 0.0f;
	//toPosDirXZ.Normalize();
	//
	////角度を求めていく。
	////XZ平面上での角度を求める。
	//float angleXZ = toPosDirXZ.Dot(vDirXZ);		//角度。
	//angleXZ = min(1.0f, max(-1.0f, angleXZ));
	//angleXZ = acosf(angleXZ);		//内積結果を角度にする。
	////Y軸回りの回転。
	//CQuaternion qRot;
	//qRot.SetRotation(CVector3::AxisY, angleXZ);
	//qRot.Multiply(toPos);

	////少し上からのぞき込む角度にする。
	//CVector3 toPosDir = toPos;
	//toPosDir.Normalize();
	//float angleY = toPosDir.Dot(vDir);		//角度。
	//angleY = min(1.0f, max(-1.0f, angleY));
	//angleY = acosf(angleY);
	//CVector3 axis;		//回転軸。
	//axis.Cross(toPosDir, vDir);
	//axis.Normalize();
	//qRot.SetRotation(axis, angleY);	
	//qRot.Multiply(toPos);

	CVector3 vDir = dir;
	vDir.y = 0.0f;
	vDir.Normalize();
	CVector3 vY = CVector3::AxisY;
	vDir = vDir * 0.8f + vY * 0.2f;		//少しyを上げる。
	vDir.Normalize();
	CVector3 toPosDir = toPos;
	toPosDir.Normalize();
	float angle = toPosDir.Dot(vDir);		//角度。
	angle = min(1.0f, max(-1.0f, angle));
	angle = acosf(angle);
	CVector3 axis;		//回転軸。
	axis.Cross(toPosDir, vDir);
	axis.Normalize();
	CQuaternion qRot;
	qRot.SetRotation(axis, angle);
	qRot.Multiply(toPos);

	CVector3 pos = target + toPos;

	//メインカメラに注視点と視点を設定する。
	m_springCamera.SetTarget(target);	//注視点　キャラクタの上あたり。
	m_springCamera.SetPosition(pos);	//カメラの座標。
}
