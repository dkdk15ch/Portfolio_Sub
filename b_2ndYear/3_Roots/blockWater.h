#pragma once
#include "mapBlockBase.h"
#include "resource.h"
#include "imgHandle.h"

class BlockWater : public BlockBase {
public:
	BlockWater(float x, float y) : BlockBase(
		BlockType::W,
		x, y, Resource(0, 10, 0),
		ImageHandles::Get(ImgName::BLOCK_WATER)
	) {}
};