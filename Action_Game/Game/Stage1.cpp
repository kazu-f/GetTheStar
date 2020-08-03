#include "stdafx.h"
#include "Stage1.h"
//#include "tkEngine/light/tkDirectionLight.h"
//#include <Player.h>
#include <GameCamera.h>
#include "JumpStand.h"
#include "GameTitle.h"
//#include "MoveBoard.h"
//#include "MoveBoardBig.h"
#include "Enemy.h"
//#include "GoalStar.h"

Stage1::Stage1()
{
}

Stage1::~Stage1()
{
}

bool Stage1::Start()
{

	//レベルの構築
	m_level.Init(L"level/myStage_01.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"unityChan") == true) {
			m_player = NewGO<Player>(0, "Player");
			m_player->SetPosition(objData.position);
			return true;
		}
		else if (objData.EqualObjectName(L"JumpStand") == true) {
			auto m_jumpStand = NewGO<JumpStand>(0, "JumpStand");
			m_jumpStand->SetPosition(objData.position);
			m_jumpStand->SetScale(objData.scale);
			return true;
		}
		else if (objData.EqualObjectName(L"Enemy") == true) {
			auto m_enemy = NewGO<Enemy>(0, "Enemy");
			m_enemy->SetPosition(objData.position);
			m_enemy->SetRotation(objData.rotation);
			return true;
		}
		else if (objData.EqualObjectName(L"GoalStar") == true) {
			m_goalStar = NewGO<GoalStar>(0, "GoalStar");
			m_goalStar->SetPosition(objData.position);
			m_goalStar->SetRotation(objData.rotation);
			return true;
		}
		else if (objData.EqualObjectName(L"Stage01Ground") == true) {
			//ステージの作成。
			m_ground = NewGO<prefab::CSkinModelRender>(0, "ground");
			m_ground->Init(L"modelData/Stage01Ground.cmo");
			m_ground->SetPosition(objData.position);
			m_ground->SetShadowReceiverFlag(true);
			//物理オブジェクトの作成。
			m_phyStaticObj.CreateMesh(objData.position, objData.rotation, m_ground->GetSkinModelData());

			return true;
		}
		return false;
	});

	//BGMの設定。
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->InitStreaming(L"sound/actionBGM.wav");
	m_bgm->Play(true);		//ループ再生。
	//タイム表示の設定。
	m_fontRender = NewGO<prefab::CFontRender>(0);
	swprintf_s(m_gameTimeTex, L"Time %02d:%05.2f", m_minTimer, m_gameTimer);		//テキストの設定。
	m_fontRender->SetText(m_gameTimeTex);
	CVector2 pivot = { 0.0f,1.0f };					//ピボットの設定。
	m_fontRender->SetPivot(pivot);
	m_fontPos = { -640.0f,360.0f };					//座標の設定。
	//m_fontPos = { -640.0f,-300.0f };					//座標の設定。
	m_fontRender->SetPosition(m_fontPos);
	m_fontRender->SetColor(m_fontColor);			//色の設定。

	//カメラ。
	NewGO<GameCamera>(0, "GameCamera");

	Light();	//ライト。

	return true;
}

void Stage1::Update()
{
	if (!m_isGameClear && !m_isGameOver) CountTime();		//ゲームクリア、もしくはゲームオーバーじゃなければカウントする。
	//ゲームオーバーの表示。
	if (m_player != nullptr && m_player->IsDead() && !m_isGameClear) {
		m_player = nullptr;
		m_isGameOver = true;
		//スプライトの作成。
		m_sprite = NewGO<prefab::CSpriteRender>(0);
		m_sprite->Init(L"sprite/GameOver.dds", 1280.0f, 720.0f);
	}
	if (m_isGameOver && Pad(0).IsTrigger(enButtonA)) {
		//ゲームシーンを削除。
		DeleteGO(this);
	}
	//ゲームクリアの表示。
	if (m_goalStar != nullptr && m_goalStar->IsDead() && !m_isGameOver) {
		m_goalStar = nullptr;
		m_isGameClear = true;
		//スプライトの作成。
		m_sprite = NewGO<prefab::CSpriteRender>(0);
		m_sprite->Init(L"sprite/GameClear.dds", 1280.0f, 720.0f);
		m_sprite->SetPosition({ 0.0f,150.0f,0.0f });
	}
	if (m_isGameClear && m_timer <= 1.5f) {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	else if (m_isGameClear && Pad(0).IsTrigger(enButtonA) && m_timer > 1.5f) {
		//ゲームシーンを削除。
		DeleteGO(this);
	}

	if (Pad(0).IsPress(enButtonStart)) {
		//ゲームシーンを削除。
		DeleteGO(this);
	}
}

void Stage1::OnDestroy()
{
	DeleteGOs("JumpStand");
	//DeleteGOs("MoveBoard");
	//DeleteGOs("MoveBoardBig");
	DeleteGOs("Enemy");
	if (m_player != nullptr) {
		DeleteGO("Player");
	}
	if (m_ground) {
		DeleteGO(m_ground);
	}
	DeleteGO("GameCamera");
	DeleteGO("GameLight");
	if (m_sprite != nullptr) {
		DeleteGO(m_sprite);
	}
	if (m_goalStar != nullptr) {
		DeleteGO(m_goalStar);
	}
	if (m_bgm) {
		DeleteGO(m_bgm);
	}
	if (m_fontRender) {
		DeleteGO(m_fontRender);
	}
}

//float Stage1::GetClearTime()
//
//{
//	if (!m_isGameClear) {
//		//クリアしていないなら最大値を渡す。
//		return FLT_MAX;
//	}
//	//タイムを秒に直す。
//	float clearTime = (float)m_minTimer * 60.0f + m_gameTimer;
//
//	return clearTime;
//}
//
//void Stage1::Light()
//{
//	//ディレクションライト(太陽光みたいなもの)をシーンに追加。
//	m_lig = NewGO<prefab::CDirectionLight>(0, "GameLight");
//	//ライトの方向を設定。
//	CVector3 lightDir;					//ライトの方向。
//	lightDir.Set(1.0f,-1.0f,0.0f);
//	lightDir.Normalize();
//	//lightDir.Set(0.707f, -0.707f, 0.0f);
//	m_lig->SetDirection(lightDir);
//	//ライトの色を設定。
//	m_lig->SetColor({ 3.5f, 3.5f, 3.5f, 1.0f });
//
//	shadow::DirectionShadowMap().SetLightDirection(lightDir);
//	//shadow::DirectionShadowMap().SetLightDirection({ 1, -1, 1 });
//}
//
//void Stage1::CountTime()
//{
//	m_gameTimer += GameTime().GetFrameDeltaTime();
//	if (m_gameTimer >= 60.0f) {
//		m_gameTimer -= 60.0f;
//		m_minTimer++;
//	}
//	swprintf_s(m_gameTimeTex, L"Time %02d:%05.2f", m_minTimer, m_gameTimer);		//テキストの設定。
//	m_fontRender->SetText(m_gameTimeTex);
//}
