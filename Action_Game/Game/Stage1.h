#pragma once
#include "StageBase.h"
//class Player;
////class BackGround;
//class JumpStand;
//class GoalStar;

class Stage1:public StageBase
{
public:
	Stage1();
	~Stage1();
	bool Start();
	void Update();
	void OnDestroy();
	///*
	//�N���A�^�C�����擾����B
	//*/
	//float GetClearTime()
	//{
	//	if (!m_isGameClear) {
	//		//�N���A���Ă��Ȃ��Ȃ�ő�l��n���B
	//		return FLT_MAX;
	//	}
	//	//�^�C����b�ɒ����B
	//	float clearTime = (float)m_minTimer * 60.0f + m_gameTimer;

	//	return clearTime;
	//}

private:
	//void Light();
	//void CountTime();

	////////////
	//prefab::CDirectionLight* m_lig = nullptr;			//���C�g�B
	////CVector3 m_lightDir = CVector3::One;					//���C�g�̕����B
	//Player* m_player = nullptr;				//�v���C���[�B
	//GoalStar* m_goalStar = nullptr;		//�S�[���B
	//CLevel m_level;							//���x���B
	//prefab::CSkinModelRender* m_ground = nullptr;	//�X�e�[�W�̃��f���B
	//CPhysicsStaticObject m_phyStaticObj;			//�X�e�[�W�̕����I�u�W�F�N�g�B
	//prefab::CSpriteRender* m_sprite = nullptr;		//�X�v���C�g�B
	////CVector3 m_spritePos = CVector3::Zero;
	//prefab::CSoundSource* m_bgm = nullptr;		//BGM�B
	//float m_timer = 0.0f;		//�^�C�}�[�B
	//prefab::CFontRender* m_fontRender = nullptr;		//�^�C����\������B
	//CVector4 m_fontColor = CVector4::White;			//�e�L�X�g�̐F�B
	//CVector2 m_fontPos = CVector2::Zero;			//�e�L�X�g�̍��W�B
	//wchar_t m_gameTimeTex[64];		//�^�C���̃e�L�X�g�B
	//float m_gameTimer = 0.0f;		//�N���A�^�C���B
	//int m_minTimer = 0;				//�N���A�^�C���B�i���j

	//bool m_isGameOver = false;
	//bool m_isGameClear = false;
};

