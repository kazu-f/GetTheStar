#include "stdafx.h"
#include "GoalStar.h"

GoalStar::GoalStar()
{
}

GoalStar::~GoalStar()
{
}

bool GoalStar::Start()
{
	//�X�L�����f�������_�[�̐ݒ�B
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/GoalStar.cmo");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetShadowCasterFlag(true);

	//�������W��y��ێ�
	m_firstPosy = m_position.y;

	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("Player");

	return true;
}

void GoalStar::Update()
{
	////�����ӂ�ӂ킳����B
	//const float high = 2.0f;
	//if (m_position.y < m_firstPosy - high && !m_floatingFlag) {
	//	//ySpeed += 3.0f * acosf(m_timer) * GameTime().GetFrameDeltaTime();
	//	//ySpeed = 50.0f * acosf(m_timer) * GameTime().GetFrameDeltaTime();
	//	//m_ySpeed += 4.0f * GameTime().GetFrameDeltaTime();
	//	//m_position.y += min(m_ySpeed, 10.0f);
	//	m_speed *= -1.0f;
	//	m_floatingFlag = true;
	//}
	//else if (m_position.y > m_firstPosy + high && m_floatingFlag) {
	//	//ySpeed += -3.0f * acosf(m_timer - 1.0f) * GameTime().GetFrameDeltaTime();
	//	//ySpeed = -50.0f * acosf(m_timer - 1.0f) * GameTime().GetFrameDeltaTime();
	//	//m_ySpeed -= 4.0f * GameTime().GetFrameDeltaTime();
	//	//m_position.y += max(m_ySpeed, -10.0f);
	//	m_speed *= -1.0f;
	//	m_floatingFlag = false;
	//}
	//m_ySpeed -= m_speed * GameTime().GetFrameDeltaTime();
	//m_position.y += min(0.5f, max(-0.5f, m_ySpeed));

	const float PI = 3.141592653589793238462643383279;	//�~�����B���������܂Ō�����Ȃ��B
	if (m_timer > PI / 2 * 3) {
		m_timer -= PI * 2.0f;
	}

	m_timer += 2.0f * GameTime().GetFrameDeltaTime();
	m_position.y = m_firstPosy + sinf(m_timer) * 20.0f;

	if (m_player != nullptr) {
		const float starLen = 40.0f;
		CVector3 playerPos = m_player->GetPosition();
		CVector3 toPlayer = playerPos - m_position;
		playerPos.y += 60.0f;
		CVector3 toPlayerBody = playerPos - m_position;
		playerPos.y += 60.0f;
		CVector3 toPlayerHead = playerPos - m_position;
		if (toPlayer.Length() < starLen
			|| toPlayerBody.Length() < starLen
			|| toPlayerHead.Length() < starLen) {
			DeleteGO(this);
			m_player->GameClear();
		}
	}


	//������]������B
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 150.0f * GameTime().GetFrameDeltaTime());	//1�b�Ԃ�180�x��]�B
	m_rotation.Multiply(qRot);

	//���f���̍X�V�B
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
}

void GoalStar::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
