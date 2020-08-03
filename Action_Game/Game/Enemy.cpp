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
	//�X�L�����f�������_�[�̐ݒ�B
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Enemy.cmo");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetShadowCasterFlag(true);
	//�L�����R���̐ݒ�B
	m_charaCon.Init(
		30.0f,		//���a�B
		10.0f,		//�����B
		m_position	//���W�B
	);
	
	//�������W�̕ۑ��B
	m_firstPosition = m_position;

	//�v���C���[�̃C���X�^���X�������B
	m_player = FindGO<Player>("Player");

	return true;
}

void Enemy::Update()
{
	m_timer -= GameTime().GetFrameDeltaTime();

	AxisUpdate();
	if (m_player != nullptr) {
		CVector3 toPlayer = m_player->GetPosition() - m_position;	//�G�l�~�[����v���C���[�܂ł̃x�N�g���B
		CVector3 toFirstPos = m_firstPosition - m_position;		//�G�l�~�[�̌��ݍ��W���珉�����W�܂ł̃x�N�g���B
		if (m_timer < 0.0f) {
			if (toPlayer.Length() < 500.0f) {	//�v���C���[�Ƃ̋�����500.0f�����������A
				EnemyRotation(toPlayer, 2.0f * GameTime().GetFrameDeltaTime());
				AxisUpdate();
				EnemyMove(toPlayer);
			}
			else if (toFirstPos.Length() > 5.0f) {//�������W���痣��Ă�����A
				//�v���C���[�������������̏����B
				EnemyRotation(toFirstPos, 1.0f);
				AxisUpdate();
				m_speed = defaultSpeed;
				//�ړ����x���o���B
				m_moveSpeed.x = m_forward.x * m_speed;
				m_moveSpeed.z = m_forward.z * m_speed;
				m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
				//�������x�̏���B
				m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);
				m_position = m_charaCon.Execute(m_moveSpeed);		//�ړ��̏����B
			}

		}
		else {
			//�m�b�N�o�b�N�̑��x�̌����B
			m_moveSpeed.x -= m_moveSpeed.x * 5.0f * GameTime().GetFrameDeltaTime();
			m_moveSpeed.z -= m_moveSpeed.z * 5.0f * GameTime().GetFrameDeltaTime();
			m_position = m_charaCon.Execute(m_moveSpeed);
			if (toPlayer.y > 10.0f && toPlayer.Length() < 55.0f) {
				Steped();
				DeleteGO(this);
			}
		}

		if (m_player->IsDead()) {	//�v���C���[�̎��S���̏����B
			m_player = nullptr;
		}

	}

	if (m_position.y < -2000.0f) {
		DeleteGO(this);		//�������玀�S�B
	}

	//�X�L�����f�������_�[�̍X�V�B
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);

}

void Enemy::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}


//��]��������x�N�g�����쐬�B
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

//�G�l�~�[��XZ���ʏ�Ɉړ������Ɍ������ĉ�]������B
//CVector3 vecMove	�ړ�����	CVector3�^
//Normalize�͕s�K�v�B
//float angleSpeed	��]���x�B
void Enemy::EnemyRotation(const CVector3& vecMove, float angleSpeed)
{
	//�x�N�g����XZ���ʏ�ɂ��Đ��K���B
	CVector3 vMove = vecMove;
	vMove.y = 0.0f;
	if (vMove.Length() < 40.0f) {
		return;
	}
	vMove.Normalize();
	//��]�ʂ����߂�B
	float angle = vMove.Dot(m_forward);
	angle = min(1.0f, max(-1.0f, angle));

	CQuaternion qRot;

	//�Ȃ񂩂��܂������Ȃ�
	angle = acosf(angle);	//�����������Ă邹����...
							//�����܂œ��ςŏo��̂�cos�Ƃ̒l�ŁA
							//���W�A���P�ʂ̊p�x�Ȃ킯�ł͂Ȃ����Ă��Ƃ��...

	//Degree�p�ɕϊ��B
	float deg = CMath::RadToDeg(angle);

	if (deg > 60.0f) {
		m_speed -= decele * GameTime().GetFrameDeltaTime();
		deg = max(60.0f, deg);
	}


	//��]�̌��������߂�B
	float dir = vMove.Dot(m_right);
	if (dir < 0) {
		deg *= -1.0f;
	}

	qRot.SetRotationDeg(CVector3::AxisY, deg * angleSpeed);
	m_rotation.Multiply(qRot);
}

void Enemy::EnemyMove(const CVector3& vecMove)
{
	CVector3 toFirstPos = m_firstPosition - m_position;		//�G�l�~�[�̌��ݍ��W���珉�����W�܂ł̃x�N�g���B
	//�ړ����x�����߂�B
	m_speed = min(MAXspeed, max(defaultSpeed, m_speed + (accele * GameTime().GetFrameDeltaTime())));
	m_moveSpeed.x = m_forward.x * m_speed;
	m_moveSpeed.z = m_forward.z * m_speed;
	m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
	//�������x�̏���B
	m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);

	CVector3 oldPos = m_position;		//�Â����W��ێ��B
	m_position = m_charaCon.Execute(m_moveSpeed);	//	�ړ������B
	//�������W���痣�ꂷ�����ꍇ�ɖ߂��B
	toFirstPos = m_firstPosition - m_position;
	if (toFirstPos.Length() > 1000.0f) {
		m_position = oldPos;
		m_charaCon.SetPosition(m_position);
	}
	if (!m_charaCon.IsOnGround()) {		//�����Ȃ��悤�ɖ߂��B
		CVector3 vReturn = oldPos - m_position;
		vReturn.y = 0.0f;
		vReturn.Normalize();
		m_position = oldPos;
		m_charaCon.SetPosition(m_position);
		m_charaCon.Execute(vReturn * defaultSpeed);
	}
	if (vecMove.Length() < 55.0f) {	//�v���C���[�ƐڐG������A
		if (vecMove.y > 10.0f) {
			Steped();
			DeleteGO(this);
		}
		else {
			//�m�b�N�o�b�N�B
			m_player->Damage(m_moveSpeed);
			m_moveSpeed.x *= -1.0f;
			m_moveSpeed.z *= -1.0f;
			//speed��timer�̐ݒ�B
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
