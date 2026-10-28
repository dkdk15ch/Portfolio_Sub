#pragma once
#include "mapBlockBase.h"
#include "resource.h"
#include "imgHandle.h"

class BlockSand : public BlockBase {
public:
	BlockSand(float x, float y) : BlockBase(
		BlockType::S,
		x, y,Resource(0, 0, 0),
		ImageHandles::Get(ImgName::BLOCK_SAND)
	){}
};
