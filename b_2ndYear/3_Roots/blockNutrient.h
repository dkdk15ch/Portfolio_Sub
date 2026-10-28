#pragma once
#include "mapBlockBase.h"
#include "resource.h"
#include "imgHandle.h"

class MutaPoint;

class BlockNutrient : public BlockBase {
public:
	BlockNutrient(float x, float y) : BlockBase(
		BlockType::N,
		x, y, Resource(10, 1, 1),
		ImageHandles::Get(ImgName::BLOCK_NUTRIENT)
	) {}
};
