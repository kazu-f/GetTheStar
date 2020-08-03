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
	Player* m_player;	//�v���C���[�B
	CVector3 toPos;		//�J�����̑��΍��W�B
	CSpringCamera m_springCamera;		//�΂˃J�����B
};

