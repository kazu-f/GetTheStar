#pragma once
class GameTitle :public IGameObject
{
public:
	//�����o�֐�
	GameTitle();
	~GameTitle();
	bool Start();
	void Update();
	void OnDestroy();

	//�I�񂾃X�e�[�W���擾�B
	int GetSelectNum()
	{
		return m_selectNum;
	}
	/*
	�x�X�g�N���A�^�C����ݒ肷��B
	*/
	void SetBestClearTime(float* time);

private:
	void StageSelect();
	//�����o�ϐ�
	prefab::CSpriteRender* m_spriteRender = nullptr;		//�^�C�g����ʂ̃X�v���C�g�����_�[�B
	bool m_isGameStart = false;								//�C���Q�[���t���O�B
	CVector4 m_mulColor = CVector4::White;					//�X�v���C�g�̃J���[�B
	//�Z���N�g�X�e�[�W�̗񋓁B
	enum {
		select_Stage1,
		select_Stage2,
		select_Stage3,
		select_Stage_Num
	};
	int m_selectNum = 0;		//�I��ł���X�e�[�W�B
	bool m_stickFlag = false;	//�X�e�B�b�N���͂̃t���O�B
	prefab::CFontRender* m_fontRender = nullptr;		//�t�H���g�����_�[�B
	CVector4 m_fontColor = CVector4::Gray;
	CVector2 m_fontPos = CVector2::Zero;
	wchar_t m_text[255];		//�e�L�X�g�B
	float m_bestTime[select_Stage_Num];		//�x�X�g�N���A�^�C���̐ݒ�B
	int m_bestTimeMin[select_Stage_Num];	//�x�X�g�N���A�^�C��(��)�B
};

