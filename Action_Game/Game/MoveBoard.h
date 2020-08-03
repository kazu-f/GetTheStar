#pragma once
#include "MoveBoardBase.h"
#include "Player.h"

class MoveBoard : public MoveBoardBase
{
public:
	MoveBoard();
	~MoveBoard();
	bool Start();
	void Update();
	void OnDestroy();

private:
	void Move();
	//void PushPlayer();

	//Player* m_player = nullptr;		//プレイヤー。
};

