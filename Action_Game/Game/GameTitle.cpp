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
	//スプライトレンダーの設定。
	m_spriteRender = NewGO<prefab::CSpriteRender>(0);
	m_spriteRender->Init(L"sprite/gameTitle.dds", 1280.0f, 720.0f);
	//フォントレンダーの設定。
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
	CVector2 fontPivot = { 0.5f,0.5f };		//ピボットを中心に設定。
	m_fontRender->SetPivot(fontPivot);

	return true;
}

void GameTitle::Update()
{
	if (Pad(0).IsTrigger(enButtonA)) {
		//効果音を鳴らす。
		prefab::CSoundSource* se = NewGO<prefab::CSoundSource>(0);
		se->Init(L"sound/se_maoudamashii_system49.wav");
		se->SetVolume(0.50f);	//ボリュームの調整。
		se->Play(false);		//ワンショット再生。
		//フラグを立てる。
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


	if (m_mulColor.x < 0.1f) {		//稀に数値がバグる？そういうわけではない？バグってる気がするな～...ここじゃないかった。
		//タイトルを削除。
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
		//秒の部分を求める。
		m_bestTime[i] = time[i];
		m_bestTime[i] = fmodf(time[i],60.0f);
		//分の部分を求める。
		m_bestTimeMin[i] = (int)time[i] / 60;
	}
}

void GameTitle::StageSelect()
{
	//ゲームセレクト。
	float stick_LX = Pad(0).GetLStickXF();
	if (fabsf(stick_LX) <= 0.2f) {		//スティックの傾きが小さいなら。
		m_stickFlag = false;
	}
	if ((stick_LX < -0.2f || 
		Pad(0).IsTrigger(enButtonLeft))		//スティックの入力または左入力。
		&& !m_stickFlag) {
		m_selectNum--;
		m_stickFlag = true;
		//ステージセレクトの制限。
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
		Pad(0).IsTrigger(enButtonRight))	//スティックの入力または右入力。
		&& !m_stickFlag) {
		m_selectNum++;
		m_stickFlag = true;
		//ステージセレクトの制限。
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


