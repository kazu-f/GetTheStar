#pragma once
#include"MoveBoardBase.h"

class MoveBoardBig:public MoveBoardBase
{
public:
	MoveBoardBig();
	~MoveBoardBig();
	bool Start();
	void Update();
	void OnDestroy();

private:
	//Player* m_player = nullptr;		//プレイヤー。
	
};

