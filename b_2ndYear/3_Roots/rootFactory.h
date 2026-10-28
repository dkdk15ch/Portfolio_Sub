#pragma once
#include "rootType.h"
#include "point.h"
#include <memory>
#include "box.h"

class IRoot;
class MapManager;

class RootFactory{
public:
	static std::shared_ptr<IRoot> Create(const MapManager& mapmgr ,RootType type,BoxRects start , BoxRects end, std::shared_ptr<IRoot> prev);
};

