#include "stdafx.h"
#include "Stage3.h"
#include "GameCamera.h"
#include "JumpStand.h"
#include "MoveBoard.h"
#include "MoveBoardBig.h"
#include "Enemy.h"

Stage3::Stage3()
{
}

Stage3::~Stage3()
{
}

bool Stage3::Start()
{
	//���x���̍\�z
	m_level.Init(L"level/myStage_03.tkl", [&](LevelObjectData& objData) {
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
		else if (objData.EqualObjectName(L"MoveBoardBig") == true) {
			auto m_moveBoardBig = NewGO<MoveBoardBig>(0, "MoveBoardBig");
			m_moveBoardBig->SetPosition(objData.position);
			m_moveBoardBig->SetRotation(objData.rotation);
			m_moveBoardBig->SetSpeed(100.0f);
			m_moveBoardBig->SetTimeLimit(5.0f);
			return true;
		}
		else if (objData.EqualObjectName(L"MoveBoardBig2") == true) {
			auto m_moveBoardBig = NewGO<MoveBoardBig>(0, "MoveBoardBig");
			m_moveBoardBig->SetPosition(objData.position);
			m_moveBoardBig->SetRotation(objData.rotation);
			m_moveBoardBig->SetSpeed(200.0f);
			m_moveBoardBig->SetTimeLimit(2.5f);
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
		else if (objData.EqualObjectName(L"Stage03Ground") == true) {
			m_ground = NewGO<prefab::CSkinModelRender>(0, "ground");
			m_ground->Init(L"modelData/Stage03Ground.cmo");
			m_ground->SetPosition(objData.position);
			m_ground->SetShadowReceiverFlag(true);

			m_phyStaticObj.CreateMesh(objData.position, objData.rotation, m_ground->GetSkinModelData());

			return true;
		}
		return false;
	});

	//BGM�̐ݒ�B
	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->InitStreaming(L"sound/actionBGM.wav");
	m_bgm->Play(true);		//���[�v�Đ��B
	//�^�C���\���̐ݒ�B
	m_fontRender = NewGO<prefab::CFontRender>(0);
	swprintf_s(m_gameTimeTex, L"Time %02d:%05.2f", m_minTimer, m_gameTimer);		//�e�L�X�g�̐ݒ�B
	m_fontRender->SetText(m_gameTimeTex);
	CVector2 pivot = { 0.0f,1.0f };					//�s�{�b�g�̐ݒ�B
	m_fontRender->SetPivot(pivot);
	m_fontPos = { -640.0f,360.0f };					//���W�̐ݒ�B
	//m_fontPos = { -640.0f,-300.0f };					//���W�̐ݒ�B
	m_fontRender->SetPosition(m_fontPos);
	m_fontRender->SetColor(m_fontColor);			//�F�̐ݒ�B

	//�J�����B
	NewGO<GameCamera>(0, "GameCamera");

	Light();	//���C�g�B

	return true;
}

void Stage3::Update()
{
	if (!m_isGameClear && !m_isGameOver) CountTime();		//�Q�[���N���A�A�������̓Q�[���I�[�o�[����Ȃ���΃J�E���g����B
	//�Q�[���I�[�o�[�̕\���B
	if (m_player != nullptr && m_player->IsDead() && !m_isGameClear) {
		m_player = nullptr;
		m_isGameOver = true;
		//�X�v���C�g�̍쐬�B
		m_sprite = NewGO<prefab::CSpriteRender>(0);
		m_sprite->Init(L"sprite/GameOver.dds", 1280.0f, 720.0f);
	}
	if (m_isGameOver && Pad(0).IsTrigger(enButtonA)) {
		//�Q�[���V�[�����폜�B
		DeleteGO(this);
	}
	//�Q�[���N���A�̕\���B
	if (m_goalStar != nullptr && m_goalStar->IsDead() && !m_isGameOver) {
		m_goalStar = nullptr;
		m_isGameClear = true;
		//�X�v���C�g�̍쐬�B
		m_sprite = NewGO<prefab::CSpriteRender>(0);
		m_sprite->Init(L"sprite/GameClear.dds", 1280.0f, 720.0f);
		m_sprite->SetPosition({ 0.0f,150.0f,0.0f });
	}
	if (m_isGameClear && m_timer <= 1.5f) {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	else if (m_isGameClear && Pad(0).IsTrigger(enButtonA) && m_timer > 1.5f) {
		//�Q�[���V�[�����폜�B
		DeleteGO(this);
	}

	if (Pad(0).IsPress(enButtonStart)) {
		//�Q�[���V�[�����폜�B
		DeleteGO(this);
	}
}

void Stage3::OnDestroy()
{
	DeleteGOs("JumpStand");
	//DeleteGOs("MoveBoard");
	DeleteGOs("MoveBoardBig");
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

