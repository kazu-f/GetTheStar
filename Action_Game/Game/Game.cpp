#include "stdafx.h"
#include "Game.h"
#include "GameTitle.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"

Game::Game() 
{
}


Game::~Game()
{
}

bool Game::Start()
{
	//�����I�u�W�F�N�g��\��
	dbg::SetDrawPhysicsCollisionEnable();
	//�S�[�X�g�̃��C���[�t���[���\����L���ɂ���B
	PhysicsWorld().SetDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);
	
	for (int i = 0; i < select_Stage_Num; i++) {
		//�x�X�g�^�C���̏����l��K���Ɍ��߂Ă����B
		m_bestTime[i] = 180.0f;
	}

	m_title = NewGO<GameTitle>(0);
	m_title->SetBestClearTime(m_bestTime);

	return true;
}

void Game::Update()
{
	if (m_title && m_title->IsDead()) {		//�H�ɂ����ɓ���Ȃ��B���S���肪��肭�����Ȃ��Ƃ�������H
		//�I�������X�e�[�W���󂯂Ƃ�B
		int stageNum = m_title->GetSelectNum();
		m_title = nullptr;
		
		switch (stageNum) {
		case select_Stage1:
			m_stage1 = NewGO<Stage1>(0);
			break;
		case select_Stage2:
			m_stage2 = NewGO<Stage2>(0);
			break;
		case select_Stage3:
			m_stage3 = NewGO<Stage3>(0);
			break;
		}
		
	}
	if (m_stage1 && m_stage1->IsDead()) {
		m_bestTime[select_Stage1] = min(m_stage1->GetClearTime(), m_bestTime[select_Stage1]);
		m_stage1 = nullptr;
		m_title = NewGO<GameTitle>(0);
		m_title->SetBestClearTime(m_bestTime);
	}
	if (m_stage2 && m_stage2->IsDead()) {
		m_bestTime[select_Stage2] = min(m_stage2->GetClearTime(), m_bestTime[select_Stage2]);
		m_stage2 = nullptr;
		m_title = NewGO<GameTitle>(0);
		m_title->SetBestClearTime(m_bestTime);
	}
	if (m_stage3 && m_stage3->IsDead()) {
		m_bestTime[select_Stage3] = min(m_stage3->GetClearTime(), m_bestTime[select_Stage3]);
		m_stage3 = nullptr;
		m_title = NewGO<GameTitle>(0);
		m_title->SetBestClearTime(m_bestTime);
	}
}

void Game::OnDestroy()
{
}