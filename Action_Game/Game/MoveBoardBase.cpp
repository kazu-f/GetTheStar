#include "stdafx.h"
#include "MoveBoardBase.h"

void MoveBoardBase::PushPlayer()
{
	//�v���C���[�Ɨ��ꂷ���Ă���Ȃ珈�������Ȃ��悤�ɂ���B
	float lenRes = max(aabb[0].Length(), aabb[1].Length());
	lenRes += 100.0f;
	CVector3 diff = m_position - m_player->GetPosition();
	float toLen = diff.Length();
	if (toLen > lenRes) {
		return;		//���ꂷ���Ă���B
	}

	//���s�ړ��s��Ɖ�]�s��B
	CMatrix mTrans, mRot;
	mTrans.MakeTranslation(m_position);
	mRot.MakeRotationFromQuaternion(m_rotation);
	//���̃��[���h�s����쐬�B
	CMatrix mWorld;
	mWorld = mRot * mTrans;
	//���̃��[���h�s��Ɖ�]�s��̋t�s����쐬�B
	CMatrix mWorldInv, mRotInv;
	mWorldInv.Inverse(mWorld);
	mRotInv.Inverse(mRot);

	const float w = 20.0f;	//�v���C���[�̕��B
	const float playerHigh = 120.0f;	//�v���C���[�̍����B
	//�v���C���[�̍��W�𔠂̋t�s����g���ĕϊ��B
	CVector3 playerPos = m_player->GetPosition();
	mWorldInv.Apply(playerPos);
	//������v���C���[�܂ł̔����W�n�̃x�N�g���B
	CVector3 toPlayerXZ = playerPos;
	toPlayerXZ.y = 0.0f;
	//float len = toPlayerXZ.Length();
	//�ړ������𔠍��W�n�ɕϊ��B
	CVector3 vMove = m_moveSpeed;
	mRotInv.Apply(vMove);
	//�ړ������ƃv���C���[�܂ł̃x�N�g���̊p�x?�����߂�B
	toPlayerXZ.Normalize();
	vMove.Normalize();
	float dir = vMove.Dot(toPlayerXZ);

	//�I�u�W�F�N�g�̌��_����O�ʂ̒��_�̊p�x���Ƃ�B
	CVector3 toVertex = aabb[0];
	toVertex.y = 0.0f;
	//float toVertexLen = max(toVertex.x, toVertex.z);
	toVertex.Normalize();
	//float dirVertex = toVertex.Dot(vMove);	//�i�s�����ƒ��_�܂ł̃x�N�g���̊p�x���Ƃ�B
	//if (dirVertex < 0.0f) {
	//	toVertex *= -1.0f;		//�����𔽓]�B
	//}
	float angle = vMove.Dot(toVertex);		//�i�s�����ƒ��_�̊p�x�B

	//aabb�̔�����Ƃ�B
	if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z		//X���W��Z���W�͈͓̔���
		&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z		//�v���C���[�����邩
		&& playerPos.y < aabb[1].y && playerPos.y + playerHigh > aabb[0].y
		/*&& dir > angle*/) {
		if (playerPos.y > aabb[0].y	//�v���C���[����ɂ��邩�B
			&& m_player->IsPlayerOnGround() == true) {
			//��ɏ���Ă鎞�̏���
			m_player->PlayerMove(m_moveSpeed);
		}
		else if (dir > angle){		//�ړ��������Ƀv���C���[�����邩
			//�{�b�N�X���O�ɂ��邩�B
			//�����o���̏����B
			m_player->PlayerMove(m_moveSpeed);
			//�v���C���[�̍��W�𔠂̋t�s����g���ĕϊ��B
			playerPos = m_player->GetPosition();
			mWorldInv.Apply(playerPos);
			if (aabb[0].x < playerPos.x && aabb[0].z < playerPos.z		//X���W��Z���W�͈͓̔���
				&& aabb[1].x > playerPos.x && aabb[1].z > playerPos.z		//�v���C���[�����邩
				&& playerPos.y < aabb[0].y) {	//�v���C���[�����ɂ��邩�B
				//������Ԃ������Ƃ��̏���
				CVector3 playerMove = m_player->GetMoveSpeed();
				playerMove.y = min(0.0f, playerMove.y);
				m_player->SetMoveSpeed(playerMove);
			}
		}
		else if (playerPos.y < aabb[0].y) {	//�v���C���[�����ɂ��邩�B
			//������Ԃ������Ƃ��̏���
			CVector3 playerMove = m_player->GetMoveSpeed();
			playerMove.y = min(0.0f, playerMove.y);
			m_player->SetMoveSpeed(playerMove);
		}
	}
}
