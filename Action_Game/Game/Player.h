#pragma once
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void OnDestroy();
	/*
	���W���擾�B
	return ���W�B
	*/
	const CVector3& GetPosition() const
	{
		return m_position;
	};	
	/*
	�v���C���[�̑O�������擾�B
	*/
	const CVector3& GetPlayerFront() const
	{
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(m_rotation);
		CVector3 m_front;
		m_front.Set(mRot.m[2][0], mRot.m[2][1], mRot.m[2][2]);
		m_front.Normalize();

		return m_front;
	};
	/*
	�ړ����x���擾�B
	return �ړ����x�B
	*/
	const CVector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/*
	�L�����R�����擾�B
	return �L�����R���B
	*/
	CCharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
	/*
	���W��ݒ�B
	*/
	void SetPosition(const CVector3& position)
	{
		m_position = position;
		m_charaCon.SetPosition(m_position);
	};
	/*
	�ړ����x��ݒ�B
	*/
	void SetMoveSpeed(const CVector3& moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}
	/*
	�n�ʂɂ��邩�ǂ����B
	*/
	bool IsPlayerOnGround() const
	{
		return m_charaCon.IsOnGround();
	};
	/*
	�Q�[���N���A����B
	*/
	bool IsGameClear() const
	{
		return isClear;
	}
	/*
	�v���C���[���L�����R���œ������B
	����	�ړ����x�B
	*/
	void PlayerMove(CVector3& moveSpeed)
	{
		m_position = m_charaCon.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
		m_skinModelRender->SetPosition(m_position);
	};
	/*
	�v���C���[�Ƀ_���[�W��^����B
	*/
	void Damage(CVector3& moveSpeed);
	/*
	�N���A���̃A�j���[�V������������B
	*/
	void GameClear()
	{
		m_skinModelRender->SetUpdateAnimationFlag(false);
		m_skinModelRender->PlayAnimation(enAnimationClip_clear);
		isClear = true;
	}
	
private:
	void Move_Jump();
	void Turn();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_���[�B
	CVector3 m_position;	//���W�B
	CVector3 m_moveSpeed = CVector3::Zero;	//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity;	//��]�B
	CCharacterController m_charaCon;	//�L�����R���B
	float jumpTimer = 0.0f;				//�W�����v�̃^�C�}�[�B
	bool isJump = false;				//�W�����v�t���O�B
	const float m_speed = 600.0f;		//�v���C���[�̑��x�B
	const float gravitation = 980.0f;	//�d�́B
	const float maxDropSpeed = -2560.0f;//�ő嗎�����x�B
	prefab::CSpriteRender* m_hpBarSprite = nullptr;	//HP�o�[�̃X�v���C�g�B
	prefab::CSpriteRender* m_hpSprite = nullptr;	//HP�̃X�v���C�g�B
	CVector3 m_hpScale = CVector3::One;				//HP�̃X�P�[���B
	int m_HP = 3;				//�v���C���[��HP�B
	float hitStop = 0.0f;		//�_���[�W���󂯂���̃q�b�g�X�g�b�v�Ɩ��G���ԁB
	bool isDeath = false;		//���S�t���O�B
	bool isClear = false;		//�N���A�t���O�B

	enum EnAnimationClip {
		enAnimationClip_idle,	//�ҋ@�A�j���[�V�����B
		enAnimationClip_run,	//����A�j���[�V�����B
		enAnimationClip_jump,	//�W�����v�A�j���[�V�����B
		enAnimationClip_damage,	//�_���[�W���󂯂��Ƃ��̃A�j���[�V����?�B
		enAnimationClip_down,	//�Q�[���I�[�o�[�̃A�j���[�V�����B
		enAnimationClip_clear,	//�Q�[���N���A�̃A�j���[�V�����B
		enAnimationClip_Num,	//�A�j���[�V�����N���b�v�̐��B
	};
	CAnimationClip m_animClips[enAnimationClip_Num];
};

