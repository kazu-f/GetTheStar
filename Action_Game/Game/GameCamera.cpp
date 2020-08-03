#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("Player");

	//�J������ݒ�B
	//���_���璍���_�܂ł̃x�N�g���B
	toPos.Set(0.0f, 50.0f, 500.0f);
	//MainCamera().SetPosition(CVector3::Zero);
	//m_springCamera.SetPosition(CVector3::Zero);

	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(10000.0f);

	//�΂˃J�����̏������B
	m_springCamera.Init(
		MainCamera(),		//�΂˃J�����̏������s���J�������w�肷��B
		2000.0f,			//�J�����̈ړ����x�̍ő�l�B1000.0���Ɖ�]�ɊԂɍ���񂩂�����B
		true,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		5.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	return true;
}

void GameCamera::Update()
{
	if (m_player != nullptr){

		if (!m_player->IsGameClear()) {
			if (Pad(0).IsTrigger(enButtonLB1)) {
				CVector3 dir = m_player->GetPlayerFront();
				dir *= -1.0f;
				MoveFront(dir);
			}
			else { 
				MoveCamera(); 
			}

		}
		else {
			CVector3 dir = m_player->GetPlayerFront();
			MoveFront(dir);
		}

		if (m_player->IsDead()) {
			m_player = nullptr;
		}
	}

	//�J�����̍X�V�B
	m_springCamera.Update();
}

void GameCamera::OnDestroy()
{
}
/*
�J�����̈ړ��A�X�V�����B
*/
void GameCamera::MoveCamera()
{
	//�J�������X�V�B
	//�����_���v�Z����B
	CVector3 target = m_player->GetPosition();
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 50.0f;

	//�Â����W��ێ��B
	CVector3 oldPos = toPos;
	//�p�b�h�̓��͂��g���ăJ��������]�B
	float x = Pad(0).GetRStickXF();
	float y = Pad(0).GetRStickYF();
	//Y�����̉�]�B
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 3.0f * x);
	qRot.Multiply(toPos);
	//X�����̉�]�B
	CVector3 axisX;
	axisX.Cross(CVector3::AxisY, toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 3.0f * y);
	qRot.Multiply(toPos);
	//�J������]�̏���l�`�F�b�N�B
	CVector3 toPosDir = toPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//����������B
		toPos = oldPos;
	}
	if (toPosDir.y > 0.8f) {
		//�����������B
		toPos = oldPos;
	}

	//���_���v�Z����B
	//�L�����N�^�̏゠����Ƒ��΍��W�𑫂��ăJ�����̃|�W�V�������o���B
	CVector3 pos = target + toPos;


	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetTarget(target);	//�����_�@�L�����N�^�̏゠����B
	m_springCamera.SetPosition(pos);	//�J�����̍��W�B
}
/*
�v���C���[�̐��ʂփJ�����𓮂����B
*/
void GameCamera::MoveFront(const CVector3& dir)
{
	//�J�������X�V�B
	//�����_���v�Z����B
	CVector3 target = m_player->GetPosition();
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 50.0f;

	//CVector3 vDir = dir;
	//vDir.y = 0.0f;
	//vDir.Normalize();
	//CVector3 vDirXZ = vDir;		//XZ���ʏ�̃x�N�g���B
	//CVector3 vY = CVector3::AxisY;
	//vDir = vDir * 0.8f + vY * 0.2f;		//����y���グ��B
	//vDir.Normalize();
	//CVector3 toPosDirXZ = toPos;
	//toPosDirXZ.y = 0.0f;
	//toPosDirXZ.Normalize();
	//
	////�p�x�����߂Ă����B
	////XZ���ʏ�ł̊p�x�����߂�B
	//float angleXZ = toPosDirXZ.Dot(vDirXZ);		//�p�x�B
	//angleXZ = min(1.0f, max(-1.0f, angleXZ));
	//angleXZ = acosf(angleXZ);		//���ό��ʂ��p�x�ɂ���B
	////Y�����̉�]�B
	//CQuaternion qRot;
	//qRot.SetRotation(CVector3::AxisY, angleXZ);
	//qRot.Multiply(toPos);

	////�����ォ��̂������ފp�x�ɂ���B
	//CVector3 toPosDir = toPos;
	//toPosDir.Normalize();
	//float angleY = toPosDir.Dot(vDir);		//�p�x�B
	//angleY = min(1.0f, max(-1.0f, angleY));
	//angleY = acosf(angleY);
	//CVector3 axis;		//��]���B
	//axis.Cross(toPosDir, vDir);
	//axis.Normalize();
	//qRot.SetRotation(axis, angleY);	
	//qRot.Multiply(toPos);

	CVector3 vDir = dir;
	vDir.y = 0.0f;
	vDir.Normalize();
	CVector3 vY = CVector3::AxisY;
	vDir = vDir * 0.8f + vY * 0.2f;		//����y���グ��B
	vDir.Normalize();
	CVector3 toPosDir = toPos;
	toPosDir.Normalize();
	float angle = toPosDir.Dot(vDir);		//�p�x�B
	angle = min(1.0f, max(-1.0f, angle));
	angle = acosf(angle);
	CVector3 axis;		//��]���B
	axis.Cross(toPosDir, vDir);
	axis.Normalize();
	CQuaternion qRot;
	qRot.SetRotation(axis, angle);
	qRot.Multiply(toPos);

	CVector3 pos = target + toPos;

	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_springCamera.SetTarget(target);	//�����_�@�L�����N�^�̏゠����B
	m_springCamera.SetPosition(pos);	//�J�����̍��W�B
}
