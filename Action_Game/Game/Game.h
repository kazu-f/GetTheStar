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
	�N���A�^�C����ݒ�B
	*/
	void SetClearTime(float time)
	{
		m_clearTimeRes = time;
	}

private:
	GameTitle* m_title = nullptr;		//�^�C�g���B
	Stage1* m_stage1 = nullptr;			//�X�e�[�W�P�B
	Stage2* m_stage2 = nullptr;			//�X�e�[�W�Q�B
	Stage3* m_stage3 = nullptr;			//�X�e�[�W�R�B

	enum {
		select_Stage1,
		select_Stage2,
		select_Stage3,
		select_Stage_Num
	};
	float m_bestTime[select_Stage_Num];		//�x�X�g�^�C���̔z��B
	float m_clearTimeRes = 0.0f;		//�N���A�^�C���B
};

