#include "stdafx.h"
#include "StageBase.h"

float StageBase::GetClearTime()
{
	if (!m_isGameClear) {
		//クリアしていないなら最大値を渡す。
		return FLT_MAX;
	}
	//タイムを秒に直す。
	float clearTime = (float)m_minTimer * 60.0f + m_gameTimer;

	return clearTime;
}

void StageBase::Light()
{
	//ディレクションライト(太陽光みたいなもの)をシーンに追加。
	m_lig = NewGO<prefab::CDirectionLight>(0, "GameLight");
	//ライトの方向を設定。
	CVector3 lightDir;					//ライトの方向。
	lightDir.Set(1.0f, -1.0f, 0.0f);
	lightDir.Normalize();
	//lightDir.Set(0.707f, -0.707f, 0.0f);
	m_lig->SetDirection(lightDir);
	//ライトの色を設定。
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
	swprintf_s(m_gameTimeTex, L"Time %02d:%05.2f", m_minTimer, m_gameTimer);		//テキストの設定。
	m_fontRender->SetText(m_gameTimeTex);
}
