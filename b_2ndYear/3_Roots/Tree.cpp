#include "Tree.h"
#include "Dxlib.h"
#include "imgHandle.h"
#include "mapBlockBase.h"
#include "mapList.h"

Tree TreeInst;

void Tree::SetImgSize(const MapManager& map)
{
	GetGraphSize(ImageHandles::Get(ImgName::TREE), &width, &height);

	int startRootIndex = 7;
	float startLeft = map.m_mapList[startRootIndex]->GetLeft();
	float startTop	= map.m_mapList[startRootIndex]->GetTop();
	int bSize = BlockBase::blockSize;
	PosX = startLeft + (bSize * 0.5) - (width * 0.5);
	PosY = startTop - height;
}

void Tree::Draw() const
{
	DrawGraphF(PosX, PosY, ImageHandles::Get(ImgName::TREE), true);
}

void Tree::Run()
{
	Draw();
}
