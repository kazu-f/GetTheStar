#pragma once
#include "Player.h"

class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void OnDestroy();

	///*
	//���W���擾�B
	//return ���W�B
	//*/
	//const CVector3& GetPosition() const
	//{
	//	return m_position;
	//};
	///*
	//�ړ����x���擾�B
	//return �ړ����x�B
	//*/
	//const CVector3& GetMoveSpeed() const
	//{
	//	return m_moveSpeed;
	//}
	///*
	//�L�����R�����擾�B
	//return �L�����R���B
	//*/
	//CCharacterController& GetCharaCon()
	//{
	//	return m_charaCon;
	//}

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




private:
	void AxisUpdate();		//�N�H�[�^�j�I��������������߂�B
	void EnemyRotation(const CVector3& vecMove, float angleSpeed);	//�G�l�~�[�̉�]������B
	void EnemyMove(const CVector3& vecMove);	//�G�l�~�[�̈ړ��B
	void Steped();		//�v���C���[�ɓ��܂ꂽ���̏����B

	//�����o�ϐ��B
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//�X�L�����f�������_�[�B
	CVector3 m_position = CVector3::Zero;	//���W�B
	CVector3 m_firstPosition = CVector3::Zero;	//�������W�B
	CVector3 m_moveSpeed = CVector3::Zero;	//�ړ����x�B
	CVector3 m_right = CVector3::Right;		//�G�l�~�[�̉E�����B
	CVector3 m_up = CVector3::Up;			//�G�l�~�[�̏�����B
	CVector3 m_forward = CVector3::Front;	//�G�l�~�[�̑O�����B
	CQuaternion m_rotation = CQuaternion::Identity;	//��]�B
	CCharacterController m_charaCon;	//�L�����R���B

	Player* m_player = nullptr;	//�v���C���[�B

	float m_speed = 0.0f;		//���x�B
	const float MAXspeed = 400.0f;		//�ō����x�B
	const float accele = 80.0f;			//�����x�B
	const float defaultSpeed = 200.0f;	//�f�t�H���g�X�s�[�h�B
	const float decele = 100.0f;		//�����x�B
	const float gravitation = 980.0f;	//�d�́B
	const float maxDropSpeed = -2560.0f;//�ő嗎�����x�B
	float m_timer = 0.0f;
};

