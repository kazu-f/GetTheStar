#include "stdafx.h"
#include "MoveBoard.h"

MoveBoard::MoveBoard()
{
}

MoveBoard::~MoveBoard()
{
}

bool MoveBoard::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/MoveBoard.cmo");

	//��]�̐ݒ�B
	m_skinModelRender->SetRotation(m_rotation);

	m_phyStaticObject.CreateMesh(m_position, m_rotation, m_scale, m_skinModelRender);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetShadowReceiverFlag(true);

	//aabb�̐ݒ�B
	const float boxX = 100.0f;
	const float boxY = 90.0f;
	const float boxZ = 100.0f;
	aabb[0].x -= boxX;
	aabb[0].z -= boxZ;
	aabb[1].x += boxX;
	aabb[1].z += boxZ;
	aabb[1].y += boxY;

	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("Player");

	timeLimit = 5.0f;
	m_timer = timeLimit / 2.0f;	//�^�C�����~�b�g�̔��������B
	AxisUpdate();

	return true;
}

void MoveBoard::Update()
{
	Move();
	if (m_player != nullptr) {
		PushPlayer();
		if (m_player->IsDead()) {
			m_player = nullptr;
		}
	}
}

void MoveBoard::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

//�ړ������B
void MoveBoard::Move()
{
	m_timer += GameTime().GetFrameDeltaTime();
	if (m_timer > timeLimit) {
		//m_speed *= -1.00f;
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
}

//void MoveBoard::PushPlayer()
//{
//	//���s�ړ��s��Ɖ�]�s��B
//	CMatrix mTrans, mRot;
//	mTrans.MakeTranslation(m_position);
//	mRot.MakeRotationFromQuaternion(m_rotation);
//	//���̃��[���h�s����쐬�B
//	CMatrix mWorld;
//	mWorld = mRot * mTrans;
//	//���̃��[���h�s��Ɖ�]�s��̋t�s����쐬�B
//	CMatrix mWorldInv, mRotInv;
//	mWorldInv.Inverse(mWorld);
//	mRotInv.Inverse(mRot);
//
//	const float w = 20.0f;	//�v���C���[�̕��B
//	const float playerHigh = 120.0f;	//�v���C���[�̍����B
//	//�v���C���[�̍��W�𔠂̋t�s����g���ĕϊ��B
//	CVector3 playerPos = m_player->GetPosition();
//	mWorldInv.Apply(playerPos);
//	//������v���C���[�܂ł̔����W�n�̃x�N�g���B
//	CVector3 toPlayerXZ = playerPos;
//	toPlayerXZ.y = 0.0f;
//	//�ړ������𔠍��W�n�ɕϊ��B
//	CVector3 vMove = m_moveSpeed;
//	mRotInv.Apply(vMove);
//	//�ړ������ƃv���C���[�܂ł̃x�N�g���̊p�x?�����߂�B
//	toPlayerXZ.Normalize();
//	vMove.Normalize();
//	float dir = vMove.Dot(toPlayerXZ);
//
//	//aabb�̔�����Ƃ�B
//	if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z		//X���W��Z���W�͈͓̔���
//		&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z		//�v���C���[�����邩
//		&& playerPos.y < aabb[1].y && playerPos.y + playerHigh > aabb[0].y
//		&& dir > 0.7f) {	//�ړ��������Ƀv���C���[�����邩
//		//�����o���̏����B
//		m_player->PlayerMove(m_moveSpeed);
//		
//		//�v���C���[�̍��W�𔠂̋t�s����g���ĕϊ��B
//		playerPos = m_player->GetPosition();
//		mWorldInv.Apply(playerPos);
//		if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z
//			&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z) {
//			//������Ԃ������Ƃ��̏���
//			CVector3 playerMove = m_player->GetMoveSpeed();
//			playerMove.y = min(0.0f, playerMove.y);
//			m_player->SetMoveSpeed(playerMove);
//		}
//	}
//	else if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z	//X���W��Z���W�͈͓̔���
//		&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z) 	//�v���C���[�����邩
//	{
//		if (playerPos.y < aabb[1].y && playerPos.y > aabb[0].y	//�v���C���[����ɂ��邩�B
//			&& m_player->IsPlayerOnGround() == true) {			//�n�ʂɂ��Ă��邩�B
//		//��ɏ���Ă鎞�̏���
//			m_player->PlayerMove(m_moveSpeed);
//		}
//		else if (playerPos.y + playerHigh > aabb[0].y
//			&& playerPos.y < aabb[0].y) {
//			//������Ԃ������Ƃ��̏���
//			CVector3 playerMove = m_player->GetMoveSpeed();
//			playerMove.y = min(0.0f, playerMove.y);
//			m_player->SetMoveSpeed(playerMove);
//		}
//	}
//}
