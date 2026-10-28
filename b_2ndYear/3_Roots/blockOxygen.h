#pragma once
#include "mapBlockBase.h"
#include "resource.h"
#include "imgHandle.h"

class BlockOxygen : public BlockBase {
public:
	BlockOxygen(float x, float y) : BlockBase(
		BlockType::O,
		x, y, Resource(0, 0, 10),
		ImageHandles::Get(ImgName::BLOCK_OXYGEN)
	) {}
};
