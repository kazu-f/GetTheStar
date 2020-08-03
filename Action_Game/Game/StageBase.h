#pragma once
#include "tkEngine/light/tkDirectionLight.h"
#include "Player.h"
#include "GoalStar.h"

class StageBase : public IGameObject
{
public:
	/*
	クリアタイムを取得する。
	*/
	float GetClearTime();

protected:
	/*
	ライトの設定。
	*/
	void Light();
	/*
	タイムをカウントする。
	*/
	void CountTime();

	//////////
	prefab::CDirectionLight* m_lig = nullptr;			//ライト。
	//CVector3 m_lightDir = CVector3::One;					//ライトの方向。
	Player* m_player = nullptr;				//プレイヤー。
	GoalStar* m_goalStar = nullptr;		//ゴール。
	CLevel m_level;							//レベル。
	prefab::CSkinModelRender* m_ground = nullptr;	//ステージのモデル。
	CPhysicsStaticObject m_phyStaticObj;			//ステージの物理オブジェクト。
	prefab::CSpriteRender* m_sprite = nullptr;		//スプライト。
	prefab::CSoundSource* m_bgm = nullptr;		//BGM。
	float m_timer = 0.0f;		//タイマー。
	prefab::CFontRender* m_fontRender = nullptr;		//タイムを表示する。
	CVector4 m_fontColor = CVector4::White;			//テキストの色。
	CVector2 m_fontPos = CVector2::Zero;			//テキストの座標。
	wchar_t m_gameTimeTex[64];		//タイムのテキスト。
	float m_gameTimer = 0.0f;		//クリアタイム。
	int m_minTimer = 0;				//クリアタイム。（分）

	bool m_isGameOver = false;
	bool m_isGameClear = false;
};

