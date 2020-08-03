#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

bool BackGround::Start()
{
	//���f���f�[�^�̃��[�h
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/ActionBG.cmo");
	m_skinModelRender->SetShadowReceiverFlag(true);
	//��]�̐ݒ�B
	//m_rotation.SetRotationDeg(CVector3::AxisX, 90.0f);
	//m_skinModelRender->SetRotation(m_rotation);
	
	//�ÓI�����I�u�W�F�N�g�̍쐬
	m_phyStaticObject.CreateMesh(CVector3::Zero, CQuaternion::Identity, CVector3::One, m_skinModelRender);

	return true;
}

void BackGround::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

//void BackGround::Update()
//{
//}
