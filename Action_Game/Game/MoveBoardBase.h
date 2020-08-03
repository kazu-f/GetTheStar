#pragma once
#include"Player.h"

class MoveBoardBase : public IGameObject
{
public:	
	//���W��ݒ�B
	void SetPosition(const CVector3& position) 
	{
		m_position = position;
	}
	//��]��ݒ�B
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}
	//���x�̐ݒ�B
	//�����@float�^�B
	void SetSpeed(float speed)
	{
		m_speed = speed;
	}
	//�^�C�}�[�̐ݒ�B
	void SetTimer(float time)
	{
		m_timer = time;
	}
	//�^�C�}�[�̏���̐ݒ�B
	void SetTimeLimit(float time)
	{
		timeLimit = time;
	}


protected:
	//��]��������x�N�g�����쐬�B
	void AxisUpdate()
	{
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(m_rotation);
		m_right = { mRot.m[0][0],mRot.m[0][1], mRot.m[0][2] };
		m_up = { mRot.m[1][0],mRot.m[1][1], mRot.m[1][2] };
		m_front = { mRot.m[2][0],mRot.m[2][1], mRot.m[2][2] };
		//���K���B
		m_right.Normalize();
		m_up.Normalize();
		m_front.Normalize();
	}
	void PushPlayer();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_scale = CVector3::One;	//�X�P�[���B
	CVector3 m_position = CVector3::Zero;	//���W�B
	CVector3 firstPos = CVector3::Zero;	//�������W�B
	CVector3 m_moveSpeed = CVector3::Zero;	//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity;	//��]�B
	CVector3 m_front = CVector3::Front;	//�O�����B
	CVector3 m_right = CVector3::Right;	//�E�����B
	CVector3 m_up = CVector3::Up;	//������B
	CVector3 aabb[2] = { CVector3::Zero,CVector3::Zero };	//aabb 0���������A1���傫���B
	//CQuaternion m_direction = CQuaternion::Identity;	//�����B
	float m_timer = 0.0f;		//�^�C�}�[�B
	float timeLimit = 0.0f;		//�^�C�}�[�̏���B
	float m_speed = 0.0f;	//���x�B
	CPhysicsStaticObject m_phyStaticObject;	//�ÓI�����I�u�W�F�N�g�B
	Player* m_player = nullptr;		//�v���C���[�B
};

