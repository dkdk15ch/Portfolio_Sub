#include "mapInfo.h"
#include "Tree.h"

ImageHandles imgHandles;

void RoadTreeImage(const MapManager& map)
{
	std::string tree = "image/tree.png";
	imgHandles.Load(ImgName::TREE, tree);
	TreeInst.SetImgSize(map);
}

void RoadBlockImage()
{
	std::string sand = "image/blockSand.jpg";
	imgHandles.Load(ImgName::BLOCK_SAND, sand);
	std::string nutrient = "image/blockNutrient.jpg";
	imgHandles.Load(ImgName::BLOCK_NUTRIENT, nutrient);
	std::string water = "image/blockWater.jpg";
	imgHandles.Load(ImgName::BLOCK_WATER, water);
	std::string oxygen = "image/blockOxygen.jpg";
	imgHandles.Load(ImgName::BLOCK_OXYGEN, oxygen);
}
