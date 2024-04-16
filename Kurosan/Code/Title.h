#pragma once

#include "SceneBase.h"

class Title :public SceneBase
{
public:
	Title();
	virtual ~Title();

	int image;

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

