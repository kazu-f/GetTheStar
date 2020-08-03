#pragma once
#include "Player.h"
//#include "tkEngine/physics/tkPhysicsGhostObject.h"


class JumpStand : public IGameObject
{
public:
	JumpStand();
	~JumpStand();
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
	/*
	�X�P�[���̐ݒ�B
	*/
	void SetScale(const CVector3& scale) 
	{
		m_scale = scale;
	}

private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[�B
	CPhysicsStaticObject m_phyStaticObject;				//�ÓI�����I�u�W�F�N�g�B
	CVector3 m_position = CVector3::Zero;			//���W�B
	CVector3 m_scale = CVector3::One;				//�X�P�[���B
	CQuaternion m_rotation = CQuaternion::Identity;		//��]�B
	Player* m_player;		//�v���C���[�B
	//CPhysicsGhostObject m_ghostObject;		//�S�[�X�g�B�Ȃ񂩏d���Ȃ邩��p���B
	CVector3 aabb[2] = { CVector3::Zero,CVector3::Zero };	//aabb�B0���������A1���傫���B
	//float jumpSpeed = 1920.0f;
	const float jumpSpeed = 980.0f;
};

