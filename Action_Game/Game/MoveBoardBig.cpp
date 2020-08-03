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

	//��]�̐ݒ�B
	m_skinModelRender->SetRotation(m_rotation);

	m_phyStaticObject.CreateMesh(m_position, m_rotation, m_scale, m_skinModelRender);
	m_skinModelRender->SetPosition(m_position);

	//aabb�̐ݒ�B
	const float boxX = 200.0f;
	const float boxY = 210.0f;
	const float boxZ = 500.0f;
	aabb[0].x = boxX / -2.0f;
	aabb[0].z = boxZ / -2.0f;
	aabb[1].x = boxX / 2.0f;
	aabb[1].z = boxZ / 2.0f;
	aabb[1].y = boxY;

	//�v���C���[�̃C���X�^���X��T���B
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

	//�ړ����x�̐ݒ�B
	m_moveSpeed = m_front * m_speed;
	//�ړ������B
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
	//�X�V�����B
	m_phyStaticObject.SetPositionAndRotation(m_position, m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

void MoveBoardBig::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
