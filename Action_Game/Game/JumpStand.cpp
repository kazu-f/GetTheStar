#include "stdafx.h"
#include "JumpStand.h"


JumpStand::JumpStand()
{
}

JumpStand::~JumpStand()
{
}

bool JumpStand::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/JumpStand.cmo");
	
	m_skinModelRender->SetPosition(m_position);	//���W
	m_skinModelRender->SetScale(m_scale);		//�X�P�[���B
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetShadowReceiverFlag(true);

	////��]�̐ݒ�B
	//m_rotation.SetRotationDeg(CVector3::AxisX, 90.0f);
	//m_skinModelRender->SetRotation(m_rotation);

	//�ÓI�����I�u�W�F�N�g�̍쐬
	m_phyStaticObject.CreateMesh(m_position, m_rotation, m_scale, m_skinModelRender);

	////�{�b�N�X�`��̃S�[�X�g���쐬�B
	//CVector3 ghostPos = m_position;
	//ghostPos.y += 40.0f;
	//m_ghostObject.CreateBox(
	//	ghostPos,			//���W�B
	//	CQuaternion::Identity,			//��]�B
	//	{ 200.0f,boxHigh,200.0f }		//�{�b�N�X�̃T�C�Y�Bx,y,z
	//);

	const float boxX = 200.0f;	//�{�b�N�X��x�B
	const float boxZ = 200.0f;	//�{�b�N�X��z�B
	const float boxHigh = 90.0f;//�{�b�N�X��y�B
	//aabb�̐ݒ�B
	aabb[0] = m_position;
	aabb[0].x += boxX / -2.0f * m_scale.x;
	aabb[0].z += boxZ / -2.0f * m_scale.z;
	
	aabb[1] = m_position;
	aabb[1].x += boxX / 2.0f * m_scale.x;
	aabb[1].y += boxHigh * m_scale.y;
	aabb[1].z += boxZ / 2.0f * m_scale.z;
	


	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("Player");


	return true;
}

void JumpStand::Update()
{
	if (m_player != nullptr) {
		//PhysicsWorld().ContactTest(m_player->GetCharaCon(), [&](const btCollisionObject& contactObject) {
		//	if (m_ghostObject.IsSelf(contactObject) == true) 
		//	{
		//		CVector3 playerMoveSpeed = m_player->GetMoveSpeed();
		//		//CVector3 playerPos = m_player->GetPosition();
		//		if (m_player->IsPlayerOnGround()) 
		//		{
		//			playerMoveSpeed.y = jumpSpeed;
		//			m_player->SetMoveSpeed(playerMoveSpeed);
		//			m_player->PlayerMove(playerMoveSpeed);
		//		}
		//		else
		//		{
		//			playerMoveSpeed.y = min(0.0f, playerMoveSpeed.y);
		//			m_player->SetMoveSpeed(playerMoveSpeed);
		//			m_player->PlayerMove(playerMoveSpeed);
		//		}
		//		
		//	}
		//	});

		//�v���C���[�̍��W���擾�B
		CVector3 playerPos = m_player->GetPosition();
		const float playerHigh = 120.0f;	//�v���C���[�̍����B
		const float w = 20.0f;	//�v���C���[�̕��B
		if (aabb[0].x - w < playerPos.x && aabb[0].z - w < playerPos.z			//X���W��Z���W�͈͓̔���
			&& aabb[1].x + w > playerPos.x && aabb[1].z + w > playerPos.z		//�v���C���[�����邩
			&& aabb[0].y < playerPos.y + playerHigh && aabb[1].y > playerPos.y)
		{
			CVector3 playerMoveSpeed = m_player->GetMoveSpeed();
			if (m_player->IsPlayerOnGround()		//�n�ʂɂ��Ă��邩�ǂ����B
				&& playerPos.y > aabb[1].y * 0.8f)	//�{�b�N�X�̏�ʂɂ��邩�ǂ����B
			{
				//�W�����v������B
				playerMoveSpeed.y = jumpSpeed;
				m_player->SetMoveSpeed(playerMoveSpeed);
				m_player->PlayerMove(playerMoveSpeed);
			}
			else if (playerPos.y < aabb[0].y)
			{
				//�����甲���Ă��Ȃ��悤�ɂ���B
				playerMoveSpeed.y = min(0.0f, playerMoveSpeed.y);
				m_player->SetMoveSpeed(playerMoveSpeed);
				m_player->PlayerMove(playerMoveSpeed);
			}
		}

		if (m_player->IsDead()) {
			m_player = nullptr;
		}

	}
}

void JumpStand::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
