#include "stdafx.h"
#include "StageBase.h"

float StageBase::GetClearTime()
{
	if (!m_isGameClear) {
		//�N���A���Ă��Ȃ��Ȃ�ő�l��n���B
		return FLT_MAX;
	}
	//�^�C����b�ɒ����B
	float clearTime = (float)m_minTimer * 60.0f + m_gameTimer;

	return clearTime;
}

void StageBase::Light()
{
	//�f�B���N�V�������C�g(���z���݂����Ȃ���)���V�[���ɒǉ��B
	m_lig = NewGO<prefab::CDirectionLight>(0, "GameLight");
	//���C�g�̕�����ݒ�B
	CVector3 lightDir;					//���C�g�̕����B
	lightDir.Set(1.0f, -1.0f, 0.0f);
	lightDir.Normalize();
	//lightDir.Set(0.707f, -0.707f, 0.0f);
	m_lig->SetDirection(lightDir);
	//���C�g�̐F��ݒ�B
	m_lig->SetColor({ 3.5f, 3.5f, 3.5f, 1.0f });

	shadow::DirectionShadowMap().SetLightDirection(lightDir);
	//shadow::DirectionShadowMap().SetLightDirection({ 1, -1, 1 });
}

void StageBase::CountTime()
{
	m_gameTimer += GameTime().GetFrameDeltaTime();
	if (m_gameTimer >= 60.0f) {
		m_gameTimer -= 60.0f;
		m_minTimer++;
	}
	swprintf_s(m_gameTimeTex, L"Time %02d:%05.2f", m_minTimer, m_gameTimer);		//�e�L�X�g�̐ݒ�B
	m_fontRender->SetText(m_gameTimeTex);
}
