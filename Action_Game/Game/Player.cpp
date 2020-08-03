#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	//�A�j���[�V�����N���b�v�̓ǂݍ��݁B
	m_animClips[enAnimationClip_idle].Load(L"animData/unityChan/idle.tka");
	m_animClips[enAnimationClip_run].Load(L"animData/unityChan/run.tka");
	m_animClips[enAnimationClip_jump].Load(L"animData/unityChan/jump.tka");
	m_animClips[enAnimationClip_damage].Load(L"animData/unityChan/damage.tka");
	m_animClips[enAnimationClip_down].Load(L"animData/unityChan/KneelDown.tka");
	m_animClips[enAnimationClip_clear].Load(L"animData/unityChan/clear.tka");

	m_animClips[enAnimationClip_idle].SetLoopFlag(false);
	m_animClips[enAnimationClip_run].SetLoopFlag(true);
	m_animClips[enAnimationClip_jump].SetLoopFlag(false);
	m_animClips[enAnimationClip_damage].SetLoopFlag(false);
	m_animClips[enAnimationClip_down].SetLoopFlag(false);
	m_animClips[enAnimationClip_clear].SetLoopFlag(false);

	//�v���C���[�̃��f���f�[�^�ǂݍ��݁B
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/unityChan.cmo", m_animClips, enAnimationClip_Num, enFbxUpAxisY);
	m_skinModelRender->SetShadowCasterFlag(true);

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->PlayAnimation(enAnimationClip_idle);

	//�L�����R���̐ݒ�B
	m_charaCon.Init(
		20.0f,		//��
		80.0f,		//����
		m_position	//���W
	);

	//HP��UI�̐ݒ�B
	CVector2 hp_pivot = { 1.0f,1.0f };		//HP�p�̃s�{�b�g�B
	CVector3 hpPos = { 640.0f, 360.0f, 0.0f };	//HP�̍��W�B
	//HP�o�[�B
	m_hpBarSprite = NewGO<prefab::CSpriteRender>(0);
	m_hpBarSprite->Init(L"sprite/Player_HP_Bar.dds", 480.0f, 48.0f);
	m_hpBarSprite->SetScale(CVector3::One);
	m_hpBarSprite->SetPivot(hp_pivot);
	m_hpBarSprite->SetPosition({ 640.0f,360.0f,0.0f });
	//HP�B
	m_hpSprite = NewGO<prefab::CSpriteRender>(0);
	m_hpSprite->Init(L"sprite/Player_HP.dds", 440.0f, 28.0f);
	m_hpSprite->SetScale(m_hpScale);
	m_hpSprite->SetPivot(hp_pivot);
	m_hpSprite->SetPosition({ 620.0f,350.0f,0.0f });

	return true;
}

void Player::Update()
{	
	if (!isClear) {
		hitStop -= GameTime().GetFrameDeltaTime();
		if (m_HP > 0 && hitStop < 0.0f) {
			Move_Jump();//�ړ������ȂǁB
			Turn();

			if (m_charaCon.IsOnGround()
				&& (fabsf(m_moveSpeed.x) < 0.001f
					&& fabsf(m_moveSpeed.z) < 0.001f)) {
				//�n��œ������Ȃ����ߑҋ@���[�V�����B
				m_skinModelRender->PlayAnimation(enAnimationClip_idle);
			}
			else if (m_charaCon.IsOnGround()) {
				//�n��ňړ����Ă��邽�߈ړ����[�V�����B
				m_skinModelRender->PlayAnimation(enAnimationClip_run);
			}
			else
			{
				//����ȊO�Ȃ�W�����v���[�V�����B
				m_skinModelRender->PlayAnimation(enAnimationClip_jump);
			}

		}
		else if (m_HP > 0 && hitStop > 0.0f) {
			//�m�b�N�o�b�N�̌����B
			m_moveSpeed.x -= m_moveSpeed.x * 5.0f * GameTime().GetFrameDeltaTime();
			m_moveSpeed.z -= m_moveSpeed.z * 5.0f * GameTime().GetFrameDeltaTime();
			//���������B
			m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
			//�������x�̏���B
			m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);
		}
		if (m_HP <= 0 && !isDeath) {
			//�A�j���[�V�����̍Đ��B
			m_skinModelRender->SetUpdateAnimationFlag(false);
			m_skinModelRender->PlayAnimation(enAnimationClip_down);
			isDeath = true;		//���S�t���O�𗧂Ă�B
		}
		else if (!m_skinModelRender->IsPlayingAnimation() && isDeath) {
			DeleteGO(this);		//���S�A�j���[�V�������I������玩�g���폜�B
		}

		//�L�����R�����g�p���č��W���X�V�B
		m_position = m_charaCon.Execute(m_moveSpeed);

		if (m_position.y < -2000.0f) {
			DeleteGO(this);		//�������玀�S�B
		}
	}
	//else if (isClear && m_skinModelRender->IsPlayingAnimation()) {

	//}
	//�X�L�����f�������_�[�ɍ��W�Ɖ�]�𔽉f�B
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
}

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_hpBarSprite);
	DeleteGO(m_hpSprite);
}

