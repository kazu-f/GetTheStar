#pragma once
#include "Player.h"

class GoalStar : public IGameObject
{
public:
	GoalStar();
	~GoalStar();
	bool Start();
	void Update();
	void OnDestroy();

	/*
	���W�̐ݒ�B
	*/
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	};
	/*
	��]�̐ݒ�B
	*/
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}


private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//�X�L�����f�������_�[�B
	Player* m_player = nullptr;		//�v���C���[�B
	CVector3 m_position = CVector3::Zero;		//���W�B
	CVector3 m_scale = CVector3::One;		//�X�P�[���B
	//CVector3 m_moveSpeed = CVector3::Zero;		//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity;		//��]�B
	float m_firstPosy = 0.0f;		//������Y���W�B
	float m_timer = 0.0f;		//�^�C�}�[�B
	//float m_ySpeed = 0.5f;		//�ӂ�ӂ킷�鑬�x�B
	//float m_speed = 4.0f;		//��{���x�B
	//bool m_floatingFlag = false;	//���V�t���O�B
};

