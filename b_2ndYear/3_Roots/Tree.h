#pragma once
#include "gameObject.h"
class MapManager;

class Tree : public GameObject
{
private:
	int width;
	int height;
	int PosX, PosY;
public:
	void SetImgSize(const MapManager& map);

	virtual void Draw() const override;
	virtual void Run() override;
};

extern Tree TreeInst;