void Player::Damage(CVector3& moveSpeed)
{
	if (hitStop > 0.0f) {
		return;
	}
	m_HP--;
	hitStop = 0.8f;
	//�X�v���C�g�̃X�P�[�������炷�B
	m_hpScale.x -= 1.0f / 3.0f;
	m_hpSprite->SetScale(m_hpScale);
	if (m_HP > 0) {
		//�m�b�N�o�b�N�B
		CVector3 damege = moveSpeed;
		damege.y += 50.0f;
		SetMoveSpeed(damege);
		//�A�j���[�V�����B
		m_skinModelRender->SetUpdateAnimationFlag(false);
		m_skinModelRender->PlayAnimation(enAnimationClip_damage);
	}
	else {
		SetMoveSpeed(CVector3::Zero);
	}
}

void Player::Move_Jump()
{
	//���X�e�B�b�N���͂��󂯎��B
	float x = Pad(0).GetLStickXF();
	float y = Pad(0).GetLStickYF();
	//�J�����̑O�����ƉE�������擾�B
	CVector3 cameraForward = MainCamera().GetForward();	//�J�����̑O�����B
	CVector3 cameraRight = MainCamera().GetRight();	//�J�����̉E�����B
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	////XZ�����̈ړ����x���N���A�B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed += cameraForward * y * m_speed;	//�������ւ̈ړ����x�����Z�B
	m_moveSpeed += cameraRight * x * m_speed;		//�E�����ւ̈ړ����x�����Z�B

	if (isnan(m_moveSpeed.Length())) {	//�񐔉���B�J�����̂ق��Ŏ������炢���񂾂���...
		m_moveSpeed = CVector3::Zero;
	}

	//�W�����v�̏����B
	const float jumpSpeed = 640.0f;		//�W�����v�X�s�[�h�B
	const float jumpAccele = 30.0f;		//�W�����v�̉����x�B
	const float minJumpSpeed = 500.0f;	//�W�����v�̍Œᑬ�x�B
	//�n�ʂɂ���Ƃ��̂݃W�����v���\�B
	if (Pad(0).IsTrigger(enButtonA)
		&& m_charaCon.IsOnGround()) {
		isJump = true;		//A�{�^���������ꂽ��W�����v�t���O�𗧂Ă�B
		m_moveSpeed.y = minJumpSpeed;	//�����x�B
	}
	//�W�����v�t���O�����������̏����B
	if (Pad(0).IsPress(enButtonA)
		&& isJump && jumpTimer < 0.2f) {
		/*m_moveSpeed.y += jumpSpeed / 0.1f * GameTime().GetFrameDeltaTime();*/		//�����������B
		if (jumpTimer > 0.08f) {
			m_moveSpeed.y = max(minJumpSpeed, min(m_moveSpeed.y += jumpAccele, jumpSpeed));	//��ԃ}�V�H
			//m_moveSpeed.y = jumpSpeed * (1.0f - jumpTimer * 2.0f);	//����̓_�������B
			//m_moveSpeed.y = jumpSpeed / 2.0f + jumpSpeed * jumpTimer * 2.0f;	//��a������΂��B
			//m_moveSpeed.y = jumpSpeed;	//���������ō����o�ɂ����B
		}
		jumpTimer += GameTime().GetFrameDeltaTime();
	}
	else {
		//A�{�^���������ꂽ��W�����v�t���O�������B
		isJump = false;
		jumpTimer = 0.0f;
	}

	//�d�͏����B
	m_moveSpeed.y -= gravitation * GameTime().GetFrameDeltaTime();
	//�������x�̏���B
	m_moveSpeed.y = max(maxDropSpeed, m_moveSpeed.y);


	return;
}

void Player::Turn()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f)
	{
		//�������Ȃ��Ƃ��ɂ͏������s��Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotation(CVector3::AxisY, angle);
}
