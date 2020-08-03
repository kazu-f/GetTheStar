#include "stdafx.h"
#include "MoveBoardBig.h"

MoveBoardBig::MoveBoardBig()
{
}

MoveBoardBig::~MoveBoardBig()
{
}

bool MoveBoardBig::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/MoveBoardBig.cmo");

	//回転の設定。
	m_skinModelRender->SetRotation(m_rotation);

	m_phyStaticObject.CreateMesh(m_position, m_rotation, m_scale, m_skinModelRender);
	m_skinModelRender->SetPosition(m_position);

	//aabbの設定。
	const float boxX = 200.0f;
	const float boxY = 210.0f;
	const float boxZ = 500.0f;
	aabb[0].x = boxX / -2.0f;
	aabb[0].z = boxZ / -2.0f;
	aabb[1].x = boxX / 2.0f;
	aabb[1].z = boxZ / 2.0f;
	aabb[1].y = boxY;

	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("Player");

	m_timer = 0.0f;
	AxisUpdate();

	return true;
}

void MoveBoardBig::Update()
{
	m_timer += GameTime().GetFrameDeltaTime();
	if (m_timer > timeLimit) {
		CQuaternion qRot;
		qRot.SetRotationDeg(m_up, 180.0f);
		m_rotation.Multiply(qRot);
		AxisUpdate();
		m_timer = 0.00f;
	}

	//移動速度の設定。
	m_moveSpeed = m_front * m_speed;
	//移動処理。
	m_position += m_moveSpeed * GameTime().GetFrameDeltaTime();
	m_phyStaticObject.SetPositionAndRotation(m_position, m_rotation);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetShadowReceiverFlag(true);

	if (m_player != nullptr) {
		PushPlayer();
		if (m_player->IsDead()) {
			m_player = nullptr;
		}
	}
	//更新処理。
	m_phyStaticObject.SetPositionAndRotation(m_position, m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

void MoveBoardBig::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
