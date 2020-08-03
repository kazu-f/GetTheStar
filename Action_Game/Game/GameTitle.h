#pragma once
class GameTitle :public IGameObject
{
public:
	//メンバ関数
	GameTitle();
	~GameTitle();
	bool Start();
	void Update();
	void OnDestroy();

	//選んだステージを取得。
	int GetSelectNum()
	{
		return m_selectNum;
	}
	/*
	ベストクリアタイムを設定する。
	*/
	void SetBestClearTime(float* time);

private:
	void StageSelect();
	//メンバ変数
	prefab::CSpriteRender* m_spriteRender = nullptr;		//タイトル画面のスプライトレンダー。
	bool m_isGameStart = false;								//インゲームフラグ。
	CVector4 m_mulColor = CVector4::White;					//スプライトのカラー。
	//セレクトステージの列挙。
	enum {
		select_Stage1,
		select_Stage2,
		select_Stage3,
		select_Stage_Num
	};
	int m_selectNum = 0;		//選んでいるステージ。
	bool m_stickFlag = false;	//スティック入力のフラグ。
	prefab::CFontRender* m_fontRender = nullptr;		//フォントレンダー。
	CVector4 m_fontColor = CVector4::Gray;
	CVector2 m_fontPos = CVector2::Zero;
	wchar_t m_text[255];		//テキスト。
	float m_bestTime[select_Stage_Num];		//ベストクリアタイムの設定。
	int m_bestTimeMin[select_Stage_Num];	//ベストクリアタイム(分)。
};

