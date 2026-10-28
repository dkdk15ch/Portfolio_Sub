#include <cassert>
#include "blockFactory.h"
#include "mapBlockBase.h"
//	サブクラス
#include "blockSand.h"
#include "blockNutrient.h"
#include "blockWater.h"
#include "blockOxygen.h"

std::unique_ptr<BlockBase> BlockFactory::CreateBlock(BlockType type, float x, float y) {
	switch (type)
	{
	case BlockType::S:
		return std::make_unique<BlockSand>(x, y);
	case BlockType::N:
		return std::make_unique<BlockNutrient>(x, y);
	case BlockType::W:
		return std::make_unique<BlockWater>(x, y);
	case BlockType::O:
		return std::make_unique<BlockOxygen>(x, y);
	default:
		assert(false && "BlockFactory : 未登録のBlockType");
		return nullptr;
	}

}