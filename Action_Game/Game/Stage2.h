#pragma once
#include "StageBase.h"

class Stage2 : public StageBase
{
public:
	Stage2();
	~Stage2();
	bool Start();
	void Update();
	void OnDestroy();	

private:
};

