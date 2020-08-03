#include "stdafx.h"
#include "GameTitle.h"
#include "Game.h"
//#include "Stage1.h"

GameTitle::GameTitle()
{
}

GameTitle::~GameTitle()
{
}

bool GameTitle::Start()
{
	//�X�v���C�g�����_�[�̐ݒ�B
	m_spriteRender = NewGO<prefab::CSpriteRender>(0);
	m_spriteRender->Init(L"sprite/gameTitle.dds", 1280.0f, 720.0f);
	//�t�H���g�����_�[�̐ݒ�B
	m_fontRender = NewGO<prefab::CFontRender>(0);
	m_fontPos = { 0.0f,-200.0f };
	m_fontRender->SetPosition(m_fontPos);
	m_fontRender->SetColor(m_fontColor);
	swprintf_s(m_text, 
		L"    Stage%2d\nBestTime %2d:%05.2f",
		m_selectNum + 1,
		m_bestTimeMin[m_selectNum],
		m_bestTime[m_selectNum]);
	m_fontRender->SetText(m_text);
	CVector2 fontPivot = { 0.5f,0.5f };		//�s�{�b�g�𒆐S�ɐݒ�B
	m_fontRender->SetPivot(fontPivot);

	return true;
}

void GameTitle::Update()
{
	if (Pad(0).IsTrigger(enButtonA)) {
		//���ʉ���炷�B
		prefab::CSoundSource* se = NewGO<prefab::CSoundSource>(0);
		se->Init(L"sound/se_maoudamashii_system49.wav");
		se->SetVolume(0.50f);	//�{�����[���̒����B
		se->Play(false);		//�����V���b�g�Đ��B
		//�t���O�𗧂Ă�B
		m_isGameStart = true;
	}
	if (!m_isGameStart)
	{
		StageSelect();
		return;
	}
	if (m_isGameStart){
		CVector4 AddColor = CVector4::White;
		m_mulColor = m_mulColor - AddColor * 1.0f * GameTime().GetFrameDeltaTime();
		m_fontColor = m_fontColor - AddColor * 1.0f * GameTime().GetFrameDeltaTime();
	}
	m_mulColor.x = max(0.0f, m_mulColor.x);
	m_mulColor.w = 1.0f;
	//m_fontColor.w = 1.0f;
	m_spriteRender->SetMulColor(m_mulColor);
	m_fontRender->SetColor(m_fontColor);


	if (m_mulColor.x < 0.1f) {		//�H�ɐ��l���o�O��H���������킯�ł͂Ȃ��H�o�O���Ă�C������ȁ`...��������Ȃ��������B
		//�^�C�g�����폜�B
		DeleteGO(this); 
	}
}

void GameTitle::OnDestroy()
{
	DeleteGO(m_spriteRender);
	DeleteGO(m_fontRender);
}

void GameTitle::SetBestClearTime(float* time)
{
	for (int i = 0; i < select_Stage_Num; i++)
	{
		//�b�̕��������߂�B
		m_bestTime[i] = time[i];
		m_bestTime[i] = fmodf(time[i],60.0f);
		//���̕��������߂�B
		m_bestTimeMin[i] = (int)time[i] / 60;
	}
}

void GameTitle::StageSelect()
{
	//�Q�[���Z���N�g�B
	float stick_LX = Pad(0).GetLStickXF();
	if (fabsf(stick_LX) <= 0.2f) {		//�X�e�B�b�N�̌X�����������Ȃ�B
		m_stickFlag = false;
	}
	if ((stick_LX < -0.2f || 
		Pad(0).IsTrigger(enButtonLeft))		//�X�e�B�b�N�̓��͂܂��͍����́B
		&& !m_stickFlag) {
		m_selectNum--;
		m_stickFlag = true;
		//�X�e�[�W�Z���N�g�̐����B
		m_selectNum = max(select_Stage1, min((select_Stage_Num - 1),m_selectNum));
		swprintf_s(m_text,
			L"    Stage%2d\nBestTime %2d:%05.2f",
			m_selectNum + 1,
			m_bestTimeMin[m_selectNum],
			m_bestTime[m_selectNum]);
		m_fontRender->SetText(m_text);
		return;
	}
	if ((stick_LX > 0.2f || 
		Pad(0).IsTrigger(enButtonRight))	//�X�e�B�b�N�̓��͂܂��͉E���́B
		&& !m_stickFlag) {
		m_selectNum++;
		m_stickFlag = true;
		//�X�e�[�W�Z���N�g�̐����B
		m_selectNum = max(select_Stage1, min((select_Stage_Num - 1), m_selectNum));
		swprintf_s(m_text,
			L"    Stage%2d\nBestTime %2d:%05.2f",
			m_selectNum + 1,
			m_bestTimeMin[m_selectNum],
			m_bestTime[m_selectNum]);
		m_fontRender->SetText(m_text);
		return;
	}

}


