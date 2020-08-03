#pragma once
#include "Player.h"
#include "tkEngine/camera/tkSpringcamera.h"

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void OnDestroy();

	void MoveCamera();
	void MoveFront(const CVector3& dir);
private:
	Player* m_player;	//プレイヤー。
	CVector3 toPos;		//カメラの相対座標。
	CSpringCamera m_springCamera;		//ばねカメラ。
};

