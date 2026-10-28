#pragma once
#include "blockType.h"
#include <memory>

class BlockBase;


class BlockFactory {
public:
	static std::unique_ptr<BlockBase> CreateBlock(BlockType type, float x, float y);
};

