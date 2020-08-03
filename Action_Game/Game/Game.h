#pragma once

//#include "tkEngine/graphics/effect/tkEffect.h"
//#include "tkEngine/physics/tkPhysicsGhostObject.h"
//#include "tkEngine/character/tkCharacterController.h"

class GameTitle;
class Stage1;
class Stage2;
class Stage3;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void OnDestroy();
	/*
	クリアタイムを設定。
	*/
	void SetClearTime(float time)
	{
		m_clearTimeRes = time;
	}

private:
	GameTitle* m_title = nullptr;		//タイトル。
	Stage1* m_stage1 = nullptr;			//ステージ１。
	Stage2* m_stage2 = nullptr;			//ステージ２。
	Stage3* m_stage3 = nullptr;			//ステージ３。

	enum {
		select_Stage1,
		select_Stage2,
		select_Stage3,
		select_Stage_Num
	};
	float m_bestTime[select_Stage_Num];		//ベストタイムの配列。
	float m_clearTimeRes = 0.0f;		//クリアタイム。
};

