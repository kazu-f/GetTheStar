#pragma once
class BackGround : public IGameObject
{
public:
	//�����o�֐�
	BackGround();
	~BackGround();
	bool Start();
	//void Update();
	void OnDestroy();

	//�����o�ϐ�
	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[
	CPhysicsStaticObject m_phyStaticObject;						//�ÓI�����I�u�W�F�N�g
	//CVector3 m_position = CVector3::Zero;						//���W
	//CVector3 scale = CVector3::One;								//�X�P�[��
	//CQuaternion m_rotation = CQuaternion::Identity;			//��]�B
};

